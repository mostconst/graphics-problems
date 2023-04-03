#pragma once

namespace nsk_cg
{
class IndexTriangle final
{
public:
    IndexTriangle(unsigned first, unsigned second, unsigned third);

    unsigned First() const;
    unsigned Second() const;
    unsigned Third() const;

private:
    const unsigned int first;
    const unsigned int second;
    const unsigned int third;
};
}