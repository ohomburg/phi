#include <phi/render.hpp>
#include <GL/glew.h>
#include <Remotery.h>
#include <phi/opengl.hpp>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include <nuklear/nuklear.h>
#include <nuklear/demo/glfw_opengl3/nuklear_glfw_gl3.h>

using namespace std;
using namespace phi;
using namespace phi::render;
using namespace phi::gl;

static vector<ShaderStage> mk_shader(std_string_view vs, std_string_view fs) {
    vector<ShaderStage> output;
    output.push_back(ShaderStage(GL_VERTEX_SHADER, vector<std_string_view>{vs}));
    if (!output.back()) { throw runtime_error("Shader compilation failed:\n" + output.back().infoLog()); }
    output.push_back(ShaderStage(GL_FRAGMENT_SHADER, vector<std_string_view>{fs}));
    if (!output.back()) { throw runtime_error("Shader compilation failed:\n" + output.back().infoLog()); }
    return output;
}

static constexpr std_string_view simple_vs = R"EOF(
#version 430

layout(location = 0) in vec2 in_position;

void main() {
    gl_Position = vec4(in_position * 0.5, 0, 1);
}

)EOF"sv, simple_fs = R"EOF(
#version 430

out vec3 color;

void main() {
    color = vec3(1, 0, 0);
}
)EOF"sv;

class Quad {
    GLuint VBO{}, VAO{};
public:
    Quad() {
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        const GLfloat vertex_data[] = {
                1, 1, -1, 1, 1, -1, -1, -1
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *) nullptr);
        glEnableVertexAttribArray(0);
    }

    void drawCall() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
};

class SimpleShader : public Shader {
public:
    SimpleShader() : Shader(mk_shader(simple_vs, simple_fs)) {}
};

struct Compositor::Impl {
    explicit Impl(Window &window) : window(window) {
        if (!simpleShader) throw runtime_error("Shader compile failed:\n" + simpleShader.infoLog());
    }

    Window &window;
    SimpleShader simpleShader;
    Quad quad;
};

void Compositor::render() {
    rmt_ScopedCPUSample(Render, 0);
    rmt_ScopedOpenGLSample(Render);
    glClearColor(0, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_BLEND);

    impl->simpleShader.bind();
    impl->quad.drawCall();

    const int MAX_VERTEX_BUFFER = 512 * 1024, MAX_ELEMENT_BUFFER = 128 * 1024;
    nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);

    impl->window.swap();
}

void Compositor::handle(const phi::event::Event &ev) {
}

Compositor::Compositor(Window &window) : impl(make_unique<Impl>(window)) {
}

Compositor::~Compositor() = default;
