CFLAGS=-O3
CC=m68k-atari-mint-gcc
CXX=m68k-atari-mint-g++

all: conway_gui conway_test

conway_gui: Dots.cpp ConwayGui.cpp ConwayGrid.cpp
	${CXX} ${CFLAGS} Dots.cpp ConwayGui.cpp ConwayGrid.cpp -o conway.prg  -lgem -lgcc

conway_test: ConwayGrid.cpp ConwayTest.cpp
	${CXX} ${CFLAGS} ConwayGrid.cpp ConwayTest.cpp -o test.prg  -lgem -lgcc

clean:
	rm -f *.prg
