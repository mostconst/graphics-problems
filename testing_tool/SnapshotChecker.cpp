#include "SnapshotChecker.h"

#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;
#include "Image.h"


namespace testing_tool
{
ReferenceDetails::ReferenceDetails(std::string testSuiteName, std::string testName, const int snapshotIndex)
    : m_testSuiteName{std::move(testSuiteName)},
    m_testName{std::move(testName)},
    m_snapshotIndex{ snapshotIndex }
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


SnapshotChecker::SnapshotChecker(std::filesystem::path referencesPath)
    : m_referencesPath(std::move(referencesPath))
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


std::filesystem::path getSnapshotPath(const std::filesystem::path& rootPath, const ReferenceDetails& referenceDetails)
{
    return rootPath / referenceDetails.GetTestSuiteName() / referenceDetails.GetTestName() / (std::to_string(referenceDetails.GetSnapshotIndex()) + ".png");
}


std::filesystem::path getTestPath(const std::filesystem::path& rootPath, const std::string& suiteName,
    const std::string& testName)
{
    return rootPath / suiteName / testName;
}


TestDriver::TestDriver(const std::filesystem::path& sourceDirectory, const std::filesystem::path& binaryDirectory,
                       std::string testSuiteName, std::string testName)
    : m_checker{sourceDirectory},
    m_testSuiteName{std::move(testSuiteName)},
    m_testName{std::move(testName)},
    m_outputPath{binaryDirectory}
{
    assert(sourceDirectory != binaryDirectory);
    fs::remove_all(getTestPath(m_outputPath, m_testSuiteName, m_testName));
}


SnapshotCheckResult TestDriver::CheckSnapshot(const Image& image, const int snapshotIndex) const
{
    const auto referenceDetails = ReferenceDetails{ m_testSuiteName, m_testName, snapshotIndex };
    const auto result = m_checker.CheckSnapshot(image, referenceDetails);
    if (result != SnapshotCheckResult::Ok)
    {
        const auto snapshotPath = getSnapshotPath(m_outputPath, referenceDetails);
        fs::create_directories(snapshotPath.parent_path());
        saveImage(image, snapshotPath);
    }

    return result;
}

std::wstring resultLogDescription(const SnapshotCheckResult result)
{
    switch (result)
    {
    case testing_tool::SnapshotCheckResult::Ok:
        return L"OK";
    case testing_tool::SnapshotCheckResult::Mismatch:
        return L"Snapshot doesn't match reference";
    case testing_tool::SnapshotCheckResult::NoReference:
        return L"No reference image";
    }

    assert(false);
    return L"Unknown test result";
}

std::wstring toReadableString(fs::path path)
{
    return path.make_preferred().native();
}

std::wstring TestDriver::GetResultLogString(const SnapshotCheckResult result, const int snapshotIndex) const
{
    return resultLogDescription(result) + L'\n' +
        L"Reference: " + toReadableString(GetReferencePath(snapshotIndex)) + L'\n' + 
        L"Snapshot: " + toReadableString(GetSnapshotPath(snapshotIndex));
}

std::filesystem::path TestDriver::GetReferencePath(const int snapshotIndex) const
{
    return getSnapshotPath(m_checker.GetReferencesPath(), { m_testSuiteName, m_testName, snapshotIndex });
}


std::filesystem::path TestDriver::GetSnapshotPath(int snapshotIndex) const
{
    return getSnapshotPath(m_outputPath, { m_testSuiteName, m_testName, snapshotIndex });
}
}
