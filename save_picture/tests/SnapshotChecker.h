#pragma once
#include <filesystem>
#include <optional>
#include <string>

namespace nsk_cg
{
class Image;
}

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
    SnapshotChecker(const std::filesystem::path& referencesPath, const std::filesystem::path& outputPath);
    SnapshotCheckResult CheckSnapshot(const nsk_cg::Image& image, const ReferenceDetails& referenceDetails) const;
    std::optional<nsk_cg::Image> GetReferenceImage(const ReferenceDetails& referenceDetails) const;
private:
    std::filesystem::path m_referencesPath;
    std::filesystem::path m_outputPath;
};
