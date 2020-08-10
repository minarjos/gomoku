#ifndef PRINTER_H
#define PRINTER_H

#include <ncurses.h>
#include "position.h"

const char symbol[] = {' ', 'X', 'O', '-'};

my_move hilight = {8, 8};

void init();
void print_row(const position &p, int row);
void print_numbers(const position &p);
void printsep(int i, int j);
void print(const position &p);


#endif
