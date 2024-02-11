#ifndef MANDIE_APP_HPP
#define MANDIE_APP_HPP

#include <cstdint>

#include <mint/sysbind.h>
#include <mint/arch/nf_ops.h>

#include "MandelbrotRenderer.hpp"
#include "Screen.hpp"

constexpr int16_t PLANES_1 = 0x00;
constexpr int16_t PLANES_2 = 0x01;
constexpr int16_t PLANES_4 = 0x02;
constexpr int16_t PLANES_8 = 0x03;
constexpr int16_t PLANES_16 = 0x04;
constexpr int16_t WIDTH_640 = 0x08;
constexpr int16_t VGA = 0x10;
constexpr int16_t NTSC = 0x00;

constexpr int16_t REZ_FROM_MODE = 3;

enum ScanCode {
    UP = 72,
    DOWN = 80,
    LEFT = 75,
    RIGHT = 77,
    KEY_Q = 16
};

class App {
public:
    App();
    ~App();

    int main_loop();

private:
    static void natfeats_init();
    void screen_init();
    void draw_timed(Screen &screen);
    static uint32_t read_system_timer_in_supervisor_mode();
    static uint32_t read_system_timer();

    int16_t saved_rez;
    void* saved_logbase;
    void* saved_physbase;

    Screen screen;

    Config config;
    std::unique_ptr<MandelbrotRenderer> mandie;

    void screen_restore() const;
};

#endif //MANDIE_APP_HPP
