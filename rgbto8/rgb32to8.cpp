#include "rgb32to8.hpp"

namespace rgb32to8
{
	Bitmap<uint8_t> rgb32to8(const Bitmap<ColorRgba>& src, std::vector<ColorRgba> &palette_out)
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
				auto &palette = *static_cast<std::vector<ColorRgba>*>(userdata);
				ColorRgba new_entry{};
				new_entry.R = r;
				new_entry.G = g;
				new_entry.B = b;
				palette.push_back(new_entry);
			}, static_cast<void *>(&palette_out)
				);
		if (palette_out.size() < 256) palette_out.push_back(ColorRgba{ 0, 0, 0, 255 });
		if (palette_out.size() < 256) palette_out.push_back(ColorRgba{ 255, 255, 255, 255 });
		auto key_color_index = palette_out.size();
		if (key_color_index > 255)
		{
			key_color_index = 255;
			palette_out[key_color_index] = ColorRgba{ 0, 0, 0, 0 };
		}
		else
		{
			palette_out.push_back(ColorRgba{ 0, 0, 0, 0 });
		}

		auto rgb_palette = std::vector<ColorRgb>();
		for (size_t i = 0; i < key_color_index; i++)
		{
			auto& entry = palette_out[i];
			rgb_palette.push_back(ColorRgb{
				entry.R,
				entry.G,
				entry.B,
			});
		}

		auto ditherer = Ditherer(rgb_palette);
		auto ordered = ditherer.ApplyOrdered<ColorRgba, IColorRgba>(src);
		auto diffused = ditherer.ApplyDiffusion(ordered);
		auto alpha_map = ditherer.ApplyAlphaDither(src);
#pragma omp parallel for
		for (int y = 0; y < static_cast<int>(diffused.get_height()); y++)
		{
			auto tns_row = alpha_map.get_row(y);
			auto dst_row = diffused.get_row(y);
			for (int x = 0; x < static_cast<int>(diffused.get_width()); x++)
			{
				auto transparency = tns_row[x];
				switch (transparency)
				{
				default:
					throw std::runtime_error("Bad transparency value, must be 0 or 255.");
				case 0:
					dst_row[x] = static_cast<uint8_t>(key_color_index);
					break;
				case 255:
					break;
				}
			}
		}

		return diffused;
	}
}
