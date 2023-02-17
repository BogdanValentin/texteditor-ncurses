#pragma once

#include <ncurses.h>

typedef struct {
    WINDOW *pad;
    int lines, cols;
    int *charsperline;
} PAD;

void init_curses();
void print_header(char filename[], int state);
void init_mainwindow(char filename[]);