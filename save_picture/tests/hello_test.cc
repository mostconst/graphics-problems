#include <gtest/gtest.h>
#include <filesystem>

#include "Framebuffer.h"
#include "glfw_utils.h"
#include "ImageDimensions.h"
#include "Texture.h"
#include "Image.h"
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

testing_tool::Image getImage(const nsk_cg::Texture& colorBuffer)
{
    glFinish();
    const auto textureData = colorBuffer.GetData();
    auto screenSize = colorBuffer.GetSize();
    return testing_tool::Image{ textureData, {screenSize.GetWidth(), screenSize.GetHeight()}, nsk_cg::textureFormatSize(colorBuffer.GetType()) };
}

// Demonstrate some basic assertions.
TEST(VisualTest, CreatingWindow) {
    int windowWidth = 800;
    int windowHeight = 600;
    const auto& testInfo = getCurrentTestInfo();
    testing_tool::TestDriver checker{"D:\\temp\\references", "D:\\temp\\output", testInfo.test_suite_name(), testInfo.name() };
    const auto window = nsk_cg::makeMinimalWindow({ windowWidth, windowHeight }, "TestWindow");
    EXPECT_TRUE(window != nullptr);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    nsk_cg::Texture colorTexture(nsk_cg::TextureFormat::Color, windowWidth, windowHeight);
    nsk_cg::Texture depthTexture(nsk_cg::TextureFormat::Depth, windowWidth, windowHeight);
    nsk_cg::Framebuffer framebuffer;
    framebuffer.Attach(colorTexture, depthTexture);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    EXPECT_EQ(checker.CheckSnapshot(getImage(colorTexture)), testing_tool::SnapshotCheckResult::Ok) << "Run command";
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    EXPECT_EQ(checker.CheckSnapshot(getImage(colorTexture)), testing_tool::SnapshotCheckResult::Ok);

    glfwTerminate();
}