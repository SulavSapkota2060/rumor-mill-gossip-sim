#include <iostream>
#include "node.hpp"
#include <vector>
#include  "network.hpp"
#include "simulation.hpp"
#include <algorithm>


void run_and_print(const Network& net, Config config){
    Simulation sim(net, config);
    sim.run();
    for (const Node& n: sim.nodes()){
        std::cout << "node " << n.id << "heard at " << n.first_heard_at << "\n";
    }
    std::cout << "\n";
}


void run_and_summarize(const Network& net, Config cfg){
    Simulation sim(net, cfg);
    sim.run();

    std::size_t infromed = 0;
    double last = 0.0;
    for(const Node& n: sim.nodes()) {
        if (n.informed){
            ++infromed; 
            last = std::max(last, n.first_heard_at);
        }
    }
    std::cout << "fanout " << cfg.fanout << ": informed " << infromed << "/" << sim.nodes().size() << ", last heard at " << last << "\n";
}



int main(){
    std::cout << std::boolalpha;

   




    std::mt19937 graph_rng(7);
    Network net = Network::random_connected(1000, 6.0, graph_rng);

    for(std::size_t f : {1u, 3u, 6u}){
        run_and_summarize(net, Config{.time_limit=60.0, .loss=0.15, .seed=42, .fanout=f});
    }




    return 0;

}