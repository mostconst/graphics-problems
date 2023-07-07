#include "Image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


namespace nsk_cg
{
Image::Image(std::vector<unsigned char> buffer, const ScreenSize& size, const int nComponents)
    : m_buffer(std::move(buffer)),
    m_size(size),
    m_nComponents(nComponents)
{
}


const std::vector<unsigned char>& Image::GetBuffer() const
{
    return m_buffer;
}


ScreenSize Image::GetSize() const
{
    return m_size;
}


int Image::GetComponents() const
{
    return m_nComponents;
}


void saveImage(const Image& image, const std::filesystem::path& filename)
{
    assert(std::filesystem::exists(filename.parent_path()));
    stbi_write_png(filename.string().data(), image.GetSize().GetWidth(), image.GetSize().GetHeight(), image.GetComponents(), image.GetBuffer().data(), image.GetSize().GetWidth() * image.GetComponents());
}
}
