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
  TString folder=bfolder+"/cms2r0/babymaker/babies/2016_06_14/mc/merged_standard/";
  TString folder_rc=bfolder+"/cms2r0/babymaker/babies/reclustered/2016_06_14/mc/merged_standard/";
  vector<TString> s_tt; 
  s_tt.push_back(folder+"*TTJets*Lept*");
  s_tt.push_back(folder+"*TTJets*HT*");
  vector<TString> s_tt_rc; 
  s_tt_rc.push_back(folder_rc+"*TTJets*Lept*");
  s_tt_rc.push_back(folder_rc+"*TTJets*HT*");

  // Reading ntuples
  vector<sfeats> Samples;
  Samples.push_back(sfeats(s_tt, "t#bar{t}, N_{reco lep}=1: m_{T} #leq 140 GeV", 31,1,
			   "mt<=140&&nleps==1&&stitch&&njets>=6"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, N_{reco lep}=1, N_{tru lep}=2: m_{T} > 140 GeV", kViolet+9,25, 
			   "mt>140&&nleps==1&&ntruleps==2&&nveto==0&&stitch&&njets>=6")); 
  Samples.back().isData = true;

  Samples.push_back(sfeats(s_tt, "t#bar{t}, N_{reco lep}=1, N_{jet}#geq6: m_{T} #leq 140 GeV", 31,1,
			   "mt<=140&&nleps==1&&stitch&&njets>=6"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, N_{reco lep}=2, N_{jet}#geq5", kViolet+9,25, 
			   "nleps==2&&nveto==0&&stitch&&njets>=5")); Samples.back().isData = true;

  Samples.push_back(sfeats(s_tt_rc, "t#bar{t}, N_{reco lep}=1: m_{T} #leq 140 GeV", 31,1,
			   "mt<=140&&nleps==1&&stitch&&njets>=6"));
  Samples.push_back(sfeats(s_tt_rc, "t#bar{t}, N_{reco lep}=1, N_{tru lep}=2: m_{T} > 140 GeV", kViolet+9,25, 
			   "mt>140&&nleps==1&&ntruleps==2&&nveto==0&&stitch&&njets>=6")); 
  Samples.back().isData = true;

  Samples.push_back(sfeats(s_tt_rc, "t#bar{t}, N_{reco lep}=1, N_{jet}#geq6: m_{T} #leq 140 GeV", 31,1,
			   "mt<=140&&nleps==1&&stitch&&njets>=6"));
  Samples.push_back(sfeats(s_tt_rc, "t#bar{t}, N_{reco lep}=2, N_{jet}#geq5", kViolet+9,25, 
			   "nleps==2&&nveto==0&&stitch&&njets>=5")); Samples.back().isData = true;


  vector<int> sam_1l;
  sam_1l.push_back(0);
  sam_1l.push_back(1);

  vector<int> sam_2l;
  sam_2l.push_back(2);
  sam_2l.push_back(3);

  int offset = 4;
  vector<int> sam_1l_rc;
  sam_1l_rc.push_back(offset+0);
  sam_1l_rc.push_back(offset+1);

  vector<int> sam_2l_rc;
  sam_2l_rc.push_back(offset+2);
  sam_2l_rc.push_back(offset+3);

  TString baseline = "ht>500&&met>200&&njets>=6&&nbm>=1&&nleps==1&&stitch&&pass";
  TString baseline2l = "ht>500&&met>200&&stitch&&pass";

  vector<hfeats> vars;

  vars.push_back(hfeats("mj14",30,0,1500, sam_1l, "M_{J}^{with lep} [GeV]", baseline,400,"withlep")); 
  vars.back().whichPlots = "14"; vars.back().maxRatio = 0.11; vars.back().hline = 0.04;

  vars.push_back(hfeats("mj14",30,0,1500, sam_2l, "M_{J}^{with lep} [GeV]", baseline2l,400,"withlep")); 
  vars.back().whichPlots = "14"; vars.back().maxRatio = 0.019; vars.back().hline = 0.0075;

  vars.push_back(hfeats("mj14",30,0,1500, sam_1l_rc, "M_{J}^{no lep} [GeV]", baseline,400,"nolep")); 
  vars.back().whichPlots = "14"; vars.back().maxRatio = 0.11;  vars.back().hline = 0.04;

  vars.push_back(hfeats("mj14",30,0,1500, sam_2l_rc, "M_{J}^{no lep} [GeV]", baseline2l,400,"nolep")); 
  vars.back().whichPlots = "14"; vars.back().maxRatio = 0.011;  vars.back().hline = 0.0045;

  vars.push_back(hfeats("mj14",30,0,1500, sam_2l_rc, "M_{J}^{no lep} [GeV]",baseline2l+"&&njets>=6",400,"nolep"));
  vars.back().whichPlots = "14"; vars.back().maxRatio = 0.011; vars.back().hline = 0.0035;

  //NOTE: Make sure RohanHack is turned off
  plot_distributions(Samples, vars, "2.3", ".pdf", "CMSPaper","rohan",true);
}

