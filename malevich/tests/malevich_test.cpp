#include <gtest/gtest.h>
#include <filesystem>

#include "glfw_utils.h"
#include "ImageDimensions.h"
#include "Image.h"
#include "SnapshotChecker.h"
#include "utils.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "test_paths.h"

const testing::TestInfo& getCurrentTestInfo()
{
    const auto instance = ::testing::UnitTest::GetInstance();
    assert(instance);
    const auto testInfo = instance->current_test_info();
    assert(testInfo);
    return *testInfo;
}

testing_tool::Image getImage(const SimpleImage& colorBuffer)
{
    return testing_tool::Image{ colorBuffer.GetBuffer(), {colorBuffer.GetWidth(), colorBuffer.GetHeight()}, colorBuffer.GetComponents() };
}

testing::AssertionResult checkSnapshot(const testing_tool::TestDriver& driver, const testing_tool::Image& snapshot, const int snapshotIndex)
{
    switch(const auto result = driver.CheckSnapshot(snapshot, snapshotIndex))
    {
    case testing_tool::SnapshotCheckResult::Ok:
        return testing::AssertionSuccess();
    case testing_tool::SnapshotCheckResult::Mismatch:
    case testing_tool::SnapshotCheckResult::NoReference:
        return testing::AssertionFailure() << '\n' << driver.GetResultLogString(result, snapshotIndex);
    }

    assert(false);
    return testing::AssertionFailure() << "Unknown test result";
}

// Demonstrate some basic assertions.
TEST(MalevichTest, ClassicSquare) {
    const auto& testInfo = getCurrentTestInfo();
    testing_tool::TestDriver driver{references_path, output_path, testInfo.test_suite_name(), testInfo.name() };
    constexpr int windowWidth = 800;
    MalevichCore app(windowWidth);
    EXPECT_TRUE(checkSnapshot(driver, getImage(app.DrawToBuffer()), 0));
}

// Demonstrate some basic assertions.
TEST(MalevichTest, DifferentBackgrounds) {
    const auto& testInfo = getCurrentTestInfo();
    testing_tool::TestDriver driver{ references_path, output_path, testInfo.test_suite_name(), testInfo.name() };
    constexpr int windowWidth = 800;
    MalevichCore app(windowWidth);
    app.SetBackground(Color{ 1.0f, 0.0f, 0.0f });
    EXPECT_TRUE(checkSnapshot(driver, getImage(app.DrawToBuffer()), 0));
    app.SetBackground(Color{ 0.0f, 1.0f, 0.0f });
    EXPECT_TRUE(checkSnapshot(driver, getImage(app.DrawToBuffer()), 1));
}