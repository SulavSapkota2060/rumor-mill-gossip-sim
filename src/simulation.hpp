#pragma once
#include <cstddef>
#include <cstdint>
#include <queue>
#include <vector>
#include "event.hpp"
#include "network.hpp"
#include "node.hpp"


struct Config{
    double latency {0.5};
    double send_interval {0.5};
    double time_limit {10.0};
    std::size_t origin {0};

};


class Simulation{
public:
Simulation(const Network& net, Config config);
void run();
const std::vector<Node>& nodes() const;


private:
void schedule(double time, Event::Type type, std::size_t node);
void on_send(const Event& e);
void on_deliver(const Event& e);

const Network& net_ ;
Config config_;
std::vector<Node> nodes_;
std::priority_queue<Event, std::vector<Event>, LaterFirst> queue_;
std::uint64_t next_seq_ = 0;

};