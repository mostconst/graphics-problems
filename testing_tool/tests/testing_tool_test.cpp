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
nsk_cg::Image makeSingleColorImage(const nsk_cg::ImageDimensions& size, const Color& color)
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

    return nsk_cg::Image(std::move(buffer), size, components);
}

namespace fs = std::filesystem;

class ImageRWTest : public testing::Test
{
protected:
    void SetUp() override
    {
        create_directories(m_testPath);
    }

    void TearDown() override
    {
        remove_all(m_testPath);
    }
    fs::path m_testPath = fs::temp_directory_path() / "1496bec4-14d1-46b5-a70f-cb0f12e31699";
};

class SnapshotsTest : public testing::Test
{
protected:
    void SetUp() override
    {
        create_directories(m_referencePath / m_testSuiteName / m_testName);
        nsk_cg::saveImage(reference0, getSnapshotPath(m_referencePath, { m_testSuiteName, m_testName, 0 }));
        saveImage(reference1, getSnapshotPath(m_referencePath, { m_testSuiteName, m_testName, 1 }));

        create_directories(m_outputPath / m_testSuiteName / m_testName);
        saveImage(reference0, getSnapshotPath(m_outputPath, { m_testSuiteName, m_testName, 0 }));
    }

    void TearDown() override
    {
        remove_all(m_testRoot);
    }
    fs::path m_testRoot = fs::temp_directory_path() / "176bca54-0aff-4940-a3bd-587184d417b6";
    fs::path m_referencePath = m_testRoot / "references";
    fs::path m_outputPath = m_testRoot / "output";
    std::string m_testSuiteName = "Suite";
    std::string m_testName = "Test";
    nsk_cg::Image reference0 = makeSingleColorImage({ 32, 32 }, { 255, 0, 0 });
    nsk_cg::Image reference1 = makeSingleColorImage({ 32, 32 }, { 0, 255, 0 });
};

// Demonstrate some basic assertions.
TEST_F(ImageRWTest, WriteAndRead)
{
    const auto image = makeSingleColorImage({ 10, 10 }, { 255, 0, 0 });
    const auto imagePath = m_testPath / "img.png";
    saveImage(image, imagePath);
    EXPECT_TRUE(fs::exists(imagePath));
    const auto savedImage = nsk_cg::readImage(imagePath);
    EXPECT_TRUE(savedImage);
    EXPECT_TRUE(nsk_cg::equalExactly(image, *savedImage));
}

TEST_F(SnapshotsTest, CheckerWorks)
{
    const SnapshotChecker checker{ m_referencePath };
    EXPECT_EQ(checker.CheckSnapshot(reference0, { m_testSuiteName, m_testName, 0 }), testing_tool::SnapshotCheckResult::Ok);
    EXPECT_EQ(checker.CheckSnapshot(reference0, { m_testSuiteName, m_testName, 1 }), testing_tool::SnapshotCheckResult::Mismatch);
    EXPECT_EQ(checker.CheckSnapshot(reference0, { m_testSuiteName, m_testName, 3 }), testing_tool::SnapshotCheckResult::NoReference);
}

std::optional<nsk_cg::Image> readSnapshot(const std::filesystem::path& root, const ReferenceDetails& referenceDetails)
{
    const auto snapshotPath = getSnapshotPath(root, referenceDetails);
    return nsk_cg::readImage(snapshotPath);
}

TEST_F(SnapshotsTest, DriverCleansUpPreviousOutput)
{
    const auto prevResults = getTestPath(m_outputPath, m_testSuiteName, m_testName);
    ASSERT_TRUE(fs::exists(prevResults));
    TestDriver driver(m_referencePath, m_outputPath, m_testSuiteName, m_testName);
    EXPECT_FALSE(fs::exists(prevResults));
}

TEST_F(SnapshotsTest, DriverWorks)
{
    TestDriver driver(m_referencePath, m_outputPath, m_testSuiteName, m_testName);
    EXPECT_EQ(driver.CheckSnapshot(reference0, 0), testing_tool::SnapshotCheckResult::Ok);
    EXPECT_EQ(driver.CheckSnapshot(reference0, 1), testing_tool::SnapshotCheckResult::Mismatch);
    EXPECT_EQ(driver.CheckSnapshot(reference1, 2), testing_tool::SnapshotCheckResult::NoReference);
    const auto snapshotActual1 = readSnapshot(m_outputPath, { m_testSuiteName, m_testName, 1 });
    EXPECT_TRUE(snapshotActual1);
    EXPECT_TRUE(equalExactly(reference0, *snapshotActual1));
    const auto snapshotActual2 = readSnapshot(m_outputPath, { m_testSuiteName, m_testName, 2 });
    EXPECT_TRUE(snapshotActual2);
    EXPECT_TRUE(equalExactly(reference1, *snapshotActual2));
}
}