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

int grid_index(int column, int row) {
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
            c = *ptr;
            printf("%c\n", c);

            if (c == 'x') {
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
