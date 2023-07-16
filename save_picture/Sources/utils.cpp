#include <vector>

#include "glad/glad.h"

void setColor(const float r, const float g, const float b)
{
	glClearColor(r, g, b, 1.0);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::vector<unsigned char> drawToBuffer()
{
	draw();
	glFinish();
	std::vector<unsigned char> buffer(800 * 600 * 3);
	glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

	return buffer;
}