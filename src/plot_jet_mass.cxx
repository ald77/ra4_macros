// plot_jet_mass: Plots average jet mass versus pT
// and jet mass distributions for representative bins

#include <iostream>
#include <vector>

#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "TFile.h"
#include "TH2.h"
#include "TGraphErrors.h"
#include "TProfile.h"

#include "styles.hpp"
#include "utilities_macros.hpp"

using namespace std;

TGraphErrors * getMassPt(TH2F* h);

int main(){ 

  // TString folder="/net/cms2/cms2r0/ald77/archive/2015_07_22/skim/";
  // TString folder_noskim="/net/cms2/cms2r0/ald77/archive/2015_07_22/";
  TString folder="/mnt/hadoop/local/cawest/ald77/archive/2015_07_22/skim/";
  TString folder_noskim="/mnt/hadoop/local/cawest/ald77/archive/2015_07_22/";
  vector<TString> s_data_ns;
  s_data_ns.push_back(folder_noskim+"*JetHT*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder_noskim+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder_noskim+"*T1tttt*1200_*PU20*");
  vector<TString> s_tt;
  s_tt.push_back(folder_noskim+"*_TTJet*");
  vector<TString> s_tt_noskim;
  s_tt_noskim.push_back(folder_noskim+"*_TTJet*12.root");
  vector<TString> s_wjets;
  s_wjets.push_back(folder_noskim+"*WJetsToLNu_HT*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder_noskim+"*_T*channel*");
  vector<TString> s_qcd;
  s_qcd.push_back(folder_noskim+"*QCD_Pt*");
  //  s_qcd.push_back("/net/cms5/cms5r0/ald77/archive/2015_06_05/*QCD_Pt*");
  vector<TString> s_other;
  s_other.push_back(folder_noskim+"*TTW*");
  s_other.push_back(folder_noskim+"*TTZ*");
  s_other.push_back(folder_noskim+"*_ZJet*");
  s_other.push_back(folder_noskim+"*DY*");
  s_other.push_back(folder_noskim+"*WH_HToBB*");

  // Reading ntuples
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_data_ns, "Data", 1, -1, "trig[12] && json_golden")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_qcd, "QCD", dps::c_qcd));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", dps::c_tt_1l, 1,
			   "ntruleps<=1"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", dps::c_tt_2l,1,
			   "ntruleps>=2"));
  Samples.push_back(sfeats(s_wjets, "W+jets", dps::c_wjets));
  Samples.push_back(sfeats(s_singlet, "Single t", dps::c_singlet));
  Samples.push_back(sfeats(s_other, "Other", dps::c_other));

  vector<int> ra4_sam;
  ra4_sam.push_back(0);
  ra4_sam.push_back(1);
  ra4_sam.push_back(2);
  ra4_sam.push_back(3);
  ra4_sam.push_back(4);
  ra4_sam.push_back(5);
  ra4_sam.push_back(6);

  vector<hfeats> vars;

  vars.push_back(hfeats("jets_m",25,0,250, ra4_sam, "AK4 jet mass (GeV)",
   			"ht>1000&&met<50&&(nmus+nels)==0",0));
  vars.push_back(hfeats("jets_pt",20,0,1000, ra4_sam, "jet p_{T} (GeV)",
   			"ht>1000&&met<50&&(nmus+nels)==0",0));

  // mass in a couple typical bins of pT
  vars.push_back(hfeats("jets_m",50,0,100, ra4_sam, "AK4 jet mass (GeV)",
    			"ht>1000&&met<50&&(nmus+nels)==0&&jets_pt>50&&jets_pt<75",0));
  vars.push_back(hfeats("jets_m",50,0,100, ra4_sam, "AK4 jet mass (GeV)",
    			"ht>1000&&met<50&&(nmus+nels)==0&&jets_pt>250",0));

  plot_distributions(Samples, vars, "0.0073", ".pdf", "CMSPaper", "1d", true);

  TH2F *jetmass_vs_pt_data = new TH2F("jetmass_vs_pt_data", "jetmass_vs_pt_data", 100, 0, 1000, 100, 0, 100);
  TH2F *jetmass_vs_pt_MC = new TH2F("jetmass_vs_pt_MC", "jetmass_vs_pt_MC", 100, 0, 1000, 100, 0, 100);

  TChain *filesData = new TChain("tree");
  filesData->Add(s_data_ns.at(0));

  TChain *filesMC = new TChain("tree");
  filesMC->Add(s_tt.at(0));
  filesMC->Add(s_wjets.at(0));
  filesMC->Add(s_singlet.at(0));
  filesMC->Add(s_qcd.at(0));
  for(unsigned int i=0; i<s_other.size(); i++) filesMC->Add(s_other.at(i));

  // make 2-d histograms for projections
  filesData->Project("jetmass_vs_pt_data", "jets_m:jets_pt", "weight*(trig[12] && json_golden && ht>1000&&met<50&&(nmus+nels)==0)");
  filesMC->Project("jetmass_vs_pt_MC", "jets_m:jets_pt", "weight*(ht>1000&&met<50&&(nmus+nels)==0)");

  styles style("LargeLabels"); style.setDefaultStyle();

  TGraphErrors *data = getMassPt(jetmass_vs_pt_data);
  TGraphErrors *MC = getMassPt(jetmass_vs_pt_MC);

  TCanvas *c = new TCanvas;
  MC->SetMinimum(0);
  MC->SetMarkerStyle(kFullCircle);
  MC->SetMarkerColor(kRed);
  MC->Draw("AP");
  MC->SetTitle(";AK4 jet p_{T} (GeV);mean AK4 jet mass (GeV)"); 
  data->SetMarkerStyle(kOpenSquare);
  data->SetMarkerColor(kBlack);
  data->Draw("P");

  TLegend *leg = new TLegend(0.6, 0.3, 0.8, 0.5);
  leg->AddEntry(data, "Data", "P");
  leg->AddEntry(MC, "MC", "P");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->Draw();

  c->SaveAs("plots/1d/mass_versus_pt.pdf");


}

// returns a TGraphErrors with mean mass per pT bin,
// given a 2D histogram of jet mass versus jet pT
TGraphErrors * getMassPt(TH2F* h)
{
  int binWidth=10;
  double zero[29];
  double ptBins[29];
  double mass[29];
  double massError[29];
  int ptBinEdges[30] = {50, 60, 70, 80, 90,
                        100, 110, 120, 130, 140,
                        150, 160, 170, 180, 190,
			200, 210, 220, 230, 240,
			250, 300, 350, 400, 450,
			500, 550, 600, 650, 700};


  for(int i=0; i<29; i++) {
    zero[i] = 0.0;
    int binNumberLow=ptBinEdges[i]/binWidth+1;
    int binNumberHigh=ptBinEdges[i+1]/binWidth+1;
    double tmpMass=h->ProfileY("_pfx", binNumberLow, binNumberHigh)->GetMean();
    double tmpMassError=h->ProfileY("_pfx_error", binNumberLow, binNumberHigh)->GetMeanError();
    std::cout << tmpMass
              << " +/- " 
              << tmpMassError << std::endl;
    mass[i]=tmpMass;
    massError[i]=tmpMassError;
    ptBins[i]=(ptBinEdges[i+1]+ptBinEdges[i])/2.0;
  }
  
  TGraphErrors *gr = new TGraphErrors(29, ptBins, mass, zero, massError);
  return gr;
}
