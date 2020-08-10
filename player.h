#ifndef PLAYER_H
#define PLAYER_H

#include "position.h"

class Player
{
public:
  virtual my_move play(position &p){};
};

#endif
