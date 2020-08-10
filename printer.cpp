#include "position.h"
#include "printer.h"
#include <ncurses.h>

void init()
{
  initscr();
  keypad(stdscr, TRUE);
  start_color();
  init_pair(0, COLOR_RED, COLOR_BLACK);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
}

void print_row(const position &p, int row)
{
  printw("    +");
  for(int i = 0; i < p.N; i++)
  {
    if((hilight.x == row || hilight.x == row+1) && hilight.y == i+1)
    {
      attron(COLOR_PAIR(3));
      printw(" - ");
      attroff(COLOR_PAIR(3));
    }
    else
      printw(" - ");
    printw("+");
  }
  printw("\n");
}

void print_numbers(const position &p)
{
  printw("    ");
  for(int i = 1; i <= p.N; i++)
    printw("%3d ", i);
  printw("\n");
}

void printsep(int i, int j)
{
  if(hilight.x == i && (hilight.y == j || hilight.y == j+1))
  {
    attron(COLOR_PAIR(3));
    printw("|");
    attroff(COLOR_PAIR(3));
  }
  else
    printw("|");
}

void print(const position &p)
{
  clear();
  print_numbers(p);
  print_row(p, 0);
  for(int i = 1; i <= p.N; i++)
  {
    printw("%3d ", i);
    printsep(i, 0);
    printw(" ");
    for(int j = 1; j <= p.N; j++)
    {
      if(!p.moves.empty() && i == p.moves.back().x && j == p.moves.back().y)
      {
        attron(COLOR_PAIR(3));
        printw("%c", symbol[p.board[i][j]]);
        attroff(COLOR_PAIR(3));
      }
      else
      {
        attron(COLOR_PAIR(p.board[i][j]));
        printw("%c", symbol[p.board[i][j]]);
        attroff(COLOR_PAIR(p.board[i][j]));
      }
      printw(" ");
      printsep(i, j);
      if(j < p.N)
        printw(" ");
    }
    printw("%3d ", i);
    printw("\n");
    print_row(p, i);
  }
  print_numbers(p);
  refresh();
}
