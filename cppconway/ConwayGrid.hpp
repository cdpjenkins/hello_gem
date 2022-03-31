#ifndef CONWAY_GRID_H
#define CONWAY_GRID_H

#include "types.hpp"

#define GRID_WIDTH 40
#define GRID_HEIGHT 30
#define CELL_WIDTH 16
#define CELL_HEIGHT 16

#define MAX_LINE_LENGTH 1000

class ConwayGrid {
public:
    int width;
    int height;
    int cell_width;
    int cell_height;
    bool running;

    ConwayGrid();
    void init_to_blank();
    void load_from_file(const char* filename);
    void save_to_file(const char* filename);
    void print();
    void step();
    void run();
    void pause();
    void screen_coords_to_grid_coords(int x, int y, int* grid_x, int* grid_y);
    void invert_cell(int grid_x, int grid_y);

    inline bool cell_alive_at(int16 x, int16 y) {
        if (x < 0 || x >= width
        || y < 0 || y >= height) {
            return FALSE;
        } else {
            int i = 0;
            while (x >= 20) {
                x -= 20;
                i++;
            }

            uint32 block = current_grid[i + y*2];
            return (block >> x) & 1;
        }
    }

    inline void set_cell(uint32 *grid, int16 x, int16 y, bool value) {
        int i = 0;
        while (x >= 20) {
            x -= 20;
            i++;
        }

        if (value) {
            grid[i + y*2] |= 1 << x;
        } else {
            grid[i + y*2] &= ~(1 << x);
        }
    }

private:
    uint32 grid1[GRID_WIDTH * GRID_HEIGHT / 20];
    uint32 grid2[GRID_WIDTH * GRID_HEIGHT / 20];
    uint32 *current_grid;
    uint32 *next_grid;

    int num_living_neighbours(int x, int y);
};

#endif // CONWAY_GRID_H
