#include "position.cpp"
#include "printer.cpp"
#include "human.cpp"
#include "ai.cpp"
#include "game.cpp"
#include <iostream>
#include <fstream>

using namespace std;

int N, to_move, move_time, secondai, firsthuman;

int main(int argc, char const *argv[])
{

  N = atoi(argv[1]);
  to_move = atoi(argv[2]);
  move_time = atoi(argv[3]);
  secondai = atoi(argv[4]);
  firsthuman = atoi(argv[5]);
  position p(N, to_move);
  init();
  hilight = {(N + 1) / 2, (N + 1) / 2};

  Human h = Human();
  Ai a = Ai(N, move_time);

  Player *p1, *p2;
  p1 = &a;
  if (firsthuman)
    p1 = &h;
  p2 = &h;
  if (secondai)
    p2 = &a;

  if (to_move == 2)
    swap(p1, p2);

  int result = game(p, p1, p2);

  if (result)
  {
    printw("Player %d won!", (result + to_move) % 2 + 1);
    getch();
  }

  endwin();
  return 0;
}
