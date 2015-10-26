#ifndef H_SYST_COUNTS_V2
#define H_SYST_COUNTS_V2

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "gamma_params.hpp"
#include "small_tree_quick.hpp"
#include "TH1.h"
#include "utilities_macros.hpp"

void PrintTable(TString method, std::vector< std::vector< std::vector<double> > > ttbar_counts, std::vector< std::vector< std::vector<double> > > ttbar_errors, std::vector< std::vector< std::vector<double> > > ttbar_counts_dilep, std::vector< std::vector< std::vector<double> > > other_counts,std::vector< std::vector< std::vector<double> > > other_counts_dilep, std::vector< std::vector<double> > delkappas, std::vector< std::vector<TString> > bins_by_region);
double GetWeight(small_tree_quick &tree, TString sysname, int ivar);

void GetCounts(double lumi, std::vector<TString> sysnames, TString method, TString baseline,
               small_tree_quick &tree,
               std::vector< std::vector< std::vector<double> > > &finalcounts,
               std::vector< std::vector< std::vector<double> > > &errors, std::vector< std::vector<TString> > bins_by_region, std::vector<hfeats> hists, std::vector< std::vector<TH1F*> > &h);

void OverlayHists(std::vector<TH1F*> h, hfeats hist, TString method, TString sysname, TString baseline);
void OverlayHistsmT(std::vector<TH1F*> h, hfeats hist, TString method, TString sysname, TString baseline);
int FindSuperBin(TString cut, std::vector<TString> other_region);
int LookUpRegion(small_tree_quick &tree, TString method);
int LookUpBin(small_tree_quick &tree, int region, std::vector< std::vector<TString> > bins_by_region);
void MakePlots(TString axisname, std::vector<int> numerator_regions, std::vector<int> denominator_regions, std::vector< std::vector< std::vector<double> > > counts,
	       std::vector< std::vector< std::vector<double> > > errors, std::vector< std::vector<TString> > bins_by_region, TString baseline, TString method, std::vector<TString> sysnames,int sysnum, std::vector<double> &delkappas );
double sqr(double x);



#endif
