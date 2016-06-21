#include <iostream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TStyle.h"

#include "small_tree_rpv.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"
#include "utilities_macros_rpv.hpp"

namespace{
  const int nmjbins = 3;
  const int nnjetbins = 4;
  bool make1D=true;
  TString plot_type=".pdf";
  TString plot_style="RA4";
  TString outDir="rpv_gs";
}

using namespace std;

vector<vector<double> >  getYields(small_tree_rpv &tree, vector<vector<double> >& low_drbb, vector<vector<double> >& high_drbb_err, vector<vector<double> >& low_drbb_err, bool isData=false);
void printPlots(TGraphErrors *graph, TString title="", TString filename="syst_gs.pdf");
void print1D(vector<vector<double> > norm);
double addError(double error, double added_error);
double divideErrors(double x, double y, double dx, double dy);

int main(){
  
  string folder = "/net/cms29/cms29r0/cawest/skims/ht1200/";

  small_tree_rpv data(folder+"*JetHT_Run2015C_25ns-05Oct2015-v1*");
  data.Add(folder+"*JetHT_Run2015D-05Oct2015-v1*");
  data.Add(folder+"*JetHT_Run2015D-PromptReco-v4*");
  
  small_tree_rpv qcd(folder+"*QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  qcd.Add(folder+"*QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");
  qcd.Add(folder+"*QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  qcd.Add(folder+"*QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");
  qcd.Add(folder+"*QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  qcd.Add(folder+"*QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");

  small_tree_rpv other(folder+"*TTJets_TuneCUETP8M1_13TeV-madgraphMLM*"); //Skimmed so there are no leptons in the dataset
  other.Add(folder+"*TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  other.Add(folder+"*TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");
  other.Add(folder+"*TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  other.Add(folder+"*TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");
  other.Add(folder+"*TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  other.Add(folder+"*TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1*");
  other.Add(folder+"*ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1*");
  other.Add(folder+"*ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1*");
  other.Add(folder+"*ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1*");
  other.Add(folder+"*ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1*");
  other.Add(folder+"*ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1*");
  other.Add(folder+"*DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  other.Add(folder+"*ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8*");
  other.Add(folder+"*TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8*");
  other.Add(folder+"*TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8*");
  other.Add(folder+"*TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8*");
  other.Add(folder+"*TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8*");
  other.Add(folder+"*TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8*");
  other.Add(folder+"*WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  other.Add(folder+"*WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8*");
  other.Add(folder+"*ZJetsToQQ_HT600toInf_13TeV-madgraph*");


  cout<<"[syst_gs] Getting Yields..."<<endl;

  // yields[MJ][Njets]: MJ=0/1/2 --> MJ>500/500<MJ<800/MJ>800 and Njets=0/1/2/3 --> 4-5/6-7/8-9/>10 jets
  vector<vector<double> > data_yields_ldrbb, data_yields_hdrbb_err, data_yields_ldrbb_err;
  vector<vector<double> > qcd_yields_ldrbb, qcd_yields_hdrbb_err, qcd_yields_ldrbb_err;
  vector<vector<double> > other_yields_ldrbb, other_yields_hdrbb_err, other_yields_ldrbb_err;
  
  vector<vector<double> > data_yields_hdrbb = getYields(data, data_yields_ldrbb, data_yields_hdrbb_err, data_yields_ldrbb_err, true);
  vector<vector<double> > qcd_yields_hdrbb = getYields(qcd, qcd_yields_ldrbb, qcd_yields_hdrbb_err, qcd_yields_ldrbb_err);
  vector<vector<double> > other_yields_hdrbb = getYields(other, other_yields_ldrbb, other_yields_hdrbb_err, other_yields_ldrbb_err);

  vector<vector<double> > norm_hdrbb(nmjbins, vector<double>(nnjetbins)); // normalization from high dr_bb region
  vector<vector<double> > norm_hdrbb_err(nmjbins, vector<double>(nnjetbins)); // error on the normalization from high dr_bb region
  vector<vector<double> > dmc_ldrbb(nmjbins, vector<double>(nnjetbins)); // data/mc for low dr_bb region
  vector<vector<double> > dmc_ldrbb_err(nmjbins, vector<double>(nnjetbins)); // error on data/mc for low dr_bb region

  cout<<"[syst_gs] Finding Normalization..."<<endl;

  for(unsigned int i=0; i<data_yields_hdrbb.size(); i++){
    for(unsigned int j=0; j<data_yields_hdrbb[i].size(); j++){

      // Subtract yields of other
      data_yields_hdrbb[i][j] -= other_yields_hdrbb[i][j];
      // Uncertainty on subtraction of other
      data_yields_hdrbb_err[i][j] = AddInQuadrature(data_yields_hdrbb_err[i][j], other_yields_hdrbb[i][j]);
      
      // Find normalization in high dr_bb region
      norm_hdrbb[i][j] = data_yields_hdrbb[i][j]/qcd_yields_hdrbb[i][j];
      // Uncertainty on normalization
      norm_hdrbb_err[i][j] = divideErrors(data_yields_hdrbb[i][j], qcd_yields_hdrbb[i][j], data_yields_hdrbb_err[i][j], qcd_yields_hdrbb_err[i][j]);

      //Get data/mc in low dr_bb region
      dmc_ldrbb[i][j] = data_yields_ldrbb[i][j]/((norm_hdrbb[i][j]*qcd_yields_ldrbb[i][j]) + other_yields_ldrbb[i][j]); 
      // Uncertainty on data/mc in low dr_bb region
      dmc_ldrbb_err[i][j] = divideErrors(data_yields_ldrbb[i][j], ((norm_hdrbb[i][j]*qcd_yields_ldrbb[i][j]) + other_yields_ldrbb[i][j]), data_yields_ldrbb_err[i][j],
					 AddInQuadrature( 
							 divideErrors(data_yields_hdrbb[i][j], qcd_yields_ldrbb[i][j], data_yields_hdrbb_err[i][j], qcd_yields_ldrbb_err[i][j])
							 , other_yields_ldrbb_err[i][j]));
    }
  }

  cout<<"[syst_gs] Writing Files..."<<endl;
  
  vector<double> xbins = {0,1,2,3};
  vector<double> xerr = {0,0,0,0};
  
  TGraphErrors *dmc_ldrbb_allmj = new TGraphErrors(dmc_ldrbb[0].size(), &(xbins[0]), &(dmc_ldrbb[0][0]), &(xerr[0]), &(dmc_ldrbb_err[0][0]));
  dmc_ldrbb_allmj->SetName("dmc_ldrbb_allmj");
  TGraphErrors *dmc_ldrbb_lowmj = new TGraphErrors(dmc_ldrbb[1].size(), &(xbins[0]), &(dmc_ldrbb[1][0]), &(xerr[0]), &(dmc_ldrbb_err[1][0]));
  dmc_ldrbb_lowmj->SetName("dmc_ldrbb_lowmj");
  TGraphErrors *dmc_ldrbb_highmj = new TGraphErrors(dmc_ldrbb[2].size(), &(xbins[0]), &(dmc_ldrbb[2][0]), &(xerr[0]), &(dmc_ldrbb_err[2][0]));
  dmc_ldrbb_highmj->SetName("dmc_ldrbb_highmj");

  // Write out values
  TFile *out = new TFile("syst_gs.root","recreate");
  out->cd();
  dmc_ldrbb_allmj->Write();
  dmc_ldrbb_lowmj->Write();
  dmc_ldrbb_highmj->Write();
  out->Close();

  // Format Plots
  printPlots(dmc_ldrbb_allmj, "M_{J}>500", "syst_gs/dmc_ldrbb_allmj.pdf");
  printPlots(dmc_ldrbb_lowmj, "500 #leq M_{J} #leq 800", "syst_gs/dmc_ldrbb_lowmj.pdf");
  printPlots(dmc_ldrbb_highmj, "M_{J}>800", "syst_gs/dmc_ldrbb_highmj.pdf");

  // print 1D Dists after the normalization in the high dr_bb region
  if(make1D){
    cout<<"[syst_gs] Making 1D Distributions... "<<endl;
    print1D(norm_hdrbb);
  }
} 

vector<vector<double> >  getYields(small_tree_rpv &tree, vector<vector<double> >& low_drbb, vector<vector<double> >& high_drbb_err, vector<vector<double> >& low_drbb_err, bool isData){
  
  // yields[MJ][Njets]: MJ=0/1/2 --> MJ>500/500<MJ<800/MJ>800 and Njets=0/1/2/3 --> 4-5/6-7/8-9/>10 jets
  vector<vector<double> > yields_hdrbb(nmjbins, vector<double>(nnjetbins)); //high dr_bb region
  vector<vector<double> > yields_ldrbb(nmjbins, vector<double>(nnjetbins)); //low dr_bb region
  vector<vector<double> > yields_hdrbb_err(nmjbins, vector<double>(nnjetbins)); 
  vector<vector<double> > yields_ldrbb_err(nmjbins, vector<double>(nnjetbins)); 

  double lumi = atof(rpv::luminosity);

  if(isData) lumi=1;

  for(int iEnt=0; iEnt<tree.GetEntries(); iEnt++){
    tree.GetEntry(iEnt);

    // Pass filters and trigger for data
    if(isData && (!tree.pass()||!tree.trig()[12])) continue;

    // At least two b-jets
    if(tree.nbm()<2) continue;
    
    // 0-lepton selection
    if(tree.nleps()==0 && tree.ht()>1500 && tree.mj()>500){
      
      // 4-5 jets
      if(tree.njets()>=4 && tree.njets()<=5){ 
	for(unsigned int iDrbb=0; iDrbb<tree.dr_bb().size(); iDrbb++){
	  // High-dr_bb
	  if(tree.dr_bb()[iDrbb]>=2.4){
	    yields_hdrbb[0][0] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	    yields_hdrbb_err[0][0] = addError(yields_hdrbb_err[0][0], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));

	    // Low-MJ
	    if(tree.mj()>500 && tree.mj()<=800){
	      yields_hdrbb[1][0] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_hdrbb_err[1][0] = addError(yields_hdrbb_err[1][0], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	    // High-MJ 
	    else if(tree.mj()>800){
	      yields_hdrbb[2][0] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_hdrbb_err[2][0] = addError(yields_hdrbb_err[2][0], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	  }
	  // Low-dr_bb
	  else if(tree.dr_bb()[iDrbb]<=1.6){
	    yields_ldrbb[0][0] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	    yields_ldrbb_err[0][0] = addError(yields_ldrbb_err[0][0], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    
	    // Low-MJ
	    if(tree.mj()>500 && tree.mj()<=800){
	      yields_ldrbb[1][0] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_ldrbb_err[1][0] = addError(yields_ldrbb_err[1][0], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	    // High-MJ
	    else if(tree.mj()>800){
	      yields_ldrbb[2][0] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_ldrbb_err[2][0] = addError(yields_ldrbb_err[2][0], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	  }
	}
      }
    
      // 6-7 jets
      else if(tree.njets()>=6 && tree.njets()<=7){ 
	for(unsigned int iDrbb=0; iDrbb<tree.dr_bb().size(); iDrbb++){
	  // High-dr_bb
	  if(tree.dr_bb()[iDrbb]>=2.4){
	    yields_hdrbb[0][1] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	    yields_hdrbb_err[0][1] = addError(yields_hdrbb_err[0][1], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
		
	    // Low-MJ
	    if(tree.mj()>500 && tree.mj()<=800){
	      yields_hdrbb[1][1] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_hdrbb_err[1][1] = addError(yields_hdrbb_err[1][1], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	    // High-MJ
	    else if(tree.mj()>800){
	      yields_hdrbb[2][1] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_hdrbb_err[2][1] = addError(yields_hdrbb_err[2][1], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	  }
	  // Low-dr_bb
	  else if(tree.dr_bb()[iDrbb]<=1.6){
	    yields_ldrbb[0][1] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	    yields_ldrbb_err[0][1] = addError(yields_ldrbb_err[0][1], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    
	    // Low-MJ
	    if(tree.mj()>500 && tree.mj()<=800){
	      yields_ldrbb[1][1] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_ldrbb_err[1][1] = addError(yields_ldrbb_err[1][1], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	    // High-MJ
	    else if(tree.mj()>800){
	      yields_ldrbb[2][1] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_ldrbb_err[2][1] = addError(yields_ldrbb_err[2][1], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	  }
	}

      }

      // 8-9 jets
      else if(tree.njets()>=8 && tree.njets()<=9){ 
	for(unsigned int iDrbb=0; iDrbb<tree.dr_bb().size(); iDrbb++){
	  
	  // High-dr_bb
	  if(tree.dr_bb()[iDrbb]>=2.4){
	    yields_hdrbb[0][2] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	    yields_hdrbb_err[0][2] = addError(yields_hdrbb_err[0][2], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
		    
	    // Low-MJ
	    if(tree.mj()>500 && tree.mj()<=800){
	      yields_hdrbb[1][2] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_hdrbb_err[1][2] = addError(yields_hdrbb_err[1][2], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	    // High-MJ
	    else if(tree.mj()>800){
	      yields_hdrbb[2][2] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_hdrbb_err[2][2] = addError(yields_hdrbb_err[2][2], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	  }
	  // Low-dr_bb
	  else if(tree.dr_bb()[iDrbb]<=1.6){
	    yields_ldrbb[0][2] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	    yields_ldrbb_err[0][2] = addError(yields_ldrbb_err[0][2], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    
	    // Low-MJ
	    if(tree.mj()>500 && tree.mj()<=800){
	      yields_ldrbb[1][2] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_ldrbb_err[1][2] = addError(yields_ldrbb_err[1][2], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	    // High-MJ
	    else if(tree.mj()>800){
	      yields_ldrbb[2][2] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_ldrbb_err[2][2] = addError(yields_ldrbb_err[2][2], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	  }
	}
      }
      else if(tree.njets()>=10){ 
	for(unsigned int iDrbb=0; iDrbb<tree.dr_bb().size(); iDrbb++){
	  
	  // High-dr_bb
	  if(tree.dr_bb()[iDrbb]>=2.4){
	    yields_hdrbb[0][3] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	    yields_hdrbb_err[0][3] = addError(yields_hdrbb_err[0][3], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    
	    // Low-MJ
	    if(tree.mj()>500 && tree.mj()<=800){
	      yields_hdrbb[1][3] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_hdrbb_err[1][3] = addError(yields_hdrbb_err[1][3], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	    //High-MJ
	    else if(tree.mj()>800){
	      yields_hdrbb[2][3] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_hdrbb_err[2][3] = addError(yields_hdrbb_err[2][3], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	  }
	  // Low-dr_bb
	  else if(tree.dr_bb()[iDrbb]<=1.6){
	    yields_ldrbb[0][3] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	    yields_ldrbb_err[0][3] = addError(yields_ldrbb_err[0][3], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));

	    // Low-MJ
	    if(tree.mj()>500 && tree.mj()<=800){
	      yields_ldrbb[1][3] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_ldrbb_err[1][3] = addError(yields_ldrbb_err[1][3], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	    // High-MJ
	    else if(tree.mj()>800){
	      yields_ldrbb[2][3] += lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig());
	      yields_ldrbb_err[2][3] = addError(yields_ldrbb_err[2][3], lumi*(tree.weight()*tree.w_pu_rpv()/tree.eff_trig()));
	    }
	  }
	}
      }
    }
  } 
  low_drbb = yields_ldrbb;
  high_drbb_err = yields_hdrbb_err;
  low_drbb_err = yields_ldrbb_err;
  return yields_hdrbb;
} 

void printPlots(TGraphErrors *graph, TString title, TString filename){

  gStyle->SetOptStat(0);

  // Format plot
  graph->SetMarkerStyle(20);

  TCanvas c1;
  c1.SetGridy(1);

  // Setup TH1F to provide axis labels
  double x[nnjetbins];
  for(int i=0; i<nnjetbins; i++)
    x[i] = i;
    
  TH1F* h = new TH1F("h",title,nnjetbins,x[0]-0.5,x[nnjetbins-1]+0.5);
  for(int i=0; i<nnjetbins; i++){
    if(i==0) h->GetXaxis()->SetBinLabel(i+1,"4 #leq N_{jets} #leq5");     
    if(i==1) h->GetXaxis()->SetBinLabel(i+1,"6 #leq N_{jets} #leq7");     
    if(i==2) h->GetXaxis()->SetBinLabel(i+1,"8 #leq N_{jets} #leq9");     
    if(i==3) h->GetXaxis()->SetBinLabel(i+1,"N_{jets} #geq10");     
  }
  h->GetYaxis()->SetTitle("Data/MC");
  h->GetYaxis()->SetTitleSize(0.045);
  h->SetMaximum(1.5);
  h->SetLabelSize(0.06);
  h->SetLabelSize(0.06,"Y");
  h->Draw();

  graph->Draw("P");

  c1.SaveAs("plots/"+filename);

  h->Delete();
}

void print1D(vector<vector<double> > norm){

    string extraWeight("w_pu_rpv/eff_trig");

    vector<TString> s_rpv_LO_M1000;
    s_rpv_LO_M1000.push_back("/homes/cawest/babymaker/CMSSW_7_4_14/src/babymaker/RPV_M1000_LO.root");
    vector<TString> s_rpv_LO_M1100;
    s_rpv_LO_M1100.push_back("/homes/cawest/babymaker/CMSSW_7_4_14/src/babymaker/RPV_M1100_LO.root");
    vector<TString> s_rpv_NLO;
    s_rpv_NLO.push_back("/homes/cawest/CMSSW_7_4_14/src/babymaker/RPV_M1000_NLO.root");

    vector<TString> s_tt_had;
    // this dataset is skimmed to remove the leptonic component
    s_tt_had.push_back(filestring("TTJets_TuneCUETP8M1_13TeV-madgraphMLM", true));
    vector<TString> s_tt;
    //  s_tt.push_back(filestring("TT_TuneCUETP8M1_13TeV-powheg-pythia8"));
    s_tt.push_back(filestring("TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_tt.push_back(filestring("TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    s_tt.push_back(filestring("TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_tt.push_back(filestring("TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    s_tt.push_back(filestring("TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_tt.push_back(filestring("TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    vector<TString> s_wjets;
    s_wjets.push_back(filestring("WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    vector<TString> s_singlet;
    s_singlet.push_back(filestring("ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1"));
    s_singlet.push_back(filestring("ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
    s_singlet.push_back(filestring("ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
    s_singlet.push_back(filestring("ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
    s_singlet.push_back(filestring("ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
    vector<TString> s_qcd;
    s_qcd.push_back(filestring("QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_qcd.push_back(filestring("QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    s_qcd.push_back(filestring("QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_qcd.push_back(filestring("QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    s_qcd.push_back(filestring("QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_qcd.push_back(filestring("QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    vector<TString> s_other;
    s_other.push_back(filestring("DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_other.push_back(filestring("TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"));
    s_other.push_back(filestring("TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"));
    s_other.push_back(filestring("TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
    s_other.push_back(filestring("TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
    s_other.push_back(filestring("ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8"));
    s_other.push_back(filestring("TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
    vector<TString> s_w_had;
    s_w_had.push_back(filestring("WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    vector<TString> s_z_had;
    s_z_had.push_back(filestring("ZJetsToQQ_HT600toInf_13TeV-madgraph"));
    vector<TString> s_jetht;
    s_jetht.push_back(filestring("JetHT_Run2015C_25ns-05Oct2015-v1"));
    s_jetht.push_back(filestring("JetHT_Run2015D-05Oct2015-v1"));
    s_jetht.push_back(filestring("JetHT_Run2015D-PromptReco-v4"));


    // Reading ntuples
    vector<sfeats> Samples;
    Samples.push_back(sfeats(s_jetht, "Data",kBlack,1,"trig[12] && pass && (njets<10 || (nmus+nels)==0)"));
    Samples.back().isData = true;
    Samples.back().doStack = false;

    Samples.push_back(sfeats(s_rpv_LO_M1000, "#tilde{g}(1.0 TeV)#rightarrow tbs", ra4::c_t1tttt));
    Samples.back().doStack = false;
    Samples.back().isSig = true;
    Samples.push_back(sfeats(s_rpv_LO_M1100, "#tilde{g}(1.1 TeV)#rightarrow tbs", ra4::c_t1tttt, 2));
    Samples.back().doStack = false;
    Samples.back().isSig = true;

    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[0][0])+"*w_pu_rpv/eff_trig"))); // 0 leps, All MJ, 4-5 jets
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[0][1])+"*w_pu_rpv/eff_trig"))); // 0 leps. All MJ, 6-7 jets
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[0][2])+"*w_pu_rpv/eff_trig"))); // 0 leps, All MJ, 8-9 jets
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[0][3])+"*w_pu_rpv/eff_trig"))); // 0 leps, All MJ, 10+ jets
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[1][0])+"*w_pu_rpv/eff_trig"))); // 0 leps, Low MJ, 4-5 jets
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[1][1])+"*w_pu_rpv/eff_trig"))); // 0 leps. Low MJ, 6-7 jets
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[1][2])+"*w_pu_rpv/eff_trig"))); // 0 leps, Low MJ, 8-9 jets
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[1][3])+"*w_pu_rpv/eff_trig"))); // 0 leps, Low MJ, 10+ jets
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[2][0])+"*w_pu_rpv/eff_trig"))); // 0 leps, High MJ, 4-5 jets
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[2][1])+"*w_pu_rpv/eff_trig"))); // 0 leps. High MJ, 6-7 jets
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[2][2])+"*w_pu_rpv/eff_trig"))); // 0 leps, High MJ, 8-9 jets
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",to_string(norm[2][3])+"*w_pu_rpv/eff_trig"))); // 0 leps, High MJ, 10+ jets

    Samples.push_back(sfeats(s_w_had, "W+jets, 0 l", ra4::c_wjets, 1, cutandweight("1",extraWeight)));
    Samples.push_back(sfeats(s_z_had, "Z+jets, 0 l", kBlack, 1, cutandweight("1",extraWeight)));
    Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", ra4::c_tt_1l, 1, cutandweight("ntruleps==1", extraWeight)));
    Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", ra4::c_tt_2l, 1, cutandweight("ntruleps>=2", extraWeight)));
    Samples.push_back(sfeats(s_tt_had, "t#bar{t}, 0 l", kTeal, 1, cutandweight("ntruleps==0", extraWeight)));
    Samples.push_back(sfeats(s_wjets, "W+jets, 1 l", ra4::c_wjets, 1, cutandweight("1",extraWeight)));
    Samples.push_back(sfeats(s_singlet, "Single t", ra4::c_singlet, 1, cutandweight("1",extraWeight)));
    Samples.push_back(sfeats(s_other, "Other", ra4::c_other, 1, cutandweight("1",extraWeight)));

    const int nBins=12;
    vector<vector<int> >rpv_sam(nBins);
    for(unsigned sam(0); sam < nBins; sam++){
      rpv_sam[sam].push_back(0);
      rpv_sam[sam].push_back(1);
      rpv_sam[sam].push_back(2);

      rpv_sam[sam].push_back(3+sam); //Only push back for the correct QCD sample corresponding to the bin

      rpv_sam[sam].push_back(15);    //Index should equal 3+nBins, i.e. first sample after QCD
      rpv_sam[sam].push_back(16);
      rpv_sam[sam].push_back(17);
      rpv_sam[sam].push_back(18);
      rpv_sam[sam].push_back(19);
      rpv_sam[sam].push_back(20);
      rpv_sam[sam].push_back(21);
      rpv_sam[sam].push_back(22);
    }

    vector<hfeats> vars;

    vector<TString> cuts;
    cuts.push_back("nleps==0&&ht>1500&&mj>500&&njets>=4&&njets<=5&&nbm>=2"); // 0 leps, All MJ, 4-5 jets
    cuts.push_back("nleps==0&&ht>1500&&mj>500&&njets>=6&&njets<=7&&nbm>=2"); // 0 leps, All MJ, 6-7 jets
    cuts.push_back("nleps==0&&ht>1500&&mj>500&&njets>=8&&njets<=9&&nbm>=2"); // 0 leps, All MJ, 8-9 jets
    cuts.push_back("nleps==0&&ht>1500&&mj>500&&njets>=10&&nbm>=2"); // 0 leps, All MJ, 10+ jets

    cuts.push_back("nleps==0&&ht>1500&&mj>500&&mj<=800&&njets>=4&&njets<=5&&nbm>=2"); // 0 leps, Low MJ, 4-5 jets
    cuts.push_back("nleps==0&&ht>1500&&mj>500&&mj<=800&&njets>=6&&njets<=7&&nbm>=2"); // 0 leps, Low MJ, 6-7 jets
    cuts.push_back("nleps==0&&ht>1500&&mj>500&&mj<=800&&njets>=8&&njets<=9&&nbm>=2"); // 0 leps, Low MJ, 8-9 jets
    cuts.push_back("nleps==0&&ht>1500&&mj>500&&mj<=800&&njets>=10&&nbm>=2"); // 0 leps, Low MJ, 10+ jets

    cuts.push_back("nleps==0&&ht>1500&&mj>800&&njets>=4&&njets<=5&&nbm>=2"); // 0 leps, High MJ, 4-5 jets
    cuts.push_back("nleps==0&&ht>1500&&mj>800&&njets>=6&&njets<=7&&nbm>=2"); // 0 leps, High MJ, 6-7 jets
    cuts.push_back("nleps==0&&ht>1500&&mj>800&&njets>=8&&njets<=9&&nbm>=2"); // 0 leps, High MJ, 8-9 jets
    cuts.push_back("nleps==0&&ht>1500&&mj>800&&njets>=10&&nbm>=2"); // 0 leps, High MJ, 10+ jets


    for(int iBins=0; iBins<nBins; iBins++){
      vars.push_back(hfeats("dr_bb",15, 0, 6, rpv_sam[iBins], "#DeltaR_{b#bar{b}}", cuts[iBins]));
      vars.back().whichPlots="12";
    }
    
    plot_distributions(Samples, vars, rpv::luminosity, plot_type, plot_style, outDir, true);
}

double addError(double error, double added_error){
  return sqrt(pow(error,2)+pow(added_error,2));
}

double divideErrors(double x, double y, double dx, double dy){
  if(x==0||y==0||dx==0||dy==0) cout<<"Dividing by 0 in divideErrors"<<endl;
  return (x/y)*sqrt(pow(dx/x,2)+pow(dy/y,2));
}
