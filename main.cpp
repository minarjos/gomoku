#include "position.cpp"
#include "printer.cpp"
#include "human.cpp"
#include "ai.cpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  position p(atoi(argv[1]), atoi(argv[2]));
  init();
  hilight = {p.N/2, p.N/2};

  Human *h = new Human();
  Ai *a = new Ai(atoi(argv[1]));

  while(1)
  {
    my_move move = h->play(p);
    if(move.x == 0 && move.y == 0)
      break;
    p.play_move(move);
    print(p);
    move = a->play(p, atoi(argv[3])).m;
    p.play_move(move);
  }


  endwin();
  return 0;
}
