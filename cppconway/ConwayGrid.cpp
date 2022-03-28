#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.hpp"
#include "ConwayGrid.hpp"

#define MAX_LINE_LENGTH 1000

ConwayGrid::ConwayGrid() {
    init_to_blank();
}

void ConwayGrid::init_to_blank() {
    current_grid = grid1;
    next_grid = grid2;
    width = GRID_WIDTH;
    height = GRID_HEIGHT;
    cell_width = CELL_WIDTH;
    cell_height = CELL_HEIGHT;
    running = FALSE;

    memset(this->grid1, 0, sizeof(grid1));
    memset(this->grid2, 0, sizeof(grid2));
}

void ConwayGrid::load_from_file(const char* filename) {
    FILE *fp;
    char row_string[MAX_LINE_LENGTH];
    int x, y;

    init_to_blank();

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open %s\n", filename);
        exit(1);
    }

    for (y = 0; fgets(row_string, MAX_LINE_LENGTH, fp) != NULL && y < height; y++) {
        char* ptr;
        char c;

        for (ptr = row_string, x = 0; ptr != NULL && x < width; ptr++, x++) {
            int i = grid_index(this, x, y);
            if (*ptr == 'x') {
                current_grid[i] = TRUE;
            } else {
                current_grid[i] = FALSE;
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
            if (grid_cell_alive_at(this, x, y)) {
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
    int x, y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (current_grid[grid_index(this, x, y)]) {
                printf("x");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");
}

bool grid_cell_alive_at(ConwayGrid* grid, int x, int y) {
    if (x < 0 || x >= grid->width
     || y < 0 || y >= grid->height) {
        return FALSE;
    } else {
        if (grid->current_grid[grid_index(grid, x, y)]) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
}

static int num_living_neighbours(ConwayGrid* grid, int x, int y) {
    int a1, a2, a3, a4, a5, a6, a7, a8;

    a1 = grid_cell_alive_at(grid, x-1, y-1) ? 1 : 0;
    a2 = grid_cell_alive_at(grid, x, y-1) ? 1 : 0;
    a3 = grid_cell_alive_at(grid, x+1, y-1) ? 1 : 0;
    a4 = grid_cell_alive_at(grid, x-1, y) ? 1 : 0;
    a5 = grid_cell_alive_at(grid, x+1, y) ? 1 : 0;
    a6 = grid_cell_alive_at(grid, x-1, y+1) ? 1 : 0;
    a7 = grid_cell_alive_at(grid, x, y+1) ? 1 : 0;
    a8 = grid_cell_alive_at(grid, x+1, y+1) ? 1 : 0;

    return a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8;
}

void ConwayGrid::step() {
    bool* temp_grid_ptr;
    int x, y;

    if (running) {
        for (x = 0; x < width; x++) {
            for (y = 0; y < height; y++) {
                bool new_value;
                int live_neighbours;
                
                live_neighbours = num_living_neighbours(this, x, y);
                if (grid_cell_alive_at(this, x, y)) {
                    new_value = live_neighbours == 2 || live_neighbours == 3;
                } else {
                    new_value = live_neighbours == 3;
                }

                next_grid[grid_index(this, x, y)] = new_value;
            }
        }

        temp_grid_ptr = next_grid;
        next_grid = current_grid;
        current_grid = temp_grid_ptr;
    }
}

void grid_run(ConwayGrid* grid) {
    grid->running = TRUE;
}

void grid_pause(ConwayGrid* grid) {
    grid->running = FALSE;
}

void grid_screen_coords_to_grid_coords(int x, int y, int* grid_x, int* grid_y) {
    *grid_x = x / CELL_WIDTH;
    *grid_y = y / CELL_HEIGHT;
}

void grid_invert_cell(ConwayGrid* grid, int grid_x, int grid_y) {
    if (!grid->running) {
        bool cell_value = grid->current_grid[grid_index(grid, grid_x, grid_y)];

        grid->current_grid[grid_index(grid, grid_x, grid_y)] = !cell_value;
    }
}