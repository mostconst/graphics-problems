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

class SnapshotChecker
{
public:
    SnapshotChecker(const std::filesystem::path& referencesPath, const std::string& testSuiteName, const std::string& testName);
    SnapshotCheckResult CheckSnapshot(const nsk_cg::Image& image);
    std::optional<nsk_cg::Image> GetReferenceImage(int snapshotIndex);
private:
    std::filesystem::path m_referencesPath;
    std::string m_testSuiteName;
    std::string m_testName;
    int m_counter = 0;
};
