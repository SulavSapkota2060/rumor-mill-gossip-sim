"""Plot t99 and messages-by-t99 against fanout, one line per loss rate."""
import sys

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

src = sys.argv[1] if len(sys.argv) > 1 else "results.csv"
out = sys.argv[2] if len(sys.argv) > 2 else "results.png"

df = pd.read_csv(src)
df = df[df["t99"] >= 0]  # only runs that reached 99%

grouped = df.groupby(["loss", "fanout"])
mean = grouped[["t99", "attempted_at_99"]].mean()
sem = grouped[["t99", "attempted_at_99"]].sem()

fig, axes = plt.subplots(1, 2, figsize=(10, 4), sharex=True)
panels = [
    ("t99", "Time to inform 99% (simulated time units)"),
    ("attempted_at_99", "Messages sent by t99"),
]
for ax, (col, label) in zip(axes, panels):
    for loss in sorted(df["loss"].unique()):
        m = mean.loc[loss][col]
        e = sem.loc[loss][col]
        ax.errorbar(m.index, m.values, yerr=e.values, marker="o", capsize=3,
                    label=f"loss {loss:.0%}")
    ax.set_xlabel("fanout")
    ax.set_ylabel(label)
    ax.set_xticks(sorted(df["fanout"].unique()))
    ax.grid(alpha=0.3)
axes[0].legend()
axes[1].set_ylim(bottom=0)

runs = int(df.groupby(["loss", "fanout"]).size().min())
fig.suptitle(f"Push gossip, 1000 nodes, mean over {runs}+ seeds (bars: standard error)",
             fontsize=10)
fig.tight_layout()
fig.savefig(out, dpi=150)
print(f"wrote {out}")
