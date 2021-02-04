#include "iterLS.h"

#define MAX_ITER 10000
#define MAX 10000000

std::vector<int> greedy(std::vector<std::vector<double>> matrix) {
  std::vector<int> used = {};
  for (int i = 1; i <= matrix.size(); i++) {
    used.push_back(0);
  }

  int startid = rand() % used.size();
  int n = used.size();
  int k = 1;
  std::vector<int> solution = {};

  solution.push_back(startid + 1);
  used[startid] = 1;

  int curid = startid;
  int min = MAX;
  int next = 0;

  while (k < matrix.size()) {
    for (int i = 0; i < n; i++) {
      if (matrix[curid][i] < min && used[i] == 0) {
        min = matrix[curid][i];
        next = i;
      }
    }
    solution.push_back(next + 1);
    min = MAX;
    used[next] = 1;
    curid = next;
    k++;
  }

  return solution;
}

std::vector<int>
iterlocalSearch(const std::vector<std::vector<double>> &matrix) {
  // generate initial solution
  std::vector<int> solution = greedy(matrix);
  double solution_cost = costCount(solution, matrix);
  std::cout << "init cost " << solution_cost << std::endl;

  // local search from initial
  std::vector<int> best = localSearch(solution, matrix);
  solution_cost = costCount(best, matrix);
  std::cout << "after 1 ls " << solution_cost << std::endl;
  if (costCount(solution, matrix) < costCount(best, matrix)) {
    best = solution;
  }

  int i = 0;
  while (i < MAX_ITER) {
    i++;
    // perturbation
    std::vector<int> newsolution = perturbation(best);
    // ls
    std::vector<int> newsolution2 = localSearch(newsolution, matrix);
    // acceptance criteria
    if (costCount(best, matrix) > costCount(newsolution2, matrix)) {
      best = newsolution2;
      std::cout << costCount(newsolution2, matrix) << std::endl;
    }
  }

  return best;
}

std::vector<int> localSearch(const std::vector<int> &solution,
                             const std::vector<std::vector<double>> &matrix) {
  int n = solution.size();
  std::vector<int> newsolution(n);
  std::vector<int> currentsolution(n);
  newsolution = solution;
  int flag = 0;
  int k = 0;
  double bestdistance = costCount(newsolution, matrix);
  double currentdistance = 0;

  while (flag == 0) { // repeat until we can change something
    flag = 1;
    k++;
    currentsolution = newsolution;
    currentsolution = two_opt(currentsolution, matrix);
    currentdistance = costCount(currentsolution, matrix);
    if (currentdistance < bestdistance) {
      newsolution = currentsolution;
      bestdistance = currentdistance;
      std::cout << "upgrade " << bestdistance << std::endl;
      flag = 0;
    }
  }

  return newsolution;
}

std::vector<int> two_opt(std::vector<int> path,
                         std::vector<std::vector<double>> matrix) {
  std::vector<int> copy = path;

  for (int i = 1; i < path.size() - 1; i++) {
    for (int j = i + 3; j < path.size(); j++) {
      if (i != j) {
        int x1 = path[i] - 1;
        int x2 = path[i + 1] - 1;

        int y1 = path[j - 1] - 1;
        int y2 = path[j] - 1;

        double size1 = matrix[x1][x2] + matrix[y1][y2];
        double size2 = matrix[x1][y1] + matrix[x2][y2];

        if (size1 > size2) {
          copy = two_optmove(path, i, j);
          if (costCount(copy, matrix) < costCount(path, matrix)) {
            goto end; // if find better solution - return
          }
        }
      }
    }
  }

  end:
  return copy;
}


std::vector<int> two_optmove(std::vector<int> path, int i, int j) {
  std::vector<int> solution;
  solution.insert(solution.end(), path.begin(), path.begin() + i);

  std::vector<int> middlePart(path.begin() + i, path.begin() + j);
  std::reverse(middlePart.begin(), middlePart.end());

  solution.insert(solution.end(), middlePart.begin(), middlePart.end());
  solution.insert(solution.end(), path.begin() + j, path.end());

  return solution;
}

std::vector<int>
perturbation(std::vector<int> solution) { // 4-opt double bridge algorithm
  std::vector<int> copy = solution;
  std::vector<int> newsolution = solution; // size at least 8
  std::vector<int> index;

  for (int i = 0; i < 4; i++) {
    int pos = rand() % copy.size();
    while (pos == copy.size() - 1 || vectorContanes(index, pos) ||
           vectorContanes(index, pos + 1)) {
      pos = rand() % copy.size();
    }
    index.push_back(pos);
    index.push_back(pos + 1);
  }

  // sort!
  sort(index.begin(), index.end());
  int a1 = index[0];
  int a2 = index[1];
  int b1 = index[2];
  int b2 = index[3];
  int c1 = index[4];
  int c2 = index[5];
  int d1 = index[6];
  int d2 = index[7];

  int i = 0;
  int dest = 0;
  for (int j = dest; j < a2; j++) {
    newsolution[j] = solution[i++];
  }
  i = c2;
  dest += a2;
  for (int j = dest; j < d1 - c2 + 1 + dest; j++) {
    newsolution[j] = solution[i++];
  }
  dest += d1 - c2 + 1;
  i = b2;
  for (int j = dest; j < c1 - b2 + 1 + dest; j++) {
    newsolution[j] = solution[i++];
  }
  i = a2;
  dest += c1 - b2 + 1;
  for (int j = dest; j < b1 - a2 + 1 + dest; j++) {
    newsolution[j] = solution[i++];
  }
  i = d2;
  dest += b1 - a2 + 1;
  for (int j = dest; j < solution.size() - d2; j++) {
    newsolution[j] = solution[i++];
  }

  return newsolution;
}

double costCount(std::vector<int> solution,
                 std::vector<std::vector<double>> matrix) {
  double cost = 0;

  for (int i = 0; i < solution.size() - 1; i++) {
    cost += matrix[solution[i] - 1][solution[i + 1] - 1];
  }
  cost += matrix[solution.back() - 1][solution.front() - 1];

  return cost;
}

int vectorContanes(std::vector<int> index, int elem) {
  for (int i = 0; i < index.size(); i++) {
    if (index[i] == elem) {
      return 1;
    }
  }
  return 0;
}

double get_dist(std::pair<long, long> city1, std::pair<long, long> city2) {
  long dx = pow((double)(city1.first - city2.first), 2);
  long dy = pow((double)(city1.second - city2.second), 2);

  return (round((double)((sqrt(dx + dy)) * 100))) / 100;
}

std::vector<std::vector<double>> loadData() {
  //  input data
  long x = 0, y = 0, id = 0, n = 0;
  std::map<long, std::pair<long, long>> my_map;
  std::srand(time(0));

  freopen("../mona_1000.txt", "r", stdin);
  scanf("%ld", &n);
  for (auto i = 0; i < n; i++) {
    scanf("%ld %ld %ld", &id, &x, &y);
    my_map[id] = std::make_pair(x, y);
  }

  std::vector<std::vector<double>> matrix(n, std::vector<double>(n, 0));
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      if (i == j) {
        matrix[i - 1][j - 1] = 0;
      } else {
        matrix[i - 1][j - 1] = get_dist(my_map[i], my_map[j]);
      }
    }
  }

  return matrix;
}