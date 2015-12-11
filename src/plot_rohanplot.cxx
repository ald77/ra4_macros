#include <iostream>
#include <vector>

#include "TString.h"

#include "utilities_macros_rohan.cpp"

using namespace std;

int main(){ 

  TString folder="/afs/cern.ch/user/m/manuelf/work/babies/2015_11_28/mc/bkg/skim_1lht500met200/";
  vector<TString> s_tt; 
  s_tt.push_back(folder+"*TTJets*Lept*");
  s_tt.push_back(folder+"*TTJets*HT*");

  // Reading ntuples
  vector<sfeats> Samples;
  Samples.push_back(sfeats(s_tt, "2 true, 1 reco lepton t#bar{t}: m_{T} > 140", kViolet+9,25, "mt>140&&ntruleps==2&&stitch")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt, "1 true, 1 reco lepton t#bar{t}: m_{T} > 140", kViolet+9,25,"mt>140&&ntruleps==1&&stitch")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt, "2 true, 1 reco lepton t#bar{t}: m_{T} #leq 140", 31,1, "mt<=140&&ntruleps==2&&stitch"));
  Samples.push_back(sfeats(s_tt, "1 true, 1 reco lepton t#bar{t}: m_{T} #leq 140", 31,1,"mt<=140&&ntruleps==1&&stitch"));

  vector<int> mt140;
  mt140.push_back(0);
  mt140.push_back(3);

  vector<int> mt140_2l;
  mt140_2l.push_back(0);
  mt140_2l.push_back(2);

  vector<int> mt140_1l;
  mt140_1l.push_back(1);
  mt140_1l.push_back(3);

  TString noCuts = "1";
  TString baseline = "ht>500&&met>200&&njets>=6&&nbm>=1&&nleps==1";

  vector<hfeats> vars;

  vars.push_back(hfeats("mj",30,0,1500, mt140,    "M_{J} [GeV]",baseline,400,"")); vars.back().maxRatio = 0.29;
  vars.push_back(hfeats("mj",30,0,1500, mt140_2l, "M_{J} [GeV]",baseline,400,"dilep")); vars.back().maxRatio = 0.99;
  vars.push_back(hfeats("mj",30,0,1500, mt140_1l, "M_{J} [GeV]",baseline,400,"singlelep")); vars.back().maxRatio = 0.29;

  //NOTE: Make sure RohanHack is turned off
  plot_distributions(Samples, vars, "3", ".pdf", "CMSPaper","",true);
  plot_distributions(Samples, vars, "3", ".root", "CMSPaper","",true);
}
