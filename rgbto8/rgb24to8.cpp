#include "rgb24to8.hpp"
#include "octree.hpp"

namespace rgb24to8
{
	template Bitmap<uint8_t> rgb24to8(const Bitmap<ColorRgba>& src, std::vector<ColorRgb> &palette_out);
	template Bitmap<uint8_t> rgb24to8(const Bitmap<ColorRgba>& src, std::vector<ColorRgba> &palette_out);
}
