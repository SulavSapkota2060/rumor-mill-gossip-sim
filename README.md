# Rumor Mill

A discrete-event simulator, written in C++20, of push-gossip dissemination: one
student starts a rumor and passes it to random friends. It is a single-process
simulation of the protocol, not a real distributed cluster.

## Model

- **Network:** a random connected graph. Each node joins a random spanning tree,
  then random extra edges are added until the target mean degree is reached.
- **Gossip:** an informed node, every `send_interval` (0.5), picks up to `fanout`
  random neighbors and sends them the rumor. A node that has just heard the rumor
  starts gossiping immediately. Informed nodes never stop gossiping, so every
  run needs a time limit.
- **Network faults:** each message is lost with probability `loss`. Delivered
  messages arrive after an exponentially distributed latency (mean 0.5).
- **Engine:** a priority queue of events ordered by time, with a sequence number
  as tie-break so runs are deterministic. All randomness comes from a seeded
  `std::mt19937`.
- **Metrics:** time to inform 50%, 90% and 99% of nodes (t50, t90, t99),
  messages sent by t99, and the share of deliveries that were duplicates.

Times are simulated time units, not seconds.

## Build, test, run

```
make            # build/main
make test       # build and run the tests
make results    # run the sweep and write results.csv
```

Summarize the sweep (needs `pandas` and `numpy`):

```
python3 -m venv .venv && source .venv/bin/activate
pip install pandas numpy
python scripts/summarize.py results.csv
pip install matplotlib
python scripts/plot.py results.csv results.png   # t99 and messages vs fanout
```

## Results

Setup: 1000 nodes, one fixed random graph (graph seed 7, mean degree 6), time
limit 60, 20 simulation seeds per row. Seeds vary loss, latency and peer
choices; the graph does not change. All 180 runs reached 99% of nodes.

| fanout | loss | t50 | t90 | t99 | messages by t99 | duplicates by t99 |
|---|---|---|---|---|---|---|
| 1 | 0.00 | 5.83 | 7.75 | 10.59 | 9960 | 88.9% |
| 1 | 0.15 | 7.12 | 9.36 | 12.84 | 11870 | 89.2% |
| 1 | 0.30 | 8.59 | 11.29 | 15.68 | 14570 | 89.5% |
| 3 | 0.00 | 1.91 | 2.56 | 3.47 | 10631 | 87.3% |
| 3 | 0.15 | 2.39 | 3.15 | 4.25 | 12394 | 87.7% |
| 3 | 0.30 | 2.88 | 3.83 | 5.18 | 14982 | 88.3% |
| 6 | 0.00 | 0.97 | 1.32 | 1.79 | 10837 | 84.6% |
| 6 | 0.15 | 1.23 | 1.63 | 2.20 | 12492 | 85.1% |
| 6 | 0.30 | 1.58 | 2.08 | 2.76 | 14684 | 85.7% |

What the numbers show:

- Higher fanout speeds up spread: at 0% loss, t99 drops from 10.6 (fanout 1) to
  3.5 (fanout 3) to 1.8 (fanout 6), with diminishing returns.
- Fanout does not make dissemination cheaper. Messages sent by t99 stay around
  10-11k at 0% loss, because faster spread is offset by more messages per round.
- Loss mostly delays things. From 0% to 30% loss, t99 rises about 48% at
  fanout 1 and about 49% at fanout 3.
- Roughly 85-90% of messages sent before t99 are duplicates, and fanout changes
  that only slightly, because informed nodes resend every interval.

## Limitations

- One graph topology and size; results are conditional on it.
- Same-seed determinism is only claimed on the same machine and standard
  library, since `std::` distributions are not portable across implementations.
- Latency and the send interval are fixed in this sweep; they were not varied.
- No partitions, node failures, or alternative protocols (push-pull).
