#include "cubemapper.hpp"

namespace cubemapper
{
	template size_t CubeMapper::get_nearest_color_index(const std::vector<Color24> &palette, uint8_t R, uint8_t G, uint8_t B);
	template size_t CubeMapper::get_nearest_color_index(const std::vector<Color32> &palette, uint8_t R, uint8_t G, uint8_t B);
	template CubeMapper::CubeMapper(const std::vector<Color24> &palette);
	template CubeMapper::CubeMapper(const std::vector<Color32> &palette);
};

