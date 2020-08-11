#ifndef AI_H
#define AI_H

#include "position.h"
#include "player.h"
#include "constants.cpp"
#include <algorithm>
#include <unordered_map>
#include <time.h>

const int combos_size = 6;
const int hash_size = 1<<(2*combos_size)+1;
const int combo_types = 15;

struct combos
{
  int xcombos[combo_types], ocombos[combo_types];

  void add_combo(int x)
  {
    if(x > 0)
      xcombos[x]++;
    else
      ocombos[-x]++;
  }
};


struct eval_move
{
  my_move m;
  int e;
};

class Ai:public Player
{
private:
  int move_time;
  std::unordered_map<long long, int> hash_map;
  std::unordered_map<long long, eval_move> hash_map_smarter[1000];
  std::unordered_map<long long, eval_move> hash_map_fours[1000];
  std::unordered_map<long long, eval_move> hash_map_threes[1000];
  int combo_hashes[hash_size];
  void parse_combos(string file);
  combos get_combos(position &p);
  int center(int x, int y);
  int sum_centers(position &p);
  bool has_fours(combos &c, combos &old);
  bool has_threes(combos &c, combos &old);
  bool have_to_defend(position &p);
public:
  int N;
  eval_move blindspot(position &p);
  int hash_evaluation(position &p);
  Ai(int n, int t, string file="combos")
  {
    std::srand(time(NULL));
    move_time = t;
    N = n;
    parse_combos(file);
  }
  eval_move play_deep_smarter_ab_selective(position &p, int d, int s, int t, int ab);
  eval_move hashed(position &p, int d, int s, int t, int ab);
  eval_move only_fours(position &p, int d, int t, int ab);
  eval_move only_threes(position &p, int d, int t, int ab);
  eval_move play_deep_smarter_ab_selective_timed(position &p, int t, int s);
  eval_move hashed_timed(position &p, int t, int s);
  eval_move only_fours_timed(position &p, int t);
  eval_move only_threes_timed(position &p, int t);
  my_move play(position &p);
};

#endif
