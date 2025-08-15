#pragma once

#include <cinttypes>
#include <cstddef>
#include <vector>

#include "paldef.hpp"

namespace bitmap
{
	using namespace paldef;

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
		Bitmap() = delete;
		Bitmap(uint32_t width, uint32_t height) : width(width), height(height), pitch(calc_pitch(width))
		{
			build_bitmap();
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
		T** get_row_pointers() { return &row_pointers[0]; }
		const T*const* get_row_pointers() const { return &row_pointers[0]; }
		T* get_row(uint32_t y) { return &row_pointers[y]; }
		const T* get_row(uint32_t y) const { return &row_pointers[y]; }
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
	extern template class Bitmap<Color24 >;
	extern template class Bitmap<Color32 >;
	extern template class Bitmap<IColor24>;
	extern template class Bitmap<IColor32>;

	extern template Bitmap<int     > Bitmap<int     >::convert(int     (*convert_fn)(int     & src)) const;
	extern template Bitmap<uint8_t > Bitmap<int     >::convert(uint8_t(*convert_fn)(int     & src)) const;
	extern template Bitmap<Color24 > Bitmap<int     >::convert(Color24(*convert_fn)(int     & src)) const;
	extern template Bitmap<Color32 > Bitmap<int     >::convert(Color32(*convert_fn)(int     & src)) const;
	extern template Bitmap<IColor24> Bitmap<int     >::convert(IColor24(*convert_fn)(int     & src)) const;
	extern template Bitmap<IColor32> Bitmap<int     >::convert(IColor32(*convert_fn)(int     & src)) const;
	extern template Bitmap<int     > Bitmap<uint8_t >::convert(int     (*convert_fn)(uint8_t & src)) const;
	extern template Bitmap<uint8_t > Bitmap<uint8_t >::convert(uint8_t(*convert_fn)(uint8_t & src)) const;
	extern template Bitmap<Color24 > Bitmap<uint8_t >::convert(Color24(*convert_fn)(uint8_t & src)) const;
	extern template Bitmap<Color32 > Bitmap<uint8_t >::convert(Color32(*convert_fn)(uint8_t & src)) const;
	extern template Bitmap<IColor24> Bitmap<uint8_t >::convert(IColor24(*convert_fn)(uint8_t & src)) const;
	extern template Bitmap<IColor32> Bitmap<uint8_t >::convert(IColor32(*convert_fn)(uint8_t & src)) const;
	extern template Bitmap<int     > Bitmap<Color24 >::convert(int     (*convert_fn)(Color24 & src)) const;
	extern template Bitmap<uint8_t > Bitmap<Color24 >::convert(uint8_t(*convert_fn)(Color24 & src)) const;
	extern template Bitmap<Color24 > Bitmap<Color24 >::convert(Color24(*convert_fn)(Color24 & src)) const;
	extern template Bitmap<Color32 > Bitmap<Color24 >::convert(Color32(*convert_fn)(Color24 & src)) const;
	extern template Bitmap<IColor24> Bitmap<Color24 >::convert(IColor24(*convert_fn)(Color24 & src)) const;
	extern template Bitmap<IColor32> Bitmap<Color24 >::convert(IColor32(*convert_fn)(Color24 & src)) const;
	extern template Bitmap<int     > Bitmap<Color32 >::convert(int     (*convert_fn)(Color32 & src)) const;
	extern template Bitmap<uint8_t > Bitmap<Color32 >::convert(uint8_t(*convert_fn)(Color32 & src)) const;
	extern template Bitmap<Color24 > Bitmap<Color32 >::convert(Color24(*convert_fn)(Color32 & src)) const;
	extern template Bitmap<Color32 > Bitmap<Color32 >::convert(Color32(*convert_fn)(Color32 & src)) const;
	extern template Bitmap<IColor24> Bitmap<Color32 >::convert(IColor24(*convert_fn)(Color32 & src)) const;
	extern template Bitmap<IColor32> Bitmap<Color32 >::convert(IColor32(*convert_fn)(Color32 & src)) const;
	extern template Bitmap<int     > Bitmap<IColor24>::convert(int     (*convert_fn)(IColor24& src)) const;
	extern template Bitmap<uint8_t > Bitmap<IColor24>::convert(uint8_t(*convert_fn)(IColor24& src)) const;
	extern template Bitmap<Color24 > Bitmap<IColor24>::convert(Color24(*convert_fn)(IColor24& src)) const;
	extern template Bitmap<Color32 > Bitmap<IColor24>::convert(Color32(*convert_fn)(IColor24& src)) const;
	extern template Bitmap<IColor24> Bitmap<IColor24>::convert(IColor24(*convert_fn)(IColor24& src)) const;
	extern template Bitmap<IColor32> Bitmap<IColor24>::convert(IColor32(*convert_fn)(IColor24& src)) const;
	extern template Bitmap<int     > Bitmap<IColor32>::convert(int     (*convert_fn)(IColor32& src)) const;
	extern template Bitmap<uint8_t > Bitmap<IColor32>::convert(uint8_t(*convert_fn)(IColor32& src)) const;
	extern template Bitmap<Color24 > Bitmap<IColor32>::convert(Color24(*convert_fn)(IColor32& src)) const;
	extern template Bitmap<Color32 > Bitmap<IColor32>::convert(Color32(*convert_fn)(IColor32& src)) const;
	extern template Bitmap<IColor24> Bitmap<IColor32>::convert(IColor24(*convert_fn)(IColor32& src)) const;
	extern template Bitmap<IColor32> Bitmap<IColor32>::convert(IColor32(*convert_fn)(IColor32& src)) const;
};

