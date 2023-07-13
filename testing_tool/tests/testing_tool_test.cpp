#include <gtest/gtest.h>

#include "Image.h"
#include "SnapshotChecker.h"

namespace testing_tool
{
struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};
Image makeSingleColorImage(const ImageDimensions& size, const Color& color)
{
    std::vector<unsigned char> buffer;
    constexpr int components = 3;
    buffer.reserve(static_cast<size_t>(size.GetWidth() * size.GetHeight() * components));
    for (int col = 0; col < size.GetWidth(); ++col)
    {
        for (int row = 0; row < size.GetHeight(); ++row)
        {
            buffer.push_back(color.r);
            buffer.push_back(color.g);
            buffer.push_back(color.b);
        }
    }

    return Image(std::move(buffer), size, components);
}
}

namespace fs = std::filesystem;

class ImageRWTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        fs::create_directories(m_testPath);
    }

    void TearDown() override
    {
        fs::remove_all(m_testPath);
    }
    fs::path m_testPath = fs::temp_directory_path() / "1496bec4-14d1-46b5-a70f-cb0f12e31699";
};

class SnapshotsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        fs::create_directories(m_testPath / m_testSuiteName / m_testName);
        testing_tool::saveImage(snapshot0, testing_tool::getSnapshotPath(m_testPath, {m_testSuiteName, m_testName, 0}));
        testing_tool::saveImage(snapshot1, testing_tool::getSnapshotPath(m_testPath, { m_testSuiteName, m_testName, 1}));
    }

    void TearDown() override
    {
        fs::remove_all(m_testPath);
    }
    fs::path m_testPath = fs::temp_directory_path() / "176bca54-0aff-4940-a3bd-587184d417b6";
    std::string m_testSuiteName = "Suite";
    std::string m_testName = "Test";
    testing_tool::Image snapshot0 = testing_tool::makeSingleColorImage({ 32, 32 }, { 255, 0, 0 });
    testing_tool::Image snapshot1 = testing_tool::makeSingleColorImage({ 32, 32 }, { 0, 255, 0 });
};

// Demonstrate some basic assertions.
TEST_F(ImageRWTest, WriteAndRead)
{
    const auto image = testing_tool::makeSingleColorImage({ 10, 10 }, { 255, 0, 0 });
    const auto imagePath = m_testPath / "img.png";
    testing_tool::saveImage(image, imagePath);
    EXPECT_TRUE(fs::exists(imagePath));
    const auto savedImage = testing_tool::readImage(imagePath);
    EXPECT_TRUE(savedImage);
    EXPECT_TRUE(testing_tool::equalExactly(image, *savedImage));
}

TEST_F(SnapshotsTest, CheckerWorks)
{
    const testing_tool::SnapshotChecker checker{ m_testPath };
    EXPECT_EQ(checker.CheckSnapshot(snapshot0, { m_testSuiteName, m_testName, 0 }), testing_tool::SnapshotCheckResult::Ok);
    EXPECT_EQ(checker.CheckSnapshot(snapshot0, { m_testSuiteName, m_testName, 1 }), testing_tool::SnapshotCheckResult::Mismatch);
    EXPECT_EQ(checker.CheckSnapshot(snapshot0, { m_testSuiteName, m_testName, 3 }), testing_tool::SnapshotCheckResult::NoReference);
}