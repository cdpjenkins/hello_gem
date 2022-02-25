#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#define GRID_WIDTH 10
#define GRID_HEIGHT 10

#define MAX_LINE_LENGTH 1000

typedef struct {
    bool grid1[GRID_WIDTH * GRID_HEIGHT];
    bool grid2[GRID_WIDTH * GRID_HEIGHT];
    bool* current_grid;
    bool* next_grid;
} ConwayGrid;

void grid_init(ConwayGrid* grid) {
    grid->current_grid = grid->grid1;
    grid->next_grid = grid->grid2;
}

void grid_init_to_blank(ConwayGrid* grid) {
    grid_init(grid);

    // I'm not even sure if you can do this!
    memset(grid->grid1, 0, sizeof(bool) * GRID_WIDTH * GRID_HEIGHT);
    memset(grid->grid2, 0, sizeof(bool) * GRID_WIDTH * GRID_HEIGHT);
}

static int grid_index(int column, int row) {
    return row * GRID_WIDTH + column;
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

    for (y = 0; fgets(row_string, MAX_LINE_LENGTH, fp) != NULL && y < GRID_HEIGHT; y++) {
        char* ptr;
        char c;

        for (ptr = row_string, x = 0; ptr != NULL && x < GRID_WIDTH; ptr++, x++) {
            if (*ptr == 'x') {
                grid->current_grid[grid_index(x, y)] = TRUE;
            } else {
                grid->current_grid[grid_index(x, y)] = FALSE;
            }
        }
    }
}

void grid_print(ConwayGrid* grid) {
    int x, y;

    for (y = 0; y < GRID_HEIGHT; y++) {
        for (x = 0; x < GRID_WIDTH; x++) {
            if (grid->current_grid[grid_index(x, y)]) {
                printf("x");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");
}

static bool cell_alive_at(ConwayGrid* grid, int x, int y) {
    if (x < 0 || x >= GRID_WIDTH
     || y < 0 || y >= GRID_HEIGHT) {
        return FALSE;
    } else {
        return grid->current_grid[grid_index(x, y)];
    }
}

static int num_living_neighbours(ConwayGrid* grid, int x, int y) {
    int a1, a2, a3, a4, a5, a6, a7, a8;

    a1 = cell_alive_at(grid, x-1, y-1) ? 1 : 0;
    a2 = cell_alive_at(grid, x, y-1) ? 1 : 0;
    a3 = cell_alive_at(grid, x+1, y-1) ? 1 : 0;
    a4 = cell_alive_at(grid, x-1, y) ? 1 : 0;
    a5 = cell_alive_at(grid, x+1, y) ? 1 : 0;
    a6 = cell_alive_at(grid, x-1, y+1) ? 1 : 0;
    a7 = cell_alive_at(grid, x, y+1) ? 1 : 0;
    a8 = cell_alive_at(grid, x+1, y+1) ? 1 : 0;

    return a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8;
}

void grid_step(ConwayGrid* grid) {
    bool* temp_grid_ptr;
    int x, y;

    for (x = 0; x < GRID_WIDTH; x++) {
        for (y = 0; y < GRID_WIDTH; y++) {
            bool new_value;
            int live_neighbours;
            
            live_neighbours = num_living_neighbours(grid, x, y);
            if (cell_alive_at(grid, x, y)) {
                new_value = live_neighbours == 2 || live_neighbours == 3;
            } else {
                new_value = live_neighbours == 3;
            }

            grid->next_grid[grid_index(x, y)] = new_value;
        }
    }

    temp_grid_ptr = grid->next_grid;
    grid->next_grid = grid->current_grid;
    grid->current_grid = temp_grid_ptr;
}
