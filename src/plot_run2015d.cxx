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
  TString luminosity="2.25"/*1.264*//*"0.832"*//*"0.135"*/; // in ifb
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

  TString folder_ns     = "/net/cms2/cms2r0/babymaker/babies/2015_11_28/mc/";
  TString folder_nsdata = "/net/cms2/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/";
  TString folder_1l     = "/net/cms2/cms2r0/babymaker/babies/2015_11_28/mc/skim_1lht500met200/";
  TString folder_1ldata = "/net/cms2/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/skim_1lht500met200/";
  //TString folder_2l     = "/net/cms2/cms2r0/babymaker/babies/2015_11_28/mc/";//skim_2l/";
  //TString folder_2ldata = "/net/cms2/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/";//skim_2l/";
  TString folder_2l     = "/net/cms2/cms2r0/jaehyeok/babies/2015_11_28/mc/skim_2l/";
  TString folder_2ldata = "/net/cms2/cms2r0/jaehyeok/babies/2015_11_20/data/skim_2l/";

  TString folder_2vl    = "/net/cms24/cms24r0/jaehyeok/susy_cfa_babies/2015_09_28/skim_2vl/";
  
  //
  //  L =  1.264 pb-1
  // /net/cms2/cms2r0/babymaker/babies/2015_10_25/data/singlelep/combined/skim_1lht500met200
  // /net/cms2/cms2r0/babymaker/babies/2015_10_25/data/hadronic/*
  // 

  /////////////////////////////////////////////// Data  ///////////////////////////////////////////////   
  vector<TString> s_trig_htmht;
  s_trig_htmht.push_back(folder_1ldata+"*HTMHT*root");
  vector<TString> s_trig_dl;
  s_trig_dl.push_back(folder_1ldata+"/*DoubleMu*root");
  s_trig_dl.push_back(folder_1ldata+"/*DoubleE*root");
  vector<TString> s_trig_sl;
  s_trig_sl.push_back(folder_1ldata+"/*Single*root");
  vector<TString> s_trig_dmu;
  s_trig_dmu.push_back(folder_1ldata+"/*DoubleMu*root");
  vector<TString> s_trig_del;
  s_trig_del.push_back(folder_1ldata+"/*DoubleE*root");
  vector<TString> s_trig_sl_2l;
  s_trig_sl_2l.push_back(folder_2ldata+"/*Run*SingleElectron_SingleMuon*root");

  /////////////////////////////////////////////// MC  ///////////////////////////////////////////////   
  // 1-lepton plots : ra4_sam
  vector<TString> s_t1t_ns;
  vector<TString> s_t1tc_ns;
  vector<TString> s_tt;
  s_tt.push_back(folder_1l+"*_TTJets*Lept*root");
  s_tt.push_back(folder_1l+"*_TTJets_HT*root");
  vector<TString> s_wjets;
  s_wjets.push_back(folder_1l+"*_WJetsToLNu*root");
  vector<TString> s_ttv;
  s_ttv.push_back(folder_1l+"*_TTWJets*root");
  s_ttv.push_back(folder_1l+"*_TTZTo*root");
  vector<TString> s_single;
  s_single.push_back(folder_1l+"*_ST_*root");
  vector<TString> s_other;
  s_other.push_back(folder_1l+"*DYJetsToLL*root");
  s_other.push_back(folder_1l+"*_QCD_HT*root");
  s_other.push_back(folder_1l+"*_ZJet*root");
  s_other.push_back(folder_1l+"*_WWTo*root");
  s_other.push_back(folder_1l+"*ggZH_HToBB*root");
  s_other.push_back(folder_1l+"*ttHJetTobb*root");
  vector<TString> s_t1t;
  s_t1t.push_back(folder_1l+"*T1tttt*1500_*root");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder_1l+"*T1tttt*1200_*root");

  // 2-lepton plots : dl_sam
  vector<TString> s_DY;
  s_DY.push_back(folder_2l+"*DYJetsToLL*root");
  vector<TString> s_tt_dl;
  s_tt_dl.push_back(folder_2l+"*_TTJets*Lept*root");
  s_tt_dl.push_back(folder_2l+"*_TTJets_HT*root");
  vector<TString> s_singleDY;
  s_singleDY.push_back(folder_2l+"*ST_*root");
  vector<TString> s_ttvDY; //negligible
  s_ttvDY.push_back(folder_2l+"*_TTWJets*root"); 
  s_ttvDY.push_back(folder_2l+"*_TTZTo*root");
  vector<TString> s_otherDY;
  s_otherDY.push_back(folder_2l+"*_ZJet*root");
  s_otherDY.push_back(folder_2l+"*QCD_HT*root");
  s_otherDY.push_back(folder_2l+"*_WJetsToLNu*root");
  s_otherDY.push_back(folder_2l+"*_WWTo*root");
  s_otherDY.push_back(folder_2l+"*ggZH_HToBB*root");
  s_otherDY.push_back(folder_2l+"*ttHJetTobb*root");

  // Reading ntuples
  vector<sfeats> Samples; 
  TString datatitle = "Data";
  //Samples.push_back(sfeats(s_trig_htmht, datatitle,kBlack,1,"trig[0]&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_sl, datatitle,kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1&&stitch"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  Samples.push_back(sfeats(s_wjets, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_single, "Single top", dps::c_singlet));
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  //Samples.push_back(sfeats(s_qcd, "QCD", dps::c_qcd));
  // Some of Other has leptons, but very little, and this is the easiest to put had tt with QCD
  //Samples.push_back(sfeats(s_other, "Other", dps::c_other, 1,"ntruleps==0")); 
  Samples.push_back(sfeats(s_other, "Other", 1001, 1)); 

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
			   "pass&&(trig[4]||trig[8])&&ht>500&&met>200&&met<400&&njets>=5&&nbm<3&&(nmus+nels)==2&&mj>250")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_sl, "Data 1l, N_{jets} #geq 6, m_{T}#leq140, n_{b}#geq1", dps::c_tt_1l, 1,
			   "pass&&(trig[4]||trig[8])&&ht>500&&met>200&&met<400&&njets>=6&&(nmus+nels)==1&&mt<=140&&nbm>=1&&mj>250")); //Samples.back().isData = true;
  vector<int> mj_sam;
  mj_sam.push_back(imj);
  mj_sam.push_back(imj+1);

  int idl(Samples.size());
  // Samples.push_back(sfeats(s_trig_dl, "Data",kBlack,1,
  // 			   "(trig[10]||trig[9])&&json")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_sl_2l, "Data",kBlack,1,
			   "(trig[20]||trig[22])&&pass")); Samples.back().isData = true;
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
 
  // Invariant masses
  TString mll("(mumu_m*(mumu_m>0&&mumu_pt1>30)+elel_m*(elel_m>0&&elel_pt1>30))>80&&(mumu_m*(mumu_m>0&&mumu_pt1>30)+elel_m*(elel_m>0&&elel_pt1>30))<100");
 
  //
  // Figure 29 
  //
