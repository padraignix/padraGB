#pragma once

#define argc __argc
#define argv __argv

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <gtk/gtk.h>

void quit(void);

/*	extern Display *dpy;
	extern Window root;
	extern GLint att[];
	extern XVisualInfo *vi;
	extern Colormap cmap;
	extern XSetWindowAttributes swa;
	extern Window win;
	extern GLXContext glc;
	extern XWindowAttributes gwa;
	extern XEvent xev;
*/

extern GtkWidget *window;
extern GdkPixbuf *pixbuf;
extern GdkPixbuf *new_pixbuf;
extern int smult;

