// plot_trigger: Plots for the trigger section of the RA4 note

#include <iostream>
#include <vector>
#include <ctime>

#include "TMath.h"
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TRandom3.h"
#include "TGraphAsymmErrors.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;

int main(){ 
  time_t begtime, endtime;
  time(&begtime);
  TString folder="/afs/cern.ch/user/m/manuelf/work/ucsb/2015_05_25/skim/";
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder+"*_T*t-channel*");
  s_singlet.push_back(folder+"*_T*s-channel*");
  s_singlet.push_back(folder+"*_T*W-channel*");
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD_HT*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");
  s_other.push_back(folder+"*TTW*");
  s_other.push_back(folder+"*TTZ*");
  s_other.push_back(folder+"*_WJets*");

  // Reading ntuples
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", ra4::c_tt_2l,1));  
  Samples.push_back(sfeats(s_singlet, "Single t", ra4::c_singlet));
  Samples.push_back(sfeats(s_other, "Other", ra4::c_other, 1)); 

  vector<TChain *> chain;
  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  vector<int> ra4_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
  } // Loop over samples


  vector<vector<float> > entries;
  vector<vector<float> > weights;
  vector<float> powers;
  vector<TString> cuts;

  // TString baseline("(nmus+nels)==1&&ht>500&&met>200&&njets>=7&&nbm>=2");
  // TString bincut("met>400"), totcut, lumi("10");
  // powers.push_back(-1);  cuts.push_back("mt<=140&&mj<=400"); // R1
  // powers.push_back(1);   cuts.push_back("mt>140&&mj<=400");  // R3
  
  
  // for(unsigned obs(0); obs < powers.size(); obs++) {
  //   entries.push_back(vector<float>());
  //   weights.push_back(vector<float>());
  //   float yield_singlet(0);
  //   for(unsigned sam(0); sam < ra4_sam.size(); sam++) {
  //     totcut = (lumi+"*weight*("+baseline+"&&"+bincut+"&&"+cuts[obs]+
  // 		"&&"+Samples[ra4_sam[sam]].cut+")");
  //     //cout << totcut<<endl;
  //     double yield(0.), sigma(0.), avWeight(1.);
  //     int Nentries(0);
  //     Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
  //     // Zero-ing out the single t, not adding its uncertainty
  //     if(Samples[ra4_sam[sam]].label=="Single t"){
  // 	if(yield>0) yield_singlet = yield;
  // 	continue;
  //     }
  //     if(Samples[ra4_sam[sam]].label=="Other") yield += yield_singlet;
  //     if(yield<=0) entries[obs].push_back(0);
  //     //else entries[obs].push_back(yield*yield/pow(sigma,2));	    
  //     else entries[obs].push_back(yield);	    
  //     if(Nentries==0){ // If no entries, find averate weight in signal bin
  // 	totcut = (lumi+"*weight*("+baseline+"&&"+cuts[obs]+")");
  // 	Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
  // 	// If no entries, find averate weight in baseline region
  // 	if(Nentries==0){
  // 	  totcut = (lumi+"*weight*("+baseline+")");
  // 	  Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
  // 	}
  //     }
  //     avWeight = sigma*sigma/yield;
  //     avWeight = 1;
  //     weights[obs].push_back(avWeight);
  //     //cout<<obs<<","<<sam<<": entries "<<entries[obs][sam]<<", weight "<<avWeight<<", yield "<<yield<<endl;
  //   } // Loop over samples
  // } // Loop over number of observables going into kappa
   

  entries.push_back(vector<float>());
  entries[0].push_back(4);
  entries.push_back(vector<float>());
  entries[1].push_back(5);
  entries.push_back(vector<float>());
  entries[2].push_back(0);

  weights.push_back(vector<float>());
  weights[0].push_back(1.);
  weights.push_back(vector<float>());
  weights[1].push_back(1.);
  weights.push_back(vector<float>());
  weights[2].push_back(1.);
  weights.push_back(vector<float>());
  weights[3].push_back(1.);

  powers.push_back(-1.);
  powers.push_back(1.);
   powers.push_back(1.);
  // powers.push_back(-1.);


  float mSigma, pSigma;
  calcKappa(entries, weights, powers, mSigma, pSigma, true, true, true);
  time(&endtime);
  cout<<endl<<"Calculation took "<<difftime(endtime, begtime)<<" seconds"<<endl<<endl; 
}

