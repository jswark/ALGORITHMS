#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <vector>
using namespace std;

double costCount(vector<int> solution, vector<vector<double>> matrix) {
  double sum = 0;
  for (int i = 0; i < solution.size() - 1; i++) {
    sum += matrix[solution[i] - 1][solution[i + 1] - 1];
  }
  sum += matrix[solution.back() - 1][solution.front() - 1];
  return sum;
}
std::vector<int> trytwo_opt(std::vector<int> vect, int i, int j) {
  std::vector<int> newVect;
  newVect.insert(newVect.end(), vect.begin(), vect.begin() + i);

  std::vector<int> middlePart(vect.begin() + i, vect.begin() + j);
  std::reverse(middlePart.begin(), middlePart.end());

  newVect.insert(newVect.end(), middlePart.begin(), middlePart.end());
  newVect.insert(newVect.end(), vect.begin() + j, vect.end());

  return newVect;
}

std::vector<int> two_opt(std::vector<int> vect, vector<vector<double>> matrix) {
  vector<int> copy = vect;
  bool better = 0;
  int i = 0, j = 0;

  for (i = 1; i < vect.size(); i++) {
    for (j = i + 3; j < vect.size(); j++) {
      if (i != j) {
        int x1 = vect[i] - 1;
        int x2 = vect[i + 1] - 1;

        int y1 = vect[j-1] - 1;
        int y2 = vect[j] - 1;

        double size1 = matrix[x1][x2] + matrix[y1][y2];
        double size2 = matrix[x1][y1] + matrix[x2][y2];

          if (size1 > size2) {
          copy = trytwo_opt(vect, i, j);
          if (costCount(copy, matrix) < costCount(vect, matrix)) {
            goto end;
          }
        }
         // goto end;
       // }
      }
    }
  }


  end:

  //if (better == 0)
   // return vect;

  return copy;
}

vector<int> randomSearch(vector<vector<double>> matrix) {
  // generate node list
  vector<int> nodes = {};
  for (int i = 1; i <= matrix.size(); i++) {
    nodes.push_back(i);
  }
  int n = nodes.size();
  vector<int> solution = {};
  for (int i = 0; i < n; i++) {
    int k = rand() % nodes.size();
    solution.push_back(nodes[k]);
    nodes.erase(nodes.begin() + k);
  }
  return solution;
}

int getMin(int i, vector<vector<double>> matrix) {
  int k = 0;
  int curid = i;
  int min = matrix[curid][1];
  int minid = 1;

  for (int i = 0; i < matrix.size(); i++) {
    if (matrix[curid][i] < min && i != curid) {
      min = matrix[curid][i];
      minid = i;
    }
  }

  return minid;
}

vector<int> Greedy(vector<vector<double>> matrix) {
  // generate node list
  vector<int> mask = {};
  for (int i = 1; i <= matrix.size(); i++) {
    mask.push_back(0);
  }

  int startid = rand() % matrix.size();
  int n = mask.size();
  int k = 1;
  vector<int> solution = {};

  solution.push_back(startid + 1);
  mask[startid] = 1;

  int curid = startid;
  int min = 100000000000;
  int next = 0;

  while (k < matrix.size()) {
    for (int i = 0; i < n; i++) {
      if (matrix[curid][i] < min && mask[i] == 0) {
        min = matrix[curid][i];
        next = i;
      }
    }
    solution.push_back(next + 1);
    min = 100000000000;
    mask[next] = 1;
    curid = next;
    k++;
  }

  return solution;
}

vector<int> localSearch(vector<int> solution,
                        vector<vector<double>> matrix) { // 2-opt algorithm
  int n = solution.size();
  vector<int> newsolution(n);
  vector<int> currentsolution(n);
  newsolution = solution;
  int flag = 0;
  double bestdistance = costCount(newsolution, matrix);
  int k = 0;
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
        std::cout << "better " << bestdistance << endl;
        flag = 0;
    }
  }
  return newsolution;
}

int vectorContanes(vector<int> index, int elem) {
  for (int i = 0; i < index.size(); i++) {
    if (index[i] == elem) {
      return 1;
    }
  }
  return 0;
}

vector<int>
perturbation(vector<int> solution) { // 4-opt double bridge algorithm
  vector<int> copy = solution;
  vector<int> newsolution = solution; // size at least 8
  vector<int> index;
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

double get_dist(pair<long, long> city1, pair<long, long> city2) {
  long dx = pow((double)(city1.first - city2.first), 2);
  long dy = pow((double)(city1.second - city2.second), 2);
  return (round((double)((sqrt(dx + dy)) * 100))) / 100;
}

int main() {
  //  input data
  long x = 0, y = 0, id = 0, n = 0;
  map<long, pair<long, long>> my_map;
  std::srand(time(0));

  freopen("../lu980.txt", "r", stdin);
  scanf("%ld", &n);
  for (auto i = 0; i < n; i++) {
    scanf("%ld %ld %ld", &id, &x, &y);
    my_map[id] = make_pair(x, y);
  }
  vector<vector<double>> matrix(n, vector<double>(n, 0));
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      if (i == j) {
        matrix[i - 1][j - 1] = 0;
      } else {
        matrix[i - 1][j - 1] = get_dist(my_map[i], my_map[j]);
      }
    }
  }

  /*for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix.size(); j++) {
      cout << matrix[i][j] << ' ';
    }
    cout << endl;
  }*/

  // generate initial solution
  vector<int> solution = randomSearch(matrix);
  double solc = costCount(solution, matrix);
  while (solc >
         13900) { //это минимум для lu из гриди, лучше начинать сразу на нем
    cout << solc << endl;
    cout << solution[0] << endl;
    solution = Greedy(matrix);
    solc = costCount(solution, matrix);
  }
  cout << solution[0];

  cout << endl;
  double solution_cost = costCount(solution, matrix);
  cout << "init cost " << solution_cost << endl;
  // local search from initial
  vector<int> solution2 = localSearch(solution, matrix);

  solution_cost = costCount(solution2, matrix);
  cout << "after 1 ls " << solution_cost << endl;
  if (costCount(solution, matrix) < costCount(solution2, matrix)) {
    solution2 = solution;
  }
  vector<int> newsolution = two_opt(solution2, matrix);
  solution_cost = costCount(newsolution, matrix);
  cout << "after 1 opt " << solution_cost << endl;
  if (costCount(newsolution, matrix) < costCount(solution2, matrix)) {
    solution2 = newsolution;
  }
  int i = 0;
  vector<int> best = solution2;
  while (i < 1000) {
    i++;
    // perturbation
    // vector<int> newsolution = perturbation(solution2);
    // cout<<costCount(newsolution, matrix)<<endl;
    vector<int> newsolution2 = localSearch(
        solution2, matrix); //делаем лс на хорошем решении и улучшаем его
    // cout<<costCount(newsolution2,matrix)<<endl;
    // acceptance criteria
    if (costCount(solution2, matrix) > costCount(newsolution2, matrix)) {
      solution2 = newsolution2;
      cout << costCount(newsolution2, matrix) << endl;
    }
    if (costCount(best, matrix) > costCount(solution2, matrix)) {
      best = solution2;
    }
    // newsolution = two_opt(solution2); место для вашего 4-опт
    // newsolution = two_opt(solution2);
   // solution2 = perturbation(solution2);
  }

  cout << "Answer: lu" << endl;
  for (int i = 0; i < newsolution.size(); i++) {
    cout << best[i] << ' ';
  }
  cout << endl;
  solution_cost = costCount(best, matrix);
  cout << solution_cost << endl;
  cout << best.size() << endl;
}
