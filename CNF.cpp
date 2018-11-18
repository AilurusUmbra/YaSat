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

  // default constructor 
	CNF() {}

  // copy constructor
  CNF(CNF &f) {
    clauses = f.clauses;
    literals = f.literals;
    literal_freq = f.literal_freq;
  }
  void print_freq(){
    std::cout<<"*******************\n";
    for(size_t i=0; i<literal_freq.size(); ++i){
      std::cout<<i<<" ";
    }
    std::cout<<std::endl;
    for(size_t i=0; i<literal_freq.size(); ++i){
      std::cout<<literal_freq[i]<<" ";
    }
    std::cout<<std::endl;
  }
};

