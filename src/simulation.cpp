#include "simulation.hpp"
#include "event.hpp"
#include "network.hpp"

Simulation::Simulation(const Network& net, Config config): net_(net), config_(config), rng(config.seed), nodes_(net.size()) {
    for(std::size_t i=0; i<nodes_.size(); ++i){
        nodes_[i].id = static_cast<int>(i);

    }

    nodes_[config_.origin].receive_rumor(0.0);
    schedule(0.0, Event::Type::Send, config_.origin);
}

void Simulation::schedule(double time, Event::Type type, std::size_t node) {
    queue_.push(Event{.time = time, .seq = next_seq_++, .type = type, .node = node});
}

const std::vector<Node> & Simulation::nodes() const {return nodes_ ;}




void Simulation::on_send(const Event& e){
    std::bernoulli_distribution is_lost(config_.loss);
    std::exponential_distribution<double> delay(1.0/ config_.latency);
    std::vector<std::size_t> targets = net_.neighbors(e.node);
    std::shuffle(targets.begin(), targets.end(), rng);
    std::size_t count = std::min(config_.fanout, targets.size());

    for(std::size_t i=0; i < count; ++i){
        if(is_lost(rng)) continue;
        schedule(e.time + delay(rng), Event::Type::Deliver, targets[i]);
    }
    schedule(e.time+config_.send_interval, Event::Type::Send, e.node);

}

void Simulation::on_deliver(const Event& e){
    if(nodes_[e.node].receive_rumor(e.time)){
        schedule(e.time , Event::Type::Send, e.node);
    }
}

void Simulation::run(){
    while (!queue_.empty()){
        Event e = queue_.top();
        queue_.pop();

        if(e.time > config_.time_limit) break;

        if (e.type == Event::Type::Send){
            on_send(e);
        }
        else{
            on_deliver(e);
        }
    }
}



