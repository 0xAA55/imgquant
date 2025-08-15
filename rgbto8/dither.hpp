#pragma once

#include <cinttypes>
#include <vector>
#include <algorithm>

#include <bitmap/bitmap.hpp>

#include "rgb2i.hpp"

namespace dither
{
	using namespace rgb2i;
	using namespace bitmap;

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
		Rgb2I palette_mapper;
		std::vector<ColorRgb> palette;

		uint32_t r_diff = 0;
		uint32_t g_diff = 0;
		uint32_t b_diff = 0;

	public:
		Ditherer() = delete;

		int sample_matrix(int x, int y) const;

		template<typename T>
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
				this->palette.push_back(ColorRgb{ palette[i].R, palette[i].G, palette[i].B });
			}
		}

		template<typename PixelSrc, typename PixelDst>
		Bitmap<PixelDst> ApplyOrdered(const Bitmap<PixelSrc>& src) const
		{
			uint32_t width = src.get_width();
			uint32_t height = src.get_height();
			auto dst = Bitmap<PixelDst>(width, height);
			int diff = (r_diff + g_diff + b_diff) / 3;
			constexpr size_t size = std::min(PixelDst::size(), PixelSrc::size());
#pragma omp parallel for
			for (int y = 0; y < static_cast<int>(height); y++)
			{
				auto* src_row = src.get_row(y);
				auto* dst_row = dst.get_row(y);
				for (int x = 0; x < static_cast<int>(width); x++)
				{
					auto& src_pix = src_row[x];
					auto& dst_pix = dst_row[x];
					int dm = sample_matrix(x, y) * diff / 128;
					for (size_t i = 0; i < std::min(size, static_cast<size_t>(3)); i++)
					{
						dst_pix[i] = static_cast<uint8_t>(std::max(std::min(static_cast<int>(src_pix[i]) + dm, 255), 0));
					}
					if (dst_pix.size() == 4)
					{
						if (src_pix.size() == 4) dst_pix[3] = src_pix[3];
						else dst_pix[3] = 255;
					}
				}
			}
			return dst;
		}

		Bitmap<uint8_t> ApplyAlphaDither(const Bitmap<ColorRgba>& src) const;
		static void diffuse_error(int& target, int error, int numerator, int denominator);

		QuantError get_quant_error(IColorRgb& src_pix, uint8_t dst_pix) const;
		static void diffuse_error(IColorRgb &target, QuantError& error, int numerator, int denominator);

		template<typename Pixel>
		Bitmap<uint8_t> ApplyDiffusion(const Bitmap<Pixel>& src) const
		{
			uint32_t width = src.get_width();
			uint32_t height = src.get_height();
			auto canvas = convert<Pixel, IColorRgb>(src, [](const Pixel & s) ->IColorRgb
			{
				return IColorRgb{
					s.R,
					s.G,
					s.B,
				};
			});
			auto dst = Bitmap<uint8_t>(width, height);
			for (uint32_t y = 0; y < height; y++)
			{
				auto src_row = canvas.get_row(y);
				auto dst_row = dst.get_row(y);
				bool is_last_line = (y == height - 1);
				auto src_row_2 = is_last_line ? canvas.get_row(y) : canvas.get_row(y + 1);
				for (uint32_t x = 0; x < width; x++)
				{
					bool is_first_pix = (x == 0);
					bool is_last_pix = (x == width - 1);
					auto &src_pix = src_row[x];
					auto dst_pix = palette_mapper.get_color_index(
						std::max(std::min(src_pix.R, 255), 0),
						std::max(std::min(src_pix.G, 255), 0),
						std::max(std::min(src_pix.B, 255), 0)
					);
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
			return dst;
		}
	};

	extern template Ditherer::Ditherer(const std::vector<ColorRgb> &palette);
	extern template Ditherer::Ditherer(const std::vector<ColorRgba> &palette);
	extern template Bitmap<ColorRgb> Ditherer::ApplyOrdered(const Bitmap<ColorRgb>& src) const;
	extern template Bitmap<ColorRgba> Ditherer::ApplyOrdered(const Bitmap<ColorRgba>& src) const;
	extern template Bitmap<IColorRgb> Ditherer::ApplyOrdered(const Bitmap<ColorRgb>& src) const;
	extern template Bitmap<IColorRgba> Ditherer::ApplyOrdered(const Bitmap<ColorRgba>& src) const;
	extern template Bitmap<uint8_t> Ditherer::ApplyDiffusion(const Bitmap<ColorRgb>& src) const;
	extern template Bitmap<uint8_t> Ditherer::ApplyDiffusion(const Bitmap<ColorRgba>& src) const;
	extern template Bitmap<uint8_t> Ditherer::ApplyDiffusion(const Bitmap<IColorRgb>& src) const;
	extern template Bitmap<uint8_t> Ditherer::ApplyDiffusion(const Bitmap<IColorRgba>& src) const;
};
