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

vector<double> GetNEvents(vector<sfeats> AllSamples, vector<int> RelevantSamples, TString cut, TString luminosity);

int main(){ 
  styles style("RA4"); style.setDefaultStyle();
  vector< vector<hfeats> > allvars;
  TString luminosity = "10";
  TCanvas can;
  TString folder="/Users/heller/code/skim_tight/";
  TString folder_noskim="archive/15-05-02//";
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_tt_noskim;
  s_tt_noskim.push_back(folder_noskim+"*_TTJet*12.root");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*WJetsToLNu_HT*");
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
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");

  // Reading ntuples
  vector<TChain *> chain;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_t1t, "T1tttt(1500,100)", ra4::c_t1tttt));//0
  Samples.push_back(sfeats(s_t1tc, "T1tttt(1200,800)", ra4::c_t1tttt,2)); //1
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", ra4::c_tt_1l, 1,
			   "ntruleps<=1"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", ra4::c_tt_2l,1,
			   "ntruleps>=2"));//3
  Samples.push_back(sfeats(s_wjets, "W+jets", ra4::c_wjets));
  Samples.push_back(sfeats(s_singlet, "Single t", ra4::c_singlet));//5
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_other, "Other", ra4::c_other));//7
  Samples.push_back(sfeats(s_tt_noskim, "t#bar{t}", ra4::c_tt_1l,1));

  /*
  vector<int> ra4_sam;
  ra4_sam.push_back(0);
  ra4_sam.push_back(1);
  ra4_sam.push_back(2);
  ra4_sam.push_back(3);
  ra4_sam.push_back(4);
  ra4_sam.push_back(5);
  ra4_sam.push_back(6);
  ra4_sam.push_back(7);


  vector<int> ra4_tt_t1;
  ra4_tt_t1.push_back(4);
  ra4_tt_t1.push_back(5);
  ra4_tt_t1.push_back(6);
  ra4_tt_t1.push_back(7);*/

  //ALL SIGNAL MUST COME FIRST
  vector<int> ra4_tt2l_t1;
  ra4_tt2l_t1.push_back(0);
  ra4_tt2l_t1.push_back(1);
  ra4_tt2l_t1.push_back(3);
 
  vector<TString> selections;
  //selections.push_back("ht>500&&mj>400&&met>400&&nbm>=2&&mt>140&&njets>=7&&(nmus+nels)==1");
  selections.push_back("mj>300&&ht>500&&met>200&&nbm>=1&&mt>140&&njets>=7&&(nmus+nels)==1");
  selections.push_back("mj>400&&ht>500&&met>200&&nbm>=1&&mt>140&&njets>=7&&(nmus+nels)==1");
  selections.push_back("mj>300&&ht>500&&met>400&&nbm>=1&&mt>140&&njets>=7&&(nmus+nels)==1");

  vector< vector<double> > ra4_tt2l_t1_yields;
  for(unsigned int isel=0; isel<selections.size(); isel++){
    ra4_tt2l_t1_yields.push_back(GetNEvents(Samples, ra4_tt2l_t1, selections[isel],luminosity));
  }
  
  //to switch between different kinds of tracks, just need to do a few search and replace:
  // hadrons use this id :!(tks_id*tks_id==169||tks_id*tks_id==121)
  // replace with tks_id*tks_id==121 for elec, tks_id*tks_id==169 for muons

  //VERY IMPORTANT (tks_id*lep_charge)<0   selects OS tracks for hadronic tracks
  // (tks_id*lep_charge)>0 selects OS tracks for leptonic tracks

  
  //different binnings are generally relevant for different kinds of tracks

  //absolute isolation is just relative isolation * pT



  

  vector<TString> isotypes;
  vector<TString> isonames;
  vector<TString> relisotypes;
  vector<TString> relisonames;
  isonames.push_back("abs chg+neu mini isolation");  isotypes.push_back("(tks_pt)*min((tks_mini_ne+tks_mini_ch),(tks_r02_ne+tks_r02_ch))");
  isonames.push_back("abs chg+neu R=0.5 mini isolation");  isotypes.push_back("(tks_pt)*min((tks_mini_ne+tks_mini_ch),(tks_r05_ne+tks_r05_ch))");
  isonames.push_back("abs chg+neu untruncated mini isolation"); isotypes.push_back("(tks_pt)*(tks_mini_ne+tks_mini_ch)");

  isonames.push_back("abs chg mini isolation");  isotypes.push_back("(tks_pt)*min((tks_mini_ch),(tks_r02_ch))");
  isonames.push_back("abs chg R=0.5 mini isolation");  isotypes.push_back("(tks_pt)*min((tks_mini_ch),(tks_r05_ch))");
  isonames.push_back("abs chg untruncated mini isolation"); isotypes.push_back("(tks_pt)*(tks_mini_ch)");

  relisonames.push_back("rel chg+neu mini isolation");  relisotypes.push_back("min((tks_mini_ne+tks_mini_ch),(tks_r02_ne+tks_r02_ch))");
  relisonames.push_back("rel chg+neu R=0.5 mini isolation");  relisotypes.push_back("min((tks_mini_ne+tks_mini_ch),(tks_r05_ne+tks_r05_ch))");
  relisonames.push_back("rel chg+neu untruncated mini isolation"); relisotypes.push_back("(tks_mini_ne+tks_mini_ch)");

  relisonames.push_back("rel chg mini isolation");  relisotypes.push_back("min((tks_mini_ch),(tks_r02_ch))");
  relisonames.push_back("rel chg R=0.5 mini isolation");  relisotypes.push_back("min((tks_mini_ch),(tks_r05_ch))");
  relisonames.push_back("rel chg untruncated mini isolation"); relisotypes.push_back("(tks_mini_ch)");
  
  
  TString prompt = "&&tks_from_w";
  TString nonprompt = "&&!tks_from_w";
  
  vector<TString> tracktype;
  tracktype.push_back( "&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121");
  //tracktype.push_back( "&&tks_pt<=20&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121");
  //tracktype.push_back( "&&tks_pt>20&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121");
  tracktype.push_back( "&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==169");
  tracktype.push_back( "&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)");
  vector<TString> tracknames;
  tracknames.push_back("electron tracks");
  //  tracknames.push_back("electron tracks, p_{T} < 20");
  //tracknames.push_back("electron tracks, p_{T} > 20");
  tracknames.push_back("muon tracks");
  tracknames.push_back("hadronic tracks");

  for(int itrack=0;itrack<3;itrack++){
    vector<hfeats> vars;
    for(int isel=0; isel<1; isel++){
      
	vars.push_back(hfeats("tks_pt",30,0,150,ra4_tt2l_t1,"track p_{T}, "+tracknames[itrack],selections[isel]+tracktype[itrack]+prompt,-1,"",true,ra4_tt2l_t1_yields[isel]));
	vars.push_back(hfeats("tks_pt",30,0,150,ra4_tt2l_t1,"track p_{T}, "+tracknames[itrack],selections[isel]+tracktype[itrack]+nonprompt,-1,"",true,ra4_tt2l_t1_yields[isel]));



	vars.push_back(hfeats("tks_mt",28,0,280,ra4_tt2l_t1,"track m_{T}, "+tracknames[itrack],selections[isel]+tracktype[itrack]+prompt,-1,"",true,ra4_tt2l_t1_yields[isel]));
	vars.push_back(hfeats("tks_mt",28,0,280,ra4_tt2l_t1,"track m_{T}, "+tracknames[itrack],selections[isel]+tracktype[itrack]+nonprompt,-1,"",true,ra4_tt2l_t1_yields[isel]));

      for(int iiso=0;iiso<6;iiso++){
	vars.push_back(hfeats(isotypes[iiso],50,0,25,ra4_tt2l_t1,isonames[iiso]+", "+tracknames[itrack],selections[isel]+tracktype[itrack]+prompt,-1,"",true,ra4_tt2l_t1_yields[isel]));
	vars.push_back(hfeats(isotypes[iiso],50,0,25,ra4_tt2l_t1,isonames[iiso]+", "+tracknames[itrack],selections[isel]+tracktype[itrack]+nonprompt,-1,"",true,ra4_tt2l_t1_yields[isel]));
	vars.push_back(hfeats(relisotypes[iiso],60,0,3,ra4_tt2l_t1,relisonames[iiso]+", "+tracknames[itrack],selections[isel]+tracktype[itrack]+prompt,-1,"",true,ra4_tt2l_t1_yields[isel]));
	vars.push_back(hfeats(relisotypes[iiso],60,0,3,ra4_tt2l_t1,relisonames[iiso]+", "+tracknames[itrack],selections[isel]+tracktype[itrack]+nonprompt,-1,"",true,ra4_tt2l_t1_yields[isel]));
      }
    }
    allvars.push_back(vars);
  }

  //absolute charge + neutral isolation, baseline study region
        //prompt
  /*vars.push_back(hfeats("(tks_pt)*min((tks_mini_ne+tks_mini_ch),(tks_r02_ne+tks_r02_ch))",40,0,100, ra4_tt2l_t1, "abs chg+neu mini isolation, electron tracks",
			"ht>500&&mj>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121&&(tks_from_w)"));

  vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,100, ra4_tt2l_t1, "abs chg+neu untruncated mini isolation, electron tracks",
			"ht>500&&mj>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121&&(tks_from_w)"));
  
        //nonprompt
  vars.push_back(hfeats("(tks_pt)*min((tks_mini_ne+tks_mini_ch),(tks_r02_ne+tks_r02_ch))",40,0,100, ra4_tt2l_t1, "abs chg+neu mini isolation, electron tracks",
			"ht>500&&mj>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121&&!(tks_from_w)"));
        //nonprompt
  vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,100, ra4_tt2l_t1, "abs chg+neu untruncated mini isolation, electron tracks",
			"ht>500&&mj>500&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121&&!(tks_from_w)"));
  
  
  //Absolute charged isolation, prompt and non-prompt
  vars.push_back(hfeats("(tks_pt)*(tks_mini_ch)",40,0,100, ra4_tt2l_t1, "abs chg untruncated mini isolation, electron tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121&&(tks_from_w)"));

    vars.push_back(hfeats("(tks_pt)*min(tks_mini_ch,tks_r02_ch)",40,0,100, ra4_tt2l_t1, "abs chg mini isolation, electron tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121&&(tks_from_w)"));
       
  vars.push_back(hfeats("(tks_pt)*(tks_mini_ch)",40,0,100, ra4_tt2l_t1, "abs chg untruncated mini isolation, electron tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121&&!(tks_from_w)"));

  vars.push_back(hfeats("(tks_pt)*min(tks_mini_ch,tks_r02_ch)",40,0,100, ra4_tt2l_t1, "abs chg mini isolation, electron tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)>0)&&(!tks_is_primary)&&tks_id*tks_id==121&&!(tks_from_w)"));
  */
  /*
  
  //absolute charged + neutral isolation, high MET region
       //prompt   
  vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,100, ra4_tt2l_t1, "abs chg+neu mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>400&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)"));
        //non-prompt 
  vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,100, ra4_tt2l_t1, "abs chg+neu mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>400&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&!(tks_from_w)"));


  //Relative charged + neutral isolation, prompt and non-prompt
  vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt2l_t1, "chg+neu mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)"));
  
  vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt2l_t1, "chg+neu mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&!(tks_from_w)"));

  //Relative charged isolation, prompt and non-prompt
  vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt2l_t1, "chg mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)"));
  vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt2l_t1, "chg mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&!(tks_from_w)"));

  
  
  //track mT with different isolation cuts
  vars.push_back(hfeats("tks_mt",30,0,300, ra4_tt2l_t1, "track m_{T} (GeV), hadronic tracks, abs chg+neu mini iso < 10",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)&&((tks_pt)*(tks_mini_ne+tks_mini_ch))<10"));
  vars.push_back(hfeats("tks_mt",30,0,300, ra4_tt2l_t1, "track m_{T} (GeV), hadronic tracks, abs chg+neu iso mini < 20",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)&&((tks_pt)*(tks_mini_ne+tks_mini_ch))<20"));
  

  //track mT without isolation cuts
  vars.push_back(hfeats("tks_mt",30,0,300, ra4_tt2l_t1, "track m_{T} (GeV), hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)"));

  
  //track pT, with and without isolation cuts
  vars.push_back(hfeats("tks_pt",30,0,150, ra4_tt2l_t1, "track p_{T} (GeV), hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)"));
  vars.push_back(hfeats("tks_pt",30,0,150, ra4_tt2l_t1, "track p_{T} (GeV), hadronic tracks, chg+neu iso < 0.2",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)&&(tks_mini_ne+tks_mini_ch)<0.2"));
  vars.push_back(hfeats("tks_pt",30,0,150, ra4_tt2l_t1, "track p_{T} (GeV), hadronic tracks, chg+neu iso < 1.0",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)&&(tks_mini_ne+tks_mini_ch)<1.0"));
  

  //Now plot only tracks with mT<100:
  //Absolute charged + neutral isolation, prompt and non-prompt
  vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,100, ra4_tt2l_t1, "abs chg+neu mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)&&tks_mt<100"));
       
  vars.push_back(hfeats("(tks_pt)*(tks_mini_ne+tks_mini_ch)",40,0,100, ra4_tt2l_t1, "abs chg+neu mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&!(tks_from_w)&&tks_mt<100"));

  //Absolute charged isolation, prompt and non-prompt
  vars.push_back(hfeats("(tks_pt)*(tks_mini_ch)",40,0,100, ra4_tt2l_t1, "abs chg mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)&&tks_mt<100"));
       
  vars.push_back(hfeats("(tks_pt)*(tks_mini_ch)",40,0,100, ra4_tt2l_t1, "abs chg mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&!(tks_from_w)&&tks_mt<100"));

  //Relative charged + neutral isolation, prompt and non-prompt
  vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt2l_t1, "chg+neu mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)&&tks_mt<100"));
  vars.push_back(hfeats("tks_mini_ne+tks_mini_ch",60,0,3, ra4_tt2l_t1, "chg+neu mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&!(tks_from_w)&&tks_mt<100"));

  //Relative charged isolation, prompt and non-prompt
  vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt2l_t1, "chg mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&(tks_from_w)&&tks_mt<100"));
  vars.push_back(hfeats("tks_mini_ch",60,0,3, ra4_tt2l_t1, "chg mini isolation, hadronic tracks",
			"ht>500&&mj>300&&met>200&&nbm>=1&&mt>140&&njets>=6&&(nmus+nels)==1&&((tks_id*lep_charge)<0)&&(!tks_is_primary)&&!(tks_id*tks_id==169||tks_id*tks_id==121)&&!(tks_from_w)&&tks_mt<100"));

      
  */
    
  plot_distributions(Samples, allvars.at(0), luminosity, ".pdf", "RA4","veto/els");
  plot_distributions(Samples, allvars.at(1), luminosity, ".pdf", "RA4","veto/mus");
  plot_distributions(Samples, allvars.at(2), luminosity, ".pdf", "RA4","veto/had");

}




vector<double> GetNEvents(vector<sfeats> AllSamples, vector<int> RelevantSamples, TString cut , TString luminosity){

  vector<double> nevents;

 for(unsigned int isam =0; isam <RelevantSamples.size(); isam++){
   TChain chain("tree");
   for(unsigned insam(0); insam < AllSamples[RelevantSamples[isam]].file.size(); insam++) chain.Add(AllSamples[RelevantSamples[isam]].file[insam]);

   double yield, uncertainty;
   TString totCut = AllSamples[RelevantSamples[isam]].factor+"*"+luminosity+"*weight*("+cut+"&&"+AllSamples[RelevantSamples[isam]].cut+")"; 
   getYieldErr(chain,totCut,yield,uncertainty);
   nevents.push_back(yield);
 
   
 }
 return nevents;


}
