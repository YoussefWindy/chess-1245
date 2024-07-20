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

XWindow::XWindow(int width, int height) : width(width), height(height) {
    // Open the display
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        cerr << "Cannot open display" << endl;
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

    // Set up colours
    colours[0] = 0xFFFFFF; // White
    colours[1] = 0x8B4513; // Dark brown (takes the place of black on the board)
    colours[2] = 0xF5F5DC; // Beige

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
    XFreeGC(d, gc);
    XCloseDisplay(d);
}

void XWindow::drawBoard(Board &board) {
    // Load font
    const char* font_name = "fixed";
    auto font_info = XLoadQueryFont(d, font_name);
    XSetFont(d, gc, font_info->fid);

    // Draw the board
    XSetForeground(d, gc, colours[2]);
    XFillRectangle(d, w, gc, 0, 0, 1000, 800);
    XSetForeground(d, gc, colours[1]);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((i + j) % 2) {
                XFillRectangle(d, w, gc, 200 + i * 100, j * 100, 100, 100);
            } else {
                XSetForeground(d, gc, colours[2]);
                XFillRectangle(d, w, gc, 200 + i * 100, j * 100, 100, 100);
                XSetForeground(d, gc, colours[1]);
            }
        }
    }

    // Demarcate the board
    for (int i = 0; i < 8; ++i) {
        XDrawString(d, w, gc, i * 100 + 240, 20, "87654321", 8);
        XDrawString(d, w, gc, 220, i * 100 + 40, "abcdefgh", 8);
    }

    // Draw the "pieces"
    for (unsigned int i = 0; i < 8; ++i) {
        for (unsigned int j = 0; j < 8; ++j) {
            if (board[{i, j}]) {
                const char name = board[{i, j}]->getName();
                const char name_str[2] = {name, '\0'};
                XDrawString(d, w, gc, i * 100 + 240, j * 100 + 40, name_str, 1);
            }
        }
    }

    // Draw the dead pieces on the side
    int white_offset = 0;
    int black_offset = 0;
    for (auto piece: board.deadPieces) {
        const char name = piece->getName();
        const char name_str[2] = {name, '\0'};
        if (piece->getColour()) {
            XSetForeground(d, gc, colours[0]);
            XDrawString(d, w, gc, 800 + (40 * (white_offset % 8)), 40 * (white_offset - 8 * (white_offset % 8)), name_str, 1);
            white_offset += 1;
        } else {
            XSetForeground(d, gc, colours[1]);
            XDrawString(d, w, gc, 800 + (40 * (black_offset % 8)), 400 + (40 * (black_offset - 8 * (black_offset % 8))), name_str, 1);
            black_offset += 1;
        }
    }

    XFlush(d);

    // Clean up
    XFreeFont(d, font_info);
}

int XWindow::getWidth() const {
    return width;
}

int XWindow::getHeight() const {
    return height;
}
