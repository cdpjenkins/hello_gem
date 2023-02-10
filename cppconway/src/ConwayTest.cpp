#include "ConwayGrid.hpp"

int main(int argc, char** argv) {
    ConwayGrid grid(GRID_WIDTH, GRID_HEIGHT);
    int i;

    grid.load_from_file("glider.cwy");

    grid.print();
    grid.run();

    for (i = 0; i < 30; i++) {
        grid.step();
        grid.print();
    }
}
