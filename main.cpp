#include <phi/engine.hpp>
#include <iostream>

using namespace phi::render;

struct Game : public phi::Engine {
    void update() override {
    }

    std::vector<Renderable> render() override {
        Renderable cube{};
        cube.mesh = phi::render::factory::cube();

        return std::vector<Renderable>{cube};
    }
};

int main(int argc, char **argv) {
    if (argc > 1) {
        phi::g_assets = argv[1];
    } else {
        phi::g_assets = ".";
    }
    Game game{};
    game.run();
    return 0;
}
