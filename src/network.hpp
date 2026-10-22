#pragma once

#include <cstddef>
#include <vector>


class Network{
    public:
    explicit Network(std::size_t node_count);

    void add_edge(std::size_t a, std::size_t b);

    const std::vector<std::size_t>& neighbors(std::size_t id) const;
    
    std::size_t size() const;
    
    private:

    std::vector<std::vector<std::size_t>> adjacency_;


};