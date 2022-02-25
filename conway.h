#ifndef CONWAY_H
#define CONWAY_H

#include "types.h"

#define GRID_WIDTH 100
#define GRID_HEIGHT 100

#define MAX_LINE_LENGTH 1000

typedef struct {
    bool grid1[GRID_WIDTH][GRID_HEIGHT];
    bool grid2[GRID_WIDTH][GRID_HEIGHT];
    bool** current_grid;
    bool** next_grid;
} ConwayGrid;

void grid_init_to_blank();
void grid_import_from_file(char* filename, ConwayGrid* grid);
void grid_print(ConwayGrid* grid);

#endif // CONWAY_H
