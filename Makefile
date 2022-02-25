all: hello conway

hello: dots.c hello.c
	m68k-atari-mint-gcc dots.c hello.c -o hello.prg  -lgem -lgcc

conway: conway.c conway_main.c
	m68k-atari-mint-gcc conway.c conway_main.c -o conway.prg  -lgem -lgcc
