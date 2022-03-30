#include <stdio.h>
#include <mint/sysbind.h>
#include "ConwayGrid.hpp"

const int16 PLANES_1 = 0x00;
const int16 PLANES_2 = 0x01;
const int16 PLANES_4 = 0x02;
const int16 PLANES_8 = 0x03;
const int16 PLANES_16 = 0x04;
const int16 WIDTH_640 = 0x08;
const int16 VGA = 0x10;
const int16 NTSC = 0x00;

const int16 REZ_FROM_MODE = 3;

const int16 WIDTH = 640;
const int16 HEIGHT = 480;
const int16 WIDTH_IN_BLOCKS = WIDTH / 16;
const int16 HEIGHT_IN_BLOCKS = HEIGHT / 16;

uint16 screen1[WIDTH * HEIGHT / 16];
uint16 screen2[WIDTH * HEIGHT / 16];

uint16 *logical_screen = screen1;
uint16 *physical_screen = screen2;

int16 block_index(int16 x, int16 y) {
    return y * WIDTH_IN_BLOCKS * 16 + x;
}

char read_key() {
    uint32 key = Crawio(0x00FF);

    uint8 scan_code = key & 0x00FF0000 >> 16;
    uint8 ascii = key & 0x000000FF;

    return ascii;
}

int main(int argc, char *argv[]) {
    ConwayGrid grid;
    grid.load_from_file("gosper.cwy");

    Cursconf(0, 0);

    int16 saved_rez = VsetMode(-1);

    void *saved_logbase = Logbase();
    void *saved_physbase = Physbase();

    VsetScreen(logical_screen, physical_screen, REZ_FROM_MODE, PLANES_1 | WIDTH_640 | VGA | NTSC);

    // // Not sure why we need to do this one, but the call to Cconis returns something if we don't
    // // swallow a keypress here
    // Cconin();

    grid.run();
    bool quit = false;
    while (!quit) {
        grid.step();

        for (int y = 0; y < HEIGHT_IN_BLOCKS; y += 1) {
            for (int x = 0; x < WIDTH_IN_BLOCKS; x += 1) {
                if (grid.cell_alive_at(x, y)) {
                    for (int16 i = 0, index = block_index(x, y); i < 16; i++, index += WIDTH_IN_BLOCKS) {
                        if (i != 0 && i != 15) {
                            logical_screen[index] = 0b0111111111111110;
                        }
                    }
                } else {
                    for (int16 i = 0, index = block_index(x, y); i < 16; i++, index += WIDTH_IN_BLOCKS) {
                        if (i != 0 && i != 15) {
                            logical_screen[index] = 0b00000000000000000;
                        }
                    }
                }
            }
        }

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

    return 0;
}

