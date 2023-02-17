#include <ncurses.h>

typedef struct {
    WINDOW *pad;
    int lines, cols;
    int *charsperline;
} PAD;

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

void print_mainwindow(char filename[]) {
    PAD mainwindow;
    
    file_max_lines_and_cols(filename, &mainwindow.lines, &mainwindow.cols);
    mainwindow.pad = newpad(mainwindow.lines, mainwindow.cols + 1) // +1 pt cursor
}