/*
  // MET needs to use unskimmed babies
  vars.push_back(hfeats("met",16,0,800, ra4_sam, "MET [GeV]",
          "pass&&(nels+nmus)==1&&ht>"+minht+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true; 

  vars.push_back(hfeats("ht",10,500,1500, ra4_sam, "HT [GeV]",
          "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("njets",11,-0.5,10.5, ra4_sam, "Number of jets",
          "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("nbm",5,-0.5,4.5, ra4_sam, "Number of b-tags (CSVM)",
          "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  // 
  // Figure 30
  // 
  vars.push_back(hfeats("jets_pt[0]",16,0,800, ra4_sam, "p_{T}(j_{1}) [GeV]",
          "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep[0]==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_pt",16,0,800, ra4_sam, "p_{T}(j) [GeV]",
          "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_eta[0]",10,-2.5,2.5, ra4_sam, "#eta(j_{1}) [GeV]",
          "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep[0]==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("jets_eta",10,-2.5,2.5, ra4_sam, "#eta(j) [GeV]",
          "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  vars.push_back(hfeats("jets_csv",20,0,1, ra4_sam, "csv(j) [GeV]",
          "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  vars.push_back(hfeats("jets_csv[0]",20,0,1, ra4_sam, "csv(j_{1}) [GeV]",
          "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm+"&&jets_islep[0]==0"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  // 
  // Figure 31
  // 
  vars.push_back(hfeats("Max$(els_pt*(els_miniso<0.1&&els_sigid&&els_pt>20))",10,0.001,200, ra4_sam, "p_{T}(electron) [GeV]",
          "pass&&(nels==1&&nmus==0)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true; 
  
  vars.push_back(hfeats("Max$(mus_pt*(mus_miniso<0.2&&mus_sigid&&mus_pt>20))",10,0.001,200, ra4_sam, "p_{T}(muon) [GeV]",
          "pass&&(nels==0&&nmus==1)&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  // 
  // Figure 32
  // 
  vars.push_back(hfeats("fjets_pt[0]",8,0,800, ra4_sam, "p_{T}(J_{1}) [GeV]",
  			"pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_pt",8,0,800, ra4_sam, "p_{T}(J) [GeV]",
  			"pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m[0]",8,0,480, ra4_sam, "m(J_{1}) [GeV]",
  			"pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m",8,0,480, ra4_sam, "m(J) [GeV]",
  			"pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_nconst[0]",6,-0.5,5.5, ra4_sam, "Number of fatjet1 constituents",
            "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_nconst",6,-0.5,5.5, ra4_sam, "Number of fatjet constituents",
            "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  // 
  // Figure 33
  // 
  vars.push_back(hfeats("nfjets",10,-0.5,9.5, ra4_sam, "Number of fatjets",
            "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_m[0]",40,0,400, ra4_sam, "m(J_{1}) [GeV]",
  			"pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets<"+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true; // for mtop peak

  vars.push_back(hfeats("mj",12,0,600, ra4_sam, "M_{J} [GeV]",
  			"pass&&mt<140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true;
 
  vars.push_back(hfeats("mj",12,0,600, ra4_sam, "M_{J} [GeV]",
  			"pass&&mt>140&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  // 
  // Figure 34
  // 
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "pass&&mj<400&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true; 
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "pass&&mj<400&&nels==1&&nmus==0&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true; 
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "pass&&mj<400&&nels==0&&nmus==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true; 
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "pass&&mj>400&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true; 

  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "pass&&mj>400&&nels==1&&nmus==0&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true; 

  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "pass&&mj>400&&nels==0&&nmus==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
 
  // 
  // Figure 35
  // 
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "pass&&(nels+nmus)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm==0"));
  vars.back().whichPlots = "1"; vars.back().normalize = true; 
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "pass&&nels==1&&nmus==0&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm==0"));
  vars.back().whichPlots = "1"; vars.back().normalize = true; 
  
  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "pass&&nels==0&&nmus==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm==0"));
  vars.back().whichPlots = "1"; vars.back().normalize = true; 
*/

  // 
  // Figure 36
  // 
  vars.push_back(hfeats("mj",10,0,500, dl_sam, "M_{J} [GeV]",
          "pass&&nels==1&&nmus==1&&met>100&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  vars.push_back(hfeats("mj",10,0,500, dl_sam, "M_{J} [GeV]",
          "pass&&nels==1&&nmus==1&&met>100&&nbm>=1&&(Sum$(leps_pt>30&&abs(leps_eta)<2.1)>0)&&ht>300"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  // 
  // Figure 37
  // 
  vars.push_back(hfeats("mj",20,0,600, dl_sam, "M_{J} [GeV]",
          "pass&&ht>350&&(nmus>=2||nels>=2)&&njets<=3&&"+mll));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  vars.push_back(hfeats("mj",20,0,600, dl_sam, "M_{J} [GeV]",
          "pass&&ht>350&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  //
  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "1d_2015d",true);

}
