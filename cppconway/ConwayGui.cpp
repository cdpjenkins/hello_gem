#include <stdio.h>
#include <stdlib.h>

#include <mint/osbind.h>

#include <gem.h>

#include "types.hpp"
#include "dots.hpp"
#include "ConwayGrid.hpp"
#include "conway.rsh"

#define true  1
#define false 0

#define WHITE 0
#define BLACK 1

#define MIN_WIDTH 50
#define MIN_HEIGHT 50

#define INITIAL_WIDTH 500
#define INITIAL_HEIGHT 380

#define MS_BETWEEN_FRAMES 100

// We really shouldn't be using scancodes here. TODO figure out how to just use ASCII codes
#define SCANCODE_P 25
#define SCANCODE_Q 16
#define SCANCODE_R 19

struct win_data {
  int16 handle;
  char title[255];
};

typedef struct {
  int16 x;
  int16 y;
  int16 width;
  int16 height;
} Rectangle;

void getinfo(uint16 ap_gtype);
void event_loop(struct win_data * wd);
void draw_example (uint16 app_handle, Rectangle* working_area, char* text);
void do_redraw (struct win_data * wd, GRECT * rec1);
void draw_within_clip (struct win_data * wd, GRECT clip);
void set_clip (bool flag, GRECT rec);
void open_vwork();
void start_program();
bool is_full_window(uint16 handle);
void do_fulled(uint16 handle);
void do_sized(uint16 handle, int16 x, int16 y, int16 w, int16 h);
void draw_conway_grid(uint16 app_handle, Rectangle* working_area, ConwayGrid* grid);
static void draw_rectangle(int16 x, int16 y, int16 width, int16 height, int16 colour);
void do_menu(struct win_data* wd, int menu_item, bool* quit);
void pause();
void run();


uint16 high_word(void* ptr);
uint16 low_word(void* ptr);

Dot dot = {
  .x = 250,
  .y = 250,
  .vx = 1,
  .vy = 2
};

ConwayGrid grid;

/* GEM arrays */
int16 work_in[11],
  work_out[57],
  contrl[12],
  intin[128],
  ptsin[128],
  intout[128],
  ptsout[128];

int16 app_handle;
struct win_data wd;
OBJECT* menu_addr;


int main(int argc, char** argv) {
  short appl_id;
  const char* input_file;

  if (argc == 2) {
    input_file = argv[1];
  } else {
    input_file = "gosper.cwy";
  }

  appl_id = appl_init();

  open_vwork();
  
  grid.load_from_file(input_file);

  start_program();
  rsrc_free();
  v_clsvwk(app_handle);

  appl_exit();
  exit(0);
}

void open_vwork() {
	int i;
	int16 dum;

	app_handle = graf_handle(&dum, &dum, &dum, &dum);
	work_in[0] = 2 + Getrez();
	for (i = 1; i < 10; work_in[i++] = 1);
	work_in[10] = 2;
	v_opnvwk(work_in, &app_handle, work_out);
}

void start_program() {
  short fullx, fully, fullw, fullh;
  short rc;

  rc = rsrc_load ("conway.rsc");
  if (!rc) {
    printf("Failed to load conway.rsc\n");
    exit(1);
  }

  /* 1. install the menu bar */
  rsrc_gaddr(R_TREE, CONWAY_MENUBAR, &menu_addr);
  rc = menu_bar(menu_addr, true);

  if (!rc) {
    printf("rc %d\n", rc);
    exit(1);
  }

  graf_mouse (ARROW, 0L);
  wind_get(0, WF_WORKXYWH, &fullx, &fully, &fullw, &fullh);
  wd.handle = wind_create(NAME|CLOSER|MOVER|SIZER|FULLER|INFO, fullx, fully, fullw, fullh);
  wind_set_str(wd.handle, WF_NAME, "Conway's Game Of Life");
  wind_set_str(wd.handle, WF_INFO, "");
  wind_open(wd.handle, fullx, fully, INITIAL_WIDTH, INITIAL_HEIGHT);

  run();

  event_loop(&wd);

  wind_close (wd.handle);

  menu_bar(menu_addr, false);

  wind_delete (wd.handle);
}

