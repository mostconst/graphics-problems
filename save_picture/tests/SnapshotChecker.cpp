#include "SnapshotChecker.h"
#include <filesystem>

namespace fs = std::filesystem;
#include "Image.h"


ReferenceDetails::ReferenceDetails(const std::string& testSuiteName, const std::string& testName,
    const int snapshotIndex): m_testSuiteName(testSuiteName),
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


SnapshotChecker::SnapshotChecker(const std::filesystem::path& referencesPath, const std::filesystem::path& outputPath)
: m_referencesPath(referencesPath)
, m_outputPath(outputPath)
{
}

namespace
{
fs::path getSnapshotPath(const fs::path& referencesPath, const ReferenceDetails& referenceDetails)
{
    return referencesPath / referenceDetails.GetTestSuiteName() / referenceDetails.GetTestName() / (std::to_string(referenceDetails.GetSnapshotIndex()) + ".png");
}
}

SnapshotCheckResult SnapshotChecker::CheckSnapshot(const nsk_cg::Image& image, const ReferenceDetails& referenceDetails) const
{
    const auto referenceImage = GetReferenceImage(referenceDetails);
    if (referenceImage == std::nullopt)
    {
        const auto snapshotPath = getSnapshotPath(m_outputPath, referenceDetails);
        fs::create_directories(snapshotPath.parent_path());
        saveImage(image, snapshotPath);
        return SnapshotCheckResult::NoReference;
    }

    return nsk_cg::equalExactly(*referenceImage, image) ? SnapshotCheckResult::Ok : SnapshotCheckResult::Mismatch;
}

std::optional<nsk_cg::Image> SnapshotChecker::GetReferenceImage(const ReferenceDetails& referenceDetails) const
{
    return nsk_cg::readImage(getSnapshotPath(m_referencesPath, referenceDetails));
}
