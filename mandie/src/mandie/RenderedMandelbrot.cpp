#include "RenderedMandelbrot.hpp"

RenderedMandelbrot::RenderedMandelbrot(const int width, const int height) :
        width(width),
        height(height),
        buffer(std::make_unique<Colour[]>(width*height))
{}
