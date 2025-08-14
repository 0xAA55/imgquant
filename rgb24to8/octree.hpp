#pragma once

#include <cinttypes>
#include <stdexcept>

namespace octree
{
	class NoReducibleNodes : public std::exception
	{
	public:
		NoReducibleNodes(const char *what) noexcept;
	};

	struct OctreeNode
	{
		uint32_t	num_colors = 0;
		uint32_t	r_sum = 0;
		uint32_t	g_sum = 0;
		uint32_t	b_sum = 0;
		bool		is_leaf = false;
		OctreeNode	*child[8] = { nullptr }; //八叉树子节点(i = 红绿蓝三个颜色值各贡献一位)
		OctreeNode	*next_sibling = nullptr; //下一个可删减节点

		OctreeNode() = default;
		~OctreeNode();
	};

	typedef void(*on_get_color)(void *userdata, uint8_t r, uint8_t g, uint8_t b);

	class PaletteGenerator
	{
	protected:
		OctreeNode	*root = nullptr;
		OctreeNode	*reducible_nodes[8] = { nullptr};
		size_t		num_leaves = 0;
		uint32_t	num_colors = 0;

		OctreeNode *create_node(int level);
		void get_palette_recursive(OctreeNode *node, on_get_color fn_on_get_color, void *userdata) const;

	public:
		PaletteGenerator();
		~PaletteGenerator();

		size_t add_color(uint8_t r, uint8_t g, uint8_t b);
		size_t get_num_colors() const;
		size_t reduce_color();
		void get_palette(on_get_color fn_on_get_color, void* userdata) const;
	};
};

