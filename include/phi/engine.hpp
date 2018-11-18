#pragma once

#include "event.hpp"
#include "render.hpp"

namespace phi {
    extern const char *g_assets;

    class Engine {
        Window window;
        render::Compositor compositor{window};
    public:
        Engine();

        bool run();
    };
}
