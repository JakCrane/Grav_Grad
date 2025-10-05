import matplotlib.pyplot as plt

def setup_accelerometer_plots(fig, gs, df_acc_0, df_acc_1):
    ax_acc0 = fig.add_subplot(gs[0, 1])
    ax_acc1 = fig.add_subplot(gs[1, 1], sharex=ax_acc0)

    ax_acc0.plot(df_acc_0["frame"], df_acc_0["reading"], label="Accelerometer 0")
    time_marker0 = ax_acc0.axvline(x=0, color="r", linestyle="--", label="Current frame")
    ax_acc0.set_ylabel("Reading 0")
    ax_acc0.legend()

    ax_acc1.plot(df_acc_1["frame"], df_acc_1["reading"], label="Accelerometer 1")
    time_marker1 = ax_acc1.axvline(x=0, color="r", linestyle="--")
    ax_acc1.set_xlabel("Frame")
    ax_acc1.set_ylabel("Reading 1")
    ax_acc1.legend()

    return ax_acc0, ax_acc1, time_marker0, time_marker1
