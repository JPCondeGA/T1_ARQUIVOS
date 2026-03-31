all: test.o data.o
	gcc test.o data.o -o exe

verifica: test.o data.o
	gcc test.o data.o -fsanitize=address -o exe

test.o:
	gcc -c test.c -o test.o

data.o:
	gcc -c data.c -o data.o

run:
	./exe

reset:
	rm arq.bin
	touch arq.bin

clean:
	rm *.o
	rm exe