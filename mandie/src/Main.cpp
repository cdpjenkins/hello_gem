#include <stdio.h>


#include <array>
#include <memory>

#include "Colour.hpp"
#include "MandelbrotRenderer.hpp"
#include "Config.hpp"
#include "Screen.hpp"
#include "App.hpp"

#ifndef VsetScreen
#define VsetScreen Vsetscreen
#endif

enum ScanCode {
    UP = 72,
    DOWN = 80,
    LEFT = 75,
    RIGHT = 77,
    KEY_Q = 16
};

uint32_t read_system_timer();

void draw_timed(MandelbrotRenderer &mandie, Screen &screen) {
    uint32_t time_before = read_system_timer();

    mandie.render_to_buffer(reinterpret_cast<Colour *>(screen.get_frame_buffer()));

    uint32_t time_after_draw = read_system_timer();

    nf_debugprintf("draw_timed: %dms\n", (time_after_draw - time_before) * 5);
}

int main(int argc, char *argv[]) {
    App app;


    bool quit = false;
    while (!quit) {
        draw_timed(*app.mandie, app.screen);

        uint32_t key = Cconin();
        nf_debugprintf("key: %08X\n", key);
        uint16_t ascii = (key & 0x000000FF);
        uint16_t scancode = key >> 16;

        switch (ascii) {
            case 'q':
            case 'Q':
                quit = true;
                break;
            case 'z':
            case 'Z':
                app.mandie->zoom_in();
                break;
            case 'x':
            case 'X':
                app.mandie->zoom_out();
                break;
        }

        switch (scancode) {
            case UP:
                app.mandie->centre = app.mandie->centre + Complex(0, -1 / app.mandie->zoom_size);
                break;
            case DOWN:
                app.mandie->centre = app.mandie->centre + Complex(0, 1 / app.mandie->zoom_size);
                break;
            case LEFT:
                app.mandie->centre = app.mandie->centre + Complex(-1 / app.mandie->zoom_size, 0);
                break;
            case RIGHT:
                app.mandie->centre = app.mandie->centre + Complex(1 / app.mandie->zoom_size, 0);
                break;
        }
    }


    return 0;
}

static uint32_t read_system_timer_in_supervisor_mode() {
    return *(uint32_t *) 0x0004ba;
}

uint32_t read_system_timer() {
    return Supexec(read_system_timer_in_supervisor_mode);
}