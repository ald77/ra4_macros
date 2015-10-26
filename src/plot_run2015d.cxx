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
  TString luminosity="0.832"/*"0.135"*/; // in ifb
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
}

using namespace std;
using std::cout;
using std::endl;

//float njets_reweight(int njets);
/*
if(njets<0) return 1;
if(njets==1) return 1.47;
if(njets==2) return 1.182;
if(njets==3) return 1.034;
if(njets==4) return 1.232;
if(njets==5) return 0.913;
if(njets==6) return 0.995;
if(njets==7) return 0.725;
if(njets==8) return 0.486;
if(njets==9) return 0.598;
if(njets>=10) return 0.516;
*/


int main(){ 
/*
  TString folder_ns     = "/net/cms24/cms24r0/jaehyeok/susy_cfa_babies/2015_10_14/";
  TString folder_nsdata = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/data/singlelep/combined/skim_1lht400/";
  TString folder_1l     = "/net/cms24/cms24r0/jaehyeok/susy_cfa_babies/2015_10_14/skim_1lht450/";
  //TString folder_1ldata = "/net/cms2/cms2r0/babymaker/babies/2015_10_13/singlelep/skim_1lht400/";
  TString folder_1ldata = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/data/singlelep/combined/skim_1lht400/";
  TString folder_2l     = "/net/cms24/cms24r0/jaehyeok/susy_cfa_babies/2015_10_14/skim_2l/";
  //TString folder_2ldata = "/net/cms2/cms2r0/babymaker/babies/2015_10_13/singlelep/skim_2l/";
  TString folder_2ldata = "/cms24r0/jaehyeok/susy_cfa_babies/2015_10_21/skim_2l/";
*/
  TString folder_ns     = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/mc/";
  TString folder_nsdata = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/data/singlelep/combined/skim_1lht400/";
  TString folder_1l     = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/mc/skim_1lht400/";
  TString folder_1ldata = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/data/singlelep/combined/skim_1lht400/";
  TString folder_2l     = "/net/cms24/cms24r0/jaehyeok/susy_cfa_babies/2015_10_24/skim_2l/";
  TString folder_2ldata = "/net/cms24/cms24r0/jaehyeok/susy_cfa_babies/2015_10_21/skim_2l/";

  TString folder_2vl    = "/net/cms24/cms24r0/jaehyeok/susy_cfa_babies/2015_09_28/skim_2vl/";

  /////////////////////////////////////////////// Data  ///////////////////////////////////////////////   
  vector<TString> s_trig_htmht;
  s_trig_htmht.push_back(folder_1ldata+"*HTMHT*");
  vector<TString> s_trig_dl;
  s_trig_dl.push_back(folder_1ldata+"/*DoubleMu*");
  s_trig_dl.push_back(folder_1ldata+"/*DoubleE*");
  vector<TString> s_trig_sl;
  s_trig_sl.push_back(folder_1ldata+"/*Single*");
  vector<TString> s_trig_dmu;
  s_trig_dmu.push_back(folder_1ldata+"/*DoubleMu*");
  vector<TString> s_trig_del;
  s_trig_del.push_back(folder_1ldata+"/*DoubleE*");
  vector<TString> s_trig_sl_2l;
  s_trig_sl_2l.push_back(folder_2ldata+"/*Single*");

  /////////////////////////////////////////////// MC  ///////////////////////////////////////////////   
  // 1-lepton plots : ra4_sam

/*
  vector<TString> s_t1t;
  //s_t1t.push_back(folder_1l+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  //s_t1tc.push_back(folder_1l+"*T1tttt*1200_*PU20*");
  vector<TString> s_t1t_ns;
  s_t1t_ns.push_back(folder_ns+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc_ns;
  s_t1tc_ns.push_back(folder_ns+"*T1tttt*1200_*PU20*");
  vector<TString> s_tt;
  s_tt.push_back(folder_1l+"*TTJets_SingleLeptFromT*25ns*");
  s_tt.push_back(folder_1l+"*TTJets_DiLept*25ns*");
  s_tt.push_back(folder_1l+"*TTJets*HT*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder_1l+"*_WJets*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder_1l+"*ST_*");
  vector<TString> s_qcd;
  s_qcd.push_back(folder_1l+"*QCD_HT*");
  vector<TString> s_other;
  s_other.push_back(folder_1l+"*_ZJet*");
  s_other.push_back(folder_1l+"*DY*");
  s_other.push_back(folder_1l+"*WH_HToBB*");
  //s_other.push_back(folder_1l+"*_TTJet*25ns*");
  s_other.push_back(folder_1l+"*TTW*"); 
  s_other.push_back(folder_1l+"*TTZ*"); 
*/

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

  // 2-lepton plots : dl_sam
  vector<TString> s_DY;
  s_DY.push_back(folder_2l+"*DYJetsToLL*");
  vector<TString> s_tt_dl;
  s_tt_dl.push_back(folder_2l+"*_TTJets*Lept*");
  s_tt_dl.push_back(folder_2l+"*_TTJets_HT*");
  vector<TString> s_singleDY;
  s_singleDY.push_back(folder_2l+"*ST_*");
  vector<TString> s_ttvDY; //negligible
  s_ttvDY.push_back(folder_2l+"*_TTWJets*"); 
  s_ttvDY.push_back(folder_2l+"*_TTZTo*");
  vector<TString> s_otherDY;
  s_otherDY.push_back(folder_2l+"*_ZJet*");
  s_otherDY.push_back(folder_2l+"*QCD_HT*");
  s_otherDY.push_back(folder_2l+"*_WJetsToLNu*");
  s_otherDY.push_back(folder_2l+"*_WWTo*");
  s_otherDY.push_back(folder_2l+"*ggZH_HToBB*");
  s_otherDY.push_back(folder_2l+"*ttHJetTobb*");

  // Reading ntuples
  vector<sfeats> Samples; 
  TString datatitle = "Data";
  //Samples.push_back(sfeats(s_trig_htmht, datatitle,kBlack,1,"trig[0]&&json")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_sl, datatitle,kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2"));
  Samples.push_back(sfeats(s_wjets, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_single, "Single top", dps::c_singlet));
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  //Samples.push_back(sfeats(s_qcd, "QCD", dps::c_qcd));
  // Some of Other has leptons, but very little, and this is the easiest to put had tt with QCD
  //Samples.push_back(sfeats(s_other, "Other", dps::c_other, 1,"ntruleps==0")); 
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

  int imj(Samples.size());
  Samples.push_back(sfeats(s_trig_sl_2l, "Data 2l, N_{jets} #geq 5", kBlack, 1,
			   "(trig[4]||trig[8])&&ht>450&&met>200&&njets>=5&&(nmus+nels)==2&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_sl, "Data 1l, N_{jets} #geq 6, m_{T}#leq140, n_{b}#geq1", dps::c_tt_1l, 1,
			   "(trig[4]||trig[8])&&ht>450&&met>200&&njets>=6&&(nmus+nels)==1&&mt<=140&&nbm>=1&&pass")); //Samples.back().isData = true;
  vector<int> mj_sam;
  mj_sam.push_back(imj);
  mj_sam.push_back(imj+1);

  int idl(Samples.size());
  // Samples.push_back(sfeats(s_trig_dl, "Data",kBlack,1,
  // 			   "(trig[10]||trig[9])&&json")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_sl_2l, "Data",kBlack,1,
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
  vector<int> dmu_sam(dl_sam), del_sam(dl_sam);
  //Samples.push_back(sfeats(s_trig_dmu, "Data",kBlack,1,
  //			   "(trig[9])&&json")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_sl, "Data",kBlack,1,
			   "(trig[18])&&pass")); Samples.back().isData = true;
  //Samples.push_back(sfeats(s_trig_del, "Data",kBlack,1,
  //			   "(trig[10])&&json")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_sl, "Data",kBlack,1,
  			   "(trig[22])&&pass")); Samples.back().isData = true;
  dmu_sam[0] = Samples.size()-2;
  del_sam[0] = Samples.size()-1;

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
  factor_c = "20"; factor_nc = "20";
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

  /////////////////////////// N-1 plots ////////////////////////////
