#pragma once
#include <filesystem>
#include <vector>

#include "ScreenSize.h"

namespace nsk_cg
{
class Image
{
public:
    Image(std::vector<unsigned char> buffer, const ScreenSize& size, const int nComponents);
    const std::vector<unsigned char>& GetBuffer() const;
    ScreenSize GetSize() const;
    int GetComponents() const;

private:
    std::vector<unsigned char> m_buffer;
    ScreenSize m_size;
    int m_nComponents;
};

void saveImage(const Image& image, const std::filesystem::path& filename);
}
