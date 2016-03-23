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
#include "utilities_macros_rpv.hpp"

namespace {
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
  int section(11);
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  std::string extraWeight("w_lumi/weight");
  std::string extraWeightTopPt("w_lumi*w_toppt/weight");

  bool showData=false;

  TString folder_links="/homes/cawest/links/";

  vector<TString> s_tt_MLM;
  s_tt_MLM.push_back(filestring("TTJets_TuneCUETP8M1_13TeV-madgraphMLM", false));

  vector<TString> s_tt_amcatnlo;
  s_tt_amcatnlo.push_back(filestring("TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", false));

  vector<TString> s_tt_powheg;
  s_tt_powheg.push_back(filestring("TT_TuneCUETP8M1_13TeV-powheg-pythia8", false));

  // Reading ntuples
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_tt_powheg, "t#bar{t}, Powheg", ra4::c_t1tttt, 1, cutandweight("1", extraWeight)));
  Samples.back().doStack = false;
  Samples.push_back(sfeats(s_tt_MLM, "t#bar{t}, aMC@NLO (LO, MLM)", ra4::c_t1tttt, 2, cutandweight("1", extraWeight)));
  Samples.back().doStack = false;
  Samples.push_back(sfeats(s_tt_MLM, "t#bar{t}, (LO, p_{T} reweighted)", kBlue, 1, cutandweight("1", extraWeightTopPt)));
  Samples.back().doStack = false;
  Samples.push_back(sfeats(s_tt_amcatnlo, "t#bar{t}, aMC@NLO (NLO, FxFx)", kBlack, 1, cutandweight("1", extraWeight)));
  Samples.back().doStack = false;

  
  // Adding non-skimmed samples
  vector<int> ra4_sam, ra4_sam_ns;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
  } // Loop over samples

  vector<hfeats> vars;

  TString cuts("(nmus+nels)==1");
  switch(section){
  case 11:

    if(!showData) {

      std::vector<TString> cutlist = {"(nmus+nels)==0", 
				      "(nmus+nels)==1",
				      "(nmus+nels)==0&&nbm>=1&&mj>500&&njets>=6&&ht>1500",
				      "(nmus+nels)==1&&nbm>=1&&mj>500&&njets>=6&&ht>1200"};
      for(auto icut : cutlist) {
	vars.push_back(hfeats("ht",40, 0, 4000, ra4_sam, "H_{T} (GeV)", icut));
	vars.push_back(hfeats("mj",25, 0, 2500, ra4_sam, "M_{J} (GeV)", icut));
	vars.push_back(hfeats("nbm",6, 0, 6, ra4_sam, "N_{b}", icut));
	vars.push_back(hfeats("njets",20, 0, 20, ra4_sam, "N_{jets}", icut));
      }


    }
    else {
      cuts = "(nmus+nels)==1&&ht>1000&&ht<1500&&nbm>=1";
      vars.push_back(hfeats("njets",15, 0, 15, ra4_sam, "N_{jets}", cuts));
      vars.back().normalize = true;
      cuts = "(nmus+nels)==1&&ht>1000&&ht<1500&&nbt>=1&&njets==5";
      vars.push_back(hfeats("nbt",6, 0, 6, ra4_sam, "N_{b,tight}", cuts));
      vars.back().normalize = true;
      cuts = "(nmus+nels)==1&&ht>1000&&ht<1500&&nbm>=1&&njets==5";
      vars.push_back(hfeats("nbm",6, 0, 6, ra4_sam, "N_{b}", cuts));
      vars.back().normalize = true;
      vars.push_back(hfeats("mj",12, 0, 600, ra4_sam, "M_{J} (GeV)", cuts));
      vars.back().normalize = true;
      vars.push_back(hfeats("Max$(jets_pt)",20, 0, 1000, ra4_sam, "p_{T1} (GeV)", cuts));
      vars.back().normalize = true;
      vars.push_back(hfeats("(jets_csv-0.89)/0.11", 10, 0, 1, ra4_sam, "(CSV-CSV_{cut})/(CSV_{max}-CSV_{cut})", cuts));
      vars.back().normalize = true;
    }
  default:
    break;
  }

  if(showData) {
    plot_distributions(Samples, vars, rpv::luminosity, plot_type, plot_style, "ttbarcomp", true);
  }
  else {
    plot_distributions(Samples, vars, rpv::luminosity, plot_type, plot_style, "ttbarcomp", false);
  }
}

