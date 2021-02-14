#include "load_data.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

std::vector<std::vector<int>> loadData(int &m, int &p, int &n_ones) {
  ifstream in;                        // поток для записи
  in.open("../contest/carrie28.txt"); // окрываем файл для записи
  m = 0;
  p = 0;
  if (!in.is_open()) {
    cout << "cant open file";
  }
  in >> m >> p;
  vector<vector<int>> matrix(m, vector<int>(p, 0));
  string buffer;
  while (getline(in, buffer)) {
    vector<int> numbers = {};
    stringstream stream(buffer);
    while (stream) {
      int tmp;
      stream >> tmp;
      numbers.push_back(tmp);
    }
    int j = numbers[0] - 1;
    for (int i = 1; i < numbers.size(); i++) {
      matrix[j][numbers[i] - 1] = 1;
    }
    numbers.clear();
  }
  in.close();

  for (auto &i : matrix) {
    for (int j : i) {
      cout << j << ' ';
    }
    cout << endl;
  }
  n_ones = 0;
  for (auto &i : matrix) {
    for (int j : i) {
      if (j == 1) {
        n_ones++;
      }
    }
  }
  int n_zeros = m * p - n_ones;

  return matrix;
}