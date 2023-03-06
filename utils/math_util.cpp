#include "math_util.h"


#include "glm/fwd.hpp"
#include "glm/vec4.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace math_utils
{
	glm::mat4 rowMajorMatrix(glm::vec4 row1, glm::vec4 row2, glm::vec4 row3, glm::vec4 row4)
	{
		return glm::transpose(glm::mat4(row1, row2, row3, row4));
	}

	glm::mat4 perspective(float width, float height, float near, float far)
	{
        const float halfWidth = width / 2.0f;
        const float halfHeight = height / 2.0f;
	    const float a = (far + near) / (far - near);
        const float b = 2 * far * near / (far - near);
		return rowMajorMatrix(
			{near / halfWidth, 0.0f, 0.0f, 0.0f},
			{ 0.0f, near / halfHeight, 0.0f, 0.0f },
			{ 0.0f, 0.0f, -a, -b },
			{ 0.0f, 0.0f, -1.0f, 0.0f }
		);
	}

	// HW_ITEM 2
    glm::mat4 perspectiveFov(float fov, float ratio, float near, float far)
    {
		const float halfFov = fov / 2;
		const float height = glm::tan(halfFov) * near * 2;
		const float width = height * ratio;
		return perspective(width, height, near, far);
    }
}
