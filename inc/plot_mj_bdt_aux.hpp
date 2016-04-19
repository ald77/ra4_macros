// plot_mj_bdt_aux: trains various BDTs which combine MJ, njets, and HT. Plots ROCs with them

#ifndef H_PLOT_MJ_BDT_AUX
#define H_PLOT_MJ_BDT_AUX

#include <vector>
#include "TH1D.h"
#include "TGraph.h"
#include "TString.h"

class marker_class {
public:
  marker_class(float icut, float isize, int icolor, int istyle);
  float cut, size;
  int color, style;
};

class sample_class {
public:
  sample_class(TString ilabel, std::vector<TString> ifiles, TString icut="1");
  TString label, cut;
  std::vector<TString> files;
};

class var_class {
public:
  var_class(std::vector<sample_class> samples, TString ivarname, float iminx, float imaxx, TString ititle, 
	    int icolor, int istyle=1, std::vector<marker_class> icuts=std::vector<marker_class>());
  TString title, varname;
  float minx, maxx;
  std::vector<marker_class> cuts;
  int color, style;
  std::vector<sample_class> samples;
};

class bdtvar_class {
public:
  bdtvar_class(TString variable, TString name, char type, TString unit="GeV");
  TString variable, name;
  char type;
  TString unit;
};

class bdt_class {
public:
  bdt_class(std::vector<bdtvar_class> ivars, TString isignal, TString ibkg);
  std::vector<bdtvar_class> vars;
  TString signal, bkg, name;
};

TGraph MakeROC(TH1D &good, TH1D &bad, const bool less_is_better, std::vector<marker_class> cuts);
void DrawROC(std::vector<var_class> vars, TString cuts, TString tag);
void plotROC(TString bdtfolder);
void trainBDT(TString ntufolder, TString bdtfolder);




#endif
