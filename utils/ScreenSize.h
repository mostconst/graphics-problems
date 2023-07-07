#pragma once

namespace nsk_cg
{
class ScreenSize final
{
public:
    ScreenSize(int width, int height);
    int GetWidth() const;
    int GetHeight() const;

private:
    int m_width;
    int m_height;
};

inline ScreenSize::ScreenSize(const int width, const int height)
    : m_width(width),
      m_height(height)
{
}


inline int ScreenSize::GetWidth() const
{
    return m_width;
}


inline int ScreenSize::GetHeight() const
{
    return m_height;
}
}
