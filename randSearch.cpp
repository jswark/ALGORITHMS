#include <iostream>
#include <map>
#include <vector>

long get_dist(std::pair<long, long> city1, std::pair<long, long> city2) {
  long dx = pow((long)(city1.first - city2.first), 2);
  long dy = pow((long)(city1.second - city2.second), 2);
  return (floor((long)(sqrt(dx + dy))));
}

long cost(const std::vector<long> &path, long n,
          std::vector<std::vector<long long>> matrix) {
  double result = 0;

  for (size_t i = 0; i < n - 1; i++)
    result += matrix[path[i]][path[i + 1]];

  result += matrix[path[n]][path[0]];

  return result;
}

std::vector<long> randSol(int n, std::vector<long> &solution) {
  std::vector<long> mask (n + 1);
  for (int i = 0; i <= n; i++) {
    mask.push_back(0);
  }

  long randCity = rand()%n;
  for (int i = 0; i < n; i++) {
    while (mask[randCity] != 0)
      randCity = rand()%n;
    solution[i] = randCity;
    mask[randCity] = 1; //used
  }

  solution[n] = solution[0];

  return solution;
}

std::vector<long> randSearch(int iter, int n, const std::vector<std::vector<long long>>& matrix) {
  std::vector<long> solution (n + 1);
  std::vector<long> bestsolution (n + 1);
  long maxCost = 10000000000000;
  for (int i = 0; i < iter; i++) {
    auto candidate = randSol(n, (std::vector<long> &)solution);

    if (cost(candidate, n, matrix) < maxCost) {
      bestsolution = candidate;
      maxCost = cost(candidate, n, matrix);
    }

  }

  return bestsolution;
}

void printSol(const std::vector<long> &solution, long n) {
  for (long i = 0; i <= n; ++i)
    std::cout << solution[i] << " ";
  std::cout << std::endl;
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

  std::vector<long> solution = randSearch(10, n, matrix);

  for (int i = 0; i <= n; ++i)
    std::cout << solution[i] << " ";
  return 0;
}