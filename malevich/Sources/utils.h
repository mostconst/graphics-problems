#pragma once
#include <vector>


struct SimpleImage
{
public:
    SimpleImage(const std::vector<unsigned char>& buffer, const int width, const int height, const int components)
        : m_buffer(buffer),
          m_width(width),
          m_height(height),
          m_components(components)
    {
    }


    std::vector<unsigned char> GetBuffer() const {return m_buffer; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetComponents() const { return m_components; }

private:
    std::vector<unsigned char> m_buffer;
    int m_width;
    int m_height;
    int m_components;
};

struct GLFWwindow;
class MalevichCore
{
public:
    MalevichCore(int windowWidth, int windowHeight);
    ~MalevichCore();
    MalevichCore(const MalevichCore& other) = delete;
    MalevichCore(MalevichCore&& other) noexcept = delete;
    MalevichCore& operator=(const MalevichCore& other) = delete;
    MalevichCore& operator=(MalevichCore&& other) noexcept = delete;

    GLFWwindow* GetWindow() const { return m_window; }
    void SetColor(float r, float g, float b);
    void Draw();
    SimpleImage DrawToBuffer();
private:
    GLFWwindow* m_window;
    int windowWidth;
    int windowHeight;
};