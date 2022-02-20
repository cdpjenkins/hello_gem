all: hello

hello: *.c
	m68k-atari-mint-gcc *.c -o hello.prg  -lgem -lgcc

