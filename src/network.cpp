#include "network.hpp"

#include <cassert>

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
