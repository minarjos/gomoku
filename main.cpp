#include "position.cpp"
#include "printer.cpp"
#include "human.cpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  position p(atoi(argv[1]), atoi(argv[2]));
  init();

  Human *h = new Human();

  while(1)
  {
    my_move move = h->play(p);
    if(move.x == 0 && move.y == 0)
      break;
    p.play_move(move);
  }


  endwin();
  return 0;
}
