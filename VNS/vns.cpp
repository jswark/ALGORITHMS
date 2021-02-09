#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

//  GE-index
double GE_index(const vector<int>&machines,const vector<int>&details,const vector<vector<int>>&matrix,int n_ones){
  double res =0;
  int n_ones_in=0;
  int n_zeros_in = 0;
  for(int i=0;i<machines.size();i++){
    for(int j=0;j<details.size();j++){
      if(machines[i]==details[j]){
        if(matrix[i][j]==0){
          n_zeros_in++;
        }else{
          n_ones_in++;
        }
      }
    }
  }
  res = (double) n_ones_in/(n_ones+n_zeros_in);
  return res;
}
//  shaking
//  exploration of neighborhood

int main(){
//  (1) parsing
  ifstream in;          // поток для записи
  in.open("../VNS/test_king.txt"); // окрываем файл для записи
  int m = 0;
  int p = 0;
  if (!in.is_open()) {
    return -1;
  }
  in >> m >> p;
  vector<vector<int>>matrix(m, vector<int>(p, 0));
  string buffer;
  while(getline(in,buffer)) {
    vector<int> numbers = {};
    stringstream stream(buffer);
    while (stream) {
      int tmp;
      stream >> tmp;
      numbers.push_back(tmp);
    }
    int j = numbers[0] - 1;
    for (int i = 1; i < numbers.size(); i++) {
      matrix[j][numbers[i] - 1] = 1;
    }
    numbers.clear();
  }
  in.close();
  for(auto & i : matrix){
    for(int j : i){
      cout<<j<<' ';
    }
    cout << endl;
  }
  int n_ones=0;
  for(auto & i : matrix){
    for(int j : i){
      if(j==1){
        n_ones++;
      }
    }
  }
  int n_zeros = m * p - n_ones;

//  (2) generate initial solution
  vector<int>machines(m);
  vector<int>details(p);
  machines={1,2,2,1,2};
  details={1,2,2,1,2,1,1};

//  (3) general vns
  int counter = 0;
  int k = 0;
  while(counter<2){
    counter++;
    k=1;
//    while(k!=k_max)
  }

//  (4) print answer
  cout<<"Answer:"<<endl;
  for(int i=0;i<m;i++){
    cout << machines[i]<<' ';
  }
  cout<<endl;
  for(int i=0;i<p;i++){
    cout<<details[i]<<' ';
  }
  cout<<endl;
}