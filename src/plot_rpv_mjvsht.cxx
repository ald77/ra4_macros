#include <iostream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TH2D.h"
#include "TStyle.h"

#include "utilities.hpp"
#include "utilities_macros.hpp"
#include "utilities_macros_rpv.hpp"

namespace{
  TString plot_type=".pdf";
  TString outdir="rpv_mjvsht";

}

using namespace std;

int main(){

  TString sig_folder = "/homes/cawest/babymaker/CMSSW_7_4_14/src/babymaker/";  
  TString folder = "/net/cms29/cms29r0/cawest/skims/ht1200/";

  TChain* sig1000 = new TChain("tree");
  sig1000->Add(sig_folder+"RPV_M1000.root");
  TChain* sig1200 = new TChain("tree");
  sig1200->Add(sig_folder+"RPV_M1200.root");
  TChain* sig1400 = new TChain("tree");
  sig1400->Add(sig_folder+"RPV_M1400.root");
  
  TChain* bkg = new TChain("tree");
  bkg->Add(folder+"*QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  bkg->Add(folder+"*QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");
  bkg->Add(folder+"*QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  bkg->Add(folder+"*QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");
  bkg->Add(folder+"*QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  bkg->Add(folder+"*QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");
  bkg->Add(folder+"*TTJets_TuneCUETP8M1_13TeV-madgraphMLM*"); //Skimmed so there are no leptons in the dataset
  bkg->Add(folder+"*TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  bkg->Add(folder+"*TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");
  bkg->Add(folder+"*TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  bkg->Add(folder+"*TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");
  bkg->Add(folder+"*TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  bkg->Add(folder+"*TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");
  bkg->Add(folder+"*ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1*");
  bkg->Add(folder+"*ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1*");
  bkg->Add(folder+"*ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1*");
  bkg->Add(folder+"*ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1*");
  bkg->Add(folder+"*ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1*");
  bkg->Add(folder+"*DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  bkg->Add(folder+"*ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8*");
  bkg->Add(folder+"*TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8*");
  bkg->Add(folder+"*TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8*");
  bkg->Add(folder+"*TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8*");
  bkg->Add(folder+"*TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8*");
  bkg->Add(folder+"*TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8*");
  bkg->Add(folder+"*WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  bkg->Add(folder+"*WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  bkg->Add(folder+"*ZJetsToQQ_HT600toInf_13TeV-madgraph*");

  //Cuts
  vector<TString> lepcuts = {"0", "1"};
  vector<TString> htcuts = {"1200","1400","1600","1800", "2000"};
  vector<TString> mjcuts = {"400","600","800","1000","1200"};
  
  // Background yields
  double bkg_yields[2][5][5];
  double bkg_yield = -999, bkg_error = -999;
  for(unsigned int ilep=0; ilep<lepcuts.size(); ilep++){
    cout<<"Getting "+lepcuts[ilep]+" lepton background yields..."<<endl;
    for(unsigned int iht=0; iht<htcuts.size(); iht++){
      for(unsigned int imj=0; imj<mjcuts.size(); imj++){
	TString cut = "njets>=4&&nleps=="+lepcuts[ilep]+"&&ht>"+htcuts[iht]+"&&mj>"+mjcuts[imj];
	getYieldErr(*bkg,"2.69*weight*w_pu_rpv/eff_trig*("+cut+")", bkg_yield, bkg_error);
	bkg_yields[ilep][iht][imj] = bkg_yield;
      }
    }
  }

  cout<<"Getting signal yields..."<<endl;

  //Signal yields
  double sig1000_yields[2][5][5];
  double sig1200_yields[2][5][5];
  double sig1400_yields[2][5][5];

  double sig_yield = -999, sig_error = -999;
  for(unsigned int ilep=0; ilep<lepcuts.size(); ilep++){
    for(unsigned int iht=0; iht<htcuts.size(); iht++){
      for(unsigned int imj=0; imj<mjcuts.size(); imj++){
	TString cut = "nbm>=2&&njets>=8&&nleps=="+lepcuts[ilep]+"&&ht>"+htcuts[iht]+"&&mj>"+mjcuts[imj];
	if(ilep==1) cut.ReplaceAll("njets>=8","njets>=6");
	//M1000
	getYieldErr(*sig1000,"2.69*weight*w_pu_rpv/eff_trig*("+cut+")", sig_yield, sig_error);
	sig1000_yields[ilep][iht][imj] = sig_yield;
	//M1200
	getYieldErr(*sig1200,"2.69*weight*w_pu_rpv/eff_trig*("+cut+")", sig_yield, sig_error);
	sig1200_yields[ilep][iht][imj] = sig_yield;
	//M1400
	getYieldErr(*sig1400,"2.69*weight*w_pu_rpv/eff_trig*("+cut+")", sig_yield, sig_error);
	sig1400_yields[ilep][iht][imj] = sig_yield;
      }
    }
  }
  //Plot printing
  cout<<"Printing plots..."<<endl;

  // S/sqrt(B), Signal, Backround and 0-lepton,1-lepton
  TH2D* h_sig1000[3][2];
  h_sig1000[0][0] = new TH2D("srb_sig1000_nlep0","S/sqrt(B);HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1000[0][1] = new TH2D("srb_sig1000_nlep1","S/sqrt(B);HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1000[1][0] = new TH2D("s_sig1000_nlep0","S;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1000[1][1] = new TH2D("s_sig1000_nlep1","S;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1000[2][0] = new TH2D("b_sig1000_nlep0","B;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1000[2][1] = new TH2D("b_sig1000_nlep1","B;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);

  TH2D* h_sig1200[3][2];
  h_sig1200[0][0] = new TH2D("srb_sig1200_nlep0","S/sqrt(B);HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1200[0][1] = new TH2D("srb_sig1200_nlep1","S/sqrt(B);HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1200[1][0] = new TH2D("s_sig1200_nlep0","S;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1200[1][1] = new TH2D("s_sig1200_nlep1","S;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1200[2][0] = new TH2D("b_sig1200_nlep0","B;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1200[2][1] = new TH2D("b_sig1200_nlep1","B;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);

  TH2D* h_sig1400[3][2];
  h_sig1400[0][0] = new TH2D("srb_sig1400_nlep0","S/sqrt(B);HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1400[0][1] = new TH2D("srb_sig1400_nlep1","S/sqrt(B);HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1400[1][0] = new TH2D("s_sig1400_nlep0","S;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1400[1][1] = new TH2D("s_sig1400_nlep1","S;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1400[2][0] = new TH2D("b_sig1400_nlep0","B;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);
  h_sig1400[2][1] = new TH2D("b_sig1400_nlep1","B;HT>X [GeV];MJ>Y [GeV]",5,1200,2200,5,400,1400);

  for(unsigned int ilep=0; ilep<lepcuts.size(); ilep++){
    for(unsigned int iht=0; iht<htcuts.size(); iht++){
      for(unsigned int imj=0; imj<mjcuts.size(); imj++){
	// S/sqrt(B)
	h_sig1000[0][ilep]->Fill(htcuts[iht].Atoi(),mjcuts[imj].Atoi(),sig1000_yields[ilep][iht][imj]/sqrt(bkg_yields[ilep][iht][imj]));
	h_sig1200[0][ilep]->Fill(htcuts[iht].Atoi(),mjcuts[imj].Atoi(),sig1200_yields[ilep][iht][imj]/sqrt(bkg_yields[ilep][iht][imj]));
	h_sig1400[0][ilep]->Fill(htcuts[iht].Atoi(),mjcuts[imj].Atoi(),sig1400_yields[ilep][iht][imj]/sqrt(bkg_yields[ilep][iht][imj]));

	// Signal
	h_sig1000[1][ilep]->Fill(htcuts[iht].Atoi(),mjcuts[imj].Atoi(),sig1000_yields[ilep][iht][imj]);
	h_sig1200[1][ilep]->Fill(htcuts[iht].Atoi(),mjcuts[imj].Atoi(),sig1200_yields[ilep][iht][imj]);
	h_sig1400[1][ilep]->Fill(htcuts[iht].Atoi(),mjcuts[imj].Atoi(),sig1400_yields[ilep][iht][imj]);

	//Background
	h_sig1000[2][ilep]->Fill(htcuts[iht].Atoi(),mjcuts[imj].Atoi(),bkg_yields[ilep][iht][imj]);
	h_sig1200[2][ilep]->Fill(htcuts[iht].Atoi(),mjcuts[imj].Atoi(),bkg_yields[ilep][iht][imj]);
	h_sig1400[2][ilep]->Fill(htcuts[iht].Atoi(),mjcuts[imj].Atoi(),bkg_yields[ilep][iht][imj]);
      }
    }
    TCanvas c1;
    gStyle->SetOptStat(0);
    gStyle->SetPaintTextFormat("8.2f");
    for(int iplot=0; iplot<3; iplot++){
      //Sig1000
      h_sig1000[iplot][ilep]->SetMarkerSize(2);
      h_sig1000[iplot][ilep]->GetXaxis()->CenterLabels(true);
      h_sig1000[iplot][ilep]->GetXaxis()->SetNdivisions(205);
      h_sig1000[iplot][ilep]->GetYaxis()->CenterLabels(true);
      h_sig1000[iplot][ilep]->GetYaxis()->SetNdivisions(205);
      h_sig1000[iplot][ilep]->Draw("colz text");
      c1.SaveAs("./plots/"+outdir+"/"+h_sig1000[iplot][ilep]->GetName()+plot_type);
      //Sig1200
      h_sig1200[iplot][ilep]->SetMarkerSize(2);
      h_sig1200[iplot][ilep]->GetXaxis()->CenterLabels(true);
      h_sig1200[iplot][ilep]->GetXaxis()->SetNdivisions(205);
      h_sig1200[iplot][ilep]->GetYaxis()->CenterLabels(true);
      h_sig1200[iplot][ilep]->GetYaxis()->SetNdivisions(205);
      h_sig1200[iplot][ilep]->Draw("colz text");
      c1.SaveAs("./plots/"+outdir+"/"+h_sig1200[iplot][ilep]->GetName()+plot_type);
      //Sig1400
      h_sig1400[iplot][ilep]->SetMarkerSize(2);
      h_sig1400[iplot][ilep]->GetXaxis()->CenterLabels(true);
      h_sig1400[iplot][ilep]->GetXaxis()->SetNdivisions(205);
      h_sig1400[iplot][ilep]->GetYaxis()->CenterLabels(true);
      h_sig1400[iplot][ilep]->GetYaxis()->SetNdivisions(205);
      h_sig1400[iplot][ilep]->Draw("colz text");
      c1.SaveAs("./plots/"+outdir+"/"+h_sig1400[iplot][ilep]->GetName()+plot_type);
    }
  }
}
