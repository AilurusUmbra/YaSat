#include<vector>
using std::vector;

#ifndef STATE
#define UNSAT 0
#define SAT 1
#endif 


class CNF{
  public:
  vector<vector<int> > clauses;
  vector<int> literals;
  vector<int> literal_freq;


};

// print SAT
