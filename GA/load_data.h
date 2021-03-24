#ifndef GA_LOAD_DATA_H_
#define GA_LOAD_DATA_H_

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#define MAX 10000000

double get_dist(std::pair<double, double> city1,
                std::pair<double, double> city2) {
  double dx = pow((city1.first - city2.first), 2);
  double dy = pow((city1.second - city2.second), 2);

  return (sqrt(dx + dy));
}

double total_cost(std::vector<long> solution,
                  std::map<double, std::pair<double, double>> my_map) {
  double cost = 0;
  for (int i = 0; i < solution.size() - 1; ++i) {
    cost += get_dist(my_map[solution[i]], my_map[solution[i + 1]]);
  }

  return cost;
}

void del_worst() {}

bool validSol(std::vector<long> solution) {
  std::vector<long> mask;
  for (int i = 0; i < solution.size(); i++) {
    mask.push_back(1);
  }

  for (int i = 0; i < solution.size(); ++i) {
    if (mask[solution[i]] == 1)
      mask[solution[i]] = 0;
    else
      return false;
  }

  return true;
}

// one point crossover
std::vector<long>
crossover(std::vector<long> &parent1, std::vector<long> &parent2,
          std::map<double, std::pair<double, double>> my_map) {

  std::vector<long> child1(parent1.size()), child2(parent1.size());
  std::vector<long> gen1, gen2; // used genes
  for (int i = 0; i < parent1.size(); i++) {
    gen1.push_back(0);
    gen2.push_back(0);
  }

  int point = rand() % parent1.size();

  for (int i = 0; i < point; ++i) {
    child1[i] = parent1[i];
    gen1[parent1[i]] = 1;

    child2[i] = parent2[i];
    gen2[parent2[i]] = 1;
  }

  for (int i = point; i < parent1.size(); ++i) {
    if (gen1[parent2[i]] == 0) {
      child1[i] = parent2[i];
      gen1[parent2[i]] = 1;
    } else {
      for (int num_gen = 0; num_gen < gen1.size(); ++num_gen) {
        if (gen1[num_gen] == 0) {
          child1[i] = num_gen;
          gen1[num_gen] = 1;
          break;
        }
      }
    }

    if (gen2[parent1[i]] == 0) {
      child2[i] = parent1[i];
      gen2[parent1[i]] = 1;
    } else {
      for (int num_gen = 0; num_gen < gen2.size(); ++num_gen) {
        if (gen2[num_gen] == 0) {
          child2[i] = num_gen;
          gen2[num_gen] = 1;
          break;
        }
      }
    }
  }

  std::cout << total_cost(child1, my_map) << " " << total_cost(child2, my_map)
            << std::endl;

  if (total_cost(child1, my_map) < total_cost(child2, my_map))
    return child1;
  else
    return child2;
}

std::vector<long> randSol(int n) {
  std::vector<long> mask;
  for (int i = 0; i < n; i++) {
    mask.push_back(0);
  }
  std::vector<long> solution;
  long randCity = rand() % n;
  for (int i = 0; i < n; i++) {
    while (mask[randCity] != 0)
      randCity = rand() % n;
    solution.push_back(randCity);
    mask[randCity] = 1; // used
  }

  return solution;
}

std::vector<long> greedy(std::map<double, std::pair<double, double>> my_map) {
  std::vector<int> used = {};
  for (int i = 0; i < my_map.size(); i++) {
    used.push_back(0);
  }

  int startid = rand() % used.size();
  int n = used.size();
  int k = 1;
  std::vector<long> solution = {};

  solution.push_back(startid);
  used[startid] = 1;

  int curid = startid;
  double min = MAX;
  int next = 0;
  double distance = 0;

  while (k < my_map.size() - 1) {
    for (int i = 0; i < n; i++) {
      if (curid != i) {
        distance = get_dist(my_map[curid], my_map[i]);
        if (distance < min && used[i] == 0) {
          min = distance;
          next = i;
        }
      }
    }

    solution.push_back(next);
    min = MAX;
    used[next] = 1;
    curid = next;
    k++;
  }

  return solution;
}

std::vector<std::vector<long>>
init_sol(std::map<double, std::pair<double, double>> my_map,
         std::vector<long> &best_solution) {
  int num_gen = 30; // value of population
  double best_cost = 10000000;
  double cur_cost = 0;
  std::vector<std::vector<long>> population;

  for (int i = 0; i < num_gen; ++i) {
    std::vector<long> chromosome(my_map.size());
    chromosome = greedy(my_map);
    population.push_back(chromosome);
    cur_cost = total_cost(chromosome, my_map);
    if (cur_cost < best_cost) {
      best_cost = cur_cost;
      best_solution = chromosome;
    }
  }

  return population;
}

std::vector<std::vector<long>>
tournamentSelection(std::vector<std::vector<long>> population,
                    std::map<double, std::pair<double, double>> my_map) {
  std::vector<std::vector<long>> parents;
  std::vector<std::vector<long>> candidates;
  std::vector<long> mask;

  for (int i = 0; i < population.size(); i++) {
    mask.push_back(0);
  }

  // choose k candidates
  int k = population.size() * 0.4;
  for (int j = 0; j < k; ++j) {
    int num = rand() % population.size();
    while (mask[num] == 1)
      num = rand() % population.size();
    candidates.push_back(population[num]);
    mask[num] = 1;
  }

  // find 2 best of these
  double best1 = 0, best2 = 1; // index
  double bestsize = total_cost(candidates[0], my_map);
  double cursize = 0;

  if (total_cost(candidates[0], my_map) > total_cost(candidates[1], my_map)) {
    std::swap(best1, best2);
    bestsize = total_cost(candidates[1], my_map);
  }

  for (int i = 2; i < candidates.size(); ++i) {
    cursize = total_cost(candidates[i], my_map);
    if (bestsize > cursize) {
      bestsize = cursize;
      best2 = best1;
      best1 = i;
    }
  }

  parents.push_back(candidates[best1]);
  parents.push_back(candidates[best2]);
  return parents;
}

std::map<double, std::pair<double, double>> load_data() {
  std::string path = "../cities.csv";
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

  return my_map;
}

#endif // GA_LOAD_DATA_H_
