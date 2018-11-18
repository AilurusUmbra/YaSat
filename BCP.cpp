#include<iostream>
#include<vector>
#include"parser.h"

using namespace std;

#define UNSAT 0
#define SAT 1
string state[2] = {"s UNSATISFIABLE", "s SATISFIABLE"};

class CNF{
  public:
  vector<vector<int> > clauses;
  vector<int> literals;
  vector<int> literal_freq;


};

// print SAT
void printSAT(CNF& f, int rst){

  cout<<state[rst]<<endl;
  
  // if SAT, print variables;
  if(rst){
    cout<<"v ";


    cout<<"0\n";
  }
  return; 
}



int main(int argc, char* argv[]){

  CNF f; 

  // maybe for random start
    
  int maxVarIndex=0;

  parse_DIMACS_CNF(f.clauses, maxVarIndex, argv[1]);

  
  cout<<"maxVarIndex: "<<maxVarIndex<<endl;

  std::cout<<"print clauses: \n";      
  for(auto& clause : f.clauses){           
    for(auto& literal : clause){         
      std::cout<<literal<<' ';           
    }                                    
    std::cout<<"\n";                     
  }                                      
  std::cout<<"read file successfully\n"; 
  cout<<f.clauses.size();
  
  f.literals.resize(maxVarIndex, -1);   // 1:true, 0:false, -1: not assigned 
  f.literal_freq.resize(maxVarIndex, 0);
  
  // BCP 
  void unitRule(CNF& f){
    if(f.clauses.empty()){
      printSAT(f, SAT);
      return;
    }


  } 
    // unit clause find
      // implication
  

  // DPLL



  return 0;
}
