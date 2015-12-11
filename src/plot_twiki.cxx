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
  TString minjets("6");
  TString mjthresh("400");
  TString metcut("met>200");
  TString minht("500");
  TString minbm("1");
  TString luminosity="2.1"/*"1.264"*//*"0.832"*//*"0.135"*/; // in ifb
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  /* 
  TString folder_ns     = "/net/cms2/cms2r0/babymaker/babies/2015_11_28/mc/bkg/skim_ht500met200ge1l/";
  TString folder_nsdata = "/net/cms2/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/skim_1lht500met200/";
  TString folder_1l     = "/net/cms2/cms2r0/babymaker/babies/2015_11_28/mc/bkg/skim_ht500met200ge1l/";
  TString folder_1ldata = "/net/cms2/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/skim_1lht500met200/";  
  TString folder_2l     = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/mc/skim_2l/";
  TString folder_2ldata = "/net/cms2/cms2r0/jaehyeok/babies/2015_11_20/data/skim_2l/";
  */ 
  ///*
  TString folder_had    = "/net/cms2/cms2r0/babymaker/babies/2015_11_20/data/hadronic/skim_njets4/";
  TString folder_ns     = "/net/cms2/cms2r0/babymaker/babies/2015_11_28/mc/";
  TString folder_nsdata = "/net/cms2/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/";
  TString folder_1l     = "/net/cms2/cms2r0/babymaker/babies/2015_11_28/mc/skim_1lht500met200/";
  TString folder_1ldata = "/net/cms2/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/skim_1lht500met200/";  
  TString folder_sig    = "/net/cms2/cms2r0/babymaker/babies/2015_11_28/mc/scan/";
  //*/
  /*
  TString folder_had    = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/data/hadronic/";
  TString folder_ns     = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/mc/";
  TString folder_nsdata = "/net/cms2/cms2r0/babymaker/babies/2015_10_25/data/singlelep/combined/";
  TString folder_1l     = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/mc/skim_1lht500met200/";
  TString folder_1ldata = "/net/cms2/cms2r0/babymaker/babies/2015_10_25/data/singlelep/combined/skim_1lht500met200/";
  TString folder_sig    = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/mc/skim_1lht500met200/";
  */ 
  /////////////////////////////////////////////// Data  ///////////////////////////////////////////////   
  vector<TString> s_trig_0l;
  s_trig_0l.push_back(folder_had+"/*JetHT*.root");
  vector<TString> s_trig_sl;
  s_trig_sl.push_back(folder_1ldata+"/*Single*.root");
  vector<TString> s_trig_dl;
  s_trig_dl.push_back(folder_nsdata+"/*Single*.root");

  /////////////////////////////////////////////// MC  ///////////////////////////////////////////////   
  // 1-lepton plots : ra4_sam
  vector<TString> s_t1t_ns;
  vector<TString> s_t1tc_ns;
  vector<TString> s_tt;
  s_tt.push_back(folder_1l+"*_TTJets*Lept*");
  s_tt.push_back(folder_1l+"*_TTJets_HT*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder_1l+"*_WJetsToLNu*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder_1l+"*_TTWJets*");
  s_ttv.push_back(folder_1l+"*_TTZTo*");
  vector<TString> s_single;
  s_single.push_back(folder_1l+"*_ST_*");
  vector<TString> s_other;
  s_other.push_back(folder_1l+"*DYJetsToLL*");
  s_other.push_back(folder_1l+"*_QCD_HT*");
  s_other.push_back(folder_1l+"*_ZJet*");
  s_other.push_back(folder_1l+"*_WWTo*");
  s_other.push_back(folder_1l+"*ggZH_HToBB*");
  s_other.push_back(folder_1l+"*ttHJetTobb*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder_1l+"*T1tttt*1500_*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder_1l+"*T1tttt*1200_*");

  // 2-lepton plots  
  vector<TString> s_DY;
  s_DY.push_back(folder_ns+"*DYJetsToLL*");
  vector<TString> s_tt_dl;
  s_tt_dl.push_back(folder_ns+"*_TTJets*Lept*");
  s_tt_dl.push_back(folder_ns+"*_TTJets_HT*");
  vector<TString> s_singleDY;
  s_singleDY.push_back(folder_ns+"*ST_*");
  vector<TString> s_ttvDY; //negligible
  s_ttvDY.push_back(folder_ns+"*_TTWJets*"); 
  s_ttvDY.push_back(folder_ns+"*_TTZTo*");
  vector<TString> s_otherDY;
  s_otherDY.push_back(folder_ns+"*_ZJet*");
  s_otherDY.push_back(folder_ns+"*QCD_HT*");
  s_otherDY.push_back(folder_ns+"*_WJetsToLNu*");
  s_otherDY.push_back(folder_ns+"*_WWTo*");
  s_otherDY.push_back(folder_ns+"*ggZH_HToBB*");
  s_otherDY.push_back(folder_ns+"*ttHJetTobb*");
  
  // 0-lepton 
  vector<TString> s_qcd;
  s_qcd.push_back(folder_ns+"*QCD_HT*");
  vector<TString> s_ttQCD;
  s_ttQCD.push_back(folder_ns+"*_TTJets*Lept*");
  s_ttQCD.push_back(folder_ns+"*_TTJets_HT*");
  vector<TString> s_singleQCD;
  s_singleQCD.push_back(folder_ns+"*ST*");
  vector<TString> s_otherQCD;
  s_otherQCD.push_back(folder_ns+"*_TTWJets*");
  s_otherQCD.push_back(folder_ns+"*_TTZTo*");
  s_otherQCD.push_back(folder_ns+"*_ZJet*");
  s_otherQCD.push_back(folder_ns+"*DY*");
  s_otherQCD.push_back(folder_ns+"*_WJetsToLNu*");
  s_otherQCD.push_back(folder_ns+"*_WWTo*");
  s_otherQCD.push_back(folder_ns+"*ggZH_HToBB*");
  s_otherQCD.push_back(folder_ns+"*ttHJetTobb*");


  // Reading ntuples
  vector<sfeats> Samples; 
  TString datatitle = "Data";
  Samples.push_back(sfeats(s_trig_sl, datatitle,kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2"));
  Samples.push_back(sfeats(s_wjets, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_single, "Single top", dps::c_singlet));
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_other, "Other", dps::c_qcd, 1)); 

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


  int idl(Samples.size());
  // Samples.push_back(sfeats(s_trig_dl, "Data",kBlack,1,
  // 			   "(trig[10]||trig[9])&&json")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_dl, "Data",kBlack,1,
			   "(trig[20]||trig[22])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt_dl, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2"));
  Samples.push_back(sfeats(s_tt_dl, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps==1"));
  Samples.push_back(sfeats(s_DY, "Z+jets", dps::c_qcd));//12
  Samples.push_back(sfeats(s_singleDY, "QCD", dps::c_singlet));
  Samples.push_back(sfeats(s_ttvDY, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_otherDY, "Other", dps::c_other)); 
  vector<int> dl_sam;
  for(unsigned sam(idl); sam < Samples.size(); sam++) {
    dl_sam.push_back(sam);
  }

  // 0-lepton 
  int iqcd(Samples.size());
  Samples.push_back(sfeats(s_trig_0l, "Data", 1, -1, "trig[12]&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_qcd, "QCD", dps::c_qcd));
  Samples.push_back(sfeats(s_ttQCD, "t#bar{t}", dps::c_tt_1l, 1));
  Samples.push_back(sfeats(s_singleQCD, "Single top", dps::c_singlet));
  Samples.push_back(sfeats(s_otherQCD, "Other", dps::c_other));

  vector<int> qcd_sam;
  for(unsigned sam(iqcd); sam < Samples.size(); sam++) {
    qcd_sam.push_back(sam);
  }

  // Defining samples with signal
  TString factor_c("100"), factor_nc("300");
  int isig(Samples.size());
  Samples.push_back(sfeats(s_t1t, "Sig NC #times "+factor_nc, kRed+2)); Samples.back().factor = factor_nc;
  Samples.push_back(sfeats(s_t1tc, "Sig C #times "+factor_c, kRed+2,2)); Samples.back().factor = factor_c;
  vector<int> sig_sam;
  sig_sam.push_back(ra4_sam[0]);
  sig_sam.push_back(isig);
  sig_sam.push_back(isig+1);
  for(unsigned ind(1); ind<ra4_sam.size(); ind++)
    sig_sam.push_back(ra4_sam[ind]);

  // Defining samples with signal, no skim, for log
  factor_c = "10"; factor_nc = "10";
  isig = Samples.size();
  Samples.push_back(sfeats(s_t1t_ns, "Sig NC #times "+factor_nc, kRed+2)); Samples.back().factor = factor_nc;
  Samples.push_back(sfeats(s_t1tc_ns, "Sig C #times "+factor_c, kRed+2,2)); Samples.back().factor = factor_c;
  vector<int> sig_sam_ns;
  sig_sam_ns.push_back(ra4_sam_ns[0]);
  sig_sam_ns.push_back(isig);
  sig_sam_ns.push_back(isig+1);
  for(unsigned ind(1); ind<ra4_sam.size(); ind++)
    sig_sam_ns.push_back(ra4_sam_ns[ind]);

  // Defining samples with signal, for log
  isig = Samples.size();
  Samples.push_back(sfeats(s_t1t, "Sig NC #times "+factor_nc, kRed+2)); Samples.back().factor = factor_nc;
  Samples.push_back(sfeats(s_t1tc, "Sig C #times "+factor_c, kRed+2,2)); Samples.back().factor = factor_c;
  vector<int> sig_sam_log;
  sig_sam_log.push_back(ra4_sam[0]);
  sig_sam_log.push_back(isig);
  sig_sam_log.push_back(isig+1);
  for(unsigned ind(1); ind<ra4_sam.size(); ind++)
    sig_sam_log.push_back(ra4_sam[ind]);

  vector<hfeats> vars;
 
  // Invariant masses
  TString mll("(mumu_m*(mumu_m>0&&mumu_pt1>30)+elel_m*(elel_m>0&&elel_pt1>30))>80&&(mumu_m*(mumu_m>0&&mumu_pt1>30)+elel_m*(elel_m>0&&elel_pt1>30))<100");
  
  /////////////////////////// N-1 plots ////////////////////////////
 
  //vars.push_back(hfeats("ht",15,500,2000, sig_sam_log, "HT [GeV]",
  //        "(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  //vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("njets",11,-0.5,10.5, sig_sam_log, "Number of jets",
          "(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("nbm",5,-0.5,4.5, sig_sam_log, "Number of b-tags (CSVM)",
          "(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
 
  //vars.push_back(hfeats("npv",30,-0.5,29.5, sig_sam_log, "N_{pv} [GeV]",
  //        "(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  //vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("mt",18,0,360, sig_sam_log, "m_{T} [GeV]",
          "mj>250&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true; 
  
  vars.push_back(hfeats("mj",12,0,600, sig_sam_log, "M_{J} [GeV]",
  			"(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "1";  vars.back().normalize = true;

  /////////////////////////// MJ validation plots //////////////////////////// 

  // nleps==1
  vars.push_back(hfeats("fjets_m[0]",8,0,400, ra4_sam, "m(J_{1}) [GeV]",
  			"(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m[0]",40,0,400, ra4_sam, "m(J_{1}) [GeV]",
  			"(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets<"+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  vars.push_back(hfeats("mj", 12,0,600, ra4_sam, "M_{J} [GeV]",
  			"(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
 
  // nleps==2 dilepton ttbar
  vars.push_back(hfeats("mj",12,0,600, dl_sam, "M_{J} [GeV]",
   			"nels==1&&nmus==1&&ht>300&&met>100&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)"));
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

}
