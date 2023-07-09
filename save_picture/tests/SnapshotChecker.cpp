#include "SnapshotChecker.h"
#include <filesystem>

namespace fs = std::filesystem;
#include "Image.h"


SnapshotChecker::SnapshotChecker(const std::filesystem::path& referencesPath, const std::string& testSuiteName, const std::string& testName)
: m_referencesPath(referencesPath),
  m_testSuiteName(testSuiteName),
  m_testName(testName)
{
}


SnapshotCheckResult SnapshotChecker::CheckSnapshot(const nsk_cg::Image& image)
{
    const auto testOutputPath = fs::current_path() / m_testSuiteName / m_testName;
    fs::create_directories(testOutputPath);
    const std::string filename = std::to_string(++m_counter) + ".png";
    saveImage(image, testOutputPath / filename);
}

std::optional<nsk_cg::Image> SnapshotChecker::GetReferenceImage(const int snapshotIndex) const
{
    return nsk_cg::readImage(m_referencesPath / m_testSuiteName / m_testName / (std::to_string(snapshotIndex) + ".png"));
}
