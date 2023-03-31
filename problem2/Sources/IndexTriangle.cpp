﻿#include "IndexTriangle.h"

namespace nsk_cg
{
unsigned IndexTriangle::First() const
{
    return first;
}

unsigned IndexTriangle::Second() const
{
    return second;
}

unsigned IndexTriangle::Third() const
{
    return third;
}

IndexTriangle::IndexTriangle(const unsigned first, const unsigned second, const unsigned third)
    : first(first),
    second(second),
    third(third)
{

}
}
