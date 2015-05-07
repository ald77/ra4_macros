#ifndef H_PLOT_PIE
#define H_PLOT_PIE

#include <vector>

#include "TString.h"

#include "utilities_macros.hpp"

class TChain;

void DrawPie(const pfeats &pfeat, const std::vector<sfeats> &samples, const std::vector<TChain*> &chains);

TString FormatName(TString name);

#endif
