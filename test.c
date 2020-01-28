#include <stdio.h>
#include <stdlib.h>

#define TEST_LINE_NUM 1000
#define MAP_SIZE 1024

#ifndef NULL
#define NULL 0
#endif

typedef struct _pos
{
	int x;
	int y;
} pos;

typedef struct _line
{
	pos s;
	pos e;
} line;

static line test_line[TEST_LINE_NUM];
void create_test_line()
{
	for (int i = 0; i < TEST_LINE_NUM; ++i)
	{
		test_line[i].s.x = random() % 1000;
		test_line[i].s.y = random() % 1000;

		test_line[i].e.x = test_line[i].s.x + random() % 24;
		test_line[i].e.y = test_line[i].s.y + random() % 24;		
	}
}

typedef struct _line_list
{
	struct _line_list *next;
	line *line;
} line_list;

typedef struct _tree
{
	int l;   //left
	int r;   //right
	int t;   //top
	int b;   //bottom
	int lr;  //middle of left and right
	int tb;  //middle of top and bottom
	line_list *lines;
	struct _tree *next[4]; //0-3 left_top, right_top, left_bottom, right_bottom
} tree;

tree *create_tree()
{
	tree *ret = (tree *)malloc(sizeof(tree));
	ret->lines = NULL;
	for (int i = 0; i < 4; ++i) {
		ret->next[i] = NULL;
	}
	return ret;
}

static tree *g_tree;
void create_4cross_tree()
{
	g_tree = create_tree();
	g_tree->l = g_tree->b = 0;
	g_tree->r = g_tree->t = MAP_SIZE;
	g_tree->lr = g_tree->tb = MAP_SIZE / 2;

	for (int i = 0; i < TEST_LINE_NUM; ++i)
	{
		
	}
}

void do_test()
{
}

int main(int argc, char *argv[])
{
	srandom(100);
	create_test_line();
	create_4cross_tree();
	do_test();
	return 0;
}