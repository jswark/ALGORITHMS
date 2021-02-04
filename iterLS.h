#ifndef iterLS_H_
#define iterLS_H_

#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <vector>

// algorithm
std::vector<int> greedy(std::vector<std::vector<double>> matrix);
std::vector<int>
iterlocalSearch(const std::vector<std::vector<double>> &matrix);
std::vector<int> localSearch(const std::vector<int> &solution,
                             const std::vector<std::vector<double>> &matrix);
std::vector<int> two_opt(std::vector<int> path,
                         std::vector<std::vector<double>> matrix);
std::vector<int> two_optmove(std::vector<int> path, int i, int j);
std::vector<int> perturbation(std::vector<int> solution);

// utils
double costCount(std::vector<int> solution,
                 std::vector<std::vector<double>> matrix);
int vectorContanes(std::vector<int> index, int elem);
double get_dist(std::pair<long, long> city1, std::pair<long, long> city2);
std::vector<std::vector<double>> loadData();

#endif // iterLS_H_
