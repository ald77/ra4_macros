#include <iostream>
#include <vector>

#include "TString.h"

#include "utilities_macros_rohan.cpp"

using namespace std;

int main(){ 

  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder
  TString folder=bfolder+"/cms2r0/babymaker/babies/2016_05_31/mc/skim_1lht500met200/";
  vector<TString> s_tt; 
  s_tt.push_back(folder+"*TTJets*Lept*");
  s_tt.push_back(folder+"*TTJets*HT*");

  // Reading ntuples
  vector<sfeats> Samples;
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons: m_{T} > 140 GeV", kViolet+9,25, "mt>140&&ntruleps==2&&nveto==0&&stitch")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton: m_{T} > 140 GeV", kViolet+9,25,"mt>140&&ntruleps==1&&nveto==0&&stitch")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons: m_{T} #leq 140 GeV", 31,1, "mt<=140&&ntruleps==2&&stitch"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton: m_{T} #leq 140 GeV", 31,1,"mt<=140&&ntruleps==1&&stitch"));

  vector<int> mt140;
  mt140.push_back(3);
  mt140.push_back(0);

  vector<int> mt140_2l;
  mt140_2l.push_back(2);
  mt140_2l.push_back(0);

  vector<int> mt140_1l;
  mt140_1l.push_back(3);
  mt140_1l.push_back(1);

  TString noCuts = "1";
  TString baseline = "ht>500&&met>200&&njets>=6&&nbm>=1&&nleps==1&&stitch&&pass";
  TString baseline_highnj = "ht>500&&met>200&&njets>=9&&nbm>=1&&nleps==1&&stitch&&pass";
  TString medMET = "ht>500&&met>350&&njets>=6&&nbm>=1&&nleps==1&&stitch&&pass";
  TString medMET_highnj = "ht>500&&met>350&&njets>=9&&nbm>=1&&nleps==1&&stitch&&pass";
  TString highMET = "ht>500&&met>500&&njets>=6&&nbm>=1&&nleps==1&&stitch&&pass";
  TString highMET_highnj = "ht>500&&met>500&&njets>=9&&nbm>=1&&nleps==1&&stitch&&pass";


  vector<hfeats> vars_paper, vars_aux;

  vars_paper.push_back(hfeats("mj14",30,0,1500, mt140,    "M_{J}^{1.4} [GeV]",baseline,400,"eventselection")); 
  vars_paper.back().whichPlots = "14"; vars_paper.back().maxRatio = 0.19;

  vars_paper.push_back(hfeats("mj14",30,0,1500, mt140,    "M_{J}^{1.4} [GeV]",baseline_highnj,400,"eventselection")); 
  vars_paper.back().whichPlots = "14"; vars_paper.back().maxRatio = 0.19;

  vars_paper.push_back(hfeats("mj14",30,0,1500, mt140,    "M_{J}^{1.4} [GeV]",medMET,400,"eventselection")); 
  vars_paper.back().whichPlots = "14"; vars_paper.back().maxRatio = 0.19;

  vars_paper.push_back(hfeats("mj14",30,0,1500, mt140,    "M_{J}^{1.4} [GeV]",medMET_highnj,400,"eventselection")); 
  vars_paper.back().whichPlots = "14"; vars_paper.back().maxRatio = 0.19;

  vars_paper.push_back(hfeats("mj14",30,0,1500, mt140,    "M_{J}^{1.4} [GeV]",highMET,400,"eventselection")); 
  vars_paper.back().whichPlots = "14"; vars_paper.back().maxRatio = 0.19;

  vars_paper.push_back(hfeats("mj14",30,0,1500, mt140,    "M_{J}^{1.4} [GeV]",highMET_highnj,400,"eventselection")); 
  vars_paper.back().whichPlots = "14"; vars_paper.back().maxRatio = 0.19;

  //  vars_aux.push_back(hfeats("mj",30,0,1500, mt140_1l, "M_{J} [GeV]",baseline,400,"syst")); 
  //  vars_aux.back().whichPlots = "14"; vars_aux.back().maxRatio = 0.19;

  //  baseline = "mj>250&&ht>500&&met>200&&nbm>=1&&nleps==1&&stitch&&pass";
  //  vars_paper.push_back(hfeats("njets",7,5.5,12.5, mt140, "N_{jets}",baseline, -1, "eventselection")); 
  //  vars_paper.back().whichPlots = "13"; vars_paper.back().maxRatio = 0.19;

  //NOTE: Make sure RohanHack is turned off
  plot_distributions(Samples, vars_paper, "2.3", ".pdf", "CMSPaper","rohan",true);
  //  plot_distributions(Samples, vars_aux, "2.3", ".pdf", "CMSPaper_Supplementary","aux",true);
}
