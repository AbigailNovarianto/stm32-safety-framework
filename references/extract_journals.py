"""
extract_journals.py
====================
Mengekstrak informasi penting dari 16 PDF jurnal:
  - Judul (dari metadata / nama file)
  - Penulis & Tahun (dari metadata PDF)
  - Paragraf yang mengandung "future work / future research / conclusion"
  - Output: journal_extracted.md (siap paste ke tabel)

Dependensi: pdfplumber (pip install pdfplumber)
"""
import io
sys_stdout_backup = None

import os
import re
import sys
from pathlib import Path

# Fix Windows cp1252 terminal encoding
if sys.stdout.encoding.lower() != 'utf-8':
    sys.stdout = open(sys.stdout.fileno(), mode='w', encoding='utf-8', buffering=1)

try:
    import pdfplumber
except ImportError:
    print("[ERROR] pdfplumber belum terinstall. Jalankan: pip install pdfplumber")
    sys.exit(1)

# ── Konfigurasi ─────────────────────────────────────────────────────────────
BASE_DIR = Path(__file__).parent / "jurnal_pdfs"
OUTPUT_FILE = Path(__file__).parent / "journal_extracted.md"

TOPIC_MAP: dict[str, str] = {
    "1-Teknologi_Perkembangan_Kontroler": "#1",
    "2-Dasar_Mikrokontroler": "#2",
    "3-Arsitektur_Mikrokontroler": "#3",
    "4-Periferal_ADC_Timer_Serial": "#4",
    "5-Embedded_Programming": "#5",
    "6-Safety_Critical_System": "#6",
}

# Keyword untuk mendeteksi seksi Future Work
FW_KEYWORDS = [
    r"future\s+work",
    r"future\s+research",
    r"future\s+direction",
    r"further\s+work",
    r"further\s+research",
    r"future\s+studi",
    r"open\s+issue",
    r"limitation",
]

FW_PATTERN = re.compile("|".join(FW_KEYWORDS), re.IGNORECASE)

# Keyword untuk mendeteksi abstract
ABS_KEYWORDS = [r"\babstract\b"]
ABS_PATTERN = re.compile("|".join(ABS_KEYWORDS), re.IGNORECASE)


def clean_text(text: str) -> str:
    """Bersihkan teks: hapus newline berlebih, spasi ganda."""
    text = re.sub(r"\n{3,}", "\n\n", text)
    text = re.sub(r"[ \t]+", " ", text)
    return text.strip()


def extract_future_work(full_text: str, window: int = 800) -> str:
    """
    Cari seksi Future Work dalam teks.
    Ambil 'window' karakter setelah keyword ditemukan.
    """
    match = FW_PATTERN.search(full_text)
    if match:
        start = match.start()
        snippet = full_text[start : start + window]
        # Potong di akhir kalimat
        last_period = snippet.rfind(".")
        if last_period > 200:
            snippet = snippet[: last_period + 1]
        return clean_text(snippet)
    return "(Future Work section not found — cek manual)"


def extract_abstract(full_text: str, window: int = 600) -> str:
    """Cari seksi Abstract dan ambil isinya."""
    match = ABS_PATTERN.search(full_text)
    if match:
        start = match.end()
        snippet = full_text[start : start + window]
        last_period = snippet.rfind(".")
        if last_period > 100:
            snippet = snippet[: last_period + 1]
        return clean_text(snippet)
    # Fallback: 600 char pertama
    return clean_text(full_text[:600])


def extract_metadata(pdf) -> tuple[str, str]:
    """
    Ekstrak penulis & tahun dari metadata PDF.
    Return: (authors_str, year_str)
    """
    meta = pdf.metadata or {}
    author = meta.get("Author", "") or meta.get("author", "") or ""
    # Bersihkan karakter aneh
    author = re.sub(r"[^\x20-\x7E]", "", author).strip()
    author = author[:80] if author else "—"

    # Cari tahun dari metadata atau teks
    year = ""
    creation = meta.get("CreationDate", "") or ""
    m = re.search(r"(20[2-9]\d)", creation)
    if m:
        year = m.group(1)
    if not year:
        mod = meta.get("ModDate", "") or ""
        m = re.search(r"(20[2-9]\d)", mod)
        if m:
            year = m.group(1)
    return author, year


