#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <random>

using namespace std;
int n_ones;
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
void shaking(vector<int> &details, vector<int> &machines, int &clusters){
  int det = rand()%details.size();
  int mach = rand()%machines.size();
  int cl = rand()%(clusters+1)+1;
  if(cl == clusters+1){
    clusters++;
  }
  details[det]=cl;
  machines[mach]=cl;
  cout<<"After shake:"<<endl;
  for (int i = 0; i < machines.size(); i++) {
    cout << machines[i] << ' ';
  }
  cout << endl;
  for (int i = 0; i < details.size(); i++) {
    cout << details[i] << ' ';
  }
  cout << endl;
}

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
        if (GE > bestGE && all_clusters(machines_new, clusters)) {
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

int main() {
  std::srand(time(0));
  //  (1) parsing
  ifstream in;                 // поток для записи
  in.open("../test_king.txt"); // окрываем файл для записи
  int m = 0;
  int p = 0;
  if (!in.is_open()) {
    return -1;
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
  bestGE = 0;

  //  (2) generate initial solution
  int clusters = 2;
  vector<int> machines(m);
  vector<int> details(p);
  machines = rand_sol(machines, clusters);
  details = rand_sol(details, clusters);
  // machines = {2,1, 1, 2, 1};
  // details = {1, 2, 1, 1, 2, 1, 1};
  bestGE = GE_index(machines, details, matrix);
  cout << endl
       << "init GE: " << GE_index(machines, details, matrix) << endl
       << "init machines " << endl;
  for (int i = 0; i < m; i++) {
    cout << machines[i] << ' ';
  }
  cout << endl << "init details " << endl;
  for (int i = 0; i < p; i++) {
    cout << details[i] << ' ';
  }
  cout << endl;

  //  (3) general vns
  int k=0;
  while(k<3) {
    k++;
    shaking(details, machines, clusters);
    machines = LS(machines, details, matrix, clusters);
  }
 /* int counter = 0;
  int k = 0;
  while (counter < 2) {
    counter++;
    k = 1;
    //    while(k!=k_max)
  }*/

  //  (4) print answer
 /* cout << "Answer:" << endl;
  for (int i = 0; i < m; i++) {
    cout << machines[i] << ' ';
  }
  cout << endl;
  for (int i = 0; i < p; i++) {
    cout << details[i] << ' ';
  }
  cout << endl;
  cout << GE_index(machines, details, matrix) << endl;

  cout << endl;*/
}