#include <stdio.h>
#include <stdlib.h>

#define TEST_LINE_NUM 1000

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

int main(int argc, char *argv[])
{
	srandom(100);
	return 0;
}
