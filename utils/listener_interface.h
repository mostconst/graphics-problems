#pragma once

namespace nsk_cg
{
struct ScreenSize;

class IScreenSizeListener
{
public:
    virtual ~IScreenSizeListener() = default;

    virtual void OnWindowSizeChange(const ScreenSize& size) = 0;
};
}