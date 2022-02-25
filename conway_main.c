#include "conway.h"

int main(int argc, char** argv) {
    ConwayGrid grid;

    grid_import_from_file("glider.txt", &grid);

    grid_print(&grid);
}
