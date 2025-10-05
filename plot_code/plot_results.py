import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


# cpp Config

number_of_frames = 50  # must match C++ code
radian_change_per_frame = 0.1
frames_per_second = 10


# Load CSV
df_field = pd.read_csv("potential_field.csv")
df_field.loc[df_field["phi"] == -1e6, "phi"] = -1e-6
frames = sorted(df_field["frame"].unique())

df_acc_0 = pd.read_csv("accelerometer_0.csv")  # your accelerometer file
df_acc_1 = pd.read_csv("accelerometer_1.csv")  # your accelerometer file

# Rename readings to x / y
df_x = df_acc_0.rename(columns={"reading": "x"})[["frame", "x"]]
df_y = df_acc_1.rename(columns={"reading": "y"})[["frame", "y"]]

# Align on frame (outer join in case frames don't perfectly match)
df_xy = pd.merge(df_x, df_y, on="frame", how="outer").sort_values("frame").reset_index(drop=True)

df_xy["magnitude"] = np.sqrt(df_xy["x"]**2 + df_xy["y"]**2)

# direction (radians): atan2(y, x); add degrees if handy
df_xy["direction_rad"] = np.arctan2(df_xy["y"], df_xy["x"])

frame15 = df_field[df_field["frame"] == 15]
print(frame15["phi"].min(), frame15["phi"].max())
print(frame15[frame15["phi"] > 0])  # see if any positive sneaked in

# Set up figure: left = field, middle = 2 stacked accelerometer plots, right = 3 stacked (magnitude, inv magnitude, direction)
fig = plt.figure(figsize=(18, 8))
gs = fig.add_gridspec(
    nrows=3, ncols=3,
    width_ratios=[1.2, 1.0, 1.2],   # tweak as you like
    height_ratios=[1, 1, 1],
    wspace=0.35, hspace=0.25
)

ax_field = fig.add_subplot(gs[:, 0])   # all rows, col 0
ax_acc0 = fig.add_subplot(gs[0, 1])
ax_acc1 = fig.add_subplot(gs[1, 1], sharex=ax_acc0)
ax_magnitude      = fig.add_subplot(gs[0, 2], sharex=ax_acc0)
ax_inv_magnitude  = fig.add_subplot(gs[1, 2], sharex=ax_acc0)
ax_direction      = fig.add_subplot(gs[2, 2], sharex=ax_acc0)


# Left: potential field plot
ax_field.set_aspect("equal")
contour = None
mass_dot, = ax_field.plot([], [], "ro")
accel_dot, = ax_field.plot([], [], "go")

# Middle top: accelerometer 0
ax_acc0.plot(df_acc_0["frame"], df_acc_0["reading"], label="Accelerometer 0")
time_marker0 = ax_acc0.axvline(x=0, color="r", linestyle="--", label="Current frame")
ax_acc0.set_ylabel("Reading 0")
ax_acc0.legend()

# Middle bottom: accelerometer 1
ax_acc1.plot(df_acc_1["frame"], df_acc_1["reading"], label="Accelerometer 1")
time_marker1 = ax_acc1.axvline(x=0, color="r", linestyle="--")
ax_acc1.set_xlabel("Frame")
ax_acc1.set_ylabel("Reading 1")
ax_acc1.legend()

# Right top: magnitude
ax_magnitude.plot(df_xy["frame"], df_xy["magnitude"], label="Accelerometer Magnitude")
time_marker2 = ax_magnitude.axvline(x=0, color="r", linestyle="--", label="Current frame")
ax_magnitude.set_ylabel("Magnitude")
ax_magnitude.legend()

# Right middle: inverse magnitude
ax_inv_magnitude.plot(df_xy["frame"], 1/df_xy["magnitude"], label="Accelerometer Magnitude")
time_marker3 = ax_inv_magnitude.axvline(x=0, color="r", linestyle="--", label="Current frame")
ax_inv_magnitude.set_ylabel("Magnitude")
ax_inv_magnitude.legend()

# Right bottom: direction (radians)
ax_direction.plot(df_xy["frame"], df_xy["direction_rad"], label="Accelerometer Direction (rad)")
time_marker4 = ax_direction.axvline(x=0, color="r", linestyle="--")
ax_direction.set_xlabel("Frame")
ax_direction.set_ylabel("Direction (rad)")
ax_direction.legend()

def update(frame_idx):
    global contour
    frame = frames[frame_idx]
    subset = df_field[df_field["frame"] == frame].copy()
    # Remove old contour
    if contour:
        for coll in contour.collections:
            coll.remove()

    subset['phi'] = - subset['phi']  # Invert phi values for better visualization
    # # Plot potential field
    # contour = ax_field.tricontourf(
    #     subset["x"], subset["y"], subset["phi"],
    #     levels=30, cmap="viridis"
    # )
    # convert to potential values
    phi_min = subset["phi"].min()
    phi_max = subset["phi"].max()

    phi_levels = np.logspace(np.log10(phi_min), np.log10(phi_max), 30)
    print(f"Frame {frame}: phi_min={phi_min}, phi_max={phi_max}, levels={phi_levels}")
    contour = ax_field.tricontourf(
        subset["x"], subset["y"], subset["phi"],
        levels=phi_levels,
        cmap="viridis",
    )
    # Move the mass
    # mass_x = 2 * np.cos(radian_change_per_frame * frame)
    # mass_y = 2 * np.sin(radian_change_per_frame * frame)
    # mass_dot.set_data(mass_x, mass_y)
    accel_dot.set_data(1.0, 1.0)  # Fixed position for accelerometer
    ax_field.set_title(f"Frame {frame}")

    # Update vertical time markers
    time_marker0.set_xdata(frame)
    time_marker1.set_xdata(frame)

    time_marker2.set_xdata(frame)
    time_marker3.set_xdata(frame)
    time_marker4.set_xdata(frame)

    return contour, mass_dot, time_marker0, time_marker1, time_marker2, time_marker3, time_marker4

ani = FuncAnimation(fig, update, frames=len(frames), interval=1000/frames_per_second, blit=False)
plt.tight_layout()
plt.show()