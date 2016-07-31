// plot_dps: Macro that plots variables both for comissioning DPS

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;

void makePlot(vector<sfeats> Samples, vector<TChain *> chain, TString cuts);

int main(){ 

  styles style("RA4"); style.setDefaultStyle();

  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder
  
  TString foldermc(bfolder+"/cms2r0/babymaker/babies/2016_06_14/mc/unskimmed/");
  TString folderscan(bfolder+"/cms2r0/babymaker/babies/2016_06_21/mc/T1tttt/");

  vector<TString> s_t1t;
  s_t1t.push_back(folderscan+"*T1tttt*1400_mLSP-1*");
  s_t1t.push_back(folderscan+"*T1tttt*1450_mLSP-1*");
  s_t1t.push_back(folderscan+"*T1tttt*1500_mLSP-1*");
  s_t1t.push_back(folderscan+"*T1tttt*1550_mLSP-1*");
  s_t1t.push_back(folderscan+"*T1tttt*1600_mLSP-1*");
  s_t1t.push_back(folderscan+"*T1tttt*1650_mLSP-1*");
  s_t1t.push_back(folderscan+"*T1tttt*1700_mLSP-1*");
  s_t1t.push_back(folderscan+"*T1tttt*1750_mLSP-1*");
  s_t1t.push_back(folderscan+"*T1tttt*1800_mLSP-1*");
  s_t1t.push_back(folderscan+"*T1tttt*1850_mLSP-1*");
  s_t1t.push_back(folderscan+"*T1tttt*1900_mLSP-1*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folderscan+"*T1tttt*1200_mLSP-800*");
  s_t1tc.push_back(folderscan+"*T1tttt*1250_mLSP-850*");
  s_t1tc.push_back(folderscan+"*T1tttt*1300_mLSP-900*");
  s_t1tc.push_back(folderscan+"*T1tttt*1350_mLSP-950*");
  s_t1tc.push_back(folderscan+"*T1tttt*1400_mLSP-1000*");
  s_t1tc.push_back(folderscan+"*T1tttt*1450_mLSP-1050*");
  s_t1tc.push_back(folderscan+"*T1tttt*1500_mLSP-1100*");
  s_t1tc.push_back(folderscan+"*T1tttt*1550_mLSP-1150*");
  s_t1tc.push_back(folderscan+"*T1tttt*1600_mLSP-1200*");
  s_t1tc.push_back(folderscan+"*T1tttt*1650_mLSP-1250*");
  s_t1tc.push_back(folderscan+"*T1tttt*1700_mLSP-1300*");
  vector<TString> s_tt_1l;
  s_tt_1l.push_back(foldermc+"*_TTJets*SingleLeptFromTbar*");
  s_tt_1l.push_back(foldermc+"*_TTJets*SingleLeptFromT_*");
  vector<TString> s_tt_2l;
  s_tt_2l.push_back(foldermc+"*_TTJets*DiLept*");

  vector<TString> s_other;
  s_other.push_back(foldermc+"*_WJetsToLNu*.root");
  s_other.push_back(foldermc+"*_TTWJets*.root");
  s_other.push_back(foldermc+"*_TTZTo*.root");
  s_other.push_back(foldermc+"*_ST_*.root");
  s_other.push_back(foldermc+"*QCD_HT*.root");
  s_other.push_back(foldermc+"*DYJetsToLL*.root");
  s_other.push_back(foldermc+"*_ZJet*.root");
  s_other.push_back(foldermc+"*ggZH_HToBB*.root");
  s_other.push_back(foldermc+"*ttHJetTobb*.root");
  s_other.push_back(foldermc+"*_TTGJets*.root");
  s_other.push_back(foldermc+"*_TTTT*.root");
  s_other.push_back(foldermc+"*_WH_HToBB*.root");
  s_other.push_back(foldermc+"*_ZH_HToBB*.root");
  s_other.push_back(foldermc+"*_WWTo*.root");
  s_other.push_back(foldermc+"*_WZTo*.root");
  s_other.push_back(foldermc+"*_ZZ_*.root");

  // Reading ntuples
  vector<sfeats> Samples; 
  //  Samples.push_back(sfeats(s_t1t, "T1tttt(1400-1900,1)", 2));
  //  Samples.push_back(sfeats(s_t1tc, "T1tttt(1200-1700,800-1300)", 42));
  Samples.push_back(sfeats(s_tt_1l, "t#bar{t}, 1 true lepton", dps::c_tt_1l));
  Samples.push_back(sfeats(s_tt_2l, "t#bar{t}, 2 true leptons", dps::c_wjets));
  //  Samples.push_back(sfeats(s_other, "Other", 2001));

  // Reading ntuples
  vector<TChain *> chain;
  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++){
      chain[sam]->Add(Samples[sam].file[insam]);
    }   
  }

  vector<TString> cuts = {"nleps==1", "ht>500", "met>200", "njets>=6", "nbm>=2", "mj14>=400","met>400","mt>140",
  			  "nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=1",
			  "nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=1&&mj14>=400",
			  "nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=1&&mj14>=400&&mt>140"};

  for(size_t ind(0); ind<cuts.size(); ind++)
    makePlot(Samples, chain, cuts[ind]);
}

