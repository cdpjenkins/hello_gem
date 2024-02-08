#ifndef MANDELBROT_MANDELBROTRENDERER_HPP
#define MANDELBROT_MANDELBROTRENDERER_HPP

#include <iostream>
using namespace std;

#include "Colour.hpp"
#include "Config.hpp"
#include "Mandelbrot.hpp"
#include "RenderedMandelbrot.hpp"

class MandelbrotRenderer {
public:
    MandelbrotRenderer(
        int screen_width,
        int screen_height,
        const Config& config,
        const Complex& centre = Complex(0, 0),
        double zoom_size = 4.0
    );

    void render_to_buffer(const Mandelbrot& render_future);
    void zoom_in_to(int x, int y);
    void zoom_in_to(const Complex& coords);
    void zoom_out_to(int x, int y);

    void scroll(int dx, int dy);

    int screen_width;
    int screen_height;

    double zoom_size = 4;
    Complex centre = Complex(0, 0);

    RenderedMandelbrot rendered_mandelbrot;

private:
    Config config;

    [[nodiscard]]
    Complex screen_to_complex(int x, int y) const;

    double aspect_ratio = (double)screen_height / screen_width;
};

Colour iterations_to_rgb(int iterations);
Colour iterations_to_rgb_using_trig(int iterations);

#endif // MANDELBROT_MANDELBROTRENDERER_HPP
