THE_OS := $(shell uname)

ifeq ($(THE_OS),FreeMiNT)
	PREFIX=
else
	PREFIX=m68k-atari-mint-
endif

CXXFLAGS=-O3 -m68040 -std=c++17
ASFLAGS=--register-prefix-optional

CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
AS=$(PREFIX)as

all: make_dist \
	dist/mandie.tos

make_dist:
	mkdir -p dist

%.o : %.cpp src/*.hpp
	${CXX} -c $(CXXFLAGS) $< -o $@

dist/mandie.tos: src/falcon_highcolour/MandieMain.o src/*.hpp \
					src/mandie/Config.o src/mandie/Mandelbrot.o \
					src/mandie/MandelbrotRenderer.o
	${CXX} ${CXXFLAGS} src/falcon_highcolour/MandieMain.o \
						src/mandie/Config.o src/mandie/Mandelbrot.o \
						src/mandie/MandelbrotRenderer.o  \
						-o dist/mandie.tos -lgcc


clean:
	rm -f src/*.o src/asm/*.o src/mandie/*.o src/falcon_highcolour/*.o
	rm -rf dist
