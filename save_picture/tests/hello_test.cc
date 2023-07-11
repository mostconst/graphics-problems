#include <gtest/gtest.h>
#include <filesystem>

#include "Framebuffer.h"
#include "glfw_utils.h"
#include "ScreenSize.h"
#include "Texture.h"
#include "SnapshotChecker.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace fs = std::filesystem;

const testing::TestInfo& getCurrentTestInfo()
{
    const auto instance = ::testing::UnitTest::GetInstance();
    assert(instance);
    const auto testInfo = instance->current_test_info();
    assert(testInfo);
    return *testInfo;
}

class TestDriver
{
public:
    TestDriver(const std::string& testSuiteName, const std::string& testName)
        : m_testSuiteName(testSuiteName),
          m_testName(testName)
    {
    }
    void TakeSnapshot(const nsk_cg::Image& image);

private:
    SnapshotChecker m_checker{ "D:\\temp\\references",  "D:\\temp\\output" };
    std::string m_testSuiteName;
    std::string m_testName;
    int m_snapshotCounter = 0;
};

void takeSnapshot(TestDriver& checker, const nsk_cg::Texture& colorBuffer)
{
    glFinish();
    auto textureData = colorBuffer.GetData();
    const nsk_cg::Image image(textureData, colorBuffer.GetSize(), nsk_cg::textureFormatSize(colorBuffer.GetType()));
    checker.TakeSnapshot(image);
}

// Demonstrate some basic assertions.
TEST(VisualTest, CreatingWindow) {
    int windowWidth = 800;
    int windowHeight = 600;
    const auto& testInfo = getCurrentTestInfo();
    TestDriver checker(testInfo.test_suite_name(), testInfo.name());
    const auto window = nsk_cg::makeMinimalWindow({ windowWidth, windowHeight }, "TestWindow");
    EXPECT_TRUE(window != nullptr);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    nsk_cg::Texture colorTexture(nsk_cg::TextureFormat::Color, windowWidth, windowHeight);
    nsk_cg::Texture depthTexture(nsk_cg::TextureFormat::Depth, windowWidth, windowHeight);
    nsk_cg::Framebuffer framebuffer;
    framebuffer.Attach(colorTexture, depthTexture);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    takeSnapshot(checker, colorTexture);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    takeSnapshot(checker, colorTexture);

    glfwTerminate();
}

TEST(ImageTest, ReadWorks)
{
	//nsk_cg::readImage("D:\\")
    ASSERT_TRUE(true);
}