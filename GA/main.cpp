#include "load_data.h"

int main() {

  std::vector<std::vector<double>> matrix = load_data();

  for (int i = 0; i < matrix.size(); ++i) {
    for (int j = 0; j < matrix.size(); ++j) {
      std::cout << matrix[i][j] << " ";
    }
  }

}