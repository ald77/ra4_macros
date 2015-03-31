// small_tree_quick: quick version of small_tree to handle reduce tree ntuples
//File generated with generate_small_tree.exe

#include "small_tree.hpp"

#include "small_tree_quick.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"

using namespace std;

small_tree_quick::small_tree_quick():
  small_tree(){
}

small_tree_quick::small_tree_quick(const string &filename):
  small_tree(filename){
}

void small_tree_quick::Fill(){
  small_tree::Fill();
  //Resetting variables
}

string small_tree_quick::Type() const{
  return "quick";
}

small_tree_quick::~small_tree_quick(){
}

void small_tree_quick::GetEntry(const long entry){
  small_tree::GetEntry(entry);

}

