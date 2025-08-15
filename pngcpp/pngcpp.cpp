#include "pngcpp.hpp"

#include <png.h>
#include <stdio.h>

namespace pngcpp
{
	InvalidArgument::InvalidArgument(const std::string &what) noexcept:
		std::runtime_error(what.c_str())
	{
	}
	LoadPngException::LoadPngException(const std::string &what) noexcept :
		std::runtime_error(what.c_str())
	{
	}
	SavePngException::SavePngException(const std::string &what) noexcept :
		std::runtime_error(what.c_str())
	{
	}
	class File
	{
	protected:
		FILE *fp;

	public:
		File() = delete;
		File(File &&) = default;
		File(const File &) = delete;
		File(const char *path, const char *mode) : fp(fopen(path, mode))
		{
			if (!fp) throw SavePngException(std::string("Could not open the file `") + path + "` for write.");
		}
		~File()
		{
			fclose(fp);
		}

		FILE *get() const
		{
			return fp;
		}
	};
	class png_writer
	{
	protected:
		png_structp p;
		png_infop i;

	public:
		png_writer() : p(png_create_write_struct(PNG_LIBPNG_VER_STRING, const_cast<void *>(reinterpret_cast<const void *>(this)), reinterpret_cast<png_error_ptr>(on_png_error), NULL))
		{
			i = png_create_info_struct(p);
			if (!i)
			{
				png_destroy_write_struct(&p, nullptr);
				throw SavePngException(std::string("The function `png_create_info_struct()` had failed."));
			}
		}
		~png_writer()
		{
			png_destroy_write_struct(&p, &i);
		}

		void write_png_rgba(const std::string &path, uint32_t width, uint32_t height, const ColorRgba*const* row_pointers) const
		{
			auto fp = File(path.c_str(), "wb");

			png_init_io(p, fp.get());
			png_set_compression_level(p, 9);

			png_set_IHDR(p, i, width, height, 8,
				PNG_COLOR_TYPE_RGBA,
				PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);

			png_color_8 sig_bit = {8, 8, 8, 0, 8,};
			png_set_sBIT(p, i, &sig_bit);
			png_write_info(p, i);
			png_write_image(p, const_cast<uint8_t**>(reinterpret_cast<const uint8_t* const*>(row_pointers)));
			png_write_end(p, i);
		}

		void write_png_8bit(const std::string &path, uint32_t width, uint32_t height, const ColorRgb *palette, size_t num_palette_entries, const uint8_t *const *row_pointers) const
		{
			auto fp = File(path.c_str(), "wb");

			png_init_io(p, fp.get());
			png_set_compression_level(p, 9);

			png_set_IHDR(p, i, width, height, 8,
				PNG_COLOR_TYPE_PALETTE,
				PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);

			png_set_PLTE(p, i, reinterpret_cast<png_const_colorp>(&palette[0]), static_cast<int>(num_palette_entries));
			png_write_info(p, i);
			png_write_image(p, const_cast<uint8_t **>(reinterpret_cast<const uint8_t *const *>(row_pointers)));
			png_write_end(p, i);
		}

		void write_png_8bit(const std::string &path, uint32_t width, uint32_t height, const ColorRgba *palette, size_t num_palette_entries, const uint8_t *const *row_pointers) const
		{
			auto fp = File(path.c_str(), "wb");

			png_init_io(p, fp.get());
			png_set_compression_level(p, 9);

			png_set_IHDR(p, i, width, height, 8,
				PNG_COLOR_TYPE_PALETTE,
				PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);

			auto RgbPalette = std::vector<ColorRgb>();
			auto AlphaPeltte = std::vector<uint8_t>();
			for (size_t i = 0; i < num_palette_entries; i++)
			{
				auto &entry = palette[i];
				RgbPalette.push_back(ColorRgb{
					entry.R,
					entry.G,
					entry.B,
				});
				AlphaPeltte.push_back(entry.A);
			}
			png_set_PLTE(p, i, reinterpret_cast<png_const_colorp>(&RgbPalette[0]), static_cast<int>(num_palette_entries));
			png_set_tRNS(p, i, &AlphaPeltte[0], static_cast<int>(num_palette_entries), nullptr);
			png_write_info(p, i);
			png_write_image(p, const_cast<uint8_t **>(reinterpret_cast<const uint8_t *const *>(row_pointers)));
			png_write_end(p, i);
		}

