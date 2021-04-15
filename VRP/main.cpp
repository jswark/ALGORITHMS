#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
struct VRP {
  int id;
  int x;
  int y;
  int demand;
  int open;
  int close;
  int free;
};

struct VRP* loadData() {
  //  input data
  int x = 0, y = 0, id = 0, demand = 0, open = 0, close = 0, free = 0;
  long n = 0, cars = 0, capacity = 0;

  std::srand(time(0));

  freopen("../I1.txt", "r", stdin);

  scanf("%ld %ld %ld", &n, &cars, &capacity);

  VRP *data = (VRP*)calloc((n + 1), sizeof(struct VRP)); //size = n+1
  for (auto i = 0; i <= n; i++) {
    scanf("%d %d %d %d %d %d %d", &id, &x, &y, &demand, &open, &close, &free);
    data[id] = {id, x, y, demand, open, close, free};
  }

  return data;
}

int main(){
  VRP *data = loadData();
  
  for (int i = 0; i <= 100; ++i) {
    std::cout << data[i].id << " " << data[i].free << std::endl;
  }

}