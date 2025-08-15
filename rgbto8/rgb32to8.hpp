#pragma once

#include <cinttypes>
#include <vector>

#include <bitmap/bitmap.hpp>

#include "rgb2i.hpp"
#include "octree.hpp"
#include "dither.hpp"

namespace rgb32to8
{
	using namespace rgb2i;
	using namespace octree;
	using namespace bitmap;
	using namespace dither;

	Bitmap<uint8_t> rgb32to8(const Bitmap<ColorRgba>& src, std::vector<ColorRgba> &palette_out);
};

