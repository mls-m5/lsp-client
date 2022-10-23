#pragma once

#include <fstream>

struct Connection {
    ~Connection();

    std::ifstream in;
    std::ofstream out;
};
