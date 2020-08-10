#include "human.h"

my_move Human::play(position &p)
{
  while(1)
  {
    print(p);

    int action = getch();

    if(action == KEY_LEFT && hilight.y > 1)
      hilight.y--;
    if(action == KEY_RIGHT && hilight.y < N)
      hilight.y++;
    if(action == KEY_UP && hilight.x > 1)
      hilight.x--;
    if(action == KEY_DOWN && hilight.x < N)
      hilight.x++;
    if(action == 10 && p.empty(hilight))
      return hilight;
  }
}
