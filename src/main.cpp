#include "connection.h"
#include <iostream>

int main(int argc, char *argv[]) {
    auto connection = Connection{};

    connection.callback([](auto &&j) { std::cout << j << std::endl; });

    return 0;
}
