#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "ConwayGrid.hpp"

#define MAX_LINE_LENGTH 1000

ConwayGrid::ConwayGrid(int16 width, int16 height) :
        width(width),
        height(height),
        cell_width(CELL_WIDTH),
        cell_height(CELL_HEIGHT),
        running(false),
        grid1(),
        grid2() {
}

void ConwayGrid::init_to_blank() {
    running = false;

    grid1.fill(0x00);
    grid2.fill(0x00);

    for (int16 x = 0; x < width; x++) {
        grid1[grid_index(x, 0)] = 0x20;
        grid1[grid_index(x, height - 1)] = 0x20;
    }

    for (int16 y = 0; y < height; y++) {
        grid1[grid_index(0, y)] = 0x20;
        grid1[grid_index(width - 1 , y)] = 0x20;
    }
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

    for (int16 y = 0; fgets(row_string, MAX_LINE_LENGTH, fp) != nullptr && y < height; y++) {
        int16 x;
        char* ptr;

        for (ptr = row_string, x = 0; ptr != nullptr && x < width; ptr++, x++) {
            if (*ptr == 'x') {
                transition_cell_from_dead_to_alive(grid1, x, y);
            }
        }
    }

    fclose(fp);
}

void ConwayGrid::save_to_file(const char* filename) {
    FILE *fp;
    int16 x, y;

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

        int16 index = 0;
        for (int16 y = 0; y < height; y++) {
            for (int16 x = 0; x < width; x++, index++) {
                uint8 &current_value = grid2[index];
                if (current_value != 0) {
                    if (current_value == 0x03) {
                        transition_cell_from_dead_to_alive(grid1, index);
                    } else if ((current_value & 0x10) && (current_value != 0x12) && (current_value != 0x13)) {
                        transition_cell_from_alive_to_dead(grid1, index);
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

void ConwayGrid::screen_coords_to_grid_coords(int16 x, int16 y, int16 *grid_x, int16 *grid_y) const {
    *grid_x = x / cell_width;
    *grid_y = y / cell_height;
}

void ConwayGrid::invert_cell(int16 grid_x, int16 grid_y) {
    if (!running) {
        if (cell_alive_at(grid_x, grid_y)) {
            transition_cell_from_alive_to_dead(grid1, grid_x, grid_y);
        } else {
            transition_cell_from_dead_to_alive(grid1, grid_x, grid_y);
        }
    }
}