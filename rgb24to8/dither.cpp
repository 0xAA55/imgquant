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
	template void Ditherer::ApplyOrdered(uint32_t width, uint32_t height, Color24 **row_pointers) const;
	template void Ditherer::ApplyOrdered(uint32_t width, uint32_t height, Color32 **row_pointers) const;
	template void Ditherer::ApplyDiffusion(uint32_t width, uint32_t height, Color24 **row_pointers, uint8_t **out_row_pointers);
	template void Ditherer::ApplyDiffusion(uint32_t width, uint32_t height, Color32 **row_pointers, uint8_t **out_row_pointers);
};
