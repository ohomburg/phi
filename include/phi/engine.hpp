#pragma once

#include "event.hpp"
#include "render.hpp"

namespace phi {
    class Engine {
        Window window;
        render::Compositor compositor{window};
    public:

        void run();
    };
}
