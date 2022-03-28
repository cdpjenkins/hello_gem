#include "ConwayGrid.hpp"

int main(int argc, char** argv) {
    ConwayGrid grid;
    int i;

    grid.load_from_file("glider.cwy");

    grid.print();
    grid_run(&grid);

    for (i = 0; i < 30; i++) {
        grid.step();
        grid.print();
    }
}
