#pragma once
#include <string>

namespace nsk_cg
{
class Image;
}

class SnapshotChecker
{
public:
    SnapshotChecker(const std::string& testSuiteName, const std::string& testName);
    void TakeSnapshot(const nsk_cg::Image& image);
private:
    std::string m_testSuiteName;
    std::string m_testName;
    int m_counter = 0;
};
