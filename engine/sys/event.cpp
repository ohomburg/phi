#include <GL/glew.h>
#include <phi/event.hpp>
#include <GLFW/glfw3.h>
#include <Remotery.h>
#include <cstring>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION

#include <nuklear/nuklear.h>
#include <nuklear/demo/glfw_opengl3/nuklear_glfw_gl3.h>

using namespace std;

struct phi::Window::Impl {
    vector<reference_wrapper<phi::EventHandler>> handlers;
    GLFWwindow *window = nullptr;
    Remotery *rmt = nullptr;
    nk_context *nuklear;
};

extern "C" void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    rmt_ScopedCPUSample(KeyboardEvent, 0);
    auto *wrapper = static_cast<phi::Window *>(glfwGetWindowUserPointer(window));
    wrapper->event(phi::event::RawKB{key, scancode, action, mods});
}

extern "C" void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    rmt_ScopedCPUSample(MouseButtonEvent, 0);
    auto *wrapper = static_cast<phi::Window *>(glfwGetWindowUserPointer(window));
    wrapper->event(phi::event::RawMB{button, action, mods});
}

extern "C" void RmtCommandCallback(const char *text, void *window) {
    rmt_ScopedCPUSample(CommandCallback, 0);
    auto *wrapper = static_cast<phi::Window *>(window);
    wrapper->event(phi::event::Command(text));
}

phi::Window::Window() {
    impl = make_unique<Impl>();

    rmtSettings *settings = rmt_Settings();
    settings->input_handler = RmtCommandCallback;
    settings->input_handler_context = this;

    // Initialize Remotery
    if (rmt_CreateGlobalInstance(&impl->rmt) != RMT_ERROR_NONE) {
        throw runtime_error("Remotery init failed");
    }

    // Initialize GLFW
    rmt_ScopedCPUSample(Init, 0);
    if (glfwInit() != GLFW_TRUE) { throw runtime_error("glfwInit failed"); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    impl->window = glfwCreateWindow(1600, 900, "Phi Engine", nullptr, nullptr);
    if (impl->window == nullptr) { throw runtime_error("Window creation failed"); }

    glfwMakeContextCurrent(impl->window);
    glfwSetWindowUserPointer(impl->window, this);
    glfwSetKeyCallback(impl->window, KeyCallback);
    glfwSetMouseButtonCallback(impl->window, MouseButtonCallback);

    // Initialize GLEW
    glewExperimental = 1;
    auto status = glewInit();
    if (status != GLEW_OK) {
        throw runtime_error("glewInit failed: " + string((const char *) (glewGetErrorString(status))));
    }

    rmt_BindOpenGL();

    // Initialize Nuklear
    impl->nuklear = nk_glfw3_init(impl->window, NK_GLFW3_DEFAULT);
    {
        nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        nk_glfw3_font_stash_end();
    }
}

phi::Window::~Window() {
    //rmt_UnbindOpenGL();
    glfwTerminate();
    //rmt_DestroyGlobalInstance(impl->rmt);
}

bool phi::Window::pumpEvents() {
    glfwPollEvents();
    nk_glfw3_new_frame();
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

void phi::Window::swap() {
    glfwSwapBuffers(impl->window);
}

nk_context *phi::Window::nuklear() {
    return impl->nuklear;
}

float phi::Window::aspect() {
    int w, h;
    glfwGetWindowSize(impl->window, &w, &h);
    return (float) w / (float) h;
}
