#include <iostream>
#include <random>
#include "network.hpp"
#include "simulation.hpp"

int main() {
    std::mt19937 graph_rng(7);
    Network net = Network::random_connected(1000, 6.0, graph_rng);

    std::cout << "fanout,loss,seed,informed,t50,t90,t99,"
                 "attempted,lost,delivered,redundant,attempted_at_99,"
                 "delivered_at_99,redundant_at_99\n";

    for (std::size_t fanout : {1u, 3u, 6u}) {
        for (double loss : {0.0, 0.15, 0.3}) {
            for (std::uint32_t seed = 1; seed <= 20; ++seed) {
                Simulation sim(net, Config{.time_limit = 60.0, .loss = loss,
                                           .seed = seed, .fanout = fanout});
                sim.run();
                const Metrics& m = sim.metrics();
                std::cout << fanout << ',' << loss << ',' << seed << ','
                          << m.informed << ','
                          << m.t50 << ',' << m.t90 << ',' << m.t99 << ','
                          << m.attempted << ',' << m.lost << ','
                          << m.delivered << ',' << m.redundant << ','
                          << m.attempted_at_99 << ','
                          << m.delivered_at_99 << ','
                          << m.redundant_at_99 << '\n';
            }
        }
    }
    return 0;
}
