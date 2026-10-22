#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>
#include "event.hpp"
#include "node.hpp"


int main() {

    std::cout << std::boolalpha;
    std::priority_queue<Event, std::vector<Event>, LaterFirst> queue;

    queue.push(Event{.time = 2.4, .seq = 0, .type = Event::Type::Send, .node = 0});
    queue.push(Event{.time = 1.0, .seq = 1, .type = Event::Type::Send, .node = 1});
    queue.push(Event{.time = 3.2, .seq = 2, .type = Event::Type::Deliver, .node = 2});
    queue.push(Event{.time = 2.0, .seq = 3, .type = Event::Type::Send, .node = 3});

    // Same time as the event above: the lower seq must come out first.
    queue.push(Event{.time = 2.0, .seq = 4, .type = Event::Type::Deliver, .node = 4});

    while (!queue.empty()) {
        const Event& e = queue.top();
        std::cout << "time " << e.time << "  seq " << e.seq << "  node " << e.node << "\n";
        queue.pop();
    }


    Node n;
    std::cout << n.receive_rumor(2.5) << "\n";
    std::cout << n.receive_rumor(4.0) << "\n";
    std::cout << n.first_heard_at << "\n";

    return 0;
}
