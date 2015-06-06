// plot_1d: Macro that plots variables both lumi weighted and normalized to the same area.

#include <iostream>
#include <vector>

#include "TString.h"

#include "utilities_macros.hpp"

using namespace std;

int main(){ 

  TString folder="/cms5r0/ald77/archive/2015_05_21/skim/";
  TString folder_noskim="/cms5r0/ald77/archive/2015_05_21/";
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");
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

  // Reading ntuples
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_t1t, "T1tttt(1500,100)", ra4::c_t1tttt));//0
  Samples.push_back(sfeats(s_t1tc, "T1tttt(1200,800)", ra4::c_t1tttt,2));//1
  Samples.push_back(sfeats(s_tt, "semileptonic t#bar{t}", 38, 1,
			   "(nmus+nels)==1&&njets>=6&&(ntruleps<=1)"));//2
  Samples.push_back(sfeats(s_tt, "fully-leptonic t#bar{t}", kRed-9,1,
			   "(nmus+nels)==1&&njets>=6&&(ntruleps>=2)"));//3
  Samples.push_back(sfeats(s_wjets, "W+jets", ra4::c_wjets));//4
  Samples.push_back(sfeats(s_singlet, "Single t", ra4::c_singlet));//5
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));//6
  Samples.push_back(sfeats(s_other, "Other", ra4::c_other));//7
  Samples.push_back(sfeats(s_tt_noskim, "t#bar{t}", ra4::c_tt_1l,1));//8


  Samples.push_back(sfeats(s_tt, "t#bar{t} n_{j}=4", 2, 1,
			   "njets==4"));//9
  Samples.push_back(sfeats(s_tt, "t#bar{t} n_{j}=5", 4, 1,
			   "njets==5"));//10  
  Samples.push_back(sfeats(s_tt, "t#bar{t} n_{j}=6", 8, 1,
			   "njets==6"));//11
  Samples.push_back(sfeats(s_tt, "t#bar{t} n_{j}=7", 6, 1,
			   "njets==7"));//12
  Samples.push_back(sfeats(s_tt, "t#bar{t} n_{j}=8+",kCyan+1, 1,
			   "njets>=8"));//13
  
  Samples.push_back(sfeats(s_tt, "t#bar{t} m_{T} > 140", 4, 1,
			   "mt>140&&njets>=6&&(nmus+nels)==1"));//14
  Samples.push_back(sfeats(s_tt, "t#bar{t} m_{T} < 140", 2, 1,
			   "mt<=140&&njets>=6&&(nmus+nels)==1"));//15
  Samples.push_back(sfeats(s_tt, "t#bar{t} dilepton reco", 8, 1,
			   "mt<=140&&njets>=5&&(nmus+nels)==2"));//16


  Samples.push_back(sfeats(s_tt, "t#bar{t} 1 l, M_{J} > 500", 4, 1,
			   "mj>500&&(ntruleps<=1)"));//17
  Samples.push_back(sfeats(s_tt, "t#bar{t} 2 l, M_{J} > 500", kRed,1,
			   "mj>500&&(ntruleps>=2)"));//18
  
  Samples.push_back(sfeats(s_tt, "t#bar{t} 1 l, M_{J} < 500", 38, 1,
			   "mj<=500&&(ntruleps<=1)"));//19
  Samples.push_back(sfeats(s_tt, "t#bar{t} 2 l, M_{J} < 500", kRed-9,1,
			   "mj<=500&&(ntruleps>=2)"));//20


  Samples.push_back(sfeats(s_tt, "t#bar{t} 1 l, 1 B", 4, 1,
			   "nbm==1&&(ntruleps<=1)"));//21
  Samples.push_back(sfeats(s_tt, "t#bar{t} 1 l, 2 B", 38, 1,
			   "nbm==2&&(ntruleps<=1)"));//22
  Samples.push_back(sfeats(s_tt, "t#bar{t} 1 l, 3+ B", 8, 1,
			   "nbm>=3&&(ntruleps<=1)"));//23
    
  Samples.push_back(sfeats(s_tt, "t#bar{t} 2 l, 1 B", kRed,1,
			   "nbm==1&&(ntruleps>=2)"));//24
  Samples.push_back(sfeats(s_tt, "t#bar{t} 2 l, 2 B", 46,1,
			   "nbm==2&&(ntruleps>=2)"));//25
  Samples.push_back(sfeats(s_tt, "t#bar{t} 2 l, 3+ B", kOrange,1,
			   "nbm>=3&&(ntruleps>=2)"));//26
  
  

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
  ra4_tt_t1.push_back(0);
  ra4_tt_t1.push_back(1);
  ra4_tt_t1.push_back(2);


  vector<int> ra4_tt_nj_slices;
  ra4_tt_nj_slices.push_back(9);
  ra4_tt_nj_slices.push_back(10);
  ra4_tt_nj_slices.push_back(11);
  ra4_tt_nj_slices.push_back(12);
  ra4_tt_nj_slices.push_back(13);

  vector<int> ra4_tt_mt_slices;
  ra4_tt_mt_slices.push_back(15);
  ra4_tt_mt_slices.push_back(3);
  ra4_tt_mt_slices.push_back(16);
  ra4_tt_mt_slices.push_back(14);
  ra4_tt_mt_slices.push_back(2);


  vector<int> ra4_tt_mt_slices2;
  //dilep, single lep high MJ low MJ
  ra4_tt_mt_slices2.push_back(17);
  ra4_tt_mt_slices2.push_back(18);
  ra4_tt_mt_slices2.push_back(19);
  ra4_tt_mt_slices2.push_back(20);


  vector<int> ra4_tt_mt_slices3;
  //dilep, single lep x 1 2 3+ B
  ra4_tt_mt_slices3.push_back(21);
  ra4_tt_mt_slices3.push_back(22);
  ra4_tt_mt_slices3.push_back(23);
  ra4_tt_mt_slices3.push_back(24);
  ra4_tt_mt_slices3.push_back(25);
  ra4_tt_mt_slices3.push_back(26);
   
  vector<hfeats> vars;


  //////////////////// N-1 plots ////////////////////////////////
  // // Run without skim
  // vars.push_back(hfeats("met",20,0,800, ra4_sam, "MET (GeV)","ht>500&&nbm>=2&&njets>=6&&(nmus+nels)==1",200));
  // vars.push_back(hfeats("ht",35,0,3500, ra4_sam, "H_{T} (GeV)","met>200&&nbm>=2&&njets>=6&&(nmus+nels)==1",500));
  // // Run without skim


  vars.push_back(hfeats("mt",20,0,280, ra4_tt_nj_slices, "combined semi and fully leptonic ttbar m_{T} (GeV)",
  			"ht>500&&met>200&&nbm>=2&&(nmus+nels)==1",140));

  vars.push_back(hfeats("mt",20,0,280, ra4_tt_nj_slices, "semileptonic ttbar m_{T} (GeV)",
			"ht>500&&met>200&&nbm>=2&&(nmus+nels)==1&&ntruleps<=1",140));

  vars.push_back(hfeats("mt",20,0,280, ra4_tt_nj_slices, "fully leptonic ttbar m_{T} (GeV)",
			"ht>500&&met>200&&nbm>=2&&(nmus+nels)==1&&ntruleps>=2",140));


  vars.push_back(hfeats("mt",20,0,280, ra4_tt_mt_slices2, "m_{T} (GeV)",
			"ht>500&&njets>=6&&met>200&&(nmus+nels)==1&&nbm>=2",140));

  vars.push_back(hfeats("mt",20,0,280, ra4_tt_mt_slices3, "m_{T} (GeV)",
			"ht>500&&njets>=6&&met>200&&(nmus+nels)==1&&nbm>=1",140));
    
  vars.push_back(hfeats("mt",20,0,280, ra4_tt_mt_slices3, "m_{T} (GeV)",
			"mj>500&&ht>500&&njets>=6&&met>200&&(nmus+nels)==1&&nbm>=1",140));
     
  vars.push_back(hfeats("mt",20,0,280, ra4_tt_mt_slices3, "m_{T} (GeV)",
			"mj<=500&&ht>500&&njets>=6&&met>200&&(nmus+nels)==1&&nbm>=1",140));

  vars.push_back(hfeats("mt",20,0,280, ra4_tt_mt_slices3, "m_{T} (GeV)",
			"mj>500&&ht>500&&njets>=6&&met>200&&(nmus+nels)==1&&nbm>=1",140));
     
  vars.push_back(hfeats("mt",20,0,280, ra4_tt_mt_slices3, "m_{T} (GeV)",
			"mj<=500&&ht>500&&njets>=6&&met>200&&(nmus+nels)==1&&nbm>=1",140));


    
  vars.push_back(hfeats("mj",15,0,1000, ra4_tt_nj_slices, "M_{J} (GeV)",
  			"ht>500&&met>200&&nbm>=2&&mt>140&&(nmus+nels)==1"));
  
  vars.push_back(hfeats("mj",20,0,1000, ra4_tt_nj_slices, "M_{J} (GeV)",
  			"ht>500&&met>200&&nbm>=2&&mt<=140&&(nmus+nels)==1"));

  
  vars.push_back(hfeats("mj",20,0,1000, ra4_tt_mt_slices, "M_{J} (GeV)",
  			"ht>500&&met>200&&nbm>=2"));
  

  
  /* vars.push_back(hfeats("mt",60,0,600, ra4_sam, "m_{T} (GeV)",
     "ht>500&&met>400&&nbm>=2&&njets>=6&&(nmus+nels)==1",140));
     vars.push_back(hfeats("met",20,200,1200, ra4_sam, "MET (GeV)",
     "ht>500&&nbm>=2&&njets>=6&&mt>140&&(nmus+nels)==1",400));
     vars.push_back(hfeats("njets",18,-0.5,17.5, ra4_sam, "Number of 40 GeV jets",
     "ht>500&&met>400&&nbm>=2&&mt>140&&(nmus+nels)==1",5.5));
     vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Number of b-tags (CSVM)",
     "ht>500&&met>400&&njets>=6&&mt>140&&(nmus+nels)==1",1.5));
     vars.push_back(hfeats("mj",32,0,1600, ra4_sam, "M_{J} (GeV)",
     "ht>500&&met>400&&nbm>=2&&njets>=6&&mt>140&&(nmus+nels)==1",500));

     vars.push_back(hfeats("mt",60,0,600, ra4_sam, "m_{T} (GeV)",
     "ht>500&&met>400&&nbm>=2&&njets>=8&&(nmus+nels)==1",140));
     vars.push_back(hfeats("met",20,200,1200, ra4_sam, "MET (GeV)",
     "ht>500&&nbm>=2&&njets>=8&&mt>140&&(nmus+nels)==1",400));
     vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Number of b-tags (CSVM)",
     "ht>500&&met>400&&njets>=8&&mt>140&&(nmus+nels)==1",1.5));
     vars.push_back(hfeats("mj",32,0,1600, ra4_sam, "M_{J} (GeV)",
     "ht>500&&met>400&&nbm>=2&&njets>=8&&mt>140&&(nmus+nels)==1",500));*/

  plot_distributions(Samples, vars, "10", ".eps", "RA4");

}

