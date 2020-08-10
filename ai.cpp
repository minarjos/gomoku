#include "ai.h"
#include <algorithm>
#include <fstream>
#include <chrono>
#include <sys/time.h>


int get_time()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  return ms;
}

void Ai::parse_combos(string file)
{
  std::fstream fin(file);
  string combo;
  int num;
  while(fin>>combo>>num)
  {
    int hsh1 = 0, hsh2 = 0, hsh3 = 0, hsh4 = 0;
    for(int i = 0; i < combo.size(); i++)
    {
      hsh1 *= 4;
      hsh2 *= 4;
      hsh3 *= 4;
      hsh4 *= 4;
      if(combo[i] == 'X')
      {
        hsh1 += 1;
        hsh2 += 1;
        hsh3 += 2;
        hsh4 += 2;
      }
      else if(combo[i] == 'O')
      {
        hsh1 += 2;
        hsh2 += 3;
        hsh3 += 1;
        hsh4 += 3;
      }
    }
    combo_hashes[hsh1] = num;
    combo_hashes[hsh2] = num;
    combo_hashes[hsh3] = -num;
    combo_hashes[hsh4] = -num;
  }
}

combos Ai::get_combos(position &p)
{
  combos c;
  for(int i = 0; i < combo_types; i++)
    c.xcombos[i] = c.ocombos[i] = 0;
  int hsh = 0;
  for(int i = 0; i <= N+1; i++)
  {
    for(int j = 0; j <= N+1; j++)
    {
      hsh *= 4;
      hsh %= (1<<(2*combos_size));
      hsh += p.board[i][j];
      if(j >= 5)
        c.add_combo(combo_hashes[hsh]);
    }
  }
  for(int i = 0; i <= N+1; i++)
  {
    for(int j = 0; j <= N+1; j++)
    {
      hsh *= 4;
      hsh %= (1<<(2*combos_size));
      hsh += p.board[j][i];
      if(j >= 5)
        c.add_combo(combo_hashes[hsh]);
    }
  }
  for(int i = 0; i <= N+1; i++)
  {
    for(int j = 0; i+j <= N+1; j++)
    {
      hsh *= 4;
      hsh %= (1<<(2*combos_size));
      hsh += p.board[i+j][j];
      if(j >= 5)
        c.add_combo(combo_hashes[hsh]);
    }
  }
  for(int i = 1; i <= N+1; i++)
  {
    for(int j = 0; i+j <= N+1; j++)
    {
      hsh *= 4;
      hsh %= (1<<(2*combos_size));
      hsh += p.board[j][i+j];
      if(j >= 5)
        c.add_combo(combo_hashes[hsh]);
    }
  }
  for(int i = 0; i <= N; i++)
  {
    for(int j = 0; i-j >= 0; j++)
    {
      hsh *= 4;
      hsh %= (1<<(2*combos_size));
      hsh += p.board[i-j][j];
      if(j >= 5)
        c.add_combo(combo_hashes[hsh]);
    }
  }
  for(int i = 0; i <= N+1; i++)
  {
    for(int j = 0; i-j >= 0; j++)
    {
      hsh *= 4;
      hsh %= (1<<(2*combos_size));
      hsh += p.board[N-(i-j)+1][N-j+1];
      if(j >= 5)
        c.add_combo(combo_hashes[hsh]);
    }
  }
  return c;
}

int Ai::center(int x, int y)
{
  return (min(x-1, N-x)+min(y-1, N-y));
}

int Ai::sum_centers(position &p)
{
  int sum = 0;
  for(int i = 1; i <= N; i++)
    for(int j = 1; j <= N; j++)
      if(p.board[i][j])
        sum -= (p.board[i][j]*2-3)*center(i, j);
  return sum;
}

const int fours = 10000;
const int threes = 1000;
//                           -  oh o  d   tb  dh  d   th  t   f   fh  ff  five    o  tb
const int xcombo_scores[] = {0, 4, 8, 40, 40, 15, 30, 25, 50, 50, 25, 100, 100000, 0, 40};
// const int ocombo_scores[] = {0, 4, 8, 40, 40, 15, 30, 25, 50, 50, 25, 100, 100000, 0, 40};
const int ocombo_scores[] = {0, 4, 8, 30, 30, 10, 18, 15, 30, 40, 20, 80, 100000, 0, 30};

