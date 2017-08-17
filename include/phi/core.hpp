#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace phi {
    struct Transform {
        glm::vec3 position, scale;
        glm::quat rotation;
    };
}