#ifndef GA_LOAD_DATA_H_
#define GA_LOAD_DATA_H_

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

double get_dist(std::pair<double, double> city1,
                std::pair<double, double> city2) {
  double dx = pow((city1.first - city2.first), 2);
  double dy = pow((city1.second - city2.second), 2);

  return (sqrt(dx + dy));
}

std::vector<std::vector<double>> load_data() {
  std::string path = "../test.csv";
  std::ifstream line(path);
  std::string newstr;
  int id;

  std::map<double, std::pair<double, double>> my_map;

  getline(line, newstr);

  while (getline(line, newstr)) {
    int i = 0;
    std::string tmp, arr[3];
    std::stringstream strstream(newstr);

    while (std::getline(strstream, tmp, ','))
      arr[i++] = tmp;

    id = std::stoi(arr[0]);
    double x = std::stof(arr[1]), y = std::stof(arr[2]);
    my_map[id] = std::make_pair(x, y);

  }

  int n = id;
  std::vector<std::vector<double>> matrix(n+1, std::vector<double>(n+1, 0));
  for (int i = 0; i <= n; i++) {
    for (int j = 0; j <= n; j++) {
      if (i == j) {
        matrix[i][j] = 0;
      } else {
        matrix[i][j] = get_dist(my_map[i], my_map[j]);
      }
    }
    std::cout << i << std::endl;
  }
  /*
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        std::cout << matrix[i][j] << " ";
      }
      std:: cout << std::endl;
    }
    */
  return matrix;
}

#endif // GA_LOAD_DATA_H_
