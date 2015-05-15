// plot_1d: Macro that plots variables both lumi weighted and normalized to the same area.

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

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  styles style("RA4"); style.setDefaultStyle();
  vector<hfeats> vars;
  TCanvas can;
  /*
  TColor ucsb_blue(1000, 1/255.,57/255.,166/255.);
  TColor ucsb_gold(1001, 255/255.,200/255.,47/255);
  TColor penn_red(1002, 149/255.,0/255.,26/255.);
  TColor uf_orange(1003, 255/255.,74/255.,0/255.);
  TColor uo_green(1004, 0/255.,79/255.,39/255.);
  TColor tcu_purple(1005, 52/255.,42/255.,123/255.);
  TColor tar_heel_blue(1006, 86/255.,160/255.,211/255.);
  TColor sig_teal(1007, 96/255.,159/255.,128/255.);
  TColor sig_gold(1008, 215/255.,162/255.,50/255.);
  TColor seal_brown(1010, 89/255.,38/255.,11/255.);
  */
  TString folder="archive/2015_05_02/skim/";
  TString folder_noskim="archive/15-05-02//";
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_tt_noskim;
  s_tt_noskim.push_back(folder_noskim+"*_TTJet*12.root");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*WJets*");
  vector<TString> s_single;
  s_single.push_back(folder+"*_T*channel*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder+"*TTW*");
  s_ttv.push_back(folder+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD_HT*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");

  // Reading ntuples
  vector<TChain *> chain;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_other, "Other", 1001));
  Samples.push_back(sfeats(s_ttv, "ttV", 1002));
  Samples.push_back(sfeats(s_single, "Single top", 1005));
  Samples.push_back(sfeats(s_wjets, "W + jets", 1004));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", 1006,1,"((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)>=2"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", 1000,1,"((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)<=1"));
  Samples.push_back(sfeats(s_t1t, "T1tttt(1500,100)", 2));
  Samples.push_back(sfeats(s_t1tc, "T1tttt(1200,800)", 2,2));
  Samples.push_back(sfeats(s_tt_noskim, "t#bar{t}", 1000,1));
  //Samples.push_back(sfeats(s_tt, "t#bar{t}", 1000,1));

  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  vector<int> ra4_sam;
  ra4_sam.push_back(0);
  ra4_sam.push_back(1);
  ra4_sam.push_back(2);
  ra4_sam.push_back(3);
  ra4_sam.push_back(4);
  ra4_sam.push_back(5);
  ra4_sam.push_back(6);
  ra4_sam.push_back(7);

  //vector<int> ra4_tt_dilep

  vector<int> ra4_tt_t1;
  ra4_tt_t1.push_back(4);
  ra4_tt_t1.push_back(5);
  ra4_tt_t1.push_back(6);
  ra4_tt_t1.push_back(7);


  vector<int> ra4_tt2l_t1;
  ra4_tt2l_t1.push_back(4);
  // ra4_tt_t1.push_back(5);
  ra4_tt2l_t1.push_back(6);
  ra4_tt2l_t1.push_back(7);
  // ra4_tt_t1.push_back(8);

  // vars.push_back(hfeats("lep_pt",30,0,600, ra4_sam, "Lepton p_{T} (GeV)",
  // 			"ht>500&&met>200&&njets>=6&&nbm>=2&&(nmus+nels)==1"));
  // vars.push_back(hfeats("lep_pt",30,0,600, ra4_tt_t1, "Lepton p_{T} (GeV)",
  // 			"ht>500&&met>200&&njets>=6&&nbm>=2&&(nmus+nels)==1",-1,"shapes"));
  // vars.push_back(hfeats("lep_pt",30,0,600, ra4_sam, "Lepton p_{T} (GeV)",
  // 			"ht>500&&met>200&&njets>=6&&nbm>=2&&mt>150&&(nmus+nels)==1"));

  //////////////////// N-1 plots ////////////////////////////////
  // // Run without skim
  // vars.push_back(hfeats("met",20,0,800, ra4_sam, "MET (GeV)","ht>500&&nbm>=2&&njets>=6&&(nmus+nels)==1",200));
  // vars.push_back(hfeats("ht",35,0,3500, ra4_sam, "H_{T} (GeV)","met>200&&nbm>=2&&njets>=6&&(nmus+nels)==1",500));
  // // Run without skim


  /* vars.push_back(hfeats("mt",48,0,600, ra4_sam, "m_{T} (GeV)", "ht>500&&met>200&&nbm>=2&&njets>=6&&(nmus+nels)==1",150));

  vars.push_back(hfeats("tks_mini_ch",20,0,5, ra4_tt_t1, "tks mini ch",
			"ht>500&&met>200&&nbm>=2&&mt>140&&njets>=6&&(nmus+nels)==1"));

  // vars.push_back(hfeats("tks_mini_ch",20,0,5, ra4_tt_t1, "tks mini ch",
  //	        "ht>500&&met>200&&nbm>=2&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)"));

   // vars.push_back(hfeats("tks_mini_ch",20,0,5, ra4_tt_t1, "tks mini ch",
   //			"ht>500&&met>200&&nbm>=2&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)"));

  vars.push_back(hfeats("tks_mini_ch",20,0,5, ra4_tt_t1, "tks mini ch",
		        "ht>500&&met>200&&nbm>=2&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)"));
  */

  /*
    vars.push_back(hfeats("tks_mini_ne",20,0,5, ra4_tt_t1, "tks mini ne",
		        "ht>500&&met>200&&nbm>=2&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)"));

  */
  
  /* vars.push_back(hfeats("tks_mini_ch",20,0,5, ra4_tt_t1, "tks mini ch",
     "ht>500&&mj>500&&met>200&&nbm>=2&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)"));


     vars.push_back(hfeats("tks_mini_ne",20,0,5, ra4_tt_t1, "tks mini ne",
		        "ht>500&&mj>500&&met>200&&nbm>=2&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)"));
  */

   vars.push_back(hfeats("mt",50,0,200, ra4_tt_t1, "m_{T} (GeV)",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1"));

    vars.push_back(hfeats("mt",50,0,200, ra4_tt_t1, "m_{T} (GeV)",
			"ht>500&&mj>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1"));

     vars.push_back(hfeats("mt",50,0,200, ra4_tt_t1, "m_{T} (GeV)",
			"ht>500&&mj>300&&met>400&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1"));

     
vars.push_back(hfeats("tks_mt",30,0,300, ra4_tt2l_t1, "track m_{T} (GeV), muon tracks, abs chg+neu iso mini < 20",
			     "ht>500&&mj>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&((tks_pt)*(tks_mini_ne+tks_mini_ch))<20"));

 vars.push_back(hfeats("tks_mt",30,0,300, ra4_tt2l_t1, "track m_{T} (GeV), muon tracks, abs chg+neu iso mini < 20",
			     "ht>500&&mj>300&&met>400&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&((tks_pt)*(tks_mini_ne+tks_mini_ch))<20"));


     
       vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,200, ra4_tt2l_t1, "abs chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w"));
       
       vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,200, ra4_tt2l_t1, "abs chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w"));
  

       
       vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,200, ra4_tt_t1, "abs chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>400&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w"));
       
       vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,200, ra4_tt_t1, "abs chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>400&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w"));

       
    vars.push_back(hfeats("mt",50,0,200, ra4_tt_t1, "m_{T} (GeV)",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1"));

    vars.push_back(hfeats("tks_mt",30,0,300, ra4_tt_t1, "track m_{T} (GeV), muon tracks, abs chg+neu mini iso < 10",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&((tks_pt)*(tks_mini_ne+tks_mini_ch))<10"));
    vars.push_back(hfeats("tks_mt",30,0,300, ra4_tt_t1, "track m_{T} (GeV), muon tracks, abs chg+neu iso mini < 20",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&((tks_pt)*(tks_mini_ne+tks_mini_ch))<20"));

    vars.push_back(hfeats("tks_mt",30,0,300, ra4_tt2l_t1, "track m_{T} (GeV), muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w"));
 
    vars.push_back(hfeats("tks_pt",30,0,150, ra4_tt2l_t1, "track p_{T} (GeV), muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w"));
    vars.push_back(hfeats("tks_pt",30,0,150, ra4_tt_t1, "track p_{T} (GeV), muon tracks, chg+neu iso < 0.2",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&(tks_mini_ne+tks_mini_ch)<0.2"));
    vars.push_back(hfeats("tks_pt",30,0,150, ra4_tt_t1, "track p_{T} (GeV), muon tracks, chg+neu iso < 1.0",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&(tks_mini_ne+tks_mini_ch)<1.0"));

     vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,200, ra4_tt_t1, "abs chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w"));
       
       vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,200, ra4_tt_t1, "abs chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w"));


       vars.push_back(hfeats("(tks_pt)*(tks_mini_ch)",40,0,200, ra4_tt_t1, "abs chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w"));
       
       vars.push_back(hfeats("(tks_pt)*(tks_mini_ch)",40,0,200, ra4_tt_t1, "abs chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w"));

        vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,200, ra4_tt_t1, "abs chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&tks_mt<100"));
       
       vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,200, ra4_tt_t1, "abs chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w&&tks_mt<100"));


       vars.push_back(hfeats("(tks_pt)*(tks_mini_ch)",40,0,200, ra4_tt_t1, "abs chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&tks_mt<100"));
       
       vars.push_back(hfeats("(tks_pt)*(tks_mini_ch)",40,0,200, ra4_tt_t1, "abs chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w&&tks_mt<100"));
  

       vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w"));
       
       vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w"));

       /*
       vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w"));
            vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w"));
       
       vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, hadronic tracks truth-matched",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==169)&&tks_from_w&&tks_from_tauhad"));

       vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, hadronic tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==169)&&!tks_from_w&&!tks_from_tauhad"));

																										      */
       vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w"));
       vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w"));

       /*
       vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w"));
             
       vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w"));

       vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, hadronic tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==169)&&tks_from_w&&tks_from_tauhad"));
       vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, hadronic tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==169)&&!tks_from_w&&!tks_from_tauhad"));
																										      */

       /// Now with mT



         vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&tks_mt<100"));
         vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w&&tks_mt<100"));

	   vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&tks_mt<100"));
       vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w&&tks_mt<100"));

       /*
       vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&tks_mt<100"));
            vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w&&tks_mt<100"));
       
       vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, hadronic tracks truth-matched",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==169)&&tks_from_w&&tks_mt<100&&tks_from_tauhad"));

       vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,6, ra4_tt_t1, "chg+neu mini isolation, hadronic tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!(tks_id*tks_id==121||tks_id*tks_id==169)&&!tks_from_w&&tks_mt<100&&!tks_from_tauhad"));
     

       vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&tks_from_w&&tks_mt<100"));
             
       vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, muon tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169&&!tks_from_w&&tks_mt<100"));

       vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, hadronic tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!(tks_id*tks_id==121||tks_id*tks_id==169)&&tks_from_w&&tks_mt<100&&tks_from_tauhad"));
       vars.push_back(hfeats("tks_mini_ch",60,0,6, ra4_tt_t1, "chg mini isolation, hadronic tracks",
			     "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!(tks_id*tks_id==121||tks_id*tks_id==169)&&!tks_from_w&&tks_mt<100&&!tks_from_tauhad"));

																												  */
       /*
  
       vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt_t1,"truth els tks mini ch",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121&&tks_from_w&&tks_mt<100"));
       vars.push_back(hfeats("tks_mini_ne",60,0,3, ra4_tt_t1, "truth els tks mini ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==121&&tks_from_w&&tks_mt<100"));
        vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt_t1, "truth els tks mini ch+ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==121&&tks_from_w&&tks_mt<100"));

	    vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt_t1,"truth mus tks mini ch",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==169&&tks_from_w&&tks_mt<100"));
       vars.push_back(hfeats("tks_mini_ne",60,0,3, ra4_tt_t1, "truth mus tks mini ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==169&&tks_from_w&&tks_mt<100"));
        vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt_t1, "truth mus tks mini ch+ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==169&&tks_from_w&&tks_mt<100"));
	
      vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt_t1,"truth had tau tks mini ch",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_tauhad&&tks_mt<100"));
     vars.push_back(hfeats("tks_mini_ne",60,0,3, ra4_tt_t1, "truth had tau tks mini ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_tauhad&&tks_mt<100"));
      vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt_t1, "truth had tau tks mini ch+ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_tauhad&&tks_mt<100"));   

     vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt_t1,"truth lep tau tks mini ch",			     		     
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_taulep&&tks_mt<100"));
     vars.push_back(hfeats("tks_mini_ne",60,0,3, ra4_tt_t1, "truth lep tau tks mini ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_taulep&&tks_mt<100"));
      vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt_t1, "truth lep tau tks mini ch+ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_taulep&&tks_mt<100"));

        vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt_t1,"not from W tks mini ch",			     		     
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!tks_from_w&&tks_mt<100"));
     vars.push_back(hfeats("tks_mini_ne",60,0,3, ra4_tt_t1, "not from W tks mini ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!tks_from_w&&tks_mt<100"));
      vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt_t1, "not from W tks mini ch+ne",
			    "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!tks_from_w&&tks_mt<100"));

     
       vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt_t1,"truth els tks mini ch",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==121&&tks_from_w&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     vars.push_back(hfeats("tks_mini_ne",60,0,3, ra4_tt_t1, "truth els tks mini ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==121&&tks_from_w&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt_t1, "truth els tks mini ch+ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==121&&tks_from_w&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));

      vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt_t1,"truth mus tks mini ch",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==169&&tks_from_w&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     vars.push_back(hfeats("tks_mini_ne",60,0,3, ra4_tt_t1, "truth mus tks mini ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==169&&tks_from_w&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt_t1, "truth mus tks mini ch+ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==169&&tks_from_w&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     
      vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt_t1,"truth had tau tks mini ch",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_tauhad&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     vars.push_back(hfeats("tks_mini_ne",60,0,3, ra4_tt_t1, "truth had tau tks mini ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_tauhad&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt_t1, "truth had tau tks mini ch+ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_tauhad&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     
       vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt_t1,"truth lep tau tks mini ch",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_taulep&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     vars.push_back(hfeats("tks_mini_ne",60,0,3, ra4_tt_t1, "truth lep tau tks mini ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_taulep&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt_t1, "truth lep tau tks mini ch+ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_taulep&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));

       vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt_t1,"not from W tks mini ch",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!tks_from_w&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     vars.push_back(hfeats("tks_mini_ne",60,0,3, ra4_tt_t1, "not from W tks mini ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!tks_from_w&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));
     vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt_t1, "not from W tks mini ch+ne",
		        "ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&!tks_from_w&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1"));

  
       */

     //TMath::Abs(TMath::Abs(TMath::Abs(phi1 - phi2) - TMath::Pi())-TMath::Pi());
  /*  vars.push_back(hfeats("abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth els tks dPhi MET",
			   "ht>500&&mj>400&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==121&&tks_from_w"));

     vars.push_back(hfeats("abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth mus tks dPhi MET",
			   "ht>500&&mj>400&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==169&&tks_from_w"));
     
     vars.push_back(hfeats("abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth had tau tks dPhi MET",
			   "ht>500&&mj>400&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_tauhad"));
     
     vars.push_back(hfeats("abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth lep tau tks dPhi MET",
			   "ht>500&&mj>400&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_taulep"));

     //2*MET_*RA4MusPt_->at(0)*(1-TMath::Cos(METPhi_-RA4MusPhi_->at(0)))
  
     
     vars.push_back(hfeats("sqrt(2*met*tks_pt*(1-cos(met_phi-tks_phi)))",20,0,200, ra4_tt_t1,"truth els tks mT MET",
			   "ht>500&&mj>400&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==121&&tks_from_w"));

     vars.push_back(hfeats("sqrt(2*met*tks_pt*(1-cos(met_phi-tks_phi)))",20,0,200, ra4_tt_t1,"truth mus tks mT MET",
			   "ht>500&&mj>400&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==169&&tks_from_w"));
     
     vars.push_back(hfeats("sqrt(2*met*tks_pt*(1-cos(met_phi-tks_phi)))",20,0,200, ra4_tt_t1,"truth had tau tks mT MET",
			   "ht>500&&mj>400&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_tauhad"));
     
     vars.push_back(hfeats("sqrt(2*met*tks_pt*(1-cos(met_phi-tks_phi)))",20,0,200, ra4_tt_t1,"truth lep tau tks mT MET",
			   "ht>500&&mj>400&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_taulep"));

																				       */
       /*
       vars.push_back(hfeats("abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth els tks dPhi MET",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==121&&tks_from_w"));

     vars.push_back(hfeats("abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth mus tks dPhi MET",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==169&&tks_from_w"));
     
     vars.push_back(hfeats("abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth had tau tks dPhi MET",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_tauhad"));
     
     vars.push_back(hfeats("abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth lep tau tks dPhi MET",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_taulep"));

     
      vars.push_back(hfeats("abs(abs(abs(tks_phi-lep_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth els tks dPhi lep",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==121&&tks_from_w"));

     vars.push_back(hfeats("abs(abs(abs(tks_phi-lep_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth mus tks dPhi lep",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==169&&tks_from_w"));
     
     vars.push_back(hfeats("abs(abs(abs(tks_phi-lep_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth had tau tks dPhi lep",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_tauhad"));
     
     vars.push_back(hfeats("abs(abs(abs(tks_phi-lep_phi)-3.14159)-3.14159)",20,0,3.15, ra4_tt_t1,"truth lep tau tks dPhi lep",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_taulep"));


     //2*MET_*RA4MusPt_->at(0)*(1-TMath::Cos(METPhi_-RA4MusPhi_->at(0)))
  
     
     vars.push_back(hfeats("tks_mt",20,0,200, ra4_tt_t1,"truth els tks mT MET",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==121&&tks_from_w"));

     vars.push_back(hfeats("tks_mt",20,0,200, ra4_tt_t1,"truth mus tks mT MET",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_tru_id*tks_tru_id==169&&tks_from_w"));
     
     vars.push_back(hfeats("tks_mt",20,0,200, ra4_tt_t1,"truth had tau tks mT MET",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_tauhad"));
     
     vars.push_back(hfeats("tks_mt",20,0,200, ra4_tt_t1,"truth lep tau tks mT MET",
			   "ht>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_from_w&&tks_from_taulep"));
       */
    
  // vars.push_back(hfeats("abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)",60,0,3, ra4_tt_t1, "tks mini ch",
  //			"ht>500&&met>200&&nbm>=2&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&((tks_eta-lep_eta)*(tks_eta-lep_eta)+((tks_pt-lep_pt)/lep_pt)*((tks_pt-lep_pt)/lep_pt))>0.01"));
  
  /*vars.push_back(hfeats("mt",48,0,600, ra4_sam, "m_{T} (GeV)",
			"ht>500&&met>400&&nbm>=2&&njets>=6&&(nmus+nels)==1",150));
  vars.push_back(hfeats("met",20,200,1200, ra4_sam, "MET (GeV)",
			"ht>500&&nbm>=2&&njets>=6&&mt>150&&(nmus+nels)==1",400));
  vars.push_back(hfeats("njets",18,-0.5,17.5, ra4_sam, "Number of 40 GeV jets",
			"ht>500&&met>400&&nbm>=2&&mt>150&&(nmus+nels)==1",5.5));
  vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Number of b-tags (CSVM)",
			"ht>500&&met>400&&njets>=6&&mt>150&&(nmus+nels)==1",1.5));
  vars.push_back(hfeats("mj",32,0,1600, ra4_sam, "M_{J} (GeV)",
			"ht>500&&met>400&&nbm>=2&&njets>=6&&mt>150&&(nmus+nels)==1",600));

  vars.push_back(hfeats("mt",48,0,600, ra4_sam, "m_{T} (GeV)",
			"ht>500&&met>400&&nbm>=2&&njets>=8&&(nmus+nels)==1",150));
  vars.push_back(hfeats("met",20,200,1200, ra4_sam, "MET (GeV)",
			"ht>500&&nbm>=2&&njets>=8&&mt>150&&(nmus+nels)==1",400));
  vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Number of b-tags (CSVM)",
			"ht>500&&met>400&&njets>=8&&mt>150&&(nmus+nels)==1",1.5));
  vars.push_back(hfeats("mj",32,0,1600, ra4_sam, "M_{J} (GeV)",
  "ht>500&&met>400&&nbm>=2&&njets>=8&&mt>150&&(nmus+nels)==1",600));*/

  //////////////////// N-1 plots ////////////////////////////////



  TString luminosity="10";
  float minLog = 0.04, maxLog = 20;
  double legX = 0.44, legY = 0.88, legSingle = 0.055;
  double legW = 0.26, legH = legSingle*vars[0].samples.size();
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.04); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(42);

  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
  vector< vector<TH1D*> > histo[2];
  vector<TH1D*> varhisto;
  vector<float> nentries;
  vector<float> nevents1;
  vector<float> nevents2;
  vector<float> nevents3;
  nevents1.resize(0);
  nevents2.resize(0); nevents3.resize(0);
  TString hname, pname, variable, leghisto, totCut, title, ytitle;
  for(unsigned var(0); var<vars.size(); var++){
  //if(vars[var].title.Contains("muon") || vars[var].title.Contains("hadronic")) continue;
    cout<<endl;
    // Generating vector of histograms
    title = vars[var].cuts; if(title=="1") title = "";
    title.ReplaceAll("Sum$(abs(mc_id)==11)>0","");title.ReplaceAll("Sum$(abs(mc_id)==13)>0","");
    title.ReplaceAll("nvmus==1&&nmus==1&&nvels==0","1 #mu");
    title.ReplaceAll("nvmus10==0&&nvels10==0", "0 leptons");  
    title.ReplaceAll("(nmus+nels)", "n_{lep}");  title.ReplaceAll("njets30","n_{jets}^{30}"); 
    title.ReplaceAll("els_pt","p^{e}_{T}");title.ReplaceAll("mus_pt","p^{#mu}_{T}");
    title.ReplaceAll("mus_reliso","RelIso"); title.ReplaceAll("els_reliso","RelIso");
    title.ReplaceAll("mus_miniso_tr15","MiniIso"); title.ReplaceAll("els_miniso_tr15","MiniIso");
    title.ReplaceAll("njets","n_{jets}");title.ReplaceAll("abs(lep_id)==13&&","");
    title.ReplaceAll(">=", " #geq "); title.ReplaceAll(">", " > "); title.ReplaceAll("&&", ", "); 
    title.ReplaceAll("met", "MET"); title.ReplaceAll("ht", "H_{T}");  title.ReplaceAll("mt", "m_{T}"); 
    title.ReplaceAll("nleps==1", "1 lepton");  title.ReplaceAll("nbm","n_{b}"); title.ReplaceAll("==", " = "); 
    title.ReplaceAll("nbl[1]","n_{b,l}");
    
    for(unsigned his(0); his < 2; his++){
      varhisto.resize(0);
      for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
	hname = "histo"; hname += var; hname += his; hname += sam;
	varhisto.push_back(new TH1D(hname, title, vars[var].nbins, vars[var].minx, vars[var].maxx));
      }
      histo[his].push_back(varhisto);
    }

    //// Plotting lumi-weighted distributions in histo[0], and then area-normalized in histo[1] ///
    leg.Clear();
    nentries.resize(0);
    variable = vars[var].varname;
    float maxhisto(-999);
    for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
      int isam = vars[var].samples[sam];
      bool isSig = Samples[isam].isSig;
      totCut = Samples[isam].factor+"*"+luminosity+"*weight*("+vars[var].cuts+"&&"+Samples[isam].cut+")"; 
      //cout<<totCut<<endl;
      chain[isam]->Project(histo[0][var][sam]->GetName(), variable, totCut);
      histo[0][var][sam]->SetBinContent(vars[var].nbins,
					  histo[0][var][sam]->GetBinContent(vars[var].nbins)+
					  histo[0][var][sam]->GetBinContent(vars[var].nbins+1));
      nentries.push_back(histo[0][var][sam]->Integral(1,vars[var].nbins));
      if(var==0) nevents1.push_back(nentries[sam]);
      if(var==1) nevents2.push_back(nentries[sam]);
      if(var==2) nevents3.push_back(nentries[sam]);
      histo[0][var][sam]->SetXTitle(vars[var].title);
      ytitle = "Entries for "+luminosity+" fb^{-1}";
      if(vars[var].unit!="") {
	int digits(0);
	float binwidth((vars[var].maxx-vars[var].minx)/static_cast<float>(vars[var].nbins));
	if(binwidth<1) digits = 1;
	ytitle += ("/("+RoundNumber(binwidth,digits) +" "+vars[var].unit+")");
      }
      histo[0][var][sam]->SetYTitle(ytitle);
      // Cloning histos for later
      for(int bin(0); bin<=histo[0][var][sam]->GetNbinsX()+1; bin++)
	histo[1][var][sam]->SetBinContent(bin, histo[0][var][sam]->GetBinContent(bin));

      if(!isSig){ // Adding previous bkg histos
	for(int bsam(sam-1); bsam >= 0; bsam--){
	  histo[0][var][sam]->Add(histo[0][var][bsam]);
	  break;
	  // if(!Samples[vars[var].samples[bsam]].file[0].Contains("T1tttt")){
	  //   histo[0][var][sam]->Add(histo[0][var][bsam]);
	  //   break;
	  // }
	}
	histo[0][var][sam]->SetFillColor(Samples[isam].color);
	histo[0][var][sam]->SetFillStyle(1001);
	histo[0][var][sam]->SetLineColor(1);
	histo[0][var][sam]->SetLineWidth(1);
      } else {
	histo[0][var][sam]->SetLineColor(Samples[isam].color);
	histo[0][var][sam]->SetLineStyle(Samples[isam].style);
	histo[0][var][sam]->SetLineWidth(3);
      }
      if(maxhisto < histo[0][var][sam]->GetMaximum()) maxhisto = histo[0][var][sam]->GetMaximum();
    } // First loop over samples
    int firstplotted(-1);
    for(int sam(vars[var].samples.size()-1); sam >= 0; sam--){
      int isam = vars[var].samples[sam];
      leghisto = Samples[isam].label+" [N = " + RoundNumber(nentries[sam],0) + "]";
      if(var==0){
	cout<<endl<<Samples[isam].label<<" Total entries: " <<RoundNumber(nentries[sam],0)<<endl;

      }
      leg.AddEntry(histo[0][var][sam], leghisto);
      bool isSig = Samples[isam].isSig;
      if(!isSig){
	if(firstplotted < 0) {
	  histo[0][var][sam]->Draw();
	  firstplotted = sam;
	} else histo[0][var][sam]->Draw("same");
      }
    }
    for(int sam(vars[var].samples.size()-1); sam >= 0; sam--){
      int isam = vars[var].samples[sam];
      bool isSig = Samples[isam].isSig;
      if(isSig) histo[0][var][sam]->Draw("same");
    }
    legH = legSingle*vars[var].samples.size(); leg.SetY1NDC(legY-legH);
    leg.SetX1NDC(legX); leg.SetX2NDC(legX+legW);
    leg.Draw(); 
    if(histo[0][var][firstplotted]->GetMinimum() > minLog) histo[0][var][firstplotted]->SetMinimum(minLog);
    histo[0][var][firstplotted]->SetMinimum(minLog);
    histo[0][var][firstplotted]->SetMaximum(maxhisto*maxLog);
    // if(variable=="mt" && var==vars.size()-1) {
    //   histo[0][var][firstplotted]->SetMinimum(0.2);
    //   histo[0][var][firstplotted]->SetMaximum(maxhisto*2);
    // }
    histo[0][var][firstplotted]->Draw("axis same");
    if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, maxhisto*maxLog);
    can.SetLogy(1);
    pname = "plots/1d6/log_lumi_"+vars[var].tag+".pdf";
    //can.SaveAs(pname);
    histo[0][var][firstplotted]->SetMinimum(0);
    histo[0][var][firstplotted]->SetMaximum(maxhisto*1.1);
    can.SetLogy(0);
    pname = "plots/1d6_mu/lumi_"+vars[var].tag+".pdf";
    can.SaveAs(pname);

    //////////// Plotting area-normalized distributions ////////////
    leg.Clear(); maxhisto = -999;
    leg.SetX1NDC(legX-0.08); leg.SetX2NDC(legX+legW-0.08);
    for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
      int isam = vars[var].samples[sam];
      histo[1][var][sam]->SetLineColor(Samples[isam].color);
      histo[1][var][sam]->SetLineStyle(Samples[isam].style);
      histo[1][var][sam]->SetLineWidth(3);
      if(nentries[sam]) histo[1][var][sam]->Scale(100./nentries[sam]);
      if(maxhisto < histo[1][var][sam]->GetMaximum()) maxhisto = histo[1][var][sam]->GetMaximum();
      if(sam==0){
	histo[1][var][sam]->SetXTitle(vars[var].title);
	histo[1][var][sam]->SetYTitle("Entries (%)");
	histo[1][var][sam]->Draw();
      } else histo[1][var][sam]->Draw("same");
      if(vars[var].title.Contains("mini") && !vars[var].title.Contains("abs") && (sam !=1 || vars[var].samples.size()<4 )){
	
	cout<<Samples[isam].label<<" track isolation efficiency"<<endl<<Form("0.4: %.f",histo[1][var][sam]->Integral(histo[1][var][sam]->FindBin(0),histo[1][var][sam]->FindBin(0.39)))<<"%"<<endl;
	cout<<Form("0.6: %.f",histo[1][var][sam]->Integral(histo[1][var][sam]->FindBin(0),histo[1][var][sam]->FindBin(0.59)))<<"%"<<endl;
	cout<<Form("1.0: %.f",histo[1][var][sam]->Integral(histo[1][var][sam]->FindBin(0),histo[1][var][sam]->FindBin(0.99)))<<"%"<<endl;
	cout<<Form("2.0: %.f",histo[1][var][sam]->Integral(histo[1][var][sam]->FindBin(0),histo[1][var][sam]->FindBin(1.99)))<<"%"<<endl;
	cout<<Form("3.0: %.f",histo[1][var][sam]->Integral(histo[1][var][sam]->FindBin(0),histo[1][var][sam]->FindBin(2.99)))<<"%"<<endl<<endl;
      	
    }

      if(vars[var].title.Contains("mini") && vars[var].title.Contains("abs") && sam !=1){
	
	cout<<Samples[isam].label<<" track isolation efficiency"<<endl<<Form("10: %.f",histo[1][var][sam]->Integral(histo[1][var][sam]->FindBin(0),histo[1][var][sam]->FindBin(9.9)))<<"%"<<endl;
	cout<<Form("20: %.f",histo[1][var][sam]->Integral(histo[1][var][sam]->FindBin(0),histo[1][var][sam]->FindBin(19.9)))<<"%"<<endl;
	cout<<Form("40: %.f",histo[1][var][sam]->Integral(histo[1][var][sam]->FindBin(0),histo[1][var][sam]->FindBin(39.9)))<<"%"<<endl;
	cout<<Form("60: %.f",histo[1][var][sam]->Integral(histo[1][var][sam]->FindBin(0),histo[1][var][sam]->FindBin(59.9)))<<"%"<<endl;
	cout<<Form("70: %.f",histo[1][var][sam]->Integral(histo[1][var][sam]->FindBin(0),histo[1][var][sam]->FindBin(69.9)))<<"%"<<endl<<endl;
      	
    }
      
      
      leghisto = Samples[isam].label;//+" [#mu = ";
      //int digits(1);
      // leghisto += RoundNumber(histo[1][var][sam]->GetMean(),digits) + "]";
      if(vars[var].cuts.Contains("mj>500&&met>200"))leghisto+= +" [N_{tks} = " + RoundNumber(nentries[sam],0) + ", from N_{events} = "+RoundNumber(nevents2[sam],0)+"]";
      else if(vars[var].cuts.Contains("mj>300&&met>400"))leghisto+= +" [N_{tks} = " + RoundNumber(nentries[sam],0) + ", from N_{events} = "+RoundNumber(nevents3[sam],0)+"]";
      else leghisto+= +" [N_{tks} = " + RoundNumber(nentries[sam],0) + ", from N_{events} = "+RoundNumber(nevents1[sam],0)+"]";
      leg.AddEntry(histo[1][var][sam], leghisto);
    } // Loop over samples
    leg.Draw(); 
    if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, maxhisto*1.1);
    histo[1][var][0]->SetMaximum(maxhisto*1.1);
    histo[1][var][0]->Draw("axis same");
    can.SetLogy(0);
    pname = "plots/1d6_mu/shapes_"+vars[var].tag+".pdf";
    can.SaveAs(pname);
    histo[1][var][0]->SetMaximum(maxhisto*maxLog);
    can.SetLogy(1);
    pname = "plots/1d6_mu/log_shapes_"+vars[var].tag+".pdf";
    //can.SaveAs(pname);
  }// Loop over variables

  for(unsigned his(0); his < 2; his++){
    for(unsigned var(0); var<vars.size(); var++){
  //if(vars[var].title.Contains("muon") || vars[var].title.Contains("hadronic")) continue;
      for(unsigned sam(0); sam < vars[var].samples.size(); sam++)
	if(histo[his][var][sam]) histo[his][var][sam]->Delete();
    }
  }
}

