#include "rgb32to8.hpp"

namespace rgb32to8
{
	template
		void rgb32to8(uint32_t src_width, uint32_t src_height, Color32 **src_row_pointers,
			std::vector<uint8_t> &bitmap_out,
			std::vector<Color32> &palette_out,
			std::vector<uint8_t *> *row_pointers_out);
}
