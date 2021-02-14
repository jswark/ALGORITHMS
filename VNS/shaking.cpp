#include "shaking.h"
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

//  GE-index
double GE_index(const vector<int> &machines, const vector<int> &details,
                const vector<vector<int>> &matrix, int& n_ones) {
  double res = 0;
  int n_ones_in = 0;
  int n_zeros_in = 0;
  for (int i = 0; i < machines.size(); i++) {
    for (int j = 0; j < details.size(); j++) {
      if (machines[i] == details[j]) {
        if (matrix[i][j] == 0) {
          n_zeros_in++;
        } else {
          n_ones_in++;
        }
      }
    }
  }
  res = (double)n_ones_in / (n_ones + n_zeros_in);
  return res;
}
//  shaking
int count_clusters(vector<int> details, vector<int> machines) {
  int clusters = 0;
  bool flag = true;
  while (flag) {
    flag = false;
    clusters++;
    for (auto m : machines) {
      if (m == clusters) {
        flag = true;
      }
    }
  }
  clusters--;
  return clusters;
}

int valid_clusters(vector<int> machines, vector<int> details,
                   vector<vector<int>> matrix, int clusters) {
  bool correct;
  int c = 0;
  while (c < clusters) {
    correct = false;
    c++;
    for (int i = 0; i < machines.size(); i++) {
      for (int j = 0; j < details.size(); j++) {
        if (machines[i] == c && details[j] == c && matrix[i][j] == 1) {
          correct = true;
        }
      }
    }
    if (!correct) {
      return 0;
    }
  }
  return 1;
}

int split(vector<int> &details, vector<int> &machines,
          const vector<vector<int>>& matrix) {
  vector<int> copy_m = machines;
  vector<int> copy_d = details;

  int clusters = count_clusters(details, machines);
  int cl = rand() % clusters + 1;
  int cl_size_m = 0;
  for (auto ma : machines) {
    if (ma == cl) {
      cl_size_m++;
    }
  }
  if ((cl_size_m == 1 && clusters == 1) || (machines.size() == clusters)) {
    return -1;
  } do{
    machines = copy_m;
    details = copy_d;
    while (cl_size_m <= 1) {
      cl = rand() % clusters + 1;
      cl_size_m = 0;
      for (auto ma : machines) {
        if (ma == cl) {
          cl_size_m++;
        }
      }
    }
    int cl_size_d = 0;
    for (auto d : details) {
      if (d == cl) {
        cl_size_d++;
      }
    }
    if (cl_size_d <= 1) {
      machines = copy_m;
      details = copy_d;
      return -1;
    }
    if ((cl_size_d == 1 && clusters == 1) || (details.size() == clusters)) {
      machines = copy_m;
      details = copy_d;
      return -1;
    }
    clusters++;
    int split_size_m = rand() % (cl_size_m - 1) + 1;
    for (int i = 0; i < split_size_m; i++) {
      int pos = rand() % machines.size();
      while (machines[pos] != cl) {
        pos = rand() % machines.size();
      }
      machines[pos] = clusters;
    }

    int split_size_d = rand() % (cl_size_d - 1) + 1;
    for (int i = 0; i < split_size_d; i++) {
      int pos = rand() % details.size();
      while (details[pos] != cl) {
        pos = rand() % details.size();
      }
      details[pos] = clusters;
    }}
  while (!valid_clusters(machines, details, matrix, clusters));

  cout << "After split:" << endl;
  for (int i = 0; i < machines.size(); i++) {
    cout << machines[i] << ' ';
  }
  cout << endl;
  for (int i = 0; i < details.size(); i++) {
    cout << details[i] << ' ';
  }
  cout << endl;
  return 0;
}

int merge(vector<int> &details, vector<int> &machines,
          vector<vector<int>> matrix) {
  vector<int> copy_m = machines;
  vector<int> copy_d = details;
  int clusters = count_clusters(details, machines);


  machines = copy_m;
  details = copy_d;
  if (clusters == 1) {
    machines = copy_m;
    details = copy_d;
    return -1;
  }
  int cl1 = rand() % clusters + 1;
  int cl2 = rand() % clusters + 1;
  while (cl1 == cl2) {
    cl2 = rand() % clusters + 1;
  }
  if (cl1 > cl2) {
    swap(cl1, cl2);
  }

  do {
    for (int i = 0; i < machines.size(); i++) {
      if (machines[i] == cl2) {
        machines[i] = cl1;
      } else if (machines[i] > cl2) {
        machines[i]--;
      }
    }
    for (int i = 0; i < details.size(); i++) {
      if (details[i] == cl2) {
        details[i] = cl1;
      } else if (details[i] > cl2) {
        details[i]--;
      }
    }
  }
  while (!valid_clusters(machines, details, matrix, clusters--));

  cout << "After merge:" << endl;
  for (int i = 0; i < machines.size(); i++) {
    cout << machines[i] << ' ';
  }
  cout << endl;
  for (int i = 0; i < details.size(); i++) {
    cout << details[i] << ' ';
  }
  cout << endl;
  return 0;
}