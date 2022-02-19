#include <stdio.h>
#include <stdlib.h>

#include <mint/osbind.h>

#include <mt_gem.h>

typedef short int16_t;
struct win_data {
  int handle;     /* identifying handle of the window */

  char * text;    /* text to display in window */
};


void getinfo(short ap_gtype);
void event_loop(struct win_data * wd);
void draw_example (int app_handle, struct win_data * wd);
void open_vwork (void);
void start_program (void);

/* GEM arrays */
short work_in[11],
  work_out[57],
  contrl[12],
  intin[128],
  ptsin[128],
  intout[128],
  ptsout[128];

short app_handle; /* application graphics handle */




int main(int argc, char** argv)
{
  int rc;

  printf("Hello World!\n");


  short applId = appl_init();

  printf("%x\n", applId);

  getinfo(AES_LARGEFONT);
  getinfo(AES_SYSTEM);
  getinfo(AES_LANGUAGE);
  getinfo(AES_MENU);

  open_vwork();
  start_program ();
  rsrc_free ();
  v_clsvwk (app_handle);

  rc = appl_exit();
  printf("%d\n", rc);

  exit(1);
}

void getinfo(short ap_gtype) {
  short ap_gout1;
  short ap_gout2;
  short ap_goat3;
  short ap_goat4;
  int rc;

  rc = appl_getinfo(ap_gtype, &ap_gout1, &ap_gout2, &ap_goat3, &ap_goat4);
  printf("%d: %x %d, %d, %d, %d\n", rc, ap_gtype, ap_gout1, ap_gout2, ap_goat3, ap_goat4);
}

void open_vwork (void) {
	int i;
	int dum;

	app_handle = graf_handle (&dum, &dum, &dum, &dum);	// (1)
	work_in[0] = 2 + Getrez ();				// (2)
	for (i = 1; i < 10; work_in[i++] = 1);
	work_in[10] = 2;
	v_opnvwk (work_in, &app_handle, work_out);		// (3)

  printf("done open_vwork\n");
}

void start_program (void) {
  struct win_data wd;
  short fullx, fully, fullw, fullh;
  short rc;

  graf_mouse (ARROW, 0L); /* ensure mouse is an arrow */

  printf("done graf_mouse\n");

  /* 1. set up and open our window */
  wind_get (0, WF_WORKXYWH, &fullx, &fully, &fullw, &fullh);

  printf("WF_WORKXYWH: %d %d %d %d\n", (int)fullx, (int)fully, (int)fullw, (int)fullh);

  printf("short %d\n", sizeof(short));
  printf("int %d\n", sizeof(int));
  printf("long %d\n", sizeof(long));

  wd.handle = wind_create (NAME|CLOSER|MOVER|SIZER, fullx, fully, fullw, fullh);

  printf("Window created\n");

  {
    char* title;
    short hw;
    short lw;

    title = "Example: Version 1";
    hw = (title && 0xFFFF0000) >> 16;
    lw = (title && 0x0000FFFF);

    rc = wind_set(wd.handle, WF_NAME, hw, lw, 0, 0);
    printf("wnd_set done: %d\n", rc);
  }
  wind_open(wd.handle, fullx, fully, 300, 200);
  printf("Window opened\n");

  /* create any application-specific data: the string to show, in this case */
  wd.text = "Hello";

  /* display content in window */
  draw_example (app_handle, &wd);

  /* 2. process events for our window */
  event_loop (&wd);

  /* 3. close and remove our window */
  wind_close (wd.handle);
  wind_delete (wd.handle);
}

void event_loop (struct win_data * wd) {
  short msg_buf[8];

  do {
    short rc = evnt_mesag (msg_buf);

    printf("%d\n", rc);

  } while (msg_buf[0] != WM_CLOSED);

}

void draw_example (int app_handle, struct win_data * wd) {

  v_gtext (app_handle, 10, 60, "Cheese-ston!");

}

