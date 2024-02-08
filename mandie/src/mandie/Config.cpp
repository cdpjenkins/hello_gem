#include <iostream>

#include "Config.hpp"

Config Config::parse(int argc, char** argv) {
    Config config;

    for (int i = 0; i < argc; i++) {
        string arg_key {argv[i]};

        if (arg_key == "--iteration-depth") {
            config.iteration_depth = stoi(argv[++i]);
        } else if (arg_key == "--zoom-factor") {
            config.zoom_factor = stod(argv[++i]);
        } else if (arg_key == "--max-zoom") {
            config.max_zoom = stod(argv[++i]);
        } else if (arg_key == "--pngs-dir") {
            config.pngs_dir = argv[++i];
        } else if (arg_key == "--auto-zoom-to") {
            config.auto_zoom = true;
            const auto *arg = argv[++i];
            Complex complex = Complex::parse(arg);
            config.zoom_to = complex;
        } else if (arg_key == "--initial-coords") {
            const auto arg = argv[++i];
            Complex complex = Complex::parse(arg);
            config.initial_coords = complex;
        } else if (arg_key == "--initial-zoom") {
            config.initial_zoom = stod(argv[++i]);
        } else if (arg_key == "--width") {
            config.initial_width = stoi(argv[++i]);
        } else if (arg_key == "--height") {
            config.initial_height = stoi(argv[++i]);
        } else if (arg_key == "--max-threads") {
            config.max_threads = stoi(argv[++i]);
        }
    }

    return config;
}

