#pragma once

#include <ncurses.h>

typedef struct {
    WINDOW *pad;
    int lines, cols;
    int *charsperline;
    struct {
        int line, col;
    } cursor;
    struct {
        int line, col;
    } viewport;
} PAD;

void init_curses();
void print_header(char filename[], int state);
void init_mainwindow(PAD *mainwindow, char filename[]);
void update_mainwindow(PAD *mainwindow, char filename[]);