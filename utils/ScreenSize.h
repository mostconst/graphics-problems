#pragma once

namespace nsk_cg
{
struct ScreenSize final
{
    ScreenSize(int width, int height);

    const int m_width;
    const int m_height;
};

inline ScreenSize::ScreenSize(const int width, const int height)
    : m_width(width),
      m_height(height)
{
}
}
