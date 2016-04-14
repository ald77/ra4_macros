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
  TString plotSet = "Signal,Syst";

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

int main(){ 

  if(plotSet.Contains("N1_R4")) doN1_R4();
  if(plotSet.Contains("Signal")) doSignal();
  if(plotSet.Contains("MJ_validation")) do_MJ_validation();
  if(plotSet.Contains("Syst")) doSyst();

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
  Samples.push_back(sfeats(s_slep_met, "Data", kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
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
  vars.push_back(hfeats("ht",39,0,1950, met_sam, "H_{T} [GeV]","nleps==1&&met>200&&njets>=6&&nbm>=1",500,"N1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("met",32,0,800, met_sam, "E_{T}^{miss} [GeV]","nleps==1&&ht>500&&njets>=6&&nbm>=1",200,"N1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("njets",12, 0.5,12.5, ra4_sam, "N_{jets}","nleps==1&&ht>500&&met>200&&nbm>=1",5.5,"N1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("nbm",5, -0.5,4.5, ra4_sam, "N_{b}","nleps==1&&ht>500&&met>200&&njets>=6",0.5,"N1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  // Distributions in R4
  TString skimCuts("nleps==1&&ht>500&&met>200");
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
  			skimCuts+"&&mt>140&&mj>400&&njets>=6",1.5,"R4"));
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
  Samples.push_back(sfeats(s_t1t, "N_{true leptons}", sigcolor, 1, "1","ntruels+ntrumus"));
  Samples.push_back(sfeats(s_t1t, "N_{reco. leptons}", 31, 2));

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

  vector<TString> s_slep;
  s_slep.push_back(folder1l+"*root"); 

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


  // Reading ntuples
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_slep, "Data", kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
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


  vector<hfeats> vars;
  TString baseline = "nleps==1&&njets>=6&&nbm>=1&&met>200&&ht>500";

  /* vars.push_back(hfeats("fjets_m[0]",16,0,480, ra4_sam, "m(J_{1}) [GeV]",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_m[1]",16,0,480, ra4_sam, "m(J_{2}) [GeV]",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_m[2]",16,0,480, ra4_sam, "m(J_{3}) [GeV]",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  */
   vars.push_back(hfeats("fjets08_m[0]",16,0,480, ra4_sam, "m(J_{1}) [GeV], R = 0.8",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets08_m[1]",16,0,480, ra4_sam, "m(J_{2}) [GeV], R = 0.8",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets08_m[2]",16,0,480, ra4_sam, "m(J_{3}) [GeV], R = 0.8",
			baseline,-10,"MJ_validation"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
 
 
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
  Sam_dilep.push_back(sfeats(s_slep, "Data 2l, N_{jets} #geq 5", kBlack,1,"(trig[4]||trig[8])&&pass&&njets>=5&&nleps==2&&nbm<=2"));
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
  //  s_other.push_back(foldermc+"*_ST_*");
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
  vars_isr.push_back(hfeats("jetsys_nob_pt",nbins,ibinning, ra4_sam, "p_{T}(ISR jets) [GeV]","nleps==2&&Max$(leps_pt)>30&&njets>2&&nbm==2",-1,"syst"));
  vars_isr.back().whichPlots = "12"; vars_isr.back().normalize = true;

  plot_distributions(Sam_isr, vars_isr, luminosity, plot_type, plot_style, "aux",true);

}
