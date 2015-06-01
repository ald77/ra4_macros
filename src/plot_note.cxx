// plot_note: Macro that plots variables both lumi weighted and normalized to the same area.

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

namespace ra4 {
  TString ntuple_date("2015_05_25");
  TString minjets("7"), midjets("9");
  TString mjthresh("400");
  TString luminosity="10";
  TString plot_type=".pdf";
  TString plot_style="RA4";
}

using namespace ra4;

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  TString folder="/cms5r0/ald77/archive/"+ntuple_date+"/skim/";
  TString folder_ns="/cms5r0/ald77/archive/"+ntuple_date+"/";


  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*WJets*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder+"*_T*channel*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder+"*TTW*");
  s_ttv.push_back(folder+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD_HT*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");
  s_other.push_back(folder+"*_TTJet*");

  // Reading ntuples
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_t1t, "T1tttt(1500,100)", ra4::c_t1tttt));
  Samples.push_back(sfeats(s_t1tc, "T1tttt(1200,800)", ra4::c_t1tttt,2));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", ra4::c_tt_1l, 1,"ntruleps==1"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", ra4::c_tt_2l,1,"ntruleps>=2"));
  Samples.push_back(sfeats(s_wjets, "W+jets", ra4::c_wjets));
  Samples.push_back(sfeats(s_singlet, "Single t", ra4::c_singlet));
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  // Some of Other has leptons, but very little, and this is the easiest to put had tt with QCD
  Samples.push_back(sfeats(s_other, "Other", ra4::c_other, 1,"ntruleps==0")); 

  // Adding non-skimmed samples
  vector<int> ra4_sam, ra4_sam_ns;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
    ra4_sam_ns.push_back(nsam+sam);
    vector<TString> sam_files = Samples[sam].file;
    for(unsigned ifile(0); ifile < sam_files.size(); ifile++)
      sam_files[ifile].ReplaceAll(folder, folder_ns);
    Samples.push_back(sfeats(sam_files, Samples[sam].label, Samples[sam].color, Samples[sam].style,
			     Samples[sam].cut));
  } // Loop over samples

  
  vector<int> ra4_tt_t1_noskim;
  ra4_tt_t1_noskim.push_back(nsam);
  ra4_tt_t1_noskim.push_back(nsam+1);
  ra4_tt_t1_noskim.push_back(nsam+2);


  vector<hfeats> vars;

  TString cuts("(nmus+nels)==1");
  vars.push_back(hfeats("ht",35,0,3500, ra4_sam_ns, "H_{T} (GeV)",cuts,500));
  cuts += "&&ht>500";
  vars.push_back(hfeats("met",40,0,800, ra4_sam_ns, "MET (GeV)",cuts,200));
  cuts += "&&met>200";
  vars.push_back(hfeats("njets",18,-0.5,17.5, ra4_sam, "Number of 30 GeV jets",cuts,6.5));
  cuts += "&&njets>="+minjets;
  vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Number of b-tags (CSVM)",cuts,1.5));
  cuts += "&&nbm>=2";
  vars.push_back(hfeats("mt",25,0,500, ra4_sam, "m_{T} (GeV)",cuts,140));
  cuts += "&&mt>140";
  vars.push_back(hfeats("mj",32,0,1600, ra4_sam, "M_{J} (GeV)",cuts,mjthresh.Atof()));


  plot_distributions(Samples, vars, luminosity, plot_type, plot_style);


}

