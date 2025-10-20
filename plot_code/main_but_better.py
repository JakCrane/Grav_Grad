import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from plot_field import setup_field_plot, update_field

# Config
frames_per_second = 10
# Load CSVs
df_field = pd.read_csv("potential_field.csv")
df_field['phi'] = np.sqrt(np.square(df_field['g_x']) + np.square(df_field['g_y']))

df_mass = pd.read_csv("square_mass_field.csv")
df_mass = df_mass[df_mass['mass'] > 0]
frames = sorted(df_field["frame"].unique())

# Set up figure
fig = plt.figure(figsize=(18, 9))
gs = fig.add_gridspec(
    nrows=1, ncols=2,
    width_ratios=[1.0, 1.0],
    height_ratios=[1],
    wspace=0.25, hspace=0.25
)

# Set up plots
ax_field_0, contour_0 = setup_field_plot(fig, gs, (0,0))
mass_points = ax_field_0.scatter([], [], c="black", s=2, label="Masses", zorder=5)

def update(frame_idx):
    global contour_0
    frame = frames[frame_idx]
    subset_0 = df_field[df_field["frame"] == frame].copy()
    subset_1 = df_mass[df_mass["frame"] == frame].copy()
    # field
    contour_0 = update_field(ax_field_0, subset_0["x"], subset_0["y"], subset_0["phi"], frame, contour_0)
    mass_points.set_offsets(np.c_[subset_1["x"], subset_1["y"]])
    mass_points.set_zorder(10)
    return contour_0, mass_points

ani = FuncAnimation(fig, update, frames=len(frames), interval=1000/frames_per_second, blit=False)
plt.show()
