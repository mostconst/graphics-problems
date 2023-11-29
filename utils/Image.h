#pragma once
#include <filesystem>
#include <vector>
#include <optional>

#include "ImageDimensions.h"

namespace nsk_cg
{
class Image
{
public:
    Image(std::vector<unsigned char> buffer, const ImageDimensions& size, int nComponents);
    Image(const unsigned char* buffer, const ImageDimensions& size, int nComponents);
    const std::vector<unsigned char>& GetBuffer() const;
    ImageDimensions GetSize() const;
    int GetComponents() const;

private:
    std::vector<unsigned char> m_buffer;
    ImageDimensions m_size;
    int m_nComponents;
};

bool equalExactly(const Image& first, const Image& second);

int imagePixelCount(const ImageDimensions& size, int nComponents);

void saveImage(const Image& image, const std::filesystem::path& filename);
std::optional<Image> readImage(const std::filesystem::path& filename);
}
