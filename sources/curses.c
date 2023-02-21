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

void init_mainwindow(PAD *mainwindow, char filename[]) {
    file_max_lines_and_cols(filename, &(mainwindow->lines), &(mainwindow->cols), &(mainwindow->charsperline));
    mainwindow->pad = newpad(mainwindow->lines, mainwindow->cols + 1); // +1 pt cursor

    keypad(mainwindow->pad, true);
    mainwindow->cursor.line = 0;
    mainwindow->cursor.col = 0;
    mainwindow->viewport.line = 0;
    mainwindow->viewport.col = 0;

    FILE *file = fopen(filename, "rt");
    char buffer;
    if(file != NULL) {
        while(fscanf(file, "%c", &buffer) == 1) {
            wprintw(mainwindow->pad, "%c", buffer);
        }
    }
    wmove(mainwindow->pad, mainwindow->cursor.line, mainwindow->cursor.col); // muta cursorul la inceputul ecranului
    prefresh(mainwindow->pad, mainwindow->viewport.line, mainwindow->viewport.col, 1, 0, LINES - 1, COLS - 1);
}

void update_mainwindow(PAD *mainwindow) {
    int buffer;
    while (1) {
        buffer = wgetch(mainwindow->pad);

        if(buffer == KEY_UP && mainwindow->cursor.line > 0) {
            mainwindow->cursor.line--;
        } else if(buffer == KEY_DOWN && mainwindow->cursor.line < mainwindow->lines - 1) {
            mainwindow->cursor.line++;
        } else if(buffer == KEY_LEFT && mainwindow->cursor.col > 0) {
            mainwindow->cursor.col--;
        } else if(buffer == KEY_RIGHT && mainwindow->cursor.col < mainwindow->charsperline[mainwindow->cursor.line]) {
            mainwindow->cursor.col++;
        } else if(buffer >= 32 && buffer <= 126) {
            if(mainwindow->cursor.line == line_max(mainwindow->lines, mainwindow->charsperline)) {
                mainwindow->cols++; // de ce
                wresize(mainwindow->pad, mainwindow->lines, mainwindow->cols + 1); // de aia
            }
            winsch(mainwindow->pad, buffer);
            mainwindow->charsperline[mainwindow->cursor.line]++;
            mainwindow->cursor.col++;
           
        } else if(buffer == KEY_BACKSPACE) {
            if(mainwindow->cursor.col > 0) { // pe acelasi rand
                mainwindow->charsperline[mainwindow->cursor.line]--;
                mvwdelch(mainwindow->pad, mainwindow->cursor.line, mainwindow->cursor.col-- - 1);
            } else {                         // mergem pe randul de sus

            }
        } else if(buffer == 330) {
            if(mainwindow->cursor.col > 0) { // pe acelasi rand
                mainwindow->charsperline[mainwindow->cursor.line]--;
                wdelch(mainwindow->pad);
            } else if(mainwindow->cursor.col == 0) {
                wdelch(mainwindow->pad);
            } else {                         // mergem pe randul de sus

            }
        }

        if(mainwindow->cursor.line > mainwindow->viewport.line + LINES - 2) {
            (mainwindow->viewport.line)++;
        }
        if(mainwindow->cursor.line < mainwindow->viewport.line) {
            (mainwindow->viewport.line)--;
        }
        
        if(mainwindow->cursor.col > mainwindow->viewport.col + COLS - 1) {
            (mainwindow->viewport.col)++;
        }
        if(mainwindow->cursor.col > mainwindow->charsperline[mainwindow->cursor.line]) {
            mainwindow->cursor.col = mainwindow->charsperline[mainwindow->cursor.line];
        }
        
        if(buffer == KEY_UP || buffer == KEY_DOWN) {
            if(mainwindow->viewport.col > mainwindow->cursor.col)
                mainwindow->viewport.col = mainwindow->charsperline[mainwindow->cursor.line] - 1;
            
        } else {
            if(mainwindow->cursor.col < mainwindow->viewport.col) {
                (mainwindow->viewport.col)--;
            }
        }
        wmove(mainwindow->pad, mainwindow->cursor.line, mainwindow->cursor.col);
        prefresh(mainwindow->pad, mainwindow->viewport.line, mainwindow->viewport.col, 1, 0, LINES - 1, COLS - 1);
    }
}