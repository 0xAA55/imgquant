#include "rgb2i.hpp"

namespace rgb2i
{
	template size_t Rgb2I::get_nearest_color_index(const std::vector<ColorRgb> &palette, uint8_t R, uint8_t G, uint8_t B);
	template size_t Rgb2I::get_nearest_color_index(const std::vector<ColorRgba> &palette, uint8_t R, uint8_t G, uint8_t B);
	template Rgb2I::Rgb2I(const std::vector<ColorRgb> &palette);
	template Rgb2I::Rgb2I(const std::vector<ColorRgba> &palette);
};

