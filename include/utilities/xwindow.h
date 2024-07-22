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
    int s, width, height;
    unsigned long colours[4];

    public:
    XWindow(int width, int height);
    ~XWindow();

    int getWidth() const;
    int getHeight() const;

    void drawBoard(Board &board);
};

#endif // XWINDOW_H
