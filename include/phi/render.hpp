#pragma once

#include <memory>
#include "event.hpp"
#include "core.hpp"

namespace phi::render {
    struct Mesh {
        virtual ~Mesh() = default;
        virtual void drawCall() = 0;
    };

    struct Renderable {
        phi::Transform tf{};
        std::shared_ptr<Mesh> mesh;

        ~Renderable();
    };

    class Compositor : public EventHandler {
        struct Impl;
        std::unique_ptr<Impl> impl;
    public:
        explicit Compositor(phi::Window &window);

        ~Compositor();

        void render(std::vector<Renderable> &&objs);

        void handle(const event::Event &ev) override;
    };

    namespace factory {
        std::shared_ptr<Mesh> cube();
        std::shared_ptr<Mesh> icoSphere();
    }
}
