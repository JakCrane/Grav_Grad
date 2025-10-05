import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from plot_field import setup_field_plot, update_field
from plot_accelerometer import setup_accelerometer_plots
from plot_derived_data import setup_derived_plots

# Config
radian_change_per_frame = 0.1
frames_per_second = 10

# Load CSVs
df_field = pd.read_csv("potential_field.csv")
df_field.loc[df_field["phi"] == -1e6, "phi"] = -1e-6
frames = sorted(df_field["frame"].unique())

df_acc_0 = pd.read_csv("accelerometer_0.csv")
df_acc_1 = pd.read_csv("accelerometer_1.csv")

df_x = df_acc_0.rename(columns={"reading": "x"})[["frame", "x"]]
df_y = df_acc_1.rename(columns={"reading": "y"})[["frame", "y"]]
df_xy = pd.merge(df_x, df_y, on="frame", how="outer").sort_values("frame").reset_index(drop=True)
df_xy["magnitude"] = np.sqrt(df_xy["x"]**2 + df_xy["y"]**2)
df_xy["direction_rad"] = np.arctan2(df_xy["y"], df_xy["x"])

# Set up figure
fig = plt.figure(figsize=(18, 8))
gs = fig.add_gridspec(
    nrows=3, ncols=3,
    width_ratios=[1.2, 1.0, 1.2],
    height_ratios=[1, 1, 1],
    wspace=0.35, hspace=0.25
)

# Set up plots
ax_field, contour, mass_dot, accel_dot = setup_field_plot(fig, gs)
ax_acc0, ax_acc1, time_marker0, time_marker1 = setup_accelerometer_plots(fig, gs, df_acc_0, df_acc_1)
ax_magnitude, ax_inv_magnitude, ax_direction, time_marker2, time_marker3, time_marker4 = setup_derived_plots(fig, gs, df_xy)

def update(frame_idx):
    global contour, mass_dot, accel_dot
    frame = frames[frame_idx]
    subset = df_field[df_field["frame"] == frame].copy()

    # field
    contour, mass_dot, accel_dot = update_field(ax_field, subset, frame, contour, mass_dot, accel_dot, radian_change_per_frame)

    # update vertical markers
    time_marker0.set_xdata(frame)
    time_marker1.set_xdata(frame)
    time_marker2.set_xdata(frame)
    time_marker3.set_xdata(frame)
    time_marker4.set_xdata(frame)

    return contour, mass_dot, accel_dot, time_marker0, time_marker1, time_marker2, time_marker3, time_marker4

ani = FuncAnimation(fig, update, frames=len(frames), interval=1000/frames_per_second, blit=False)
plt.tight_layout()
plt.show()
