#include <phi/render.hpp>
#include <phi/engine.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <Remotery.h>
#include <phi/opengl.hpp>
#include <fstream>
#include <string>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include <nuklear/nuklear.h>
#include <nuklear/demo/glfw_opengl3/nuklear_glfw_gl3.h>
#include <iostream>

using std::vector;
using namespace std::literals;
using namespace phi;
using namespace phi::render;
using namespace phi::gl;

static ShaderStage load_shader_stage(const std::string &filename) {
    std::string ext = filename.substr(filename.find_last_of('.') + 1);
    GLenum type;
    if (ext == "vert")
        type = GL_VERTEX_SHADER;
    else if (ext == "frag")
        type = GL_FRAGMENT_SHADER;
    else if (ext == "geom")
        type = GL_GEOMETRY_SHADER;
    else if (ext == "comp")
        type = GL_COMPUTE_SHADER;
    else if (ext == "tesc")
        type = GL_TESS_CONTROL_SHADER;
    else if (ext == "tese")
        type = GL_TESS_EVALUATION_SHADER;
    else
        type = GL_VERTEX_SHADER;

    std::ifstream ifs(g_assets + "/shader/"s + filename);
    std::string contents{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};

    return ShaderStage{type, std::vector{std_string_view(contents)}};
}

static vector<ShaderStage> load_shader(const std::string &name) {
    vector<ShaderStage> output;
    output.push_back(load_shader_stage(name + ".vert"));
    output.push_back(load_shader_stage(name + ".frag"));
    return output;
}

struct Compositor::Impl {
    explicit Impl(Window &window) : window(window), shader(load_shader("solid")) {
        if (!shader) throw std::runtime_error("Shader compile failed:\n" + shader.infoLog());
    }

    Window &window;
    Shader shader;
    Cube cube;
};

void Compositor::render() {
    {
        rmt_ScopedCPUSample(Render, 0);
        rmt_ScopedOpenGLSample(Render);
        glClearColor(0, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);

        impl->shader.bind();
        glm::mat4 model(1.f);
        glm::vec3 eye{0, 0, 5};
        glm::vec3 center{0, 0, 0};
        glm::mat4 cameraTransform = glm::translate(glm::mat4(1.f), eye);
        glm::mat4 viewProject =
                glm::perspective(glm::radians(68.f), impl->window.aspect(), 0.1f, 1000.f) * glm::inverse(cameraTransform);
        //std::cout << glm::to_string(viewProject) << '\n';
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(viewProject));

        impl->cube.drawCall();

        const int MAX_VERTEX_BUFFER = 512 * 1024, MAX_ELEMENT_BUFFER = 128 * 1024;
        nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    }

    impl->window.swap();
}

void Compositor::handle(const phi::event::Event &ev) {
}

Compositor::Compositor(Window &window) : impl(std::make_unique<Impl>(window)) {
}

Compositor::~Compositor() = default;
