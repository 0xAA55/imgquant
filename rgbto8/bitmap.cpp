#include "bitmap.hpp"

namespace bitmap
{
	template class Bitmap<int>;
	template class Bitmap<uint8_t>;
	template class Bitmap<Color24>;
	template class Bitmap<Color32>;
	template class Bitmap<IColor24>;
	template class Bitmap<IColor32>;

	template Bitmap<int     > Bitmap<int     >::convert(int     (*convert_fn)(int     & src)) const;
	template Bitmap<uint8_t > Bitmap<int     >::convert(uint8_t (*convert_fn)(int     & src)) const;
	template Bitmap<Color24 > Bitmap<int     >::convert(Color24 (*convert_fn)(int     & src)) const;
	template Bitmap<Color32 > Bitmap<int     >::convert(Color32 (*convert_fn)(int     & src)) const;
	template Bitmap<IColor24> Bitmap<int     >::convert(IColor24(*convert_fn)(int     & src)) const;
	template Bitmap<IColor32> Bitmap<int     >::convert(IColor32(*convert_fn)(int     & src)) const;
	template Bitmap<int     > Bitmap<uint8_t >::convert(int     (*convert_fn)(uint8_t & src)) const;
	template Bitmap<uint8_t > Bitmap<uint8_t >::convert(uint8_t (*convert_fn)(uint8_t & src)) const;
	template Bitmap<Color24 > Bitmap<uint8_t >::convert(Color24 (*convert_fn)(uint8_t & src)) const;
	template Bitmap<Color32 > Bitmap<uint8_t >::convert(Color32 (*convert_fn)(uint8_t & src)) const;
	template Bitmap<IColor24> Bitmap<uint8_t >::convert(IColor24(*convert_fn)(uint8_t & src)) const;
	template Bitmap<IColor32> Bitmap<uint8_t >::convert(IColor32(*convert_fn)(uint8_t & src)) const;
	template Bitmap<int     > Bitmap<Color24 >::convert(int     (*convert_fn)(Color24 & src)) const;
	template Bitmap<uint8_t > Bitmap<Color24 >::convert(uint8_t (*convert_fn)(Color24 & src)) const;
	template Bitmap<Color24 > Bitmap<Color24 >::convert(Color24 (*convert_fn)(Color24 & src)) const;
	template Bitmap<Color32 > Bitmap<Color24 >::convert(Color32 (*convert_fn)(Color24 & src)) const;
	template Bitmap<IColor24> Bitmap<Color24 >::convert(IColor24(*convert_fn)(Color24 & src)) const;
	template Bitmap<IColor32> Bitmap<Color24 >::convert(IColor32(*convert_fn)(Color24 & src)) const;
	template Bitmap<int     > Bitmap<Color32 >::convert(int     (*convert_fn)(Color32 & src)) const;
	template Bitmap<uint8_t > Bitmap<Color32 >::convert(uint8_t (*convert_fn)(Color32 & src)) const;
	template Bitmap<Color24 > Bitmap<Color32 >::convert(Color24 (*convert_fn)(Color32 & src)) const;
	template Bitmap<Color32 > Bitmap<Color32 >::convert(Color32 (*convert_fn)(Color32 & src)) const;
	template Bitmap<IColor24> Bitmap<Color32 >::convert(IColor24(*convert_fn)(Color32 & src)) const;
	template Bitmap<IColor32> Bitmap<Color32 >::convert(IColor32(*convert_fn)(Color32 & src)) const;
	template Bitmap<int     > Bitmap<IColor24>::convert(int     (*convert_fn)(IColor24& src)) const;
	template Bitmap<uint8_t > Bitmap<IColor24>::convert(uint8_t (*convert_fn)(IColor24& src)) const;
	template Bitmap<Color24 > Bitmap<IColor24>::convert(Color24 (*convert_fn)(IColor24& src)) const;
	template Bitmap<Color32 > Bitmap<IColor24>::convert(Color32 (*convert_fn)(IColor24& src)) const;
	template Bitmap<IColor24> Bitmap<IColor24>::convert(IColor24(*convert_fn)(IColor24& src)) const;
	template Bitmap<IColor32> Bitmap<IColor24>::convert(IColor32(*convert_fn)(IColor24& src)) const;
	template Bitmap<int     > Bitmap<IColor32>::convert(int     (*convert_fn)(IColor32& src)) const;
	template Bitmap<uint8_t > Bitmap<IColor32>::convert(uint8_t (*convert_fn)(IColor32& src)) const;
	template Bitmap<Color24 > Bitmap<IColor32>::convert(Color24 (*convert_fn)(IColor32& src)) const;
	template Bitmap<Color32 > Bitmap<IColor32>::convert(Color32 (*convert_fn)(IColor32& src)) const;
	template Bitmap<IColor24> Bitmap<IColor32>::convert(IColor24(*convert_fn)(IColor32& src)) const;
	template Bitmap<IColor32> Bitmap<IColor32>::convert(IColor32(*convert_fn)(IColor32& src)) const;
};

