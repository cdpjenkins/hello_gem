CFLAGS=-O3
CC=m68k-atari-mint-gcc

all: conway_gui conway_test

conway_gui: dots.c conway_gui.c conway_grid.c
	${CC} ${CFLAGS} dots.c conway_gui.c conway_grid.c -o conway.prg  -lgem -lgcc

conway_test: conway_grid.c conway_test.c
	${CC} ${CFLAGS} conway_grid.c conway_test.c -o test.prg  -lgem -lgcc

clean:
	rm -f *.prg
