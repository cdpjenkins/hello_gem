#ifndef MANDELBROT_MANDELBROT_HPP
#define MANDELBROT_MANDELBROT_HPP

#include "Complex.hpp"

class Mandelbrot {
public:
    explicit Mandelbrot(int iteration_depth) {
        this->iteration_depth = iteration_depth;
    }

    [[nodiscard]]
    int compute(const Complex& k) const;
    int iteration_depth;
};

#endif // MANDELBROT_MANDELBROT_HPP
