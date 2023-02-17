#include "../headers/curses.h"
#include "../headers/calculate.h"

void init_curses() {
    initscr();
    cbreak();
    noecho();
    start_color();
    // culori
}

/* 
    functia primeste numele fisierului si un int
    (0 daca fisierul a fost salvat sau 1 daca a
    fost modificat)
*/
void print_header(char filename[], int state) {
    WINDOW *header = newwin(1, COLS, 0, 0);
    wprintw(header, "%s", filename);
    if(state == 1) {
        wprintw(header, "*");
    } else {
        mvwprintw(header, 0, COLS - 6, "SAVED");
    }
    wrefresh(header);
}

void init_mainwindow(char filename[]) {
    PAD mainwindow;
    
    file_max_lines_and_cols(filename, &mainwindow.lines, &mainwindow.cols, &mainwindow.charsperline);
    mainwindow.pad = newpad(mainwindow.lines, mainwindow.cols + 1); // +1 pt cursor

    FILE *file = fopen(filename, "rt");
    char buffer;
    if(file != NULL) {
        while(fscanf(file, "%c", &buffer) == 1) {
            wprintw(mainwindow.pad, "%c", buffer);
        }
    }
    prefresh(mainwindow.pad, 0, 0, 1, 0, LINES - 1, COLS - 1);
}