#pragma once

#include <cinttypes>
#include <vector>
#include <string>
#include <stdexcept>

#include <bitmap/bitmap.hpp>

namespace pngcpp
{
	using namespace bitmap;

	class InvalidArgument : public std::runtime_error {
	public:
		InvalidArgument(const std::string &what) noexcept;
	};

	class LoadPngException : public std::runtime_error {
	public:
		LoadPngException(const std::string &what) noexcept;
	};

	class SavePngException : public std::runtime_error {
	public:
		SavePngException(const std::string &what) noexcept;
	};

	class PngImage
	{
	protected:
		Bitmap<ColorRgba> bmp;

	public:
		PngImage() = delete;
		PngImage(PngImage &&) = default;
		PngImage(const PngImage &) = default;
		PngImage(uint32_t width, uint32_t height, const ColorRgba& default_color);
		PngImage(const char* path);
		PngImage(const std::string& path);

		uint32_t get_width() const;
		uint32_t get_height() const;

		Bitmap<ColorRgba>& get_bmp() { return bmp; }
		const Bitmap<ColorRgba>& get_bmp() const { return bmp; }
		ColorRgba **get_row_pointers();
		const ColorRgba *const *get_row_pointers() const;
		const ColorRgba &get_pixel(uint32_t x, uint32_t y) const;
		ColorRgba &get_pixel(uint32_t x, uint32_t y);
		void set_pixel(uint32_t x, uint32_t y, const ColorRgba& pixel);

		void save_png32_to(const std::string &path) const;
		static void save_png32_to(const std::string &path, uint32_t width, uint32_t height, const ColorRgba *const *row_pointers);
		static void save_png8_to(const std::string &path, uint32_t width, uint32_t height, const ColorRgb *palette, size_t num_palette_entries, const uint8_t *const *row_pointers);
		static void save_png8_to(const std::string &path, uint32_t width, uint32_t height, const ColorRgba *palette, size_t num_palette_entries, const uint8_t *const *row_pointers);
	};
};
