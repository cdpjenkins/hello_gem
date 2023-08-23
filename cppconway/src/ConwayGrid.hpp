#ifndef CONWAY_GRID_H
#define CONWAY_GRID_H

#include <array>
#include <string>
using namespace std;

#include <cstring>

#include "types.hpp"

#define MAX_LINE_LENGTH 1000

template<int GRID_WIDTH, int GRID_HEIGHT>
class ConwayGrid {
public:
    static constexpr int16 width = GRID_WIDTH;
    static constexpr int16 height = GRID_HEIGHT;
    bool running;

    ConwayGrid() :
            running(false),
            grid1(),
            grid2() {
    }

    void init_to_blank() {
        running = false;

        grid1.fill(0x00);
        grid2.fill(0x00);

        for (int16 x = 0; x < width; x++) {
            grid1[grid_index(x, 0)] = 0x20;
            grid1[grid_index(x, height - 1)] = 0x20;
        }

        for (int16 y = 0; y < height; y++) {
            grid1[grid_index(0, y)] = 0x20;
            grid1[grid_index(width - 1 , y)] = 0x20;
        }
    }

    void load_from_file(const char* filename) {
        FILE *fp;
        char row_string[MAX_LINE_LENGTH];

        init_to_blank();

        fp = fopen(filename, "r");
        if (!fp) {
            printf("Failed to open %s\n", filename);
            exit(1);
        }

        for (int16 y = 0; fgets(row_string, MAX_LINE_LENGTH, fp) != nullptr && y < height; y++) {
            int16 x;
            char* ptr;

            for (ptr = row_string, x = 0; ptr != nullptr && x < width; ptr++, x++) {
                if (*ptr == 'x') {
                    transition_cell_from_dead_to_alive(grid1, x, y);
                }
            }
        }

        fclose(fp);
    }

    void save_to_file(const char* filename) {
        FILE *fp;
        int16 x, y;

        fp = fopen(filename, "w");
        if (!fp) {
            printf("Failed to open %s for writing\n", filename);
            perror("arghghghgh");
            exit(1);
        }

        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                if (cell_alive_at(x, y)) {
                    fprintf(fp, "x");
                } else {
                    fprintf(fp, ".");
                }
            }
            fprintf(fp, "\n");
        }

        fclose(fp);
    }

    void print() {
        for (int16 y = 0; y < height; y++) {
            for (int16 x = 0; x < width; x++) {
                if (cell_alive_at(x, y)) {
                    printf("x");
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
        printf("\n");
    }

    void step() {
        if (running) {
            memcpy(&grid2, &grid1, sizeof(grid1));

            int16 index = 0;
            for (int16 y = 0; y < height; y++) {
                for (int16 x = 0; x < width; x++, index++) {
                    uint8 &current_value = grid2[index];
                    if (current_value != 0) {
                        if (current_value == 0x03) {
                            transition_cell_from_dead_to_alive(grid1, index);
                        } else if ((current_value & 0x10) && (current_value != 0x12) && (current_value != 0x13)) {
                            transition_cell_from_alive_to_dead(grid1, index);
                        }
                    }
                }
            }
        }
    }

    void run() {
        running = true;
    }

    void pause() {
        running = false;
    }

    void screen_coords_to_grid_coords(int16 x, int16 y, int16 *grid_x, int16 *grid_y, int16 cell_size) const {
        *grid_x = x / cell_size;
        *grid_y = y / cell_size;
    }

    void invert_cell(int16 grid_x, int16 grid_y) {
        if (!running) {
            if (cell_alive_at(grid_x, grid_y)) {
                transition_cell_from_alive_to_dead(grid1, grid_x, grid_y);
            } else {
                transition_cell_from_dead_to_alive(grid1, grid_x, grid_y);
            }
        }
    }

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
