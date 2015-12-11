#include <iostream>
#include <vector>

#include "TString.h"

#include "utilities_macros.cpp"

using namespace std;

int main(){ 

  TString folder="/afs/cern.ch/user/m/manuelf/work/babies/2015_11_28/mc/skim_1lht500met200/";
  vector<TString> s_tt, s_t4t_nc; 
  s_tt.push_back(folder+"*TTJets*Lept*");
  s_tt.push_back(folder+"*TTJets*HT*");
  s_t4t_nc.push_back(folder+"*T1tttt*1500*100");

  // Reading ntuples
  vector<sfeats> Samples;
  Samples.push_back(sfeats(s_tt, "2 true lepton t#bar{t}", ra4::c_tt_2l,1,"ntruleps==2&&stitch"));
  Samples.push_back(sfeats(s_tt, "1 true lepton t#bar{t}",ra4::c_tt_1l,1,"ntruleps==1&&stitch"));
  Samples.push_back(sfeats(s_t4t_nc, "#tilde{g}#tilde{g}, #tilde{g} #rightarrow t#bar{t}#tilde{#chi}_{1}^{0} (1500,100)",ra4::c_t1tttt,1, "stitch"));

  vector<int> mjisr;
  mjisr.push_back(0);
  mjisr.push_back(1);
  mjisr.push_back(2);

  TString cuts = "ht>500&&met>200&&nleps>=1";

  vector<hfeats> vars;

  vars.push_back(hfeats("mj",20,0,1000, mjisr,"M_{J} [GeV]",cuts+"&&abs(isr_tru_pt)<10",346,""));
  vars.push_back(hfeats("mj",20,0,1000, mjisr,"M_{J} [GeV]",cuts+"&&abs(isr_tru_pt)>100",346,""));

  //NOTE: Make sure RohanHack is turned off
  plot_distributions(Samples, vars, "3", ".pdf", "CMSPaper","",false);
}
