all: main.o header.o data.o avl.o create.o select.o select_where.o delete.o insert.o update.o contexto.o fornecidas.o
	gcc main.o header.o data.o avl.o create.o select.o select_where.o delete.o insert.o update.o contexto.o fornecidas.o -o exe 

verifica: main.o header.o data.o avl.o create.o select.o select_where.o delete.o insert.o update.o contexto.o fornecidas.o
	gcc main.o header.o data.o avl.o create.o select.o select_where.o delete.o insert.o update.o contexto.o fornecidas.o -fsanitize=address -o exe 

main.o: 
	gcc -c main.c -o main.o

header.o:
	gcc -c TAD_HEADER/header.c -o header.o

data.o:
	gcc -c TAD_DATA/data.c -o data.o

avl.o:
	gcc -c TAD_AVL/avl.c -o avl.o

create.o:
	gcc -c FUNC/create.c -o create.o

select.o:
	gcc -c FUNC/select.c -o select.o

select_where.o:
	gcc -c FUNC/select_where.c -o select_where.o

delete.o:
	gcc -c FUNC/delete.c -o delete.o

insert.o:
	gcc -c FUNC/insert.c -o insert.o

update.o:
	gcc -c FUNC/update.c -o update.o

contexto.o:
	gcc -c CONTEXTO/contexto.c -o contexto.o

fornecidas.o:
	gcc -c CONTEXTO/fornecidas.c -o fornecidas.o

reset:
	rm estacoes.bin
	touch estacoes.bin

run:
	./exe

clean:
	rm *.o
	rm exe