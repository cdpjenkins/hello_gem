#include "App.hpp"

#ifndef VsetScreen
#define VsetScreen Vsetscreen
#endif

App::App() :
        mandie{make_unique<MandelbrotRenderer>(640, 480, config)} {
    natfeats_init();
    screen_init();
}

App::~App() {
    screen_restore();
}

int App::main_loop() {
    bool quit = false;
    while (!quit) {
        draw_timed(screen);

        uint32_t key = Cconin();
        nf_debugprintf("key: %08X\n", key);
        uint16_t ascii = (key & 0x000000FF);
        uint16_t scancode = key >> 16;

        switch (ascii) {
            case 'q':
            case 'Q':
                quit = true;
                break;
            case 'z':
            case 'Z':
                mandie->zoom_in();
                break;
            case 'x':
            case 'X':
                mandie->zoom_out();
                break;
            default:
                // meh don't care about other keys
                break;
        }

        switch (scancode) {
            case UP:
                mandie->centre = mandie->centre + Complex(0, -1 / mandie->zoom_size);
                break;
            case DOWN:
                mandie->centre = mandie->centre + Complex(0, 1 / mandie->zoom_size);
                break;
            case LEFT:
                mandie->centre = mandie->centre + Complex(-1 / mandie->zoom_size, 0);
                break;
            case RIGHT:
                mandie->centre = mandie->centre + Complex(1 / mandie->zoom_size, 0);
                break;
            default:
                // meh don't care about other keys
                break;
        }
    }

    return 0;
}

void App::natfeats_init() {
    nf_ops *nf_context = nf_init();
    if (!nf_context) {
        printf("Unable to initialise natfeats\n");
    } else {
        printf("Natfeats is initialised: %08X\n", nf_context);
        nf_debugprintf("Natfeats is initialised: %08X\n", nf_context);
    }
}

void App::screen_init() {
    Cursconf(0, 0);

    screen.clear();

    saved_rez = VsetMode(-1);
    saved_logbase = Logbase();
    saved_physbase = Physbase();

    VsetScreen(screen.get_frame_buffer(), screen.get_frame_buffer(), REZ_FROM_MODE, PLANES_16 | WIDTH_640 | VGA | NTSC);
}

void App::screen_restore() const {
    VsetScreen(saved_logbase, saved_physbase, REZ_FROM_MODE, saved_rez);
}

void App::draw_timed(Screen &screen) {
    uint32_t time_before = read_system_timer();
    mandie->render_to_buffer(reinterpret_cast<Colour *>(screen.get_frame_buffer()));
    uint32_t time_after_draw = read_system_timer();
    nf_debugprintf("draw_timed: %dms\n", (time_after_draw - time_before) * 5);
}

uint32_t App::read_system_timer_in_supervisor_mode() {
    return *(uint32_t *) 0x0004ba;
}

uint32_t App::read_system_timer() {
    return Supexec(read_system_timer_in_supervisor_mode);
}
