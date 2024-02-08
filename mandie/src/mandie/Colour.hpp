#ifndef MANDELBROT_COLOUR_HPP
#define MANDELBROT_COLOUR_HPP

#include <cstdint>
using namespace std;

struct Colour {
    unsigned int r : 5;
    unsigned int g : 5;
    unsigned int b : 5;

    inline Colour() : Colour(0, 0, 0) {}

    inline Colour(int r, int g, int b) :
            r (r),
            g (g),
            b (b) {}
};

#endif // MANDELBROT_COLOUR_HPP
