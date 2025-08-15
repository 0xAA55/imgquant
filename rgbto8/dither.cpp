#include "dither.hpp"

namespace dither
{
	uint32_t Ditherer::bit_interleave(uint32_t a, uint32_t b)
	{
		uint32_t ret = 0;
		for (int i = 0; i < 16; i++)
		{
			ret |= ((1 << i) & a) << (i + 0);
			ret |= ((1 << i) & b) << (i + 1);
		}
		return ret;
	}

	uint32_t Ditherer::bit_reverse(uint32_t v)
	{
		uint32_t ret = 0;
		for (int i = 0; i < 32; i++)
		{
			ret |= ((v << i) & 0x80000000) >> (31 - i);
		}
		return ret;
	}

	int Ditherer::sample_matrix(int x, int y) const
	{
		return dither_matrix[static_cast<size_t>(x & 0xFF) + static_cast<size_t>(y & 0xFF) * 256];
	}

	template Ditherer::Ditherer(const std::vector<ColorRgb> &palette);
	template Ditherer::Ditherer(const std::vector<ColorRgba> &palette);
	template Bitmap<ColorRgb> Ditherer::ApplyOrdered(const Bitmap<ColorRgb>& src) const;
	template Bitmap<ColorRgba> Ditherer::ApplyOrdered(const Bitmap<ColorRgba>& src) const;
	template Bitmap<IColorRgb> Ditherer::ApplyOrdered(const Bitmap<ColorRgb>& src) const;
	template Bitmap<IColorRgba> Ditherer::ApplyOrdered(const Bitmap<ColorRgba>& src) const;
	template Bitmap<uint8_t> Ditherer::ApplyDiffusion(const Bitmap<ColorRgb>& src) const;
	template Bitmap<uint8_t> Ditherer::ApplyDiffusion(const Bitmap<ColorRgba>& src) const;
	template Bitmap<uint8_t> Ditherer::ApplyDiffusion(const Bitmap<IColorRgb>& src) const;
	template Bitmap<uint8_t> Ditherer::ApplyDiffusion(const Bitmap<IColorRgba>& src) const;

	Bitmap<uint8_t> Ditherer::ApplyAlphaDither(const Bitmap<ColorRgba>& src) const
	{
		auto ret = convert<ColorRgba, uint8_t>(src, [](const ColorRgba& c) -> uint8_t { return c.A; });
#pragma omp parallel for
		for (int y = 0; y < static_cast<int>(ret.get_height()); y++)
		{
			auto row = ret.get_row(y);
			for (int x = 0; x < static_cast<int>(ret.get_width()); x++)
			{
				auto &pix = row[x];
				if (pix > sample_matrix(x, y) + 128) pix = 255;
				else pix = 0;
			}
		}
		return ret;
	}
	void Ditherer::diffuse_error(int& target, int error, int numerator, int denominator)
	{
		int diffuse = error * numerator / denominator;
		target += diffuse;
	}
	QuantError Ditherer::get_quant_error(IColorRgb& src_pix, uint8_t dst_pix) const
	{
		auto& dst_col = palette[dst_pix];
		return QuantError
		{
			src_pix.R - static_cast<int>(dst_col.R),
			src_pix.G - static_cast<int>(dst_col.G),
			src_pix.B - static_cast<int>(dst_col.B),
		};
	}
	void Ditherer::diffuse_error(IColorRgb& target, QuantError & error, int numerator, int denominator)
	{
		int R_diffuse = error.R * numerator / denominator;
		int G_diffuse = error.G * numerator / denominator;
		int B_diffuse = error.B * numerator / denominator;
		target.R += R_diffuse;
		target.G += G_diffuse;
		target.B += B_diffuse;
	}
};
