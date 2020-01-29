#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

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
	int index;
	LIST_ENTRY(_line);
} line;

static line test_line[TEST_LINE_NUM];
static int  sortx_line[TEST_LINE_NUM];
static int  sorty_line[TEST_LINE_NUM];

int compar_x(const void *a, const void *b)
{
	line *la = &test_line[*(int *)a];
	line *lb = &test_line[*(int *)b];
	if (la->s.x > lb->s.x)
		return 1;
	if (la->s.x < lb->s.x)
		return -1;	
	return (0);
}
int compar_y(const void *a, const void *b)
{
	line *la = &test_line[*(int *)a];
	line *lb = &test_line[*(int *)b];
	if (la->s.y > lb->s.y)
		return 1;
	if (la->s.y < lb->s.y)
		return -1;	
	return (0);
}

void create_test_line()
{
	for (int i = 0; i < TEST_LINE_NUM; ++i)
	{
		sortx_line[i] = i;
		sorty_line[i] = i;		
		
		test_line[i].index = i;
		test_line[i].s.x = random() % 1000;
		test_line[i].s.y = random() % 1000;

		test_line[i].e.x = test_line[i].s.x + random() % 24;
		test_line[i].e.y = test_line[i].s.y + random() % 24;		
	}
	qsort(&sortx_line[0], TEST_LINE_NUM, sizeof(int), compar_x);
	qsort(&sorty_line[0], TEST_LINE_NUM, sizeof(int), compar_y);	
}

typedef struct _line_list
{
	struct _line_list *next;
	line *line;
} line_list;
LIST_HEAD(linehead, line);

typedef struct _tree
{
	int l;   //left
	int r;   //right
	int t;   //top
	int b;   //bottom
	int lr;  //middle of left and right
	int tb;  //middle of top and bottom
	struct linehead linehead;
	struct _tree *next[4]; //0-3 left_top, right_top, left_bottom, right_bottom
} tree;

tree *create_tree()
{
	tree *ret = (tree *)malloc(sizeof(tree));
	LIST_INIT(&ret->linehead);
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
