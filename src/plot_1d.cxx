// plot_1d: Macro that plots variables both lumi weighted and normalized to the same area.

#include <iostream>
#include <vector>

#include "TString.h"

#include "utilities_macros.hpp"

using namespace std;

int main(){ 

  TString folder="/cms5r0/ald77/archive/2015_05_10/skim/";
  TString folder_noskim="/cms5r0/ald77/archive/2015_05_10/";
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");
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

  vector<hfeats> vars;


  //////////////////// N-1 plots ////////////////////////////////
  // // Run without skim
  // vars.push_back(hfeats("met",20,0,800, ra4_sam, "MET (GeV)","ht>500&&nbm>=2&&njets>=6&&(nmus+nels)==1",200));
  // vars.push_back(hfeats("ht",35,0,3500, ra4_sam, "H_{T} (GeV)","met>200&&nbm>=2&&njets>=6&&(nmus+nels)==1",500));
  // // Run without skim


  vars.push_back(hfeats("mt",60,0,600, ra4_sam, "m_{T} (GeV)",
  			"ht>500&&met>200&&nbm>=2&&njets>=6&&(nmus+nels)==1",140));
  vars.push_back(hfeats("mt",60,0,600, ra4_sam, "m_{T} (GeV)",
  			"ht>500&&met>400&&nbm>=2&&njets>=6&&(nmus+nels)==1",140));
  vars.push_back(hfeats("met",20,200,1200, ra4_sam, "MET (GeV)",
  			"ht>500&&nbm>=2&&njets>=6&&mt>140&&(nmus+nels)==1",400));
  vars.push_back(hfeats("njets",18,-0.5,17.5, ra4_sam, "Number of 40 GeV jets",
  			"ht>500&&met>400&&nbm>=2&&mt>140&&(nmus+nels)==1",5.5));
  vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Number of b-tags (CSVM)",
  			"ht>500&&met>400&&njets>=6&&mt>140&&(nmus+nels)==1",1.5));
  vars.push_back(hfeats("mj",32,0,1600, ra4_sam, "M_{J} (GeV)",
  			"ht>500&&met>400&&nbm>=2&&njets>=6&&mt>140&&(nmus+nels)==1",500));

  vars.push_back(hfeats("mt",60,0,600, ra4_sam, "m_{T} (GeV)",
  			"ht>500&&met>400&&nbm>=2&&njets>=8&&(nmus+nels)==1",140));
  vars.push_back(hfeats("met",20,200,1200, ra4_sam, "MET (GeV)",
  			"ht>500&&nbm>=2&&njets>=8&&mt>140&&(nmus+nels)==1",400));
  vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Number of b-tags (CSVM)",
  			"ht>500&&met>400&&njets>=8&&mt>140&&(nmus+nels)==1",1.5));
  vars.push_back(hfeats("mj",32,0,1600, ra4_sam, "M_{J} (GeV)",
  			"ht>500&&met>400&&nbm>=2&&njets>=8&&mt>140&&(nmus+nels)==1",500));

  plot_distributions(Samples, vars, "10", ".eps", "RA4");

}

