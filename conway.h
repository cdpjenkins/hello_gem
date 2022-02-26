#ifndef CONWAY_H
#define CONWAY_H

#include "types.h"

#define GRID_WIDTH 60
#define GRID_HEIGHT 30
#define CELL_WIDTH 16
#define CELL_HEIGHT 16

#define MAX_LINE_LENGTH 1000

typedef struct {
    bool grid1[GRID_WIDTH * GRID_HEIGHT];
    bool grid2[GRID_WIDTH * GRID_HEIGHT];
    bool* current_grid;
    bool* next_grid;
    int width;
    int height;
    int cell_width;
    int cell_height;
} ConwayGrid;

void grid_init_to_blank();
void grid_import_from_file(char* filename, ConwayGrid* grid);
void grid_print(ConwayGrid* grid);
void grid_step(ConwayGrid* grid);
bool grid_cell_alive_at(ConwayGrid* grid, int x, int y);

#endif // CONWAY_H
