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

namespace {
  TString ntuple_date("2015_05_25");
  TString minjets("7"), midjets("9");
  TString mjthresh("400");
  TString luminosity="10";
  TString plot_type=".pdf";
  TString plot_style="RA4";
  int section(6);
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  TString folder="/cms5r0/ald77/archive/"+ntuple_date+"/skim/";
  TString folder_ns="/cms5r0/ald77/archive/"+ntuple_date+"/";
  folder="/afs/cern.ch/user/m/manuelf/work/ucsb/2015_05_25/skim/";
  folder_ns="/afs/cern.ch/user/m/manuelf/work/ucsb/2015_05_25/skim/";


  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*_WJets*");
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
  vector<TString> s_bkg;
  s_bkg.push_back(folder+"*_TTJet*");
  s_bkg.push_back(folder+"*_WJets*");
  s_bkg.push_back(folder+"*_T*channel*");
  s_bkg.push_back(folder+"*TTW*");
  s_bkg.push_back(folder+"*TTZ*");
  s_bkg.push_back(folder+"*QCD_HT*");
  s_bkg.push_back(folder+"*_ZJet*");
  s_bkg.push_back(folder+"*DY*");
  s_bkg.push_back(folder+"*WH_HToBB*");
  s_bkg.push_back(folder+"*_TTJet*");

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

  // Other samples
  unsigned isec1 = Samples.size();
  Samples.push_back(sfeats(s_t1t, "True", ra4::c_t1tttt,1,"1","ntruels+ntrumus"));
  Samples.push_back(sfeats(s_t1t, "Reco Rel. iso.", kGreen+2,2,"1","nels_reliso+nmus_reliso"));
  Samples.push_back(sfeats(s_t1t, "Reco Mini iso.", 4,2,"1","nels+nmus"));

  unsigned isec6 = Samples.size();
  // Samples.push_back(sfeats(s_bkg, "All bkg, m_{T} #leq 140", 1, 1,"mt<=140"));
  // Samples.push_back(sfeats(s_bkg, "All bkg, m_{T} > 140", 2, -1,"mt>140"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, m_{T} #leq 140", 1, 1,"mt<=140"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, m_{T} > 140", 2, -1,"mt>140"));
  // Samples.push_back(sfeats(s_tt, "t#bar{t} 1l, m_{T} #leq 140", 1, 1,"ntruleps==1&&mt<=140"));
  // Samples.push_back(sfeats(s_tt, "t#bar{t} 1l, m_{T} > 140", 3, -1,"ntruleps==1&&mt>140"));
  // Samples.push_back(sfeats(s_tt, "t#bar{t} 2l, m_{T} #leq 140", 2, -1,"ntruleps>=2&&mt<=140"));
  // Samples.push_back(sfeats(s_tt, "t#bar{t} 2l, m_{T} > 140", 4, -1,"ntruleps>=2&&mt>140"));

  
  vector<int> ra4_sec1;
  ra4_sec1.push_back(isec1);
  ra4_sec1.push_back(isec1+1);
  ra4_sec1.push_back(isec1+2);

  vector<int> ra4_sec6;
  ra4_sec6.push_back(isec6);
  ra4_sec6.push_back(isec6+1);
  //ra4_sec6.push_back(isec6+2);
  //ra4_sec6.push_back(isec6+3);

  vector<int> ra4_tt_t1_noskim;
  ra4_tt_t1_noskim.push_back(nsam);
  ra4_tt_t1_noskim.push_back(nsam+1);
  ra4_tt_t1_noskim.push_back(nsam+2);


  vector<hfeats> vars;

