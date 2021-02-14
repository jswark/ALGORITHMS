#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <random>

using namespace std;
int n_ones;
int m;
int p;

double bestGE;
bool all_clusters(vector<int> check, int clusters) {
  vector<int> cl(clusters);
  for (int i = 1; i <= clusters; i++)
    cl[i - 1] = i;

  for (int i = 0; i < check.size(); i++) {
    for (int j = 0; j < cl.size(); j++) {
      if (check[i] == cl[j])
        cl.erase(cl.begin() + j);
    }
  }

  if (cl.empty())
    return true;

  return false;
}
//  GE-index
double GE_index(const vector<int> &machines, const vector<int> &details,
                const vector<vector<int>> &matrix) {
  double res = 0;
  int n_ones_in = 0;
  int n_zeros_in = 0;
  for (int i = 0; i < machines.size(); i++) {
    for (int j = 0; j < details.size(); j++) {
      if (machines[i] == details[j]) {
        if (matrix[i][j] == 0) {
          n_zeros_in++;
        } else {
          n_ones_in++;
        }
      }
    }
  }
  res = (double)n_ones_in / (n_ones + n_zeros_in);
  return res;
}
//  shaking
int count_clusters(vector<int> details, vector<int> machines){
  int clusters = 0;
  bool flag = true;
  while(flag){
    flag = false;
    clusters++;
    for(auto m:machines){
      if(m == clusters){
        flag = true;

      }
    }
  }
  clusters --;
  return clusters;
}

int valid_clusters(vector<int>machines,vector<int>details, vector<vector<int>> matrix, int clusters){
  bool correct;
  int c=0;
  while(c<clusters){
    correct=false;
    c++;
    for(int i=0;i<machines.size();i++){
      for(int j=0;j<details.size();j++){
        if(machines[i]==c && details[j]==c && matrix[i][j]==1){
          correct = true;
        }
      }
    }
    if(correct== false){
      return 0;
    }
  }
  return 1;
}

int split(vector<int> &details, vector<int> &machines, vector<vector<int>> matrix){
  vector<int>copy_m=machines;
  vector<int>copy_d=details;
  int clusters = count_clusters(details,machines);
  int cl = rand()%clusters+1;
  int cl_size_m = 0;
  for(auto m:machines){
    if(m == cl){
      cl_size_m++;
    }
  }
  if((cl_size_m == 1 && clusters == 1) || (machines.size()==clusters)){
    return -1;
  }
  while(cl_size_m<=1){
    cl = rand()%clusters+1;
    cl_size_m = 0;
    for(auto m:machines){
      if(m == cl){
        cl_size_m++;
      }
    }
  }
  int cl_size_d = 0;
  for(auto d:details){
    if(d == cl){
      cl_size_d++;
    }
  }
  if(cl_size_d <= 1){
    machines=copy_m;
    details=copy_d;
    return -1;
  }
  if((cl_size_d == 1 && clusters == 1) || (details.size()==clusters)){
    machines=copy_m;
    details=copy_d;
    return -1;
  }
  clusters++;
  int split_size_m = rand()%(cl_size_m-1)+1;
  for(int i=0;i<split_size_m;i++){
    int pos = rand()%machines.size();
    while (machines[pos] != cl){
      pos = rand()%machines.size();
    }
    machines[pos]=clusters;
  }

  int split_size_d = rand()%(cl_size_d-1)+1;
  for(int i=0;i<split_size_d;i++){
    int pos = rand()%details.size();
    while (details[pos] != cl){
      pos = rand()%details.size();
    }
    details[pos]=clusters;
  }
  if(!valid_clusters(machines,details,matrix,clusters)){
    machines=copy_m;
    details=copy_d;
    return -1;
  }
  cout<<"After split:"<<endl;
  for (int i = 0; i < machines.size(); i++) {
    cout << machines[i] << ' ';
  }
  cout << endl;
  for (int i = 0; i < details.size(); i++) {
    cout << details[i] << ' ';
  }
  cout << endl;
  return 0;
}

int merge(vector<int> &details, vector<int> &machines, vector<vector<int>>matrix){
  vector<int>copy_m=machines;
  vector<int>copy_d=details;
  int clusters = count_clusters(details,machines);
  if(clusters==1){
    machines=copy_m;
    details=copy_d;
    return -1;
  }
  int cl1 = rand()%clusters+1;
  int cl2 = rand()%clusters+1;
  while(cl1==cl2){
    cl2 = rand()%clusters+1;
  }
  if(cl1>cl2){
    swap(cl1,cl2);
  }
  for(int i=0;i<machines.size();i++){
    if(machines[i]==cl2){
      machines[i]=cl1;
    }else if(machines[i]>cl2){
      machines[i]--;
    }
  }
  for(int i=0;i<details.size();i++){
    if(details[i]==cl2){
      details[i]=cl1;
    }else if(details[i]>cl2){
      details[i]--;
    }
  }
  if(!valid_clusters(machines,details,matrix,clusters)){
    machines=copy_m;
    details=copy_d;
    return -1;
  }
  cout<<"After merge:"<<endl;
  for (int i = 0; i < machines.size(); i++) {
    cout << machines[i] << ' ';
  }
  cout << endl;
  for (int i = 0; i < details.size(); i++) {
    cout << details[i] << ' ';
  }
  cout << endl;
  return 0;
}

