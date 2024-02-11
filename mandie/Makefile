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

MANDIE_OBJS=src/Main.o src/Config.o src/MandelbrotRenderer.o
MANDIE_EXECUTABLE=dist/mandie.tos

all: make_dist \
	$(MANDIE_EXECUTABLE)

make_dist:
	mkdir -p dist

%.o : %.cpp src/*.hpp Makefile
	${CXX} -c $(CXXFLAGS) $< -o $@

$(MANDIE_EXECUTABLE): $(MANDIE_OBJS) \
					src/*.hpp \
					Makefile
	${CXX} ${CXXFLAGS} $(MANDIE_OBJS) \
						-o dist/mandie.tos -lgcc


clean:
	rm -f src/*.o
	rm -rf dist
