#include "position.cpp"
#include "printer.cpp"
#include "human.cpp"
#include "ai.cpp"
#include <iostream>
#include <fstream>

using namespace std;

int N, to_move, move_time;

int main(int argc, char const *argv[]) {

  N = atoi(argv[1]);
  to_move = atoi(argv[2]);
  move_time = atoi(argv[3]);
  position p(N, to_move);
  init();
  hilight = {(N+1)/2, (N+1)/2};

  Human h = *(new Human());
  Ai a = *(new Ai(N, move_time));

  Player *p1, *p2;
  p1 = &a;
  p2 = &a;

  if(to_move == 2)
    swap(p1, p2);

  char c;

  while(1)
  {
    my_move move = p1->play(p);
    if(move.x == 0 && move.y == 0)
      break;
    p.play_move(move);
    print(p);
    move = p2->play(p);
    p.play_move(move);
    print(p);
  }


  endwin();
  return 0;
}
