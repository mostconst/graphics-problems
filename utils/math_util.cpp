#include "math_util.h"


#include "glm/fwd.hpp"
#include "glm/vec4.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace nsk_cg::math_utils
{
	glm::mat4 rowMajorMatrix(const glm::vec4 row1, const glm::vec4 row2, const glm::vec4 row3, const glm::vec4 row4)
	{
		return glm::transpose(glm::mat4(row1, row2, row3, row4));
	}

	glm::mat4 perspective(const float width, const float height, const float near, const float far)
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
    glm::mat4 perspectiveFov(const float fov, const float ratio, const float near, const float far)
    {
		const float halfFov = fov / 2;
		const float height = glm::tan(halfFov) * near * 2;
		const float width = height * ratio;
		return perspective(width, height, near, far);
    }
}
