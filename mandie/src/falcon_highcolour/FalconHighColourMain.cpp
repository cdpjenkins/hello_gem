#include <stdio.h>

#include <mint/sysbind.h>
#include <mint/linea.h>

#include <mint/arch/nf_ops.h>

#include <cstring>
#include <array>
#include <memory>

#include "../ConwayGrid.hpp"
#include "../asm/fast_draw.h"

#include "../mandie/Colour.hpp"
#include "../mandie/MandelbrotRenderer.hpp"
#include "../mandie/Config.hpp"

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

constexpr int16 PLANES_1 = 0x00;
constexpr int16 PLANES_2 = 0x01;
constexpr int16 PLANES_4 = 0x02;
constexpr int16 PLANES_8 = 0x03;
constexpr int16 PLANES_16 = 0x04;
constexpr int16 WIDTH_640 = 0x08;
constexpr int16 VGA = 0x10;
constexpr int16 NTSC = 0x00;

constexpr int16 REZ_FROM_MODE = 3;

constexpr int16 SCREEN_WIDTH = 640;
constexpr int16 SCREEN_HEIGHT = 480;

constexpr int16 CELL_SIZE = 8;
constexpr int16 CELL_SIZE_MINUS_PADDING = max(CELL_SIZE - 1, 1);
constexpr int16 WIDTH_IN_BLOCKS = SCREEN_WIDTH / 16;
constexpr int16 WIDTH_IN_CELLS = SCREEN_WIDTH / CELL_SIZE;
constexpr int16 HEIGHT_IN_CELLS = SCREEN_HEIGHT / CELL_SIZE;

using Grid = ConwayGrid<SCREEN_WIDTH / CELL_SIZE, SCREEN_HEIGHT / CELL_SIZE>;

using ScreenArray = std::array<uint16, SCREEN_WIDTH * SCREEN_HEIGHT>;

ScreenArray screen1;
ScreenArray screen2;

uint16 *logical_screen = screen1.data();
uint16 *physical_screen = screen2.data();

void draw(MandelbrotRenderer &mandie) {
    mandie.render_to_buffer();
}

int main(int argc, char *argv[]) {
    nf_ops *nf_thingie = nf_init();
    nf_debugprintf("Natfeats is initialised: %08X\n", nf_thingie);

    Config config;
    std::unique_ptr<MandelbrotRenderer> mandie = make_unique<MandelbrotRenderer>(
        640, 480, config, (Colour*)physical_screen
    );

    Cursconf(0, 0);

    screen1.fill(0xFFFF);
    screen2.fill(0xFFFF);

    int16 saved_rez = VsetMode(-1);

    void *saved_logbase = Logbase();
    void *saved_physbase = Physbase();

    VsetScreen(logical_screen, physical_screen, REZ_FROM_MODE, PLANES_16 | WIDTH_640 | VGA | NTSC);

    Super(0);

    volatile uint32 *timer = (uint32 *)0x0004ba;

    bool quit = false;
    while (!quit) {
        uint32 time_before = *timer;
        draw(*mandie);
        uint32 time_after_draw = *timer;

        nf_debugprintf("Timings:\n");
        nf_debugprintf("draw: %dms\n", (time_after_draw - time_before) * 5);

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