int Ai::hash_evaluation(position &p)
{
  long long hsh = p.get_hash();
  if(hash_map.count(hsh))
    return hash_map[hsh];
  combos c = get_combos(p);
  int score = sum_centers(p);
  if(p.to_move == 1)
  {
    swap(c.xcombos, c.ocombos);
    score = -score;
  }
  score += rand()%(abs(score/10)+1)*(rand()%3-1);
  for(int i = 0; i < combo_types; i++)
    score += c.xcombos[i]*xcombo_scores[i];
  for(int i = 0; i < combo_types; i++)
    score -= c.ocombos[i]*ocombo_scores[i];

  int xfours = 2*c.xcombos[11]+c.xcombos[9]+c.xcombos[10]/2;
  int xthrees = c.xcombos[8]+c.xcombos[7]/2;
  int ofours = 2*c.ocombos[11]+c.ocombos[9]+c.ocombos[10]/2;
  int othrees = c.ocombos[8]+c.ocombos[7]/2;
  if(ofours >= 1)
    score -= fours*ofours+threes*othrees;
  else if(xfours >= 2)
    score += fours*xfours+threes*xthrees;
  else if(xfours >= 1 && xthrees >= 1)
    score += fours*xfours+threes*xthrees;
  else if(othrees >= 1 && xfours == 0)
    score -= threes*othrees;
  else if(xthrees >= 2)
    score += threes*xthrees;
  if(p.to_move == 1)
    score *= -1;
  hash_map[hsh] = score;
  return score;
}

const int inf = 5e4;

eval_move Ai::blindspot(position &p)
{
  for(int i = 1; i <= N; i++)
  {
    for(int j = 1; j <= N; j++)
    {
      if(p.board[i][j])
        continue;
      p.play_move({i, j});
      int neweval = hash_evaluation(p);
      if(neweval*(p.to_move*2-3) > 1000)
      {
        p.undo();
        return {i, j, 1};
      }
      p.undo();
    }
  }
  return {0, 0, 0};
}

eval_move Ai::play_deep_smarter_ab_selective(position &p, int d, int s, int t = 1e9, int ab = -1)
{
  int best = p.to_move == 2 ? inf : -inf;
  if(ab == -1)
    ab = -best;
  if(get_time() > t)
    return {0, 0, 0};
  int p_eval = hash_evaluation(p);
  eval_move best_move = {0, 0, p_eval};
  if(d <= 0)
    return best_move;

  vector<eval_move> moves;
  for(int i = 1; i <= N; i++)
  {
    for(int j = 1; j <= N; j++)
    {
      if(p.board[i][j])
        continue;
      p.play_move({i, j});
      moves.push_back({i, j, hash_evaluation(p)});
      p.undo();
    }
  }

  sort(moves.begin(), moves.end(), p.to_move==2 ?
        [](eval_move a, eval_move b){return a.e < b.e;} :
        [](eval_move a, eval_move b){return a.e > b.e;});

  best_move = moves[0];
  if(d <= 1)
    return best_move;

  bool foundok = false;

  for(int i = 0; i < min((int)moves.size(), s); i++)
  {
    p.play_move(moves[i].m);
    int eval = play_deep_smarter_ab_selective(p, d-1, s, t, best).e;
    if((p.to_move == 2 ? eval > best : eval < best))
    {
      eval_move blind = blindspot(p);
      if(blind.e)
      {
        if(!foundok)
          best_move = {blind.m, eval};
      }
      else
      {
        foundok = true;
        best = eval;
        best_move = {moves[i].m, eval};
      }
    }
    p.undo();
    if(p.to_move == 1 ? best > ab : best < ab)
      return best_move;
  }
  return best_move;
}

eval_move Ai::play_deep_smarter_ab_selective_timed(position &p, int t, int s)
{
  int end_time = get_time()+t;
  int depth = 1;
  eval_move best_move = {0, 0, 0};
  while(get_time() < end_time)
  {
    eval_move m = play_deep_smarter_ab_selective(p, depth, s, end_time);
    if(get_time() < end_time)
      best_move = m;
    if(abs(best_move.e) > inf)
      return best_move;
    depth++;
  }
  return best_move;
}

