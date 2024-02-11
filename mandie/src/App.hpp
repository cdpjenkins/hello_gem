//
// Created by Chris Jenkins on 11/02/2024.
//

#ifndef MANDIE_APP_HPP
#define MANDIE_APP_HPP

#include <mint/sysbind.h>
#include <mint/arch/nf_ops.h>

constexpr int16_t PLANES_1 = 0x00;
constexpr int16_t PLANES_2 = 0x01;
constexpr int16_t PLANES_4 = 0x02;
constexpr int16_t PLANES_8 = 0x03;
constexpr int16_t PLANES_16 = 0x04;
constexpr int16_t WIDTH_640 = 0x08;
constexpr int16_t VGA = 0x10;
constexpr int16_t NTSC = 0x00;

constexpr int16_t REZ_FROM_MODE = 3;

struct App {

    App() {
        natfeats_init();

        Cursconf(0, 0);

        screen.clear();

        saved_rez = VsetMode(-1);
        saved_logbase = Logbase();
        saved_physbase = Physbase();

        VsetScreen(screen.get_frame_buffer(), screen.get_frame_buffer(), REZ_FROM_MODE, PLANES_16 | WIDTH_640 | VGA | NTSC);
    }

    ~App() {
        VsetScreen(saved_logbase, saved_physbase, REZ_FROM_MODE, saved_rez);
    }

    void natfeats_init() {
        nf_ops *nf_context = nf_init();
        if (!nf_context) {
            printf("Unable to initialise natfeats\n");
        } else {
            printf("Natfeats is initialised: %08X\n", nf_context);
            nf_debugprintf("Natfeats is initialised: %08X\n", nf_context);
        }
    }

    int16_t saved_rez;
    void* saved_logbase;
    void* saved_physbase;

    Screen screen;

    Config config;
    std::unique_ptr<MandelbrotRenderer> mandie = make_unique<MandelbrotRenderer>(640, 480, config);

};


#endif //MANDIE_APP_HPP
