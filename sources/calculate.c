#include <ncurses.h>

void file_max_lines_and_cols(char filename[], int *lines, int *cols) {
    int cols_max = 0;
    *lines = 1;
    *cols = 0;

    FILE *file = fopen(filename, "rt");
    char buffer;
    while(fscanf(file, "%c", &buffer) == 1) {
        if(buffer == '\n') {
            (*lines)++;
            if(*cols > cols_max) {
                cols_max = *cols;
            }
            *cols = 0;
        } else {
            (*cols)++;
        }
    }
    if(*cols > cols_max) {
        cols_max = *cols;
    }
    if(cols_max > 0) {
        *cols = cols_max;
    }
}