#ifndef MANDELBROT_RENDEREDMANDELBROT_HPP
#define MANDELBROT_RENDEREDMANDELBROT_HPP

#include <memory>
#include <iostream>

#include "Colour.hpp"

class RenderedMandelbrot {

public:
    RenderedMandelbrot(const int width, const int height);

    void set_pixel(int x, int y, Colour && colour) {
        buffer[y * width + x] = colour;
    }

    Colour * get_buffer() {
        return buffer.get();
    }

    int width;
    int height;

private:
    std::unique_ptr<Colour[]> buffer;
};

#endif //MANDELBROT_RENDEREDMANDELBROT_HPP
