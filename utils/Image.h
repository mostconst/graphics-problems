#pragma once
#include <filesystem>
#include <vector>
#include <optional>

#include "ScreenSize.h"

namespace nsk_cg
{
class Image
{
public:
    Image(std::vector<unsigned char> buffer, const ScreenSize& size, int nComponents);
    Image(const unsigned char* buffer, const ScreenSize& size, int nComponents);
    const std::vector<unsigned char>& GetBuffer() const;
    ScreenSize GetSize() const;
    int GetComponents() const;

private:
    std::vector<unsigned char> m_buffer;
    ScreenSize m_size;
    int m_nComponents;
};

bool equalExactly(const Image& first, const Image& second);

int imagePixelCount(const ScreenSize& size, int nComponents);

void saveImage(const Image& image, const std::filesystem::path& filename);
std::optional<Image> readImage(const std::filesystem::path& filename);
}
