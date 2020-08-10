#ifndef POSITION_H
#define POSITION_H

#include<vector>


struct my_move
{
  int x, y;
};

struct position
{
  int N;
  int to_move = 1;
  char board[50][50];
  std::vector<my_move> moves;
  void play_move(my_move m);
  bool empty(my_move m);
  position(int n, int to_mov);
  long long get_hash();
};


#endif