eval_move Ai::hashed(position &p, int d, int s, int t = 1e9, int ab = -1)
{
  int best = p.to_move == 2 ? inf : -inf;
  if(ab == -1)
    ab = -best;
  if(get_time() > t)
    return {0, 0, 0};
  long long hsh = p.get_hash();
  if(hash_map_smarter[d].count(hsh))
    return hash_map_smarter[d][hsh];
  int p_eval = hash_evaluation(p);
  eval_move best_move = {0, 0, p_eval};
  if(d <= 0)
  {
    hash_map_smarter[d][hsh];
    return best_move;
  }

  vector<eval_move> moves;
  for(int i = 1; i <= N; i++)
  {
    for(int j = 1; j <= N; j++)
    {
      if(p.board[i][j])
        continue;
      p.play_move({i, j});
      moves.push_back({i, j, hash_evaluation(p)});
      p.undo();
    }
  }

  sort(moves.begin(), moves.end(), p.to_move==2 ?
        [](eval_move a, eval_move b){return a.e < b.e;} :
        [](eval_move a, eval_move b){return a.e > b.e;});

  best_move = moves[0];
  if(d <= 1)
  {
    hash_map_smarter[d][hsh] = best_move;
    return best_move;
  }

  bool foundok = false;

  for(int i = 0; i < min((int)moves.size(), s); i++)
  {
    p.play_move(moves[i].m);
    int eval = play_deep_smarter_ab_selective(p, d-1, s, t, best).e;
    if((p.to_move == 2 ? eval > best : eval < best))
    {
      eval_move blind = blindspot(p);
      if(blind.e)
      {
        if(!foundok)
          best_move = {blind.m, eval};
      }
      else
      {
        foundok = true;
        best = eval;
        best_move = {moves[i].m, eval};
      }
    }
    p.undo();
    if(p.to_move == 1 ? best > ab : best < ab)
      return best_move;
  }
  hash_map_smarter[d][hsh] = best_move;
  return best_move;
}

eval_move Ai::hashed_timed(position &p, int t, int s)
{
  int end_time = get_time()+t;
  int depth = 1;
  eval_move best_move;
  while(get_time() < end_time)
  {
    eval_move m = hashed(p, depth, s, end_time);
    if(get_time() < end_time)
      best_move = m;
    if(abs(best_move.e) > 1000)
      return best_move;
    depth++;
  }
  return best_move;
}

bool Ai::has_fours(combos &c, combos &old)
{
  int xfours = 2*c.xcombos[11]+c.xcombos[9]+c.xcombos[10]/2;
  int ofours = 2*c.ocombos[11]+c.ocombos[9]+c.ocombos[10]/2;
  int xfoursold = 2*old.xcombos[11]+old.xcombos[9]+old.xcombos[10]/2;
  int ofoursold = 2*old.ocombos[11]+old.ocombos[9]+old.ocombos[10]/2;
  return xfours != xfoursold || ofours != ofoursold;
}

eval_move Ai::only_fours(position &p, int d, int t = 1e9, int ab = -1)
{
  int best = p.to_move == 2 ? inf : -inf;
  if(ab == -1)
    ab = -best;
  if(get_time() > t)
    return {0, 0, 0};

  long long hsh = p.get_hash();
  if(hash_map_fours[d].count(hsh))
    return hash_map_fours[d][hsh];
  int p_eval = hash_evaluation(p);
  eval_move best_move = {0, 0, p_eval};
  if(d <= 0 || (p.to_move == 2 ? p_eval < -1000 : p_eval > 1000))
  {
    hash_map_fours[d][hsh] = best_move;
    return best_move;
  }
  if(p.to_move == 1 ? p_eval > ab : p_eval < ab)
      return best_move;

  combos oldc = get_combos(p);

  for(int i = 1; i <= N; i++)
  {
    for(int j = 1; j <= N; j++)
    {
      if(p.board[i][j])
        continue;
      p.play_move({i, j});
      combos c = get_combos(p);
      if(has_fours(c, oldc))
      {
        int eval = only_fours(p, d-1, t, best).e;
        if((p.to_move == 2 ? eval > best : eval < best))
        {
          best = eval;
          best_move = {i, j, eval};
        }
      }
      p.undo();
      if(p.to_move == 1 ? best > ab : best < ab)
        return best_move;
    }
  }
  hash_map_fours[d][hsh] = best_move;
  return best_move;
}


eval_move Ai::only_fours_timed(position &p, int t)
{
  int end_time = get_time()+t;
  int depth = 1;
  eval_move best_move = {0, 0, 0};
  while(get_time() < end_time)
  {
    eval_move m = only_fours(p, depth, end_time);
    if(get_time() < end_time)
      best_move = m;
    if(abs(best_move.e) > 1000 || depth > 100)
      return best_move;
    depth++;
  }
  return best_move;
}