/* 
  // (nels+mus)==1
  vars.push_back(hfeats("met",16,0,800, ra4_sam, "MET [GeV]",
          "npv>=10&&npv<=15&&mt<140&&(nels+nmus)==1&&ht>"+minht+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("ht",10,500,1500, ra4_sam, "HT [GeV]",
          "npv>=10&&npv<=15&&mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("njets",11,-0.5,10.5, ra4_sam, "Number of jets",
          "npv>=10&&npv<=15&&mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("nbm",5,-0.5,4.5, ra4_sam, "Number of b-tags (CSVM)",
          "npv>=10&&npv<=15&&mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
*/

/* 
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "mt<140&&mj>400&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true; 

  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "mt<140&&mj>400&&nels==1&&nmus==0&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true; 

  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "mt<140&&mj>400&&nels==0&&nmus==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
*/

  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "mj<400&&(nels+nmus)==1&&mt>120&&mt<240&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "mj<400&&nels==1&&nmus==0&&mt>120&&mt<240&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;
 
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "mj<400&&nels==0&&nmus==1&&mt>120&&mt<240&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "(nels+nmus)==1&&mt>120&&mt<240&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm==0"));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "nels==1&&nmus==0&&mt>120&&mt<240&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm==0"));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "nels==0&&nmus==1&&mt>120&&mt<240&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm==0"));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;

  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "mj<400&&(nels+nmus)==1&&mt>240&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "mj<400&&nels==1&&nmus==0&&mt>240&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;
 
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "mj<400&&nels==0&&nmus==1&&mt>240&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "(nels+nmus)==1&&mt>240&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm==0"));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "nels==1&&nmus==0&&mt>240&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm==0"));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "nels==0&&nmus==1&&mt>240&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm==0"));
  vars.back().whichPlots = "1"; //vars.back().normalize = true;
