#include "Image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace testing_tool
{
Image::Image(std::vector<unsigned char> buffer, const ImageDimensions& size, const int nComponents)
    : m_buffer(std::move(buffer)),
    m_size(size),
    m_nComponents(nComponents)
{
    assert(m_buffer.size() == imagePixelCount(m_size, m_nComponents));
}

Image::Image(const unsigned char* buffer, const ImageDimensions& size, const int nComponents)
    : m_buffer(buffer, buffer + imagePixelCount(size, nComponents)),
      m_size(size),
      m_nComponents(nComponents)
{
}


const std::vector<unsigned char>& Image::GetBuffer() const
{
    return m_buffer;
}


ImageDimensions Image::GetSize() const
{
    return m_size;
}


int Image::GetComponents() const
{
    return m_nComponents;
}


bool equalExactly(const Image& first, const Image& second)
{
    return first.GetSize() == second.GetSize() && first.GetBuffer() == second.GetBuffer();
}

int imagePixelCount(const ImageDimensions& size, const int nComponents)
{
    assert(nComponents > 0);
    return size.GetWidth() * size.GetHeight() * nComponents;
}

void saveImage(const Image& image, const std::filesystem::path& filename)
{
    assert(std::filesystem::exists(filename.parent_path()));
    stbi_write_png(filename.string().data(), image.GetSize().GetWidth(), image.GetSize().GetHeight(), image.GetComponents(), image.GetBuffer().data(), image.GetSize().GetWidth() * image.GetComponents());
}

class StbiGuard
{
public:
	explicit StbiGuard(unsigned char* data)
		: m_data(data)
	{
	}
    ~StbiGuard()
	{
        stbi_image_free(m_data);
	}

	StbiGuard(const StbiGuard& other) = delete;
	StbiGuard(StbiGuard&& other) noexcept = delete;
	StbiGuard& operator=(const StbiGuard& other) = delete;
	StbiGuard& operator=(StbiGuard&& other) noexcept = delete;

	[[nodiscard]] unsigned char* data() const
	{
		return m_data;
	}

private:
    unsigned char* m_data = nullptr;
};

std::optional<Image> readImage(const std::filesystem::path& filename)
{
    if(std::filesystem::exists(filename) == false)
    {
        return std::nullopt;
    }
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    const std::unique_ptr<unsigned char, void(*)(void*)> data(stbi_load(filename.string().data(), &width, &height, &nrChannels, 0), stbi_image_free);
    if (data == nullptr)
    {
        return std::nullopt;
    }

    return Image{ data.get(), {width, height}, nrChannels };
}
}
