// plot_dps: Macro that plots variables both for comissioning DPS

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
  TString luminosity="1.26"; // in ifb
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  TString folder_1l="/afs/cern.ch/work/m/manuelf/public/babies/2015_10_19/mc/skim_1lht400/";
  TString folder_ns="/afs/cern.ch/work/m/manuelf/public/babies/2015_10_19/mc/";
  TString folder_1ldata="/afs/cern.ch/work/m/manuelf/public/babies/2015_10_25/data/singlelep/combined/skim_1lht400/";
  TString folder_2ldata="/afs/cern.ch/work/m/manuelf/public/babies/2015_10_25/data/doublelep/combined/";
  TString folder_nsdata="/afs/cern.ch/work/m/manuelf/public/babies/2015_10_25/data/singlelep/combined/";

  /////////////////////////////////////////////// Data  ///////////////////////////////////////////////   
  // vector<TString> s_trig_htmht;
  // s_trig_htmht.push_back(folder_1ldata+"*HTMHT*");
  // vector<TString> s_trig_dl;
  // s_trig_dl.push_back(folder_2ldata+"/*DoubleMu*");
  // s_trig_dl.push_back(folder_2ldata+"/*DoubleE*");
  vector<TString> s_trig_sl;
  s_trig_sl.push_back(folder_1ldata+"/*Single*root");
  // s_trig_sl.push_back(folder_2ldata+"/*DoubleE*root");
  // vector<TString> s_trig_dmu;
  // s_trig_dmu.push_back(folder_2ldata+"/*DoubleMu*");
  // vector<TString> s_trig_del;
  // s_trig_del.push_back(folder_2ldata+"/*DoubleE*");

  /////////////////////////////////////////////// MC  ///////////////////////////////////////////////   
  vector<TString> s_tt;
  s_tt.push_back(folder_1l+"*_TTJets*Lept*");
  s_tt.push_back(folder_1l+"*_TTJets_HT-*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder_1l+"*_WJetsToLNu_HT*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder_1l+"*_ST_*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder_1l+"*TTW*");
  s_ttv.push_back(folder_1l+"*TTZ*");
  // vector<TString> s_qcd;
  // s_qcd.push_back(folder_1l+"*_QCD_HT*");
  vector<TString> s_DY;
  s_DY.push_back(folder_1l+"*_DYJetsToLL_M-50_HT*");
  vector<TString> s_other;
  // s_other.push_back(folder_1l+"*_ZJet*");
  s_other.push_back(folder_1l+"*_QCD_HT*");
  s_other.push_back(folder_1l+"*_ttHJetTobb*");
  s_other.push_back(folder_1l+"*_WWTo2L2Nu*");
  s_other.push_back(folder_1l+"*_WWToLNuQQ*");
  s_other.push_back(folder_1l+"*_ggZH_HToBB*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder_1l+"*-T1tttt_mGluino-1500_mLSP-100*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder_1l+"*-T1tttt_mGluino-1200_mLSP-800*");


  // Reading ntuples
  vector<sfeats> Samples; 
  TString datatitle = "Data";
  Samples.push_back(sfeats(s_trig_sl, datatitle,kBlack,1,"(trig[4]||trig[8])")); Samples.back().isData = true; //single trigger
  // Samples.push_back(sfeats(s_trig_sl, datatitle,kBlack,1,"trig[10]")); Samples.back().isData = true; //diel trigger
  // Samples.push_back(sfeats(s_trig_sl, datatitle,kBlack,1,"trig[9]")); Samples.back().isData = true; //dimuon trigger
  Samples.push_back(sfeats(s_DY, "Z+jets", dps::c_qcd));//12
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps==1"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2"));
  Samples.push_back(sfeats(s_wjets, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_singlet, "Single top", dps::c_singlet));
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  // Samples.push_back(sfeats(s_qcd, "QCD", dps::c_qcd));
  // Some of Other has leptons, but very little, and this is the easiest to put had tt with QCD
  // Samples.push_back(sfeats(s_other, "Other", dps::c_other, 1,"ntruleps==0")); 
  Samples.push_back(sfeats(s_other, "Other", dps::c_other, 1)); 

  vector<int> ra4_sam, ra4_sam_ns;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
    ra4_sam_ns.push_back(nsam+sam);
    vector<TString> sam_files = Samples[sam].file;
    for(unsigned ifile(0); ifile < sam_files.size(); ifile++)
      if(Samples[sam].isData) sam_files[ifile].ReplaceAll(folder_1ldata, folder_nsdata);
      else sam_files[ifile].ReplaceAll(folder_1l, folder_ns);
    Samples.push_back(sfeats(sam_files, Samples[sam].label, Samples[sam].color, Samples[sam].style,
           Samples[sam].cut));
    Samples.back().isData = Samples[sam].isData;
  } // Loop over samples

  vector<hfeats> vars;
  
  //Single lepton trigger
  // ----------- muons
  // vars.push_back(hfeats("mumuv_pt",16,0,800, ra4_sam, "p_{T}(#mu#mu) [GeV]",
  //       "pass&&ht>400&&nvmus==2&&nmus>=1&&mumuv_m>80&&mumuv_m<100"));
  // vars.back().whichPlots = "2"; vars.back().normalize = true;

  // // ----------- electrons
  // vars.push_back(hfeats("elelv_pt",16,0,800, ra4_sam, "p_{T}(ee) [GeV]",
  //       "pass&&ht>400&&nvels==2&&nels>=1&&elelv_m>80&&elelv_m<100"));
  // vars.back().whichPlots = "2"; vars.back().normalize = true;
  
  // ----------- any lepton
  // float ibinning[] = {0.,50., 100., 150., 200.,250., 300., 350., 400., 450., 500., 575., 650., 800., 1000. };
  // float ibinning[] = {0., 100., 200., 300., 400., 500., 650., 800., 1000. };
  // vars.push_back(hfeats("elelv_pt*(elelv_m>0)+mumuv_pt*(mumuv_m>0)",8,ibinning, ra4_sam, "p_{T}(ll) [GeV]",
  //       "pass&&ht>400&&nvleps==2&&nleps>=1&&((elelv_m>80&&elelv_m<100)||(mumuv_m>80&&mumuv_m<100))"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;

  // vars.push_back(hfeats("njets",11,-0.5,10.5, ra4_sam, "N_{jets} [GeV] [GeV]",
  //                "pass&&ht>400&&nvleps==2&&nleps>=1&&((elelv_m>80&&elelv_m<100)||(mumuv_m>80&&mumuv_m<100))"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  

  // vars.push_back(hfeats("njets",11,-0.5,10.5, ra4_sam, "N_{jets} [GeV] [GeV]",
  //       "pass&&ht>400&&nvmus==2&&nmus>=1&&mumuv_m>80&&mumuv_m<100"));
  // vars.back().whichPlots = "2"; vars.back().normalize = true;

  // vars.push_back(hfeats("njets",11,-0.5,10.5, ra4_sam, "N_{jets} [GeV] [GeV]",
  //       "pass&&ht>400&&nvels==2&&nels>=1&&elelv_m>80&&elelv_m<100"));
  // vars.back().whichPlots = "2"; vars.back().normalize = true;

  // vars.push_back(hfeats("njets*(elelv_m>0||mumuv_m>0)",11,-0.5,10.5, ra4_sam, "N_{jets} [GeV] [GeV]",
  //                "pass&&ht>400&&nvleps==2&&nleps>=1&&((elelv_m>80&&elelv_m<100)||(mumuv_m>80&&mumuv_m<100))"));
  // vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("jetsys_nob_pt",12,0,600, ra4_sam, "p_{T}(ISR jets) [GeV]",
        "pass&&ht>400&&nels==1&&nmus==1&&njets>2&&nbm==2"));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("njets-2",11,-0.5,10.5, ra4_sam, "N_{ISR jets} [GeV] [GeV]",
        "pass&&ht>400&&nels==1&&nmus==1&&njets>2&&nbm==2"));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  // vars.push_back(hfeats("njets-2",11,-0.5,10.5, ra4_sam, "N_{ISR jets} [GeV] [GeV]",
  //       "pass&&ht>400&&nels==1&&nmus==1&&njets>2"));
  // vars.back().whichPlots = "2"; vars.back().normalize = true;
  //
  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "isr",true);

}

