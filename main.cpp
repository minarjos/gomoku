#include "position.cpp"
#include "printer.cpp"
#include "human.cpp"
#include "ai.cpp"
#include "game.cpp"
#include <iostream>
#include <fstream>

using namespace std;

int N, to_move, move_time, firstai, secondhuman;

int main(int argc, char const *argv[]) {

  N = atoi(argv[1]);
  to_move = atoi(argv[2]);
  move_time = atoi(argv[3]);
  firstai = atoi(argv[4]);
  secondhuman = atoi(argv[5]);
  position p(N, to_move);
  init();
  hilight = {(N+1)/2, (N+1)/2};

  Human h = *(new Human());
  Ai a = *(new Ai(N, move_time));

  Player *p1, *p2;
  p1 = &h;
  if(firstai)
    p1 = &a;
  p2 = &a;
  if(secondhuman)
    p2 = &h;

  if(to_move == 2)
    swap(p1, p2);

  int result = game(p, p1, p2);

  if(result)
  {
    printw("Player %d won!", (result + to_move)%2+1);
    getch();
  }

  endwin();
  return 0;
}
