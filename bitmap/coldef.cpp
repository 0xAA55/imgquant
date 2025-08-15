#pragma once

#include "coldef.hpp"

namespace coldef
{
	template struct BColor1<uint8_t>;
	template struct BColor1<int>;
	template struct BColorRgb<uint8_t>;
	template struct BColorRgba<uint8_t>;
	template struct BColorRgb<int>;
	template struct BColorRgba<int>;
};
