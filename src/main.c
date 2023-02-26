#include "../include/curses.h"

int main(int argc, char *argv[])
{
    init_curses();

    print_header(argv[1], 0);

    PAD mainwindow;
    init_mainwindow(&mainwindow, argv[1]);
    update_mainwindow(&mainwindow, argv[1]);

    
    endwin();
    
    return 0;
}