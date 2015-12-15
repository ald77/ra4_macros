// plot_dps: Macro that plots variables both for comissioning DPS

#include <iostream>
#include <vector>
#include <ctime>

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
  TString minjets("6");
  TString metcut("met>200");
  TString minht("500");
  TString luminosity="2.1"; // in ifb
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
}

using namespace std;

int main(){ 
  time_t begtime, endtime;
  time(&begtime);

  TString skim_1l    = "skim_1lht500met200/", skim_dy = "skim_dy_ht300/";
  TString skim_tt    = "skim_ttdilep_ht300/", skim_had = "skim_qcd_njet10/";
  TString foldermc   = "/cms2r0/babymaker/babies/2015_11_28/mc/";
  TString folderdata = "/cms2r0/babymaker/babies/2015_11_20/data/";
			 
  TString foldermc_1l	 = foldermc+skim_1l;
  TString folderdata_1l	 = folderdata+"singlelep/combined/"+skim_1l;
  TString foldermc_dy	 = foldermc+skim_dy;
  TString folderdata_dy	 = folderdata+"singlelep/combined/"+skim_dy;
  TString foldermc_tt	 = foldermc+skim_tt;
  TString folderdata_tt	 = folderdata+"singlelep/combined/"+skim_tt;
  TString foldermc_had	 = foldermc+skim_had;
  TString folderdata_had = folderdata+"hadronic/"+skim_had;

  /////////////////////////////////////////////// 1l plots  ///////////////////////////////////////////////   
  vector<TString> s_trig_1l;
  s_trig_1l.push_back(folderdata_1l+"/*Single*.root");

  vector<TString> s_tt_1l;
  s_tt_1l.push_back(foldermc_1l+"*_TTJets*Lept*");
  s_tt_1l.push_back(foldermc_1l+"*_TTJets_HT*");
  vector<TString> s_wjets_1l;
  s_wjets_1l.push_back(foldermc_1l+"*_WJetsToLNu*");
  vector<TString> s_ttv_1l;
  s_ttv_1l.push_back(foldermc_1l+"*_TTWJets*");
  s_ttv_1l.push_back(foldermc_1l+"*_TTZTo*");
  vector<TString> s_single_1l;
  s_single_1l.push_back(foldermc_1l+"*_ST_*");
  vector<TString> s_other_1l;
  s_other_1l.push_back(foldermc_1l+"*DYJetsToLL*");
  s_other_1l.push_back(foldermc_1l+"*_QCD_HT*");
  s_other_1l.push_back(foldermc_1l+"*_ZJet*");
  s_other_1l.push_back(foldermc_1l+"*_WWTo*");
  s_other_1l.push_back(foldermc_1l+"*ggZH_HToBB*");
  s_other_1l.push_back(foldermc_1l+"*ttHJetTobb*");
  vector<TString> s_t1t_1l;
  s_t1t_1l.push_back(foldermc_1l+"*T1tttt*1500_*");
  vector<TString> s_t1tc_1l;
  s_t1tc_1l.push_back(foldermc_1l+"*T1tttt*1200_*");

  /////////////////////////////////////////////// 2l tt plots  ///////////////////////////////////////////////   
  vector<TString> s_trig_tt;
  s_trig_tt.push_back(folderdata_tt+"/*Single*.root");

  vector<TString> s_tt_tt;
  s_tt_tt.push_back(foldermc_tt+"*_TTJets*Lept*");
  s_tt_tt.push_back(foldermc_tt+"*_TTJets_HT*");
  vector<TString> s_wjets_tt;
  s_wjets_tt.push_back(foldermc_tt+"*_WJetsToLNu*");
  vector<TString> s_ttv_tt;
  s_ttv_tt.push_back(foldermc_tt+"*_TTWJets*");
  s_ttv_tt.push_back(foldermc_tt+"*_TTZTo*");
  vector<TString> s_single_tt;
  s_single_tt.push_back(foldermc_tt+"*_ST_*");
  vector<TString> s_other_tt;
  s_other_tt.push_back(foldermc_tt+"*DYJetsToLL*");
  s_other_tt.push_back(foldermc_tt+"*_QCD_HT*");
  s_other_tt.push_back(foldermc_tt+"*_ZJet*");
  s_other_tt.push_back(foldermc_tt+"*_WWTo*");
  s_other_tt.push_back(foldermc_tt+"*ggZH_HToBB*");
  s_other_tt.push_back(foldermc_tt+"*ttHJetTobb*");
  vector<TString> s_t1t_tt;
  s_t1t_tt.push_back(foldermc_tt+"*T1tttt*1500_*");
  vector<TString> s_t1tc_tt;
  s_t1tc_tt.push_back(foldermc_tt+"*T1tttt*1200_*");

  /////////////////////////////////////////////// DY plots  ///////////////////////////////////////////////   
  vector<TString> s_trig_dy;
  s_trig_dy.push_back(folderdata_dy+"/*Single*.root");

  vector<TString> s_DY;
  s_DY.push_back(foldermc_dy+"*DYJetsToLL*");
  vector<TString> s_tt_dl;
  s_tt_dl.push_back(foldermc_dy+"*_TTJets*Lept*");
  s_tt_dl.push_back(foldermc_dy+"*_TTJets_HT*");
  vector<TString> s_singleDY;
  s_singleDY.push_back(foldermc_dy+"*ST_*");
  vector<TString> s_ttvDY; //negligible
  s_ttvDY.push_back(foldermc_dy+"*_TTWJets*"); 
  s_ttvDY.push_back(foldermc_dy+"*_TTZTo*");
  vector<TString> s_otherDY;
  s_otherDY.push_back(foldermc_dy+"*_ZJet*");
  s_otherDY.push_back(foldermc_dy+"*QCD_HT*");
  s_otherDY.push_back(foldermc_dy+"*_WJetsToLNu*");
  s_otherDY.push_back(foldermc_dy+"*_WWTo*");
  s_otherDY.push_back(foldermc_dy+"*ggZH_HToBB*");
  s_otherDY.push_back(foldermc_dy+"*ttHJetTobb*");
  
  
  /////////////////////////////////////////////// had plots  ///////////////////////////////////////////////   
  vector<TString> s_trig_had;
  s_trig_had.push_back(folderdata_had+"/*JetHT*.root");

  vector<TString> s_qcd;
  s_qcd.push_back(foldermc_had+"*QCD_HT*");
  vector<TString> s_ttQCD;
  s_ttQCD.push_back(foldermc_had+"*_TTJets*Lept*");
  s_ttQCD.push_back(foldermc_had+"*_TTJets_HT*");
  vector<TString> s_singleQCD;
  s_singleQCD.push_back(foldermc_had+"*ST*");
  vector<TString> s_otherQCD;
  s_otherQCD.push_back(foldermc_had+"*_TTWJets*");
  s_otherQCD.push_back(foldermc_had+"*_TTZTo*");
  s_otherQCD.push_back(foldermc_had+"*_ZJet*");
  s_otherQCD.push_back(foldermc_had+"*DY*");
  s_otherQCD.push_back(foldermc_had+"*_WJetsToLNu*");
  s_otherQCD.push_back(foldermc_had+"*_WWTo*");
  s_otherQCD.push_back(foldermc_had+"*ggZH_HToBB*");
  s_otherQCD.push_back(foldermc_had+"*ttHJetTobb*");


  // Reading 1l ntuples
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_trig_1l, "Data",kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt_1l, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1&&stitch"));
  Samples.push_back(sfeats(s_tt_1l, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  Samples.push_back(sfeats(s_wjets_1l, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_single_1l, "Single top", dps::c_singlet));
  Samples.push_back(sfeats(s_ttv_1l, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_other_1l, "Other", dps::c_qcd, 1, "stitch")); 

  vector<int> ra4_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++)
    ra4_sam.push_back(sam);

  // Reading 2l tt ntuples
  int itt(Samples.size());
  Samples.push_back(sfeats(s_trig_tt, "Data",kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1&&stitch"));
  Samples.push_back(sfeats(s_tt_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  Samples.push_back(sfeats(s_wjets_tt, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_single_tt, "Single top", dps::c_singlet));
  Samples.push_back(sfeats(s_ttv_tt, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_other_tt, "Other", dps::c_qcd, 1, "stitch")); 

  vector<int> tt_sam;
  for(unsigned sam(itt); sam < Samples.size(); sam++)
    tt_sam.push_back(sam);


  // Reading dy ntuples
  int idl(Samples.size());
  Samples.push_back(sfeats(s_trig_dy, "Data",kBlack,1,
			   "(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt_dl, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  Samples.push_back(sfeats(s_tt_dl, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps==1&&stitch"));
  Samples.push_back(sfeats(s_DY, "Z+jets", dps::c_qcd, 1, "stitch"));//12
  Samples.push_back(sfeats(s_singleDY, "QCD", dps::c_singlet));
  Samples.push_back(sfeats(s_ttvDY, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_otherDY, "Other", dps::c_other)); 
  vector<int> dl_sam;
  for(unsigned sam(idl); sam < Samples.size(); sam++) {
    dl_sam.push_back(sam);
  }

  // 0-lepton 
  int iqcd(Samples.size());
  Samples.push_back(sfeats(s_trig_had, "Data", 1, -1, "trig[12]&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_qcd, "QCD", dps::c_qcd));
  Samples.push_back(sfeats(s_ttQCD, "t#bar{t}", dps::c_tt_1l, 1, "stitch"));
  Samples.push_back(sfeats(s_singleQCD, "Single top", dps::c_singlet));
  Samples.push_back(sfeats(s_otherQCD, "Other", dps::c_other, 1, "stitch"));

  vector<int> qcd_sam;
  for(unsigned sam(iqcd); sam < Samples.size(); sam++) {
    qcd_sam.push_back(sam);
  }

  vector<hfeats> vars;
 
  // Invariant masses
  TString mll("(mumu_m*(mumu_m>0&&mumu_pt1>30)+elel_m*(elel_m>0&&elel_pt1>30))>80&&(mumu_m*(mumu_m>0&&mumu_pt1>30)+elel_m*(elel_m>0&&elel_pt1>30))<100");
  

  /////////////////////////// MJ validation plots //////////////////////////// 

  // nleps==1
  vars.push_back(hfeats("fjets_m[0]",20,0,500, ra4_sam, "m(J_{1}) [GeV]",
  			"(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m[0]",40,0,400, ra4_sam, "m(J_{1}) [GeV]",
  			"(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets<"+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  vars.push_back(hfeats("mj", 16,0,800, ra4_sam, "M_{J} [GeV]",
  			"(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
 
  // nleps==2 dilepton ttbar
  vars.push_back(hfeats("mj",12,0,600, tt_sam, "M_{J} [GeV]",
   			"nels==1&&nmus==1&&ht>300&&met>100&&nbm>=1&&Sum$(leps_pt>30)>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  // nleps==2 DY 
  vars.push_back(hfeats("mj",24,0,600, dl_sam, "M_{J} [GeV]",
   			"ht>350&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  // nleps==2 QCD
  vars.push_back(hfeats("mj",50,0,1500, qcd_sam, "M_{J} [GeV]",
    			"ht>1000&&met<50&&(nvmus+nvels)==0&&njets>=10",0));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  //
  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "1d_twiki",true);

  time(&endtime); 
  cout<<"Plots took "<<difftime(endtime, begtime)<<" seconds to draw "
      <<vars.size()<<" plots."<<endl<<endl;  
}
