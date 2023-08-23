#include <stdio.h>

#include <mint/sysbind.h>
#include <mint/linea.h>

#include <cstring>

#include "ConwayGrid.hpp"
#include "STScreen.hpp"
#include "asm/fast_draw.h"

#ifndef VsetScreen
#define VsetScreen Vsetscreen
#endif

constexpr int16 PLANES_1 = 0x00;
constexpr int16 PLANES_2 = 0x01;
constexpr int16 PLANES_4 = 0x02;
constexpr int16 PLANES_8 = 0x03;
constexpr int16 PLANES_16 = 0x04;
constexpr int16 WIDTH_640 = 0x08;
constexpr int16 VGA = 0x10;
constexpr int16 NTSC = 0x00;

constexpr int16 REZ_FROM_MODE = 3;

constexpr int16 WIDTH = 640;
constexpr int16 HEIGHT = 480;
constexpr int16 CELL_SIZE = 8;
constexpr int16 WIDTH_IN_BLOCKS = WIDTH / 16;
constexpr int16 WIDTH_IN_CELLS = WIDTH / CELL_SIZE;
constexpr int16 HEIGHT_IN_CELLS = HEIGHT / CELL_SIZE;

uint16 screen1[WIDTH * HEIGHT / 16];
uint16 screen2[WIDTH * HEIGHT / 16];

uint16 *logical_screen = screen1;
uint16 *physical_screen = screen2;

int16 block_index(int16 x, int16 y) {
    return y * WIDTH_IN_BLOCKS * 16 + x;
}

using Grid = ConwayGrid<80, 60>;

char read_key() {
    uint32 key = Crawio(0x00FF);

    uint8 scan_code = key & 0x00FF0000 >> 16;
    uint8 ascii = key & 0x000000FF;

    return ascii;
}

static inline void draw_strip_c(void *strip_src, uint16 *dest) {
    for (int16 i = 0; i < 7; i++) {
        std::memcpy(dest, strip_src, WIDTH_IN_BLOCKS * 2);
        dest += WIDTH_IN_BLOCKS;
    }
}

static inline void draw_strip(uint8 *strip, uint16 *ptr) {
#ifdef FAST_DRAW
    fast_draw_strip(strip, ptr);
#else
    draw_strip_c(strip, ptr);
#endif
}

void draw_in_strips(Grid *grid) {
    uint16 *ptr = logical_screen;

    for (int16 y = 0; y < HEIGHT_IN_CELLS; y += 1) {
        uint8 strip[WIDTH_IN_CELLS];
        for (int16 i = 0; i < WIDTH_IN_CELLS; i++) {
            if (grid->cell_alive_at(i, y)) {
                strip[i] = 0b11111110;
            } else {
                strip[i] = 0b00000000;
            }
        }

        draw_strip(strip, ptr);

        ptr += WIDTH_IN_BLOCKS * 8;
    }
}

int main(int argc, char *argv[]) {
    Grid grid;
    STScreen screen;

    if (argc > 1) {
        grid.load_from_file(argv[1]);
    } else {
        grid.load_from_file("gosper.cwy");
    }

    Cursconf(0, 0);

    int16 saved_rez = VsetMode(-1);

    void *saved_logbase = Logbase();
    void *saved_physbase = Physbase();

    VsetScreen(logical_screen, physical_screen, REZ_FROM_MODE, PLANES_1 | WIDTH_640 | VGA | NTSC);

    grid.run();
    bool quit = false;

    Super(0);

    volatile uint32 *timer = (uint32 *)0x0004ba;
    uint32 time_before_step;
    uint32 time_after_step_before_draw;
    uint32 time_after_draw;

    while (!quit) {
        time_before_step = *timer;

        grid.step();

        time_after_step_before_draw = *timer;

        draw_in_strips(&grid);

        time_after_draw = *timer;

        Vsync();

        uint16 *temp;
        temp = logical_screen;
        logical_screen = physical_screen;
        physical_screen = temp;

        VsetScreen(logical_screen, physical_screen, -1, -1);

        while (Cconis()) {
            char ascii = read_key();

            switch (ascii) {
                case 'q':
                case 'Q':
                    quit = true;
                    break;
                case 'r':
                case 'R':
                    grid.run();
                    break;
                case 'p':
                case 'P':
                    grid.pause();
                    break;
            }
        }
    }

    VsetScreen(saved_logbase, saved_physbase, REZ_FROM_MODE, saved_rez);

    printf("Timings:\n");
    printf("step: %dms\n", (time_after_step_before_draw - time_before_step) * 5);
    printf("draw: %dms\n", (time_after_draw - time_after_step_before_draw) * 5);

    Cconin();

    return 0;
}
