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

		template<typename T_dst>
		Bitmap<T_dst> convert(T_dst(*convert_fn)(T& src)) const;
	};

	extern template class Bitmap<int>;
	extern template class Bitmap<uint8_t>;
	extern template class Bitmap<ColorRgb>;
	extern template class Bitmap<ColorRgba>;
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

