#include <iostream>
#include <map>

int main() {
  long x = 0, y = 0, id = 0, n = 0;
  std::map<long, std::pair<long, long>> my_map;

  std::cin >> n;
  for (auto i = 0; i < n; i++) {
    std::cin >> id >> x >> y;
    my_map[id] = std::make_pair(x, y);
  }
  
  std::pair<long, long> p = my_map[0];
  std::cout << p.first << ' ' << p.second;

  return 0;
}