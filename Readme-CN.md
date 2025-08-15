# imgquant: 针对 PNG 的有损压缩算法，对标 pngquant

## 语言 Language

简体中文|[Chinglish](Readme.md)

## 用法

先编译。

### 编译项目
* Windows: 用 VS2022 编译 `imgquant.sln`，得到 `imgquant.exe`
  * 运行 `imgquant.exe 你的PNG文件.png 输出文件.png`
* Linux:
  * 确保系统里安装了 `libpng-dev` 和 `libz-dev`，或者 `libpng-devel` 和 `libz-devel`
  * 运行 `make -j`，得到可执行文件 `quant`
  * 运行 `./quant 你的PNG文件.png 输出文件.png`

可选参数：`--preserve-alpha`，使用后，将保留 Alpha 通道。

## 压缩算法

1. 针对原图，利用八叉树数据结构算法取 256 色调色板。
2. 对调色板进行排序，取红绿蓝各组分相邻值的最大值，再求各组分相邻值最大值的平均值，用作矩阵抖动的抖动幅度。
3. 应用矩阵抖动，但并不立刻取调色板索引。
4. 在应用了矩阵抖动后，再在矩阵抖动的基础上再应用一次 Floyd-Steinberg 误差扩散抖动，一边进行抖动一边取调色板索引，得到 8-bit 索引颜色。
5. 使用 `libpng` 的 API，提供调色板，写入 8-bit 索引颜色位图。

## 压缩能力

* RGB 图像被压缩到三分之一以内。
* RGBA 图像的透明通道将被丢弃，体积被压缩到四分之一以内。
  * 使用 `--preserve-alpha` 将生成对 Alpha 通道进行抖动计算的图像，体积被压缩到四分之一以内。
* 通常对于颜色越少的位图，压缩效果越明显。
