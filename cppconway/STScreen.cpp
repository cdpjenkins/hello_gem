#include <mint/linea.h>

#include "types.hpp"
#include "STScreen.hpp"

void STScreen::draw_line(int16 colour, int16 x1, int16 y1, int16 x2, int16 y2) {
    COLBIT0 = colour;
    LSTLIN = 0;
    LNMASK = 0XFFFF;
    WMODE = 0;
    X1 = x1;
    Y1 = y1;
    X2 = x2;
    Y2 = y2;    linea3();
}

void STScreen::fill_rect(int16 colour, int16 x1, int16 y1, int16 x2, int16 y2) {
    COLBIT0 = colour;
    LSTLIN = 0;
    LNMASK = 0XFFFF;
    WMODE = 0;
    X1 = x1;
    Y1 = y1;
    X2 = x2;
    Y2 = y2;
    linea5();
}