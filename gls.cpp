#include <iostream>
#include <map>
#include <vector>
const int lambda = 10;

long cost(const std::vector<long> &path, long n,
          std::vector<std::vector<long long>> matrix) {
  long result = 0;

  for (size_t i = 0; i < n - 1; i++)
    result += matrix[path[i]][path[i + 1]];

  result += matrix[path[n]][path[0]];

  return result;
}

std::vector<long> randSol(long n, std::vector<long> &solution) {
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

void printSol(const std::vector<long> &solution, long n) {
  for (long i = 0; i <= n; ++i)
    std::cout << solution[i] << " ";
  std::cout << std::endl;
}

std::vector<long> two_opt(std::vector<long> vect, const long i,
                          const long j) {

  std::vector<long> newVect;
  newVect.insert(newVect.end(), vect.begin(), vect.begin() + i);

  std::vector<long> middlePart(vect.begin() + i, vect.begin() + j);
  std::reverse(middlePart.begin(), middlePart.end());

  newVect.insert(newVect.end(), middlePart.begin(), middlePart.end());
  newVect.insert(newVect.end(), vect.begin() + j, vect.end());

  return newVect;
}

std::vector<std::vector<long>> shuffle(long n,
                                            const std::vector<long> &solution) {
  std::vector<std::vector<long>> result;

  int i = 0;
  for (size_t j = 0; j <= n; j++) {
    if (i == 0) {
      std::vector<long> tmp = solution;
      tmp = two_opt(tmp, i, n);
      result.push_back(tmp);
      i++;
    }
    else if (j == (n - 1)) {
      std::vector<long> tmp = solution;
      tmp = two_opt(tmp, 0, j);
      result.push_back(tmp);
    }

    else {
      std::vector<long> tmp = solution;
      tmp = two_opt(tmp, i, j);
      result.push_back(tmp);
    }
  }

  return result;
}

long getPenalty(const std::vector<long> &path, long n, const std::vector<std::vector<long long>> &fine) {
  long result = 0;

  for (size_t i = 0; i < n - 1; i++)
    result += fine[path[i]][path[i + 1]];

  result += fine[path[n]][path[0]];

  return result * lambda;
}

std::vector<long> LS(const std::vector<std::vector<long long>> &fine, long n,
                     const std::vector<std::vector<long long>> &matrix) {
  bool isThereBetter = true;
  long maxCost = 10000000;
  long record = 10000000;
  std::vector<long> solution (n + 1);
  solution = randSol(n, solution);

  while (isThereBetter) {
    auto new_sol = shuffle(n, solution);

    isThereBetter = false;
    for (size_t i = 0; i < new_sol.size(); i++) {
      if ((maxCost = cost(new_sol[i], n, matrix) /*+
                    getPenalty(neighbors[i], n, fine)*/) < record) {
        record = maxCost;
        solution = new_sol[i];
        isThereBetter = true;
        //TODO giveFine
        break;
      }
    }
  }

  return solution;
}

std::vector<long> gls(long n,
                      const std::vector<std::vector<long long>> &matrix) {

  std::vector<long> solution (n + 1);
  solution = randSol(n, solution);

  std::vector<std::vector<long long>> fine(n);
  for (int i = 0; i <= n; ++i) {
    fine.push_back(std::vector<long long>());
    for (int j = 0; j < n; ++j) {
      if (i == j) {
        fine[i].push_back(0);
      } else {
        fine[i].push_back(0);
      }
    }
  }
  
  solution = LS(fine, n, matrix);
  printSol(solution, n);
  solution = LS(fine, n, matrix);
  printSol(solution, n);
  solution = LS(fine, n, matrix);
  printSol(solution, n);
  solution = LS(fine, n, matrix);
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

  for (int i = 0; i <= n; ++i)
    std::cout << solution[i] << " ";
  return 0;
}