#include <iostream>
#include "node.hpp"
#include <vector>
#include  "network.hpp"
#include "simulation.hpp"





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


    Simulation sim(net, Config {});
    sim.run();
    for(const Node& n : sim.nodes()){
        std::cout << "node" << n.id << " heard at " << n.first_heard_at << "\n";
    }


    






    return 0;

}