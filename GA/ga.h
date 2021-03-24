#ifndef GA_H_
#define GA_H_

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

std::vector<std::vector<long>>
init_sol(std::map<double, std::pair<double, double>> my_map,
         std::vector<long> &best_solution);

// select parents (choose 2 best) Tournament selection
std::vector<std::vector<long>>
tournamentSelection(std::vector<std::vector<long>> population,
                    std::map<double, std::pair<double, double>> my_map);

// one point crossover
std::vector<long> crossover(std::vector<long> &parent1,
                            std::vector<long> &parent2,
                            std::map<double, std::pair<double, double>> my_map);

#endif // GA_H_
