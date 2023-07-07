#include "SnapshotChecker.h"
#include <filesystem>

namespace fs = std::filesystem;
#include "Image.h"


SnapshotChecker::SnapshotChecker(const std::string& testSuiteName, const std::string& testName): m_testSuiteName(testSuiteName),
                                                                                                 m_testName(testName)
{
}


void SnapshotChecker::TakeSnapshot(const nsk_cg::Image& image)
{
    const auto testOutputPath = fs::current_path() / m_testSuiteName / m_testName;
    fs::create_directories(testOutputPath);
    const std::string filename = std::to_string(++m_counter) + ".png";
    saveImage(image, testOutputPath / filename);
}
