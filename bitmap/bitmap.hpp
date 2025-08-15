#pragma once

#include <cinttypes>
#include <cstddef>

#include <vector>

#include "coldef.hpp"

namespace bitmap
{
	using namespace coldef;

	template<typename T>
	class Bitmap
	{
	protected:
		T* bitmap = nullptr;
		std::vector<T*> row_pointers;
		size_t pitch = 0;
		uint32_t width = 0;
		uint32_t height = 0;

		static size_t calc_pitch(uint32_t width)
		{
			return ((static_cast<size_t>(width) * sizeof(T) - 1) / 4 + 1) * 4;
		}
		void build_bitmap();
		void copy_bitmap(const T*const* from_row_pointers);

	public:
		Bitmap() = default;
		~Bitmap();
		Bitmap(const Bitmap& copy);
		Bitmap(Bitmap&& move) noexcept;
		Bitmap& operator =(const Bitmap& to);
		Bitmap(uint32_t width, uint32_t height);
		Bitmap(uint32_t width, uint32_t height, const T& default_color);
		Bitmap(uint32_t width, uint32_t height, const T*const* from_row_pointers);

		T* get_bitmap();
		const T* get_bitmap() const;
		T** get_row_pointers();
		const T*const* get_row_pointers() const;
		T* get_row(size_t y);
		const T* get_row(size_t y) const;
		T& get_pixel(uint32_t x, uint32_t y);
		const T& get_pixel(uint32_t x, uint32_t y) const;
		void set_pixel(uint32_t x, uint32_t y, const T& v);
		uint32_t get_width() const;
		uint32_t get_height() const;
		size_t get_pitch() const;
	};

	extern template class Bitmap<int>;
	extern template class Bitmap<uint8_t>;
	extern template class Bitmap<ColorRgb>;
	extern template class Bitmap<ColorRgba>;
	extern template class Bitmap<IColorRgb>;
	extern template class Bitmap<IColorRgba>;

	template<typename T_src, typename T_dst>
	Bitmap<T_dst> convert(const Bitmap<T_src>& src, T_dst(*convert_fn)(const T_src& src))
	{
		auto ret = Bitmap<T_dst>(src.get_width(), src.get_height());
#pragma omp parallel for
		for (std::ptrdiff_t y = 0; y < static_cast<std::ptrdiff_t>(src.get_height()); y++)
		{
			auto src_row = src.get_row(y);
			auto dst_row = ret.get_row(y);
			for (size_t x = 0; x < src.get_width(); x++)
			{
				dst_row[x] = convert_fn(src_row[x]);
			}
		}

		return ret;
	}

