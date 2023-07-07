#pragma once

namespace nsk_cg
{
class ScreenSize;

class IScreenSizeListener
{
public:
    virtual ~IScreenSizeListener() = default;

    virtual void OnWindowSizeChange(const ScreenSize& size) = 0;
};
}