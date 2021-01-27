#include <iostream>
#include <map>
#include <vector>
const double lambda = 10;

void printSol(const std::vector<long> &solution, long n, const std::vector<std::vector<long long>> &matrix){
  for (long i = 0; i < n; ++i)
    std::cout << solution[i] << " ";
}

std::vector<long> randsolution(long n) {
  std::vector<long> solution(n);

  for (long i = 0; i < n; i++) {
    solution[i] = i;
  }

  return solution;
}

std::vector<std::vector<long>> getNeighbors(int n,
                                            const std::vector<long> &solution) {
  std::vector<std::vector<long>> result;

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      if (i != j) {
        auto tmp = solution;
        std::swap(tmp[i], tmp[j]);
        result.push_back(tmp);
      }
    }
  }

  return result;
}

double getLenght(const std::vector<long> &path, long n,
                 std::vector<std::vector<long long>> matrix) {
  double result = 0;

  for (size_t i = 0; i < n - 1; i++)
    result += matrix[path[i]][path[i + 1]];

  result += matrix[path[n - 1]][path[0]];

  return result;
}

double getPenalty(const std::vector<long> &path, long n, int **penalty) {
  double result = 0;

  for (size_t i = 0; i < n - 1; i++)
    result += penalty[path[i]][path[i + 1]];

  result += penalty[path[n - 1]][path[0]];

  return result * lambda;
}

std::vector<long> LS(int **penalty, long n,
                     const std::vector<std::vector<long long>> &matrix) {
  bool isThereBetter = true;

  int m_iterations = 0;
  int record = 10000000;
  std::vector<long> solution = randsolution(n);

  while (isThereBetter) {
    m_iterations++;
    auto neighbors = getNeighbors(n, solution);

    isThereBetter = false;
    for (size_t i = 0; i < neighbors.size(); i++) {
      int lenght = 10000000;
      if ((lenght = getLenght(neighbors[i], n, matrix) +
          getPenalty(neighbors[i], n, penalty)) < record) {
        record = lenght;
        solution = neighbors[i];
        isThereBetter = true;
        std::cout << "New record: " << record << std::endl;
        break;
      }
    }
  }
  return solution;
}

std::vector<long> gls(long n,
                      const std::vector<std::vector<long long>> &matrix) {
  int k = 0;
  std::vector<long> solution = randsolution(n);

  int **penaltysMatrix = new int *[n];
  for (int i = 0; i < n; i++) {
    penaltysMatrix[i] = new int[n];
  }
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; ++j) {
      penaltysMatrix[i][j] = 0;
    }
  int iter = 10, i = 0;

  solution = LS(penaltysMatrix, n, matrix);
  return solution;
}

long get_dist(std::pair<long, long> city1, std::pair<long, long> city2) {
  long dx = pow((long)(city1.first - city2.first), 2);
  long dy = pow((long)(city1.second - city2.second), 2);
  return (floor((long)(sqrt(dx + dy))));
}

int main() {
  long x = 0, y = 0, id = 0, n = 0;
  std::map<long, std::pair<long, long>> my_map;

  freopen("../data.txt", "r", stdin);
  scanf("%ld", &n);
  for (auto i = 0; i < n; i++) {
    scanf("%ld %ld %ld", &id, &x, &y);
    my_map[id] = std::make_pair(x, y);
  }

  // std::pair<long, long> p = my_map[3];
  // std::cout << p.first << ' ' << p.second;

  std::vector<std::vector<long long>> matrix(n);
  for (int i = 0; i <= n; ++i) {
    matrix.push_back(std::vector<long long>());
    for (int j = 0; j < n; ++j) {
      if (i == j) {
        matrix[i].push_back(0);
      } else {
        matrix[i].push_back(get_dist(my_map[i], my_map[j]));
      }
    }
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }

  std::vector<long> solution = gls(n, matrix);
  for (int i = 0; i < n; ++i)
    std::cout << solution[i] << " ";
  return 0;
}