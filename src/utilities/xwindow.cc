// src/utilities/xwindow.cc

#include "../../include/utilities/xwindow.h"
#include "../../include/board.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>

using namespace std;

XWindow::XWindow(int width, int height) : font_info(nullptr), width(width), height(height) {
    // Open the display
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        // cerr << "Cannot open display" << endl;
        exit(1);
    }
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                            BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);

    // Create a pixmap
    Pixmap pix = XCreatePixmap(d, w, width, height, DefaultDepth(d, DefaultScreen(d)));
    gc = XCreateGC(d, pix, 0, 0);

    XFlush(d);

    // Load font
    const char* font_name = "-adobe-courier-medium-r-normal--24-240-75-75-m-150-iso8859-1";
    font_info = XLoadQueryFont(d, font_name);
    if (font_info == nullptr) {
        // cerr << "Cannot open font: loaded fixed" << endl;
        font_info = XLoadQueryFont(d, "fixed");
    }
    XSetFont(d, gc, font_info->fid);

    // Set up colours
    colours[0] = 0xFFFFFF; // White
    colours[1] = 0x8B4513; // Dark brown (takes the place of black on the board)
    colours[2] = 0x808080; // Grey
    colours[3] = 0x000000; // Black

    XSetForeground(d, gc, colours[2]);

    // Set up hints
    XSizeHints hints;
    hints.flags = (USPosition | PSize | PMinSize | PMaxSize);\
    hints.height = hints.min_height = hints.max_height = height;
    hints.width = hints.min_width = hints.max_width = width;
    XSetNormalHints(d, w, &hints);

    // Synchronize
    XSynchronize(d, True);
    XFlush(d);
}

XWindow::~XWindow() {
    // cerr << "Closing display" << endl;
    if (font_info) {
        XFreeFont(d, font_info);
        // cerr << "Freed font" << endl;
    }
    XFreeGC(d, gc);
    // cerr << "Freed GC" << endl;
    XDestroyWindow(d, w);
    // cerr << "Destroyed window" << endl;
    XCloseDisplay(d);
    // cerr << "Closed display" << endl;
}

void XWindow::drawBoard(const Board &board) {
    int vert_off = 100;
    int horiz_off = 100;

    // Draw the board
    XSetForeground(d, gc, colours[2]);
    XFillRectangle(d, w, gc, 0, 0, width, height);
    XSetForeground(d, gc, colours[3]);
    XFillRectangle(d, w, gc, horiz_off - 10, vert_off - 10, 820, 820);
    XSetForeground(d, gc, colours[1]);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((i + j) % 2) {
                XFillRectangle(d, w, gc, horiz_off + i * 100, j * 100 + vert_off, 100, 100);
            } else {
                XSetForeground(d, gc, colours[0]);
                XFillRectangle(d, w, gc, horiz_off + i * 100, j * 100 + vert_off, 100, 100);
                XSetForeground(d, gc, colours[1]);
            }
        }
    }

    // Demarcate the board
    XSetForeground(d, gc, colours[3]);
    const char* num_demarc = "12345678";
    const char* alpha_demarc = "abcdefgh";
    for (int i = 0; i < 8; ++i) {
        const char num_str[2] = {num_demarc[i], '\0'};
        const char alpha_str[2] = {alpha_demarc[i], '\0'};
        XDrawString(d, w, gc, i * 100 + 40 + horiz_off, height - vert_off + 50, alpha_str, 1);
        XDrawString(d, w, gc, horiz_off - 40, (7 - i) * 100 + 50 + vert_off, num_str, 1);
    }

    // Draw the "pieces"
    for (unsigned int i = 0; i < 8; ++i) {
        for (unsigned int j = 0; j < 8; ++j) {
            if (board[{i, j}]) {
                const char name = board[{i, j}]->getName();
                const char name_str[2] = {name, '\0'};
                XDrawString(d, w, gc, i * 100 + 40 + horiz_off, (7 - j) * 100 + 50 + vert_off, name_str, 1);
            }
        }
    }

    // Draw the dead pieces on the side
    if (board.showDead) {
      int white_offset = 0;
      int black_offset = 0;
      for (auto piece: board.deadPieces) {
          const char name = piece->getName();
          const char name_str[2] = {name, '\0'};
          if (piece->getColour()) {
            XSetForeground(d, gc, colours[0]);
            XDrawString(d, w, gc, 1000 + (40 * (white_offset / 7)), 50 * (white_offset - 7 * (white_offset / 7)) + vert_off, name_str, 1);
            white_offset += 1;
          } else {
            XSetForeground(d, gc, colours[1]);
            XDrawString(d, w, gc, 1000 + (40 * (black_offset / 7)), (50 * (black_offset - 7 * (black_offset / 7))) + 500 + vert_off, name_str, 1);
            black_offset += 1;
          }
      }
  }

    XFlush(d);
}

int XWindow::getWidth() const {
    return width;
}

int XWindow::getHeight() const {
    return height;
}
