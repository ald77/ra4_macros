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
  TString plot_type=".eps";
  TString plot_style="CMSPaper";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  TString folder="/cms2r0/ald77/archive/"+ntuple_date+"/";
  folder = "/cms7r0/heller/code/susy_cfa/out/ttbar_7_23/";
  TString folder_2l="/cms2r0/ald77/archive/2015_07_22/skim_2l/";

  // vector<TString> s_t1t;
  // s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  // vector<TString> s_t1tc;
  // s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");

  vector<TString> s_trig_htmht;
  s_trig_htmht.push_back(folder+"*HTMHT*");
  vector<TString> s_trig_all;
  //s_trig_all.push_back(folder+"/alldata/*");
  s_trig_all.push_back(folder+"/*SingleMuon*");
  s_trig_all.push_back(folder+"/*SingleElectron*");
  vector<TString> s_trig_dl;
  s_trig_dl.push_back(folder_2l+"/*DoubleMu*");
  s_trig_dl.push_back(folder_2l+"/*DoubleE*");
  vector<TString> s_trig_dmu;
  s_trig_dmu.push_back(folder_2l+"/*DoubleMu*");
  vector<TString> s_trig_del;
  s_trig_del.push_back(folder_2l+"/*DoubleE*");


  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*25ns*");
  vector<TString> s_tt_ns;
  s_tt_ns.push_back(folder_2l+"*_TTJet*25ns*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*_WJets*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder+"*ST_*");
  vector<TString> s_DY;
  s_DY.push_back(folder_2l+"*DY*");
  vector<TString> s_qcd;
  s_qcd.push_back(folder+"*QCD_Pt*");
  vector<TString> s_ttv; //negligible
  s_ttv.push_back(folder_2l+"*TTW*"); 
  s_ttv.push_back(folder_2l+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");
  s_other.push_back(folder+"*_TTJet*25ns*");
  vector<TString> s_otherDY;
  s_otherDY.push_back(folder_2l+"*_ZJet*");
  s_otherDY.push_back(folder_2l+"*WH_HToBB*");
  s_otherDY.push_back(folder_2l+"*QCD_Pt*");
  s_otherDY.push_back(folder_2l+"*ST_*");
  s_otherDY.push_back(folder_2l+"*_WJets*");

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

  Samples.push_back(sfeats(s_trig_all, "Lep15_VVVL",kBlack,1,"(trig[1]||trig[5])")); Samples.back().isData = true;
  vector<int> all_sam(ra4_sam);
  all_sam[0] = Samples.size()-1;

  int imj(Samples.size());
  Samples.push_back(sfeats(s_trig_htmht, "Data 2l, N_{jets} #geq 4",kBlack,1,
			   "trig[0]&&njets>=4&&(nvmus+nvels)==2")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_htmht, "Data 1l, N_{jets}#geq5, m_{T}#leq140, n_{b}#geq1",dps::c_tt_1l,1,
			   "trig[0]&&njets>=5&&(nvmus+nvels)==1&&mt<=140&&nbm>=1")); //Samples.back().isData = true;
  vector<int> mj_sam;
  mj_sam.push_back(imj);
  mj_sam.push_back(imj+1);

  int idl(Samples.size());
  Samples.push_back(sfeats(s_trig_dl, "Data",kBlack,1,
			   "(trig[10]||trig[9])")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_DY, "Z+jets", dps::c_qcd));//12
  Samples.push_back(sfeats(s_tt_ns, "t#bar{t}, 2 l", dps::c_tt_2l,1,"ntruleps>=2"));
  Samples.push_back(sfeats(s_tt_ns, "t#bar{t}, 1 l", dps::c_tt_1l, 1,"ntruleps==1"));
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_otherDY, "Other", dps::c_other)); 
  vector<int> dl_sam;
  for(unsigned sam(idl); sam < Samples.size(); sam++) {
    dl_sam.push_back(sam);
  }
  vector<int> dmu_sam(dl_sam), del_sam(dl_sam);
  Samples.push_back(sfeats(s_trig_dmu, "Data",kBlack,1,
			   "(trig[9])")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_del, "Data",kBlack,1,
			   "(trig[10])")); Samples.back().isData = true;
  dmu_sam[0] = Samples.size()-2;
  del_sam[0] = Samples.size()-1;

  vector<hfeats> vars;

  //////////////////////////// DY plots for Ryan's talk ////////////////////////////

  // Invariant masses
  vars.push_back(hfeats("mumu_m",32,20,180, dmu_sam, "m_{#mu#mu} [GeV]",
   			"ht_hlt>350&&nvmus>=2&&njets<=3"));
  vars.back().whichPlots = "2"; //vars.back().normalize = true;
  vars.push_back(hfeats("elel_m",32,20,180, del_sam, "m_{ee} [GeV]",
   			"ht_hlt>350&&nvels>=2&&njets<=3"));
  vars.back().whichPlots = "2"; //vars.back().normalize = true;
  vars.push_back(hfeats("mumu_m*(mumu_m>0)+elel_m*(elel_m>0)",32,20,180, dl_sam, "m_{ll} [GeV]",
   			"ht_hlt>350&&(nvmus>=2||nvels>=2)&&njets<=3"));
  vars.back().whichPlots = "2"; //vars.back().normalize = true;


  vars.push_back(hfeats("mumu_m",32,20,180, dmu_sam, "m_{#mu#mu} [GeV]",
   			"ht_hlt>350&&nvmus>=2&&njets>=4"));
  vars.back().whichPlots = "2"; //vars.back().normalize = true;
  vars.push_back(hfeats("elel_m",32,20,180, del_sam, "m_{ee} [GeV]",
   			"ht_hlt>350&&nvels>=2&&njets>=4"));
  vars.back().whichPlots = "2"; //vars.back().normalize = true;
  vars.push_back(hfeats("mumu_m*(mumu_m>0)+elel_m*(elel_m>0)",32,20,180, dl_sam, "m_{ll} [GeV]",
   			"ht_hlt>350&&(nvmus>=2||nvels>=2)&&njets>=4"));
  vars.back().whichPlots = "2"; //vars.back().normalize = true;


  // Kinematic distributions
  TString mll("(mumu_m*(mumu_m>0)+elel_m*(elel_m>0))>80&&(mumu_m*(mumu_m>0)+elel_m*(elel_m>0))<100");
  vars.push_back(hfeats("ht_hlt",20,0,1000, dl_sam, "H_{T}^{HLT} [GeV]",
   			"(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;
  vars.push_back(hfeats("njets",9,-0.5,8.5, dl_sam, "N_{jets} [GeV]",
   			"ht_hlt>350&&(nvmus>=2||nvels>=2)&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;
  vars.push_back(hfeats("fjets_m[0]",20,0,500, dl_sam, "Most massive fat jet mass [GeV]",
   			"ht_hlt>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;
  vars.push_back(hfeats("nfjets",10,-0.5,9.5, dl_sam, "N_{fat jets}",
   			"ht_hlt>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;


  // MJ distributions
  vars.push_back(hfeats("mj",20,0,600, dl_sam, "M_{J} [GeV]",
   			"ht_hlt>350&&(nvmus>=2||nvels>=2)&&njets<=3&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;
  vars.push_back(hfeats("mj",20,0,600, dl_sam, "M_{J} [GeV]",
   			"ht_hlt>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;


  ////////////////////////////// ttbar plots for Ryan's talk ////////////////////////////
  vars.push_back(hfeats("mj",20,0,1000, ra4_sam, "M_{J} [GeV]",
   			"ht>400&&met>150&nbm>=1&&njets>=5&&(nvmus+nvels)==1&&mt<=140"));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",20,0,1000, ra4_sam, "M_{J} [GeV]",
   			"ht>400&&met>150&&njets>=4&&(nvmus+nvels)==2"));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",20,0,1000, mj_sam, "M_{J} [GeV]",
   			"ht>400&&met>150"));
  vars.back().whichPlots = "2"; vars.back().normalize = true;


  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "1d",true);


}

