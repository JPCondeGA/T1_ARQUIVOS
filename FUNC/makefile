all: header.o data.o avl.o create.o test.o 
	gcc data.o avl.o header.o test.o create.o -o exe

verifica: test.o func.o
	gcc test.o func.o -fsanitize=address -o exe

test.o:
	gcc -c test.c -o test.o

create.o:
	gcc -c create.c -o create.o

avl.o:
	gcc -c ../TAD_AVL/avl.c -o avl.o

header.o:
	gcc -c ../TAD_HEADER/header.c -o header.o

data.o:
	gcc -c ../TAD_DATA/data.c -o data.o

run:
	touch arq.bin
	./exe

reset:
	rm arq.bin
	touch arq.bin

clean:
	rm *.o
	rm exe