all: main.o header.o data.o avl.o func.o
	gcc main.o header.o data.o avl.o func.o -o exe 

verifica: main.o header.o data.o avl.o func.o
	gcc main.o header.o data.o avl.o func.o -fsanitize=address -o exe 

main.o: 
	gcc -c main.c -o main.o

header.o:
	gcc -c TAD_HEADER/header.c -o header.o

data.o:
	gcc -c TAD_DATA/data.c -o data.o

avl.o:
	gcc -c TAD_AVL/avl.c -o avl.o

func.o:
	gcc -c FUNC/func.c -o func.o

run:
	./exe

clean:
	rm *.o
	rm exe