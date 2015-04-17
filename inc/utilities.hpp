//----------------------------------------------------------------------------
// utilities - Various functions used accross the code
//----------------------------------------------------------------------------

#ifndef H_UTILITIES
#define H_UTILITIES

#include <cstddef>
#include <cstdio>
#include <cmath>

#include <string>
#include <vector>

#include "TString.h"
#include "TTree.h"
#include "TGraph.h"
#include "TH1D.h"

typedef std::pair<int,double> int_double;
typedef std::pair<double,double> double_double;
const long double PI = acos(-1.L);

float cross_section(const TString &file);
std::vector<TString> dirlist(const TString &folder,
                             const TString &inname="dir",
                             const TString &tag="");
bool eigen2x2(float matrix[2][2], float &eig1, float &eig2);
bool id_big2small(const int_double& left, const int_double& right);
bool dd_big2small(const double_double& left, const double_double& right);
bool dd_small2big(const double_double& left, const double_double& right);
long double DeltaPhi(long double phi1, long double phi2);
long double SignedDeltaPhi(long double phi1, long double phi2);
float dR(float eta1, float eta2, float phi1, float phi2);
TString RoundNumber(double num, int decimals, double denom=1.);
long double AddInQuadrature(long double x, long double y);
long double GetMass(long double e, long double px, long double py, long double pz);
long double GetMT(long double m1, long double pt1, long double phi1,
                  long double m2, long double pt2, long double phi2);
long double GetMT(long double pt1, long double phi1,
                  long double pt2, long double phi2);
bool Contains(const std::string& text, const std::string& pattern);

std::vector<std::string> Tokenize(const std::string& input,
                                  const std::string& tokens=" ");
void get_count_and_uncertainty(TTree& tree,
                               const std::string& cut,
                               double& count,
                               double& uncertainty);
void AddPoint(TGraph& graph, const double x, const double y);

template<class T>
bool is_nan(const T &x){return x!=x;}

template<class T>
short Sign(T val){
  return (T(0) < val) - (val < T(0));
}

std::string execute(const std::string &cmd);
std::string RemoveTrailingNewlines(std::string str);

std::vector<double> LinearSpacing(size_t npts, double low, double high);
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
