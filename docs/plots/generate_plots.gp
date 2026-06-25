# Generate plots for ETS report — run from docs/plots/
set terminal pngcairo size 1200,700 enhanced font "Arial,12"
set datafile separator ","
set grid lc rgb "#DDDDDD" lt 1 lw 0.5
set key top right box

# Plot 1: Sensor readings and PWM
set output "grafik_sensor_pwm.png"
set title "Respon Sistem: Sensor Analog dan PWM Aktuator" font ",14"
set xlabel "Waktu (ms)"
set ylabel "Tegangan (mV) / PWM (%)"
set y2label "PWM (%)"
set y2tics
set style line 1 lc rgb "#0060AD" lw 2
set style line 2 lc rgb "#DD181F" lw 2
set style line 3 lc rgb "#00A651" lw 2
plot "../../data/simulation_data.csv" using 1:2 with lines ls 1 title "Sensor 1 (mV)", \
     "../../data/simulation_data.csv" using 1:3 with lines ls 2 title "Sensor 2 (mV)", \
     "../../data/simulation_data.csv" using 1:4 axes x1y2 with lines ls 3 title "PWM (%)"

# Plot 2: Safety state
reset
set terminal pngcairo size 1200,700 enhanced font "Arial,12"
set datafile separator ","
set grid lc rgb "#DDDDDD" lt 1 lw 0.5
set key top right box
set output "grafik_state_fsm.png"
set title "Transisi State Machine Keamanan (NORMAL-WARNING-FAULT)" font ",14"
set xlabel "Waktu (ms)"
set ylabel "State (0=NORMAL, 1=WARNING, 2=FAULT)"
set yrange [-0.2:2.2]
set ytics ("NORMAL" 0, "WARNING" 1, "FAULT" 2)
set style line 4 lc rgb "#FF6600" lw 3
plot "../../data/simulation_data.csv" using 1:5 with steps ls 4 title "System State"
