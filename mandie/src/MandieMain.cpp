#include <stdio.h>

#include <mint/sysbind.h>
#include <mint/arch/nf_ops.h>

#include <array>
#include <memory>

#include "Colour.hpp"
#include "MandelbrotRenderer.hpp"
#include "Config.hpp"

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

constexpr int16_t PLANES_1 = 0x00;
constexpr int16_t PLANES_2 = 0x01;
constexpr int16_t PLANES_4 = 0x02;
constexpr int16_t PLANES_8 = 0x03;
constexpr int16_t PLANES_16 = 0x04;
constexpr int16_t WIDTH_640 = 0x08;
constexpr int16_t VGA = 0x10;
constexpr int16_t NTSC = 0x00;

constexpr int16_t REZ_FROM_MODE = 3;

constexpr int16_t SCREEN_WIDTH = 640;
constexpr int16_t SCREEN_HEIGHT = 480;

using ScreenArray = std::array<uint16_t, SCREEN_WIDTH * SCREEN_HEIGHT>;
ScreenArray screen;
uint16_t *physical_screen = screen.data();

void natfeats_init();
uint32_t read_system_timer();

void draw(MandelbrotRenderer &mandie) {
    uint32_t time_before = read_system_timer();

    mandie.render_to_buffer((Colour*)physical_screen);

    uint32_t time_after_draw = read_system_timer();

    nf_debugprintf("draw: %dms\n", (time_after_draw - time_before) * 5);
}

int main(int argc, char *argv[]) {
    natfeats_init();

    Config config;
    std::unique_ptr<MandelbrotRenderer> mandie = make_unique<MandelbrotRenderer>(
        640, 480, config
    );

    Cursconf(0, 0);

    screen.fill(0xFFFF);

    int16_t saved_rez = VsetMode(-1);

    void *saved_logbase = Logbase();
    void *saved_physbase = Physbase();

    VsetScreen(physical_screen, physical_screen, REZ_FROM_MODE, PLANES_16 | WIDTH_640 | VGA | NTSC);

    bool quit = false;
    while (!quit) {
        draw(*mandie);

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
                mandie->zoom_in();
                break;
            case 'x':
            case 'X':
                mandie->zoom_out();
                break;
        }

        switch (scancode) {
            case UP:
                mandie->centre = mandie->centre + Complex(0, -1 / mandie->zoom_size);
                break;
            case DOWN:
                mandie->centre = mandie->centre + Complex(0, 1 / mandie->zoom_size);
                break;
            case LEFT:
                mandie->centre = mandie->centre + Complex(-1 / mandie->zoom_size, 0);
                break;
            case RIGHT:
                mandie->centre = mandie->centre + Complex(1 / mandie->zoom_size, 0);
                break;
        }
    }

    VsetScreen(saved_logbase, saved_physbase, REZ_FROM_MODE, saved_rez);

    return 0;
}

static uint32_t read_system_timer_in_supervisor_mode() {
    return *(uint32_t *) 0x0004ba;
}

uint32_t read_system_timer() {
    return Supexec(read_system_timer_in_supervisor_mode);
}

void natfeats_init() {
    nf_ops *nf_context = nf_init();
    if (!nf_context) {
        printf("Unable to initialise natfeats\n");
    } else {
        printf("Natfeats is initialised: %08X\n", nf_context);
        nf_debugprintf("Natfeats is initialised: %08X\n", nf_context);
    }
}
