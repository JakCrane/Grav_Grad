import numpy as np
import matplotlib.pyplot as plt

def setup_field_plot(fig, gs, position=(slice(None), 0), aspect="equal"):
    """
    Create a field subplot in a flexible GridSpec position.

    Parameters
    ----------
    fig : matplotlib.figure.Figure
    gs  : matplotlib.gridspec.GridSpec
    position : tuple
        Index or slice into the GridSpec, e.g. (0,0), (1,1), (slice(1,3), 2), etc.
    aspect : str or float
        Aspect ratio for the axis.

    Returns
    -------
    ax_field : matplotlib.axes.Axes
    contour  : None (placeholder)
    """
    ax_field = fig.add_subplot(gs[position])
    ax_field.set_aspect(aspect)
    contour = None
    return ax_field, contour

def update_field(ax_field, subset_x, subset_y, subset_value, frame, contour):
    # Remove old contour
    if contour:
        for coll in contour.collections:
            coll.remove()

    # # scale
    # value_min = subset_value.min()
    # value_max = subset_value.max()

    # phi_levels = np.linspace(value_min, value_max, 30)

    contour = ax_field.tricontourf(
        subset_x, subset_y, subset_value,
        cmap="viridis"
    )

    ax_field.set_title(f"Frame {frame}")

    return contour
