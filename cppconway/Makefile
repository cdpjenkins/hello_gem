CXXFLAGS=-O3 -m68000
CC=m68k-atari-mint-gcc
CXX=m68k-atari-mint-g++
AS=m68k-atari-mint-as

all: conway.prg test.tos fullconw.tos

conway.prg: Dots.cpp ConwayGui.cpp ConwayGrid.cpp
	${CXX} ${CXXFLAGS} Dots.cpp ConwayGui.cpp ConwayGrid.cpp -o conway.prg  -lgem -lgcc

test.tos: ConwayGrid.cpp ConwayTest.cpp
	${CXX} ${CXXFLAGS} ConwayGrid.cpp ConwayTest.cpp -o test.tos  -lgem -lgcc

%.o : %.cpp *.hpp
	${CXX} -c $(CXXFLAGS) $< -o $@

asm/%.o: asm/%.s
	$(AS) $< -o $@

fullconw.tos: ConwayGrid.o FullScreenMain.o STScreen.o asm/fast_draw.o
	${CXX} ${CXXFLAGS} ConwayGrid.o STScreen.o FullScreenMain.o asm/fast_draw.o -o fullconw.tos -lgcc

clean:
	rm -f *.prg *.tos
