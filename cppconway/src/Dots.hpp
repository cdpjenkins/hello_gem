
#ifndef DOTS_H
#define DOTS_H

#include "types.hpp"

typedef struct
{
    int16 x, y, vx, vy;
} Dot;

void dot_move(Dot* dot);

#endif /* DOTS_H */
