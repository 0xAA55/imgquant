#pragma once

#include <cinttypes>
#include <vector>

#include "paldef.hpp"
#include "octree.hpp"
#include "dither.hpp"
#include "rgb2i.hpp"

namespace rgb32to8
{
	using namespace paldef;
	using namespace octree;
	using namespace dither;
	using namespace rgb2i;

	template<Rgba_c T_pixel, Rgba_c T_palette>
	void rgb32to8(uint32_t src_width, uint32_t src_height, T_pixel **src_row_pointers,
		std::vector<uint8_t> &bitmap_out,
		std::vector<T_palette> &palette_out,
		std::vector<uint8_t *> *row_pointers_out)
	{
		auto palette_gen = PaletteGenerator();
		for (uint32_t y = 0; y < src_height; y++)
		{
			auto row = src_row_pointers[y];
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
		if (palette_out.size() < 256) palette_out.push_back(T_palette{ 0, 0, 0, 255 });
		if (palette_out.size() < 256) palette_out.push_back(T_palette{ 255, 255, 255, 255 });
		auto key_color_index = palette_out.size();
		if (key_color_index > 255)
		{
			key_color_index = 255;
			palette_out[key_color_index] = T_palette{ 0, 0, 0, 0 };
		}
		else
		{
			palette_out.push_back(T_palette{ 0, 0, 0, 0 });
		}

		size_t pitch = (static_cast<size_t>(src_width - 1) / 4 + 1) * 4;
		bitmap_out.resize(pitch * src_height);
		auto dst_row_pointers = std::vector<uint8_t *>();

		for (uint32_t y = 0; y < src_height; y++)
		{
			auto dst_row = &bitmap_out[pitch * y];
			dst_row_pointers.push_back(dst_row);
		}


	}




	extern template
		void rgb32to8(uint32_t src_width, uint32_t src_height, Color32 **src_row_pointers,
			std::vector<uint8_t> &bitmap_out,
			std::vector<Color32> &palette_out,
			std::vector<uint8_t *> *row_pointers_out);
};

