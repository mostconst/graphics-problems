#include "SnapshotChecker.h"
#include <filesystem>

namespace fs = std::filesystem;
#include "Image.h"


namespace testing_tool
{
ReferenceDetails::ReferenceDetails(const std::string& testSuiteName, const std::string& testName,
    const int snapshotIndex) : m_testSuiteName(testSuiteName),
    m_testName(testName),
    m_snapshotIndex(snapshotIndex)
{
}


const std::string& ReferenceDetails::GetTestSuiteName() const
{
    return m_testSuiteName;
}


const std::string& ReferenceDetails::GetTestName() const
{
    return m_testName;
}


int ReferenceDetails::GetSnapshotIndex() const
{
    return m_snapshotIndex;
}


SnapshotChecker::SnapshotChecker(const std::filesystem::path& referencesPath)
    : m_referencesPath(referencesPath)
{
}

SnapshotCheckResult SnapshotChecker::CheckSnapshot(const Image& image, const ReferenceDetails& referenceDetails) const
{
    const auto referenceImage = GetReferenceImage(referenceDetails);
    if (referenceImage == std::nullopt)
    {
        return SnapshotCheckResult::NoReference;
    }

    return equalExactly(*referenceImage, image) ? SnapshotCheckResult::Ok : SnapshotCheckResult::Mismatch;
}

std::optional<Image> SnapshotChecker::GetReferenceImage(const ReferenceDetails& referenceDetails) const
{
    return readImage(getSnapshotPath(m_referencesPath, referenceDetails));
}


TestDriver::TestDriver(const std::filesystem::path& referencePath, const std::filesystem::path& outputPath, const std::string& testSuiteName, const std::string& testName)
    : m_checker{ referencePath },
    m_testSuiteName{ testSuiteName },
    m_testName{testName},
m_outputPath{outputPath}
{
}


SnapshotCheckResult TestDriver::CheckSnapshot(const Image& image)
{
    const auto referenceDetails = ReferenceDetails{ m_testSuiteName, m_testName, m_snapshotCounter++ };
    const auto result = m_checker.CheckSnapshot(image, referenceDetails);
    if (result != SnapshotCheckResult::Ok)
    {
        const auto snapshotPath = getSnapshotPath(m_outputPath, referenceDetails);
        std::filesystem::create_directories(snapshotPath.parent_path());
        saveImage(image, snapshotPath);
    }

    return result;
}
}
