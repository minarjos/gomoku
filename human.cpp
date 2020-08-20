#include "human.h"

my_move Human::play(position &p)
{
  while (1)
  {
    print(p);
    int action = getch();

    if (action == KEY_LEFT && hilight.y > 1)
      hilight.y--;
    else if (action == KEY_RIGHT && hilight.y < p.N)
      hilight.y++;
    else if (action == KEY_UP && hilight.x > 1)
      hilight.x--;
    else if (action == KEY_DOWN && hilight.x < p.N)
      hilight.x++;
    else if (action == 10 && p.empty(hilight))
      return hilight;
    else if (action == 27)
      return {0, 0};
  }
}
