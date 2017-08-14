#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace phi {
    namespace event {
        struct Event {
            enum class Type {
                APPLICATION, KEYBOARD, MOUSE
            } type;

            explicit Event(Type type) : type(type) {}
        };

        struct RawKB : public Event {
            RawKB(int key, int scancode, int action, int mods) : Event(Type::KEYBOARD), key(key), scancode(scancode),
                                                                 action(action), mods(mods) {}

            int key, scancode, action, mods;
        };

        struct RawMB : public Event {
            RawMB(int button, int action, int mods) : Event(Type::MOUSE), button(button), action(action), mods(mods) {}

            int button, action, mods;
        };
    }

    class EventHandler {
    public:
        virtual void handle(const event::Event &ev) = 0;
    };

    class Window {
        struct Impl;
        std::unique_ptr<Impl> impl;
    public:
        Window();

        ~Window();

        bool pumpEvents();

        void event(const event::Event &ev);

        void registerHandler(EventHandler &e);
    };
}