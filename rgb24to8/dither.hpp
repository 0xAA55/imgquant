#pragma once

#include <cinttypes>
#include <vector>
#include <algorithm>

#include "cubemapper.hpp"

namespace dither
{
	using namespace cubemapper;

	struct QuantError
	{
		int R;
		int G;
		int B;
	};

	class Ditherer
	{
	protected:
		static uint32_t bit_interleave(uint32_t a, uint32_t b);
		static uint32_t bit_reverse(uint32_t v);

		std::vector<int> dither_matrix;
		CubeMapper palette_mapper;

		uint32_t r_diff = 0;
		uint32_t g_diff = 0;
		uint32_t b_diff = 0;

	public:
		Ditherer() = delete;

		template<Rgb_c T>
		Ditherer(const std::vector<T> &palette) : palette_mapper(palette)
		{
			dither_matrix.resize(65536);
			for (uint32_t y = 0; y < 256; y++)
			{
				for (uint32_t x = 0; x < 256; x++)
				{
					dither_matrix[static_cast<size_t>(x) + static_cast<size_t>(y) * 256] =
						bit_reverse(bit_interleave(x ^ y, x));
				}
			}

			std::vector<T> sort_palette = palette;
			std::sort(sort_palette.begin(), sort_palette.end(), [](const T &a, const T &b) -> int { return a.R < b.R; });
			for (size_t i = 0; i < sort_palette.size() - 1; i++)
			{
				uint32_t diff = sort_palette[i + 1].R - sort_palette[i].R;
				if (diff > r_diff) r_diff = diff;
			}
			std::sort(sort_palette.begin(), sort_palette.end(), [](const T &a, const T &b) -> int { return a.G < b.G; });
			for (size_t i = 0; i < sort_palette.size() - 1; i++)
			{
				uint32_t diff = sort_palette[i + 1].G - sort_palette[i].G;
				if (diff > g_diff) g_diff = diff;
			}
			std::sort(sort_palette.begin(), sort_palette.end(), [](const T &a, const T &b) -> int { return a.B < b.B; });
			for (size_t i = 0; i < sort_palette.size() - 1; i++)
			{
				uint32_t diff = sort_palette[i + 1].B - sort_palette[i].B;
				if (diff > b_diff) b_diff = diff;
			}
		}

		template<Rgb_c Pixel>
		void ApplyOrdered(uint32_t width, uint32_t height, Pixel **row_pointers) const
		{
// #pragma omp parallel for
			for (ptrdiff_t y = 0; y < static_cast<ptrdiff_t>(height); y++)
			{
				auto row = row_pointers[y];
				for (ptrdiff_t x = 0; x < static_cast<ptrdiff_t>(width); x++)
				{

				}
			}
		}

		template<Rgb_c Pixel>
		void ApplyDiffusion(uint32_t width, uint32_t height, Pixel **row_pointers, uint8_t **out_row_pointers)
		{
			for (size_t y = 0; y < static_cast<size_t>(height); y++)
			{
				auto row = row_pointers[y];
				for (size_t x = 0; x < static_cast<size_t>(width); x++)
				{

				}
			}
		}
	};

	extern template Ditherer::Ditherer(const std::vector<Color24> &palette);
	extern template Ditherer::Ditherer(const std::vector<Color32> &palette);
	extern template void Ditherer::ApplyOrdered(uint32_t width, uint32_t height, Color24 **row_pointers) const;
	extern template void Ditherer::ApplyOrdered(uint32_t width, uint32_t height, Color32 **row_pointers) const;
	extern template void Ditherer::ApplyDiffusion(uint32_t width, uint32_t height, Color24 **row_pointers, uint8_t **out_row_pointers);
	extern template void Ditherer::ApplyDiffusion(uint32_t width, uint32_t height, Color32 **row_pointers, uint8_t **out_row_pointers);
};
