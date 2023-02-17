#include <ncurses.h>

void file_max_lines_and_cols(char filename[], int *lines, int *cols, int **charsperline) {
    int cols_max = 0;
    *lines = 1;
    *cols = 0;
    *charsperline = malloc(sizeof(int));

    FILE *file = fopen(filename, "rt");
    if(file != NULL) {
        char buffer;
        while(fscanf(file, "%c", &buffer) == 1) {
            if(buffer == '\n') {
                *charsperline = realloc(*charsperline, *lines * sizeof(int));
                (*charsperline)[*lines - 1] = *cols;
                (*lines)++;
                if(*cols > cols_max) {
                    cols_max = *cols;
                }
                *cols = 0;
            } else {
                (*cols)++;
            } 
        }
        *charsperline = realloc(*charsperline, (*lines) * sizeof(int));
        (*charsperline)[*lines - 1] = *cols;
        if(*cols > cols_max) { // tratam cazul cu ultimul rand cel mai lung
            cols_max = *cols;
        }
        if(cols_max > 0) { // tratam cazul cu un singur rand
            *cols = cols_max;
        }

    }
}