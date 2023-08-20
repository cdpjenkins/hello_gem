#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.hpp"
#include "ConwayGrid.hpp"

#define MAX_LINE_LENGTH 1000

ConwayGrid::ConwayGrid(int16 width, int16 height) : width(width), height(height) {
    init_to_blank();
}

void ConwayGrid::init_to_blank() {
    width = GRID_WIDTH;
    height = GRID_HEIGHT;
    cell_width = CELL_WIDTH;
    cell_height = CELL_HEIGHT;
    running = false;

    grid1.fill(false);
    grid2.fill(false);
}

void ConwayGrid::load_from_file(const char* filename) {
    FILE *fp;
    char row_string[MAX_LINE_LENGTH];

    init_to_blank();

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open %s\n", filename);
        exit(1);
    }

    for (int16 y = 0; fgets(row_string, MAX_LINE_LENGTH, fp) != NULL && y < height; y++) {
        int16 x;
        char* ptr;

        for (ptr = row_string, x = 0; ptr != NULL && x < width; ptr++, x++) {
            if (*ptr == 'x') {
                set_cell_from_false_to_true(grid1, x, y, true);
            }
        }
    }

    fclose(fp);
}

void ConwayGrid::save_to_file(const char* filename) {
    FILE *fp;
    uint16 x, y;

    fp = fopen(filename, "w");
    if (!fp) {
        printf("Failed to open %s for writing\n", filename);
        perror("arghghghgh");
        exit(1);
    }

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (cell_alive_at(x, y)) {
                fprintf(fp, "x");
            } else {
                fprintf(fp, ".");
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void ConwayGrid::print() {
    for (int16 y = 0; y < height; y++) {
        for (int16 x = 0; x < width; x++) {
            if (cell_alive_at(x, y)) {
                printf("x");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void ConwayGrid::step() {
    if (running) {
        memcpy(&grid2, &grid1, sizeof(grid1));

        for (int16 x = 1; x < width - 1; x++) {
            for (int16 y = 1; y < height - 1; y++) {
                bool new_value;

                int16 index = grid_index(x, y);

                int16 live_neighbours = grid2[index] & 0x0F;

                if (grid2[grid_index(x, y)] & 0x10) {
                    new_value = live_neighbours == 2 || live_neighbours == 3;

                    if (!new_value) {
                        set_cell_from_true_to_false(grid1, x, y, false);
                    }
                } else {
                    new_value = live_neighbours == 3;

                    if (new_value) {
                        set_cell_from_false_to_true(grid1, x, y, false);
                    }
                }
            }
        }
    }
}

void ConwayGrid::run() {
    running = true;
}

void ConwayGrid::pause() {
    running = false;
}

void ConwayGrid::screen_coords_to_grid_coords(int16 x, int16 y, int16 *grid_x, int16 *grid_y) {
    *grid_x = x / cell_width;
    *grid_y = y / cell_height;
}

void ConwayGrid::invert_cell(int16 grid_x, int16 grid_y) {
    if (!running) {
        bool cell_value = grid1[grid_index(grid_x, grid_y)];
        grid1[grid_index(grid_x, grid_y)] = !cell_value;
    }
}
