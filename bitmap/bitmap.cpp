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
	T* Bitmap<T>::get_bitmap()
	{
		return bitmap;
	}

	template<typename T>
	const T* Bitmap<T>::get_bitmap() const
	{
		return bitmap;
	}

	template<typename T>
	T** Bitmap<T>::get_row_pointers()
	{
		return &row_pointers[0];
	}

	template<typename T>
	const T*const* Bitmap<T>::get_row_pointers() const
	{
		return &row_pointers[0];
	}

	template<typename T>
	T* Bitmap<T>::get_row(size_t y)
	{
		return row_pointers[y];
	}

	template<typename T>
	const T* Bitmap<T>::get_row(size_t y) const
	{
		return row_pointers[y];
	}

	template<typename T>
	T& Bitmap<T>::get_pixel(uint32_t x, uint32_t y)
	{
		return row_pointers[y][x];
	}

	template<typename T>
	const T& Bitmap<T>::get_pixel(uint32_t x, uint32_t y) const
	{
		return row_pointers[y][x];
	}

	template<typename T>
	void Bitmap<T>::set_pixel(uint32_t x, uint32_t y, const T& v)
	{
		row_pointers[y][x] = v;
	}

	template<typename T>
	uint32_t Bitmap<T>::get_width() const
	{
		return width;
	}

	template<typename T>
	uint32_t Bitmap<T>::get_height() const
	{
		return height;
	}

	template<typename T>
	size_t Bitmap<T>::get_pitch() const
	{
		return pitch;
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

	template Bitmap<int       > convert(const Bitmap<int       >& src, int       (*convert_fn)(const int       & src));
	template Bitmap<uint8_t   > convert(const Bitmap<int       >& src, uint8_t   (*convert_fn)(const int       & src));
	template Bitmap<ColorRgb  > convert(const Bitmap<int       >& src, ColorRgb  (*convert_fn)(const int       & src));
	template Bitmap<ColorRgba > convert(const Bitmap<int       >& src, ColorRgba (*convert_fn)(const int       & src));
	template Bitmap<IColorRgb > convert(const Bitmap<int       >& src, IColorRgb (*convert_fn)(const int       & src));
	template Bitmap<IColorRgba> convert(const Bitmap<int       >& src, IColorRgba(*convert_fn)(const int       & src));
	template Bitmap<int       > convert(const Bitmap<uint8_t   >& src, int       (*convert_fn)(const uint8_t   & src));
	template Bitmap<uint8_t   > convert(const Bitmap<uint8_t   >& src, uint8_t   (*convert_fn)(const uint8_t   & src));
	template Bitmap<ColorRgb  > convert(const Bitmap<uint8_t   >& src, ColorRgb  (*convert_fn)(const uint8_t   & src));
	template Bitmap<ColorRgba > convert(const Bitmap<uint8_t   >& src, ColorRgba (*convert_fn)(const uint8_t   & src));
	template Bitmap<IColorRgb > convert(const Bitmap<uint8_t   >& src, IColorRgb (*convert_fn)(const uint8_t   & src));
	template Bitmap<IColorRgba> convert(const Bitmap<uint8_t   >& src, IColorRgba(*convert_fn)(const uint8_t   & src));
	template Bitmap<int       > convert(const Bitmap<ColorRgb  >& src, int       (*convert_fn)(const ColorRgb  & src));
	template Bitmap<uint8_t   > convert(const Bitmap<ColorRgb  >& src, uint8_t   (*convert_fn)(const ColorRgb  & src));
	template Bitmap<ColorRgb  > convert(const Bitmap<ColorRgb  >& src, ColorRgb  (*convert_fn)(const ColorRgb  & src));
	template Bitmap<ColorRgba > convert(const Bitmap<ColorRgb  >& src, ColorRgba (*convert_fn)(const ColorRgb  & src));
	template Bitmap<IColorRgb > convert(const Bitmap<ColorRgb  >& src, IColorRgb (*convert_fn)(const ColorRgb  & src));
	template Bitmap<IColorRgba> convert(const Bitmap<ColorRgb  >& src, IColorRgba(*convert_fn)(const ColorRgb  & src));
	template Bitmap<int       > convert(const Bitmap<ColorRgba >& src, int       (*convert_fn)(const ColorRgba & src));
	template Bitmap<uint8_t   > convert(const Bitmap<ColorRgba >& src, uint8_t   (*convert_fn)(const ColorRgba & src));
	template Bitmap<ColorRgb  > convert(const Bitmap<ColorRgba >& src, ColorRgb  (*convert_fn)(const ColorRgba & src));
	template Bitmap<ColorRgba > convert(const Bitmap<ColorRgba >& src, ColorRgba (*convert_fn)(const ColorRgba & src));
	template Bitmap<IColorRgb > convert(const Bitmap<ColorRgba >& src, IColorRgb (*convert_fn)(const ColorRgba & src));
	template Bitmap<IColorRgba> convert(const Bitmap<ColorRgba >& src, IColorRgba(*convert_fn)(const ColorRgba & src));
	template Bitmap<int       > convert(const Bitmap<IColorRgb >& src, int       (*convert_fn)(const IColorRgb & src));
	template Bitmap<uint8_t   > convert(const Bitmap<IColorRgb >& src, uint8_t   (*convert_fn)(const IColorRgb & src));
	template Bitmap<ColorRgb  > convert(const Bitmap<IColorRgb >& src, ColorRgb  (*convert_fn)(const IColorRgb & src));
	template Bitmap<ColorRgba > convert(const Bitmap<IColorRgb >& src, ColorRgba (*convert_fn)(const IColorRgb & src));
	template Bitmap<IColorRgb > convert(const Bitmap<IColorRgb >& src, IColorRgb (*convert_fn)(const IColorRgb & src));
	template Bitmap<IColorRgba> convert(const Bitmap<IColorRgb >& src, IColorRgba(*convert_fn)(const IColorRgb & src));
	template Bitmap<int       > convert(const Bitmap<IColorRgba>& src, int       (*convert_fn)(const IColorRgba& src));
	template Bitmap<uint8_t   > convert(const Bitmap<IColorRgba>& src, uint8_t   (*convert_fn)(const IColorRgba& src));
	template Bitmap<ColorRgb  > convert(const Bitmap<IColorRgba>& src, ColorRgb  (*convert_fn)(const IColorRgba& src));
	template Bitmap<ColorRgba > convert(const Bitmap<IColorRgba>& src, ColorRgba (*convert_fn)(const IColorRgba& src));
	template Bitmap<IColorRgb > convert(const Bitmap<IColorRgba>& src, IColorRgb (*convert_fn)(const IColorRgba& src));
	template Bitmap<IColorRgba> convert(const Bitmap<IColorRgba>& src, IColorRgba(*convert_fn)(const IColorRgba& src));
};