/*  
  vars.push_back(hfeats("npv",30,-0.5,29.5, ra4_sam, "N_{pv} [GeV]",
          "mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
*/
  // nels==1&&nmus==0
/*
  vars.push_back(hfeats("met",16,0,800, ra4_sam, "MET [GeV]",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("ht",10,400,1400, ra4_sam, "HT [GeV]",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("njets",11,-0.5,10.5, ra4_sam, "Number of jets",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("nbm",5,-0.5,4.5, ra4_sam, "Number of b-tags (CSVM)",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]", // for barrel electron
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&Max$(abs(els_eta)*(els_sigid&&els_miniso<0.1&&els_pt>20))<1.479"));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]", // for end cap electron
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&Max$(abs(els_eta)*(els_sigid&&els_miniso<0.1&&els_pt>20))>1.479"));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm==0"));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
*/
  
  // nels==0&&nmus==1
/*
  vars.push_back(hfeats("met",16,0,800, ra4_sam, "MET [GeV]",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("ht",10,400,1400, ra4_sam, "HT [GeV]",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("njets",11,-0.5,10.5, ra4_sam, "Number of jets",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("nbm",5,-0.5,4.5, ra4_sam, "Number of b-tags (CSVM)",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm==0"));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
*/  
  /////////////////////////// jets //////////////////////////// 
 
  // (nels+nmus)==1 
/*
  vars.push_back(hfeats("jets_pt[0]",16,0,800, ra4_sam, "p_{T}(j_{1}) [GeV]",
          "mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep[0]==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_pt",16,0,800, ra4_sam, "p_{T}(j) [GeV]",
          "mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_eta[0]",10,-2.5,2.5, ra4_sam, "#eta(j_{1}) [GeV]",
          "mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep[0]==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_eta",10,-2.5,2.5, ra4_sam, "#eta(j) [GeV]",
          "mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  vars.push_back(hfeats("jets_csv",20,0,1, ra4_sam, "csv(j) [GeV]",
          "mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  vars.push_back(hfeats("jets_csv[0]",20,0,1, ra4_sam, "csv(j_{1}) [GeV]",
          "mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep[0]==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
*/
  
  // nels==1&&nmus==0 
