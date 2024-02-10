#include <stdio.h>

#include <mint/sysbind.h>
#include <mint/linea.h>

#include <cstring>
#include <array>
#include <memory>

#include "../ConwayGrid.hpp"
#include "../STScreen.hpp"
#include "../asm/fast_draw.h"

#include "../mandie/Colour.hpp"
#include "../mandie/MandelbrotRenderer.hpp"
#include "../mandie/Config.hpp"

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

int16 block_index(int16 x, int16 y) {
    return y * WIDTH_IN_BLOCKS * 16 + x;
}

char read_key() {
    uint32 key = Crawio(0x00FF);

    uint8 scan_code = key & 0x00FF0000 >> 16;
    uint8 ascii = key & 0x000000FF;

    return ascii;
}

static inline void draw_strip_c(void *strip_src, uint16 *dest) {
    for (int16 i = 0; i < 4; i++) {
        std::memcpy(dest, strip_src, 640);
        dest += 640;
    }
}

static inline void draw_strip(uint16 *strip, uint16 *ptr) {
    draw_strip_c(strip, ptr);
}

void draw(Grid& grid, MandelbrotRenderer& mandie) {
    Colour* pixel_ptr = (Colour*)physical_screen;

    Mandelbrot mandelbrot{50};

    mandie.render_to_buffer(mandelbrot);
}

int main(int argc, char *argv[]) {
    bool quit = false;

    uint32_t key_now = Cconin();

    printf("key now: %08X\n", key_now);
    Cconin();

    std::unique_ptr<Grid> grid = make_unique<Grid>();

    Config config;

    std::unique_ptr<MandelbrotRenderer> mandie = make_unique<MandelbrotRenderer>(
        640, 480, config, (Colour*)physical_screen
    );

    STScreen screen;

    if (argc > 1) {
        grid->load_from_file(argv[1]);
    } else {
        grid->load_from_file("gosper.cwy");
    }

    Cursconf(0, 0);

    screen1.fill(0xFFFF);
    screen2.fill(0xFFFF);

    int16 saved_rez = VsetMode(-1);

    void *saved_logbase = Logbase();
    void *saved_physbase = Physbase();

    VsetScreen(logical_screen, physical_screen, REZ_FROM_MODE, PLANES_16 | WIDTH_640 | VGA | NTSC);

    grid->run();

    Super(0);

    volatile uint32 *timer = (uint32 *)0x0004ba;
    uint32 time_before_step;
    uint32 time_after_step_before_draw;
    uint32 time_after_draw;

    while (!quit) {
        time_before_step = *timer;

        time_after_step_before_draw = *timer;

        draw(*grid, *mandie);

        time_after_draw = *timer;

        uint32_t key = Cconin();
        uint16_t ascii = (key & 0x000000FF);
        uint16_t scancode = key >> 16;
        if (ascii == 'Q' || ascii == 'q') {
            quit = true;
        }
        if (ascii == 'z') {
            mandie->zoom_in();
        }
        if (ascii == 'x') {
            mandie->zoom_out();
        }

        if (scancode == 72) {
            // up arrow
            mandie->centre = mandie->centre + Complex(0, -1 / mandie->zoom_size);
        } 

        if (scancode == 80) {
            // down arrow
            mandie->centre = mandie->centre + Complex(0, 1 / mandie->zoom_size);
        } 

        if (scancode == 75) {
            // left arrow
            mandie->centre = mandie->centre + Complex(-1 / mandie->zoom_size, 0);
        }

        if (scancode == 77) {
            // right arrow
            mandie->centre = mandie->centre + Complex(-1 / mandie->zoom_size, 0);
        } 

    }

    VsetScreen(saved_logbase, saved_physbase, REZ_FROM_MODE, saved_rez);

    printf("Timings:\n");
    printf("step: %dms\n", (time_after_step_before_draw - time_before_step) * 5);
    printf("draw: %dms\n", (time_after_draw - time_after_step_before_draw) * 5);

    Cconin();

    return 0;
}
