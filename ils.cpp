#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

float costCount(vector<int> solution,vector<vector<float>> matrix){
  float sum = 0;
  for (int i = 0;i<solution.size()-1;i++){
    sum+=matrix[solution[i]-1][solution[i+1]-1];
  }
  sum+=matrix[solution.back()-1][solution.front()-1];
  return sum;
}

vector<int> randomSearch(vector<vector<float>> matrix){
  // generate node list
  vector<int> nodes={1,2,3,4,5,6,7,8};
  int n = nodes.size();
  vector <int> solution={};
  for (int i=0;i<n;i++){
    int k = rand() % nodes.size();
    solution.push_back(nodes[k]);
    nodes.erase(nodes.begin()+k);
  }
  return solution;
}

int allused(vector<int>used,vector<int>ans) {
  for (int i = 0; i < used.size(); i++) {
    if (used[i] == 0) {
      return 0;
    }
  }
  return 1;
}

int greedyRecursive(int v,vector<vector<float>>matrix,float &cost,vector<int>&ans,int n,vector<int>&used){
  used[v]++;
  ans.push_back(v);
  float min = -1;
  int nv=-1;
  int j = 1;
  for (int i = 0; i < n && j; i++) {
    if (matrix[v][i] != 0 && used[i]==0) {
      min = matrix[v][i];
      nv = i;
      j = 0;
    }
  }
  for (int i = 0; i < n; i++) {
    if (matrix[v][i] < min && matrix[v][i] != 0 && used[i]==0) {
      min = matrix[v][i];
      nv = i;
    }
  }
  if (allused(used,ans)) {
    if (matrix[v][ans[0]] != 0) {
      ans.push_back(ans[0]);
      cost += matrix[v][ans[0]];
      return 1;
    }
    else {
      return 0;
    }

  }
  if (min == -1) {
    return 0;
  }
  cost += min;
  greedyRecursive(nv, matrix, cost, ans, n,used);
  return 1;
}

vector<int> greedySearch(vector<vector<float>> matrix){
  int n = matrix.size();
  float cost = 0, res = -1;
  vector<int>ans;
  vector<int>itog(n+1);
  for (int i = 0; i < n; i++) {
    vector<int>used(n, 0);
    int ff=greedyRecursive(i,matrix,cost,ans,n,used);
    if (ans[0]==ans.back()) {
      if (res == -1 || res > cost) {
        res = cost;
        for (int i = 0; i < ans.size(); i++) {
          itog[i] = ans[i];
        }

      }
    }
    ans.clear();
    cost = 0;
  }
  itog.erase(itog.end()-1);
  for (int i=0;i<itog.size();i++){
    itog[i]++;
  }
  return itog;
}

vector<int> localSearch(vector<int> solution,vector<vector<float>> matrix){  // 2-opt algorithm
  int n = solution.size();
  vector<int> newsolution(n);
  vector<int> currentsolution(n);
  newsolution=solution;
  int flag = 0;
  float bestdistance=costCount(newsolution,matrix);
  while (flag == 0) { // repeat until we can change something
    flag = 1;
    start:
    for (int i = 1; i < n-1; i++) {
      for (int j = i + 1; j < n; j++) {
        currentsolution = newsolution;
        swap(currentsolution[i],currentsolution[j]);
        float currentdistance = costCount(currentsolution, matrix);
        if (currentdistance < bestdistance) { // if current solution is better - save and restart
          newsolution = currentsolution;
          bestdistance = currentdistance;
          flag = 0;
          goto start;
        }
      }
    }
  }
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
  vector<int>newsolution(solution.size());  // size at least 8
  vector<int>index;
  for(int i=0;i<4;i++){
    int pos = rand()%copy.size();
    while(copy[pos] == copy[copy.size()-1] || vectorContanes(index,copy[pos]) || vectorContanes(index,copy[pos+1])){
      pos = rand()%copy.size();
    }
    index.push_back(copy[pos]);
    index.push_back(copy[pos+1]);
    copy.erase(copy.begin() + 1+pos);
    copy.erase(copy.begin() + pos);
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

int main(){
//  input data
  int n = 8;
  vector<vector<float>> matrix = {{0.0, 3.0, 4.0, 2.0, 7.0, 1.1, 11.2, 3.0},
                                  {3.0, 0.0, 4.0, 6.0, 3.0, 3.3, 4.5, 1.0},
                                  {4.0, 4.0, 0.0, 5.0, 8.0, 5.6, 5.0, 1.1},
                                  {2.0, 6.0, 5.0, 0.0, 6.0, 6.0, 1.1, 1.0},
                                  {7.0, 3.0, 8.0, 6.0, 0.0, 5.5, 5.0, 5.0},
                                  {1.1, 3.3, 5.6, 6.0, 5.5, 0.0, 2.2, 2.2},
                                  {11.2, 4.5, 5.0, 1.1, 5.0, 2.2, 0.0, 0.5},
                                  {3.0, 1.0, 1.1, 1.0, 5.0, 2.2, 0.5, 0.0}};

// generate initial solution
  vector<int>solution=greedySearch(matrix); // or randomSearch()
  float solution_cost = costCount(solution,matrix);
  cout<<solution_cost<<endl;
// local search from initial
  vector<int> solution2 = localSearch(solution,matrix);
  for (int i=0;i<solution2.size();i++){
    cout<<solution2[i]<<' ';
  }
  solution_cost = costCount(solution2,matrix);
  cout<<solution_cost<<endl;
  if(costCount(solution,matrix)<costCount(solution2,matrix)){
    solution2 = solution;
  }
  int i = 0;
  while (i<20){
    i++;
    // perturbation
    vector<int> newsolution = perturbation(solution2);
    vector<int> newsolution2 = localSearch(newsolution,matrix);
    cout<<i<<' '<<costCount(newsolution2,matrix)<<endl;
    // acceptance criteria
    if (costCount(solution2,matrix)>costCount(newsolution2,matrix)){
      solution2 = newsolution2;
    }
  }
  cout << "Answer:"<<endl;
  for (int i=0;i<solution2.size();i++){
    cout<<solution2[i]<<' ';
  }
  solution_cost = costCount(solution2,matrix);
  cout<<solution_cost<<endl;
}
