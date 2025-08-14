#pragma once

#include <cinttypes>
#include <vector>

#include "paldef.hpp"
#include "octree.hpp"
#include "cubemapper.hpp"

namespace rgb24to8
{
	using namespace paldef;
	using namespace octree;
	using namespace cubemapper;

	template<Rgb_c T_pixel, Rgb_c T_palette>
	void rgb24to8(uint32_t src_width, uint32_t src_height, const T_pixel *const *src_row_pointers,
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

		auto mapper = CubeMapper(palette_out);

		size_t pitch = (static_cast<size_t>(src_width - 1) / 4 + 1) * 4;
		bitmap_out.resize(pitch * src_height);
		if (row_pointers_out) row_pointers_out->clear();

		for (uint32_t y = 0; y < src_height; y++)
		{
			auto src_row = src_row_pointers[y];
			auto dst_row = &bitmap_out[pitch * y];
			if (row_pointers_out) row_pointers_out->push_back(dst_row);
			for (uint32_t x = 0; x < src_width; x++)
			{
				auto src_pix = src_row[x];
				dst_row[x] = mapper.get_color_index(src_pix.R, src_pix.G, src_pix.B);
			}
		}
	}

	extern template
		void rgb24to8(uint32_t src_width, uint32_t src_height, Color24 **src_row_pointers,
			std::vector<uint8_t> &bitmap_out,
			std::vector<Color24> &palette_out,
			std::vector<uint8_t *> *row_pointers_out);

	extern template
		void rgb24to8(uint32_t src_width, uint32_t src_height, Color32 **src_row_pointers,
			std::vector<uint8_t> &bitmap_out,
			std::vector<Color24> &palette_out,
			std::vector<uint8_t *> *row_pointers_out);

	extern template
		void rgb24to8(uint32_t src_width, uint32_t src_height, Color24 **src_row_pointers,
			std::vector<uint8_t> &bitmap_out,
			std::vector<Color32> &palette_out,
			std::vector<uint8_t *> *row_pointers_out);

	extern template
		void rgb24to8(uint32_t src_width, uint32_t src_height, Color32 **src_row_pointers,
			std::vector<uint8_t> &bitmap_out,
			std::vector<Color32> &palette_out,
			std::vector<uint8_t *> *row_pointers_out);
};

