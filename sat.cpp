#include<iostream>
#include<vector>
#include<algorithm>
#include"parser.h"

using namespace std;

#ifndef STATE
#define UNSAT 0
#define SAT 1
#endif

string state[2] = {"s UNSATISFIABLE", "s SATISFIABLE"};


#include "CNF.cpp"


// print SAT
void printSAT(CNF& f, int rst){

  cout<<state[rst]<<endl;
  
  // if SAT, print variables;
  if(rst){
    cout<<"v ";
    for(auto& lit : f.literals){
      std::cout<<lit<<' ';           
    }
    cout<<"0\n";
  }
  return; 
}

// BCP 
void assignment(CNF& f, int l){

  for(auto& cl : f.clauses){

    // check for (a v a')
    vector<int>::iterator pos_it;
    pos_it = find (cl.begin(), cl.end(), l);

    vector<int>::iterator neg_it;
    neg_it = find (cl.begin(), cl.end(), -l);
    
    if (pos_it != cl.end() && neg_it != cl.end()){ 
        printSAT(f, UNSAT);
        exit(0);
    }

    for(auto& lit : cl){
      if(lit==l){
        cout<<"find it!\n";
      }
    }
  }

}

void unitRule(CNF& f){

  // for empty CNF
  if(f.clauses.empty()){
    printSAT(f, SAT);
    return;
  }

  for(auto& cl : f.clauses){
    
    // find unit clause
    if(cl.size()==1){
      cout<<"unit: "<<cl[0]<<endl;

      // set literal = 1 if unit clause is positive,
      //             = 0 if unit clause is negative.
      bool pos_clause = (cl[0]>0); 
      int lit = pos_clause ? cl[0] : -cl[0];
      f.literals[lit] = pos_clause ? 1 : 0; 
      
      // apply the assignment each clause 
      assignment(f, lit);
    }


  }

  cout<<"literals: ";
  for(auto& lit : f.literals){
    std::cout<<lit<<' ';           
  }
  cout<<endl;
} 
    // unit clause find
      // implication

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
  cout<<f.clauses.size()<<endl;
  
  f.literals.resize(maxVarIndex+1, -1);   // 1:true, 0:false, -1: not assigned 
  f.literal_freq.resize(maxVarIndex+1, 0);
  
  // BCP
  unitRule(f);


  // DPLL



  return 0;
}
