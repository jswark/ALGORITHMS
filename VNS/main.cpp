#include "vns.cpp"

int main() {
  std::srand(time(0));
  //  (1) parsing
  vector<vector<int>> matrix = loadData();
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
  vector<int> new_machines(m);
  new_machines = LS(machines, details, matrix, clusters);
  //место для вашего шейка

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