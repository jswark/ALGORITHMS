#include "ls.h"
#include "shaking.h"
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

bool all_clusters(vector<int> check, int clusters) {
  vector<int> cl(clusters);
  for (int i = 1; i <= clusters; i++)
    cl[i - 1] = i;

  for (int i = 0; i < check.size(); i++) {
    for (int j = 0; j < cl.size(); j++) {
      if (check[i] == cl[j])
        cl.erase(cl.begin() + j);
    }
  }

  if (cl.empty())
    return true;

  return false;
}

vector<int> move_cols(vector<int> &details, vector<int> &machines,
                      vector<vector<int>> &matrix, int clusters, int n_ones, double bestGE) {
  vector<int> details_new = details;
  vector<int> best_move = details;
  double GE = bestGE;
  int tmp_cl = 0;

  for (int i = 0; i < details.size(); i++) {
    for (int j = 1; j <= clusters; j++) {
      if (j != details[i]) {
        tmp_cl = details[i];
        details_new[i] = j;

        GE = GE_index(machines, details_new, matrix, n_ones);
        if (GE >= GE_index(machines, best_move, matrix, n_ones) &&
            all_clusters(details_new, clusters)) {
          best_move = details_new;
          details_new[i] = tmp_cl;
        } else
          details_new[i] = tmp_cl;
      }
    }
  }

  if (bestGE < GE_index(machines, best_move, matrix, n_ones)) {
    bestGE = GE_index(machines, best_move, matrix, n_ones);
  }

  return best_move;
}

vector<int> move_rows(vector<int> &machines, vector<int> &details,
                      vector<vector<int>> &matrix, int clusters, int n_ones, double  bestGE) {
  vector<int> machines_new = machines;
  vector<int> best_move = machines;
  double GE = bestGE;
  int tmp_cl = 0;

  for (int i = 0; i < machines.size(); i++) {
    for (int j = 1; j <= clusters; j++) {
      if (j != machines[i]) {
        tmp_cl = machines[i];
        machines_new[i] = j;

        GE = GE_index(machines_new, details, matrix, n_ones);
        if (GE >= GE_index(best_move, details, matrix, n_ones) &&
            all_clusters(machines_new, clusters)) {
          best_move = machines_new;
          machines_new[i] = tmp_cl;
          // return machines_new;
        } else
          machines_new[i] = tmp_cl;
      }
    }
  }

  if (bestGE < GE_index(best_move, details, matrix, n_ones)) {
    bestGE = GE_index(best_move, details, matrix, n_ones);
  }

  return best_move;
}

void LS(vector<int> &machines, vector<int> &details,
        vector<vector<int>> &matrix, int &clusters, int n_ones, double bestGE) {
  vector<int> machines_new = machines;
  vector<int> details_new = details;
  double cur_GE = bestGE;
  bool better_sol = true;
  bool better_rows = true;
  bool better_cols = true;
  while (better_sol) {
    better_sol = false;
    while (better_rows) {
      better_rows = false;
      machines_new = move_rows(machines_new, details_new, matrix, clusters, n_ones, bestGE);
      // details_new = move_cols(details_new, machines_new, matrix, clusters);
      if (cur_GE < bestGE) {
        cur_GE = bestGE;
        better_rows = true;
        better_sol = true;
        better_cols = true;
      }
    }

    while (better_cols) {
      better_cols = false;
      details_new = move_cols(details_new, machines_new, matrix, clusters, n_ones, bestGE);
      if (cur_GE < bestGE) {
        cur_GE = bestGE;
        better_cols = true;
        better_sol = true;
        better_rows = true;
      }
    }
  }


  machines = machines_new;
  details = details_new;


  cout << "LS Answer: ______________________" << endl;
  for (int i = 0; i < machines_new.size(); i++) {
    cout << machines[i] << ' ';
  }
  cout << endl;
  for (int i = 0; i < details_new.size(); i++) {
    cout << details[i] << ' ';
  }
  cout << endl;
  cout << GE_index(machines, details, matrix, n_ones) << endl;
  cout << " ______________________" << endl;
}

vector<int> rand_sol(vector<int> &for_rand, int clusters) {
  vector<int> filled = for_rand;
  while (!all_clusters(filled, clusters)) {
    filled = for_rand;
    for (int i = 0; i < for_rand.size(); i++)
      filled[i] = rand() % clusters + 1;
  }
  return filled;
}