/*  
  vars.push_back(hfeats("jets_pt[0]",16,0,800, ra4_sam, "p_{T}(j_{1}) [GeV]",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep[0]==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_pt",16,0,800, ra4_sam, "p_{T}(j) [GeV]",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_eta[0]",10,-2.5,2.5, ra4_sam, "#eta(j_{1}) [GeV]",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep[0]==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_eta",10,-2.5,2.5, ra4_sam, "#eta(j) [GeV]",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  vars.push_back(hfeats("jets_csv",20,0,1, ra4_sam, "csv [GeV]",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  // nels==0&&nmus==1

  vars.push_back(hfeats("jets_pt[0]",16,0,800, ra4_sam, "p_{T}(j_{1}) [GeV]",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep[0]==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_pt",16,0,800, ra4_sam, "p_{T}(j) [GeV]",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_eta[0]",10,-2.5,2.5, ra4_sam, "#eta(j_{1}) [GeV]",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep[0]==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_eta",10,-2.5,2.5, ra4_sam, "#eta(j) [GeV]",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  vars.push_back(hfeats("jets_csv",20,0,1, ra4_sam, "csv [GeV]",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
*/

  /////////////////////////// leptons //////////////////////////// 
/*  
  vars.push_back(hfeats("Max$(els_pt*(els_miniso<0.1&&els_sigid&&els_pt>20))",10,0.001,200, ra4_sam, "p_{T}(electron) [GeV]",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("Max$(els_eta*(els_miniso<0.1&&els_sigid&&els_pt>20))",10,-2.5,2.5, ra4_sam, "#eta(electron)",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("Max$(mus_pt*(mus_miniso<0.2&&mus_sigid&&mus_pt>20))",10,0.001,200, ra4_sam, "p_{T}(muon) [GeV]",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  vars.push_back(hfeats("Max$(mus_eta*(mus_miniso<0.2&&mus_sigid&&mus_pt>20))",10,-2.5,2.5, ra4_sam, "#eta(muon)",
          "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
*/

  /////////////////////////// FJ related plots //////////////////////////// 

  // (nels+nmus)==1
/*
  vars.push_back(hfeats("fjets_m[0]",8,0,480, ra4_sam, "m(J_{1}) [GeV]",
  			"mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m",8,0,480, ra4_sam, "m(J) [GeV]",
  			"mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_pt[0]",8,0,800, ra4_sam, "p_{T}(J_{1}) [GeV]",
  			"mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_pt",8,0,800, ra4_sam, "p_{T}(J) [GeV]",
  			"mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
 
  vars.push_back(hfeats("mj",12,0,600, ra4_sam, "M_{J} [GeV]",
  			"mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
 
  vars.push_back(hfeats("mj",12,0,600, ra4_sam, "M_{J} [GeV]",
  			"mt>140&&mj<400&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
 
  vars.push_back(hfeats("nfjets",10,-0.5,9.5, ra4_sam, "Number of fatjets",
            "mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_nconst[0]",6,-0.5,5.5, ra4_sam, "Number of fatjet1 constituents",
            "mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_nconst",6,-0.5,5.5, ra4_sam, "Number of fatjet constituents",
            "mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
*/
  // nels==1&&nmus==0 
/*
  vars.push_back(hfeats("fjets_m[0]",8,0,480, ra4_sam, "m(J_{1}) [GeV]",
  			"mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m",8,0,480, ra4_sam, "m(J) [GeV]",
  			"mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_pt[0]",8,0,800, ra4_sam, "p_{T}(J_{1}) [GeV]",
  			"mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_pt",8,0,800, ra4_sam, "p_{T}(J) [GeV]",
  			"mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("mj",10,0,600, ra4_sam, "M_{J} [GeV]",
  			"mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("nfjets",10,-0.5,9.5, ra4_sam, "Number of fatjets",
          "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_nconst[0]",6,-0.5,5.5, ra4_sam, "Number of fatjet1 constituents",
            "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_nconst",6,-0.5,5.5, ra4_sam, "Number of fatjet constituents",
            "mt<140&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
*/
  // nels==0&&nmus==1
