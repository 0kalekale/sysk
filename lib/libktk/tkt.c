#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdlib.h>
#include <unistd.h>

Display *dis;
int screen;
Window win;
GC gc; 
XEvent e;

void tkt_init(int x, int y, const char *window_name)  {
	unsigned long black, white;

	dis = XOpenDisplay((char *)0);
	screen = DefaultScreen(dis);

	black = BlackPixel(dis, screen);
	white = WhitePixel(dis, screen);
	
	win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 200, 300, 5, white, black);
	
	XSetStandardProperties(dis,win, window_name,"HI!",None,NULL,0,NULL);
	XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);
	
	gc=XCreateGC(dis, win, 0,0);
	
	XSetBackground(dis,gc,white);
	XSetForeground(dis,gc,black);
	XClearWindow(dis, win);
	XMapRaised(dis, win);
	
	while (1) {
		XNextEvent(dis, &e);
		if (e.type == Expose) {
			XFillRectangle(dis, win, DefaultGC(dis, screen), 20, 20, 10, 10);
			XDrawString(dis, win, DefaultGC(dis, screen), 10, 50, window_name, strlen(window_name));
		}
	if (e.type == KeyPress)
		break;
   	}
} 

void tkt_close() {
	XFreeGC(dis, gc);
	XDestroyWindow(dis,win);
	XCloseDisplay(dis);	
	exit(0);
}