  TString cuts("(nmus+nels)==1");
  switch(section){
  case 1: // Intro
    vars.push_back(hfeats("ntruels+ntrumus",5,-0.5,4.5, ra4_sec1, "Number of prompt e+#mu in T1tttt(1500,100)"));
    break;
  case 2: // MJ-mT plane
    cuts = "(nmus+nels)==1&&ht>500&&met>200&&njets>=7&&nbm>=2&&met>400";
    vars.push_back(hfeats("mt",25,0,500, ra4_sam, "m_{T} (GeV)",cuts,140));
    cuts += "&&mt>140";
    vars.push_back(hfeats("mj",32,0,1600, ra4_sam, "M_{J} (GeV)",cuts,400));
    cuts = "(nmus+nels)==1&&ht>500&&met>200&&njets>=9&&nbm>=2&&met>400";
    vars.push_back(hfeats("mt",25,0,500, ra4_sam, "m_{T} (GeV)",cuts,140));
    cuts += "&&mt>140";
    vars.push_back(hfeats("mj",32,0,1600, ra4_sam, "M_{J} (GeV)",cuts,400));

  case 5: // Event selection - N-1 plots
    vars.push_back(hfeats("ht",35,0,3500, ra4_sam_ns, "H_{T} (GeV)",
			  "(nmus+nels)==1&&met>200&&njets>="+minjets+"&&nbm>=2",500));
    cuts += "&&ht>500";
    vars.push_back(hfeats("met",40,0,800, ra4_sam_ns, "MET (GeV)",
			  "(nmus+nels)==1&&ht>500&&njets>="+minjets+"&&nbm>=2",200));
    cuts += "&&met>200";
    vars.push_back(hfeats("njets",18,-0.5,17.5, ra4_sam, "Number of jets",
			  "(nmus+nels)==1&&ht>500&&met>200&&nbm>=2",6.5));
    cuts += "&&njets>="+minjets;
    vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Number of b-tags (CSVM)",
			  "(nmus+nels)==1&&ht>500&&met>200&&njets>="+minjets,1.5));
    cuts += "&&nbm>=2";
    vars.push_back(hfeats("mt",25,0,500, ra4_sam, "m_{T} (GeV)",cuts,140));
    cuts += "&&mt>140";
    vars.push_back(hfeats("mj",32,0,1600, ra4_sam, "M_{J} (GeV)",cuts,mjthresh.Atof()));

    break;

  case 6: // Background estimation
    cuts += "&&ht>500&&met>200";
    vars.push_back(hfeats("mj",28,0,1400, ra4_sec6, "M_{J} (GeV)",cuts+"&&njets<=4", mjthresh.Atof()));
    vars.push_back(hfeats("mj",28,0,1400, ra4_sec6, "M_{J} (GeV)",cuts+"&&njets>=5&&njets<=6", mjthresh.Atof()));
    vars.push_back(hfeats("mj",28,0,1400, ra4_sec6, "M_{J} (GeV)",cuts+"&&njets>=7&&njets<=8", mjthresh.Atof()));
    vars.push_back(hfeats("mj",28,0,1400, ra4_sec6, "M_{J} (GeV)",cuts+"&&njets>=9", mjthresh.Atof()));

    vars.push_back(hfeats("njets",7,-0.5,6.5, ra4_sec6, "Number of 30 GeV jets",cuts+"&&njets<=6"));
    vars.push_back(hfeats("njets",6,6.5,12.5, ra4_sec6, "Number of 30 GeV jets",cuts+"&&njets>=7"));

    vars.push_back(hfeats("njets",7,-0.5,6.5, ra4_sam, "Number of 30 GeV jets",cuts+"&&njets<=6&&mt<=140", -1, "components"));
    vars.push_back(hfeats("njets",7,-0.5,6.5, ra4_sam, "Number of 30 GeV jets",cuts+"&&njets<=6&&mt>140", -1, "components"));
    vars.push_back(hfeats("njets",6,6.5,12.5, ra4_sam, "Number of 30 GeV jets",cuts+"&&njets>=7&&mt<=140", -1, "components"));
    vars.push_back(hfeats("njets",6,6.5,12.5, ra4_sam, "Number of 30 GeV jets",cuts+"&&njets>=7&&mt>140", -1, "components"));

    break;
  case 7: // HT distributions to compare MC stats with tt HT binning
    vars.push_back(hfeats("ht",30,500,3500, ra4_sam, "H_{T} (GeV)",
			  "(nmus+nels)==1&&met>200&&njets>=7&&nbm>=2&&mt>140",500));
    vars.push_back(hfeats("ht",30,500,3500, ra4_sam, "H_{T} (GeV)",
			  "(nmus+nels)==1&&met>200&&njets>=7&&nbm>=2&&mt>140&&mj>400",500));
    vars.push_back(hfeats("ht",30,500,3500, ra4_sam, "H_{T} (GeV)",
			  "(nmus+nels)==1&&met>200&&njets>=7&&nbm>=2&&mt>140&&njets>=9",500));
    
  default:
    break;
  }

  plot_distributions(Samples, vars, luminosity, plot_type, plot_style);


}

