#ifndef CONWAY_GRID_H
#define CONWAY_GRID_H

#include "types.hpp"

#define GRID_WIDTH 80
#define GRID_HEIGHT 60
#define CELL_WIDTH 8
#define CELL_HEIGHT 8

#define MAX_LINE_LENGTH 1000

class ConwayGrid {
public:
    int16 width = GRID_WIDTH;
    int16 height = GRID_HEIGHT;
    int16 cell_width;
    int16 cell_height;
    bool running;

    ConwayGrid();
    void init_to_blank();
    void load_from_file(const char* filename);
    void save_to_file(const char* filename);
    void print();
    void step();
    void run();
    void pause();
    void screen_coords_to_grid_coords(int16 x, int16 y, int16 *grid_x, int16 *grid_y);
    void invert_cell(int16 grid_x, int16 grid_y);

    inline bool cell_alive_at(int16 x, int16 y) {
        return current_grid[grid_index(x, y)];
    }

private:
    bool grid1[GRID_WIDTH * GRID_HEIGHT];
    bool grid2[GRID_WIDTH * GRID_HEIGHT];
    bool* current_grid;
    bool* next_grid;

    int16 num_living_neighbours(int16 x, int16 y);

    inline void set_cell(bool* grid, int16 x, int16 y, bool value) {
        current_grid[grid_index(x, y)] = value;
    }

    inline int16 grid_index(int16 column, int16 row) {
        return row * width + column;
    }
};

#endif // CONWAY_GRID_H
