ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: main.o functions.o
	$(CC) -o shell main.o functions.o

main.o: main.c functions.h
	$(CC) -c main.c

functions.o: functions.c functions.h
	$(CC) -c functions.c

run:
	./shell

clean:
	rm *.o
	rm shell
	rm *~

memcheck:
	valgrind --leak-check=yes ./shell
