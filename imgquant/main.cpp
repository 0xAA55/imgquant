
#include <pngcpp/pngcpp.hpp>
#include <rgb24to8/rgb24to8.hpp>

#include <iostream>

namespace rgb24to8
{
    using namespace pngcpp;
    template
        void rgb24to8(uint32_t src_width, uint32_t src_height, const Rgba *const *src_row_pointers,
            std::vector<uint8_t> &bitmap_out,
            std::vector<Rgb> &palette_out,
            std::vector<uint8_t *> *row_pointers_out);

    extern"C" int main(int argc, char **argv)
    {
        if (argc < 3)
        {
            printf("Usage: %s <input.png> <output.png>", argv[0]);
            return 1;
        }

        auto src_png_path = argv[1];
        auto out_png_path = argv[2];

        auto src_png = PngImage(src_png_path);
        std::vector<uint8_t> bitmap_out;
        std::vector<Rgb> palette_out;
        std::vector<uint8_t *> bitmap_row_pointers;
        rgb24to8(src_png.get_width(), src_png.get_height(), reinterpret_cast<const Rgba *const *>(src_png.get_row_pointers()), bitmap_out, palette_out, &bitmap_row_pointers);
        try
        {
            PngImage::save_png8_to(std::string(out_png_path), src_png.get_width(), src_png.get_height(),
                palette_out, &bitmap_row_pointers[0]);
        }
        catch (const SavePngException &e)
        {
            std::cerr << "SavePngException: " << e.what() << std::endl;
        }

        return 0;
    }

}