/*
  vars.push_back(hfeats("fjets_m[0]",8,0,480, ra4_sam, "m(J_{1}) [GeV]",
  			"mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m",8,0,480, ra4_sam, "m(J) [GeV]",
  			"mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_pt[0]",8,0,800, ra4_sam, "p_{T}(J_{1}) [GeV]",
  			"mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_pt",8,0,800, ra4_sam, "p_{T}(J) [GeV]",
  			"mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("mj",10,0,600, ra4_sam, "M_{J} [GeV]",
  			"mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("nfjets",10,-0.5,9.5, ra4_sam, "Number of fatjets",
            "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
 
  vars.push_back(hfeats("fjets_nconst[0]",6,-0.5,5.5, ra4_sam, "Number of fatjet1 constituents",
            "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_nconst",6,-0.5,5.5, ra4_sam, "Number of fatjet constituents",
            "mt<140&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
*/  
  /////////////////////////////// 1l vs 2l MJ comparison ////////////////////////////
/*  
  vars.push_back(hfeats("mj", 6, 0, 600, mj_sam, "M_{J} [GeV]",
   			//"pass&&ht>500&&met>100&&nbm>=1&&njets>=5&&(nmus+nels)==2"));
   			//"ht>500&&met>200&&njets>=5&&(nmus+nels)==2"));
   			"pass"));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  //vars.push_back(hfeats("mj", nbins, xbins, mj_sam, "M_{J} [GeV]",
  // 			"pass&&ht>450&&met>100&&njets>=4&&(nmus+nels)==2"));
  //vars.back().whichPlots = "2"; vars.back().normalize = true;
 
  vars.push_back(hfeats("mj", nbins, xbins, mj_sam, "M_{J} [GeV]",
   			"pass&ht>450&&met>50"));
  vars.back().whichPlots = "2"; vars.back().normalize = true; vars.back().maxRatio = 2.2;
 
  vars.push_back(hfeats("mj", nbins, xbins, mj_sam, "M_{J} [GeV]",
   			"pass&ht>450&&met>100"));
  vars.back().whichPlots = "2"; vars.back().normalize = true; vars.back().maxRatio = 2.2;
 
  vars.push_back(hfeats("mj", 6, 0, 600, mj_sam, "M_{J} [GeV]",
   			""));
  vars.back().whichPlots = "2"; vars.back().normalize = true; vars.back().maxRatio = 2.2;
*/ 

  /////////////////////////////// dilepton events //////////////////////////// 
/*
  vars.push_back(hfeats("ht",20,0,1000, dl_sam, "H_{T} [GeV]",
   			"nels==1&&nmus==1&&met>100&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("njets",10,-0.5,9.5, dl_sam, "Number of jets",
   			"nels==1&&nmus==1&&met>100&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("nfjets",7,-0.5,6.5, dl_sam, "Nunmber of fatjets",
   			"nels==1&&nmus==1&&met>100&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("mj",10,0,500, dl_sam, "M_{J} [GeV]",
   			"nels==1&&nmus==1&&met>100&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("mj",10,0,500, dl_sam, "M_{J} [GeV]",
   			"nels==1&&nmus==1&&met>100&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)&&ht>300"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m[0]",20,0,500, dl_sam, "Most massive fat jet mass [GeV]",
   			"nels==1&&nmus==1&&met>100&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m",20,0,500, dl_sam, "Fat jet mass [GeV]",
   			"nels==1&&nmus==1&&met>100&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
*/
/*
  vars.push_back(hfeats("ht",20,0,1000, dl_sam, "H_{T} [GeV]",
   			"nels==1&&nmus==1&&met>150&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)&&ht>400"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("njets",10,-0.5,9.5, dl_sam, "njets [GeV]",
   			"nels==1&&nmus==1&&met>150&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)&&ht>400"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("mj",10,0,500, dl_sam, "njets [GeV]",
   			"nels==1&&nmus==1&&met>150&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)&&ht>400"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m[0]",20,0,500, dl_sam, "Most massive fat jet mass [GeV]",
   			"nels==1&&nmus==1&&met>150&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)&&ht>400"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m",20,0,500, dl_sam, "Fat jet mass [GeV]",
   			"nels==1&&nmus==1&&met>150&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)&&ht>400"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
*/

  
  
  // Invariant masses
  //TString mll("(mumu_m*(mumu_m>0&&mumu_pt1>30)+elel_m*(elel_m>0&&elel_pt1>30))>80&&(mumu_m*(mumu_m>0&&mumu_pt1>30)+elel_m*(elel_m>0&&elel_pt1>30))<100");
  /*
  vars.push_back(hfeats("mumu_m",32,20,180, dl_sam, "m_{#mu#mu} [GeV]",
   			"ht>350&&nmus>=2&&njets<=3"));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("elel_m",32,20,180, dl_sam, "m_{ee} [GeV]",
   			"ht>350&&nels>=2&&njets<=3"));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
  */
