#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include "features.hpp"

namespace phi::gl {
    class ShaderStage final {
        friend class Shader;

        bool compiled;
        GLuint shader = 0;
    public:
        ShaderStage(GLenum type, const std::vector<std_string_view> &sources);

        ShaderStage(ShaderStage &&move) noexcept;

        ~ShaderStage();

        std::string infoLog() const;

        explicit operator bool() { return glIsShader(shader) && compiled; }
    };

    class Shader {
        bool linked;
        GLuint program = 0;
    public:
        explicit Shader(const std::vector<ShaderStage> &stages);

        Shader(Shader &&move) noexcept;

        virtual ~Shader();

        virtual void bind() const;

        std::string infoLog() const;

        explicit operator bool() { return glIsProgram(program) && linked; }
    };
}
