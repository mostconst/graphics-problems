#pragma once

namespace nsk_cg
{
class ImageDimensions final
{
public:
    ImageDimensions(int width, int height);
    int GetWidth() const;
    int GetHeight() const;

private:
    int m_width;
    int m_height;
};

inline ImageDimensions::ImageDimensions(const int width, const int height)
    : m_width(width),
      m_height(height)
{
}


inline int ImageDimensions::GetWidth() const
{
    return m_width;
}


inline int ImageDimensions::GetHeight() const
{
    return m_height;
}

inline bool operator==(const ImageDimensions& lhs, const ImageDimensions& rhs)
{
    return lhs.GetWidth() == rhs.GetWidth()
        && lhs.GetHeight() == rhs.GetHeight();
}

inline bool operator!=(const ImageDimensions& lhs, const ImageDimensions& rhs)
{
    return !(lhs == rhs);
}
}
