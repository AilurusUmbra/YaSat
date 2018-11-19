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


#include<fstream>

// print SAT
string outFileName;
void printSAT(CNF& f, int rst){

  ofstream ofile;
  ofile.open(outFileName, ios::out);
  if(ofile.fail()){
    cout<<"fail to open output file...\n";
    exit(3);
  } else {
    
    //ofile<<state[rst]<<endl;
    cout<<state[rst]<<endl;
   
    // if SAT, print variables;
    if(rst){
      //ofile<<"v ";
      cout<<"v ";
      for(auto lit=1; lit<f.literals.size(); ++lit){
        //ofile<<lit<<' ';           
        if(f.literals[lit]==1)
          cout<<lit<<' ';
        else if(f.literals[lit]==0)
          cout<<(-lit)<<' ';
      }
      //ofile<<"0\n";
      cout<<"0\n";
    }
  }

  ofile.close();
  return; 
}

void print_clauses(CNF& f){

  int idx=0;
  std::cout<<"print clauses: \n";      
  for(auto& clause : f.clauses){
    cout<<"cl["<<idx++<<"]: ";
    for(auto& literal : clause){         
      std::cout<<literal<<' ';           
    }                                    
    std::cout<<"\n";                     
  }                                      
  cout<<"clauses size: "<<f.clauses.size()<<endl;
  cout<<"--------------------\n";
}


// BCP 
void assignment(CNF& f, int l){

  int lit_value = f.literals[l];

  int idx=0;
  for(int cl=0; cl<f.clauses.size(); ++cl){
  idx=0; 
    for(int lit=0; lit<f.clauses[cl].size(); ++lit){
    cout<<"["<<cl<<"]["<<idx++<<"]"<<"lit: "<<lit<<endl;
      if(f.clauses[cl][lit]==l){
        cout<<"find pos lit in clauses!\n";
        if(lit_value==1){
          cout<<"POS: lit_value is 1, remove this clause\n";
          // l is true, then remove this clause
          try {
            f.clauses.erase(f.clauses.begin()+cl);
            f.clauses.shrink_to_fit(); 
          } catch (int e){
            cout << "An exception occurred. Exception Nr. " << e << '\n';
          }
          print_clauses(f);
          --cl;
          // SAT if CNF(clauses) is empty
          if(f.clauses.empty()){
            printSAT(f, SAT);
            return;
          }
        }
        else if (lit_value==0){
          cout<<"POS: lit_value is 0, remove this literal\n";
          // l is false, remove this literal
          f.clauses[cl].erase(f.clauses[cl].begin()+lit);
          print_clauses(f);
          
          // UNSAT if this clause(literals) is empty
          if(f.clauses[cl].empty()){
            printSAT(f, UNSAT);
            return;
          }
        }
      }
      else if((-f.clauses[cl][lit])==l){
        //cout<<f.clauses[cl][lit]<<endl;
        cout<<"find neg lit in clauses!\n";
        if(lit_value==0){
          //l is true, then remove this clause
          f.clauses.erase(f.clauses.begin()+cl);
            f.clauses.shrink_to_fit(); 
          print_clauses(f);
          --cl;
          // SAT if CNF(clauses) is empty
          if(f.clauses.empty()){
            printSAT(f, SAT);
            return;
          }

        }
        else if (lit_value==1){
          // l is false, remove this literal
          f.clauses[cl].erase(f.clauses[cl].begin()+lit);
          print_clauses(f);
          // UNSAT if this clause(literals) is empty
          if(f.clauses[cl].empty()){
            printSAT(f, UNSAT);
            return;
          }
        }

      }
    }
  }
  cout<<"assign finish\n";
  return;
}

void unitRule(CNF& f){

  // for empty CNF
  if(f.clauses.empty()){
    printSAT(f, SAT);
    return;
  }

  bool find_unit=false;

  do{
    find_unit=false;
    for(auto& cl : f.clauses){
      // find unit clause
      if(cl.size()==1){
        find_unit=true;
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

  } while(find_unit);

  return;
} 
    // unit clause find
      // implication

int main(int argc, char* argv[]){

  CNF f; 

  // maybe for random start
    
  int maxVarIndex=0;

  parse_DIMACS_CNF(f.clauses, maxVarIndex, argv[1]);
  // move "*.cnf" to "*.sat" 
  outFileName = string(argv[1]);
  string satExtent = ".sat";
  outFileName.replace(outFileName.end()-4,outFileName.end(), satExtent);   
  


  //cout<<"maxVarIndex: "<<maxVarIndex<<endl;

  print_clauses(f);

  f.literals.resize(maxVarIndex+1, -1);   // 1:true, 0:false, -1: not assigned 
  f.literal_freq.resize(maxVarIndex+1, 0);
  
  for(auto& cl : f.clauses){
    for(auto& l : cl){
      // check for (a v a')
      vector<int>::iterator pos_it;
      pos_it = find (cl.begin(), cl.end(), l);

      vector<int>::iterator neg_it;
      neg_it = find (cl.begin(), cl.end(), -l);
      
      if (pos_it != cl.end() && neg_it != cl.end()){ 
          printSAT(f, UNSAT);
          exit(0);
      }
    }
  }
    
  // BCP
  unitRule(f);


  // DPLL



  return 0;
}
