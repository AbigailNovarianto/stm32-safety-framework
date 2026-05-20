# ============================================================
# GNUPlot Template — Hasil Simulasi ETS Pemrograman Kontroller
# ============================================================
# Cara pakai:
#   1. Ganti 'data.csv' dengan nama file data hasil simulasi
#   2. Sesuaikan kolom (using 1:2, 1:3, dst.)
#   3. Jalankan: gnuplot plot_template.gp
# ============================================================

# --- Output Settings ---
set terminal pngcairo size 1200,800 enhanced font 'Arial,14'
set output 'hasil_simulasi.png'

# --- Title & Labels ---
set title 'Simulation Results — [Your Method Title]' font 'Arial Bold,16'
set xlabel 'Time (ms)' font 'Arial,13'
set ylabel 'ADC Value (V)' font 'Arial,13'

# --- Grid & Style ---
set grid lc rgb '#DDDDDD' lt 1 lw 0.5
set style line 1 lc rgb '#0060AD' lt 1 lw 2 pt 7 ps 0.8
set style line 2 lc rgb '#DD181F' lt 1 lw 2 pt 5 ps 0.8
set style line 3 lc rgb '#00A651' lt 1 lw 2 pt 9 ps 0.8

# --- Legend ---
set key top right box lc rgb '#888888'

# --- Data separator (untuk CSV) ---
set datafile separator ','

# --- Plot ---
# TODO: Ganti 'data.csv' dan sesuaikan kolom
# Kolom 1 = waktu, kolom 2 = sensor 1, kolom 3 = sensor 2, dst.
plot 'data.csv' using 1:2 with linespoints ls 1 title 'Sensor 1 (ADC Ch0)', \
     'data.csv' using 1:3 with linespoints ls 2 title 'Sensor 2 (ADC Ch1)', \
     'data.csv' using 1:4 with linespoints ls 3 title 'PWM Output'

# ============================================================
# Plot kedua (opsional): untuk perbandingan metode
# ============================================================
# set output 'perbandingan_metode.png'
# set title 'Comparison: Proposed vs Existing Method'
# set xlabel 'Sample Number'
# set ylabel 'Response Time (us)'
# plot 'comparison.csv' using 1:2 with linespoints ls 1 title 'Proposed Method', \
#      'comparison.csv' using 1:3 with linespoints ls 2 title 'Existing Method'
