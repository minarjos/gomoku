#include "position.h"

using namespace std;

void position::play_move(my_move m)
{
  board[m.x][m.y] = to_move;
  moves.push_back(m);
  to_move = (1+to_move%2);
}

bool position::empty(my_move m)
{
  return !board[m.x][m.y];
}

void position::undo()
{
  my_move m = moves.back();
  board[m.x][m.y] = 0;
  moves.pop_back();
  to_move = (1+to_move%2);
}

position::position(int n, int to_mov)
{
  N = n;
  to_move = to_mov;
  for(int i = 0; i <= N; i++)
    for(int j = 0; j <= N; j++)
      board[i][j] = 0;
  for(int i = 0; i <= N+1; i++)
    board[0][i] = board[i][0] = board[N+1][i] = board[i][N+1] = 3;
  moves = vector<my_move>();
}

long long position::get_hash()
{
  long long ans = 0;
  for(int i = 1; i <= N; i++)
      for(int j = 1; j <= N; j++)
        ans = 1000003*ans+1000007*board[i][j];
  return 47*ans+43*to_move;
}
