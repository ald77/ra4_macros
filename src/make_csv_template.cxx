// make_csv_template.cxx: Makes templates of CSV template for jets passing
// the CSV requirement. Used for reweighting flavor composition of QCD MC

#include <iostream>
#include <vector>
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TColor.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"
#include "utilities_macros_rpv.hpp"

namespace {
  const int nBins=20;
  const float xMin=0;
  const float xMax=1.0;
  const TString csvVar("(jets_csv-0.89)/0.11");
}

void makeCSVHist(TFile *file, const std::vector<TString>& samples, const TString& name, const TString& extracut);

int main(){ 

  std::vector<TString> s_qcd;
  s_qcd.push_back(filestring("QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_qcd.push_back(filestring("QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_qcd.push_back(filestring("QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  // The CSV reweighting only affects QCD; the flavor composition of other samples
  // well defined and does not need correction
  std::vector<TString> s_other;
  // ttbar
  //  s_other.push_back(filestring("TTJets_TuneCUETP8M1_13TeV-madgraphMLM"));
  s_other.push_back(filestring("TT_TuneCUETP8M1_13TeV-powheg-pythia8"));
  // W/Z
  s_other.push_back(filestring("WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_other.push_back(filestring("WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_other.push_back(filestring("ZJetsToQQ_HT600toInf_13TeV-madgraph"));
  s_other.push_back(filestring("DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  // single top
  s_other.push_back(filestring("ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  // ttbar(W,Z,H,ttbar)
  s_other.push_back(filestring("TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"));
  s_other.push_back(filestring("TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"));
  s_other.push_back(filestring("TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
  s_other.push_back(filestring("TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
  s_other.push_back(filestring("ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8"));
  s_other.push_back(filestring("TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
  std::vector<TString> s_jetht;
  s_jetht.push_back(filestring("JetHT_Run2015C_25ns-05Oct2015-v1"));
  s_jetht.push_back(filestring("JetHT_Run2015D-05Oct2015-v1"));
  s_jetht.push_back(filestring("JetHT_Run2015D-PromptReco-v4"));

  // Reading ntuples
  std::vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_jetht, "Data",kBlack,1,"(trig[11] || trig[12]) && pass && njets<10")); 
  Samples.back().isData = true;
  Samples.back().doStack = false;
  
  // Adding non-skimmed samples
  std::vector<int> rpv_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    rpv_sam.push_back(sam);
    std::vector<TString> sam_files = Samples[sam].file;
    Samples.push_back(sfeats(sam_files, Samples[sam].label, Samples[sam].color, Samples[sam].style,
			     Samples[sam].cut));
  } // Loop over samples

  std::vector<hfeats> vars;

  TString cuts("(nmus+nels)==1");
  cuts = "(nmus+nels)==0&&ht>1500&&nbm>=2&&njets>=6&&njets<10 && mj>800";
  vars.push_back(hfeats(csvVar, nBins, xMin, xMax, rpv_sam, "(CSV-CSV_{cut})/(CSV_{max}-CSV_{cut})", cuts));
  vars.back().normalize = true;

  // the old method produced per-jet weights rather than per-event weights
  bool useOldMethod=false;
  std::string outFile("csv_newmethod.root");
  if(useOldMethod) outFile="csv_oldmethod.root";

  TFile *out = new TFile(outFile.c_str(), "recreate");

  if(useOldMethod) {
    makeCSVHist(out, s_jetht, "data_low_njet", "njets<10");
    makeCSVHist(out, s_qcd, "QCD_b_low_njet", "jets_hflavor==5 && njets<10");
    makeCSVHist(out, s_qcd, "QCD_c_low_njet", "jets_hflavor==4 && njets<10");
    makeCSVHist(out, s_qcd, "QCD_l_low_njet", "jets_hflavor==0 && njets<10");
    makeCSVHist(out, s_other, "other_low_njet", "njets<10");
    
    makeCSVHist(out, s_jetht, "data_high_njet", "njets>=10"); 
    makeCSVHist(out, s_qcd, "QCD_b_high_njet", "jets_hflavor==5 && njets>=10");
    makeCSVHist(out, s_qcd, "QCD_c_high_njet", "jets_hflavor==4 && njets>=10");
    makeCSVHist(out, s_qcd, "QCD_l_high_njet", "jets_hflavor==0 && njets>=10");
    makeCSVHist(out, s_other, "other_high_njet", "njets>=10");
    
    makeCSVHist(out, s_jetht, "data_low_njet_low_mj", "njets<10 && mj>500 && mj<800");
    makeCSVHist(out, s_qcd, "QCD_b_low_njet_low_mj", "jets_hflavor==5 && njets<10 && mj>500 && mj<800");
    makeCSVHist(out, s_qcd, "QCD_c_low_njet_low_mj", "jets_hflavor==4 && njets<10 && mj>500 && mj<800");
    makeCSVHist(out, s_qcd, "QCD_l_low_njet_low_mj", "jets_hflavor==0 && njets<10 && mj>500 && mj<800");
    makeCSVHist(out, s_other, "other_low_njet_low_mj", "njets<10 && mj>500 && mj<800");
    
    makeCSVHist(out, s_jetht, "data_low_njet_high_mj", "njets<10 && mj>500 && mj>=800");
    makeCSVHist(out, s_qcd, "QCD_b_low_njet_high_mj", "jets_hflavor==5 && njets<10 && mj>=800");
    makeCSVHist(out, s_qcd, "QCD_c_low_njet_high_mj", "jets_hflavor==4 && njets<10 && mj>=800");
    makeCSVHist(out, s_qcd, "QCD_l_low_njet_high_mj", "jets_hflavor==0 && njets<10 && mj>=800");
    makeCSVHist(out, s_other, "other_low_njet_high_mj", "njets<10 && mj>=800");
  }
  else {
    makeCSVHist(out, s_jetht, "data_low_njet", "njets<8");
    makeCSVHist(out, s_qcd, "QCD_b_low_njet", "Sum$(abs(jets_hflavor)==5)>0 && njets<8");
    makeCSVHist(out, s_qcd, "QCD_c_low_njet", "Sum$(abs(jets_hflavor)==5)==0 && Sum$(abs(jets_hflavor)==4)>0  && njets<8");
    makeCSVHist(out, s_qcd, "QCD_l_low_njet", "Sum$(abs(jets_hflavor)==5)==0 && Sum$(abs(jets_hflavor)==4)==0 && njets<8");
    makeCSVHist(out, s_other, "other_low_njet", "njets<8");
    
    makeCSVHist(out, s_jetht, "data_high_njet", "njets>=8"); 
    makeCSVHist(out, s_qcd, "QCD_b_high_njet", "Sum$(abs(jets_hflavor)==5)>0 && njets>=8");
    makeCSVHist(out, s_qcd, "QCD_c_high_njet", "Sum$(abs(jets_hflavor)==5)==0 && Sum$(abs(jets_hflavor)==4)>0 && njets>=8");
    makeCSVHist(out, s_qcd, "QCD_l_high_njet", "Sum$(abs(jets_hflavor)==5)==0 && Sum$(abs(jets_hflavor)==4)==0 && njets>=8");
    makeCSVHist(out, s_other, "other_high_njet", "njets>=8");
    
    makeCSVHist(out, s_jetht, "data_low_njet_low_mj", "njets<8 && mj>500 && mj<800");
    makeCSVHist(out, s_qcd, "QCD_b_low_njet_low_mj", "Sum$(abs(jets_hflavor)==5)>0 && njets<8 && mj>500 && mj<800");
    makeCSVHist(out, s_qcd, "QCD_c_low_njet_low_mj", "Sum$(abs(jets_hflavor)==5)==0 && Sum$(abs(jets_hflavor)==4)>0 && njets<8 && mj>500 && mj<800");
    makeCSVHist(out, s_qcd, "QCD_l_low_njet_low_mj", "Sum$(abs(jets_hflavor)==5)==0 && Sum$(abs(jets_hflavor)==4)==0 && njets<8 && mj>500 && mj<800");
    makeCSVHist(out, s_other, "other_low_njet_low_mj", "njets<8 && mj>500 && mj<800");
    
    makeCSVHist(out, s_jetht, "data_low_njet_high_mj", "njets<8 && mj>500 && mj>=800");
    makeCSVHist(out, s_qcd, "QCD_b_low_njet_high_mj", "Sum$(abs(jets_hflavor)==5)>0 && njets<8 && mj>=800");
    makeCSVHist(out, s_qcd, "QCD_c_low_njet_high_mj", "Sum$(abs(jets_hflavor)==5)==0 && Sum$(abs(jets_hflavor)==4)>0 && njets<8 && mj>=800");
    makeCSVHist(out, s_qcd, "QCD_l_low_njet_high_mj", "Sum$(abs(jets_hflavor)==5)==0 && Sum$(abs(jets_hflavor)==4)==0 && njets<8 && mj>=800");
    makeCSVHist(out, s_other, "other_low_njet_high_mj", "njets<8 && mj>=800");
  }
  out->Close();
}

void makeCSVHist(TFile *file, const std::vector<TString>& samples, const TString& name, const TString& extracut)
{

  std::cout << "Making histograms " << name << " for cut " << extracut << std::endl;

  TChain *ch = new TChain("tree");
  for(auto isample : samples) {
    ch->Add(isample);
  }

  TString cut("(nmus+nels)==0&&ht>1500&&nbm>=2&&njets>=6");
  TString weightandcut;
  if(extracut.Length()==0) {
    weightandcut=Form("%s*weight/eff_trig*(%s)", rpv::luminosity.Data(), cut.Data());
  }
  else weightandcut=Form("%s*weight/eff_trig*(%s&&%s)", rpv::luminosity.Data(), cut.Data(), extracut.Data());
  TString cutAndExtraCut(Form("(%s&&%s)", cut.Data(), extracut.Data()));

  // weighted histogram for convenient display
  TH1F *weightedHist = new TH1F(name, name, nBins, xMin, xMax);
  ch->Project(name, csvVar.Data(), weightandcut.Data());

  // raw number of MC counts
  TString rawHistName(Form("%s_raw", name.Data()));
  TH1F *rawHist = new TH1F(rawHistName, rawHistName, nBins, xMin, xMax);
  ch->Project(rawHistName, csvVar.Data(), cutAndExtraCut.Data());

  // weights need to be saved separately for proper application of Barlow-Beeston method
  TString weightHistName(Form("%s_weights", name.Data()));
  TH1F *weight = new TH1F(weightHistName, weightHistName, nBins, xMin, xMax);
  weight->Add(weightedHist);
  for(int i=1; i<=weight->GetNbinsX(); i++) {
    weight->SetBinContent(i, weightedHist->GetBinContent(i)/rawHist->GetBinContent(i));
    weight->SetBinError(i, weightedHist->GetBinError(i)/rawHist->GetBinContent(i));
  }

  file->cd();
  weightedHist->Write();
  weight->Write();
  rawHist->Write();
}
