#pragma once

#include <cstddef>
#include <vector>
#include <random>


class Network{
    public:
    explicit Network(std::size_t node_count);

    void add_edge(std::size_t a, std::size_t b);

    const std::vector<std::size_t>& neighbors(std::size_t id) const;
    
    std::size_t size() const;

    bool has_edge(std::size_t a, std::size_t b) const;

    static Network random_connected(std::size_t node_count, double avg_degree, std::mt19937& rng);



    
    private:

    std::vector<std::vector<std::size_t>> adjacency_;


};