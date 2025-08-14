# imgquant: A lossy compression algorithm for PNG, comparable to pngquant

## 语言 Language

[简体中文](Readme-CN.md) | Chinglish

## Usage

Compile first.

### Compiling the Project
* Windows: Compile `imgquant.sln` with VS2022 to get `imgquant.exe`
  * Run `imgquant.exe _your_PNG_file_.png _output_file_.png`
* Linux:
  * Ensure `libpng-dev` and `libz-dev`, or `libpng-devel` and `libz-devel`, are installed on your system.
  * Run `make -j` to get the executable `quant`
  * Run `./quant _your_PNG_file_.png _output_file_.png`

## Compression Algorithm

1. Use the octree data structure algorithm to extract a 256-color palette from the original image.
2. Sort the palette, take the maximum of the adjacent values for each red, green, and blue component, and then average the maximum values for each component to use as the dither amplitude for matrix dithering.
3. Apply matrix dithering, but do not immediately retrieve the palette index.
4. After applying matrix dithering, apply Floyd-Steinberg error diffusion dithering again on top of the matrix dithering, taking the palette index as the dithering proceeds, to obtain 8-bit indexed color.
5. Use the `libpng` API to provide the palette and write an 8-bit indexed color bitmap.

## Compression Capability

* RGB images are compressed to within one-third of their original size.
* RGBA images have their transparency channel discarded, resulting in a compression of less than one-quarter their original size.
* Generally, compression is most pronounced for bitmaps with fewer colors.
