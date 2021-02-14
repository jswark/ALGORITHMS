#ifndef INCLUDE_SHAKING_H_
#define INCLUDE_SHAKING_H_
#include <vector>

double GE_index(const std::vector<int> &machines, const std::vector<int> &details,
                const std::vector<std::vector<int>> &matrix, int& n_ones);
int count_clusters(std::vector<int> details, std::vector<int> machines);
int valid_clusters(std::vector<int> machines, std::vector<int> details,
                   std::vector<std::vector<int>> matrix, int clusters);
int split(std::vector<int> &details, std::vector<int> &machines,
          const std::vector<std::vector<int>>& matrix);
int merge(std::vector<int> &details, std::vector<int> &machines,
          std::vector<std::vector<int>> matrix);


#endif  // INCLUDE_SHAKING_H_
