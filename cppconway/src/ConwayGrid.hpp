#ifndef CONWAY_GRID_H
#define CONWAY_GRID_H

#include <array>
#include <string>
using namespace std;

#include "types.hpp"

#define GRID_WIDTH 80
#define GRID_HEIGHT 60
#define CELL_WIDTH 8
#define CELL_HEIGHT 8

#define MAX_LINE_LENGTH 1000

class ConwayGrid {
public:
    int16 width;
    int16 height;
    int16 cell_width;
    int16 cell_height;
    bool running;

    ConwayGrid(int16 width, int16 height);
    void init_to_blank();
    void load_from_file(const char* filename);
    void save_to_file(const char* filename);
    void print();
    void step();
    void run();
    void pause();
    void screen_coords_to_grid_coords(int16 x, int16 y, int16 *grid_x, int16 *grid_y);
    void invert_cell(int16 grid_x, int16 grid_y);

    inline bool cell_alive_at(int16 index) {
        return grid2[index] & 0x10;
    }

    inline bool cell_alive_at(int16 x, int16 y) {
        return grid2[grid_index(x, y)] & 0x10;
    }

private:
    typedef array<uint8, GRID_WIDTH * GRID_HEIGHT> GridArray;

    GridArray grid1;
    GridArray grid2;

    inline void set_cell_from_false_to_true(GridArray &grid, int16 x, int16 y, bool value) {
        int16 index = grid_index(x, y);

        grid[index] |= 0x10;

        grid[index - GRID_WIDTH - 1]++;
        grid[index - GRID_WIDTH]++;
        grid[index - GRID_WIDTH + 1]++;
        grid[index - 1]++;
        grid[index + 1]++;
        grid[index + GRID_WIDTH - 1]++;
        grid[index + GRID_WIDTH]++;
        grid[index + GRID_WIDTH + 1]++;
    }

    inline void set_cell_from_true_to_false(GridArray &grid, int16 x, int16 y, bool value) {
        int16 index = grid_index(x, y);

        grid1[index] = grid[index] & (~0x10);

        grid[index - GRID_WIDTH - 1]--;
        grid[index - GRID_WIDTH]--;
        grid[index - GRID_WIDTH + 1]--;
        grid[index - 1]--;
        grid[index + 1]--;
        grid[index + GRID_WIDTH - 1]--;
        grid[index + GRID_WIDTH]--;
        grid[index + GRID_WIDTH + 1]--;
    }

    inline int16 grid_index(int16 column, int16 row) {
        return row * width + column;
    }
};

#endif // CONWAY_GRID_H
