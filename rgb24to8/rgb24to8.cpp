#include "rgb24to8.hpp"
#include "octree.hpp"

namespace rgb24to8
{
	template
		void rgb24to8(uint32_t src_width, uint32_t src_height, const Palette24 *const *src_row_pointers,
			std::vector<uint8_t> &bitmap_out,
			std::vector<Palette24> &palette_out,
			std::vector<uint8_t *> *row_pointers_out);

	template
		void rgb24to8(uint32_t src_width, uint32_t src_height, const Palette32 *const *src_row_pointers,
			std::vector<uint8_t> &bitmap_out,
			std::vector<Palette24> &palette_out,
			std::vector<uint8_t *> *row_pointers_out);

	template
		void rgb24to8(uint32_t src_width, uint32_t src_height, const Palette24 *const *src_row_pointers,
			std::vector<uint8_t> &bitmap_out,
			std::vector<Palette32> &palette_out,
			std::vector<uint8_t *> *row_pointers_out);

	template
		void rgb24to8(uint32_t src_width, uint32_t src_height, const Palette32 *const *src_row_pointers,
			std::vector<uint8_t> &bitmap_out,
			std::vector<Palette32> &palette_out,
			std::vector<uint8_t *> *row_pointers_out);
}
