#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include "network.hpp"
#include "simulation.hpp"

static int failures = 0;

#define CHECK(cond) \
    do { if (!(cond)) { std::cout << "FAIL line " << __LINE__ << ": " #cond "\n"; ++failures; } } while (0)


static Network make_line(std::size_t n) {
    Network net(n);
    for (std::size_t i = 0; i + 1 < n; ++i) net.add_edge(i, i + 1);
    return net;
}

static std::size_t count_informed(const Simulation& sim) {
    std::size_t k = 0;
    for (const Node& n : sim.nodes()) if (n.informed) ++k;
    return k;
}

static void test_total_loss_reaches_only_origin() {
    Network net = make_line(10);
    Simulation sim(net, Config{.time_limit = 30.0, .loss = 1.0, .seed = 1});
    sim.run();
    CHECK(count_informed(sim) == 1);
    CHECK(sim.nodes()[0].informed);
    CHECK(sim.metrics().delivered == 0);
    CHECK(sim.metrics().lost == sim.metrics().attempted);
}
static void test_no_loss_reaches_everyone() {
    std::mt19937 graph_rng(3);
    Network net = Network::random_connected(200, 4.0, graph_rng);
    Simulation sim(net, Config{.time_limit = 60.0, .loss = 0.0, .seed = 5});
    sim.run();
    CHECK(count_informed(sim) == 200);
    CHECK(sim.metrics().informed == 200);
    CHECK(sim.metrics().lost == 0);
    // every student except the origin is informed by exactly one non-redundant delivery
    CHECK(sim.metrics().delivered - sim.metrics().redundant == 199);
}

static void test_same_seed_same_result() {
    std::mt19937 graph_rng(3);
    Network net = Network::random_connected(200, 4.0, graph_rng);
    Config cfg{.time_limit = 60.0, .loss = 0.2, .seed = 9};

    Simulation a(net, cfg);
    Simulation b(net, cfg);
    a.run();
    b.run();

    CHECK(a.metrics().attempted == b.metrics().attempted);
    CHECK(a.metrics().delivered == b.metrics().delivered);
    CHECK(a.metrics().t99 == b.metrics().t99);
    for (std::size_t i = 0; i < a.nodes().size(); ++i) {
        CHECK(a.nodes()[i].first_heard_at == b.nodes()[i].first_heard_at);
    }
}

static void test_random_graph_is_connected_with_target_degree() {
    std::mt19937 rng(11);
    const std::size_t n = 500;
    Network net = Network::random_connected(n, 6.0, rng);

    // BFS from node 0
    std::vector<bool> seen(n, false);
    std::vector<std::size_t> stack{0};
    seen[0] = true;
    std::size_t reached = 1;
    while (!stack.empty()) {
        std::size_t u = stack.back();
        stack.pop_back();
        for (std::size_t v : net.neighbors(u)) {
            if (!seen[v]) { seen[v] = true; ++reached; stack.push_back(v); }
        }
    }
    CHECK(reached == n);

    std::size_t degree_sum = 0;
    bool simple = true;
    for (std::size_t u = 0; u < n; ++u) {
        const auto& nb = net.neighbors(u);
        degree_sum += nb.size();
        std::vector<std::size_t> sorted = nb;
        std::sort(sorted.begin(), sorted.end());
        if (std::adjacent_find(sorted.begin(), sorted.end()) != sorted.end()) simple = false;
        if (std::find(nb.begin(), nb.end(), u) != nb.end()) simple = false;
    }
    CHECK(simple);  // no self-loops, no duplicate edges
    CHECK(degree_sum == 6 * n);  // target edges = 6 * n / 2, each counted twice
}

static void test_time_limit_stops_spread() {
    Network net = make_line(50);
    Simulation sim(net, Config{.time_limit = 1.0, .loss = 0.0, .seed = 2});
    sim.run();
    CHECK(count_informed(sim) < 50);
    for (const Node& node : sim.nodes()) {
        if (node.informed) CHECK(node.first_heard_at <= 1.0);
    }
}

static void test_milestones_are_ordered() {
    std::mt19937 graph_rng(3);
    Network net = Network::random_connected(300, 5.0, graph_rng);
    Simulation sim(net, Config{.time_limit = 60.0, .loss = 0.1, .seed = 4});
    sim.run();
    const Metrics& m = sim.metrics();
    CHECK(m.t50 >= 0 && m.t90 >= 0 && m.t99 >= 0);
    CHECK(m.t50 <= m.t90 && m.t90 <= m.t99);
    CHECK(m.attempted_at_99 <= m.attempted);
    CHECK(m.redundant_at_99 <= m.delivered_at_99);
    CHECK(m.delivered_at_99 <= m.delivered);
}

int main() {
    test_total_loss_reaches_only_origin();
    test_no_loss_reaches_everyone();
    test_same_seed_same_result();
    test_random_graph_is_connected_with_target_degree();
    test_time_limit_stops_spread();
    test_milestones_are_ordered();

    std::cout << (failures == 0 ? "ALL PASS\n" : "SOME FAILED\n");
    return failures == 0 ? 0 : 1;
}
