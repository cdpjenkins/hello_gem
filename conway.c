#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "conway.h"

#define MAX_LINE_LENGTH 1000

void grid_init(ConwayGrid* grid) {
    grid->current_grid = grid->grid1;
    grid->next_grid = grid->grid2;
    grid->width = GRID_WIDTH;
    grid->height = GRID_HEIGHT;
    grid->cell_width = CELL_WIDTH;
    grid->cell_height = CELL_HEIGHT;
}

void grid_init_to_blank(ConwayGrid* grid) {
    grid_init(grid);

    // I'm not even sure if you can do this!
    memset(grid->grid1, 0, sizeof(bool) * grid->width * grid->height);
    memset(grid->grid2, 0, sizeof(bool) * grid->width * grid->height);
}

static int grid_index(ConwayGrid* grid, int column, int row) {
    return row * grid->width + column;
}

void grid_import_from_file(char* filename, ConwayGrid* grid) {
    FILE *fp;
    char row_string[MAX_LINE_LENGTH];
    int x, y;

    grid_init_to_blank(grid);

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open %s\n", filename);
        exit(1);
    }

    for (y = 0; fgets(row_string, MAX_LINE_LENGTH, fp) != NULL && y < grid->height; y++) {
        char* ptr;
        char c;

        for (ptr = row_string, x = 0; ptr != NULL && x < grid->width; ptr++, x++) {
            if (*ptr == 'x') {
                grid->current_grid[grid_index(grid, x, y)] = TRUE;
            } else {
                grid->current_grid[grid_index(grid, x, y)] = FALSE;
            }
        }
    }
}

void grid_print(ConwayGrid* grid) {
    int x, y;

    for (y = 0; y < grid->height; y++) {
        for (x = 0; x < grid->width; x++) {
            if (grid->current_grid[grid_index(grid, x, y)]) {
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

void grid_step(ConwayGrid* grid) {
    bool* temp_grid_ptr;
    int x, y;

    for (x = 0; x < grid->width; x++) {
        for (y = 0; y < grid->height; y++) {
            bool new_value;
            int live_neighbours;
            
            live_neighbours = num_living_neighbours(grid, x, y);
            if (grid_cell_alive_at(grid, x, y)) {
                new_value = live_neighbours == 2 || live_neighbours == 3;
            } else {
                new_value = live_neighbours == 3;
            }

            grid->next_grid[grid_index(grid, x, y)] = new_value;
        }
    }

    temp_grid_ptr = grid->next_grid;
    grid->next_grid = grid->current_grid;
    grid->current_grid = temp_grid_ptr;
}
