#ifndef POSITION_H
#define POSITION_H

#include<vector>

const int N = 15;

struct my_move
{
  int x, y;
};

struct position
{
  int to_move = 1;
  char board[N+2][N+2];
  std::vector<my_move> moves;
  void play_move(my_move m);
  bool empty(my_move m);
  position();
  long long get_hash();
};


#endif
