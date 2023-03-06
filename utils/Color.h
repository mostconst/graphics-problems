#pragma once
#include <cstdint>
#include "helper_enums.h"

namespace utils
{
	class Color
	{
	public:
		Color(uint32_t color);

		Color(float red, float green, float blue);

        float GetRed() const
		{
			return red;
		}

		float GetGreen() const
		{
			return green;
		}

		float GetBlue() const
		{
			return blue;
		}

		static size_t GetComponentSize()
		{
			return sizeof(decltype(red));
		}
	private:
		const float red;
		const float green;
		const float blue;

	public:
		static constexpr size_t nComponents = 3;
		static constexpr AttribType componentType = AttribType::Float;
	};
}