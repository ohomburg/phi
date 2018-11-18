#include <phi/opengl.hpp>
#include <memory>
#include <Remotery.h>

static WARN_UNUSED(bool) compileShader(GLuint shader) {
    rmt_ScopedOpenGLSample(ShaderCompile)
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    return success == GL_TRUE;
}

static WARN_UNUSED(bool) linkProgram(GLuint program) {
    rmt_ScopedOpenGLSample(ShaderLink)
    glLinkProgram(program);

    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    return success == GL_TRUE;
}

phi::gl::ShaderStage::ShaderStage(GLenum type, const std::vector<std_string_view> &sources) {
    shader = glCreateShader(type);
    auto num = sources.size();
    std::vector<GLint> lengths(num);
    std::vector<const char *> pointers(num);
    for (size_t i = 0; i < num; ++i) {
        lengths[i] = (GLint) sources[i].length();
        pointers[i] = sources[i].data();
    }
    glShaderSource(shader, (GLsizei) num, pointers.data(), lengths.data());
    compiled = compileShader(shader);
}

phi::gl::ShaderStage::ShaderStage(ShaderStage &&move) noexcept {
    shader = move.shader;
    compiled = move.compiled;
    move.shader = 0;
    move.compiled = false;
}

phi::gl::ShaderStage::~ShaderStage() {
    glDeleteShader(shader);
}

std::string phi::gl::ShaderStage::infoLog() const {
    int len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    std::string log((size_t) len, '#');
    glGetShaderInfoLog(shader, len + 1, &len, log.data());
    return log;
}

phi::gl::Shader::Shader(const std::vector<ShaderStage> &stages) {
    program = glCreateProgram();
    for (auto &&stage : stages) glAttachShader(program, stage.shader);
    linked = linkProgram(program);
    for (auto &&stage : stages) glDetachShader(program, stage.shader);
}

phi::gl::Shader::Shader(Shader &&move) noexcept {
    program = move.program;
    linked = move.linked;
    move.program = 0;
    move.linked = false;
}

void phi::gl::Shader::bind() const {
    glUseProgram(program);
}

std::string phi::gl::Shader::infoLog() const {
    int len;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    auto log = std::make_unique<char[]>((size_t) len + 1);
    glGetProgramInfoLog(program, len + 1, &len, log.get());
    return std::string(log.get(), (size_t) len);
}

phi::gl::Shader::~Shader() {
    glDeleteProgram(program);
}
