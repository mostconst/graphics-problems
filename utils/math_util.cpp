#include "math_util.h"


#include "glm/fwd.hpp"
#include "glm/vec4.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 rowMajorMatrix(glm::vec4 row1, glm::vec4 row2, glm::vec4 row3, glm::vec4 row4)
{
    return glm::transpose(glm::mat4(row1, row2, row3, row4));
}
