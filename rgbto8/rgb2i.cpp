#include "rgb2i.hpp"

namespace rgb2i
{
	template size_t Rgb2I::get_nearest_color_index(const std::vector<Color24> &palette, uint8_t R, uint8_t G, uint8_t B);
	template size_t Rgb2I::get_nearest_color_index(const std::vector<Color32> &palette, uint8_t R, uint8_t G, uint8_t B);
	template Rgb2I::Rgb2I(const std::vector<Color24> &palette);
	template Rgb2I::Rgb2I(const std::vector<Color32> &palette);
};

