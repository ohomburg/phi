#include <phi/engine.hpp>
#include <iostream>

const char *phi::g_assets = "###";

phi::Engine::Engine() : window() {
}

bool phi::Engine::run() {
    compositor.render();
    return window.pumpEvents();
}
