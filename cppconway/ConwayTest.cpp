#include "ConwayGrid.hpp"

int main(int argc, char** argv) {
    ConwayGrid grid;
    int i;

    grid.load_from_file("glider.cwy");

    grid_print(&grid);
    grid_run(&grid);

    for (i = 0; i < 30; i++) {
        grid_step(&grid);
        grid_print(&grid);
    }
}
