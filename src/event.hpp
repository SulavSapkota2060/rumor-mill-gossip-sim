#pragma once
#include <cstddef>
#include <cstdint>


struct Event {
    enum class Type { Send, Deliver };
    double time;
    std::uint64_t seq;
    Type type;
    std::size_t node;
};

struct LaterFirst {
    // priority_queue is a max-heap, so "a goes after b" means a happens later
    bool operator()(const Event& a, const Event& b) const {
        if (a.time != b.time) return a.time > b.time;
        return a.seq > b.seq;
    }
};