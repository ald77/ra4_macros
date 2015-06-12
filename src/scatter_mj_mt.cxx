#include "scatter_mj_mt.hpp"

#include <cstdlib>

#include <string>
#include <sstream>
#include <set>
#include <vector>

#include <unistd.h>
#include <getopt.h>

#include "TColor.h"
#include "TPaveText.h"
#include "TMath.h"
#include "TGraph.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"
#include "TRandom3.h"

#include "small_tree_quick.hpp"
#include "timer.hpp"
#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;

namespace{
  double met_min = 200.;
  double met_max = 0.;
  int njets_min = 7;
  int njets_max = 0;
  bool merge_ttbar = false;
  bool compressed = false;
  bool no_signal = false;
  bool full_stats = false;
}

//Not sure why I can't get the colors from utilities_macros...
TColor c_tt_2l(1006, 86/255.,160/255.,211/255.);
TColor c_tt_1l(1000, 1/255.,57/255.,166/255.);

int main(int argc, char *argv[]){
  TRandom3 rand3(0);
  GetOptions(argc, argv);
  
  styles style("2Dnobar");
  style.setDefaultStyle();
  
  string folder = "/cms5r0/ald77/archive/2015_05_25/skim/";
  string sig_name = compressed ? "*T1tttt*1200*800*":"*T1tttt*1500*100*";
  small_tree_quick st_sig(folder+sig_name);
  small_tree_quick st_bkg(folder+"*TTJets*");

  double mj_max = 1000.;
  double mt_max = 400.;

  TGraph g_sig, g_bkg, g_bkg1, g_bkg2;
  TGraph g_sig_full, g_bkg_full, g_bkg1_full, g_bkg2_full;
  TH2D h("h", ";M_{J} [GeV];m_{T} [GeV]", 1, 0., mj_max, 1, 0., mt_max);
  
  int line_width = 4;
  TLine l_mj(400.,0.,400.,mt_max);
  TLine l_mt(0.,140.,mj_max,140.);
  l_mj.SetLineWidth(line_width);
  l_mt.SetLineWidth(line_width);

  double ttbar_norm = 1.;
  double sig_norm = 1.;
  if(full_stats){
    ttbar_norm = std::numeric_limits<float>::max();
    sig_norm = 100.;
  }
  Process(st_sig, g_sig, g_sig_full, rand3, 2, 20, 1, sig_norm);
  if(merge_ttbar){
    Process(st_bkg, g_bkg, g_bkg_full, rand3, 1006, 21, 1, ttbar_norm);
  }else{
    Process(st_bkg, g_bkg1, g_bkg1_full, rand3, 1000, 23, 1, ttbar_norm, 1);
    Process(st_bkg, g_bkg2, g_bkg2_full, rand3, 1006, 22, 1, ttbar_norm, 2);
  }

  double rho_sig = g_sig_full.GetCorrelationFactor();
  double rho_bkg = g_bkg_full.GetCorrelationFactor();
  double rho_bkg1 = g_bkg1_full.GetCorrelationFactor();
  double rho_bkg2 = g_bkg2_full.GetCorrelationFactor();

  TLegend l(style.PadLeftMargin, 1.-style.PadTopMargin, 1.-style.PadRightMargin, 1.0);
  if(merge_ttbar){
    l.SetNColumns(2);
  }else{
    l.SetNColumns(3);
  }
  l.SetFillColor(0);
  l.SetFillStyle(4000);
  l.SetBorderSize(0);
  if(merge_ttbar){
    l.AddEntry(&g_bkg, GetLabel("ttbar",rho_bkg).c_str(), "p");
  }else{
    l.AddEntry(&g_bkg1, GetLabel("ttbar (1l)",rho_bkg1).c_str(), "p");
    l.AddEntry(&g_bkg2, GetLabel("ttbar (2l)",rho_bkg2).c_str(), "p");
  }
  if(!no_signal){
    l.AddEntry(&g_sig, GetLabel((compressed?"T1tttt(1200,800)":"T1tttt(1500,100)"),rho_sig).c_str(), "p");
  }

  double height = 0.125;
  double width = 0.125;
  TPaveText l1(style.PadLeftMargin, style.PadBottomMargin,
	       style.PadLeftMargin+width, style.PadBottomMargin+height, "NDCNB");
  TPaveText l2(1.-style.PadRightMargin-width, style.PadBottomMargin,
	       1.-style.PadRightMargin, style.PadBottomMargin+height, "NDCNB");
  TPaveText l3(style.PadLeftMargin, 1.-style.PadTopMargin-height,
	       style.PadLeftMargin+width, 1.-style.PadTopMargin, "NDCNB");
  TPaveText l4(1.-style.PadRightMargin-width, 1.-style.PadTopMargin-height,
	       1.-style.PadRightMargin, 1.-style.PadTopMargin, "NDCNB");

  l1.AddText("R1");
  l2.AddText("R2");
  l3.AddText("R3");
  l4.AddText("R4");

  SetStyle(l1);
  SetStyle(l2);
  SetStyle(l3);
  SetStyle(l4);

  TCanvas c;
  h.Draw();
  if(merge_ttbar){
    g_bkg.Draw("psame");
  }else{
    g_bkg1.Draw("psame");
    g_bkg2.Draw("psame");
  }
  if(!no_signal){
    g_sig.Draw("psame");
  }
  l_mj.Draw("same");
  l_mt.Draw("same");
  l.Draw("same");
  l1.Draw("same");
  l2.Draw("same");
  l3.Draw("same");
  l4.Draw("same");

  ostringstream outname;
  outname << "plots/scat_mj_mt_met_"
	  << met_min << '_' << met_max
	  << "_njets_" << njets_min << '_' << njets_max
	  << (merge_ttbar?"_merged":"_split")
	  << (no_signal ? "_no_signal" : (compressed ? "_T1tttt_1200_800" : "_T1tttt_1500_100"))
	  << (full_stats ? "_shapes" : "_lumi")
	  << ".pdf";
  c.Print(outname.str().c_str());
}

