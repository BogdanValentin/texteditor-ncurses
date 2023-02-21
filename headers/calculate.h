#pragma once

#include <ncurses.h>
#include <stdlib.h>

void file_max_lines_and_cols(char filename[], int *lines, int *cols, int **charsperline);
int line_max(int lines, int charsperline[]);