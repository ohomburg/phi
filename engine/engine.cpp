#include <phi/engine.hpp>

void phi::Engine::run() {
    while (window.pumpEvents()) {
        compositor.render();
    }
}
