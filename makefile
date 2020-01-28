all: test

test: test.c
	gcc -g -O0 -o test test.c

clean:
	rm -rf test

