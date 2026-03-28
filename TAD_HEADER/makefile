all: test.o header.o
	gcc test.o header.o -o exe

verifica: test.o header.o
	gcc test.o header.o -fsanitize=address -o exe

test.o:
	gcc -c test.c -o test.o

header.o:
	gcc -c header.c -o header.o

run:
	./exe

clean:
	rm *.o
	rm exe
	rm *.bin