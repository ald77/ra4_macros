#ifndef H_SCATTER_MJ_MT
#define H_SCATTER_MJ_MT

#include<vector>
#include <string>

#include "TGraph.h"
#include "TRandom3.h"
#include "TPaveText.h"

#include "small_tree_quick.hpp"

void GetRandomPoints(TRandom3 &rand3, const std::vector<double> &x_in, const std::vector<double> &y_in,
		     std::vector<double> &x_out, std::vector<double> &y_out);

void Process(small_tree_quick &st, TGraph &g, TGraph &g_full, TRandom3 &rand,
	     int color, int marker, int size, double norm, int nleps = 0);

std::string GetLabel(const std::string &str, double rho);

void GetOptions(int argc, char *argv[]);

void SetStyle(TPaveText &pt);

#endif
