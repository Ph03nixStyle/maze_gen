CC= gcc
CFLAGS= -Wall -lm

HEADERS = headers/alg.h headers/main.h

default: maze

all: maze

maze: main.c alg.c ${HEADERS}
	${CC} main.c -o maze ${CFLAGS}

alg: alg.c alg.h
	${CC} alg.c -o alg -Wall

debug: main.c alg.c $(HEADERS)
	$(CC) -g main.c -o debug $(CFLAGS)
	-gdb debug

clean:
	-rm -f alg
	-rm -f main
	-rm -f maze
	-rm -f debug
	-rm -f a.out