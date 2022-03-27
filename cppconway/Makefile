CFLAGS=-O3
CC=m68k-atari-mint-gcc
CXX=m68k-atari-mint-g++

all: conway_gui conway_test

conway_gui: dots.c conway_gui.c conway_grid.c
	${CXX} ${CFLAGS} dots.c conway_gui.c conway_grid.c -o conway.prg  -lgem -lgcc

conway_test: conway_grid.c conway_test.c
	${CXX} ${CFLAGS} conway_grid.c conway_test.c -o test.prg  -lgem -lgcc

clean:
	rm -f *.prg
