// include/utilities/xwindow.h
#ifndef XWINDOW_H
#define XWINDOW_H
#include <X11/Xlib.h>
#include <X11/Xutil.h>

class Board;

class XWindow {
    Display *d;
    Window w;
    GC gc;
    int s;
    unsigned long colours[3];
    Board &board;

    public:
    XWindow(int width, int height, Board &board);
    ~XWindow();

    void drawBoard();
};

#endif // XWINDOW_H
