CXXFLAGS=-O3 -m68000
CC=m68k-atari-mint-gcc
CXX=m68k-atari-mint-g++
AS=m68k-atari-mint-as

all: make_dist dist/conway.prg dist/test.tos dist/fullconw.tos dist/fastconw.tos

make_dist:
	mkdir -p dist
	cp grids/*.cwy dist/
	cp conway.rsc dist/

dist/conway.prg: Dots.cpp ConwayGui.cpp ConwayGrid.cpp
	${CXX} ${CXXFLAGS} Dots.cpp ConwayGui.cpp ConwayGrid.cpp -o dist/conway.prg  -lgem -lgcc

dist/test.tos: ConwayGrid.cpp ConwayTest.cpp
	${CXX} ${CXXFLAGS} ConwayGrid.cpp ConwayTest.cpp -o dist/test.tos  -lgem -lgcc

%.o : %.cpp *.hpp
	${CXX} -c $(CXXFLAGS) $< -o $@

fastFullScreenMain.o: FullScreenMain.cpp *.hpp
	${CXX} -c $(CXXFLAGS) -DFAST_DRAW FullScreenMain.cpp -o fastFullScreenMain.o

asm/%.o: asm/%.s
	$(AS) $< -o $@

dist/fullconw.tos: ConwayGrid.o FullScreenMain.o STScreen.o asm/fast_draw.o
	${CXX} ${CXXFLAGS} ConwayGrid.o STScreen.o FullScreenMain.o asm/fast_draw.o -o dist/fullconw.tos -lgcc

dist/fastconw.tos: ConwayGrid.o FullScreenMain.o STScreen.o asm/fast_draw.o fastFullScreenMain.o
	${CXX} ${CXXFLAGS} ConwayGrid.o STScreen.o fastFullScreenMain.o asm/fast_draw.o -o dist/fastconw.tos -lgcc

clean:
	rm *.o asm/*.o
	rm -rf dist
