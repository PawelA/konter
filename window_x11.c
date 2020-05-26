#include <stdio.h>
#include <stdlib.h>

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "window.h"

// TODO: query the actual screen size
#define WIDTH 1920
#define HEIGHT 1080
	
static GLint glattrs[] = { 
	GLX_RGBA,
	GLX_DEPTH_SIZE, 24,
	GLX_DOUBLEBUFFER,
	None
};

static Display *dpy;
static Window win;

static int cursor_x;
static int cursor_y;
static int delta_x;
static int delta_y;
static int alive;
static int warp;

static Atom atom_delete;
static Atom atom_state;
static Atom atom_fullscreen;

static void (*key_cb)(int, int);

static void warp_cursor() {
	if (abs(cursor_x - WIDTH / 2) > 200 || abs(cursor_y - HEIGHT / 2) > 200) {
		XWarpPointer(dpy, win, win, 0, 0, 0, 0, WIDTH / 2, HEIGHT / 2);
		cursor_x = WIDTH / 2;
		cursor_y = HEIGHT / 2;
		XFlush(dpy);
	}
}

int window_init() {
	dpy = XOpenDisplay(NULL);
	if (!dpy)
		return 0;

	atom_delete = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	atom_state = XInternAtom(dpy, "_NET_WM_STATE", False);
	atom_fullscreen = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);

	return 1;
}

int window_create(int width, int height, int mode) {
	Window root;
	XVisualInfo *visinfo;
	XSetWindowAttributes winattrs;
	GLXContext glc;
	int screen;

	screen = XDefaultScreen(dpy);
	root = XRootWindow(dpy, screen);
	visinfo = glXChooseVisual(dpy, screen, glattrs);
	if (!visinfo) {
		printf("no appropriate visual found\n");
		return 0;
	}

	winattrs.colormap = XCreateColormap(dpy, root, visinfo->visual, AllocNone);
	winattrs.event_mask =
			ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask;

	win = XCreateWindow(dpy, root, 0, 0, width, height, 0, visinfo->depth,
			InputOutput, visinfo->visual, CWColormap | CWEventMask, &winattrs);

	XSetWMProtocols(dpy, win, &atom_delete, 1);
	switch (mode) {
	case WINDOW_FULLSCREEN:
		XChangeProperty(dpy, win, atom_state, XA_ATOM, 32, PropModeReplace,
				(unsigned char *) &atom_fullscreen, 1);
		break;
	}

	XMapWindow(dpy, win);

	glc = glXCreateContext(dpy, visinfo, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);

	alive = 1;

	return 1;
}

void window_set_cursor(int val) {
	Cursor cur;
	Pixmap pix;
	XColor black;
	char empty[8] = { 0 };

	if (val) {
		XDefineCursor(dpy, win, 0);
	}
	else {
		black.red = black.green = black.blue = 0;
		pix = XCreateBitmapFromData(dpy, win, empty, 8, 8);
		cur = XCreatePixmapCursor(dpy, pix, pix, &black, &black, 0, 0);

		XDefineCursor(dpy, win, cur);
		XFreeCursor(dpy, cur);
		XFreePixmap(dpy, pix);
	}
}

void window_set_warp(int val) {
	warp = val;
}

void window_poll_events() {
	XEvent xev;
	KeySym *ksyms;
	int one = 1;
	
	while (XPending(dpy) > 0) {
		XNextEvent(dpy, &xev);
		switch (xev.type) {
		case KeyPress:
		case KeyRelease:
			ksyms = XGetKeyboardMapping(dpy, xev.xkey.keycode, one, &one);
			if (key_cb) key_cb(*ksyms, xev.type == KeyPress);
			XFree(ksyms);
			break;
		case MotionNotify:
			delta_x += xev.xmotion.x - cursor_x;
			delta_y += xev.xmotion.y - cursor_y;
			cursor_x = xev.xmotion.x;
			cursor_y = xev.xmotion.y;
			break;
		case ClientMessage:
			alive = 0;
		}
	}

	if (warp)
		warp_cursor();
}

void window_swap_buffers() {
	glXSwapBuffers(dpy, win);
}

void window_get_cursor_delta(int *x, int *y) {
	*x = delta_x;
	*y = delta_y;
	delta_x = 0;
	delta_y = 0;
}

void window_get_cursor_pos(int *x, int *y) {
	*x = cursor_x;
	*y = cursor_y;
}

void window_register_key_cb(void (*cb)(int, int)) {
	key_cb = cb;
}

int window_alive() {
	return alive;
}
