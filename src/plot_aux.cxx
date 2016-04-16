// plot_aux: Macro that makes the auxiliary plots for SUS-15-007

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

namespace {
  TString plotSet = "MJ_validation";

  int sigcolor(kRed);
  int stcolor(kMagenta-2);
  TString luminosity="2.3";
  TString plot_type=".pdf";
  TString plot_style="CMSPaper_Supplementary";
}

using namespace std;
using std::cout;
using std::endl;

void doN1_R4(); // N-1 and R4 plots
void doSignal(); // Signal plots
void do_MJ_validation();
void doSyst(); // Syst plots (rohanplot is made in plot_rohanplots.cxx)
void doMC1D();

int main(){ 

  if(plotSet.Contains("N1_R4")) doN1_R4();
  if(plotSet.Contains("Signal")) doSignal();
  if(plotSet.Contains("MJ_validation")) do_MJ_validation();
  if(plotSet.Contains("Syst")) doSyst();
  if(plotSet.Contains("MC1D")) doMC1D();
}

// N-1 and R4 plots
void doN1_R4(){

  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder
  
  TString folder1l(bfolder+"/cms2r0/babymaker/babies/2016_02_04/data/singlelep/combined/skim_1lht500met200/");
  TString foldermc(bfolder+"/cms2r0/babymaker/babies/2015_11_28/mc/skim_1lht500met200/");

  // Folders with no MET or HT cuts
  TString folder1l_met(bfolder+"/cms2r0/babymaker/babies/2016_02_04/data/singlelep/combined/skim_1lnjets6nb1/");
  TString foldermc_met(bfolder+"/cms2r0/babymaker/babies/2015_11_28/mc/skim_1lnjets6nb1/");

  vector<TString> s_slep;
  s_slep.push_back(folder1l+"*root"); 
  vector<TString> s_slep_met;
  s_slep_met.push_back(folder1l_met+"*root");


  vector<TString> s_t1t;
  s_t1t.push_back(foldermc+"*T1tttt*1500_*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(foldermc+"*T1tttt*1200_*");
  vector<TString> s_tt;
  s_tt.push_back(foldermc+"*_TTJets*Lept*");
  s_tt.push_back(foldermc+"*_TTJets_HT*");
  vector<TString> s_wjets;
  s_wjets.push_back(foldermc+"*_WJetsToLNu*");
  vector<TString> s_ttv;
  s_ttv.push_back(foldermc+"*_TTWJets*");
  s_ttv.push_back(foldermc+"*_TTZTo*");
  vector<TString> s_single;
  s_single.push_back(foldermc+"*_ST_*");
  vector<TString> s_other;
  s_other.push_back(foldermc+"*DYJetsToLL*");
  s_other.push_back(foldermc+"*_QCD_HT*");
  s_other.push_back(foldermc+"*_ZJet*");
  s_other.push_back(foldermc+"*_WWTo*");
  s_other.push_back(foldermc+"*ggZH_HToBB*");
  s_other.push_back(foldermc+"*ttHJetTobb*");

  vector<TString> s_t1t_met;
  s_t1t_met.push_back(foldermc_met+"*T1tttt*1500_*");
  vector<TString> s_t1tc_met;
  s_t1tc_met.push_back(foldermc_met+"*T1tttt*1200_*");
  vector<TString> s_tt_met;
  s_tt_met.push_back(foldermc_met+"*_TTJets*Lept*");
  s_tt_met.push_back(foldermc_met+"*_TTJets_HT*");
  vector<TString> s_wjets_met;
  s_wjets_met.push_back(foldermc_met+"*_WJetsToLNu*");
  vector<TString> s_ttv_met;
  s_ttv_met.push_back(foldermc_met+"*_TTWJets*");
  s_ttv_met.push_back(foldermc_met+"*_TTZTo*");
  vector<TString> s_single_met;
  s_single_met.push_back(foldermc_met+"*_ST_*");
  vector<TString> s_other_met;
  s_other_met.push_back(foldermc_met+"*DYJetsToLL*");
  s_other_met.push_back(foldermc_met+"*_QCD_HT*");
  s_other_met.push_back(foldermc_met+"*_ZJet*");
  s_other_met.push_back(foldermc_met+"*_WWTo*");
  s_other_met.push_back(foldermc_met+"*ggZH_HToBB*");
  s_other_met.push_back(foldermc_met+"*ttHJetTobb*");


  // Reading ntuples
  TString lsp = "{#lower[-0.1]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}}";
  TString t1t_label = "#scale[0.95]{#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#kern[0.18]{#bar{t}}#kern[0.18]"+lsp;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_slep, "Data", kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_t1t, t1t_label+" (1500,100)}", sigcolor, 1));
  Samples.push_back(sfeats(s_t1tc, t1t_label+" (1200,800)}", sigcolor, 2));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1&&stitch"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  Samples.push_back(sfeats(s_wjets, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_single, "Single t", stcolor));
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_other, "Other", 2001, 1)); 

  vector<int> ra4_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
  } // Loop over samples

  //// Samples for the HT and MET N-1 plots
  int imet(Samples.size());
  Samples.push_back(sfeats(s_slep_met, "Data", kBlack,1,"(trig[4]||trig[8])")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_t1t_met, t1t_label+" (1500,100)}", sigcolor, 1));
  Samples.push_back(sfeats(s_t1tc_met, t1t_label+" (1200,800)}", sigcolor, 2));
  Samples.push_back(sfeats(s_tt_met, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1&&stitch"));
  Samples.push_back(sfeats(s_tt_met, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  Samples.push_back(sfeats(s_wjets_met, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_single_met, "Single t", stcolor));
  Samples.push_back(sfeats(s_ttv_met, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_other_met, "Other", 2001, 1)); 
  vector<int> met_sam;
  for(unsigned sam(imet); sam < Samples.size(); sam++){
    met_sam.push_back(sam);
  } // Loop over samples


  vector<hfeats> vars;

  // // N-1 plots
  vars.push_back(hfeats("ht",39,0,1950, met_sam, "H_{T} [GeV]","nleps==1&&pass&&met>200&&njets>=6&&nbm>=1",500,"N1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("met",32,0,800, met_sam, "E_{T}^{miss} [GeV]","nleps==1&&pass&&ht>500&&njets>=6&&nbm>=1",200,"N1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("njets",12, 0.5,12.5, ra4_sam, "N_{jets}","nleps==1&&pass&&ht>500&&met>200&&nbm>=1",5.5,"N1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("nbm",5, -0.5,4.5, ra4_sam, "N_{b}","nleps==1&&pass&&ht>500&&met>200&&njets>=6",0.5,"N1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  // Distributions in R4
  TString skimCuts("nleps==1&&pass&&ht>500&&met>200");
  vars.push_back(hfeats("met",10,200,700, ra4_sam, "E_{T}^{miss} [GeV]",
  			skimCuts+"&&mt>140&&mj>400&&nbm>=1&&njets>=6",400,"R4"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",12,0,1200, ra4_sam, "M_{J} [GeV]",
  			skimCuts+"&&mt>140&&nbm>=1&&njets>=6",400,"R4"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mt",14,0,490, ra4_sam, "m_{T} [GeV]",
  			skimCuts+"&&mj>400&&nbm>=1&&njets>=6",140,"R4"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("njets",12, 0.5,12.5, ra4_sam, "N_{jets}",
  			skimCuts+"&&mt>140&&mj>400&&nbm>=1",8.5,"R4"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("nbm",5, -0.5,4.5, ra4_sam, "N_{b}",
  			skimCuts+"&&mt>140&&mj>400&&njets>=6",2.5,"R4"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "aux",true);
}

// Signal
void doSignal(){

  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder
  
  TString foldermc(bfolder+"/cms2r0/babymaker/babies/2015_11_28/mc/");

  vector<TString> s_t1t;
  s_t1t.push_back(foldermc+"*T1tttt*1500_*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(foldermc+"*T1tttt*1200_*");

  // Reading ntuples
  TString lsp = "{#lower[-0.1]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}}";
  TString t1t_label = "#scale[0.95]{#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#kern[0.18]{#bar{t}}#kern[0.18]"+lsp;

  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_t1t, "N_{true leptons}", sigcolor, 1, "pass","ntruels+ntrumus"));
  Samples.push_back(sfeats(s_t1t, "N_{reco. leptons}", 31, 2, "pass"));

  vector<int> ra4_t1t;
  ra4_t1t.push_back(0);
  ra4_t1t.push_back(1);

  vector<hfeats> vars;

  // nleps<1234 used as a hack to get signal point printed on canvas
  vars.push_back(hfeats("nleps",5,-0.5,4.5, ra4_t1t, "N_{prompt e+#mu}","nleps<1234",-1,"signal"));
  vars.back().whichPlots = "3";
 
  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "aux", false);
}

void do_MJ_validation(){
  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder
  
  TString folder1l(bfolder+"/cms2r0/babymaker/babies/2016_02_04/data/singlelep/combined/skim_1lht500met200/");
  TString foldermc(bfolder+"/cms2r0/babymaker/babies/2015_11_28/mc/skim_1lht500met200/");

  TString folder1l_zisr(bfolder+"/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/skim_dy_ht300/"); //skim doesn't exist
  TString foldermc_zisr(bfolder+"/cms2r0/babymaker/babies/2015_11_28/mc/skim_dy_ht300/");

  vector<TString> s_slep;
  s_slep.push_back(folder1l+"*root"); 

  vector<TString> s_t1t;
  s_t1t.push_back(foldermc+"*T1tttt*1500_*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(foldermc+"*T1tttt*1200_*");
  vector<TString> s_tt;
  s_tt.push_back(foldermc+"*_TTJets*Lept*");
  s_tt.push_back(foldermc+"*_TTJets_HT*");
  vector<TString> s_wjets;
  s_wjets.push_back(foldermc+"*_WJetsToLNu*");
  vector<TString> s_ttv;
  s_ttv.push_back(foldermc+"*_TTWJets*");
  s_ttv.push_back(foldermc+"*_TTZTo*");
  vector<TString> s_single;
  s_single.push_back(foldermc+"*_ST_*");
  vector<TString> s_other;
  s_other.push_back(foldermc+"*DYJetsToLL*");
  s_other.push_back(foldermc+"*_QCD_HT*");
  s_other.push_back(foldermc+"*_ZJet*");
  s_other.push_back(foldermc+"*_WWTo*");
  s_other.push_back(foldermc+"*ggZH_HToBB*");
  s_other.push_back(foldermc+"*ttHJetTobb*");



  vector<TString> s_slep_dy;
  s_slep_dy.push_back(folder1l_zisr+"*root"); 

  vector<TString> s_tt_dy;
  s_tt_dy.push_back(foldermc_zisr+"*_TTJets*Lept*");
  s_tt_dy.push_back(foldermc_zisr+"*_TTJets_HT*");

  vector<TString> s_ttv_dy;
  s_ttv_dy.push_back(foldermc_zisr+"*_TTWJets*");
  s_ttv_dy.push_back(foldermc_zisr+"*_TTZTo*");
  
  vector<TString> s_dyjets;
  s_dyjets.push_back(foldermc_zisr+"*_DYJetsToLL*");

  vector<TString> s_other_dy;
  s_other_dy.push_back(foldermc_zisr+"*_WJetsToLNu*");
  s_other_dy.push_back(foldermc_zisr+"*_QCD_HT*");
  s_other_dy.push_back(foldermc_zisr+"*_ZJet*");
  s_other_dy.push_back(foldermc_zisr+"*_WWTo*");
  s_other_dy.push_back(foldermc_zisr+"*ggZH_HToBB*");
  s_other_dy.push_back(foldermc_zisr+"*ttHJetTobb*");
  s_other_dy.push_back(foldermc_zisr+"*_ST_*");


  // ttbar selection
  TString lsp = "{#lower[-0.1]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}}";
  TString t1t_label = "#scale[0.95]{#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#kern[0.18]{#bar{t}}#kern[0.18]"+lsp;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_slep, "Data", kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_t1t, t1t_label+" (1500,100)}", sigcolor, 1));
  Samples.push_back(sfeats(s_t1tc, t1t_label+" (1200,800)}", sigcolor, 2));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1&&stitch"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  Samples.push_back(sfeats(s_wjets, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_single, "Single t", stcolor));
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_other, "Other", 2001, 1)); 

  vector<int> ra4_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
  } // Loop over samples

  //Z+jets selection
  int idy(Samples.size());
  Samples.push_back(sfeats(s_slep_dy, "Data", kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_dyjets, "Z+jets", dps::c_wjets,1,"stitch"));
  Samples.push_back(sfeats(s_tt_dy, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  Samples.push_back(sfeats(s_tt_dy, "t#bar{t}, 1 true lepton", dps::c_tt_1l,1,"ntruleps<=1&&stitch"));
  Samples.push_back(sfeats(s_ttv_dy, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_other_dy, "Other", 2001, 1));
  
  vector<int> dy_sam;
  for(unsigned sam(idy); sam < Samples.size(); sam++){
    dy_sam.push_back(sam);
  } // Loop over samples

  vector<hfeats> vars;
  TString baseline = "nleps==1&&njets>=4&&nbm>=1&&met>200&&ht>500&&pass";
  
  vars.push_back(hfeats("fjets_m[0]",39,0,390, ra4_sam, "m(J_{1}) [GeV]",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_m[1]",29,0,290, ra4_sam, "m(J_{2}) [GeV]",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_m[2]",18,0,180, ra4_sam, "m(J_{3}) [GeV]",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets08_m[0]",39,0,390, ra4_sam, "m(J_{1}) [GeV], R = 0.8",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets08_m[1]",29,0,290, ra4_sam, "m(J_{2}) [GeV], R = 0.8",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets08_m[2]",18,0,180, ra4_sam, "m(J_{3}) [GeV], R = 0.8",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  
  vars.push_back(hfeats("mj",39,0,780, ra4_sam, "M_{J} [GeV]",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  

  //TString mll("(mumuv_m*(mumuv_m>0&&mumu_pt1>25)+elelv_m*(elelv_m>0&&elel_pt1>30))>80&&(mumuv_m*(mumuv_m>0&&mumu_pt1>25)+elelv_m*(elelv_m>0&&elel_pt1>30))<100");
  TString mll("(mumu_m*(mumu_m>0&&mumu_pt1>30)+elel_m*(elel_m>0&&elel_pt1>30))>80&&(mumu_m*(mumu_m>0&&mumu_pt1>30)+elel_m*(elel_m>0&&elel_pt1>30))<100");
  vars.push_back(hfeats("fjets_m[0]",20,0,500, dy_sam, "m(J_{1}) [GeV]",
			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",24,0,600, dy_sam, "M_{J} [GeV]",
   			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
 
 //For investigating Z+ jets normalization
  /* vars.push_back(hfeats("njets",10,0,10, dy_sam, "N_{jets}",
   			"pass&&ht>500&&(nmus>=2||nels>=2)&&"+mll));
  vars.back().whichPlots = "12"; 

  vars.push_back(hfeats("ht",30,0,1500, dy_sam, "H_{T}",
   			"pass&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "12"; 

  vars.push_back(hfeats("nels",4,0,4, dy_sam, "nels",
   			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "12"; 

  vars.push_back(hfeats("nmus",4,0,4, dy_sam, "nmus",
   			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "12"; 

  vars.push_back(hfeats("mumu_m",30,0,300, dy_sam, "m_{mumu}",
   			"pass&&(nmus>=2||nels>=2)&&njets>=4&&ht>500"));
  vars.back().whichPlots = "12";

  vars.push_back(hfeats("elel_m",30,0,300, dy_sam, "m_{elel}",
   			"pass&&(nmus>=2||nels>=2)&&njets>=4&&ht>500"));
			vars.back().whichPlots = "12";*/

  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "aux",true);
}

// Systematic plots
void doSyst(){

  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder

  // 1l vs 2l MJ Data Comparison
  TString folder1l(bfolder+"/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/skim_1lht500met200/");
  vector<TString> s_slep;
  s_slep.push_back(folder1l+"*root"); // With "__Single" you exclude the extra 48 ipb

  vector<sfeats> Sam_dilep;
  Sam_dilep.push_back(sfeats(s_slep, "Data 2l, N_{jets} #geq 5, N_{b} #leq 2", kBlack,1,"(trig[4]||trig[8])&&pass&&njets>=5&&nleps==2&&nbm<=2"));
  Sam_dilep.back().isData=true;
  Sam_dilep.push_back(sfeats(s_slep, "Data 1l, N_{jets} #geq 6, m_{T} #leq 140, N_{b} #geq 1", kBlue+2,1,
			     "(trig[4]||trig[8])&&pass&&mt<=140&&njets>=6&&nleps==1&&nbm>=1")); Sam_dilep.back().doBand= true;
  vector<int> dilep_sam;
  dilep_sam.push_back(0);
  dilep_sam.push_back(1);

  vector<hfeats> vars_dilep;
  vars_dilep.push_back(hfeats("mj",6,250,700, dilep_sam, "M_{J} [GeV]","ht>500&&met>200&&pass&&met<=400",-1,"syst"));
  vars_dilep.back().whichPlots = "12"; vars_dilep.back().normalize = true;

  plot_distributions(Sam_dilep, vars_dilep, luminosity, plot_type, plot_style, "aux",true);

  // ISR pT plots
  TString skim = "ttisr";
  TString folder1l_isr(bfolder+"/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/skim_"+skim+"/");
  TString foldermc(bfolder+"/cms2r0/babymaker/babies/2015_11_28/mc/skim_"+skim+"/");
  TString folderdy(bfolder+"/cms2r0/babymaker/babies/2015_11_28/mc/skim_"+skim+"/");

  vector<TString> s_slep_isr;
  s_slep_isr.push_back(folder1l_isr+"*root");

  vector<TString> s_tt;
  s_tt.push_back(foldermc+"*_TTJets*Lept*");
  s_tt.push_back(foldermc+"*_TTJets_HT*");
  vector<TString> s_ttv;
  s_ttv.push_back(foldermc+"*_TTWJets*");
  s_ttv.push_back(foldermc+"*_TTZTo*");
  s_ttv.push_back(foldermc+"*_TTG*");
  s_ttv.push_back(foldermc+"*_TTTT*");
  vector<TString> s_dy;
  s_dy.push_back(folderdy+"*DYJetsToLL_M-50_*");
  vector<TString> s_st;
  s_st.push_back(foldermc+"*_ST_*");
  vector<TString> s_other;
  s_other.push_back(foldermc+"*_ZJet*");
  s_other.push_back(foldermc+"*_WJetsToLNu*");
  s_other.push_back(foldermc+"*ggZH_HToBB*");
  s_other.push_back(foldermc+"*ttHJetTobb*");
  s_other.push_back(foldermc+"*QCD*");
  s_other.push_back(foldermc+"*_WZTo*");
  s_other.push_back(foldermc+"*_WWTo*");
  s_other.push_back(foldermc+"*_ZZTo*");


  vector<sfeats> Sam_isr;
  Sam_isr.push_back(sfeats(s_slep_isr, "Data", kBlack,1,"pass&&(trig[18]||trig[21]||trig[23]||trig[24])")); Sam_isr.back().isData = true;
  Sam_isr.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  Sam_isr.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1&&stitch"));
  Sam_isr.push_back(sfeats(s_dy, "Z+jets",dps::c_wjets,1,"stitch"));
  Sam_isr.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  Sam_isr.push_back(sfeats(s_st, "Single t", stcolor));
  Sam_isr.push_back(sfeats(s_other, "Other", 2001, 1));

  vector<int> ra4_sam;
  unsigned nsam(Sam_isr.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
  } // Loop over samples

  vector<hfeats> vars_isr;
  float ibinning[] = {0., 50., 100., 150., 200., 300., 400.,600., 800.};
  int nbins = sizeof(ibinning)/sizeof(float)-1;

  //---------- TTBAR ISR --------------------
  vars_isr.push_back(hfeats("jetsys_nob_pt",nbins,ibinning, ra4_sam, "p_{T}(ISR jets) [GeV]","nleps==2&&Max$(leps_pt)>30&&njets>2&&nbm==2&&pass",-1,"syst"));
  vars_isr.back().whichPlots = "12"; vars_isr.back().normalize = true;

  plot_distributions(Sam_isr, vars_isr, luminosity, plot_type, plot_style, "aux",true);

}

void doMC1D(){
  TString folder="/cms2r0/babymaker/babies/2015_11_28/mc/skim_1lht500met200/";
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  folder = "/net/cms2"+folder;
  if(Contains(hostname, "lxplus"))     
    folder="/afs/cern.ch/user/m/manuelf/work/babies/2015_11_28/mc/skim_1lht500met200/";

  vector<TString> s_tt, s_t4t_nc,s_t4t_c; 
  s_tt.push_back(folder+"*TTJets*Lept*");
  s_tt.push_back(folder+"*TTJets*HT*");
  s_t4t_nc.push_back(folder+"*T1tttt*1500*100*.root");
  s_t4t_c.push_back(folder+"*T1tttt*1200*800*.root");

  // Reading ntuples
  vector<sfeats> Samples;
  Samples.push_back(sfeats(s_t4t_nc, "#scale[0.92]{#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#bar{t}#tilde{#chi}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}} (1500,100)}",ra4::c_t1tttt,1, "stitch")); 
  Samples.push_back(sfeats(s_t4t_c, "#scale[0.92]{#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#bar{t}#tilde{#chi}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}} (1200,800)}",ra4::c_t1tttt,2, "stitch")); 
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton",dps::c_tt_1l,1,"ntruleps==1&&stitch"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps==2&&stitch"));
  
 
  vector<int> clean;
  clean.push_back(2);
  clean.push_back(3);
  clean.push_back(0);
  clean.push_back(1);

  vector<hfeats> vars;

  
  vars.push_back(hfeats("mj",35,0,1400, clean,"M_{J} [GeV]","ht>500&&met>200&&njets>=6&&nbm>=1&&nleps>=1&&pass"));
  vars.back().whichPlots = "34";

  vars.push_back(hfeats("mt",12,0,420, clean,"m_{T} [GeV]","ht>500&&met>200&&njets>=6&&nbm>=1&&nleps>=1&&pass"));
  vars.back().whichPlots = "34";

  vars.push_back(hfeats("njets",12,0.5,12.5, clean,"N_{jets}","ht>500&&met>200&&nbm>=1&&nleps>=1&&pass"));
  vars.back().whichPlots = "4";

  vars.push_back(hfeats("nbm",5,-0.5,4.5, clean,"N_{b}","ht>500&&met>200&&njets>=6&&nleps>=1&&pass"));
  vars.back().whichPlots = "4";

  vars.push_back(hfeats("met",12,200,800, clean,"MET [GeV]","ht>500&&met>200&&njets>=6&&nbm>=1&&nleps>=1&&pass"));
  vars.back().whichPlots = "4";
  

  //NOTE: Make sure RohanHack is turned off
  plot_distributions(Samples, vars, luminosity, plot_type, "CMSPaperNoRatio_Supplementary", "aux",false);


}
