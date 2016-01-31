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
  TString luminosity="2.25";
  TString plot_type=".pdf";
  TString plot_style="CMSPaperNoRatio";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 

  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder
  
  TString folder1l(bfolder+"/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/skim_1lht500met200/");
  TString foldermc(bfolder+"/cms2r0/babymaker/babies/2015_11_28/mc/skim_1lht500met200/");

  vector<TString> s_slep;
  s_slep.push_back(folder1l+"*root"); // With "__Single" you exclude the extra 48 ipb


  vector<TString> s_t1t;
  s_t1t.push_back(foldermc+"*T1tttt*1500_*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(foldermc+"*T1tttt*1200_*");
  vector<TString> s_tt;
  s_tt.push_back(foldermc+"*_TTJets*Lept*");
  s_tt.push_back(foldermc+"*_TTJets_HT*");
  vector<TString> s_wjets;
  s_wjets.push_back(foldermc+"*_WJetsToLNu*");
  vector<TString> s_ttv;
  s_ttv.push_back(foldermc+"*_TTWJets*");
  s_ttv.push_back(foldermc+"*_TTZTo*");
  vector<TString> s_single;
  s_single.push_back(foldermc+"*_ST_*");
  vector<TString> s_other;
  s_other.push_back(foldermc+"*DYJetsToLL*");
  s_other.push_back(foldermc+"*_QCD_HT*");
  s_other.push_back(foldermc+"*_ZJet*");
  s_other.push_back(foldermc+"*_WWTo*");
  s_other.push_back(foldermc+"*ggZH_HToBB*");
  s_other.push_back(foldermc+"*ttHJetTobb*");


  // Reading ntuples
  int sigcolor(2);
  TString lsp = "#tilde{#chi}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  TString t1t_label = "#scale[0.92]{#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#bar{t}"+lsp;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_slep, "Data", kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_t1t, t1t_label+" (1500,100)}", sigcolor, 1));
  Samples.push_back(sfeats(s_t1tc, t1t_label+" (1200,800)}", sigcolor, 2));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1&&stitch"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  Samples.push_back(sfeats(s_wjets, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_single, "Single top", dps::c_singlet));
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_other, "Other", 2001, 1)); 

  vector<int> ra4_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
  } // Loop over samples


  int imj(Samples.size());
  Samples.push_back(sfeats(s_slep, "Data, m_{T} > 140 GeV", kBlack,1,"(trig[4]||trig[8])&&pass&&mt>140")); Samples.back().isData=true;
  Samples.push_back(sfeats(s_slep, "Data, m_{T} #leq 140 GeV", kBlue+2,1,"(trig[4]||trig[8])&&pass&&mt<=140")); 
  Samples.back().doBand = true;
  Samples.push_back(sfeats(s_t1t, t1t_label+" (1500,100)}", sigcolor, 1, "mt>140"));
  Samples.push_back(sfeats(s_t1tc, t1t_label+" (1200,800)}", sigcolor, 2, "mt>140"));
  vector<int> mj_sam;
  mj_sam.push_back(imj);
  mj_sam.push_back(imj+1);
  mj_sam.push_back(imj+2);
  mj_sam.push_back(imj+3);


  vector<hfeats> vars;

  vars.push_back(hfeats("mt",18,0,630, ra4_sam, "m_{T} [GeV]",
  			"nleps==1&&ht>500&&met>200&&pass&&njets>=6&&nbm>=1",140, "baseline"));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",10,25,775, mj_sam, "M_{J} [GeV]",
			"nleps==1&&ht>500&&met>200&&pass&&njets>=6&&nbm==1",400,"results"));
  vars.back().whichPlots = "2"; vars.back().normalize = true;
  vars.push_back(hfeats("mj",10,25,775, mj_sam, "M_{J} [GeV]",
			"nleps==1&&ht>500&&met>200&&pass&&njets>=6&&nbm>=2",400,"results"));
  vars.back().whichPlots = "2"; vars.back().normalize = true;


  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "paper",false);

}
