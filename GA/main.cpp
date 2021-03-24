#include "load_data.h"

int main() {

  std::map<double, std::pair<double, double>> my_map = load_data();

  for (int i = 0; i < my_map.size(); ++i) {
   std:: cout << my_map[i].first << ' ' << my_map[i].second << std::endl;
  }
  std::srand(time(0));

  /*
  long x = 0, y = 0, id = 0, n = 0;
  std::map<double, std::pair<double, double>> my_map;


  freopen("../mona_1000.txt", "r", stdin);
  scanf("%ld", &n);
  for (auto i = 0; i < n; i++) {
    scanf("%ld %ld %ld", &id, &x, &y);
    my_map[id-1] = std::make_pair(x, y);
  }
*/

  std::vector<std::vector<long>> init;
  std::vector<long> best_gen;

  //initialise
  init = init_sol(my_map, best_gen);
  std::vector<std::vector<long>> new_gen = init;
  double best_size = total_cost(best_gen, my_map);

  //evaluate
  std::vector<std::vector<long>> parents;
  std::cout << best_size << std::endl;
  bool isBetter = true;

  while (isBetter) {
    isBetter = false;
    for (int i = 0 ; i < 1000; ++i) {
      // select parents (choose 2 best) Tournament selection
      parents = tournamentSelection(new_gen, my_map);
      // reproduction crossover
      new_gen.push_back(crossover(parents[0], parents[1], my_map));

      //TODO mutation
    }

    //evaluate - find best
    for (int i = 0; i < new_gen.size(); ++i){
      double cur_cost = total_cost(new_gen[i], my_map);
      if (cur_cost < best_size){
        best_size = cur_cost;
        best_gen = new_gen[i];
        isBetter = true;
      }
    }

    //TODO buid next generation (delete worst)

  }

  for (int i = 0; i < best_gen.size(); ++i)
    std::cout << best_gen[i] << " ";

  double mycost = total_cost(best_gen, my_map);
  std::cout << std::endl << mycost;


}