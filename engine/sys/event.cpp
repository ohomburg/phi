#include <phi/event.hpp>
#include <GLFW/glfw3.h>
#include <Remotery.h>

using namespace std;

struct phi::Window::Impl {
    vector<reference_wrapper<phi::EventHandler>> handlers;
    GLFWwindow *window = nullptr;
    Remotery *rmt = nullptr;
};

static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    rmt_ScopedCPUSample(KeyboardEvent, 0);
    auto *wrapper = static_cast<phi::Window *>(glfwGetWindowUserPointer(window));
    wrapper->event(phi::event::RawKB{key, scancode, action, mods});
}

static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    rmt_ScopedCPUSample(MouseButtonEvent, 0);
    auto *wrapper = static_cast<phi::Window *>(glfwGetWindowUserPointer(window));
    wrapper->event(phi::event::RawMB{button, action, mods});
}

phi::Window::Window() {
    impl = make_unique<Impl>();

    // Initialize Remotery
    if (rmt_CreateGlobalInstance(&impl->rmt) != RMT_ERROR_NONE) {
        throw runtime_error("Remotery init failed");
    }

    // Initialize GLFW
    rmt_ScopedCPUSample(Init, 0);
    if (glfwInit() != GLFW_TRUE) {
        throw runtime_error("glfwInit failed");
    }
    impl->window = glfwCreateWindow(1600, 900, "Phi Engine", nullptr, nullptr);
    if (impl->window == nullptr) {
        throw runtime_error("Window creation failed");
    }
    glfwSetWindowUserPointer(impl->window, this);
    glfwSetKeyCallback(impl->window, KeyCallback);
    glfwSetMouseButtonCallback(impl->window, MouseButtonCallback);
}

phi::Window::~Window() {
    glfwTerminate();
    rmt_DestroyGlobalInstance(impl->rmt);
}

bool phi::Window::pumpEvents() {
    glfwPollEvents();
    return glfwWindowShouldClose(impl->window) == 0;
}

void phi::Window::registerHandler(phi::EventHandler &e) {
    impl->handlers.push_back(ref(e));
}

void phi::Window::event(const phi::event::Event &ev) {
    for (auto &handler : impl->handlers) {
        handler.get().handle(ev);
    }
}
