#include <stdio.h>
#include <stdlib.h>

#include <mint/osbind.h>

#include <mt_gem.h>

typedef unsigned short uint16;
typedef short          int16;
typedef uint16         bool;

#define true  1
#define false 0

/* I'm totally not sure about these :-) */
#define WHITE 0
#define BLACK 1

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

uint16 high_word(void* ptr);
uint16 low_word(void* ptr);

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

  printf("%x\n", applId);

  open_vwork();
  start_program();
  rsrc_free();
  v_clsvwk (app_handle);

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

  printf("done open_vwork\n");
}

void start_program() {
  struct win_data wd;
  short fullx, fully, fullw, fullh;
  short rc;

  graf_mouse (ARROW, 0L);

  printf("done graf_mouse\n");

  wind_get(0, WF_WORKXYWH, &fullx, &fully, &fullw, &fullh);

  printf("WF_WORKXYWH: %d %d %d %d\n", fullx, fully, fullw, fullh);

  wd.handle = wind_create(NAME|CLOSER|MOVER|SIZER|FULLER, fullx, fully, fullw, fullh);

  rc = wind_set_str(wd.handle, WF_NAME, "Hello GEM!", 0, 0);
  printf("wnd_set done: %d\n", rc);

  wind_open(wd.handle, fullx, fully, 300, 200);
  printf("Window opened\n");

  event_loop (&wd);

  wind_close (wd.handle);
  wind_delete (wd.handle);
}

void event_loop (struct win_data * wd) {
  uint16 msg_buf[8];
  int16 x;
  int16 y;
  int16 w;
  int16 h;
  uint16 handle;
  uint16 rc;

  do {
    rc = evnt_mesag(msg_buf);
    handle = msg_buf[3];

    switch (msg_buf[0]) {
      case WM_REDRAW:
        printf("WM_REDRAW\n");
        do_redraw (wd, (GRECT *)&msg_buf[4]);	// (3)
        break;
      case WM_TOPPED:
        printf("WM_TOPPED\n");
        wind_set(msg_buf[3], WF_TOP, 0, 0);
        break;
      case WM_MOVED:
        x = msg_buf[4];
        y = msg_buf[5];
        w = msg_buf[6];
        h = msg_buf[7];
        printf("WM_MOVED %d %d %d %d\n", x, y, w, h);
        printf("%d\n", handle);
        wind_set(msg_buf[3], WF_CURRXYWH, x, y, w, h);
        break;
      case WM_FULLED:
        printf("WM_FULLED\n");
        do_fulled(handle);
        break;
    }
  } while (msg_buf[0] != WM_CLOSED);
}

void do_redraw (struct win_data * wd, GRECT * rec1) {
	GRECT rec2;

	wind_update (BEG_UPDATE);

	wind_get (wd->handle, WF_FIRSTXYWH,
            &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h);
	while (rec2.g_w && rec2.g_h) {
		if (rc_intersect (rec1, &rec2)) {
			draw_within_clip (wd, rec2);
		}
		wind_get (wd->handle, WF_NEXTXYWH,
              &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h);
	}

	wind_update (END_UPDATE);
}

void draw_within_clip(struct win_data * wd, GRECT clip) {
	uint16 pxy[4];
  Rectangle working_area;

	graf_mouse (M_OFF, 0L);
	set_clip (true, clip);
	wind_get (wd->handle, WF_WORKXYWH, &working_area.x, &working_area.y,
            &working_area.width, &working_area.height);

	vsf_color (app_handle, WHITE);
	pxy[0] = working_area.x;
	pxy[1] = working_area.y;
	pxy[2] = working_area.x + working_area.width - 1;
	pxy[3] = working_area.y + working_area.height - 1;
	vr_recfl(app_handle, pxy);

	draw_example (app_handle, &working_area, "Lalalalala I am some text");

	set_clip (false, clip);
	graf_mouse (M_ON, 0L);
}

void draw_example (uint16 app_handle, Rectangle* working_area, char* text){
  vsf_color(app_handle, BLACK);
  v_gtext (app_handle, working_area->x + 10, working_area->y + 60, text);

  v_circle(app_handle, working_area->x + 40, working_area->y + 100, 10);
}

void set_clip (bool flag, GRECT rec) {
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

    printf("Shrink to restore!\n");

		wind_get (handle, WF_PREVXYWH, &oldx, &oldy, &oldw, &oldh);
		wind_get (handle, WF_FULLXYWH, &fullx, &fully, &fullw, &fullh);
		graf_shrinkbox (oldx, oldy, oldw, oldh, fullx, fully, fullw, fullh);
		wind_set (handle, WF_CURRXYWH, oldx, oldy, oldw, oldh);

	} else {
		int16 curx, cury, curw, curh;
		int16 fullx, fully, fullw, fullh;

    printf("Expand to full!\n");

		wind_get (handle, WF_CURRXYWH, &curx, &cury, &curw, &curh);
		wind_get (handle, WF_FULLXYWH, &fullx, &fully, &fullw, &fullh);

    printf("Expand to full! %d %d %d %d\n", fullx, fully, fullw, fullh);

		graf_growbox (curx, cury, curw, curh, fullx, fully, fullw, fullh);
		wind_set (handle, WF_CURRXYWH, fullx, fully, fullw, fullh);
	}
}
