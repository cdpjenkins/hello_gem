#ifndef MANDELBROT_CONFIG_HPP
#define MANDELBROT_CONFIG_HPP

#include <string>
using namespace std;

#include "Complex.hpp"

class Config {
public:
    int iteration_depth = 100;
    int depth_increment = 100;
    float zoom_factor = 0.7;
    float max_zoom = 0.000000000000000000001;
    string pngs_dir = "pngs"s;
    bool auto_zoom = false;
    Complex zoom_to = Complex(0, 0);
    Complex initial_coords = Complex(0, 0);
    float initial_zoom = 4.0;

    static Config parse(int argc, char** argv);

    int initial_width = 1280;
    int initial_height = 800;
    int max_threads = 16;
};

#endif // MANDELBROT_CONFIG_HPP
