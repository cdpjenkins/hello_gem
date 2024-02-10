#ifndef MANDELBROT_COMPLEX_HPP
#define MANDELBROT_COMPLEX_HPP

#include <sstream>
#include <iostream>
#include <cmath>
using namespace std;

template <typename T>
class ComplexTemplate {
public:
    T re, im;

    ComplexTemplate(const T re, const T im) {
        this->re = re;
        this->im = im;
    }

    [[maybe_unused]] [[nodiscard]]
    inline ComplexTemplate squared() const {
        T new_re = re*re - im*im;
        T new_im = re * im * 2;

        return ComplexTemplate{new_re, new_im};
    }

    inline void square_and_add(const ComplexTemplate& k) {
        T new_re = re*re - im*im + k.re;
        T new_im = re * im * 2 + k.im;

        re = new_re;
        im = new_im;
    }

    inline ComplexTemplate operator+(const ComplexTemplate& that) const {
        return ComplexTemplate{this->re + that.re, this->im + that.im};
    }

    [[nodiscard]]
    inline T norm() const {
        return re*re + im*im;
    }

    static inline ComplexTemplate ZERO() {
        return ComplexTemplate{0, 0};
    }

    static ComplexTemplate parse(const char *arg) {
        istringstream coords_string_stream(arg);
        string re_string;
        string im_string;
        getline(coords_string_stream, re_string, ',');
        getline(coords_string_stream, im_string, ',');
        return ComplexTemplate{stof(re_string), stof(im_string)};
    }
};

#define ComplexType float
typedef ComplexTemplate<ComplexType> Complex;

#endif // MANDELBROT_COMPLEX_HPP