#ifndef H_MAKE_CARD
#define H_MAKE_CARD

#include <vector>

#include "small_tree_quick.hpp"

void GetCounts(size_t nbins,
	       double lumi,
	       small_tree_quick &tree,
	       std::vector<double> &low_cnt,
	       std::vector<double> &low_sqr,
	       std::vector<double> &hgh_cnt,
	       std::vector<double> &hgh_sqr);

void Fill(small_tree_quick &tree,
	  std::vector<double> &cnt,
	  std::vector<double> &sqr,
	  double weight);

double KappaUncert(const std::vector<double> &cnt,
		   const std::vector<double> &sqr);

#endif
