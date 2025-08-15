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
		std::vector<T> bitmap;
		std::vector<T*> row_pointers;
		size_t pitch;
		uint32_t width;
		uint32_t height;

		static size_t calc_pitch(uint32_t width)
		{
			return ((static_cast<size_t>(width) * sizeof(T) - 1) / 4 + 1) * 4;
		}
		void build_bitmap()
		{
			bitmap.resize(pitch * height);
			row_pointers.resize(height);
			for (size_t y = 0; y < height; y++)
			{
				row_pointers[y] = reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(&bitmap[0]) + pitch);
			}
		}

	public:
		Bitmap() = default;
		Bitmap(const Bitmap& copy) : Bitmap(copy.width, copy.height, copy.get_row_pointers())
		{
		}
		Bitmap(Bitmap&& move) :
			bitmap(std::move(move.bitmap)),
			row_pointers(std::move(move.row_pointers)),
			pitch(move.pitch),
			width(move.width),
			height(move.height)
		{
		}
		Bitmap& operator =(const Bitmap& to) = default;
		Bitmap(uint32_t width, uint32_t height) : width(width), height(height), pitch(calc_pitch(width))
		{
			build_bitmap();
		}
		Bitmap(uint32_t width, uint32_t height, const T& default_color) : width(width), height(height), pitch(calc_pitch(width))
		{
			build_bitmap();
#pragma omp parallel for
			for (std::ptrdiff_t y = 0; y < static_cast<std::ptrdiff_t>(height); y++)
			{
				auto dst_row = row_pointers[y];
				for (size_t x = 0; x < width; x++)
				{
					dst_row[x] = default_color;
				}
			}
		}
		Bitmap(uint32_t width, uint32_t height, const T*const* from_row_pointers) : width(width), height(height), pitch(calc_pitch(width))
		{
			build_bitmap();
#pragma omp parallel for
			for (std::ptrdiff_t y = 0; y < static_cast<std::ptrdiff_t>(height); y++)
			{
				auto src_row = from_row_pointers[y];
				auto dst_row = row_pointers[y];
				for (size_t x = 0; x < width; x++)
				{
					dst_row[x] = src_row[x];
				}
			}
		}
		T* get_bitmap() { return &bitmap[0]; }
		const T* get_bitmap() const { return &bitmap[0]; }
		T** get_row_pointers() { return &row_pointers[0]; }
		const T*const* get_row_pointers() const { return &row_pointers[0]; }
		T* get_row(size_t y) { return row_pointers[y]; }
		const T* get_row(size_t y) const { return row_pointers[y]; }
		T& get_pixel(uint32_t x, uint32_t y) { return row_pointers[y][x]; }
		const T& get_pixel(uint32_t x, uint32_t y) const { return row_pointers[y][x]; }
		void set_pixel(uint32_t x, uint32_t y, const T& v) { row_pointers[y][x] = v; }

		uint32_t get_width() const {return width;}
		uint32_t get_height() const { return height; }
		size_t get_pitch() const { return pitch; }

		template<typename T_dst>
		Bitmap<T_dst> convert(T_dst(*convert_fn)(T& src)) const
		{
			auto ret = Bitmap<T_dst>(width, height);
			auto ret_row = ret.get_row_pointers();
#pragma omp parallel for
			for (std::ptrdiff_t y = 0; y < static_cast<std::ptrdiff_t>(height); y++)
			{
				auto src_row = row_pointers[y];
				auto dst_row = ret_row[y];
				for (size_t x = 0; x < width; x++)
				{
					dst_row[x] = convert_fn(src_row[x]);
				}
			}

			return ret;
		}
	};

	extern template class Bitmap<int     >;
	extern template class Bitmap<uint8_t >;
	extern template class Bitmap<ColorRgb >;
	extern template class Bitmap<ColorRgba >;
	extern template class Bitmap<IColorRgb>;
	extern template class Bitmap<IColorRgba>;

	extern template Bitmap<int       > Bitmap<int       >::convert(int       (*convert_fn)(int       & src)) const;
	extern template Bitmap<uint8_t   > Bitmap<int       >::convert(uint8_t   (*convert_fn)(int       & src)) const;
	extern template Bitmap<ColorRgb  > Bitmap<int       >::convert(ColorRgb  (*convert_fn)(int       & src)) const;
	extern template Bitmap<ColorRgba > Bitmap<int       >::convert(ColorRgba (*convert_fn)(int       & src)) const;
	extern template Bitmap<IColorRgb > Bitmap<int       >::convert(IColorRgb (*convert_fn)(int       & src)) const;
	extern template Bitmap<IColorRgba> Bitmap<int       >::convert(IColorRgba(*convert_fn)(int       & src)) const;
	extern template Bitmap<int       > Bitmap<uint8_t   >::convert(int       (*convert_fn)(uint8_t   & src)) const;
	extern template Bitmap<uint8_t   > Bitmap<uint8_t   >::convert(uint8_t   (*convert_fn)(uint8_t   & src)) const;
	extern template Bitmap<ColorRgb  > Bitmap<uint8_t   >::convert(ColorRgb  (*convert_fn)(uint8_t   & src)) const;
	extern template Bitmap<ColorRgba > Bitmap<uint8_t   >::convert(ColorRgba (*convert_fn)(uint8_t   & src)) const;
	extern template Bitmap<IColorRgb > Bitmap<uint8_t   >::convert(IColorRgb (*convert_fn)(uint8_t   & src)) const;
	extern template Bitmap<IColorRgba> Bitmap<uint8_t   >::convert(IColorRgba(*convert_fn)(uint8_t   & src)) const;
	extern template Bitmap<int       > Bitmap<ColorRgb  >::convert(int       (*convert_fn)(ColorRgb  & src)) const;
	extern template Bitmap<uint8_t   > Bitmap<ColorRgb  >::convert(uint8_t   (*convert_fn)(ColorRgb  & src)) const;
	extern template Bitmap<ColorRgb  > Bitmap<ColorRgb  >::convert(ColorRgb  (*convert_fn)(ColorRgb  & src)) const;
	extern template Bitmap<ColorRgba > Bitmap<ColorRgb  >::convert(ColorRgba (*convert_fn)(ColorRgb  & src)) const;
	extern template Bitmap<IColorRgb > Bitmap<ColorRgb  >::convert(IColorRgb (*convert_fn)(ColorRgb  & src)) const;
	extern template Bitmap<IColorRgba> Bitmap<ColorRgb  >::convert(IColorRgba(*convert_fn)(ColorRgb  & src)) const;
	extern template Bitmap<int       > Bitmap<ColorRgba >::convert(int       (*convert_fn)(ColorRgba & src)) const;
	extern template Bitmap<uint8_t   > Bitmap<ColorRgba >::convert(uint8_t   (*convert_fn)(ColorRgba & src)) const;
	extern template Bitmap<ColorRgb  > Bitmap<ColorRgba >::convert(ColorRgb  (*convert_fn)(ColorRgba & src)) const;
	extern template Bitmap<ColorRgba > Bitmap<ColorRgba >::convert(ColorRgba (*convert_fn)(ColorRgba & src)) const;
	extern template Bitmap<IColorRgb > Bitmap<ColorRgba >::convert(IColorRgb (*convert_fn)(ColorRgba & src)) const;
	extern template Bitmap<IColorRgba> Bitmap<ColorRgba >::convert(IColorRgba(*convert_fn)(ColorRgba & src)) const;
	extern template Bitmap<int       > Bitmap<IColorRgb >::convert(int       (*convert_fn)(IColorRgb & src)) const;
	extern template Bitmap<uint8_t   > Bitmap<IColorRgb >::convert(uint8_t   (*convert_fn)(IColorRgb & src)) const;
	extern template Bitmap<ColorRgb  > Bitmap<IColorRgb >::convert(ColorRgb  (*convert_fn)(IColorRgb & src)) const;
	extern template Bitmap<ColorRgba > Bitmap<IColorRgb >::convert(ColorRgba (*convert_fn)(IColorRgb & src)) const;
	extern template Bitmap<IColorRgb > Bitmap<IColorRgb >::convert(IColorRgb (*convert_fn)(IColorRgb & src)) const;
	extern template Bitmap<IColorRgba> Bitmap<IColorRgb >::convert(IColorRgba(*convert_fn)(IColorRgb & src)) const;
	extern template Bitmap<int       > Bitmap<IColorRgba>::convert(int       (*convert_fn)(IColorRgba& src)) const;
	extern template Bitmap<uint8_t   > Bitmap<IColorRgba>::convert(uint8_t   (*convert_fn)(IColorRgba& src)) const;
	extern template Bitmap<ColorRgb  > Bitmap<IColorRgba>::convert(ColorRgb  (*convert_fn)(IColorRgba& src)) const;
	extern template Bitmap<ColorRgba > Bitmap<IColorRgba>::convert(ColorRgba (*convert_fn)(IColorRgba& src)) const;
	extern template Bitmap<IColorRgb > Bitmap<IColorRgba>::convert(IColorRgb (*convert_fn)(IColorRgba& src)) const;
	extern template Bitmap<IColorRgba> Bitmap<IColorRgba>::convert(IColorRgba(*convert_fn)(IColorRgba& src)) const;
};

