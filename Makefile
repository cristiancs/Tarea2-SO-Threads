CC = gcc
CFLAGS = -pthread -Wall
clean:
	rm -f *.o
%.o: %.c
	gcc -g -Wall -D_POSIX_PTHREAD_SEMANTICS $^ -c -o $@
ej1: sthread.o dictionary.o ejercicio1.o
	$(CC) $(CFLAGS) $^ -o ejercicio1