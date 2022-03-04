CFLAGS=-O3

all: conway_gui conway_test

conway_gui: dots.c conway_gui.c conway_grid.c
	m68k-atari-mint-gcc ${CFLAGS} dots.c conway_gui.c conway_grid.c -o conway.prg  -lgem -lgcc

conway_test: conway_grid.c conway_test.c
	m68k-atari-mint-gcc  ${CFLAGS} conway_grid.c conway_test.c -o test.prg  -lgem -lgcc

clean:
	rm -f *.prg
