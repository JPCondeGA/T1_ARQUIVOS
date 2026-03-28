all: test.o func.o
	gcc test.o func.o -o exe

verifica: test.o func.o
	gcc test.o func.o -fsanitize=address -o exe

test.o:
	gcc -c test.c -o test.o

func.o:
	gcc -c func.c -o func.o

run:
	./exe

clean:
	rm *.o
	rm exe