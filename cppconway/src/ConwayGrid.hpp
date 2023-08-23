#ifndef CONWAY_GRID_H
#define CONWAY_GRID_H

#include <array>
#include <string>
using namespace std;

#include "types.hpp"

#define GRID_WIDTH 160
#define GRID_HEIGHT 120
#define MAX_LINE_LENGTH 1000

class ConwayGrid {
public:
    int16 width;
    int16 height;
    bool running;

    ConwayGrid(int16 width, int16 height);
    void init_to_blank();
    void load_from_file(const char* filename);
    void save_to_file(const char* filename);
    void print();
    void step();
    void run();
    void pause();
    void screen_coords_to_grid_coords(int16 x, int16 y, int16 *grid_x, int16 *grid_y, int16 cell_size) const;
    void invert_cell(int16 grid_x, int16 grid_y);

    inline bool cell_alive_at(int16 x, int16 y) {
        return grid1[grid_index(x, y)] & 0x10;
    }

    inline bool cell_is_buffer(int x, int y) {
        return grid1[grid_index(x, y)] & 0x20;
    }

private:
    typedef array<uint8, GRID_WIDTH * GRID_HEIGHT> GridArray;

    GridArray grid1;
    GridArray grid2;

    static inline void transition_cell_from_dead_to_alive(GridArray &grid, int16 index) {
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

    inline void transition_cell_from_dead_to_alive(GridArray &grid, int16 x, int16 y) {
        transition_cell_from_dead_to_alive(grid, grid_index(x, y));
    }

    static inline void transition_cell_from_alive_to_dead(GridArray &grid, int16 index) {
        grid[index] = grid[index] & (~0x10);

        grid[index - GRID_WIDTH - 1]--;
        grid[index - GRID_WIDTH]--;
        grid[index - GRID_WIDTH + 1]--;
        grid[index - 1]--;
        grid[index + 1]--;
        grid[index + GRID_WIDTH - 1]--;
        grid[index + GRID_WIDTH]--;
        grid[index + GRID_WIDTH + 1]--;
    }

    inline void transition_cell_from_alive_to_dead(GridArray &grid, int16 x, int16 y) {
        transition_cell_from_alive_to_dead(grid, grid_index(x, y));
    }


    inline int16 grid_index(int16 column, int16 row) const {
        return row * width + column;
    }
};

#endif // CONWAY_GRID_H
