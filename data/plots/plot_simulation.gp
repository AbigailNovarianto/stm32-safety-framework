# ============================================================
# plot_simulation.gp
# GNUPlot script — Adaptive Safety-Critical Multi-Sensor Framework
# Visualisasi data hasil simulasi STM32F407VGTx
#
# Jalankan: gnuplot plot_simulation.gp
# ============================================================

set datafile separator ","

# ── Output PNG ───────────────────────────────────────────────
set terminal pngcairo size 1200,900 enhanced font 'Arial,11' linewidth 1.5
set encoding utf8

# ── Gaya warna ───────────────────────────────────────────────
set style line 1 lc rgb '#2196F3' lt 1 lw 2 pt 7 ps 0.4   # Blue  — Sensor 1
set style line 2 lc rgb '#4CAF50' lt 1 lw 2 pt 5 ps 0.4   # Green — Sensor 2
set style line 3 lc rgb '#FF9800' lt 1 lw 2 pt 9 ps 0.4   # Orange— PWM
set style line 4 lc rgb '#F44336' lt 2 lw 1.5              # Red   — batas atas
set style line 5 lc rgb '#9C27B0' lt 2 lw 1.5              # Purple— batas bawah
set style line 6 lc rgb '#F44336' lt 1 lw 1.5 pt 13 ps 0.6 # Red  — State
set style line 7 lc rgb '#CCCCCC' lt 0 lw 0.5              # Grid

# ============================================================
# PLOT 1: Tegangan Sensor 1 & 2 vs Waktu
# ============================================================
set output 'plot1_sensor_voltage.png'

set multiplot layout 3,1 title \
    "Adaptive Safety-Critical Multi-Sensor System — STM32F407VGTx\n" \
    font 'Arial Bold,13'

# ── Subplot 1: Tegangan sensor ──────────────────────────────
set title "Tegangan Sensor (Moving Average Filtered)" font 'Arial,11'
set xlabel ""
set ylabel "Tegangan (mV)" font 'Arial,10'
set xrange [0:18500]
set yrange [0:3500]
set grid ls 7
set key top left font 'Arial,9'

# Garis batas safety
set arrow 1 from 0,2800 to 18500,2800 nohead ls 4 dashtype 2
set arrow 2 from 0,200  to 18500,200  nohead ls 5 dashtype 2
set label 1 "Batas Atas (2800 mV)" at 1000,2870 font 'Arial,8' tc rgb '#F44336'
set label 2 "Batas Bawah (200 mV)"  at 1000,140  font 'Arial,8' tc rgb '#9C27B0'

plot 'simulation_data.csv' using 1:2 with lines ls 1 title "Sensor 1 (PA0)", \
     'simulation_data.csv' using 1:3 with lines ls 2 title "Sensor 2 (PA1)"

unset arrow 1; unset arrow 2
unset label 1; unset label 2

# ── Subplot 2: PWM Duty Cycle ───────────────────────────────
set title "PWM Duty Cycle Output (TIM1 CH1)" font 'Arial,11'
set ylabel "Duty Cycle (%)" font 'Arial,10'
set yrange [0:110]
set key top left font 'Arial,9'

plot 'simulation_data.csv' using 1:4 with lines ls 3 title "PWM Duty (%)"

# ── Subplot 3: System State ─────────────────────────────────
set title "System State (0=Normal, 1=Warning, 2=Fault)" font 'Arial,11'
set xlabel "Waktu (ms)" font 'Arial,10'
set ylabel "State" font 'Arial,10'
set yrange [-0.5:2.5]
set ytics ("NORMAL" 0, "WARNING" 1, "FAULT" 2)
set key top left font 'Arial,9'

# Background untuk state WARNING
set object 1 rect from 17400,-0.5 to 17800,2.5 fc rgb '#FFF9C4' fs solid 0.3 noborder
# Background untuk state FAULT
set object 2 rect from 17800,-0.5 to 18500,2.5 fc rgb '#FFCDD2' fs solid 0.3 noborder

set label 3 "WARN" at 17500,2.1 font 'Arial,8' tc rgb '#FF9800'
set label 4 "FAULT" at 17900,2.1 font 'Arial,8' tc rgb '#F44336'

plot 'simulation_data.csv' using 1:5 with steps ls 6 title "System State", \
     'simulation_data.csv' using 1:5 with points ls 6 notitle

unset multiplot

# ============================================================
# PLOT 2: Scatter plot S1 vs S2
# ============================================================
set output 'plot2_sensor_scatter.png'
set terminal pngcairo size 700,600 enhanced font 'Arial,11' linewidth 1.5

unset object 1; unset object 2
unset label 3; unset label 4
unset multiplot

set title "Scatter Plot: Sensor 1 vs Sensor 2\n(warna = state sistem)" font 'Arial Bold,12'
set xlabel "Sensor 1 (mV)" font 'Arial,10'
set ylabel "Sensor 2 (mV)" font 'Arial,10'
set xrange [0:3500]
set yrange [0:3500]
set grid ls 7
set key top left font 'Arial,9'

# Batas safety sebagai kotak
set object 3 rect from 200,200 to 2800,2800 \
    fc rgb '#E8F5E9' fs solid 0.2 behind
set label 5 "SAFE ZONE" at 400,2600 font 'Arial,9' tc rgb '#2E7D32'

plot 'simulation_data.csv' using ($5==0?$2:1/0):($5==0?$3:1/0) \
         with points pt 7 ps 0.7 lc rgb '#2196F3' title "Normal", \
     'simulation_data.csv' using ($5==1?$2:1/0):($5==1?$3:1/0) \
         with points pt 7 ps 0.9 lc rgb '#FF9800' title "Warning", \
     'simulation_data.csv' using ($5==2?$2:1/0):($5==2?$3:1/0) \
         with points pt 7 ps 1.1 lc rgb '#F44336' title "Fault"

# ============================================================
# SELESAI
# ============================================================
# Jalankan: gnuplot plot_simulation.gp
# Output  : data/plots/plot1_sensor_voltage.png
#           data/plots/plot2_sensor_scatter.png
