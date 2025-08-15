
#include <pngcpp/pngcpp.hpp>
#include <rgbto8/rgb24to8.hpp>
#include <rgbto8/rgb32to8.hpp>

#include <iostream>
#include <cstring>

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

#ifndef _DEBUG
        try
#endif // !_DEBUG
        {
            Bitmap<ColorRgba> src_png = std::move(PngImage(src_png_path).get_bmp());
            std::vector<ColorRgb> palette_out;
            auto ibmp = rgb24to8(src_png, palette_out);

            if (!preserve_alpha)
            {
                PngImage::save_png8_to(std::string(out_png_path), src_png.get_width(), src_png.get_height(),
                    reinterpret_cast<ColorRgb *>(&palette_out[0]), palette_out.size(), ibmp.get_row_pointers());
            }
            else
            {
                auto rgba = Bitmap<ColorRgba>(src_png.get_width(), src_png.get_height());
#pragma omp parallel for
                for (std::ptrdiff_t y = 0; y < static_cast<std::ptrdiff_t>(src_png.get_height()); y++)
                {
                    auto src_row = src_png.get_row(y);
                    auto imm_row = ibmp.get_row(y);
                    auto dst_row = rgba.get_row(y);
                    for (size_t x = 0; x < src_png.get_width(); x++)
                    {
                        auto &src_pix = src_row[x];
                        auto &imm_pix = imm_row[x];
                        auto &dst_pix = dst_row[x];
                        auto &color = palette_out[imm_pix];
                        dst_pix.A = src_pix.A;
                        dst_pix.R = color.R;
                        dst_pix.G = color.G;
                        dst_pix.B = color.B;
                    }
                }
                PngImage::save_png32_to(std::string(out_png_path), src_png.get_width(), src_png.get_height(), rgba.get_row_pointers());
            }
        }
#ifndef _DEBUG
        catch (const LoadPngException &e)
        {
            std::cerr << "LoadPngException: " << e.what() << std::endl;
        }
        catch (const SavePngException &e)
        {
            std::cerr << "SavePngException: " << e.what() << std::endl;
        }
#endif // !_DEBUG

        return 0;
    }

}