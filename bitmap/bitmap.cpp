#include "bitmap.hpp"

#include <stdexcept>

#include <cstdlib>
#include <cassert>

namespace bitmap
{
	template<typename T>
	Bitmap<T>::Bitmap(const Bitmap& copy) :
		Bitmap(copy.width, copy.height, copy.get_row_pointers())
	{
	}

	template<typename T>
	Bitmap<T>& Bitmap<T>::operator =(const Bitmap& to)
	{
		free(bitmap);
		row_pointers.clear();
		width = to.width;
		height = to.height;
		pitch = to.pitch;
		build_bitmap();
		copy_bitmap(to.get_row_pointers());
		return *this;
	}

	template<typename T>
	Bitmap<T>::Bitmap(Bitmap&& move) noexcept :
		bitmap(move.bitmap),
		row_pointers(std::move(move.row_pointers)),
		pitch(move.pitch),
		width(move.width),
		height(move.height)
	{
		move.bitmap = nullptr;
	}

	template<typename T>
	Bitmap<T>::Bitmap(uint32_t width, uint32_t height) :
		width(width), height(height), pitch(calc_pitch(width))
	{
		build_bitmap();
	}

	template<typename T>
	Bitmap<T>::Bitmap(uint32_t width, uint32_t height, const T& default_color) :
		width(width), height(height), pitch(calc_pitch(width))
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

	template<typename T>
	Bitmap<T>::Bitmap(uint32_t width, uint32_t height, const T*const* from_row_pointers) :
		width(width), height(height), pitch(calc_pitch(width))
	{
		build_bitmap();
		copy_bitmap(from_row_pointers);
	}

	template<typename T>
	void Bitmap<T>::build_bitmap()
	{
		uint8_t *ptr = reinterpret_cast<uint8_t*>(malloc(pitch * height));
		if (!ptr) throw std::bad_alloc();
		bitmap = reinterpret_cast<T*>(ptr);
		row_pointers.resize(height);
		for (size_t y = 0; y < height; y++)
		{
			row_pointers[y] = reinterpret_cast<T*>(&ptr[pitch * y]);
		}
	}

