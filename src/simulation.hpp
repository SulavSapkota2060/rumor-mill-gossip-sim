#pragma once
#include <cstddef>
#include <cstdint>
#include <queue>
#include <vector>
#include "event.hpp"
#include "network.hpp"
#include "node.hpp"
#include <random>

struct Config{
    double latency {0.5};
    double send_interval {0.5};
    double time_limit {10.0};
    std::size_t origin {0};
    double loss{0.0};
    std::uint32_t seed {42};
    std::size_t fanout {3};

};


struct Metrics {
    std::size_t attempted{0};
    std::size_t lost{0};
    std::size_t delivered{0};
    std::size_t redundant{0};
    std::size_t informed{0};
    double t50{-1.0};
    double t90{-1.0};
    double t99{-1.0};
    std::size_t attempted_at_99{0};
    std::size_t delivered_at_99{0};
    std::size_t redundant_at_99{0};
};


class Simulation{
public:
Simulation(const Network& net, Config config);
void run();
const std::vector<Node>& nodes() const;
const Metrics& metrics() const;

private:
void schedule(double time, Event::Type type, std::size_t node);
void on_send(const Event& e);
void on_deliver(const Event& e);
Metrics metrics_;

const Network& net_ ;
Config config_;
std::mt19937 rng;
std::vector<Node> nodes_;
std::priority_queue<Event, std::vector<Event>, LaterFirst> queue_;
std::uint64_t next_seq_ = 0;

};