#pragma once

#include <cinttypes>
#include <vector>
#include <string>
#include <stdexcept>

namespace pngcpp
{
#pragma pack(push, 1)
	struct Rgb
	{
		uint8_t R;
		uint8_t G;
		uint8_t B;

		Rgb() = default;
		Rgb(uint8_t R, uint8_t G, uint8_t B) : R(R), G(G), B(B) {}
	};

	struct Rgba: public Rgb
	{
		uint8_t A;

		Rgba() = default;
		Rgba(uint8_t R, uint8_t G, uint8_t B, uint8_t A) : Rgb(R, G, B), A(A) {}
	};

	union Pixel
	{
		Rgba rgba;
		uint32_t u32;
	};
#pragma pack(pop)

	class InvalidArgument : public std::exception {
	public:
		InvalidArgument(const std::string &what) noexcept;
	};

	class LoadPngException : public std::exception {
	public:
		LoadPngException(const std::string &what) noexcept;
	};

	class SavePngException : public std::exception {
	public:
		SavePngException(const std::string &what) noexcept;
	};

	class PngImage
	{
	protected:
		std::vector<Pixel> pixels;
		std::vector<Pixel *> row_pointers;
		uint32_t width;
		uint32_t height;

	public:
		PngImage() = delete;
		PngImage(PngImage &&) = default;
		PngImage(const PngImage &) = default;
		PngImage(uint32_t width, uint32_t height, const Pixel& default_color);
		PngImage(const char* path);
		PngImage(const std::string& path);

		uint32_t get_width() const;
		uint32_t get_height() const;

		const Pixel * const* get_row_pointers() const;
		const Pixel &get_pixel(uint32_t x, uint32_t y) const;
		Pixel &get_pixel(uint32_t x, uint32_t y);
		void set_pixel(uint32_t x, uint32_t y, const Pixel& pixel);

		void save_png32_to(const std::string &path) const;
		static void save_png8_to(const std::string &path, uint32_t width, uint32_t height, const std::vector<Rgb> palette, const uint8_t *const *row_pointers);
	};
};
