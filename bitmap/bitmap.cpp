#include "bitmap.hpp"

namespace bitmap
{
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

