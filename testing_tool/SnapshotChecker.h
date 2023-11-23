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
    ReferenceDetails(std::string testSuiteName, std::string testName, int snapshotIndex);
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
    SnapshotChecker(std::filesystem::path referencesPath);
    SnapshotCheckResult CheckSnapshot(const Image& image, const ReferenceDetails& referenceDetails) const;
    std::optional<Image> GetReferenceImage(const ReferenceDetails& referenceDetails) const;
    const std::filesystem::path& GetReferencesPath() const { return m_referencesPath; }

private:
    std::filesystem::path m_referencesPath;
};

std::filesystem::path getSnapshotPath(const std::filesystem::path& rootPath, const ReferenceDetails& referenceDetails);
std::filesystem::path getTestPath(const std::filesystem::path& rootPath, const std::string& suiteName, const std::string& testName);

class TestDriver
{
public:
    TestDriver(const std::filesystem::path& sourceDirectory, const std::filesystem::path& binaryDirectory,
               std::string testSuiteName, std::string testName);
    SnapshotCheckResult CheckSnapshot(const Image& image, int snapshotIndex) const;
    std::wstring GetResultLogString(SnapshotCheckResult result, int snapshotIndex) const;
    std::filesystem::path GetReferencePath(int snapshotIndex) const;
    std::filesystem::path GetSnapshotPath(int snapshotIndex) const;

private:
    SnapshotChecker m_checker;
    std::string m_testSuiteName;
    std::string m_testName;
    std::filesystem::path m_outputPath;
};
}
