"""Summarize results.csv from the rumor mill sweep, one row per (fanout, loss)."""
import sys

import numpy as np
import pandas as pd

path = sys.argv[1] if len(sys.argv) > 1 else "results.csv"
df = pd.read_csv(path)

# -1 means "never reached that fraction"; keep it out of the means.
for col in ["t50", "t90", "t99"]:
    df[col] = df[col].where(df[col] >= 0, np.nan)

df["reached_99"] = df["t99"].notna()
df["redundant_frac"] = (df["redundant_at_99"] / df["delivered_at_99"]).where(df["reached_99"], np.nan)
# attempted_at_99 is only meaningful for runs that reached 99%
df["msgs_at_99"] = df["attempted_at_99"].where(df["reached_99"], np.nan)

summary = (
    df.groupby(["fanout", "loss"])
    .agg(
        runs=("seed", "count"),
        reached_99=("reached_99", "mean"),
        informed=("informed", "mean"),
        t50=("t50", "mean"),
        t90=("t90", "mean"),
        t99=("t99", "mean"),
        msgs_at_99=("msgs_at_99", "mean"),
        redundant_frac=("redundant_frac", "mean"),
    )
    .round(3)
    .reset_index()
)

print(summary.to_string(index=False))
print()
print("t50/t90/t99/msgs_at_99 are means over only the runs that reached that level;")
print("reached_99 is the fraction of runs that got there.")