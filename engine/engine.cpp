#include <phi/engine.hpp>
#include <iostream>

const char *phi::g_assets = "###";

phi::Engine::Engine() : window() {
}

void phi::Engine::run() {
    while (window.pumpEvents()) {
        update();
        compositor.render(render());
    }
}
