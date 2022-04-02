#include "dots.hpp"

void dot_move(Dot* dot) {
    dot->x += dot->vx;
    dot->y += dot->vy;

    if (dot->x > 300 || dot->x < 200) {
        dot->vx = -dot->vx;
    }

        if (dot->y > 300 || dot->y < 200) {
        dot->vy = -dot->vy;
    }
}
