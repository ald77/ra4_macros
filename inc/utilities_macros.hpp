//----------------------------------------------------------------------------
// utilities_macros - Various functions used accross the code
//----------------------------------------------------------------------------

#ifndef H_UTILITIES_MACROS
#define H_UTILITIES_MACROS

#include <vector>

#include "TString.h"
#include "TH1D.h"

class hfeats {
public:
  hfeats(TString ivarname, int inbins, float iminx, float imaxx, std::vector<int> isamples,
	 TString ititle="", TString icuts="1", float icut=-1);
  hfeats(TString ivarname, int inbins, float* ibinning, std::vector<int> isamples,
	 TString ititle="", TString icuts="1", float icut=-1);
  TString title, varname, tag, cuts, unit;
  int nbins;
  float *binning;
  float minx, maxx, cut;
  std::vector<int> samples;
  void format_tag();
};

class sfeats {
public:
  sfeats(std::vector<TString> ifile, TString ilabel, int icolor, int istyle=1, TString icut="1");
  std::vector<TString> file;
  TString label, cut, factor;
  int color, style;
  bool isSig;
};

void calc_chi2_diff(TH1D *histo1, TH1D *histo2, float &chi2, int &ndof, float &pvalue, float *average);
void calc_chi2(TH1D *histo, float &chi2, int &ndof, float &pvalue, float &average);


#endif
