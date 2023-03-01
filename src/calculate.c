#include "../include/calculate.h"

/*  Functia returneaza prin referinta numarul minim de linii si coloane necesare stocarii
    tuturor caracterelor din fisierul "filename" si numarul de caractere de pe fiecare linie */
void file_max_lines_and_cols(char filename[], int *lines, int *cols, int **charsperline) {
    int cols_max = 0;
    *lines = 1;
    *cols = 0;
    *charsperline = malloc(sizeof(int));

    FILE *file = fopen(filename, "rt");
    if(file != NULL) {
        // tratam cazul fisierului existent
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
        // tratam cazul cu ultimul rand cel mai lung
        if(*cols > cols_max) {
            cols_max = *cols;
        }
        // tratam cazul cu un singur rand
        if(cols_max > 0) {
            *cols = cols_max;
        }
        fclose(file);
    } else {
        // tratam cazul fisierului inexistent
        (*charsperline)[0] = 0;
    }
}

/*  Functia returneaza indicele liniei cu cel mai mare numar de caractere */
int line_max(int lines, int charsperline[]) {
    int max = -1, position;
    for(int i = 0; i < lines; i++) {
        if(charsperline[i] > max) {
            max = charsperline[i];
            position = i;
        }
    }
    return position;
}

int is_printable_key(int key) {
    if(key >= PRINTABLE_CHARS_LOWER_LIMIT && key <= PRINTABLE_CHARS_UPPER_LIMIT) {
        return 1;
    } else {
        return 0;
    }
}