import numpy as np
import matplotlib.pyplot as plt

def setup_field_plot(fig, gs):
    ax_field = fig.add_subplot(gs[:, 0])
    ax_field.set_aspect("equal")
    contour = None
    mass_dot, = ax_field.plot([], [], "ro")
    accel_dot, = ax_field.plot([], [], "go")
    return ax_field, contour, mass_dot, accel_dot

def update_field(ax_field, subset, frame, contour, mass_dot, accel_dot,
                 radian_change_per_frame):
    # Remove old contour
    if contour:
        for coll in contour.collections:
            coll.remove()

    # invert phi for plotting
    subset = subset.copy()
    subset['phi'] = -subset['phi']

    # scale
    phi_min = subset["phi"].min()
    phi_max = subset["phi"].max()

    # if positive values only -> logspace
    if phi_min > 0:
        phi_levels = np.logspace(np.log10(phi_min), np.log10(phi_max), 30)
    else:
        phi_levels = np.linspace(phi_min, phi_max, 30)

    contour = ax_field.tricontourf(
        subset["x"], subset["y"], subset["phi"],
        levels=phi_levels,
        cmap="viridis"
    )

    accel_dot.set_data(1.0, 1.0)
    ax_field.set_title(f"Frame {frame}")

    return contour, mass_dot, accel_dot
