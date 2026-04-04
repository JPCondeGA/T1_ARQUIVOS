all: header.o data.o avl.o fornecidas.o contexto.o create.o select_where.o test.o 
	gcc data.o avl.o header.o fornecidas.o contexto.o test.o select_where.o create.o -o exe

verifica: header.o data.o avl.o fornecidas.o contexto.o create.o select_where.o test.o 
	gcc data.o avl.o header.o fornecidas.o contexto.o test.o select_where.o create.o -fsanitize=address -o exe

test.o:
	gcc -c test.c -o test.o

create.o:
	gcc -c create.c -o create.o

select_where.o:
	gcc -c select_where.c -o select_where.o

contexto.o:
	gcc -c ../CONTEXTO/contexto.c -o contexto.o

fornecidas.o:
	gcc -c ../CONTEXTO/fornecidas.c -o fornecidas.o

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