void makePlot(vector<sfeats> Samples, vector<TChain *> chain, TString cuts){
  // Finding histograms
  TCanvas can;
  double legSingle = 0.08;
  double legX=0.22, legY=0.18, legW=0.1, legH=legSingle*Samples.size();
  TLegend leg(legX,legY, legX+legW, legY+legH);
  leg.SetTextSize(0.05); leg.SetFillColor(0); leg.SetBorderSize(0); leg.SetFillStyle(0);
  float minh = 0, maxh = 50;
  int nbins = (maxh-minh)/5;
  TString hname;
  vector<TH1D*> histo[2];
  for(unsigned sam(0); sam < Samples.size(); sam++){
    for(size_t ind(0); ind<2; ind++){
      hname = "histo"; hname += sam;  hname += ind;
      histo[ind].push_back(new TH1D(hname, cuts2title(cuts), nbins, minh, maxh));
      //histo[ind][sam]->Sumw2();
      histo[ind][sam]->SetBinErrorOption(TH1::kPoisson);
      histo[ind][sam]->SetLineColor(Samples[sam].color);
      histo[ind][sam]->SetLineWidth(3);
      histo[ind][sam]->SetXTitle("True n_{PV}");
      histo[ind][sam]->SetYTitle("#epsilon  normalized to [15 #leq n_{PV} < 20]");
      histo[ind][sam]->SetBinContent(nbins, histo[ind][sam]->GetBinContent(nbins)+
				     histo[ind][sam]->GetBinContent(nbins+1));
    }
    chain[sam]->Project(histo[0][sam]->GetName(), "ntrupv", "weight*("+cuts+")");
    chain[sam]->Project(histo[1][sam]->GetName(), "ntrupv", "weight");
    histo[0][sam]->Divide(histo[1][sam]);
    histo[0][sam]->Scale(1/histo[0][sam]->GetBinContent(4));
    histo[0][sam]->SetMinimum(0.55);
    histo[0][sam]->SetMaximum(1.3);
    leg.AddEntry(histo[0][sam], Samples[sam].label);
    if(sam==0){
      histo[0][sam]->Draw("");
      TLine line; line.SetLineStyle(2);
      line.DrawLine(minh, 1, maxh, 1);
    } else histo[0][sam]->Draw("same");
  } // Loop over samples
  leg.Draw();
  TString pname = "eff_pu_"+format_tag(cuts)+".pdf";
  can.SaveAs("plots/eff_pu/"+pname);
  
  for(unsigned sam(0); sam < Samples.size(); sam++)
    for(size_t ind(0); ind<2; ind++)
      histo[ind][sam]->Delete();
}
