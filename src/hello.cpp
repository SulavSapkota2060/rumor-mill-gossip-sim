#include <algorithm>
#include <iostream>
#include <queue>
#include <random>
#include <vector>
#include "network.hpp"

bool is_connected(const Network& net) {
    if (net.size() == 0) return true;
    std::vector<bool> seen(net.size(), false);
    std::queue<std::size_t> frontier;
    frontier.push(0);
    seen[0] = true;
    std::size_t count = 1;
    while (!frontier.empty()) {
        std::size_t u = frontier.front();
        frontier.pop();
        for (std::size_t v : net.neighbors(u)) {
            if (!seen[v]) {
                seen[v] = true;
                ++count;
                frontier.push(v);
            }
        }
    }
    return count == net.size();
}

int main() {
    bool all_ok = true;

    for (unsigned seed = 1; seed <= 20; ++seed) {
        std::mt19937 rng(seed);
        const std::size_t n = 200;
        Network net = Network::random_connected(n, 4.0, rng);

        std::size_t degree_sum = 0;
        for (std::size_t u = 0; u < n; ++u) {
            std::vector<std::size_t> list = net.neighbors(u);
            degree_sum += list.size();

            for (std::size_t v : list) {
                if (v == u) { std::cout << "FAIL self-loop, seed " << seed << "\n"; all_ok = false; }
                if (!net.has_edge(v, u)) { std::cout << "FAIL not symmetric, seed " << seed << "\n"; all_ok = false; }
            }
            std::sort(list.begin(), list.end());
            if (std::adjacent_find(list.begin(), list.end()) != list.end()) {
                std::cout << "FAIL duplicate friend, seed " << seed << "\n"; all_ok = false;
            }
        }
        if (!is_connected(net)) { std::cout << "FAIL not connected, seed " << seed << "\n"; all_ok = false; }
        if (degree_sum / 2 != 400) { std::cout << "FAIL edges " << degree_sum / 2 << " != 400, seed " << seed << "\n"; all_ok = false; }
    }

    // Tiny sizes must terminate and not crash.
    std::mt19937 rng(1);
    Network one = Network::random_connected(1, 4.0, rng);
    Network two = Network::random_connected(2, 5.0, rng);   // asks for more edges than can exist
    if (one.neighbors(0).size() != 0) { std::cout << "FAIL n=1\n"; all_ok = false; }
    if (two.neighbors(0).size() != 1 || two.neighbors(1).size() != 1) { std::cout << "FAIL n=2\n"; all_ok = false; }

    std::cout << (all_ok ? "ALL PASS\n" : "SOME FAILED\n");
    return all_ok ? 0 : 1;
}
