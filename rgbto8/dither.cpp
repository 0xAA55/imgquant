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

	template Ditherer::Ditherer(const std::vector<Color24> &palette);
	template Ditherer::Ditherer(const std::vector<Color32> &palette);
	template void Ditherer::ApplyOrdered(Bitmap<Color24>& to_apply) const;
	template void Ditherer::ApplyOrdered(Bitmap<Color32>& to_apply) const;
	template void Ditherer::ApplyDiffusion(uint32_t width, uint32_t height, const Color24*const* row_pointers, uint8_t **out_row_pointers);
	template void Ditherer::ApplyDiffusion(uint32_t width, uint32_t height, const Color32*const* row_pointers, uint8_t **out_row_pointers);

	void Ditherer::ApplyOrdered(uint32_t width, uint32_t height, uint8_t **row_pointers) const
	{
#pragma omp parallel for
		for (std::ptrdiff_t y = 0; y < static_cast<std::ptrdiff_t>(height); y++)
		{
			auto *row = row_pointers[y];
			for (std::ptrdiff_t x = 0; x < static_cast<std::ptrdiff_t>(width); x++)
			{
				auto &pix = row[x];
				int dm = dither_matrix[(x & 0xFF) + (y & 0xFF) * 256] + 128;
				if (pix > dm) pix = 255;
				else pix = 0;
			}
		}
	}
	QuantError Ditherer::get_quant_error(QuantError & src_pix, uint8_t dst_pix) const
	{
		auto& dst_col = palette[dst_pix];
		return QuantError
		{
			src_pix.R - static_cast<int>(dst_col.R),
			src_pix.G - static_cast<int>(dst_col.G),
			src_pix.B - static_cast<int>(dst_col.B),
		};
	}
	void Ditherer::diffuse_error(QuantError & target, QuantError & error, int numerator, int denominator)
	{
		int R_diffuse = error.R * numerator / denominator;
		int G_diffuse = error.G * numerator / denominator;
		int B_diffuse = error.B * numerator / denominator;
		target.R += R_diffuse;
		target.G += G_diffuse;
		target.B += B_diffuse;
	}
};