bool Ai::has_threes(combos &c, combos &old)
{
  int xfours = 2*c.xcombos[11]+c.xcombos[9]+c.xcombos[10]/2;
  int ofours = 2*c.ocombos[11]+c.ocombos[9]+c.ocombos[10]/2;
  int xfoursold = 2*old.xcombos[11]+old.xcombos[9]+old.xcombos[10]/2;
  int ofoursold = 2*old.ocombos[11]+old.ocombos[9]+old.ocombos[10]/2;
  int xthrees = c.xcombos[8]+c.xcombos[7]/2;
  int xthreesold = old.xcombos[8]+old.xcombos[7]/2;
  int othrees = c.ocombos[8]+c.ocombos[7]/2;
  int othreesold = old.ocombos[8]+old.ocombos[7]/2;
  return xfours != xfoursold || ofours != ofoursold
      || xthrees != xthreesold || othrees != othreesold;
}

eval_move Ai::only_threes(position &p, int d, int t = 1e9, int ab = -1)
{
  int best = p.to_move == 2 ? inf : -inf;
  if(ab == -1)
    ab = -best;
  if(get_time() > t)
    return {0, 0, 0};

  long long hsh = p.get_hash();
  if(hash_map_threes[d].count(hsh))
    return hash_map_threes[d][hsh];
  int p_eval = hash_evaluation(p);
  eval_move best_move = {0, 0, p_eval};
  if(d <= 0 || (p.to_move == 2 ? p_eval < -1000 : p_eval > 1000))
  {
    hash_map_fours[d][hsh] = best_move;
    return best_move;
  }
  if(p.to_move == 1 ? p_eval > ab : p_eval < ab)
      return best_move;

  combos oldc = get_combos(p);

  for(int i = 1; i <= N; i++)
  {
    for(int j = 1; j <= N; j++)
    {
      if(p.board[i][j])
        continue;
      p.play_move({i, j});
      combos c = get_combos(p);
      if(has_threes(c, oldc))
      {
        int eval = only_threes(p, d-1, t, best).e;
        if((p.to_move == 2 ? eval > best : eval < best))
        {
          best = eval;
          best_move = {i, j, eval};
        }
      }
      p.undo();
      if(p.to_move == 1 ? best > ab : best < ab)
        return best_move;
    }
  }
  hash_map_threes[d][hsh] = best_move;
  return best_move;
}


eval_move Ai::only_threes_timed(position &p, int t)
{
  int end_time = get_time()+t;
  int depth = 1;
  eval_move best_move = {0, 0, 0};
  while(get_time() < end_time)
  {
    eval_move m = only_threes(p, depth, end_time);
    if(get_time() < end_time)
      best_move = m;
    if(abs(best_move.e) > 1000 || depth > 100)
      return best_move;
    depth++;
  }
  return best_move;
}

bool Ai::have_to_defend(position &p)
{
  combos c = get_combos(p);
  return c.xcombos[11]+c.xcombos[9]+c.xcombos[10]+c.xcombos[8]+c.xcombos[7];
}


my_move Ai::play(position &p)
{
  eval_move first = hashed_timed(p, 4*move_time/10, 12);
  eval_move second = only_fours_timed(p, 2*move_time/10);
  eval_move third = only_threes_timed(p, 2*move_time/10);
  if((p.to_move == 1 ? second.e > 1000 : second.e < -1000) && (second.m.x || second.m.y) && !(p.to_move == 1 ? first.e > 1000 : first.e < -1000))
    return second.m;
  if((p.to_move == 1 ? third.e > 1000 : third.e < -1000) && (third.m.x || third.m.y) && !(p.to_move == 1 ? first.e > 1000 : first.e < -1000))
    return third.m;
  p.play_move(first.m);
  eval_move problems = only_fours_timed(p, move_time/10);
  p.undo();
  if((p.to_move == 1 ? problems.e < -1000 : problems.e > 1000) && (problems.m.x || problems.m.y))
  {
    if(have_to_defend(p))
      return hashed(p, 1, 12).m;
    return problems.m;
  }
  p.play_move(first.m);
  problems = only_threes_timed(p, move_time/10);
  p.undo();
  if((p.to_move == 1 ? problems.e < -1000 : problems.e > 1000) && (problems.m.x || problems.m.y))
  {
    if(have_to_defend(p))
      return hashed(p, 1, 12).m;
    return problems.m;
  }
  if(p.to_move == 1 ? first.e < -1000 : first.e > 1000)
    return hashed(p, 1, 12).m;
  return first.m;
}
