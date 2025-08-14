#include "octree.hpp"

namespace octree
{
	NoReducibleNodes::NoReducibleNodes(const char *what) noexcept :
		std::exception(what)
	{
	}
	OctreeNode::~OctreeNode()
	{
		for (int i = 0; i < 8; i++)
		{
			if (child[i]) delete child[i];
		}
	}
	size_t PaletteGenerator::reduce_color()
	{
		int i;

		//�ҵ�����һ���Ŀ�ɾ���ڵ㡣
		for (i = 7; i; i--)
		{
			if (reducible_nodes[i])
				break;
		}
		if (!reducible_nodes[i])
		{
			throw NoReducibleNodes("Could not found any reducible nodes.");
		}

		//����ָ��ͬ����һ����ɾ���ڵ�
		OctreeNode *node = reducible_nodes[i];
		reducible_nodes[i] = node->next_sibling;

		//ɾ���ڵ�
		for (i = 0; i < 8; i++)
		{
			if (node->child[i])
			{
				node->r_sum += node->child[i]->r_sum;
				node->g_sum += node->child[i]->g_sum;
				node->b_sum += node->child[i]->b_sum;
				node->num_colors += node->child[i]->num_colors;
				delete node->child[i];
				node->child[i] = nullptr;
				num_leaves--;
			}
		}
		node->is_leaf = 1;
		num_leaves++;

		return num_leaves;
	}
	OctreeNode *PaletteGenerator::create_node(int level)
	{
		auto *node = new OctreeNode;

		if (level == 8)
		{
			//�ж��Ƿ�����һ��
			node->is_leaf = true;
			num_leaves++;
		}
		else
		{
			//�����������еĽڵ�
			node->next_sibling = reducible_nodes[level];
			reducible_nodes[level] = node;
		}
		return node;
	}
	void PaletteGenerator::get_palette_recursive(OctreeNode *node, on_get_color fn_on_get_color, void *userdata) const
	{
		if (node->is_leaf)
			fn_on_get_color(userdata,
				(uint8_t)(node->r_sum / node->num_colors),
				(uint8_t)(node->g_sum / node->num_colors),
				(uint8_t)(node->b_sum / node->num_colors));
		else
		{
			for (int i = 0; i < 8; i++)
			{
				if (node->child[i]) get_palette_recursive(node->child[i], fn_on_get_color, userdata);
			}
		}
	}
	PaletteGenerator::PaletteGenerator() : root(create_node(0))
	{
	}
	PaletteGenerator::~PaletteGenerator()
	{
		delete root;
	}
	size_t PaletteGenerator::add_color(uint8_t r, uint8_t g, uint8_t b)
	{
		int i, index, shift;

		OctreeNode *node = root;

		//������չ��֦
		for (i = 1; i <= 8; i++)
		{
			if (node->is_leaf)//����Ѿ�����Ҷ������չ��֦��
				break;
			shift = 8 - i;
			index =
				((r >> shift) & 1) << 0 |
				((r >> shift) & 1) << 1 |
				((r >> shift) & 1) << 2;
			if (!(node->child[index]))
			{
				node->child[index] = create_node(i);
			}
			node = node->child[index];
		}

		//��Ҷͳ����ɫ
		node->r_sum += r;
		node->g_sum += g;
		node->b_sum += b;
		node->num_colors++;
		num_colors++;
		return num_colors;
	}
	void PaletteGenerator::get_palette(on_get_color fn_on_get_color, void *userdata) const
	{
		get_palette_recursive(root, fn_on_get_color, userdata);
	}
};