	extern template Bitmap<int       > convert(const Bitmap<int       >& src, int       (*convert_fn)(const int       & src));
	extern template Bitmap<uint8_t   > convert(const Bitmap<int       >& src, uint8_t   (*convert_fn)(const int       & src));
	extern template Bitmap<ColorRgb  > convert(const Bitmap<int       >& src, ColorRgb  (*convert_fn)(const int       & src));
	extern template Bitmap<ColorRgba > convert(const Bitmap<int       >& src, ColorRgba (*convert_fn)(const int       & src));
	extern template Bitmap<IColorRgb > convert(const Bitmap<int       >& src, IColorRgb (*convert_fn)(const int       & src));
	extern template Bitmap<IColorRgba> convert(const Bitmap<int       >& src, IColorRgba(*convert_fn)(const int       & src));
	extern template Bitmap<int       > convert(const Bitmap<uint8_t   >& src, int       (*convert_fn)(const uint8_t   & src));
	extern template Bitmap<uint8_t   > convert(const Bitmap<uint8_t   >& src, uint8_t   (*convert_fn)(const uint8_t   & src));
	extern template Bitmap<ColorRgb  > convert(const Bitmap<uint8_t   >& src, ColorRgb  (*convert_fn)(const uint8_t   & src));
	extern template Bitmap<ColorRgba > convert(const Bitmap<uint8_t   >& src, ColorRgba (*convert_fn)(const uint8_t   & src));
	extern template Bitmap<IColorRgb > convert(const Bitmap<uint8_t   >& src, IColorRgb (*convert_fn)(const uint8_t   & src));
	extern template Bitmap<IColorRgba> convert(const Bitmap<uint8_t   >& src, IColorRgba(*convert_fn)(const uint8_t   & src));
	extern template Bitmap<int       > convert(const Bitmap<ColorRgb  >& src, int       (*convert_fn)(const ColorRgb  & src));
	extern template Bitmap<uint8_t   > convert(const Bitmap<ColorRgb  >& src, uint8_t   (*convert_fn)(const ColorRgb  & src));
	extern template Bitmap<ColorRgb  > convert(const Bitmap<ColorRgb  >& src, ColorRgb  (*convert_fn)(const ColorRgb  & src));
	extern template Bitmap<ColorRgba > convert(const Bitmap<ColorRgb  >& src, ColorRgba (*convert_fn)(const ColorRgb  & src));
	extern template Bitmap<IColorRgb > convert(const Bitmap<ColorRgb  >& src, IColorRgb (*convert_fn)(const ColorRgb  & src));
	extern template Bitmap<IColorRgba> convert(const Bitmap<ColorRgb  >& src, IColorRgba(*convert_fn)(const ColorRgb  & src));
	extern template Bitmap<int       > convert(const Bitmap<ColorRgba >& src, int       (*convert_fn)(const ColorRgba & src));
	extern template Bitmap<uint8_t   > convert(const Bitmap<ColorRgba >& src, uint8_t   (*convert_fn)(const ColorRgba & src));
	extern template Bitmap<ColorRgb  > convert(const Bitmap<ColorRgba >& src, ColorRgb  (*convert_fn)(const ColorRgba & src));
	extern template Bitmap<ColorRgba > convert(const Bitmap<ColorRgba >& src, ColorRgba (*convert_fn)(const ColorRgba & src));
	extern template Bitmap<IColorRgb > convert(const Bitmap<ColorRgba >& src, IColorRgb (*convert_fn)(const ColorRgba & src));
	extern template Bitmap<IColorRgba> convert(const Bitmap<ColorRgba >& src, IColorRgba(*convert_fn)(const ColorRgba & src));
	extern template Bitmap<int       > convert(const Bitmap<IColorRgb >& src, int       (*convert_fn)(const IColorRgb & src));
	extern template Bitmap<uint8_t   > convert(const Bitmap<IColorRgb >& src, uint8_t   (*convert_fn)(const IColorRgb & src));
	extern template Bitmap<ColorRgb  > convert(const Bitmap<IColorRgb >& src, ColorRgb  (*convert_fn)(const IColorRgb & src));
	extern template Bitmap<ColorRgba > convert(const Bitmap<IColorRgb >& src, ColorRgba (*convert_fn)(const IColorRgb & src));
	extern template Bitmap<IColorRgb > convert(const Bitmap<IColorRgb >& src, IColorRgb (*convert_fn)(const IColorRgb & src));
	extern template Bitmap<IColorRgba> convert(const Bitmap<IColorRgb >& src, IColorRgba(*convert_fn)(const IColorRgb & src));
	extern template Bitmap<int       > convert(const Bitmap<IColorRgba>& src, int       (*convert_fn)(const IColorRgba& src));
	extern template Bitmap<uint8_t   > convert(const Bitmap<IColorRgba>& src, uint8_t   (*convert_fn)(const IColorRgba& src));
	extern template Bitmap<ColorRgb  > convert(const Bitmap<IColorRgba>& src, ColorRgb  (*convert_fn)(const IColorRgba& src));
	extern template Bitmap<ColorRgba > convert(const Bitmap<IColorRgba>& src, ColorRgba (*convert_fn)(const IColorRgba& src));
	extern template Bitmap<IColorRgb > convert(const Bitmap<IColorRgba>& src, IColorRgb (*convert_fn)(const IColorRgba& src));
	extern template Bitmap<IColorRgba> convert(const Bitmap<IColorRgba>& src, IColorRgba(*convert_fn)(const IColorRgba& src));
};

