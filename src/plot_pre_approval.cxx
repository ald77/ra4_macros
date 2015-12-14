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
  TString luminosity="2.1";
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 

  TString folder1l("/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/skim_1lht500met200/");
  TString foldermc("/cms2r0/babymaker/babies/2015_11_28/mc/skim_1lht500met200/");

  vector<TString> s_slep;
  s_slep.push_back(folder1l+"*root");


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
  Samples.push_back(sfeats(s_other, "Other", 1001, 1)); 

  vector<int> ra4_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
  } // Loop over samples

  int imt(Samples.size());
  Samples.push_back(sfeats(s_slep, "Data, M_{J} > 400", kBlack,1,"(trig[4]||trig[8])&&pass&&mj>400")); 
  Samples.back().isData = true;
  Samples.push_back(sfeats(s_slep, "Data, M_{J} #leq 400", kBlue+2,1,"(trig[4]||trig[8])&&pass&&mj>250&&mj<=400")); 
  Samples.back().doBand = true;
  vector<int> mt_sam;
  mt_sam.push_back(imt);
  mt_sam.push_back(imt+1);

  int idilep(Samples.size());
  Samples.push_back(sfeats(s_slep, "Data 2l, N_{jets} #geq 5", kBlack,1,"(trig[4]||trig[8])&&pass&&njets>=5&&nleps==2&&nbm<=2")); 
  Samples.back().isData=true;
  Samples.push_back(sfeats(s_slep, "Data 1l, N_{jets} #geq 6, m_{T} #leq 140, N_{b} #geq 1", kBlue+2,1,
			   "(trig[4]||trig[8])&&pass&&mt<=140&&njets>=6&&nleps==1&&nbm>=1")); Samples.back().doBand = true;
  vector<int> dilep_sam;
  dilep_sam.push_back(idilep);
  dilep_sam.push_back(idilep+1);

  int imj(Samples.size());
  Samples.push_back(sfeats(s_slep, "Data, m_{T} > 140", kBlack,1,"(trig[4]||trig[8])&&pass&&mt>140")); Samples.back().isData=true;
  Samples.push_back(sfeats(s_slep, "Data, m_{T} #leq 140", kBlue+2,1,"(trig[4]||trig[8])&&pass&&mt<=140")); 
  Samples.back().doBand = true;
  vector<int> mj_sam;
  mj_sam.push_back(imj);
  mj_sam.push_back(imj+1);


  vector<hfeats> vars;

  //// mT and MJ after baseline
  vars.push_back(hfeats("mj",12,0,1200, ra4_sam, "M_{J} [GeV]","nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=1&&mt>140",400, "baseline"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("mt",18,0,630, ra4_sam, "m_{T} [GeV]","nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=1",140, "baseline"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  //// Data vs Data
  vars.push_back(hfeats("mj",6,250,700, dilep_sam, "M_{J} [GeV]","ht>500&&met>200&&met<=400",-1,"mj250_1b"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",12,0,1200, mj_sam, "M_{J} [GeV]","nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=2"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("mt",18,0,630, mt_sam, "m_{T} [GeV]","nleps==1&&ht>500&&met>200&&mj>250&&njets>=6&&nbm>=2"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("mj",12,0,1200, mj_sam, "M_{J} [GeV]","nleps==1&&ht>500&&met>200&&njets>=6&&nbm==1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("mt",18,0,630, mt_sam, "m_{T} [GeV]","nleps==1&&ht>500&&met>200&&mj>250&&njets>=6&&nbm==1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  //// 1b plots
  vars.push_back(hfeats("mj",12,0,1200, ra4_sam, "M_{J} [GeV]","mt>140&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm==1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("mj",12,0,1200, ra4_sam, "M_{J} [GeV]","mt<=140&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm==1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mt",18,0,630, ra4_sam, "m_{T} [GeV]","mj>250&&mj<400&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm==1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("mt",18,0,630, ra4_sam, "m_{T} [GeV]","mj>400&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm==1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  //// 2b plots
  vars.push_back(hfeats("mj",12,0,1200, ra4_sam, "M_{J} [GeV]","mt>140&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=2"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("mj",12,0,1200, ra4_sam, "M_{J} [GeV]","mt<=140&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=2"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mt",18,0,630, ra4_sam, "m_{T} [GeV]","mj>250&&mj<400&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=2"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("mt",18,0,630, ra4_sam, "m_{T} [GeV]","mj>400&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=2"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;


  //// 1+b plots
  vars.push_back(hfeats("met",12,200,800, ra4_sam, "MET [GeV]","mt>140&&mj>250&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("met",12,200,800, ra4_sam, "MET [GeV]","mt<=140&&mj>250&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("njets",13, 0.5,13.5, ra4_sam, "n_{jets}","mt>140&&mj>250&&nleps==1&&ht>500&&met>200&&nbm>=1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;
  vars.push_back(hfeats("njets",13, 0.5,13.5, ra4_sam, "n_{jets}","mt<=140&&mj>250&&nleps==1&&ht>500&&met>200&&nbm>=1"));
  vars.back().whichPlots = "12"; vars.back().normalize = true;


  // // Not in PAS
  // //// Data vs Data
  // vars.push_back(hfeats("mj",14,0,700, dilep_sam, "M_{J} [GeV]","ht>500&&met>200&&met<=400",-1,"full_1b"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  // vars.push_back(hfeats("mj",12,0,1200, mj_sam, "M_{J} [GeV]","nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=1"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  // vars.push_back(hfeats("mt",18,0,630, mt_sam, "m_{T} [GeV]","nleps==1&&ht>500&&met>200&&mj>250&&njets>=6&&nbm>=1"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;

  // // Not in PAS
  // //// njets, nbm N-1
  // vars.push_back(hfeats("njets",13, 0.5,13.5, ra4_sam, "n_{jets}","nleps==1&&ht>500&&met>200&&nbm>=1"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  // vars.push_back(hfeats("nbm",6, -0.5,5.5, ra4_sam, "n_{b}","nleps==1&&ht>500&&met>200&&njets>=6"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;

  // // // Not in PAS
  // //// 1b plots
  // vars.push_back(hfeats("met",12,200,800, ra4_sam, "MET [GeV]","mt>140&&mj>250&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm==1"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  // vars.push_back(hfeats("met",12,200,800, ra4_sam, "MET [GeV]","mt<=140&&mj>250&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm==1"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;

  // vars.push_back(hfeats("njets",13, 0.5,13.5, ra4_sam, "n_{jets}","mt>140&&mj>250&&nleps==1&&ht>500&&met>200&&nbm==1"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  // vars.push_back(hfeats("njets",13, 0.5,13.5, ra4_sam, "n_{jets}","mt<=140&&mj>250&&nleps==1&&ht>500&&met>200&&nbm==1"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;

  // //// 2b plots
  // vars.push_back(hfeats("met",12,200,800, ra4_sam, "MET [GeV]","mt>140&&mj>250&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=2"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  // vars.push_back(hfeats("met",12,200,800, ra4_sam, "MET [GeV]","mt<=140&&mj>250&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=2"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  // vars.push_back(hfeats("njets",13, 0.5,13.5, ra4_sam, "n_{jets}","mt>140&&mj>250&&nleps==1&&ht>500&&met>200&&nbm>=2"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  // vars.push_back(hfeats("njets",13, 0.5,13.5, ra4_sam, "n_{jets}","mt<=140&&mj>250&&nleps==1&&ht>500&&met>200&&nbm>=2"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;


  // //// njets, nbm N-1
  // vars.push_back(hfeats("njets",13, 0.5,13.5, ra4_sam, "n_{jets}","mt>140&&mj>250&&nleps==1&&ht>500&&met>200&&nbm>=1"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  // vars.push_back(hfeats("njets",13, 0.5,13.5, ra4_sam, "n_{jets}","mt<=140&&mj>250&&nleps==1&&ht>500&&met>200&&nbm>=1"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  // vars.push_back(hfeats("fjets_m[0]",20,0,400, ra4_sam, "m(J_{1}) [GeV]","mt>140&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=1"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;
  // vars.push_back(hfeats("fjets_m[0]",20,0,400, ra4_sam, "m(J_{1}) [GeV]","mt<=140&&nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=1"));
  // vars.back().whichPlots = "12"; vars.back().normalize = true;

  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "pas",true);

}
