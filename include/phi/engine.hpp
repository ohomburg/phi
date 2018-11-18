#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "event.hpp"
#include "render.hpp"
#include "core.hpp"

namespace phi {
    extern const char *g_assets;

    class Engine {
        Window window;
        render::Compositor compositor{window};
    public:
        Engine();

        virtual ~Engine() = default;

        void run();

        virtual void update() = 0;

        virtual std::vector<render::Renderable> render() = 0;
    };
}