	template<typename T>
	void Bitmap<T>::copy_bitmap(const T*const* from_row_pointers)
	{
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

	template<typename T>
	template<typename T_dst>
	Bitmap<T_dst> Bitmap<T>::convert(T_dst(*convert_fn)(T& src)) const
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

	template<typename T>
	Bitmap<T>::~Bitmap()
	{
		free(bitmap);
	}

	template class Bitmap<int>;
	template class Bitmap<uint8_t>;
	template class Bitmap<ColorRgb>;
	template class Bitmap<ColorRgba>;
	template class Bitmap<IColorRgb>;
	template class Bitmap<IColorRgba>;

	template Bitmap<int       > Bitmap<int       >::convert(int       (*convert_fn)(int       & src)) const;
	template Bitmap<uint8_t   > Bitmap<int       >::convert(uint8_t(*convert_fn)(int       & src)) const;
	template Bitmap<ColorRgb  > Bitmap<int       >::convert(ColorRgb(*convert_fn)(int       & src)) const;
	template Bitmap<ColorRgba > Bitmap<int       >::convert(ColorRgba(*convert_fn)(int       & src)) const;
	template Bitmap<IColorRgb > Bitmap<int       >::convert(IColorRgb(*convert_fn)(int       & src)) const;
	template Bitmap<IColorRgba> Bitmap<int       >::convert(IColorRgba(*convert_fn)(int       & src)) const;
	template Bitmap<int       > Bitmap<uint8_t   >::convert(int       (*convert_fn)(uint8_t   & src)) const;
	template Bitmap<uint8_t   > Bitmap<uint8_t   >::convert(uint8_t(*convert_fn)(uint8_t   & src)) const;
	template Bitmap<ColorRgb  > Bitmap<uint8_t   >::convert(ColorRgb(*convert_fn)(uint8_t   & src)) const;
	template Bitmap<ColorRgba > Bitmap<uint8_t   >::convert(ColorRgba(*convert_fn)(uint8_t   & src)) const;
	template Bitmap<IColorRgb > Bitmap<uint8_t   >::convert(IColorRgb(*convert_fn)(uint8_t   & src)) const;
	template Bitmap<IColorRgba> Bitmap<uint8_t   >::convert(IColorRgba(*convert_fn)(uint8_t   & src)) const;
	template Bitmap<int       > Bitmap<ColorRgb  >::convert(int       (*convert_fn)(ColorRgb  & src)) const;
	template Bitmap<uint8_t   > Bitmap<ColorRgb  >::convert(uint8_t(*convert_fn)(ColorRgb  & src)) const;
	template Bitmap<ColorRgb  > Bitmap<ColorRgb  >::convert(ColorRgb(*convert_fn)(ColorRgb  & src)) const;
	template Bitmap<ColorRgba > Bitmap<ColorRgb  >::convert(ColorRgba(*convert_fn)(ColorRgb  & src)) const;
	template Bitmap<IColorRgb > Bitmap<ColorRgb  >::convert(IColorRgb(*convert_fn)(ColorRgb  & src)) const;
	template Bitmap<IColorRgba> Bitmap<ColorRgb  >::convert(IColorRgba(*convert_fn)(ColorRgb  & src)) const;
	template Bitmap<int       > Bitmap<ColorRgba >::convert(int       (*convert_fn)(ColorRgba & src)) const;
	template Bitmap<uint8_t   > Bitmap<ColorRgba >::convert(uint8_t(*convert_fn)(ColorRgba & src)) const;
	template Bitmap<ColorRgb  > Bitmap<ColorRgba >::convert(ColorRgb(*convert_fn)(ColorRgba & src)) const;
	template Bitmap<ColorRgba > Bitmap<ColorRgba >::convert(ColorRgba(*convert_fn)(ColorRgba & src)) const;
	template Bitmap<IColorRgb > Bitmap<ColorRgba >::convert(IColorRgb(*convert_fn)(ColorRgba & src)) const;
	template Bitmap<IColorRgba> Bitmap<ColorRgba >::convert(IColorRgba(*convert_fn)(ColorRgba & src)) const;
	template Bitmap<int       > Bitmap<IColorRgb >::convert(int       (*convert_fn)(IColorRgb & src)) const;
	template Bitmap<uint8_t   > Bitmap<IColorRgb >::convert(uint8_t(*convert_fn)(IColorRgb & src)) const;
	template Bitmap<ColorRgb  > Bitmap<IColorRgb >::convert(ColorRgb(*convert_fn)(IColorRgb & src)) const;
	template Bitmap<ColorRgba > Bitmap<IColorRgb >::convert(ColorRgba(*convert_fn)(IColorRgb & src)) const;
	template Bitmap<IColorRgb > Bitmap<IColorRgb >::convert(IColorRgb(*convert_fn)(IColorRgb & src)) const;
	template Bitmap<IColorRgba> Bitmap<IColorRgb >::convert(IColorRgba(*convert_fn)(IColorRgb & src)) const;
	template Bitmap<int       > Bitmap<IColorRgba>::convert(int       (*convert_fn)(IColorRgba& src)) const;
	template Bitmap<uint8_t   > Bitmap<IColorRgba>::convert(uint8_t(*convert_fn)(IColorRgba& src)) const;
	template Bitmap<ColorRgb  > Bitmap<IColorRgba>::convert(ColorRgb(*convert_fn)(IColorRgba& src)) const;
	template Bitmap<ColorRgba > Bitmap<IColorRgba>::convert(ColorRgba(*convert_fn)(IColorRgba& src)) const;
	template Bitmap<IColorRgb > Bitmap<IColorRgba>::convert(IColorRgb(*convert_fn)(IColorRgba& src)) const;
	template Bitmap<IColorRgba> Bitmap<IColorRgba>::convert(IColorRgba(*convert_fn)(IColorRgba& src)) const;
};

