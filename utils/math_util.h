#pragma once
#include "glm/fwd.hpp"

namespace nsk_cg::math_utils
{
    glm::mat4 rowMajorMatrix(glm::vec4 row1, glm::vec4 row2, glm::vec4 row3, glm::vec4 row4);
    glm::mat4 perspective(float width, float height, float near, float far);
    glm::mat4 perspectiveFov(float fov, float ratio, float near, float far);
}
