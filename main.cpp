#include "iterLS.h"

int main() {

  std::vector<std::vector<double>> matrixCost = loadData();
  std::vector<int> best = iterlocalSearch(matrixCost);

  std::cout << "Answer: " << std::endl;
  for (int i = 0; i < best.size(); i++) {
    std::cout << best[i] << ' ';
  }

  double solution_cost = costCount(best, matrixCost);
  std::cout << std::endl << solution_cost << std::endl;
  std::cout << best.size() << std::endl;
}
