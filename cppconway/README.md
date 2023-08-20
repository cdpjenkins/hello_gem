# cppconway
An implementation of Conway's Game Of Life for Atari machines. I've only ever run this in emulators
because my ST has been in a box since 1999 but it runs alright on an emulated ST and Falcom, as
well as in Aranym.

Written in C++ and built using a cross-mint GCC compiler. Linked against mintlib, which results
in the executables being full of several hundred K of library functions that we don't use. This
might have been a problem if we needed to fit them on a floppy disk but doesn't matter so much
nowadays.

The algorithm is heavily inspired by the one described in
[Michael Abrash's Graphics Programming Black Book](https://github.com/jagregory/abrash-black-book).

At the time of writing, there are a few different executables:

## conway.prg
Runs in a GEM GUI. Runs on both Aranym and an emulated real Atari machine in pretty much any resolution.
Fairly slow, though, because it draws the grid to screen using VDI calls.

## fullconw.tos
Runs full-screen in ST high resolution. Much faster than the GEM version. Draws the grid by hitting
screen memory directly.

## fastconw.tos
As fullconw.tos above but uses a little bit of 68000 assembly code to speed up the process of
drawing to the screen. Probably not really worth it given that much a greater speed-up was
achieved by optimising the step method.
