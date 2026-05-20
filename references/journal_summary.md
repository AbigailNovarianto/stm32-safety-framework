# Tabel Ringkasan 15 Jurnal — Studi Literatur

> **Instruksi**: Isi tabel di bawah untuk setiap jurnal yang kamu temukan.
> - Sumber: Scopus / Web of Science
> - Tahun: 2021–2026
> - Minimal 2–3 jurnal per topik (6 topik × ~2.5 = 15)

## Topik yang Harus Tercakup

1. Teknologi & perkembangan kontroler
2. Dasar mikrokontroler
3. Arsitektur mikrokontroler
4. Periferal (timer, ADC, komunikasi serial, dll.)
5. Embedded programming (C/C++ & Rust)
6. Safety-critical sistem tertanam

---

## Tabel Ringkasan Jurnal

| No | Judul | Penulis | Tahun | Nama Jurnal | DOI | Topik (#) | Metode | Hasil Utama | Future Work |
|----|-------|---------|-------|-------------|-----|-----------|--------|-------------|-------------|
| 1  | Comparison of Embedded System Design for Industrial Applications | — | 2011 | IEEE Transactions on Industrial Informatics | 10.1109/TII.2011.2124466 | #1 | Survey & komparasi platform embedded (MCU, FPGA, DSP, ASIC) | Memetakan kelebihan/kekurangan tiap platform embedded untuk aplikasi industri | FW1: Eksplorasi platform hybrid (FPGA+MCU) untuk kontrol industri real-time dengan kebutuhan safety tinggi |
| 2  | Energy-Efficient Dual-Core RISC-V Architecture for Edge AI | Cristian Andy Tanase | 2026 | Computers (MDPI) | 10.3390/computers15040219 | #1 | Dual-core RISC-V dengan MAC sharing, DFS, dan FIFO-based arbitration | Efisiensi energi meningkat signifikan untuk inferensi AI di edge dengan resource terbatas | FW2: Replikasi MAC unit dengan arbitrasi hierarkis dan dedikasi MAC unit khusus NPU untuk throughput AI terjamin |
| 3  | Hardware-Accelerated Digital Power Control for High-Frequency Hybrid Energy Storage | Wenhao Lin, Guanying Chu | 2025 | IET Power Electronics | 10.1049/pel2.70043 | #1 | Implementasi type-II compensator pada STM32G474RB via FMAC untuk kontrol 250 kHz | Kontroler digital frekuensi tinggi pada MCU general-purpose menggantikan DSP dedicated | FW3: Eksplorasi peningkatan waktu sampling ADC dan metodologi alternatif untuk mengurangi delay sampling pada kontrol frekuensi sangat tinggi |
| 4  | Quantization and Deployment of Deep Neural Networks on Embedded Targets | Pierre-Emmanuel Novac et al. | 2021 | Sensors (MDPI) | 10.3390/s21092984 | #1 | Survey metode kuantisasi DNN (post-training, QAT) untuk deployment di embedded MCU | DNN dapat di-deploy di embedded target dengan optimasi kuantisasi; trade-off akurasi vs efisiensi | FW4: Pemahaman lebih dalam tentang batas kemampuan berbagai kasus penggunaan embedded AI dan optimasi memory footprint DNN |
| 5  | RTOS-Integrated Time Synchronization for Self-Deployable Wireless Sensor Networks | Sarah Goossens et al. | 2026 | Sensors (MDPI) | 10.3390/s26072121 | #2 | Integrasi sinkronisasi waktu UTC berbasis GNSS ke dalam RTOS untuk WSN | Sinkronisasi jaringan WSN yang presisi tanpa overhead komunikasi dedicated | FW5: Adaptive synchronization frequency untuk reduksi energi di node; integrasi timestamp ke dalam data packet regular untuk eliminasi dedicated sync thread |
| 6  | Ultralight Situation-Aware Hypervisor With Adaptive Starvation-Free Feedback Control | — | 2025 | IEEE Access | 10.1109/ACCESS.2025.3594619 | #2 | Kombinasi round-robin dan priority scheduling dengan feedback control adaptif pada hypervisor MCU | Hypervisor ultraringan mampu mengelola multi-OS pada embedded system dengan overhead O(N) | FW6: Integrasi deadline-based scheduling dengan priority-based approach untuk memenuhi hard real-time constraint pada sistem kompleks |
| 7  | uTango: An Open-Source TEE for IoT Devices | — | 2022 | IEEE Access | 10.1109/ACCESS.2022.3152781 | #2 | TEE open-source berbasis TrustZone-M pada Cortex-M MCU untuk isolasi aplikasi IoT kritis | Implementasi TEE yang efisien di MCU IoT berbiaya rendah tanpa dependensi eksternal | FW7: Demonstrasi layanan secure (secure storage, kriptografi, remote attestation, OTA update) dalam dedicated NSVW |
| 8  | Design and Implementation of an Atrial Fibrillation Detection Algorithm on the ARM Cortex-M4 | Marek Zylinski, Amir Nassibi, Danilo P. Mandic | 2023 | Sensors (MDPI) | 10.3390/s23177521 | #3 | Klasifier ML untuk deteksi AF pada ARM Cortex-M4; end-to-end (akuisisi → DSP → ML) | Cortex-M4 mampu menjalankan pipeline ML lengkap untuk deteksi aritmia pada perangkat wearable | FW8: Ekspansi ke lebih banyak tipe aritmia, optimasi model untuk latency lebih rendah, dan validasi pada dataset klinis yang lebih beragam |
| 9  | LLVM-Based Efficient Hybrid Cache and TCM Memory Allocation for Low-Latency | — | 2025 | IEEE Embedded Systems Letters | 10.1109/LES.2025.3625376 | #3 | Alokasi memori hybrid (Cache + TCM) berbasis LLVM compiler pass untuk Cortex-M7 | Latensi akses data berkurang signifikan tanpa modifikasi hardware; optimasi oleh compiler | FW9: Perluasan ke multi-core MCU, pertimbangan access pattern dinamis, dan integrasi dengan OS-level memory manager |
| 10 | An Ultra-Low-Power Time-Domain Level-Crossing ADC With Adaptive Sampling Rate | — | 2025 | IEEE Transactions on Circuits and Systems II | 10.1109/TCSII.2025.3592482 | #4 | TD-LC ADC dengan teknik sampling non-uniform berbasis level-crossing; VTC + DTC | Konsumsi daya ADC berkurang drastis dengan adaptive sampling rate tanpa kehilangan akurasi sinyal | FW10: Implementasi pada proses CMOS yang lebih canggih dan evaluasi pada sinyal biomedis dengan bandwidth lebih tinggi |
| 11 | Front-End Gateway System With Serial Communication Protocol Conversion and Edge Computing | — | 2023 | IEEE Access | 10.1109/ACCESS.2023.3307631 | #4 | Konversi protokol UART-to-I2C pada gateway dengan edge computing untuk integrasi data sensor | Waktu pemrosesan data berkurang dan jumlah bus komunikasi diminimalkan pada sistem multi-sensor | FW11: Penambahan protokol komunikasi lain (SPI, CAN), skalabilitas untuk jumlah sensor lebih besar, dan security pada lapisan protokol |
| 12 | A C-Based Framework for Low-Cost Real-Time Embedded Systems | Ivan Cibrario Bertolotti | 2025 | Future Internet (MDPI) | 10.3390/fi17060269 | #5 | Framework C/C++ yang mengabstraksikan manajemen jaringan multi-teknologi untuk IoT firmware | Framework mengurangi kompleksitas programming embedded; overhead worst-case <6% untuk Modbus-CAN | FW12: Penambahan dukungan protokol jaringan baru, integrasi mekanisme keamanan, dan adaptasi untuk RTOS lain selain yang diuji |
| 13 | Energy Efficient Mixed Task Handling on Real-Time Embedded Systems Using FreeRTOS | Deepak Ramegowda, Man Lin | 2022 | Journal of Systems Architecture | 10.1016/j.sysarc.2022.102708 | #5 | CCMT-DVFS (Dynamic Voltage Frequency Scaling) untuk mixed task set pada FreeRTOS | Konsumsi energi berkurang pada embedded system dengan beban task campuran (periodic + aperiodic) | FW13: Ekstensi CCMT Model ke multi-processor dan integrasi karakteristik task application-specific ke dalam kernel FreeRTOS |
| 14 | Implementation of a Universal Framework Using SOLID Design Patterns for Microcontrollers | Marek Babiuch, Petr Foltynek | 2024 | Sensors (MDPI) | 10.3390/s24103116 | #5 | Penerapan prinsip SOLID dan design pattern (Factory, Observer, Strategy) pada firmware MCU IoT | Framework modular meningkatkan maintainability dan reusability kode firmware embedded | FW14: Evaluasi framework pada MCU dengan resource lebih terbatas, pengujian pada sistem IoT skala besar, dan integrasi dengan CI/CD pipeline |
| 15 | Embedded Control Barrier Functions for Safety-Critical Control of High-Relative-Degree Systems | — | 2025 | IEEE Transactions on Industrial Informatics | 10.1109/TII.2025.3584464 | #6 | EMB-CBF-SCC: framework kontrol safety-critical berbasis Control Barrier Function untuk sistem nonlinear orde tinggi | Sistem mampu menjaga safety constraint multipel pada robot manipulator dengan trajektori yang telah didesain | FW15: Perluasan ke sistem dengan ketidakpastian model yang lebih besar dan implementasi hardware real-time pada MCU industrial |
| 16 | Security and Functional Safety for AI in Embedded Automotive Systems: A Tutorial | — | 2024 | IEEE Transactions on Circuits and Systems II | 10.1109/TCSII.2023.3334273 | #6 | Survey & tutorial: integrasi keamanan (adversarial attack defense) dan functional safety (ISO 26262) untuk AI automotive | Peta jalan komprehensif untuk rekayasa keamanan + safety sistem AI embedded otomotif | FW16: Pengembangan standar keamanan AI baru untuk siklus hidup produk panjang, dan metode verifikasi formal untuk model ML di lingkungan safety-critical |

---

## Daftar Future Work (FW1–FW16)

Gabungan seluruh future work dari 16 jurnal — akan menjadi dasar **metode baru**:

```
FW1:  [Jurnal #1 — Topik #1]  — Eksplorasi platform hybrid (FPGA+MCU) untuk kontrol industri real-time
FW2:  [Jurnal #2 — Topik #1]  — Arsitektur multi-MAC dengan arbitrasi hierarkis dan dedicated NPU MAC untuk AI throughput
FW3:  [Jurnal #3 — Topik #1]  — Peningkatan ADC sampling timing dan alternatif metodologi untuk mitigasi sampling delay frekuensi tinggi
FW4:  [Jurnal #4 — Topik #1]  — Optimasi memory footprint DNN untuk berbagai use case embedded AI
FW5:  [Jurnal #5 — Topik #2]  — Adaptive sync frequency untuk energi efisien; integrasi timestamp ke paket data reguler
FW6:  [Jurnal #6 — Topik #2]  — Integrasi deadline-based scheduling dengan priority scheduling untuk hard real-time
FW7:  [Jurnal #7 — Topik #2]  — Implementasi secure services (kriptografi, storage, OTA update) dalam dedicated NSVW
FW8:  [Jurnal #8 — Topik #3]  — Ekspansi deteksi multi-aritmia, optimasi latensi model ML, validasi dataset klinis lebih luas
FW9:  [Jurnal #9 — Topik #3]  — Perluasan hybrid cache+TCM ke multi-core MCU dengan access pattern dinamis
FW10: [Jurnal #10 — Topik #4] — Implementasi TD-LC ADC pada proses CMOS lebih maju untuk sinyal biomedis bandwidth tinggi
FW11: [Jurnal #11 — Topik #4] — Dukungan multi-protokol serial (SPI, CAN) dan security pada lapisan gateway
FW12: [Jurnal #12 — Topik #5] — Dukungan protokol jaringan baru, mekanisme keamanan, dan adaptasi multi-RTOS
FW13: [Jurnal #13 — Topik #5] — Ekstensi CCMT-DVFS ke multi-processor dan integrasi ke kernel FreeRTOS
FW14: [Jurnal #14 — Topik #5] — Evaluasi SOLID framework pada MCU resource-constrained dan integrasi CI/CD
FW15: [Jurnal #15 — Topik #6] — Perluasan CBF ke sistem dengan model uncertainty tinggi dan implementasi real-time MCU
FW16: [Jurnal #16 — Topik #6] — Standar keamanan AI baru dan verifikasi formal model ML untuk safety-critical embedded
```

---

## Catatan Pencarian

| Topik | Keyword yang Digunakan | Database | Jumlah Hasil | Jurnal Terpilih |
|-------|------------------------|----------|--------------|-----------------| 
| #1 | embedded system design, RISC-V edge AI, digital power control STM32, DNN deployment MCU | Scopus / IEEE Xplore / MDPI | — | 4 |
| #2 | RTOS time synchronization WSN, hypervisor microcontroller, TEE IoT TrustZone | Scopus / IEEE Xplore / MDPI | — | 3 |
| #3 | ARM Cortex-M4 implementation, LLVM cache TCM embedded | Scopus / IEEE Xplore / MDPI | — | 2 |
| #4 | ultra-low-power ADC, serial protocol gateway edge computing | Scopus / IEEE Xplore | — | 2 |
| #5 | C framework real-time embedded, FreeRTOS DVFS energy, SOLID design pattern microcontroller | Scopus / IEEE Xplore / MDPI | — | 3 |
| #6 | control barrier function safety-critical, AI safety automotive embedded | Scopus / IEEE Xplore | — | 2 |
