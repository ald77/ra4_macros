// small_tree_quick: quick version of small_tree to handle reduce tree ntuples
// File generated with generate_small_tree.exe

#ifndef H_SMALL_TREE_QUICK
#define H_SMALL_TREE_QUICK

#include <vector>
#include <string>

#include "TTree.h"
#include "TChain.h"

#include "small_tree.hpp"

class small_tree_quick : public small_tree{
public:
  small_tree_quick(); // Constructor to create tree
  small_tree_quick(const std::string &filename); // Constructor to read tree

  virtual void GetEntry(const long entry);

  virtual void Fill();

  virtual std::string Type() const;

  virtual ~small_tree_quick();


private:
};

#endif
