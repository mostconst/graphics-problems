#pragma once

namespace nsk_cg
{
class IScreenSizeListener
{
public:
    virtual ~IScreenSizeListener() = default;

    virtual void OnWindowSizeChange(int width, int height) = 0;
};
}