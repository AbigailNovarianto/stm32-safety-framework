# Cara Compile Laporan LaTeX (MiKTeX / pdflatex)

## Persyaratan
- MiKTeX atau TeX Live (paket: `babel-indonesian`, `graphicx`, `booktabs`, `longtable`, `listings`, `IEEEtran` bib style)
- GNUPlot (opsional, untuk regenerasi grafik)

## Langkah compile (dari folder `docs/`)

```powershell
cd docs
pdflatex report.tex
bibtex report
pdflatex report.tex
pdflatex report.tex
```

Output: **`report.pdf`**

## Regenerasi grafik GNUPlot

```powershell
cd docs/plots
gnuplot generate_plots.gp
```

Grafik disimpan di `docs/plots/grafik_sensor_pwm.png` dan `grafik_state_fsm.png`.

## Struktur file laporan
| File | Fungsi |
|------|--------|
| `report.tex` | Laporan utama |
| `references.bib` | 16 referensi jurnal |
| `Picture/*.png` | Screenshot simulasi Proteus |
| `plots/*.png` | Grafik dari `data/simulation_data.csv` |
