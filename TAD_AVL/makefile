all: test.o avl.o
	gcc test.o avl.o -o exe

verifica: test.o avl.o
	gcc test.o avl.o -fsanitize=address -o exe

test.o:
	gcc -c test.c -o test.o

avl.o:
	gcc -c avl.c -o avl.o

run:
	./exe

clean:
	rm *.o
	rm exe