CFLAGS=-O3
CC=m68k-atari-mint-gcc
CXX=m68k-atari-mint-g++

all: conway.prg test.prg fullconw.tos

conway.prg: Dots.cpp ConwayGui.cpp ConwayGrid.cpp
	${CXX} ${CFLAGS} Dots.cpp ConwayGui.cpp ConwayGrid.cpp -o conway.prg  -lgem -lgcc

test.prg: ConwayGrid.cpp ConwayTest.cpp
	${CXX} ${CFLAGS} ConwayGrid.cpp ConwayTest.cpp -o test.prg  -lgem -lgcc

fullconw.tos: ConwayGrid.cpp FullScreenMain.cpp
	${CXX} ${CFLAGS} ConwayGrid.cpp FullScreenMain.cpp -o fullconw.tos -lgcc

clean:
	rm -f *.prg
