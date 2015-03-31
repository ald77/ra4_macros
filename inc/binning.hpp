#ifndef H_BINNING
#define H_BINNING

#include "small_tree_quick.hpp"

void Fill(double &counts, double &squares, const small_tree_quick &tree);
double PredUncert(double h, double sh, double l, double sl);
void GetCounts(small_tree_quick &tree, double &low, double &low_sw, double &high, double &high_sq);
void PrintLine(double h, double sh, double l, double sl, double sig, double ssig);
bool IsoTkChargeVeto(const small_tree_quick &tree);
int CountLepsNew(const small_tree_quick &tree,
                 float &mt,
		 int &charge,
		 bool &is_el,
		 size_t &the_lep);

#endif
