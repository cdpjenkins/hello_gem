#ifndef CONWAY_GRID_H
#define CONWAY_GRID_H

#include "types.hpp"

#define GRID_WIDTH 60
#define GRID_HEIGHT 30
#define CELL_WIDTH 16
#define CELL_HEIGHT 16

#define MAX_LINE_LENGTH 1000

class ConwayGrid {
public:
    ConwayGrid();
    void init_to_blank();
    void load_from_file(const char* filename);
    void save_to_file(const char* filename);

    bool grid1[GRID_WIDTH * GRID_HEIGHT];
    bool grid2[GRID_WIDTH * GRID_HEIGHT];
    bool* current_grid;
    bool* next_grid;
    int width;
    int height;
    int cell_width;
    int cell_height;
    bool running;
};

static inline int grid_index(ConwayGrid* grid, int column, int row) {
    return row * grid->width + column;
}

void grid_print(ConwayGrid* grid);
void grid_step(ConwayGrid* grid);
bool grid_cell_alive_at(ConwayGrid* grid, int x, int y);
void grid_run(ConwayGrid* grid);
void grid_pause(ConwayGrid* grid);
void grid_screen_coords_to_grid_coords(int x, int y, int* grid_x, int* grid_y);
void grid_invert_cell(ConwayGrid* grid, int grid_x, int grid_y);

#endif // CONWAY_GRID_H
