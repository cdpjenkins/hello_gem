CFLAGS=-O3
CC=m68k-atari-mint-gcc-4.6.4

all: testing

testing: testing.c
	${CC} ${CFLAGS} testing.c -o testing.prg  -lgem -lgcc

clean:
	rm -f *.prg
