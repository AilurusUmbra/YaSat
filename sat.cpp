#include<iostream>
#include<vector>
#include"parser.h"

using namespace std;

int main(int argc, char* argv[]){

  vector<vector<int> > cl;
  int maxVarIndex=0;

  parse_DIMACS_CNF(cl, maxVarIndex, argv[1]);
  

  
  



  return 0;
}
