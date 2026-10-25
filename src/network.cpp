#include "network.hpp"
#include <cassert>
#include <algorithm>
#include <numeric>


Network::Network(std::size_t node_count) : adjacency_(node_count) {}

void Network::add_edge(std::size_t a, std::size_t b) {
    assert(a < size() && b < size());
    adjacency_[a].push_back(b);
    adjacency_[b].push_back(a);
}

const std::vector<std::size_t>& Network::neighbors(std::size_t id) const {
    assert(id < size());
    return adjacency_[id];
}

std::size_t Network::size() const {
    return adjacency_.size();
}

Network Network::random_connected(std::size_t node_count, double avg_degree, std::mt19937& rng){
    Network net(node_count);
    if (node_count < 2) return net;

    std::vector<std::size_t> order(node_count);
    std::iota(order.begin(), order.end(), 0);
    std::shuffle(order.begin(), order.end(), rng);

    for(std::size_t i = 1; i < node_count; ++i){
        std::uniform_int_distribution<std::size_t> pick(0,i-1);
        net.add_edge(order[i], order[pick(rng)]);
    }
    
    const std::size_t max_edges = node_count * (node_count - 1) / 2;
    std::size_t target = static_cast<std::size_t>(avg_degree * static_cast<double>(node_count) / 2.0);
    target = std::min(target, max_edges);

    std::uniform_int_distribution<std::size_t> any(0, node_count - 1);
    std::size_t edges = node_count - 1;
    std::size_t attempts = 0;
    const std::size_t max_attempts = target * 20 + 100;

    while (edges < target && attempts < max_attempts) {
        ++attempts;
        std::size_t a = any(rng);
        std::size_t b = any(rng);
        if (a == b || net.has_edge(a, b)) continue;
        net.add_edge(a, b);
        ++edges;
    }
    return net;



}

bool Network::has_edge(std::size_t a, std::size_t b) const{
    const auto& list = adjacency_[a];
    return std::find(list.begin(), list.end(), b) != list.end();
}
