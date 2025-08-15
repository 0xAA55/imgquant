#pragma once

#include <cinttypes>
#include <vector>

#include <bitmap/bitmap.hpp>

#include "rgb2i.hpp"
#include "octree.hpp"
#include "dither.hpp"

namespace rgb24to8
{
	using namespace rgb2i;
	using namespace octree;
	using namespace bitmap;
	using namespace dither;

	template<typename T_pixel, typename T_palette>
	Bitmap<uint8_t> rgb24to8(const Bitmap<T_pixel>& src, std::vector<T_palette> &palette_out)
	{
		uint32_t src_width = src.get_width();
		uint32_t src_height = src.get_height();
		auto palette_gen = PaletteGenerator();
		for (uint32_t y = 0; y < src_height; y++)
		{
			auto row = src.get_row(y);
			for (uint32_t x = 0; x < src_width; x++)
			{
				auto pix = row[x];
				palette_gen.add_color(pix.R, pix.G, pix.B);
				if (palette_gen.get_num_colors() >= 256)
					palette_gen.reduce_color();
			}
		}

		palette_out.clear();
		palette_gen.get_palette([](void *userdata, uint8_t r, uint8_t g, uint8_t b)
			{
				auto &palette = *static_cast<std::vector<T_palette>*>(userdata);
				T_palette new_entry{};
				new_entry.R = r;
				new_entry.G = g;
				new_entry.B = b;
				palette.push_back(new_entry);
			}, static_cast<void *>(&palette_out)
		);
		if (palette_out.size() < 256) palette_out.push_back(T_palette{ 0, 0, 0 });
		if (palette_out.size() < 256) palette_out.push_back(T_palette{ 255, 255, 255 });

		auto ditherer = Ditherer(palette_out);
		auto ordered = ditherer.ApplyOrdered<ColorRgba, IColorRgba>(src);
		return ditherer.ApplyDiffusion(ordered);
	}

	extern template Bitmap<uint8_t> rgb24to8(const Bitmap<ColorRgba>& src, std::vector<ColorRgb> &palette_out);
	extern template Bitmap<uint8_t> rgb24to8(const Bitmap<ColorRgba>& src, std::vector<ColorRgba> &palette_out);
};

