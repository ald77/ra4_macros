// plot_trigger: Macro that plots variables both lumi weighted and normalized to the same area.

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  TString folders[] ={"/net/cms2/cms2r0/manuelf/root/small/hlt/15-04-01/el15/",
		      "/net/cms2/cms2r0/manuelf/root/small/hlt/15-04-01/mu15/"};
  vector<TString> s_t1t[2], s_t1tc[2], s_tt[2], s_qcd[2], s_wjets[2];
  vector<sfeats> Samples; 
  unsigned Nsam(0);
  for(int lep(0); lep<2; lep++){
    s_t1t[lep].push_back(folders[lep]+"*T1tttt*1500_*PU20*");
    s_t1tc[lep].push_back(folders[lep]+"*T1tttt*1200_*PU20*");
    s_tt[lep].push_back(folders[lep]+"*TT_Tune*");
    s_wjets[lep].push_back(folders[lep]+"*WTo*");
    s_qcd[lep].push_back(folders[lep]+"*QCD_Pt*");

    Samples.push_back(sfeats(s_t1t[lep], "T1tttt(1500,100)", ra4::c_t1tttt));
    Samples.push_back(sfeats(s_t1tc[lep], "T1tttt(1200,800)", ra4::c_t1tttt,2));
    Samples.push_back(sfeats(s_tt[lep], "t#bar{t}", ra4::c_tt_1l));
    //Samples.push_back(sfeats(s_wjets[lep], "W+jets", ra4::c_singlet));
    Samples.push_back(sfeats(s_qcd[lep], "QCD", 1));
    if(lep==0) Nsam = Samples.size();
  }

  vector<int> all_sam[2], tt_t1[2];
  for(unsigned lep(0); lep<2; lep++){
    for(unsigned iall(0); iall<Nsam; iall++) all_sam[lep].push_back(iall+lep*Nsam);
    tt_t1[lep].push_back(0+lep*Nsam);
    tt_t1[lep].push_back(1+lep*Nsam);
    tt_t1[lep].push_back(2+lep*Nsam);
  }

  TString leptag[] = {"electron", "muon"};
  vector<hfeats> vars;
  vars.push_back(hfeats("Max$(mus_pt)",60,0,300, tt_t1[1], "Leading #mu HLT p_{T} [GeV]",
			"onht>350&&Sum$(genmus_pt>15)>=1",20,"muon"));
  vars.push_back(hfeats("Max$(els_pt)",60,0,300, tt_t1[0], "Leading e HLT p_{T} [GeV]",
			"onht>350&&Sum$(genels_pt>15)>=1",20,"electron"));
  for(unsigned lep(0); lep<2; lep++){
    vars.push_back(hfeats("onht",75,0,2500, all_sam[lep], "HLT H_{T} [GeV]",
			  "1",350,leptag[lep]));
    vars.push_back(hfeats("onmet",65,0,650, all_sam[lep], "HLT MET [GeV]",
			  "1",-1,leptag[lep]));
  }

  plot_distributions(Samples, vars, "10", ".eps", "RA4");

  

}

