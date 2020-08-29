#include "position.h"
#include "printer.h"
#include <ncurses.h>

void init()
{
  initscr();
  keypad(stdscr, TRUE);
  start_color();
  init_color(COLOR_RED, 1000, 0, 0);
  init_color(COLOR_BLUE, 0, 0, 1000);
  init_color(COLOR_MAGENTA, 0, 0, 0);
  init_color(COLOR_WHITE, 1000, 1000, 1000);
  init_color(COLOR_BLACK, 600, 600, 600);
  init_color(COLOR_GREEN, 600, 600, 600);
  init_pair(0, COLOR_BLACK, COLOR_WHITE);
  init_pair(1, COLOR_RED, COLOR_WHITE);
  init_pair(2, COLOR_BLUE, COLOR_WHITE);
  init_pair(3, COLOR_MAGENTA, COLOR_WHITE);
  init_pair(4, COLOR_BLACK, COLOR_WHITE);
  init_pair(5, COLOR_GREEN, COLOR_WHITE);
}

void print_row(const position &p, int row)
{
  if ((hilight.x == row || hilight.x == row + 1) && (hilight.y == 1))
  {
    attron(COLOR_PAIR(3));
    printw("    +");
    attroff(COLOR_PAIR(3));
  }
  else
  {
    attron(COLOR_PAIR(4));
    printw("    +");
    attroff(COLOR_PAIR(4));
  }
  for (int i = 0; i < p.N; i++)
  {
    if ((hilight.x == row || hilight.x == row + 1) && hilight.y == i + 1)
    {
      attron(COLOR_PAIR(3));
      printw(" - ");
      attroff(COLOR_PAIR(3));
    }
    else
    {
      attron(COLOR_PAIR(5));
      printw(" - ");
      attroff(COLOR_PAIR(5));
    }
    if ((hilight.x == row || hilight.x == row + 1) && (hilight.y == i + 1 || hilight.y == i+2))
    {
      attron(COLOR_PAIR(3));
      printw("+");
      attroff(COLOR_PAIR(3));
    }
    else
    {
      attron(COLOR_PAIR(4));
      printw("+");
      attroff(COLOR_PAIR(4));
    }
  }
  attron(COLOR_PAIR(4));
  printw("    \n");
  attroff(COLOR_PAIR(4));
}

void print_numbers(const position &p)
{
  attron(COLOR_PAIR(4));
  printw("    ");
  for (int i = 1; i <= p.N; i++)
    printw("%3d ", i);
  printw("     \n");
  attroff(COLOR_PAIR(4));
}

void printsep(int i, int j)
{
  attron(COLOR_PAIR(5));
  if (hilight.x == i && (hilight.y == j || hilight.y == j + 1))
  {
    attron(COLOR_PAIR(3));
    printw("|");
    attroff(COLOR_PAIR(3));
  }
  else
    printw("|");
  attroff(COLOR_PAIR(5));
}

void print(const position &p)
{
  clear();
  print_numbers(p);
  print_row(p, 0);
  for (int i = 1; i <= p.N; i++)
  {
    attron(COLOR_PAIR(4));
    printw("%3d ", i);
    printsep(i, 0);
    attron(COLOR_PAIR(4));
    printw(" ");
    attroff(COLOR_PAIR(4));
    for (int j = 1; j <= p.N; j++)
    {
      if (!p.moves.empty() && i == p.moves.back().x && j == p.moves.back().y)
      {
        attron(COLOR_PAIR(3));
        printw("%c", symbol[p.board[i][j]]);
        attroff(COLOR_PAIR(3));
      }
      else if(symbol[p.board[i][j]] == ' ')
      {
        attron(COLOR_PAIR(4));
        printw("%c", symbol[p.board[i][j]]);
        attroff(COLOR_PAIR(4));
      }
      else
      {
        attron(COLOR_PAIR(p.board[i][j]));
        printw("%c", symbol[p.board[i][j]]);
        attroff(COLOR_PAIR(p.board[i][j]));
      }
      attron(COLOR_PAIR(4));
      printw(" ");
      printsep(i, j);
      attron(COLOR_PAIR(4));
      if (j < p.N)
        printw(" ");
      attroff(COLOR_PAIR(4));
    }
    attron(COLOR_PAIR(4));
    printw("%3d ", i);
    printw("\n");
    print_row(p, i);
    attroff(COLOR_PAIR(4));
  }
  print_numbers(p);
  refresh();
}
