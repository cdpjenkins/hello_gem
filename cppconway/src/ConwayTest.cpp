#include "ConwayGrid.hpp"

int main(int argc, char** argv) {
    ConwayGrid<40, 30> grid;
    int i;

    grid.load_from_file("glider.cwy");

    grid.print();
    grid.run();

    for (i = 0; i < 30; i++) {
        grid.step();
        grid.print();
    }
}
