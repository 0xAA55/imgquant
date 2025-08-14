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

		void write_png_rgba(const std::string &path, uint32_t width, uint32_t height, const Pixel * const*row_pointers) const
		{
			auto fp = File(path.c_str(), "wb");

			png_init_io(p, fp.get());

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

		void write_png_8bit(const std::string &path, uint32_t width, uint32_t height, const Rgb *palette, size_t num_palette_entries, const uint8_t *const*row_pointers) const
		{
			auto fp = File(path.c_str(), "wb");

			png_init_io(p, fp.get());

			png_set_IHDR(p, i, width, height, 8,
				PNG_COLOR_TYPE_PALETTE,
				PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);

			png_set_PLTE(p, i, reinterpret_cast<png_const_colorp>(&palette[0]), static_cast<int>(num_palette_entries));
			png_write_info(p, i);
			png_write_image(p, const_cast<uint8_t **>(reinterpret_cast<const uint8_t *const*>(row_pointers)));
			png_write_end(p, i);
		}

	private:
		static void on_png_error(void *self, const char *message)
		{
			throw SavePngException(std::string(message));
		}
	};

	PngImage::PngImage(uint32_t width, uint32_t height, const Pixel &default_color) :
		width(width), height(height)
	{
		pixels.resize(static_cast<size_t>(width) * height);
		for (size_t i = 0; i < pixels.size(); i++) pixels[i] = default_color;
		for (size_t y = 0; y < height; y++) row_pointers.push_back(&pixels[y * width]);
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
			width = image.width;
			height = image.height;
			pixels.resize(static_cast<size_t>(width) * height);
			for (size_t y = 0; y < height; y++) row_pointers.push_back(&pixels[y * width]);
			if (!png_image_finish_read(&image, NULL, &pixels[0], 0, NULL))
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
		return width;
	}
	uint32_t PngImage::get_height() const
	{
		return height;
	}
	Pixel **PngImage::get_row_pointers()
	{
		return &row_pointers[0];
	}
	const Pixel *const *PngImage::get_row_pointers() const
	{
		return &row_pointers[0];
	}
	const Pixel &PngImage::get_pixel(uint32_t x, uint32_t y) const
	{
		if (x >= width || y >= height) throw InvalidArgument(std::string("Invalid coordinate (") + std::to_string(x) + ", " + std::to_string(y) + ")");
		return row_pointers[y][x];
	}
	Pixel &PngImage::get_pixel(uint32_t x, uint32_t y)
	{
		if (x >= width || y >= height) throw InvalidArgument(std::string("Invalid coordinate (") + std::to_string(x) + ", " + std::to_string(y) + ")");
		return row_pointers[y][x];
	}
	void PngImage::set_pixel(uint32_t x, uint32_t y, const Pixel &pixel)
	{
		if (x >= width || y >= height) throw InvalidArgument(std::string("Invalid coordinate (") + std::to_string(x) + ", " + std::to_string(y) + ")");
		row_pointers[y][x] = pixel;
	}
	void PngImage::save_png32_to(const std::string &path) const
	{
		auto w = png_writer();
		w.write_png_rgba(path, width, height, &row_pointers[0]);
	}
	void PngImage::save_png8_to(const std::string &path, uint32_t width, uint32_t height, const Rgb *palette, size_t num_palette_entries, const uint8_t *const *row_pointers)
	{
		auto w = png_writer();
		w.write_png_8bit(path, width, height, palette, num_palette_entries, &row_pointers[0]);
	}

}
