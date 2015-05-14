//----------------------------------------------------------------------------
// utilities_macros - Various functions used accross the code
//----------------------------------------------------------------------------

#ifndef H_UTILITIES_MACROS
#define H_UTILITIES_MACROS

#include <vector>

#include "TString.h"
#include "TH1D.h"

namespace ra4{
  // Had to define the TColor objects in the cpp
  enum {
    c_t1tttt   = 2,
    c_tt_1l    = 1000, // ucsb_blue
    c_tt_2l    = 1006, // tar_heel_blue
    c_wjets    = 1001, // ucsb_gold
    c_singlet  = kGreen+3,
    c_ttv      = 1002, // penn_red
    c_other    = kBlue+4
  };

}

struct pfeats{
  pfeats(const std::vector<int> &isamples, const TString &icut = "1", const TString &itagname="");

  std::vector<int> samples;
  TString cut, tagname;
};

class hfeats {
public:
  hfeats(TString ivarname, int inbins, float iminx, float imaxx, std::vector<int> isamples,
         TString ititle="", TString icuts="1", float icut=-1, TString itagname="");
  hfeats(TString ivarname, int inbins, float* ibinning, std::vector<int> isamples,
         TString ititle="", TString icuts="1", float icut=-1, TString itagname="");
  TString title, varname, tag, cuts, unit;
  int nbins;
  float *binning;
  float minx, maxx, cut;
  std::vector<int> samples;
  TString tagname;
  void format_tag();
};

class sfeats {
public:
  sfeats(std::vector<TString> ifile, TString ilabel, int icolor, int istyle=1, TString icut="1");
  std::vector<TString> file;
  TString label, cut, factor,tag;
  int color, style;
  bool isSig;
};

void calc_chi2_diff(TH1D *histo1, TH1D *histo2, float &chi2, int &ndof, float &pvalue, float *average);
void calc_chi2(TH1D *histo, float &chi2, int &ndof, float &pvalue, float &average);

void plot_distributions(std::vector<sfeats> Samples, std::vector<hfeats> vars, TString luminosity="10", 
			TString filetype=".eps", TString namestyle="LargeLabels");

#endif
