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
  TString minjets("5");
  TString mjthresh("400");
  TString metcut("met_nohf>150");
  TString minht("400");
  TString minbm("1");
  TString luminosity="0.0419"; // in ifb
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  TString folder_ns="/net/cms2/cms2r0/ald77/archive/2015_08_13/";
  TString DY = "/net/cms29/cms29r0/heller/susy_cfa/out/DY_NLO/";
  TString singlelep = "/net/cms29/cms29r0/heller/susy_cfa/out/SingleLep/";
  TString doublelep = "/net/cms29/cms29r0/heller/susy_cfa/out/DoubleLep/";

  vector<TString> s_singlelep;
  s_singlelep.push_back(singlelep+"*.root");
  vector<TString> s_doublelep;
  s_doublelep.push_back(doublelep+"*.root");
  vector<TString> s_tt;
  s_tt.push_back(folder_ns+"*_TTJet*50ns*");
  vector<TString> s_DY;
  s_DY.push_back(DY+"*DY*");
  vector<TString> s_otherDY;
  s_otherDY.push_back(folder_ns+"*_ZJet*");
  s_otherDY.push_back(folder_ns+"*WH_HToBB*");
  s_otherDY.push_back(folder_ns+"*ST_*");
  s_otherDY.push_back(folder_ns+"*_WJets*");
  s_otherDY.push_back(folder_ns+"*QCD_Pt*");

  // Reading ntuples
  vector<sfeats> Samples; 
  
  Samples.push_back(sfeats(s_doublelep, "Data",kBlack,1,
			   "(trig[25]||trig[15])&&json_golden")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_singlelep, "Data",kBlack,1,
			   "(trig[20]||trig[32])&&json_golden")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_DY, "Z+jets", dps::c_qcd));
  Samples.push_back(sfeats(s_tt, "ttbar", dps::c_tt_2l));
  Samples.push_back(sfeats(s_otherDY, "Other", dps::c_other));
 
  vector<int> npv_dl;
  npv_dl.push_back(0);
  npv_dl.push_back(2);
  npv_dl.push_back(3);
  npv_dl.push_back(4);

  vector<int> npv_sl;
  npv_sl.push_back(1);
  npv_sl.push_back(2);
  npv_sl.push_back(3);
  npv_sl.push_back(4);


  

  vector<hfeats> vars;

  TString mll("&&(mumuv_m*(mumuv_m>0)+elelv_m*(elelv_m>0))>80&&(mumuv_m*(mumuv_m>0)+elelv_m*(elelv_m>0))<100");
  
  /* vars.push_back(hfeats("npv",40,0,40, npv_dl, "NPV",
   			"pass&&nmus>=2"+mll));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("npv",40,0,40, npv_dl, "NPV",
   			"pass&&nels>=2"+mll));
			vars.back().whichPlots = "2"; vars.back().normalize = true;*/
  
  vars.push_back(hfeats("npv",40,0,40, npv_dl, "NPV",
   			"(nmus>=2||nels>=2)&&pass"+mll));
  vars.back().whichPlots = "2"; vars.back().normalize = true; vars.back().PU_reweight = true;
  /*

  vars.push_back(hfeats("npv",40,0,40, npv_sl, "NPV",
   			"nmus>=2&&pass"+mll));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("npv",40,0,40, npv_sl, "NPV",
   			"nels>=2&&pass"+mll));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
  
  vars.push_back(hfeats("npv",40,0,40, npv_sl, "NPV",
   			"(nmus>=2||nels>=2)&&pass"+mll));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
  */


  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "1d",true);


}

