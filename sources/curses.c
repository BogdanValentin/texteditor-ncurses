#include "../headers/curses.h"
#include "../headers/calculate.h"

void init_curses() {
    initscr();
    cbreak();
    noecho();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    // culori
}

/* 
    functia primeste numele fisierului si un int
    (0 daca fisierul nu a fost modificat, 1 daca a
    fost modificat sau 2 daca a fost salvat)
*/
void print_header(char filename[], int state) {
    WINDOW *header = newwin(1, COLS, 0, 0);
    wattron(header, COLOR_PAIR(1));

    // coloram in alb tot headerul
    for(int i = 0; i < COLS; i++) {
        wprintw(header, " ");
    }
    mvwprintw(header, 0, 0, "%s", filename);
    if(state == 1) {
        wprintw(header, "*");
    } else if(state == 2) {
        mvwprintw(header, 0, COLS - 5, "SAVED");
    }
    wattroff(header, COLOR_PAIR(1));
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