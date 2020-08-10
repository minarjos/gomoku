#include "game.h"

int game(position &p, Player *p1, Player *p2)
{
  Ai a = *(new Ai(p.N, 0));
  while(1)
  {
    my_move move = p1->play(p);
    if(move.x == 0 && move.y == 0 || p.moves.size() == p.N*p.N)
      return 0;
    p.play_move(move);
    print(p);
    if(abs(a.hash_evaluation(p)) > 5e4)
      return 1;
    move = p2->play(p);
    if(move.x == 0 && move.y == 0 || p.moves.size() == p.N*p.N)
      return 0;
    p.play_move(move);
    print(p);
    if(abs(a.hash_evaluation(p)) > 5e4)
      return 2;
  }
}
