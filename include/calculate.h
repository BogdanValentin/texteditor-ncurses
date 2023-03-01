#pragma once

#include <ncurses.h>
#include <stdlib.h>

#define PRINTABLE_CHARS_LOWER_LIMIT 32
#define PRINTABLE_CHARS_UPPER_LIMIT 126

void file_max_lines_and_cols(char filename[], int *lines, int *cols, int **charsperline);
int line_max(int lines, int charsperline[]);
int is_printable_key(int key);