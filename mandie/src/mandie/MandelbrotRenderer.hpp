#ifndef MANDELBROT_MANDELBROTRENDERER_HPP
#define MANDELBROT_MANDELBROTRENDERER_HPP

#include <iostream>
using namespace std;

#include "Colour.hpp"
#include "Config.hpp"
#include "Mandelbrot.hpp"

class MandelbrotRenderer {
public:
    MandelbrotRenderer(
        int width,
        int height,
        const Config& config,
        Colour* buffer,
        const Complex& centre = Complex(0, 0),
        double zoom_size = 4.0
    );

    void render_to_buffer(const Mandelbrot& render_future);

    void zoom_in();
    void zoom_in_to(int x, int y);
    void zoom_in_to(const Complex& coords);

    void zoom_out();
    void zoom_out_to(int x, int y);
    void zoom_out_to(const Complex& coords);


    void scroll(int dx, int dy);

    int width;
    int height;

    double zoom_size = 4;
    Complex centre = Complex(0, 0);

    Colour* buffer;

    // RenderedMandelbrot rendered_mandelbrot;

private:
    Config config;

    [[nodiscard]]
    Complex screen_to_complex(int x, int y) const;

    double aspect_ratio = (double)height / width;
};

Colour iterations_to_rgb(int iterations);
Colour iterations_to_rgb_using_trig(int iterations);

#endif // MANDELBROT_MANDELBROTRENDERER_HPP
