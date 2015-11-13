#include <iostream>
#include <vector>

#include "TString.h"

#include "utilities_macros_rohan.cpp"

using namespace std;

int main(){ 

  TString folder="/afs/cern.ch/work/m/manuelf/public/babies/2015_10_19/mc/skim_1lht500met200/";
  vector<TString> s_tt; 
  s_tt.push_back(folder+"*TTJets*Lept*");
  s_tt.push_back(folder+"*TTJets*HT*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*");


  // Reading ntuples
  vector<sfeats> Samples;
  Samples.push_back(sfeats(s_tt, "2 true l t#bar{t}, mt > 140", 1,0, "mt>140&&ntruleps==2")); Samples.back().isData = true; 
  Samples.push_back(sfeats(s_tt, "1 true l t#bar{t}, mt #leq 140", 31,1,"mt<=140&&ntruleps==1"));
  Samples.push_back(sfeats(s_t1t, "T1tttt NC", 2));
  Samples.push_back(sfeats(s_t1tc, "T1tttt C", 2,2));

  vector<int> mt140;
  mt140.push_back(0);
  mt140.push_back(1);
  mt140.push_back(2);

  TString noCuts = "1";
  TString baseline = "ht>500&&met>200&&njets>=6&&nbm>=1&&nleps==1";

  vector<hfeats> vars;

  vars.push_back(hfeats("mj",30,0,1500, mt140, "MJ",baseline,400,"")); vars.back().maxRatio = 0.3;

  //NOTE: Make sure RohanHack is turned off
  plot_distributions(Samples, vars, "3", ".pdf", "RA4","",true);
}
