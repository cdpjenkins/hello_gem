#ifndef ST_SCREEN_HPP

class STScreen {
public:
    void draw_line(int16 colour, int16 x1, int16 y1, int16 x2, int16 y2);
    void fill_rect(int16 colour, int16 x1, int16 y1, int16 x2, int16 y2);
};

#endif // ST_SCREEN_HPP
