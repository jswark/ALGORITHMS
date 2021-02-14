#include "load_data.h"
#include "ls.h"
#include "shaking.h"
#include <iostream>
#include <ctime>
#include <vector>

using namespace  std;

int main() {
  std::srand(time(0));
  //  (1) parsing
  int m = 0;
  int p =0;
  int n_ones=0;
  double bestGE = 0;
  vector<vector<int>> matrix = loadData(m,p, n_ones);

  //  (2) generate initial solution
  int clusters = 2;
  vector<int> machines(m);
  vector<int> details(p);
  machines = rand_sol(machines, clusters);
  details = rand_sol(details, clusters);
  while (valid_clusters(machines, details, matrix, clusters) == 0) {
    machines = rand_sol(machines, clusters);
    details = rand_sol(details, clusters);
  }
  bestGE = GE_index(machines, details, matrix, n_ones);
  cout << endl
       << "init GE: " << GE_index(machines, details, matrix, n_ones) << endl
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
  vector<int> best_machines = machines;
  vector<int> best_details = details;
  double cur_best = GE_index(best_machines, best_details, matrix, n_ones);
  double new_best = 0;
  LS(machines, details, matrix, clusters, n_ones, bestGE);
  new_best = GE_index(machines, details, matrix, n_ones);
  if (new_best > cur_best) {
    cur_best = new_best;
    best_machines = machines;
    best_details = details;
  } else {
    machines = best_machines;
    details = best_details;
  }

  bool flagbetter = true;
  while (flagbetter) {
    flagbetter = false;
    bool flagsp = true;
    bool flagme = true;
    vector<int> tmp_machines = best_machines;
    vector<int> tmp_details = best_details;
    for (int i = 0; (i < 100) && flagsp; i++) {
      details = tmp_details;
      machines = tmp_machines;
      split(details, machines, matrix);
      clusters = count_clusters(details, machines);
      LS(machines, details, matrix, clusters, n_ones, bestGE);
      new_best = GE_index(machines, details, matrix, n_ones);
      if (new_best > cur_best) {
        cur_best = new_best;
        best_machines = machines;
        best_details = details;
        flagsp = false;
        flagme = true;
        flagbetter = true;
      }
    }

    tmp_details = best_details;
    tmp_machines = best_machines;
    clusters = count_clusters(tmp_details, tmp_machines);

    if (flagme) {
      for (int i = 0; (i < 100) && flagme && clusters > 2; i++) {
        details = tmp_details;
        machines = tmp_machines;
        merge(details, machines, matrix);
        clusters = count_clusters(details, machines);
        LS(machines, details, matrix, clusters, n_ones, bestGE);
        new_best = GE_index(machines, details, matrix, n_ones);
        if (new_best > cur_best) {
          cur_best = new_best;
          best_machines = machines;
          best_details = details;
          flagme = false;
          flagbetter = true;
          flagsp = true;
        }
      }
    }
  }
  //  (4) print answer
  cout << "Answer:" << endl;
  for (int i = 0; i < m; i++) {
    cout << best_machines[i] << ' ';
  }
  cout << endl;
  for (int i = 0; i < p; i++) {
    cout << best_details[i] << ' ';
  }
  cout << endl;
  cout << GE_index(best_machines, best_details, matrix, n_ones) << endl;

  cout << endl;
}