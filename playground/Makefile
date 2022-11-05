CFLAGS=-O3
CC=m68k-atari-mint-gcc-4.6.4

all: cookies.tos

cookies.tos: cookies.c
	${CC} ${CFLAGS} cookies.c -o cookies.tos -lgem -lgcc

clean:
	rm -f *.tos

