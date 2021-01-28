#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <map>
using namespace std;
std::vector<int> two_opt(std::vector<int> vect) {
  vector<int> copy = vect;
  long i = rand()%vect.size();
  long j=rand()%vect.size();
  if (i > j)
    std::swap(i, j);
  std::vector<int> newVect;
  newVect.insert(newVect.end(), vect.begin(), vect.begin() + i);

  std::vector<int> middlePart(vect.begin() + i, vect.begin() + j);
  std::reverse(middlePart.begin(), middlePart.end());

  newVect.insert(newVect.end(), middlePart.begin(), middlePart.end());
  newVect.insert(newVect.end(), vect.begin() + j, vect.end());

  return newVect;
}
long long costCount(vector<int> solution,vector<vector<long long>> matrix){
  long long sum = 0;
  for (int i = 0;i<solution.size()-1;i++){
    sum+=matrix[solution[i]-1][solution[i+1]-1];
  }
  sum+=matrix[solution.back()-1][solution.front()-1];
  return sum;
}

vector<int> randomSearch(vector<vector<long long>> matrix){
  // generate node list
  vector<int> nodes={};
  for (int i=1;i<=matrix.size();i++){
    nodes.push_back(i);
  }
  int n = nodes.size();
  vector <int> solution={};
  for (int i=0;i<n;i++){
    int k = rand() % nodes.size();
    solution.push_back(nodes[k]);
    nodes.erase(nodes.begin()+k);
  }
  return solution;
}

vector<int> Greedy(vector<vector<long long>> matrix){
  // generate node list
  vector<int> mask={};
  for (int i=1;i<=matrix.size();i++){
    mask.push_back(0);
  }

  int startid = rand()%mask.size();
  int n = mask.size();
  int k = 1;
  vector <int> solution={};

  solution.push_back(startid+1);
  mask[startid] = 1;

  int curid = startid;
  int min = 100000000000;
  int next = 0;

  while (k < matrix.size()) {
    for (int i = 0; i < n; i++) {
      if (matrix[curid][i] < min && mask[i] == 0) {
        min = matrix[curid][i];
        next = i;
      }
    }
    solution.push_back(next+1);
    min = 100000000000;
    mask[next] = 1;
    curid = next;
    k++;
  }

  return solution;
}

vector<int> localSearch(vector<int> solution,vector<vector<long long>> matrix){  // 2-opt algorithm
  int n = solution.size();
  vector<int> newsolution(n);
  vector<int> currentsolution(n);
  newsolution=solution;
  int flag = 0;
  long long bestdistance=costCount(newsolution,matrix);
  int k=0;
  while (flag == 0 && k<5) { // repeat until we can change something
    flag = 1;
    k++;
  int i = rand()%n;
  int j = rand()%n;
  currentsolution = newsolution;
        if(i!=j) {
          two_opt(currentsolution);
          //swap(currentsolution[i], currentsolution[j]);
          float currentdistance = costCount(currentsolution, matrix);
          if (currentdistance < bestdistance) {
            newsolution = currentsolution;
            bestdistance = currentdistance;
            flag = 0;
          }}}
  return newsolution;
}

int vectorContanes(vector<int>index, int elem){
  for (int i=0;i<index.size();i++){
    if(index[i] == elem){
      return 1;
    }
  }
  return 0;
}

vector<int> perturbation(vector<int> solution){  // 4-opt double bridge algorithm
  vector<int>copy = solution;
  vector<int>newsolution=solution;  // size at least 8
  vector<int>index;
  for(int i=0;i<4;i++){
    int pos = rand()%copy.size();
    while(pos == copy.size()-1 || vectorContanes(index,pos) || vectorContanes(index,pos+1)){
      pos = rand()%copy.size();
    }
    index.push_back(pos);
    index.push_back(pos+1);
  }
  // sort!
  sort(index.begin(),index.end());
  int a1 = index[0];
  int a2 = index[1];
  int b1 = index[2];
  int b2 = index[3];
  int c1 = index[4];
  int c2 = index[5];
  int d1 = index[6];
  int d2 = index[7];
  int i=0;
  int dest=0;
  for(int j=dest;j<a2;j++){
    newsolution[j]=solution[i++];
  }
  i=c2;
  dest+=a2;
  for(int j=dest;j<d1-c2+1+dest;j++){
    newsolution[j]=solution[i++];
  }
  dest+= d1-c2+1;
  i=b2;
  for(int j=dest;j<c1-b2+1+dest;j++){
    newsolution[j]=solution[i++];
  }
  i=a2;
  dest += c1-b2+1;
  for(int j=dest;j<b1-a2+1+dest;j++){
    newsolution[j]=solution[i++];
  }
  i=d2;
  dest += b1-a2+1;
  for(int j=dest;j<solution.size()-d2;j++){
    newsolution[j]=solution[i++];
  }
  return newsolution;
}

long get_dist(pair<long, long> city1, pair<long, long> city2) {
  long dx = pow((long)(city1.first - city2.first), 2);
  long dy = pow((long)(city1.second - city2.second), 2);
  return (floor((long)(sqrt(dx + dy))));
}

int main(){
//  input data
  long x = 0, y = 0, id = 0, n = 0;
  map<long, pair<long, long>> my_map;
  std::srand(time(0));

  freopen("../data.txt", "r", stdin);
  scanf("%ld", &n);
  for (auto i = 0; i < n; i++) {
    scanf("%ld %ld %ld", &id, &x, &y);
    my_map[id] = make_pair(x, y);
  }
  vector<vector<long long>> matrix (n,vector<long long>(n,0));
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      if (i == j) {
        matrix[i-1][j-1]=0;
      } else {
        matrix[i-1][j-1]=get_dist(my_map[i], my_map[j]);
      }
    }
  }


  for (int i=0;i<matrix.size();i++){
    for (int j=0;j<matrix.size();j++) {
      cout << matrix[i][j] << ' ';
    }
    cout<<endl;
  }

// generate initial solution
  vector<int>solution=Greedy(matrix);
  cout << endl;
  long solution_cost = costCount(solution,matrix);
  cout<<solution_cost<<endl;
// local search from initial
  vector<int> solution2 = localSearch(solution,matrix);

  solution_cost = costCount(solution2,matrix);
  cout<<solution_cost<<endl;
  if(costCount(solution,matrix)<costCount(solution2,matrix)){
    solution2 = solution;
  }
  int i = 0;
  while (i<10000){
    i++;
    // perturbation
    vector<int> newsolution = two_opt(solution2);
    vector<int> newsolution2 = localSearch(newsolution,matrix);
    cout<<costCount(newsolution2,matrix)<<endl;
    // acceptance criteria
    if (costCount(solution2,matrix)>costCount(newsolution2,matrix)){
      solution2 = newsolution2;
    }
  }
  cout << "Answer:"<<endl;
  for (int i=0;i<solution2.size();i++){
    cout<<solution2[i]<<' ';
  }
  cout << endl;
  solution_cost = costCount(solution2,matrix);
  cout<<solution_cost<<endl;
  cout<<solution.size()<<endl;

}
