#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include "MandelbrotRenderer.hpp"

const int colour_cycle_period = 256;

// URGH - MiNTLib and/or gcc doesn't appear to have these
# define M_PI		3.14159265358979323846	/* pi */
# define M_PI_2		1.57079632679489661923	/* pi/2 */
# define M_PI_4		0.785398163397448309116	/* pi/4 */

MandelbrotRenderer::MandelbrotRenderer(int width, int height, const Config &config, const Complex &centre,
                                       float zoom_size) :
        width(width),
        height(height),
        config(config),
        centre(centre),
        zoom_size(zoom_size)
{}

Colour iterations_to_rgb(int iterations) {
    // This is kind of like doing HSV to RGB conversation, except:
    // thankfully we don't have to do *full* HSV->RGB conversation because
    // saturation and value are always full

    if (iterations == -1) {
        return Colour{0, 0, 0};
    }

    int segment_size = colour_cycle_period / 6;
    int segment = (iterations / segment_size) % 6;
    int pos_in_segment = iterations % segment_size;
    uint8_t segment_progress = (pos_in_segment * 31) / segment_size;
    uint8_t inverse_segment_progress = 31 - segment_progress;

    switch (segment) {
        case 0:
            return Colour{31, segment_progress, 0};
        case 1:
            return Colour{inverse_segment_progress, 31, 0};
        case 2:
            return Colour{0, 31, segment_progress};
        case 3:
            return Colour{0, inverse_segment_progress, 31};
        case 4:
            return Colour{segment_progress, 0, 31};
        case 5:
            return Colour{31, 0, inverse_segment_progress};
        default:
            // we can never get here, but if we do...
            return Colour{31, 31, 31};
    }
}

Colour iterations_to_rgb_using_trig(int iterations) {
    if (iterations == -1) {
        return Colour{0, 0, 0};
    }

    double theta = 2 * M_PI * iterations / colour_cycle_period;

    uint8_t r = uint8_t(sin(theta) * 15 + 15);
    uint8_t g = uint8_t(sin(theta - M_PI_4) * 15 + 15);
    uint8_t b = uint8_t(sin(theta - M_PI_2) * 15 + 15);

    return Colour{r, g, b};
}

void MandelbrotRenderer::render_to_buffer(Colour *buffer) {
    Colour* ptr = buffer;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Complex k = screen_to_complex(x, y);

            int n = compute(k);

            *ptr++ = iterations_to_rgb(n);
        }
    }
}

Complex MandelbrotRenderer::screen_to_complex(const int x, const int y) const {
    float x_progress = (float)x / (float)width;
    float y_progress = (float)y / (float)height;

    Complex top_left{centre.re - zoom_size, centre.im - zoom_size*aspect_ratio};

    float complex_width = zoom_size * 2;
    float complex_height = zoom_size * 2 * aspect_ratio;

    float re = top_left.re + complex_width * x_progress;
    float im = top_left.im + complex_height * y_progress;

    return Complex{re, im};
}

void MandelbrotRenderer::zoom_in() {
    zoom_in_to(this->centre);
}

void MandelbrotRenderer::zoom_in_to(const int x, const int y) {
    centre = screen_to_complex(x, y);
    zoom_in_to(centre);
}

void MandelbrotRenderer::zoom_in_to(const Complex& coords) {
    zoom_size *= config.zoom_factor;
    this->centre = coords;
}

void MandelbrotRenderer::zoom_out() {
    zoom_out_to(this->centre);
}

void MandelbrotRenderer::zoom_out_to(const int x, const int y) {
    centre = screen_to_complex(x, y);
    zoom_size /= config.zoom_factor;
}

void MandelbrotRenderer::zoom_out_to(const Complex& coords) {
    zoom_size /= config.zoom_factor;
    this->centre = coords;
}

int MandelbrotRenderer::compute(const Complex &k) const {
    Complex z = Complex::ZERO();

    for (int n = 0; n < config.iteration_depth; n++) {
        z.square_and_add(k);
        if (z.norm() >= 16) {
            return n;
        }
    }

    return -1;
}
