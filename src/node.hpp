#pragma once

struct Node {
    int id = -1;
    bool informed = false;
    bool online = true;
    double first_heard_at = -1.0;

    bool receive_rumor(double time){
        if(informed) return false;
        informed = true;
        first_heard_at = time;
        return true;
    }
};