/*  
  vars.push_back(hfeats("mumu_m*(mumu_m>0)+elel_m*(elel_m>0)",32,20,180, dl_sam, "m_{ll} [GeV]",
   			"ht>350&&(nmus>=2||nels>=2)&&njets<=3"));
  vars.back().whichPlots = "2"; //vars.back().normalize = true;
    
  vars.push_back(hfeats("mumu_m",32,20,180, dmu_sam, "m_{#mu#mu} [GeV]",
   			"ht>350&&nmus>=2&&njets>=4"));
  vars.back().whichPlots = "2"; //vars.back().normalize = true;
  
  vars.push_back(hfeats("elel_m",32,20,180, del_sam, "m_{ee} [GeV]",
   			"ht>350&&nels>=2&&njets>=4"));
  vars.back().whichPlots = "2"; //vars.back().normalize = true;
*/
/*
  vars.push_back(hfeats("mumu_m*(mumu_m>0)+elel_m*(elel_m>0)",32,20,180, dl_sam, "m_{ll} [GeV]",
   			"ht>350&&(nmus>=2||nels>=2)&&njets>=4"));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  // Kinematic distributions
  vars.push_back(hfeats("ht",20,0,1000, dl_sam, "H_{T}^{HLT} [GeV]",
   			"(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("njets",9,-0.5,8.5, dl_sam, "N_{jets} [GeV]",
   			"ht>350&&(nmus>=2||nels>=2)&&"+mll));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m[0]",20,0,500, dl_sam, "Most massive fat jet mass [GeV]",
   			"ht>350&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("nfjets",10,-0.5,9.5, dl_sam, "N_{fat jets}",
   			"ht>350&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  // MJ distributions
  vars.push_back(hfeats("mj",20,0,600, dl_sam, "M_{J} [GeV]",
   			"ht>350&&(nmus>=2||nels>=2)&&njets<=3&&"+mll));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("mj",20,0,600, dl_sam, "M_{J} [GeV]",
   			"ht>350&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
*/ 
  // ZpT : elel and mumu separately
 /* 
  // (nvmus>=2||nvels>=2)
  vars.push_back(hfeats("mumuv_zpt*(mumuv_zpt>0)+elelv_zpt*(elelv_zpt>0)",10,0,500, dl_sam, "p_{T}(#font[12]{ll}) [GeV]",
   			"ht>350&&(nvmus>=2||nvels>=2)&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;
  
  vars.push_back(hfeats("mumuv_zpt*(mumuv_zpt>0)+elelv_zpt*(elelv_zpt>0)",10,0,500, dl_sam, "p_{T}(#font[12]{ll}) [GeV]",
   			"ht>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;
  
  // (nvels>=2) 
  vars.push_back(hfeats("elelv_zpt*(elelv_zpt>0)",10,0,500, dl_sam, "p_{T}(#font[12]{ee}) [GeV]",
   			"ht>350&&(nvels>=2)&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;
  
  vars.push_back(hfeats("elelv_zpt*(elelv_zpt>0)",10,0,500, dl_sam, "p_{T}(#font[12]{ee}) [GeV]",
   			"ht>350&&(nvels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;
  
  // (nvmus>=2) 
  vars.push_back(hfeats("mumuv_zpt*(mumuv_zpt>0)",10,0,500, dl_sam, "p_{T}(#mu#mu) [GeV]",
   			"ht>350&&(nvmus>=2)&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;
  
  vars.push_back(hfeats("mumuv_zpt*(mumuv_zpt>0)",10,0,500, dl_sam, "p_{T}(#mu#mu) [GeV]",
   			"ht>350&&(nvmus>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2"; // vars.back().normalize = true;
*/  
  //
  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "1d_2015d_test",true);

}

//float njets_reweight(int njets) {
//    if(njets==0) return 1.;
//}
