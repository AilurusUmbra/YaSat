#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "parser.h"
#include "CNF.cpp"

using namespace std;

#ifndef STATE
#define UNSAT 0
#define SAT 1
#endif

string state[2] = {"s UNSATISFIABLE", "s SATISFIABLE"};


// absolute value
int abs(int x){
	return (x>0) ? x : -x;
}


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
        if(f.literals[lit]==0) // 0: false
          cout<<(-lit)<<' ';
				else        
          // 1: true, -1: dont care
          cout<<lit<<' ';
      }
      //ofile<<"0\n";
      cout<<"0\n";
    }
  }

  ofile.close();
  exit(0);
  return; 
}

/* for debugging
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
*/

// BCP 
void assignment(CNF& f, int l){

  int lit_value = f.literals[l];

  int idx=0;
  for(int cl=0; cl<f.clauses.size(); ++cl){
  idx=0; 
    for(int lit=0; lit<f.clauses[cl].size(); ++lit){
    //cout<<"["<<cl<<"]["<<idx++<<"]"<<"lit: "<<lit<<endl;
      if(f.clauses[cl][lit]==l){
 //       cout<<"find pos lit in clauses!\n";
        if(lit_value==1){
   //       cout<<"POS: lit_value is 1, remove this clause\n";
          // l is true, then remove this clause
          try {
            f.clauses.erase(f.clauses.begin()+cl);
            f.clauses.shrink_to_fit(); 
          } catch (int e){
     //       cout << "An exception occurred. Exception Nr. " << e << '\n';
          }
          --cl;
          // SAT if CNF(clauses) is empty
          if(f.clauses.empty()){
            printSAT(f, SAT);
            return;
          }
          break;
        }
        else if (lit_value==0){
       //   cout<<"POS: lit_value is 0, remove this literal\n";
          // l is false, remove this literal
          f.clauses[cl].erase(f.clauses[cl].begin()+lit);
          
          // UNSAT if this clause(literals) is empty
          if(f.clauses[cl].empty()){
            printSAT(f, UNSAT);
            return;
          }
          break;
        }
      }
      else if((-f.clauses[cl][lit])==l){
        if(lit_value==0){
          //l is true, then remove this clause
          f.clauses.erase(f.clauses.begin()+cl);
            f.clauses.shrink_to_fit(); 
          --cl;
          // SAT if CNF(clauses) is empty
          if(f.clauses.empty()){
            printSAT(f, SAT);
            return;
          }
          break;
        }
        else if (lit_value==1){
          // l is false, remove this literal
          f.clauses[cl].erase(f.clauses[cl].begin()+lit);
          // UNSAT if this clause(literals) is empty
          if(f.clauses[cl].empty()){
            printSAT(f, UNSAT);
            return;
          }
          break;
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

        // set literal = 1 if unit clause is positive,
        //             = 0 if unit clause is negative.
        bool pos_clause = (cl[0]>0); 
        int lit = pos_clause ? cl[0] : -cl[0];
        f.literals[lit] = pos_clause ? 1 : 0; 
        
        // apply the assignment each clause 
        assignment(f, lit);
      }


    }
  } while(find_unit);

  return;
} 


void DPLL(CNF& f){

  // BCP
  unitRule(f);
  
  // if reach here, it means CNF is not solved yet.
  
// need to apply twice, once true, the other false
  int mostAppear = distance(
      f.literal_freq.begin(),
      max_element(f.literal_freq.begin(), f.literal_freq.end()));		
  
	for (int j = 0; j < 2; j++) {
    CNF f_(f); // copy the formula before recursing
    f_.literals[mostAppear] = j; 	 
    f_.literal_freq[mostAppear] = -1; // reset the frequency to -1 to ignore in the future
    assignment(f_, mostAppear); // apply the change to all the clauses
    DPLL(f_); // recursively call DPLL on the new formula
  }
  return;
}


int main(int argc, char* argv[]){

  CNF f; 
  int maxVarIndex=0;
  parse_DIMACS_CNF(f.clauses, maxVarIndex, argv[1]);
  
  // move "*.cnf" to "*.sat" 
  outFileName = string(argv[1]);
  string satExtent = ".sat";
  outFileName.replace(outFileName.end()-4,outFileName.end(), satExtent);   


  f.literals.resize(maxVarIndex+1, -1);   // 1:true, 0:false, -1: not assigned 
  f.literal_freq.resize(maxVarIndex+1, 0); // dont use [0] element	
	f.literal_freq[0] = -1; // dont use [0] element	
	
	
	// count freq
  for(auto& cl : f.clauses){
    for(auto& l : cl){
    	++f.literal_freq[abs(l)];
		}
  }
  
  //print_clauses(f);
	//f.print_freq();
  
  // check for (a v a')
  for(auto& cl : f.clauses){
    for(auto& l : cl){
      vector<int>::iterator pos_it;
      pos_it = find (cl.begin(), cl.end(), l);

      vector<int>::iterator neg_it;
      neg_it = find (cl.begin(), cl.end(), -l);
      
      if (pos_it != cl.end() && neg_it != cl.end()){ 
          printSAT(f, UNSAT);
      }
    }
  }

  // DPLL
  DPLL(f);

  return 0;
}
