
#include <pngcpp/pngcpp.hpp>
#include <rgb24to8/rgb24to8.hpp>

#include <iostream>

namespace rgb24to8
{
    using namespace pngcpp;

    void usage(char *argv0)
    {
        std::cout << "Usage:" << argv0 << " [--preserve-alpha] <input.png> <output.png>" << std::endl;
    }

    extern"C" int main(int argc, char **argv)
    {
        if (argc < 3)
        {
            usage(argv[0]);
            return 1;
        }

        bool preserve_alpha = false;
        char *src_png_path = nullptr;
        char *out_png_path = nullptr;
        if (argc == 3)
        {
            src_png_path = argv[1];
            out_png_path = argv[2];
        }
        else
        {
            for (int i = 1; i < argc; i++)
            {
                auto arg = argv[i];
                if (!memcmp("--", arg, 2))
                {
                    if (!strcmp(arg, "--preserve-alpha"))
                    {
                        preserve_alpha = true;
                    }
                    else
                    {
                        std::cerr << "Unknown option `" << arg << "`." << std::endl;
                        usage(argv[0]);
                        return 1;
                    }
                }
                else
                {
                    if (!src_png_path)
                    {
                        src_png_path = arg;
                        continue;
                    }
                    if (!out_png_path)
                    {
                        out_png_path = arg;
                        continue;
                    }
                }
            }
        }

        if (!src_png_path || !out_png_path)
        {
            usage(argv[0]);
            return 1;
        }
        auto src_png = PngImage(src_png_path);
        std::vector<uint8_t> bitmap_out;
        std::vector<Color24> palette_out;
        std::vector<uint8_t *> bitmap_row_pointers;
        rgb24to8(src_png.get_width(), src_png.get_height(), reinterpret_cast<Color32 **>(src_png.get_row_pointers()), bitmap_out, palette_out, &bitmap_row_pointers);

#ifndef _DEBUG
        try
#endif // !_DEBUG
        {
            PngImage::save_png8_to(std::string(out_png_path), src_png.get_width(), src_png.get_height(),
                reinterpret_cast<Rgb *>(&palette_out[0]), palette_out.size(), &bitmap_row_pointers[0]);
        }
#ifndef _DEBUG
        catch (const SavePngException &e)
        {
            std::cerr << "SavePngException: " << e.what() << std::endl;
        }
#endif // !_DEBUG

        return 0;
    }

}