CFLAGS=-O3

all: hello conway_test

hello: dots.c hello.c conway_grid.c
	m68k-atari-mint-gcc ${CFLAGS} dots.c hello.c conway_grid.c -o hello.prg  -lgem -lgcc

conway_test: conway_grid.c conway_test.c
	m68k-atari-mint-gcc  ${CFLAGS} conway_grid.c conway_test.c -o test.prg  -lgem -lgcc
