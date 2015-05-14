// plot_note: Macro that plots variables both lumi weighted and normalized to the same area.

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TColor.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  TString folder="/cms5r0/ald77/archive/2015_05_10/skim/";
  TString folder_noskim="/cms5r0/ald77/archive/2015_05_10/";
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");
  vector<TString> s_t1t_noskim;
  s_t1t_noskim.push_back(folder_noskim+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc_noskim;
  s_t1tc_noskim.push_back(folder_noskim+"*T1tttt*1200_*PU20*");


  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_tt_noskim;
  s_tt_noskim.push_back(folder_noskim+"*_TTJet*12.root");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*WJets*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder+"*_T*channel*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder+"*TTW*");
  s_ttv.push_back(folder+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD_HT*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");

  // Reading ntuples
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_t1t, "T1tttt(1500,100)", ra4::c_t1tttt));
  Samples.push_back(sfeats(s_t1tc, "T1tttt(1200,800)", ra4::c_t1tttt,2));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", ra4::c_tt_1l, 1,
			   "((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)<=1"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", ra4::c_tt_2l,1,
			   "((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)>=2"));
  Samples.push_back(sfeats(s_wjets, "W+jets", ra4::c_wjets));
  Samples.push_back(sfeats(s_singlet, "Single t", ra4::c_singlet));
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_other, "Other", ra4::c_other));

  Samples.push_back(sfeats(s_t1t_noskim, "T1tttt(1500,100)", ra4::c_t1tttt));
  Samples.push_back(sfeats(s_t1tc_noskim, "T1tttt(1200,800)", ra4::c_t1tttt,2));
  Samples.push_back(sfeats(s_tt_noskim, "t#bar{t}", ra4::c_tt_1l,1));

  vector<int> ra4_sam;
  ra4_sam.push_back(0);
  ra4_sam.push_back(1);
  ra4_sam.push_back(2);
  ra4_sam.push_back(3);
  ra4_sam.push_back(4);
  ra4_sam.push_back(5);
  ra4_sam.push_back(6);
  ra4_sam.push_back(7);

  vector<int> ra4_tt_t1;
  ra4_tt_t1.push_back(0);
  ra4_tt_t1.push_back(1);
  ra4_tt_t1.push_back(2);

  vector<int> ra4_tt_t1_noskim;
  ra4_tt_t1_noskim.push_back(8);
  ra4_tt_t1_noskim.push_back(9);
  ra4_tt_t1_noskim.push_back(10);


  vector<hfeats> vars;

  vars.push_back(hfeats("Max$(mc_pt*(abs(mc_id)==13))",60,0,300, ra4_tt_t1_noskim, "Leading gen #mu p_{T} (GeV)",
			"Sum$(abs(mc_id)==13)>0",20,"muon"));
  vars.push_back(hfeats("Max$(mc_pt*(abs(mc_id)==11))",60,0,300, ra4_tt_t1_noskim, "Leading gen e p_{T} (GeV)",
			"Sum$(abs(mc_id)==11)>0",20,"electron"));

  plot_distributions(Samples, vars, "10", ".pdf", "RA4");


}

