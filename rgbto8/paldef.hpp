#pragma once

#include <cinttypes>
#include <concepts>

namespace paldef
{
	template<typename T> concept Rgb_c = requires(T t)
	{
		t.R;
		t.G;
		t.B;
	};

	template<typename T> concept Rgba_c = requires(T t)
	{
		t.R;
		t.G;
		t.B;
		t.A;
	};

#pragma pack(push, 1)
	struct Color24
	{
		uint8_t R;
		uint8_t G;
		uint8_t B;
	};

	struct Color32 : public Color24 {
		uint8_t A;
	};
#pragma pack(pop)
};

