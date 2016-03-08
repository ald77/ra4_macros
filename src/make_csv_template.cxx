// plot_note: Macro that plots variables both lumi weighted and normalized to the same area.

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
  TString luminosity="2.69";
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
  int section(1);
}

void makeCSVHist(TFile *file, const std::vector<TString>& samples, const TString& name, const TString& extracut);

int main(){ 
  std::string extraWeight("w_lumi*w_btag/weight");

  std::vector<TString> s_qcd;
  s_qcd.push_back(filestring("QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_qcd.push_back(filestring("QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_qcd.push_back(filestring("QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  // The CSV reweighting only affects QCD; the flavor composition of other samples
  // well defined and does not need correction
  std::vector<TString> s_other;
  //  s_other.push_back(filestring("TTJets_TuneCUETP8M1_13TeV-madgraphMLM"));
  s_other.push_back(filestring("TT_TuneCUETP8M1_13TeV-powheg-pythia8"));
  s_other.push_back(filestring("WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_other.push_back(filestring("WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_other.push_back(filestring("ZJetsToQQ_HT600toInf_13TeV-madgraph"));
  s_other.push_back(filestring("ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ZJetsToQQ_HT600toInf_13TeV-madgraph"));
  s_other.push_back(filestring("DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_other.push_back(filestring("TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"));
  s_other.push_back(filestring("TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"));
  s_other.push_back(filestring("TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
  s_other.push_back(filestring("TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
  s_other.push_back(filestring("ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8"));
  s_other.push_back(filestring("TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
  std::vector<TString> s_jetht;
  s_jetht.push_back(filestring("JetHT_Run2015C-05Oct2015-v1"));
  s_jetht.push_back(filestring("JetHT_Run2015D-05Oct2015-v1"));
  s_jetht.push_back(filestring("JetHT_Run2015D-PromptReco-v4"));

  // Reading ntuples
  std::vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_jetht, "Data",kBlack,1,"(trig[11] || trig[12]) && pass && njets<10")); 
  Samples.back().isData = true;
  Samples.back().doStack = false;
  
  // Adding non-skimmed samples
  std::vector<int> ra4_sam, ra4_sam_ns;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
    ra4_sam_ns.push_back(nsam+sam);
    std::vector<TString> sam_files = Samples[sam].file;
    // for(unsigned ifile(0); ifile < sam_files.size(); ifile++)
    //   sam_files[ifile].ReplaceAll(folder, folder_ns);
    Samples.push_back(sfeats(sam_files, Samples[sam].label, Samples[sam].color, Samples[sam].style,
			     Samples[sam].cut));
    //    Samples.back().doStack = Samples[sam].doStack;
  } // Loop over samples

  std::vector<hfeats> vars;

  std::string region("");
  TString cuts("(nmus+nels)==1");
  switch(section){
  case 1:
    cuts = "(nmus+nels)==0&&ht>1500&&nbm>=2&&njets>=6&&njets<10 && mj>800";
    vars.push_back(hfeats("(jets_csv-0.89)/0.11", 20, 0, 1, ra4_sam, "(CSV-CSV_{cut})/(CSV_{max}-CSV_{cut})", cuts));
    vars.back().normalize = true;
    break;
  case 2:
    break;
  default:
    break;
  } 

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
  std::cout << "Making histogram " << name << " for cut " << extracut << std::endl;

  TChain *ch = new TChain("tree");
  for(auto isample : samples) {
    ch->Add(isample);
  }

  TString cut("(nmus+nels)==0&&ht>1500&&nbm>=2&&njets>=6");
  TString weightandcut;
  if(extracut.Length()==0) {
    weightandcut=Form("weight*(%s)", cut.Data());
  }
  else weightandcut=Form("weight*(%s&&%s)", cut.Data(), extracut.Data());

  TH1F *h = new TH1F(name, name, 20, 0, 1);
  ch->Project(name, "(jets_csv-0.89)/0.11", weightandcut.Data());

  file->cd();
  h->Write();
}
