#include <iostream>
#include <vector>

#include "TString.h"

#include "utilities_macros.cpp"

using namespace std;

int main(){ 

  TString folder="/cms2r0/babymaker/babies/2015_11_28/mc/skim_1lht500met200/";
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  folder = "/net/cms2"+folder;
  if(Contains(hostname, "lxplus")) folder="/cms2r0/babymaker/babies/2015_11_28/mc/skim_1lht500met200/";

  vector<TString> s_tt, s_t4t_nc; 
  s_tt.push_back(folder+"*TTJets*Lept*");
  s_tt.push_back(folder+"*TTJets*HT*");
  s_t4t_nc.push_back(folder+"*T1tttt*1500*100*.root");

  // Reading ntuples
  vector<sfeats> Samples;
  Samples.push_back(sfeats(s_t4t_nc, "#scale[0.92]{#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#bar{t}#tilde{#chi}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}} (1500,100)}",ra4::c_t1tttt,1, "stitch")); 
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton",dps::c_tt_1l,1,"ntruleps==1&&stitch"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps==2&&stitch"));
  
 
  vector<int> mjisr;
  mjisr.push_back(0);
  mjisr.push_back(1);
  mjisr.push_back(2);

  TString cuts = "ht>500&&met>200&&nleps>=1&&pass";

  vector<hfeats> vars;

  vars.push_back(hfeats("mj",25,0,1000, mjisr,"M_{J} [GeV]",cuts+"&&abs(isr_tru_pt)<10"));
  vars.back().whichPlots = "4";
  vars.push_back(hfeats("mj",25,0,1000, mjisr,"M_{J} [GeV]",cuts+"&&abs(isr_tru_pt)>100"));
  vars.back().whichPlots = "4";

  //NOTE: Make sure RohanHack is turned off
  plot_distributions(Samples, vars, "2.2", ".pdf", "CMSPaperNoRatio","mj_isr",false);
}
