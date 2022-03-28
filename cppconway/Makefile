CFLAGS=-O3
CC=m68k-atari-mint-gcc
CXX=m68k-atari-mint-g++

all: conway_gui conway_test

conway_gui: dots.cpp conway_gui.cpp conway_grid.cpp
	${CXX} ${CFLAGS} dots.cpp conway_gui.cpp conway_grid.cpp -o conway.prg  -lgem -lgcc

conway_test: conway_grid.cpp conway_test.cpp
	${CXX} ${CFLAGS} conway_grid.cpp conway_test.cpp -o test.prg  -lgem -lgcc

clean:
	rm -f *.prg