void event_loop (struct win_data* wd) {
  int16 x;
  int16 y;
  int16 w;
  int16 h;
  uint16 handle;
  uint16 rc;
  int16 ev_mmgpbuff[8];
  bool quit = FALSE;

  do {
    uint16 events;
    int16 ev_mmox, ev_mmoy;
    int16 ev_mmobutton, ev_mmokstate;
    int16 ev_mkreturn, ev_mbreturn;

    events = evnt_multi(MU_TIMER | MU_MESAG | MU_BUTTON | MU_KEYBD,
                        1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        ev_mmgpbuff,
                        MS_BETWEEN_FRAMES,
                        &ev_mmox, &ev_mmoy, &ev_mmobutton, &ev_mmokstate,
                        &ev_mkreturn, &ev_mbreturn);
    if (events & MU_MESAG) {
      handle = ev_mmgpbuff[3];
      switch (ev_mmgpbuff[0]) {
        case WM_REDRAW:
          do_redraw(wd, (GRECT *)&ev_mmgpbuff[4]);
          break;
        case WM_TOPPED:
          wind_set(ev_mmgpbuff[3], WF_TOP, 0, 0, 0, 0);
          break;
        case WM_MOVED:
          x = ev_mmgpbuff[4];
          y = ev_mmgpbuff[5];
          w = ev_mmgpbuff[6];
          h = ev_mmgpbuff[7];
          wind_set(ev_mmgpbuff[3], WF_CURRXYWH, x, y, w, h);
          break;
        case WM_FULLED:
          do_fulled(handle);
          break;
        case WM_SIZED:
          x = ev_mmgpbuff[4];
          y = ev_mmgpbuff[5];
          w = ev_mmgpbuff[6];
          h = ev_mmgpbuff[7];
          do_sized(handle, x, y, w, h);
          break;
        case MN_SELECTED:
          do_menu(wd, ev_mmgpbuff[4], &quit);
        	menu_tnormal(menu_addr, ev_mmgpbuff[3], true);
		break;
        case WM_CLOSED:
          quit = TRUE;
      }
    }
    if (events & MU_TIMER) {
      GRECT r;

      if (grid.running) {
        grid.step();
        wind_get(wd->handle, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h);
        do_redraw(wd, &r);
      }
    }
    if (events & MU_KEYBD) {
      uint8 keycode, ascii;

      keycode = ev_mkreturn >> 8;
      ascii = ev_mkreturn && 0x00FF;

      if (keycode == SCANCODE_R) {
        run();
      }
      if (keycode == SCANCODE_P) {
        pause();
      }
      if (keycode == SCANCODE_Q) {
        quit = TRUE;
      }
    }
    if (events & MU_BUTTON) {
      int grid_x, grid_y;
      GRECT rec2;

      wind_get (wd->handle, WF_FIRSTXYWH,
          &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h);
      grid_screen_coords_to_grid_coords(ev_mmox - rec2.g_x, ev_mmoy - rec2.g_y, &grid_x, &grid_y);
      grid_invert_cell(&grid, grid_x, grid_y);
      do_redraw(wd, &rec2);
    }
  } while (!quit);
}

void do_redraw(struct win_data * wd, GRECT * rec1) {
  GRECT rec2;

  wind_update(BEG_UPDATE);

  wind_get (wd->handle, WF_FIRSTXYWH,
            &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h);
  while (rec2.g_w && rec2.g_h) {
    if (rc_intersect (rec1, &rec2)) {
      draw_within_clip(wd, rec2);
    }
    wind_get(wd->handle, WF_NEXTXYWH,
              &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h);
  }

  wind_update(END_UPDATE);
}

void draw_within_clip(struct win_data * wd, GRECT clip) {
  uint16 pxy[4];
  Rectangle working_area;

  graf_mouse(M_OFF, 0L);
  set_clip (true, clip);
  wind_get(wd->handle, WF_WORKXYWH, &working_area.x, &working_area.y,
           &working_area.width, &working_area.height);

  // draw_example (app_handle, &working_area, "Lalalalala I am some text");
  draw_conway_grid(app_handle, &working_area, &grid);

  set_clip (false, clip);
  graf_mouse(M_ON, 0L);
}

void draw_example (uint16 app_handle, Rectangle* working_area, char* text){
  vsf_color(app_handle, BLACK);
  v_gtext (app_handle, working_area->x + 10, working_area->y + 60, text);

  v_circle(app_handle, dot.x, dot.y, 5);
}