void GetRandomPoints(TRandom3 &rand3, const vector<double> &x_in, const vector<double> &y_in,
		     vector<double> &x_out, vector<double> &y_out){
  if(x_out.size() != y_out.size() || x_in.size() != y_in.size()
     || x_out.size() > x_in.size() || y_out.size() > y_in.size()){
    x_out = x_in;
    y_out = y_in;
    return;
  }
  
  set<size_t> indices;
  while(indices.size() < x_out.size()){
    indices.insert(rand3.Integer(x_in.size()));
  }

  x_out.resize(indices.size());
  y_out.resize(indices.size());
  size_t i = 0;
  for(set<size_t>::const_iterator it = indices.begin();
      it != indices.end();
      ++it, ++i){
    x_out.at(i) = x_in.at(*it);
    y_out.at(i) = y_in.at(*it);
  }
}

void Process(small_tree_quick &st, TGraph &g, TGraph &g_full, TRandom3 &rand3,
	     int color, int marker, int size, double norm, int nleps){
  g_full = TGraph(0);
  int num_entries = st.GetEntries();
  Timer timer(num_entries, 1.);
  timer.Start();
  double sum = 0.;
  int raw = 0;
  vector<double> mj, mt;
  for(int entry = 0; entry < num_entries; ++entry){
    timer.Iterate();
    st.GetEntry(entry);

    if(false
       || st.nbm()<2
       || st.njets()<njets_min
       || (njets_max > 0 && st.njets()>njets_max)
       || st.met()<=met_min
       || (met_max > 0. && st.met()>met_max)
       || st.ht()<=500.
       || (st.nmus()+st.nels())!=1
       || ((nleps == 1 && st.ntruleps()>1) || (nleps == 2 && st.ntruleps()<2))
       ) continue;

    sum += 10.*st.weight();
    ++raw;
    mj.push_back(st.mj());
    mt.push_back(st.mt());
    AddPoint(g_full, st.mj(), st.mt());
  }

  int num_points = TMath::Nint(sum*norm);
  if(num_points < 0 || num_points > raw) num_points = raw;
  
  vector<double> mj_out(num_points), mt_out(num_points);
  GetRandomPoints(rand3, mj, mt, mj_out, mt_out);

  g = TGraph(num_points, &mj_out.at(0), &mt_out.at(0));
  g.SetLineColor(color);
  g.SetFillColor(color);
  g.SetMarkerColor(color);
  g.SetMarkerStyle(marker);
  g.SetMarkerSize(size);
}

string GetLabel(const string &str, double rho){
  ostringstream oss;
  oss.precision(2);
  oss << str << ", #rho=" << rho << flush;
  return oss.str();
}

void GetOptions(int argc, char *argv[]){
  while(true){
    static struct option long_options[] = {
      {"met_min", required_argument, 0, 0},
      {"met_max", required_argument, 0, 0},
      {"njets_min", required_argument, 0, 0},
      {"njets_max", required_argument, 0, 0},
      {"merge_ttbar", no_argument, 0, 0},
      {"compressed", no_argument, 0, 0},
      {"no_signal", no_argument, 0, 0},
      {"full_stats", no_argument, 0, 0},
      {0, 0, 0, 0}
    };

    char opt = -1;
    int option_index;
    opt = getopt_long(argc, argv, "", long_options, &option_index);
    if(opt == -1) break;

    string optname;
    switch(opt){
    case 0:
      optname = long_options[option_index].name;
      if(optname == "met_min"){
	met_min = atof(optarg);
      }else if(optname == "met_max"){
	met_max = atof(optarg);
      }else if(optname == "njets_min"){
	njets_min = atoi(optarg);
      }else if(optname == "njets_max"){
	njets_max = atoi(optarg);
      }else if(optname == "merge_ttbar"){
	merge_ttbar = true;
      }else if(optname == "compressed"){
	compressed = true;
      }else if(optname == "no_signal"){
	no_signal = true;
      }else if(optname == "full_stats"){
	full_stats = true;
      }
      break;
    default: break;
    }
  }
}

void SetStyle(TPaveText &pt){
  pt.SetFillColor(0);
  pt.SetFillStyle(4000);
  pt.SetBorderSize(0);
  pt.SetTextColorAlpha(1,0.5);
}
