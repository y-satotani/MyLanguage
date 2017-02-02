CC=gcc
CFLAGS=-Og -Wall
LIBSRC=value.c variable.c ast.c reg.c vtable.c
CLIB=-lfl -ly

.PHONY: clean

%.yy.c: %.l
	flex -o $@ $<

%.tab.c: %.y
	bison -Wall -d $<

l3.out: l3.c l3.yy.c l3.tab.c $(LIBSRC)
	$(CC) $(CFLAGS) -o $@ $^ $(CLIB)

clean:
	rm -rf *.o *.out *.tab.h *.tab.c *.yy.c
