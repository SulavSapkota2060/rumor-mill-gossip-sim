#include <iostream>
#include "node.hpp"
#include <vector>
#include  "network.hpp"





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


    for(std::size_t i = 0; i < nodes.size(); ++i){
        nodes[i].id = static_cast<int>(i);
        std::cout << nodes[i].id << " | Informed: " << nodes[i].informed << "\n";
        std::cout << "{";
        const std::vector<std::size_t>& nbrs = net.neighbors(i);
        for(std::size_t j=0; j<nbrs.size(); ++j){
            if(j>0) std::cout << ", ";
            std::cout << nbrs[j];
        }

        std::cout << "}\n";
    }







    return 0;

}