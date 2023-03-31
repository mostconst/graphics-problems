#pragma once
#include <cstdint>
#include "helper_enums.h"

namespace nsk_cg::utils
{
class Color final
{
public:
	Color(uint32_t color);
	Color(float red, float green, float blue);

    float GetRed() const;
	float GetGreen() const;
	float GetBlue() const;
	static size_t GetComponentSize();

private:
	const float red;
	const float green;
	const float blue;

public:
	static constexpr size_t nComponents = 3;
	static constexpr AttributeType componentType = AttributeType::Float;
};
}