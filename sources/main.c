#include "../headers/curses.h"

int main(int argc, char *argv[])
{
    init_curses();

    print_header(argv[1], 1);
    init_mainwindow(argv[1]);
    
    endwin();
    return 0;
}