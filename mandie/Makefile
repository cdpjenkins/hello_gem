THE_OS := $(shell uname)

ifeq ($(THE_OS),FreeMiNT)
	PREFIX=
else
	PREFIX=m68k-atari-mint-
endif

CXXFLAGS=-O3 -m68000 -std=c++17
ASFLAGS=--register-prefix-optional

CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
AS=$(PREFIX)as

all: make_dist \
	dist/conway.prg \
	dist/highcol.tos \
	resources \
	dist/falctest.tos

make_dist:
	mkdir -p dist
	cp grids/*.cwy dist/

resources: src/conway.*
	cp src/conway.rsc dist

dist/conway.prg: src/ConwayGui.cpp src/*.hpp
	${CXX} ${CXXFLAGS} src/ConwayGui.cpp -o dist/conway.prg  -lgem -lgcc

dist/test.tos: src/ConwayTest.cpp
	${CXX} ${CXXFLAGS} src/ConwayTest.cpp -o dist/test.tos  -lgem -lgcc

%.o : %.cpp src/*.hpp
	${CXX} -c $(CXXFLAGS) $< -o $@

src/fastFullScreenMain.o: src/FullScreenMain.cpp src/*.hpp
	${CXX} -c $(CXXFLAGS) -DFAST_DRAW src/FullScreenMain.cpp -o src/fastFullScreenMain.o

src/asm/%.o: src/asm/%.s
	$(AS) $(ASFLAGS) $< -o $@

dist/highcol.tos: src/falcon_highcolour/FalconHighColourMain.o src/STScreen.o src/asm/fast_draw.o src/*.hpp \
					src/mandie/Complex.o src/mandie/Config.o src/mandie/Mandelbrot.o \
					src/mandie/MandelbrotRenderer.o  src/mandie/RenderedMandelbrot.o \
					
	${CXX} ${CXXFLAGS} src/falcon_highcolour/FalconHighColourMain.o src/STScreen.o src/asm/fast_draw.o \
						-o dist/highcol.tos -lgcc

dist/falctest.tos: src/falcon_highcolour/test.o
	${CXX} ${CXXFLAGS} src/falcon_highcolour/test.o \
						-o dist/falctest.tos -lgcc


clean:
	rm -f src/*.o src/asm/*.o
	rm -rf dist
