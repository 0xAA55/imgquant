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
		std::vector<Color24> palette;

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
						static_cast<int>(bit_reverse(bit_interleave(x ^ y, x))) >> 24;
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

			for (size_t i = 0; i < palette.size(); i++)
			{
				this->palette.push_back(Color24{ palette[i].R, palette[i].G, palette[i].B });
			}
		}

		template<Rgb_c Pixel>
		void ApplyOrdered(uint32_t width, uint32_t height, Pixel **row_pointers) const
		{
			int diff = (r_diff + g_diff + b_diff) / 3;
#pragma omp parallel for
			for (std::ptrdiff_t y = 0; y < static_cast<std::ptrdiff_t>(height); y++)
			{
				auto *row = row_pointers[y];
				for (std::ptrdiff_t x = 0; x < static_cast<std::ptrdiff_t>(width); x++)
				{
					auto &pix = row[x];
					int dm = dither_matrix[(x & 0xFF) + (y & 0xFF) * 256];
					dm = dm * diff / 128;
					pix.R = static_cast<uint8_t>(std::max(std::min(static_cast<int>(pix.R) + dm, 255), 0));
					pix.G = static_cast<uint8_t>(std::max(std::min(static_cast<int>(pix.G) + dm, 255), 0));
					pix.B = static_cast<uint8_t>(std::max(std::min(static_cast<int>(pix.B) + dm, 255), 0));
				}
			}
		}

		template<Rgb_c Pixel>
		QuantError get_quant_error(Pixel& src_pix, uint8_t dst_pix)
		{
			auto dst_col = palette[dst_pix];
			return QuantError
			{
				static_cast<int>(src_pix.R) - static_cast<int>(dst_col.R),
				static_cast<int>(src_pix.G) - static_cast<int>(dst_col.G),
				static_cast<int>(src_pix.B) - static_cast<int>(dst_col.B),
			};
		}

		template<Rgb_c Pixel>
		static void diffuse_error(Pixel &target, QuantError& error, int numerator, int denominator)
		{
			int R_diffuse = error.R * numerator / denominator;
			int G_diffuse = error.G * numerator / denominator;
			int B_diffuse = error.B * numerator / denominator;
			target.R = static_cast<uint8_t>(std::max(std::min(static_cast<int>(target.R) + R_diffuse, 255), 0));
			target.G = static_cast<uint8_t>(std::max(std::min(static_cast<int>(target.G) + G_diffuse, 255), 0));
			target.B = static_cast<uint8_t>(std::max(std::min(static_cast<int>(target.B) + B_diffuse, 255), 0));
		}

		template<Rgb_c Pixel>
		void ApplyDiffusion(uint32_t width, uint32_t height, Pixel **row_pointers, uint8_t **out_row_pointers)
		{
			for (size_t y = 0; y < static_cast<size_t>(height); y++)
			{
				auto src_row = row_pointers[y];
				auto dst_row = out_row_pointers[y];
				bool is_last_line = (y == static_cast<size_t>(height) - 1);
				auto src_row_2 = is_last_line ? row_pointers[y] : row_pointers[y + 1];
				for (size_t x = 0; x < static_cast<size_t>(width); x++)
				{
					bool is_first_pix = (x == 0);
					bool is_last_pix = (x == static_cast<size_t>(width) - 1);
					auto &src_pix = src_row[x];
					auto dst_pix = palette_mapper.get_color_index(src_pix.R, src_pix.G, src_pix.B);
					dst_row[x] = dst_pix;
					auto quant_error = get_quant_error(src_pix, dst_pix);
					if (!is_last_line)
					{
						if (!is_first_pix) diffuse_error(src_row_2[x - 1], quant_error, 3, 16);
						diffuse_error(src_row_2[x], quant_error, 5, 16);
						if (!is_last_pix) diffuse_error(src_row_2[x + 1], quant_error, 1, 16);
					}
					if (!is_last_pix) diffuse_error(src_row[x + 1], quant_error, 7, 16);
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
