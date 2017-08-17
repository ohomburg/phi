#pragma once

#include <memory>
#include "event.hpp"

namespace phi::render {
    class Compositor : public EventHandler {
        struct Impl;
        std::unique_ptr<Impl> impl;
    public:
        Compositor(phi::Window &window);

        ~Compositor();

        void render();

        void handle(const event::Event &ev) override;
    };
}