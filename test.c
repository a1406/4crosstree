#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

#define TEST_LINE_NUM 1000
#define MAP_SIZE 1024
#define TREE_DEPTH 4

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
	LIST_ENTRY(_line) entries;
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

LIST_HEAD(linehead, _line);
typedef struct _tree
{
	int l;   //left
	int r;   //right
	int t;   //top
	int b;   //bottom
	int lr;  //middle of left and right
	int tb;  //middle of top and bottom
	struct linehead linehead;
	int line_count;
	struct _tree *next[4]; //0-3 left_top, right_top, left_bottom, right_bottom
	struct _tree *parent;
	int depth;
} tree;

tree *create_tree()
{
	tree *ret = (tree *)malloc(sizeof(tree));
	LIST_INIT(&ret->linehead);
	ret->line_count = 0;
	for (int i = 0; i < 4; ++i) {
		ret->next[i] = NULL;
	}
	return ret;
}

//0-3 left_top, right_top, left_bottom, right_bottom  -1: cur
int get_tree_pos(line *l, tree *t)
{
	if (l->e.x < t->lr) //left
	{
		if (l->e.y < t->tb) //bottom
		{
			return 2;
		}
		else  //top
		{
			if (l->s.y < t->tb)
				return -1;
			return 0;
		}
	}
	else //right
	{
		if (l->s.x < t->lr)
			return -1;
		if (l->e.y < t->tb) //bottom
		{
			return 3;
		}
		else  //top
		{
			if (l->s.y < t->tb)
				return -1;
			return 1;
		}
	}
	return -1;
}

void add_line_to_tree(line *l, tree *t)
{
	LIST_INSERT_HEAD(&t->linehead, l, entries);
	t->line_count++;
}

static void create_sub_tree(tree *t)
{
	int depth = t->depth + 1;
	t->next[0]         = create_tree();
	t->next[0]->parent = t;
	t->next[0]->depth  = depth;
	t->next[0]->l      = t->l;
	t->next[0]->b      = t->tb;
	t->next[0]->r      = t->lr;
	t->next[0]->t      = t->t;
	t->next[0]->tb     = (t->next[0]->t - t->next[0]->b) / 2;
	t->next[0]->lr     = (t->next[0]->r - t->next[0]->l) / 2;

	t->next[1]         = create_tree();
	t->next[1]->parent = t;
	t->next[1]->depth  = depth;	
	t->next[1]->l      = t->lr;
	t->next[1]->r      = t->r;
	t->next[1]->b      = t->tb;
	t->next[1]->t      = t->t;
	t->next[1]->tb     = (t->next[1]->t - t->next[1]->b) / 2;
	t->next[1]->lr     = (t->next[1]->r - t->next[1]->l) / 2;	

	t->next[2]         = create_tree();
	t->next[2]->parent = t;
	t->next[2]->depth  = depth;	
	t->next[2]->l      = t->l;
	t->next[2]->r      = t->lr;
	t->next[2]->b      = t->b;
	t->next[2]->t      = t->tb;
	t->next[2]->tb     = (t->next[2]->t - t->next[2]->b) / 2;
	t->next[2]->lr     = (t->next[2]->r - t->next[2]->l) / 2;	

	t->next[3]         = create_tree();
	t->next[3]->parent = t;
	t->next[3]->depth  = depth;	
	t->next[3]->l      = t->lr;
	t->next[3]->r      = t->r;
	t->next[3]->b      = t->b;
	t->next[3]->t      = t->tb;
	t->next[3]->tb     = (t->next[3]->t - t->next[3]->b) / 2;
	t->next[3]->lr     = (t->next[3]->r - t->next[3]->l) / 2;

	if (depth < TREE_DEPTH)
	{
		create_sub_tree(t->next[0]);
		create_sub_tree(t->next[1]);
		create_sub_tree(t->next[2]);
		create_sub_tree(t->next[3]);
	}
}

static tree *g_tree;
void add_line(line *l)
{
	tree *t = g_tree;
	for (;;)
	{
		int pos = get_tree_pos(l, t);
		if (pos < 0 || !t->next[pos])
		{
			add_line_to_tree(l, t);
			return;
		}
		t = t->next[pos];
	}
}
void create_4cross_tree()
{
	g_tree = create_tree();
	g_tree->parent = NULL;
	g_tree->depth = 0;
	g_tree->l = g_tree->b = 0;
	g_tree->r = g_tree->t = MAP_SIZE;
	g_tree->lr = g_tree->tb = MAP_SIZE / 2;

	create_sub_tree(g_tree);

	for (int i = 0; i < TEST_LINE_NUM; ++i)
	{
		add_line(&test_line[i]);
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
