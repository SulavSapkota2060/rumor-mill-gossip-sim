#include <iostream>
#include "node.hpp"
#include <vector>
#include  "network.hpp"
#include "simulation.hpp"


void run_and_print(const Network& net, Config config){
    Simulation sim(net, config);
    sim.run();
    for (const Node& n: sim.nodes()){
        std::cout << "node " << n.id << "heard at " << n.first_heard_at << "\n";
    }
    std::cout << "\n";
}


int main(){
    std::cout << std::boolalpha;

    std::vector<Node> nodes(5);

    Network net(5);

    nodes[0].informed = true;
    nodes[0].first_heard_at = 0.0;

    net.add_edge(0,1);
    net.add_edge(1,2);
    net.add_edge(2,3);
    net.add_edge(3,4);


    // 1. Same seed twice: output must be identical
    run_and_print(net, Config{.time_limit = 30.0, .loss=0.3, .seed=42});
    run_and_print(net, Config{.time_limit = 30.0, .loss=0.3, .seed=42});

    // 2. Different seed: output should differ
    run_and_print(net, Config{.time_limit=30.0, .loss = 0.3, .seed=43});

    // 3. Total loss: only node 0 should have a time, the rest stay at -1
    run_and_print(net, Config{.time_limit=30.0, .loss=1.0, .seed=42});

    // 4. Heavy loss: should still reach everyone, just later
    run_and_print(net, Config{.time_limit=30.0, .loss=0.5, .seed=42});

    



    






    return 0;

}