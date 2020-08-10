#ifndef HUMAN_H
#define HUMAN_H

#include "position.h"
#include "player.h"

class Human:public Player
{
public:
  my_move play(position &p);
};



#endif