void draw_conway_grid(uint16 app_handle, Rectangle* working_area, ConwayGrid* grid) {
  int x, y;
  int16 x1, y1, x2, y2;
  int16 pxyarray[4];
  int16 sx;
  int16 sy;

  vsf_color(app_handle, BLACK);
  for (y = 0, sy = working_area->y;
       y < grid->height;
       y++, sy+=grid->cell_height) {
    draw_rectangle(working_area->x, sy, working_area->width, grid->cell_height, WHITE);
    for (x = 0, sx = working_area->x;
         x < grid->width;
         x++, sx += grid->cell_width) {
      sx = working_area->x + x * grid->cell_width;
      if (grid->cell_alive_at(x, y)) {
        draw_rectangle(sx,
                       sy,
                       grid->cell_width - 1,
                       grid->cell_height - 1,
                       BLACK);
      }
    }
  }
  if (sy < working_area->y + working_area->height) {
    draw_rectangle(working_area->x,
                  sy,
                  working_area->width,
                  working_area->y + working_area->height - sy,
                  WHITE);
  }
}

static void draw_rectangle(int16 x, int16 y, int16 width, int16 height, int16 colour) {
    int16 pxyarray[4];

    pxyarray[0] = x;
    pxyarray[1] = y;
    pxyarray[2] = x + width - 1;
    pxyarray[3] = y + height - 1;
    
    vsf_color(app_handle, colour);

    vr_recfl(app_handle, pxyarray);
}

void set_clip(bool flag, GRECT rec) {
  int16 pxy[4];

  pxy[0] = rec.g_x;
  pxy[1] = rec.g_y;
  pxy[2] = rec.g_x + rec.g_w - 1;
  pxy[3] = rec.g_y + rec.g_h - 1;

  vs_clip (app_handle, flag, pxy);
}

bool is_full_window(uint16 handle) {
	int16 curx, cury, curw, curh;
	int16 fullx, fully, fullw, fullh;

	wind_get (handle, WF_CURRXYWH, &curx, &cury, &curw, &curh);
	wind_get (handle, WF_FULLXYWH, &fullx, &fully, &fullw, &fullh);
	if (curx != fullx || cury != fully || curw != fullw || curh != fullh) {
		return false;
	} else {
		return true;
	}
}

void do_fulled(uint16 handle) {
	if (is_full_window(handle)) {
		int16 oldx, oldy, oldw, oldh;
		int16 fullx, fully, fullw, fullh;

		wind_get (handle, WF_PREVXYWH, &oldx, &oldy, &oldw, &oldh);
		wind_get (handle, WF_FULLXYWH, &fullx, &fully, &fullw, &fullh);
		graf_shrinkbox (oldx, oldy, oldw, oldh, fullx, fully, fullw, fullh);
		wind_set (handle, WF_CURRXYWH, oldx, oldy, oldw, oldh);
	} else {
		int16 curx, cury, curw, curh;
		int16 fullx, fully, fullw, fullh;

		wind_get (handle, WF_CURRXYWH, &curx, &cury, &curw, &curh);
		wind_get (handle, WF_FULLXYWH, &fullx, &fully, &fullw, &fullh);

		graf_growbox (curx, cury, curw, curh, fullx, fully, fullw, fullh);
		wind_set (handle, WF_CURRXYWH, fullx, fully, fullw, fullh);
	}
}

void do_sized(uint16 handle, int16 x, int16 y, int16 w, int16 h) {
	if (w < MIN_WIDTH) w = MIN_WIDTH;
	if (h < MIN_HEIGHT) h = MIN_HEIGHT;

	wind_set (handle, WF_CURRXYWH, x, y, w, h);
}

void do_menu(struct win_data* wd, int menu_item, bool* quit) {
	switch (menu_item) {
	    case CONWAY_MENUBAR_RUN:
            if (grid.running) {
                pause();
            } else {
                run();
            }
		    break;
        case CONWAY_MENUBAR_QUIT:
            *quit = TRUE;
            break;
        case CONWAY_MENUBAR_SAVE:
            const char *out_file = "out.cwy";
            grid.save_to_file(out_file);
            break;
	}
}

void pause() {
    grid.pause();
    wind_set_str(wd.handle, WF_INFO, "Paused");
    menu_icheck(menu_addr, CONWAY_MENUBAR_RUN, FALSE);
}

void run() {
    grid.run();
    wind_set_str(wd.handle, WF_INFO, "Running");
    menu_icheck(menu_addr, CONWAY_MENUBAR_RUN, TRUE);
}
