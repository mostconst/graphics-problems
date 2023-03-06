#include "Color.h"

#include <cassert>

utils::Color::Color(uint32_t color)
    : red(static_cast<float>((color >> 16) & 0xFF) / 0xFF),
      green(static_cast<float>((color >> 8) & 0xFF) / 0xFF),
      blue(static_cast<float>((color >> 0) & 0xFF) / 0xFF)
{
    assert(color <= 0xFFFFFF);
}

utils::Color::Color(float red, float green, float blue)
    : red(red),
      green(green),
      blue(blue)
{
}
