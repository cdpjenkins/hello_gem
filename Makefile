all: hello

hello: hello.c
	m68k-atari-mint-gcc hello.c -o hello.prg  -lgem -lgcc