def process_pdf(pdf_path: Path, topic: str, index: int) -> dict:
    """Proses satu PDF dan kembalikan dict berisi info jurnal."""
    result = {
        "no": index,
        "judul": pdf_path.stem.replace("_", " ").replace("-", " "),
        "penulis": "—",
        "tahun": "—",
        "nama_jurnal": "—",
        "doi": "—",
        "topik": topic,
        "abstract": "(tidak ditemukan)",
        "future_work": "(tidak ditemukan)",
        "file": pdf_path.name,
    }

    try:
        with pdfplumber.open(pdf_path) as pdf:
            # Metadata
            author, year = extract_metadata(pdf)
            result["penulis"] = author
            result["tahun"] = year

            # Ambil teks dari halaman 1–5 (cukup untuk abstract & metadata)
            pages_text = ""
            for i, page in enumerate(pdf.pages):
                if i >= 5:
                    break
                t = page.extract_text()
                if t:
                    pages_text += t + "\n"

            # Ambil semua teks untuk future work (dari halaman terakhir)
            full_text = pages_text
            total_pages = len(pdf.pages)
            # Baca dari halaman ke-5 ke belakang untuk FW
            fw_text = ""
            start_page = max(0, total_pages - 5)
            for page in pdf.pages[start_page:]:
                t = page.extract_text()
                if t:
                    fw_text += t + "\n"

            result["abstract"] = extract_abstract(pages_text)
            result["future_work"] = extract_future_work(fw_text) or extract_future_work(full_text)

            # Cari DOI
            doi_match = re.search(r"10\.\d{4,9}/[^\s\"'<>]+", pages_text)
            if doi_match:
                result["doi"] = doi_match.group(0).rstrip(".,;)")

            # Cari nama jurnal dari metadata
            meta = pdf.metadata or {}
            subject = meta.get("Subject", "") or ""
            if subject:
                result["nama_jurnal"] = subject[:80]

    except Exception as e:
        result["error"] = str(e)
        print(f"  [WARN] Error pada {pdf_path.name}: {e}")

    return result


def main():
    print("=" * 60)
    print("  Journal PDF Extractor — ETS Bagian B")
    print("=" * 60)

    if not BASE_DIR.exists():
        print(f"[ERROR] Direktori tidak ditemukan: {BASE_DIR}")
        sys.exit(1)

    journals: list[dict] = []
    global_index = 1

    for folder_name, topic_id in TOPIC_MAP.items():
        folder_path = BASE_DIR / folder_name
        if not folder_path.exists():
            print(f"[SKIP] Folder tidak ada: {folder_path}")
            continue

        pdf_files = sorted(folder_path.glob("*.pdf"))
        print(f"\n[Topik {topic_id}] {folder_name} — {len(pdf_files)} jurnal")

        for pdf_path in pdf_files:
            print(f"  -> [{global_index:02d}] {pdf_path.name[:60]}")
            info = process_pdf(pdf_path, topic_id, global_index)
            journals.append(info)
            global_index += 1

    # ── Tulis output ke Markdown ────────────────────────────────────────────
    with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
        f.write("# Hasil Ekstraksi Otomatis — 16 Jurnal ETS Bagian B\n\n")
        f.write("> **Catatan**: Data diekstrak otomatis dari PDF. ")
        f.write("Periksa dan lengkapi kolom yang masih kosong (—) secara manual.\n\n")
        f.write("---\n\n")

        # ── Tabel Ringkasan ──────────────────────────────────────────────────
        f.write("## Tabel Ringkasan Jurnal\n\n")
        f.write("| No | Judul | Penulis | Tahun | Nama Jurnal | DOI | Topik |\n")
        f.write("|----|-------|---------|-------|-------------|-----|-------|\n")
        for j in journals:
            judul = j["judul"][:60].replace("|", "/")
            penulis = j["penulis"][:30].replace("|", "/")
            f.write(
                f"| {j['no']} | {judul} | {penulis} | {j['tahun']} "
                f"| {j['nama_jurnal'][:25]} | {j['doi'][:20]} | {j['topik']} |\n"
            )

        f.write("\n---\n\n")

        # ── Detail per Jurnal ────────────────────────────────────────────────
        f.write("## Detail per Jurnal\n\n")
        for j in journals:
            f.write(f"### [{j['no']:02d}] {j['judul']}\n\n")
            f.write(f"- **File**: `{j['file']}`\n")
            f.write(f"- **Topik**: {j['topik']}\n")
            f.write(f"- **Penulis**: {j['penulis']}\n")
            f.write(f"- **Tahun**: {j['tahun']}\n")
            f.write(f"- **DOI**: `{j['doi']}`\n\n")
            f.write(f"**Abstract/Intro:**\n```\n{j['abstract']}\n```\n\n")
            f.write(f"**Future Work:**\n```\n{j['future_work']}\n```\n\n")
            f.write("---\n\n")

    print(f"\n{'=' * 60}")
    print(f"  SELESAI — {len(journals)} jurnal diproses")
    print(f"  Output: {OUTPUT_FILE}")
    print("=" * 60)


if __name__ == "__main__":
    main()
