import numpy as np
import matplotlib.pyplot as plt

def setup_derived_plots(fig, gs, df_xy):
    ax_magnitude = fig.add_subplot(gs[0, 2])
    ax_inv_magnitude = fig.add_subplot(gs[1, 2], sharex=ax_magnitude)
    ax_direction = fig.add_subplot(gs[2, 2], sharex=ax_magnitude)

    ax_magnitude.plot(df_xy["frame"], df_xy["magnitude"], label="Magnitude")
    time_marker2 = ax_magnitude.axvline(x=0, color="r", linestyle="--")
    ax_magnitude.set_ylabel("Magnitude")
    ax_magnitude.legend()

    ax_inv_magnitude.plot(df_xy["frame"], 1/df_xy["magnitude"], label="Inv Magnitude")
    time_marker3 = ax_inv_magnitude.axvline(x=0, color="r", linestyle="--")
    ax_inv_magnitude.set_ylabel("Inv Magnitude")
    ax_inv_magnitude.legend()

    ax_direction.plot(df_xy["frame"], df_xy["direction_rad"], label="Direction (rad)")
    time_marker4 = ax_direction.axvline(x=0, color="r", linestyle="--")
    ax_direction.set_xlabel("Frame")
    ax_direction.set_ylabel("Direction (rad)")
    ax_direction.legend()

    return ax_magnitude, ax_inv_magnitude, ax_direction, time_marker2, time_marker3, time_marker4
