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

uint16 logical_screen[WIDTH * HEIGHT / 16];
uint16 physical_screen[WIDTH * HEIGHT / 16];

int16 block_index(int16 x, int16 y) {
    return y * WIDTH_IN_BLOCKS * 16 + x;
}

int main(int argc, char *argv[]) {
    printf("Yo!\n");

    Cursconf(0, 0);

    int16 saved_rez = VsetMode(-1);

    printf("Rez: %X\n", saved_rez);

    void *saved_logbase = Logbase();
    void *saved_physbase = Physbase();

    printf("logical screen: %X\n",  saved_logbase);
    printf("physical screen: %X\n", saved_physbase);

    printf("About to change rez\n");
    Cconin();

    VsetScreen(logical_screen, physical_screen, REZ_FROM_MODE, PLANES_1 | WIDTH_640 | VGA | NTSC);

    printf("Rez: %X\n", VsetMode(-1));

    printf("logical screen: %X\n",  Logbase());
    printf("physical screen: %X\n", Physbase());

    printf("Saved screen size: %X\n", VgetSize(saved_rez));
    printf("New screen size: %X\n", VgetSize(PLANES_16 | WIDTH_640 | VGA | NTSC));


    Vsync();
    for (int y = 0; y < HEIGHT_IN_BLOCKS; y += 1) {
        for (int x = 0; x < WIDTH_IN_BLOCKS; x += 1) {
            for (int16 i = 0, index = block_index(x, y); i < 16; i++, index += WIDTH_IN_BLOCKS) {
                if (i != 0 && i != 15) {
                    physical_screen[index] = 0b0111111111111110;
                }
            }
            Vsync();
        }
    }

    Cconin();

    VsetScreen(saved_logbase, saved_physbase, REZ_FROM_MODE, saved_rez);

    printf("logical screen: %X\n",  Logbase());
    printf("physical screen: %X\n", Physbase());


    Cconin();

    return 0;
}

