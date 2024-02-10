# mandie

Draw a mandelbrot set on Atari Falcon. Hopefully will turn out to be faster than the
GFA BASIC mandelbrot set renderer that I created when I was 17.

Right now, it only runs on 68040 and above (which thankfully Hatari can emulate) in
order to use the internal FPU. This enables it to render a Mandelbrot set
full-screen in VGA highcolour mode in a little over 10 seconds. It might be possible
to shave a bit of time off that but I'm not hopeful of shaving _much_ off it.

## highcol.tos

Runs full-screen in Falcon highcolor mode.

### Keys:

| Key |          |
| --- | -------- |
| z   | zoom in  |
| x   | zoom out |
| ⇧   | up       |
| ⇩   | down     |
| ⇦   | left     |
| ⇨   | right    |

## TODOs

- Try fixed-point maths in order to get it working on a machine without an FPU.
  Apparently 68020 and above has 32 bit multiply instructions that _might_ make it
  bearable to implement this. I'm thinking you'd only need 2 bits before the binary
  point (because if |k| > 2 then k is definitely not in the Mandelbrot set) so you
  could actually get a little bit more precision than a single-precision float. But I
  bet it's a pain to code and I bet it ends up performing _worse_ than the 68040's
  FPU.
- Tidy up the horrifically hacky code.
- Make it run in a GEM window (and thus make it run on FreeMiNT in Aranym).
- Optimise the Mandelbrot::calculate method - implement it in asm? Maaaybe... But
  TBH I think the compiler has already done a better job of optimising it than I could.
