#include "Mandelbrot.hpp"

int Mandelbrot::compute(const Complex& k) const {
    Complex z = Complex::ZERO();

    for (int n = 0; n < iteration_depth; n++) {
        z.square_and_add(k);
        if (z.norm() >= 16) {
            return n;
        }
    }

    return -1;
}
