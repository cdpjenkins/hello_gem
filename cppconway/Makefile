CXXFLAGS=-O3 -m68000
CC=m68k-atari-mint-gcc
CXX=m68k-atari-mint-g++
AS=m68k-atari-mint-as

all: make_dist dist/conway.prg dist/test.tos dist/fullconw.tos dist/fastconw.tos resources

make_dist:
	mkdir -p dist
	cp grids/*.cwy dist/

resources: src/conway.*
	cp src/conway.rsc dist

dist/conway.prg: src/ConwayGui.cpp src/ConwayGrid.cpp
	${CXX} ${CXXFLAGS} src/ConwayGui.cpp src/ConwayGrid.cpp -o dist/conway.prg  -lgem -lgcc

dist/test.tos: src/ConwayGrid.cpp src/ConwayTest.cpp
	${CXX} ${CXXFLAGS} src/ConwayGrid.cpp src/ConwayTest.cpp -o dist/test.tos  -lgem -lgcc

%.o : %.cpp *.hpp
	${CXX} -c $(CXXFLAGS) $< -o $@

src/fastFullScreenMain.o: src/FullScreenMain.cpp src/*.hpp
	${CXX} -c $(CXXFLAGS) -DFAST_DRAW src/FullScreenMain.cpp -o src/fastFullScreenMain.o

src/asm/%.o: src/asm/%.s
	$(AS) $< -o $@

dist/fullconw.tos: src/ConwayGrid.o src/FullScreenMain.o src/STScreen.o src/asm/fast_draw.o
	${CXX} ${CXXFLAGS} src/ConwayGrid.o src/STScreen.o src/FullScreenMain.o src/asm/fast_draw.o -o dist/fullconw.tos -lgcc

dist/fastconw.tos: src/ConwayGrid.o src/FullScreenMain.o src/STScreen.o src/asm/fast_draw.o src/fastFullScreenMain.o
	${CXX} ${CXXFLAGS} src/ConwayGrid.o src/STScreen.o src/fastFullScreenMain.o src/asm/fast_draw.o -o dist/fastconw.tos -lgcc

clean:
	rm -f src/*.o src/asm/*.o
	rm -rf dist
