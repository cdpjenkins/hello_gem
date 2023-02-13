CXXFLAGS=-m68040
CFLAGS=-m68040
CC=m68k-atari-mint-gcc
CXX=m68k-atari-mint-g++
AS=m68k-atari-mint-as

PRG_NAME=dist/floats.tos

all: make_dist ${PRG_NAME}

make_dist:
	mkdir -p dist

${PRG_NAME}: src/main.c
	${CC} ${CFLAGS} src/main.c -o ${PRG_NAME} -lgem -lgcc

clean:
	rm -f src/*.o src/asm/*.o
	rm -rf dist
