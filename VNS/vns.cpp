#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
int n_ones;
int m;
int p;
double bestGE;

//  GE-index
double GE_index(const vector<int> &machines, const vector<int> &details,
                const vector<vector<int>> &matrix) {
  double res = 0;
  int n_ones_in = 0;
  int n_zeros_in = 0;
  for (int i = 0; i < machines.size(); i++) {
    for (int j = 0; j < details.size(); j++) {
      if (machines[i] == details[j]) {
        if (matrix[i][j] == 0) {
          n_zeros_in++;
        } else {
          n_ones_in++;
        }
      }
    }
  }
  res = (double)n_ones_in / (n_ones + n_zeros_in);
  return res;
}
//  shaking
vector<int> divide();
vector<int> merge();
vector<int> shake();

//  exploration of neighborhood
vector<int> move_cols(vector<int> &details, vector<int> &machines,
                      vector<vector<int>> &matrix, int clusters) {
  vector<int> details_new = details;
  double GE = bestGE;
  int tmp_cl = 0;

  for (int i = 0; i < details.size(); i++) {
    for (int j = 1; j <= clusters; j++) {
      if (j != details[i]) {
        tmp_cl = details[i];
        details_new[i] = j;

        GE = GE_index(machines, details_new, matrix);
        if (GE >= bestGE) {
          bestGE = GE;
          return details_new;
        } else
          details_new[i] = tmp_cl;
      }
    }
  }

  return details;
}

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

vector<int> move_rows(vector<int> &machines, vector<int> &details,
                      vector<vector<int>> &matrix, int clusters) {
  vector<int> machines_new = machines;
  double GE = bestGE;
  int tmp_cl = 0;

  for (int i = 0; i < machines.size(); i++) {
    for (int j = 1; j <= clusters; j++) {
      if (j != machines[i]) {
        tmp_cl = machines[i];
        machines_new[i] = j;

        GE = GE_index(machines_new, details, matrix);
        if (GE > bestGE and all_clusters(machines_new, clusters)) {
          bestGE = GE;
          return machines_new;
        } else
          machines_new[i] = tmp_cl;
      }
    }
  }

  return machines;
}

vector<int> LS(vector<int> &machines, vector<int> &details,
               vector<vector<int>> &matrix, int clusters) {
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
      machines_new = move_rows(machines_new, details_new, matrix, clusters);
      details_new = move_cols(details_new, machines_new, matrix, clusters);
      if (cur_GE < bestGE) {
        cur_GE = bestGE;
        better_rows = true;
        better_sol = true;
        better_cols = true;
      }
    }

    while (better_cols) {
      better_cols = false;
      details_new = move_cols(details_new, machines_new, matrix, clusters);
      if (cur_GE < bestGE) {
        cur_GE = bestGE;
        better_cols = true;
        better_sol = true;
        better_rows = true;
      }
    }
  }

  cout << "Answer: ______________________" << endl;
  for (int i = 0; i < machines_new.size(); i++) {
    cout << machines_new[i] << ' ';
  }
  cout << endl;
  for (int i = 0; i < details_new.size(); i++) {
    cout << details_new[i] << ' ';
  }
  cout << endl;
  cout << GE_index(machines_new, details_new, matrix) << endl;
  cout << "Answer: ______________________" << endl;

  return machines_new;
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

std::vector<std::vector<int>> loadData() {
  ifstream in;                 // поток для записи
  in.open("../test_king.txt"); // окрываем файл для записи
  m = 0;
  p = 0;
  if (!in.is_open()) {
    cout << "cant open file";
  }
  in >> m >> p;
  vector<vector<int>> matrix(m, vector<int>(p, 0));
  string buffer;
  while (getline(in, buffer)) {
    vector<int> numbers = {};
    stringstream stream(buffer);
    while (stream) {
      int tmp;
      stream >> tmp;
      numbers.push_back(tmp);
    }
    int j = numbers[0] - 1;
    for (int i = 1; i < numbers.size(); i++) {
      matrix[j][numbers[i] - 1] = 1;
    }
    numbers.clear();
  }
  in.close();

  for (auto &i : matrix) {
    for (int j : i) {
      cout << j << ' ';
    }
    cout << endl;
  }
  n_ones = 0;
  for (auto &i : matrix) {
    for (int j : i) {
      if (j == 1) {
        n_ones++;
      }
    }
  }
  int n_zeros = m * p - n_ones;

  return matrix;
}
