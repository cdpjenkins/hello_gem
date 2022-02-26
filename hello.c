#include <stdio.h>
#include <stdlib.h>

#include <mint/osbind.h>

#include <mt_gem.h>

#include "types.h"
#include "dots.h"
#include "conway.h"

#define true  1
#define false 0

/* I'm totally not sure about these :-) */
#define WHITE 0
#define BLACK 1

#define MIN_WIDTH 50
#define MIN_HEIGHT 50

#define WIDTH 500
#define HEIGHT 380

#define WAIT_BETWEEN_FRAMES 50

struct win_data {
  int16 handle;     /* identifying handle of the window */
  char* text;     /* text to display in window */
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
uint16 work_in[11],
  work_out[57],
  contrl[12],
  intin[128],
  ptsin[128],
  intout[128],
  ptsout[128];

uint16 app_handle;

int main(int argc, char** argv)
{
  short applId = appl_init();

  open_vwork();
  start_program();
  rsrc_free();
  v_clsvwk(app_handle);

  appl_exit();
  exit(0);
}

void open_vwork() {
	int i;
	int16 dum;

	app_handle = graf_handle (&dum, &dum, &dum, &dum);
	work_in[0] = 2 + Getrez();
	for (i = 1; i < 10; work_in[i++] = 1);
	work_in[10] = 2;
	v_opnvwk(work_in, &app_handle, work_out);
}

void start_program() {
  struct win_data wd;
  short fullx, fully, fullw, fullh;
  short rc;

  graf_mouse (ARROW, 0L);
  grid_import_from_file("gosper.txt", &grid);
  wind_get(0, WF_WORKXYWH, &fullx, &fully, &fullw, &fullh);
  wd.handle = wind_create(NAME|CLOSER|MOVER|SIZER|FULLER, fullx, fully, fullw, fullh);
  rc = wind_set_str(wd.handle, WF_NAME, "Conway's Game Of Life", 0, 0);
  wind_open(wd.handle, fullx, fully, WIDTH, HEIGHT);

  event_loop (&wd);

  wind_close (wd.handle);
  wind_delete (wd.handle);
}

void event_loop (struct win_data * wd) {
  int16 x;
  int16 y;
  int16 w;
  int16 h;
  uint16 handle;
  uint16 rc;
  uint16 ev_mmgpbuff[8];

  do {
    uint16 events;
    uint16 ev_mmox, ev_mmoy;
    uint16 ev_mmobutton, ev_mmokstate;
    uint16 ev_mkreturn, ev_mbreturn;

    events = evnt_multi ( MU_TIMER | MU_WHEEL | MU_MESAG,
                          1, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          ev_mmgpbuff,
                          WAIT_BETWEEN_FRAMES, 0,
                          &ev_mmox, &ev_mmoy, &ev_mmobutton, &ev_mmokstate,
                          &ev_mkreturn, &ev_mbreturn );
    if (events & MU_MESAG) {
      handle = ev_mmgpbuff[3];
      switch (ev_mmgpbuff[0]) {
        case WM_REDRAW:
          do_redraw(wd, (GRECT *)&ev_mmgpbuff[4]);
          break;
        case WM_TOPPED:
          wind_set(ev_mmgpbuff[3], WF_TOP, 0, 0);
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
      }
    }
    if (events & MU_TIMER) {
      GRECT r;

      grid_step(&grid);
      // grid_print(&grid);

      wind_get(wd->handle, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h);
      do_redraw(wd, &r);

      dot_move(&dot);
    }
  } while (ev_mmgpbuff[0] != WM_CLOSED);
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

  graf_mouse (M_OFF, 0L);
  set_clip (true, clip);
  wind_get (wd->handle, WF_WORKXYWH, &working_area.x, &working_area.y,
            &working_area.width, &working_area.height);

  vsf_color(app_handle, WHITE);
  pxy[0] = working_area.x;
  pxy[1] = working_area.y;
  pxy[2] = working_area.x + working_area.width - 1;
  pxy[3] = working_area.y + working_area.height - 1;
  vr_recfl(app_handle, pxy);

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

  // todo move width and height into the freaking grid
  vsf_color(app_handle, BLACK);
  for (x = 0; x < grid->width; x++) {
    for (y = 0; y < grid->height; y++) {
      if (grid_cell_alive_at(grid, x, y)) {
        pxyarray[0] = working_area->x + x * grid->cell_width;
        pxyarray[1] = working_area->y + y * grid->cell_height;
        pxyarray[2] = working_area->x + (x+1) * grid->cell_width - 2;
        pxyarray[3] = working_area->y + (y+1) * grid->cell_height - 2;
        vr_recfl(app_handle, pxyarray);
      }     
    }
  }
}

void set_clip(bool flag, GRECT rec) {
  uint16 pxy[4];

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