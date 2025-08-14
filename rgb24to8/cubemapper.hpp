#pragma once

#include <cinttypes>
#include <vector>

#include "paldef.hpp"

namespace cubemapper
{
	using namespace paldef;

	class CubeMapper
	{
	protected:
		std::vector<uint8_t> mapper;
		std::vector<std::vector<uint8_t *>> router;

		template<Rgb_c T>
		static size_t get_nearest_color_index(const std::vector<T> &palette, uint8_t R, uint8_t G, uint8_t B)
		{
			size_t best_i = 0;
			uint32_t best_score = 0xFFFFFFFF;
			for (size_t i = 0; i < palette.size(); i++)
			{
				int r_diff = static_cast<int>(R) - static_cast<int>(palette[i].R);
				int g_diff = static_cast<int>(G) - static_cast<int>(palette[i].G);
				int b_diff = static_cast<int>(B) - static_cast<int>(palette[i].B);
				uint32_t diff_score = static_cast<uint32_t>(r_diff * r_diff + g_diff * g_diff + b_diff * b_diff);
				if (diff_score < best_score)
				{
					best_score = diff_score;
					best_i = i;
				}
			}
			return best_i;
		}

	public:
		CubeMapper() = delete;
		template<Rgb_c T>
		CubeMapper(const std::vector<T> &palette)
		{
			mapper.resize(static_cast<size_t>(256) * 256 * 256);
			for (size_t z = 0; z < 256; z++)
			{
				router.push_back(std::vector<uint8_t *>());
				auto &row = router.back();
				for (size_t y = 0; y < 256; y++)
				{
					row.push_back(&mapper[z * 256 * 256 + y * 256]);
				}
			}
#pragma omp parallel for
			for (ptrdiff_t z = 0; z < 256; z++)
			{
				auto &plane = router[z];
				for (size_t y = 0; y < 256; y++)
				{
					auto &row = plane[y];
					for (size_t x = 0; x < 256; x++)
					{
						row[x] = static_cast<uint8_t>(get_nearest_color_index(palette,
							static_cast<uint8_t>(x),
							static_cast<uint8_t>(y),
							static_cast<uint8_t>(z)));
					}
				}
			}
		}

		uint8_t get_color_index(uint8_t R, uint8_t G, uint8_t B) const
		{
			return router[R][G][B];
		}
	};

	extern template size_t CubeMapper::get_nearest_color_index(const std::vector<Palette24> &palette, uint8_t R, uint8_t G, uint8_t B);
	extern template CubeMapper::CubeMapper(const std::vector<Palette24> &palette);
	extern template size_t CubeMapper::get_nearest_color_index(const std::vector<Palette32> &palette, uint8_t R, uint8_t G, uint8_t B);
	extern template CubeMapper::CubeMapper(const std::vector<Palette32> &palette);
};

