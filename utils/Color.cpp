#include "Color.h"

#include <cassert>

namespace nsk_cg::utils
{
Color::Color(const uint32_t color)
    : red(static_cast<float>((color >> 16) & 0xFF) / 0xFF),
    green(static_cast<float>((color >> 8) & 0xFF) / 0xFF),
    blue(static_cast<float>((color >> 0) & 0xFF) / 0xFF)
{
    assert(color <= 0xFFFFFF);
}

Color::Color(const float red, const float green, const float blue)
    : red(red),
    green(green),
    blue(blue)
{
}

float Color::GetRed() const
{
    return red;
}

float Color::GetGreen() const
{
    return green;
}

float Color::GetBlue() const
{
    return blue;
}

size_t Color::GetComponentSize()
{
    return sizeof(decltype(red));
}

glm::vec3 colorAsVector(const Color& color)
{
    return { color.GetRed(), color.GetGreen(), color.GetBlue() };
}
}
