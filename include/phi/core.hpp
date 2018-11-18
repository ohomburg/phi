#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace phi {
    struct Transform {
        glm::vec3 position{0, 0, 0}, scale{1, 1, 1};
        glm::quat rotation{glm::identity<glm::quat>()};

        glm::mat4 transform() const;
    };
}