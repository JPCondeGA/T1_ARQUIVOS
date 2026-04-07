all: header.o data.o avl.o fornecidas.o contexto.o create.o select.o select_where.o delete.o insert.o update.o test.o 
	gcc data.o avl.o header.o fornecidas.o contexto.o test.o update.o insert.o delete.o select_where.o select.o create.o -o exe

verifica: header.o data.o avl.o fornecidas.o contexto.o create.o select.o select_where.o delete.o insert.o update.o test.o 
	gcc data.o avl.o header.o fornecidas.o contexto.o test.o update.o insert.o delete.o select_where.o select.o create.o -fsanitize=address -o exe

test.o:
	gcc -c test.c -o test.o

create.o:
	gcc -c create.c -o create.o
	
select.o:
	gcc -c select.c -o select.o

select_where.o:
	gcc -c select_where.c -o select_where.o
	
delete.o:
	gcc -c delete.c -o delete.o
	
insert.o:
	gcc -c insert.c -o insert.o
	
update.o:
	gcc -c update.c -o update.o

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
