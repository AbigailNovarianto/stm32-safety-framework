"""
generate_plots.py
Generate grafik sensor+PWM dan grafik FSM dari simulation_data.csv
Output: docs/Picture/grafik_sensor_pwm.png dan grafik_state_fsm.png
"""

import csv
import os
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

# ── Path ───────────────────────────────────────────────────────────
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
CSV_PATH   = os.path.join(SCRIPT_DIR, '..', 'data', 'simulation_data.csv')
OUT_DIR    = os.path.join(SCRIPT_DIR, 'Picture')
os.makedirs(OUT_DIR, exist_ok=True)

# ── Baca CSV ───────────────────────────────────────────────────────
t, s1, s2, pwm, state = [], [], [], [], []
with open(CSV_PATH, newline='') as f:
    reader = csv.DictReader(f)
    for row in reader:
        if not row['timestamp_ms']:
            continue
        t.append(int(row['timestamp_ms']))
        s1.append(int(row['s1_mv']))
        s2.append(int(row['s2_mv']))
        pwm.append(int(row['pwm_pct']))
        state.append(int(row['state']))

t_s = [v / 1000.0 for v in t]          # konversi ke detik

# Titik transisi keadaan
t_warn  = next((t_s[i] for i, s in enumerate(state) if s >= 1), None)
t_fault = next((t_s[i] for i, s in enumerate(state) if s == 2), None)

# ══════════════════════════════════════════════════════════════════
# Grafik 1 — Sensor 1, Sensor 2, dan PWM vs Waktu
# ══════════════════════════════════════════════════════════════════
fig, ax1 = plt.subplots(figsize=(12, 5))

ax1.set_xlabel('Waktu (detik)', fontsize=12)
ax1.set_ylabel('Tegangan Sensor (mV)', fontsize=12, color='tab:blue')
l1, = ax1.plot(t_s, s1, color='tab:blue',  linewidth=1.8, label='Sensor 1 (mV)')
l2, = ax1.plot(t_s, s2, color='tab:cyan',  linewidth=1.8, linestyle='--', label='Sensor 2 (mV)')
ax1.axhline(200,  color='red',    linestyle=':', linewidth=1.2, alpha=0.7, label='Batas bawah (200 mV)')
ax1.axhline(2800, color='orange', linestyle=':', linewidth=1.2, alpha=0.7, label='Batas atas (2800 mV)')
ax1.set_ylim(0, 3500)
ax1.tick_params(axis='y', labelcolor='tab:blue')

ax2 = ax1.twinx()
ax2.set_ylabel('Duty Cycle PWM (%)', fontsize=12, color='tab:green')
l3, = ax2.plot(t_s, pwm, color='tab:green', linewidth=2.2, label='PWM Duty (%)')
ax2.set_ylim(0, 110)
ax2.tick_params(axis='y', labelcolor='tab:green')

# Tandai WARNING dan FAULT
if t_warn:
    ax1.axvline(t_warn,  color='goldenrod', linestyle='--', linewidth=1.5, label=f'WARNING @ {t_warn:.1f}s')
if t_fault:
    ax1.axvline(t_fault, color='red',       linestyle='-',  linewidth=1.8, label=f'FAULT @ {t_fault:.1f}s')

lines = [l1, l2, l3]
labels = [l.get_label() for l in lines]
patches = [
    mpatches.Patch(color='red',    linestyle=':', label='Batas bawah (200 mV)', fill=False, linewidth=1.2),
    mpatches.Patch(color='orange', linestyle=':', label='Batas atas (2800 mV)', fill=False, linewidth=1.2),
]
ax1.legend(handles=lines + patches, loc='upper left', fontsize=9, ncol=2)

plt.title('Respons Sensor 1, Sensor 2, dan PWM Duty Cycle terhadap Waktu', fontsize=13, fontweight='bold')
plt.tight_layout()
plt.savefig(os.path.join(OUT_DIR, 'grafik_sensor_pwm.png'), dpi=150)
plt.close()
print('[OK] grafik_sensor_pwm.png')

# ══════════════════════════════════════════════════════════════════
# Grafik 2 — State FSM vs Waktu
# ══════════════════════════════════════════════════════════════════
fig, ax = plt.subplots(figsize=(12, 3.5))

colors = {0: 'green', 1: 'goldenrod', 2: 'red'}
segment_color = [colors[s] for s in state]

for i in range(len(t_s) - 1):
    ax.fill_betweenx([0, 1], t_s[i], t_s[i+1], color=segment_color[i], alpha=0.7)

ax.plot(t_s, state, color='black', linewidth=2)
ax.set_yticks([0, 1, 2])
ax.set_yticklabels(['NORMAL', 'WARNING', 'FAULT'], fontsize=11)
ax.set_xlabel('Waktu (detik)', fontsize=12)
ax.set_ylabel('Keadaan Sistem', fontsize=12)
ax.set_ylim(-0.3, 2.5)

if t_warn:
    ax.axvline(t_warn,  color='goldenrod', linestyle='--', linewidth=1.5, label=f'WARNING @ {t_warn:.1f}s')
if t_fault:
    ax.axvline(t_fault, color='red',       linestyle='-',  linewidth=1.8, label=f'FAULT @ {t_fault:.1f}s')

p_normal  = mpatches.Patch(color='green',     alpha=0.7, label='NORMAL (0)')
p_warning = mpatches.Patch(color='goldenrod', alpha=0.7, label='WARNING (1)')
p_fault   = mpatches.Patch(color='red',       alpha=0.7, label='FAULT (2)')
ax.legend(handles=[p_normal, p_warning, p_fault], loc='upper left', fontsize=9)

plt.title('Transisi Mesin Keadaan Keamanan (FSM): NORMAL → WARNING → FAULT', fontsize=13, fontweight='bold')
plt.tight_layout()
plt.savefig(os.path.join(OUT_DIR, 'grafik_state_fsm.png'), dpi=150)
plt.close()
print('[OK] grafik_state_fsm.png')

print('\nSelesai! Cek folder docs/Picture/')
