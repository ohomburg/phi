#pragma once

#include <memory>
#include "event.hpp"

namespace phi::render {
    class Compositor : public EventHandler {
        struct Impl;
        std::unique_ptr<Impl> impl;
    public:
        explicit Compositor(phi::Window &window);

        ~Compositor();

        void render();

        void handle(const event::Event &ev) override;
    };

    class Quad {
        struct Impl;
        std::unique_ptr<Impl> impl;
    public:
        ~Quad();

        Quad();

        void drawCall();
    };

    class IcoSphere {
        struct Impl;
        std::unique_ptr<Impl> impl;
    public:
        ~IcoSphere();

        IcoSphere();

        void drawCall();
    };

    class Cube {
        struct Impl;
        std::unique_ptr<Impl> impl;
    public:
        ~Cube();

        Cube();

        void drawCall();
    };
}