//  exploration of neighborhood
vector<int> move_cols(vector<int> &details, vector<int> &machines,
                      vector<vector<int>> &matrix, int clusters) {
  vector<int> details_new = details;
  double GE = bestGE;
  int tmp_cl = 0;

  for (int i = 0; i < details.size(); i++) {
    for (int j = 1; j <= clusters; j++) {
      if (j != details[i]) {
        tmp_cl = details[i];
        details_new[i] = j;

        GE = GE_index(machines, details_new, matrix);
        if (GE >= bestGE && all_clusters(details_new, clusters)) {
          bestGE = GE;
          return details_new;
        } else
          details_new[i] = tmp_cl;
      }
    }
  }

  return details;
}

vector<int> move_rows(vector<int> &machines, vector<int> &details,
                      vector<vector<int>> &matrix, int clusters) {
  vector<int> machines_new = machines;
  double GE = bestGE;
  int tmp_cl = 0;

  for (int i = 0; i < machines.size(); i++) {
    for (int j = 1; j <= clusters; j++) {
      if (j != machines[i]) {
        tmp_cl = machines[i];
        machines_new[i] = j;

        GE = GE_index(machines_new, details, matrix);
        if (GE > bestGE && all_clusters(machines_new, clusters)) {
          bestGE = GE;
          return machines_new;
        } else
          machines_new[i] = tmp_cl;
      }
    }
  }

  return machines;
}

void LS(vector<int> &machines, vector<int> &details,
               vector<vector<int>> &matrix, int &clusters) {
  vector<int> machines_new = machines;
  vector<int> details_new = details;
  double cur_GE = bestGE;
  bool better_sol = true;
  bool better_rows = true;
  bool better_cols = true;
  while (better_sol) {
    better_sol = false;
    while (better_rows) {
      better_rows = false;
      machines_new = move_rows(machines_new, details_new, matrix, clusters);
     // details_new = move_cols(details_new, machines_new, matrix, clusters);
      if (cur_GE < bestGE) {
        cur_GE = bestGE;
        better_rows = true;
        better_sol = true;
        better_cols = true;
      }
    }

    while (better_cols) {

      better_cols = false;
      details_new = move_cols(details_new, machines_new, matrix, clusters);
      if (cur_GE < bestGE) {
        cur_GE = bestGE;
        better_cols = true;
        better_sol = true;
        better_rows = true;
      }
    }
  }

  if (all_clusters(machines_new, clusters) && all_clusters(details_new, clusters)) {
    machines = machines_new;
    details = details_new;
    }
  else {
      clusters--;
    }


  cout << "LS Answer: ______________________" << endl;
  for (int i = 0; i < machines_new.size(); i++) {
    cout << machines[i] << ' ';
  }
  cout << endl;
  for (int i = 0; i < details_new.size(); i++) {
    cout << details[i] << ' ';
  }
  cout << endl;
  cout << GE_index(machines, details, matrix) << endl;
  cout << "_________________________________" << endl;
}

vector<int> rand_sol(vector<int> &for_rand, int clusters) {
  vector<int> filled = for_rand;
  while (!all_clusters(filled, clusters)) {
    filled = for_rand;
    for (int i = 0; i < for_rand.size(); i++)
      filled[i] = rand() % clusters + 1;
  }
  return filled;
}

std::vector<std::vector<int>> loadData() {
  ifstream in;                 // поток для записи
  in.open("../VNS/contest/zolf50.txt"); // окрываем файл для записи
  m = 0;
  p = 0;
  if (!in.is_open()) {
    cout << "cant open file";
  }
  in >> m >> p;
  vector<vector<int>> matrix(m, vector<int>(p, 0));
  string buffer;
  while (getline(in, buffer)) {
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

  for (auto &i : matrix) {
    for (int j : i) {
      cout << j << ' ';
    }
    cout << endl;
  }
  n_ones = 0;
  for (auto &i : matrix) {
    for (int j : i) {
      if (j == 1) {
        n_ones++;
      }
    }
  }
  int n_zeros = m * p - n_ones;

  return matrix;
}
