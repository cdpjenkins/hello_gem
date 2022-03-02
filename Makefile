CFLAGS=-O3

all: hello conway

hello: dots.c hello.c conway.c
	m68k-atari-mint-gcc ${CFLAGS} dots.c hello.c conway.c -o hello.prg  -lgem -lgcc

conway: conway.c conway_main.c
	m68k-atari-mint-gcc  ${CFLAGS} conway.c conway_main.c -o conway.prg  -lgem -lgcc
