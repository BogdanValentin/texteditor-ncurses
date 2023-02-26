#include "../include/curses.h"
#include "../include/calculate.h"

void init_curses() {
    initscr();
    cbreak();
    noecho();
    
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    // culori
}

// pt bugul cu ghosting
void footer(PAD mainwindow) {
    WINDOW *footer = newwin(LINES - mainwindow.lines, COLS, mainwindow.lines, 0);
    wrefresh(footer);
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
        fclose(file);
    }
    
}

void update_mainwindow(PAD *mainwindow, char filename[]) {
    int buffer;
    while (1) {
        wmove(mainwindow->pad, mainwindow->cursor.line, mainwindow->cursor.col);
        prefresh(mainwindow->pad, mainwindow->viewport.line, mainwindow->viewport.col, 1, 0, LINES - 1, COLS - 1);

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
        } else if(buffer == 10) { // tasta enter
            // la sfarsitul randului
            if(mainwindow->cursor.col == mainwindow->charsperline[mainwindow->cursor.line]) {
                mainwindow->lines++;
                wresize(mainwindow->pad, mainwindow->lines, mainwindow->cols);
                
                mainwindow->cursor.line++;
                mainwindow->cursor.col = 0;
                
                wmove(mainwindow->pad, mainwindow->cursor.line, mainwindow->cursor.col);
                int i;
                for(i = mainwindow->lines - 1; i > mainwindow->cursor.line; i--) {
                    mainwindow->charsperline[i] = mainwindow->charsperline[i - 1];
                }
                mainwindow->charsperline[i] = 0;
                winsertln(mainwindow->pad);
                // to do sf de fisier
            }
        } else if(buffer == KEY_BACKSPACE) {
            if(mainwindow->cursor.col > 0) { // pe acelasi rand
                mainwindow->charsperline[mainwindow->cursor.line]--;
                mvwdelch(mainwindow->pad, mainwindow->cursor.line, mainwindow->cursor.col-- - 1);
            } else if(mainwindow->cursor.line > 0){                         // mergem pe randul de sus  // daca nu e primul rand
                // creare vector cu elementele randului
                char *line_contents = malloc((mainwindow->charsperline[mainwindow->cursor.line] + 1) * sizeof(char));
                int nr_elem = mainwindow->charsperline[mainwindow->cursor.line] + 1;
                winnstr(mainwindow->pad, line_contents, nr_elem);
                line_contents[nr_elem - 1] = '\0';
                for(int i = mainwindow->cursor.line; i < mainwindow->lines - 1; i++) {
                    mainwindow->charsperline[i] = mainwindow->charsperline[i + 1];
                }
                mainwindow->lines--;
                prefresh(mainwindow->pad, mainwindow->viewport.line, mainwindow->viewport.col, 1, 0, LINES - 1, COLS - 1);
                mainwindow->charsperline = realloc(mainwindow->charsperline, mainwindow->lines * sizeof(int));
                wmove(mainwindow->pad, mainwindow->cursor.line, mainwindow->cursor.col);
                wdeleteln(mainwindow->pad);
                mainwindow->cursor.line--;
                mainwindow->cursor.col = mainwindow->charsperline[mainwindow->cursor.line];
                // daca linia de mai sus e cea mai mare din pad
                if(mainwindow->charsperline[line_max(mainwindow->lines, mainwindow->charsperline)] < 
                   mainwindow->charsperline[mainwindow->cursor.line] + nr_elem) {
                    mainwindow->cols += nr_elem;
                    wresize(mainwindow->pad, mainwindow->lines, mainwindow->cols);
                    mainwindow->viewport.col = mainwindow->cols - COLS - 1;
                    footer(*mainwindow);
                }
                wmove(mainwindow->pad, mainwindow->cursor.line, mainwindow->cursor.col);
                wprintw(mainwindow->pad, "%s", line_contents);
                mainwindow->charsperline[mainwindow->cursor.line] += nr_elem - 1;
            }
        } else if(buffer == 330) { // tasta delete
            if(mainwindow->cursor.col >= 0) { // pe acelasi rand
                mainwindow->charsperline[mainwindow->cursor.line]--;
                wdelch(mainwindow->pad);
            } 
        } else if(buffer == 1) { // CTRL + A aka save
            FILE *file = fopen(filename, "wt");
            if(file != NULL) {
                for(int i = 0; i < mainwindow->lines; i++) {
                    char *line_contents = malloc((mainwindow->charsperline[i] + 1) * sizeof(char));
                    int nr_elem = mainwindow->charsperline[i] + 1;
                    mvwinnstr(mainwindow->pad, i, 0, line_contents, nr_elem);
                    line_contents[nr_elem - 1] = '\0';
                    fprintf(file, "%s", line_contents);
                    if(i != mainwindow->lines - 1) {
                        fprintf(file, "\n");
                    }
                    free(line_contents);
                }
                fclose(file);
            }
            
            int x = mainwindow->cursor.line, y = mainwindow->cursor.col;
            init_mainwindow(mainwindow, filename);
            mainwindow->cursor.line = x;
            mainwindow->cursor.col = y;
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
        
    }
}