#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "conway_grid.h"

#define MAX_LINE_LENGTH 1000

void grid_init(ConwayGrid* grid) {
    grid->current_grid = grid->grid1;
    grid->next_grid = grid->grid2;
    grid->width = GRID_WIDTH;
    grid->height = GRID_HEIGHT;
    grid->cell_width = CELL_WIDTH;
    grid->cell_height = CELL_HEIGHT;
    grid->running = FALSE;
}

void grid_init_to_blank(ConwayGrid* grid) {
    grid_init(grid);

    // I'm not even sure if you can do this!
    memset(grid->grid1, 0, sizeof(bool) * grid->width * grid->height);
    memset(grid->grid2, 0, sizeof(bool) * grid->width * grid->height);
}

static inline int grid_index(ConwayGrid* grid, int column, int row) {
    return row * grid->width + column;
}

void grid_load_from_file(char* filename, ConwayGrid* grid) {
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
            int i = grid_index(grid, x, y);
            if (*ptr == 'x') {
                grid->current_grid[i] = TRUE;
            } else {
                grid->current_grid[i] = FALSE;
            }
        }
    }

    fclose(fp);
}

void grid_save_to_file(ConwayGrid* grid, char* filename) {
    FILE *fp;
    uint16 x, y;

    fp = fopen(filename, "w");
    if (!fp) {
        printf("Failed to open %s for writing\n", filename);
        perror("arghghghgh");
        exit(1);
    }

    for (y = 0; y < grid->height; y++) {
        for (x = 0; x < grid->width; x++) {
            if (grid_cell_alive_at(grid, x, y)) {
                fprintf(fp, "x");
            } else {
                fprintf(fp, ".");
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
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

    if (grid->running) {
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