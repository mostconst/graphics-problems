#pragma once
#include <filesystem>
#include <optional>
#include <string>

#include "Image.h"

namespace testing_tool
{
class Image;
enum class SnapshotCheckResult
{
    Ok,
    Mismatch,
    NoReference
};

class ReferenceDetails
{
public:
    ReferenceDetails(const std::string& testSuiteName, const std::string& testName, const int snapshotIndex);
    const std::string& GetTestSuiteName() const;
    const std::string& GetTestName() const;
    int GetSnapshotIndex() const;

private:
    std::string m_testSuiteName;
    std::string m_testName;
    int m_snapshotIndex;
};

class SnapshotChecker
{
public:
    SnapshotChecker(const std::filesystem::path& referencesPath);
    SnapshotCheckResult CheckSnapshot(const Image& image, const ReferenceDetails& referenceDetails) const;
    std::optional<Image> GetReferenceImage(const ReferenceDetails& referenceDetails) const;
private:
    std::filesystem::path m_referencesPath;
};

inline std::filesystem::path getSnapshotPath(const std::filesystem::path& referencesPath, const ReferenceDetails& referenceDetails)
{
    return referencesPath / referenceDetails.GetTestSuiteName() / referenceDetails.GetTestName() / (std::to_string(referenceDetails.GetSnapshotIndex()) + ".png");
}

class TestDriver
{
public:
    TestDriver(const std::filesystem::path& referencePath, const std::filesystem::path& outputPath, const std::string& testSuiteName, const std::string& testName);
    SnapshotCheckResult CheckSnapshot(const Image& image);

private:
    SnapshotChecker m_checker;
    std::string m_testSuiteName;
    std::string m_testName;
    int m_snapshotCounter = 0;
    std::filesystem::path m_outputPath;
};
}
