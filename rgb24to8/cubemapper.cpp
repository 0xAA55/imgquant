#include "cubemapper.hpp"

namespace cubemapper
{
	template size_t CubeMapper::get_nearest_color_index(const std::vector<Palette24> &palette, uint8_t R, uint8_t G, uint8_t B);
	template CubeMapper::CubeMapper(const std::vector<Palette32> &palette);
	template size_t CubeMapper::get_nearest_color_index(const std::vector<Palette32> &palette, uint8_t R, uint8_t G, uint8_t B);
	template CubeMapper::CubeMapper(const std::vector<Palette32> &palette);
};

