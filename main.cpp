#include <phi/engine.hpp>
#include <iostream>

int main(int argc, char **argv) {
    if (argc > 1) {
        phi::g_assets = argv[1];
    } else {
        phi::g_assets = ".";
    }
    phi::Engine engine{};
    while (engine.run()) {

    }
    return 0;
}