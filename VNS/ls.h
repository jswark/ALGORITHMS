#ifndef INCLUDE_LS_H_
#define INCLUDE_LS_H_
#include <vector>

std::vector<int> move_cols(std::vector<int> &details,
                           std::vector<int> &machines,
                           std::vector<std::vector<int>> &matrix, int clusters,
                           int n_ones, double bestGE);
std::vector<int> move_rows(std::vector<int> &machines,
                           std::vector<int> &details,
                           std::vector<std::vector<int>> &matrix, int clusters,
                           int n_ones, double bestGE);
void LS(std::vector<int> &machines, std::vector<int> &details,
        std::vector<std::vector<int>> &matrix, int &clusters, int n_ones,
        double bestGE);
std::vector<int> rand_sol(std::vector<int> &for_rand, int clusters);
bool all_clusters(std::vector<int> check, int clusters);

#endif // INCLUDE_LS_H_