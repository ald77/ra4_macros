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
  TString ntuple_date("2015_07_22");
  TString minjets("5");
  TString mjthresh("400");
  TString minmet("150");
  TString minht("400");
  TString minbm("1");
  TString luminosity="0.043"; // in ifb
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  TString folder="/cms2r0/ald77/archive/"+ntuple_date+"/";

  // vector<TString> s_t1t;
  // s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  // vector<TString> s_t1tc;
  // s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");

  vector<TString> s_trig_htmht;
  s_trig_htmht.push_back(folder+"*HTMHT*");

  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*25ns*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*_WJets*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder+"*ST_*");
  vector<TString> s_qcd;
  s_qcd.push_back(folder+"*QCD_Pt*");
  // vector<TString> s_ttv; //negligible
  // s_ttv.push_back(folder+"*TTW*"); 
  // s_ttv.push_back(folder+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");
  s_other.push_back(folder+"*_TTJet*25ns*");

  // Reading ntuples
  vector<sfeats> Samples; 
  // Samples.push_back(sfeats(s_t1t, "T1tttt(1500,100)", dps::c_t1tttt));
  // Samples.push_back(sfeats(s_t1tc, "T1tttt(1200,800)", dps::c_t1tttt,2));
  TString datatitle = "Data";
  Samples.push_back(sfeats(s_trig_htmht, datatitle,kBlack,1,"trig[0]")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", dps::c_tt_1l, 1,"ntruleps==1"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", dps::c_tt_2l,1,"ntruleps>=2"));
  Samples.push_back(sfeats(s_wjets, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_singlet, "Single t", dps::c_singlet));
  // Samples.push_back(sfeats(s_ttv, "ttV", dps::c_ttv));
  Samples.push_back(sfeats(s_qcd, "QCD", dps::c_qcd));
  // Some of Other has leptons, but very little, and this is the easiest to put had tt with QCD
  Samples.push_back(sfeats(s_other, "Other", dps::c_other, 1,"ntruleps==0")); 

  vector<int> ra4_sam;
  for(unsigned sam(0); sam < Samples.size(); sam++) {
    ra4_sam.push_back(sam);
  }

  vector<hfeats> vars;

  TString cuts("(nvmus+nvels)==1");

  // vars.push_back(hfeats("onmet",16,0,800, ra4_sam, "online MET [GeV]","onht>350"));
  // vars.back().whichPlots = "12";

  // vars.push_back(hfeats("onht",16,0,800, ra4_sam, "online HT [GeV]","onmet>100"));
  // vars.back().whichPlots = "12";

  // vars.push_back(hfeats("ht",17,0,3400, ra4_sam, "H_{T} [GeV]",
  // 			  cuts+"&&met>"+minmet+"&&njets>="+minjets+"&&nbm>="+minbm));
  // vars.back().whichPlots = "12";
  // vars.push_back(hfeats("met",16,0,800, ra4_sam, "MET [GeV]",
  // 			  cuts+"&&ht>"+minht+"&&njets>="+minjets+"&&nbm>="+minbm));
  // vars.back().whichPlots = "12";
  // vars.push_back(hfeats("njets",18,-0.5,17.5, ra4_sam, "Number of jets",
  // 			  cuts+"&&ht>"+minht+"&&met>"+minmet+"&&nbm>="+minbm));
  // vars.back().whichPlots = "12";
  // vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Number of b-tags (CSVM)",
  // 			  cuts+"&&ht>"+minht+"&&met>"+minmet+"&&njets>="+minjets));
  // vars.back().whichPlots = "12";
  cuts += "&&ht>"+minht+"&&met>"+minmet+"&&njets>="+minjets+"&&nbm>="+minbm;
  // vars.push_back(hfeats("mt",25,0,500, ra4_sam, "m_{T} [GeV]",cuts));
  // vars.back().whichPlots = "12";
  cuts += "&&mt<140";
  vars.push_back(hfeats("mj",20,0,1000, ra4_sam, "M_{J} [GeV]",cuts));
  vars.back().whichPlots = "12";
  

  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "1d",true);


}