	private:
		static void on_png_error(void *self, const char *message)
		{
			throw SavePngException(std::string(message));
		}
	};

	PngImage::PngImage(uint32_t width, uint32_t height, const ColorRgba &default_color) : bmp(width, height, default_color)
	{
	}
	PngImage::PngImage(const char *path) : PngImage(std::string(path))
	{
	}
	PngImage::PngImage(const std::string &path)
	{
		png_image image = { 0 };
		image.version = PNG_IMAGE_VERSION;
		if (png_image_begin_read_from_file(&image, path.c_str()))
		{
			image.format = PNG_FORMAT_RGBA;
			bmp = Bitmap<ColorRgba>(image.width, image.height);
			if (!png_image_finish_read(&image, NULL, bmp.get_bitmap(), static_cast<png_int_32>(bmp.get_pitch()), NULL))
			{
				throw LoadPngException(std::string("Read png file failed: \"") + path + "\"");
			}
		}
		else
		{
			throw LoadPngException(std::string("Could not load png file: \"") + path + "\"");
		}
	}
	uint32_t PngImage::get_width() const
	{
		return bmp.get_width();
	}
	uint32_t PngImage::get_height() const
	{
		return bmp.get_height();
	}
	ColorRgba **PngImage::get_row_pointers()
	{
		return bmp.get_row_pointers();
	}
	const ColorRgba*const* PngImage::get_row_pointers() const
	{
		return bmp.get_row_pointers();
	}
	const ColorRgba &PngImage::get_pixel(uint32_t x, uint32_t y) const
	{
		if (x >= get_width() || y >= get_height()) throw InvalidArgument(std::string("Invalid coordinate (") + std::to_string(x) + ", " + std::to_string(y) + ")");
		return bmp.get_row(y)[x];
	}
	ColorRgba &PngImage::get_pixel(uint32_t x, uint32_t y)
	{
		if (x >= get_width() || y >= get_height()) throw InvalidArgument(std::string("Invalid coordinate (") + std::to_string(x) + ", " + std::to_string(y) + ")");
		return bmp.get_row(y)[x];
	}
	void PngImage::set_pixel(uint32_t x, uint32_t y, const ColorRgba &pixel)
	{
		if (x >= get_width() || y >= get_height()) throw InvalidArgument(std::string("Invalid coordinate (") + std::to_string(x) + ", " + std::to_string(y) + ")");
		bmp.get_row(y)[x] = pixel;
	}
	void PngImage::save_png32_to(const std::string &path) const
	{
		auto w = png_writer();
		w.write_png_rgba(path, get_width(), get_height(), bmp.get_row_pointers());
	}
	void PngImage::save_png32_to(const std::string &path, uint32_t width, uint32_t height, const ColorRgba *const *row_pointers)
	{
		auto w = png_writer();
		w.write_png_rgba(path, width, height, &row_pointers[0]);
	}
	void PngImage::save_png8_to(const std::string &path, uint32_t width, uint32_t height, const ColorRgb *palette, size_t num_palette_entries, const uint8_t *const *row_pointers)
	{
		auto w = png_writer();
		w.write_png_8bit(path, width, height, palette, num_palette_entries, &row_pointers[0]);
	}
	void PngImage::save_png8_to(const std::string &path, uint32_t width, uint32_t height, const ColorRgba *palette, size_t num_palette_entries, const uint8_t *const *row_pointers)
	{
		auto w = png_writer();
		w.write_png_8bit(path, width, height, palette, num_palette_entries, &row_pointers[0]);
	}
}
