#include <stdio.h>
#include <stdlib.h>

#include <mint/osbind.h>

#include <mt_gem.h>

typedef unsigned short uint16;
typedef uint16         bool;

#define true  1
#define false 0

/* I'm totally not sure about these :-) */
#define WHITE 0
#define BLACK 1

struct win_data {
  int handle;     /* identifying handle of the window */
  char * text;    /* text to display in window */
};

void getinfo(uint16 ap_gtype);
void event_loop(struct win_data * wd);
void draw_example (uint16 app_handle, char* text);
void do_redraw (struct win_data * wd, GRECT * rec1);
void draw_interior (struct win_data * wd, GRECT clip);
void set_clip (bool flag, GRECT rec);
void open_vwork (void);
void start_program (void);

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
	int dum;

	app_handle = graf_handle (&dum, &dum, &dum, &dum);
	work_in[0] = 2 + Getrez();
	for (i = 1; i < 10; work_in[i++] = 1);
	work_in[10] = 2;
	v_opnvwk (work_in, &app_handle, work_out);

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

  wd.handle = wind_create(NAME|CLOSER|MOVER|SIZER, fullx, fully, fullw, fullh);

  rc = wind_set_str(wd.handle, WF_NAME, "Hello GEM!", 0, 0);
  printf("wnd_set done: %d\n", rc);

  wind_open(wd.handle, fullx, fully, 300, 200);
  printf("Window opened\n");

  event_loop (&wd);

  wind_close (wd.handle);
  wind_delete (wd.handle);
}

void event_loop (struct win_data * wd) {
  short msg_buf[8];

  do {
    evnt_mesag(msg_buf);

    switch (msg_buf[0]) {                                 	// (1)
		case WM_REDRAW:   				// (2)
			do_redraw (wd, (GRECT *)&msg_buf[4]);	// (3)
			break;
    }
  } while (msg_buf[0] != WM_CLOSED);
}

void do_redraw (struct win_data * wd, GRECT * rec1) {
	GRECT rec2;

	wind_update (BEG_UPDATE);			                     // (1)

	wind_get (wd->handle, WF_FIRSTXYWH,
            &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h);   // (2)
	while (rec2.g_w && rec2.g_h) {			               // (3)
		if (rc_intersect (rec1, &rec2)) {	               // (4)
			draw_interior (wd, rec2);	                     // (5)
		}
		wind_get (wd->handle, WF_NEXTXYWH,
              &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h); // (6)
	}

	wind_update (END_UPDATE);			                     // (7)
}

/* Draw interior of window, within given clipping rectangle */
void draw_interior(struct win_data * wd, GRECT clip) {
	uint16 pxy[4];
	uint16 wrkx, wrky, wrkw, wrkh; /* some variables describing current working area */

	/* set up drawing, by hiding mouse and setting clipping on */
	graf_mouse (M_OFF, 0L);
	set_clip (true, clip);
	wind_get (wd->handle, WF_WORKXYWH, &wrkx, &wrky, &wrkw, &wrkh);

	/* clears the display */
	vsf_color (app_handle, WHITE);
	pxy[0] = wrkx;
	pxy[1] = wrky;
	pxy[2] = wrkx + wrkw - 1;
	pxy[3] = wrky + wrkh - 1;
	vr_recfl (app_handle, pxy);

	/* draws our specific code */
	draw_example (app_handle, "Lalalalala I am some text");

	/* tidies up */
	set_clip (false, clip);
	graf_mouse (M_ON, 0L);
}

void draw_example (uint16 app_handle, char* text) {
  vsf_color(app_handle, BLACK);
  v_gtext (app_handle, 10, 60, text);

  v_circle(app_handle, 40, 100, 10);
}

void set_clip (bool flag, GRECT rec) {
  uint16 pxy[4];

  pxy[0] = rec.g_x;
  pxy[1] = rec.g_y;
  pxy[2] = rec.g_x + rec.g_w - 1;
  pxy[3] = rec.g_y + rec.g_h - 1;

  vs_clip (app_handle, flag, pxy);
}
