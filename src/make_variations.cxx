// make_variations.cxx : output histograms for each variation type

#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TColor.h"
#include "TFile.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"
#include "utilities_macros_rpv.hpp"

void makeVariations(std::string &syst);
std::string cutandweightForVariations(std::string cut, std::string weight);
std::string cutandweightForVariationsQCD(std::string cut, std::string weight, std::string flavorWeight);
std::string cutandweightForVariationsdata(std::string cut, std::string weight);
void setOverflow(TH1F *hist);
void protectFromZero(TH1F* hist);


TFile *f;
std::map<std::string, std::string> prettySampleName;

// this is a hack to add an additional weight
std::string cutandweightForVariations(std::string cut, std::string weight)
{
  std::string newcut("(");
  newcut+=weight;
  // the default weight includes the RA4 trigger efficiency; need to exclude this
  newcut+="*";
  newcut+=rpv::luminosity.Data();
  newcut+="*weight*w_pu_rpv/eff_trig)*(";
  newcut+=cut;
  newcut+="";

  return newcut;
} 

// this is a hack to add an additional weight
// QCD also needs to have flavor reweighted
std::string cutandweightForVariationsQCD(std::string cut, std::string weight, std::string flavorWeight)
{
  std::string newcut("(");
  newcut+=weight;
  newcut+="*";
  newcut+=rpv::luminosity.Data();
  newcut+="*weight*w_pu_rpv/eff_trig*";
  newcut+=flavorWeight;
  newcut+=")*(";
  newcut+=cut;
  newcut+="";
  
  return newcut;
} 

// this is a hack to add an additional weight
std::string cutandweightForVariationsdata(std::string cut, std::string weight)
{
  std::string newcut("(");
  newcut+=weight;
  newcut+=")*(";
  newcut+=cut;
  newcut+="";

  return newcut;
} 

void setOverflow(TH1F *hist)
{
  int maxBin = hist->GetNbinsX();
  float lastBinContent = hist->GetBinContent(maxBin-1);
  float overflow = hist->GetBinContent(maxBin);
  hist->SetBinContent(maxBin-1, lastBinContent+overflow);
}

// need to avoid negative weights from single top samples
void protectFromZero(TH1F *hist)
{
  for(int i=0; i<hist->GetNbinsX(); i++) {
    float bincontent = hist->GetBinContent(i);
    float binerror = hist->GetBinError(i);
    if(bincontent-binerror<0 || isnan(bincontent)) {
      if(isnan(bincontent)) std::cout << "Found nan" << std::endl;
      hist->SetBinContent(i, 0.0);
      hist->SetBinError(i, 0.0);
    }
  }
}


void jetVariations(TString &nbm, TString &cut, const std::string &variation)
{
  std::cout << "Processing variation: " << variation << std::endl;
  // variations have index 0 (JER smearing), 1 (JES up) and 2 (JES down)
  // only include 1-sided variations of resolution
  if(variation.find("jerUp")!=std::string::npos) {
    cut.ReplaceAll("ht", "sys_ht[0]");
    cut.ReplaceAll("njets", "sys_njets[0]");
    cut.ReplaceAll("nbm", "sys_nbm[0]");
    cut.ReplaceAll("mj", "sys_mj[0]");
    nbm.ReplaceAll("nbm", "sys_nbm[0]");
  }
  if(variation.find("jesUp")!=std::string::npos) {
    cut.ReplaceAll("ht", "sys_ht[1]");
    cut.ReplaceAll("njets", "sys_njets[1]");
    cut.ReplaceAll("nbm", "sys_nbm[1]");
    cut.ReplaceAll("mj", "sys_mj[1]");
    nbm.ReplaceAll("nbm", "sys_nbm[1]");
  }
  if(variation.find("jesDown")!=std::string::npos) {
    cut.ReplaceAll("ht", "sys_ht[2]");
    cut.ReplaceAll("njets", "sys_njets[2]");
    cut.ReplaceAll("nbm", "sys_nbm[2]");
    cut.ReplaceAll("mj", "sys_mj[2]");
    nbm.ReplaceAll("nbm", "sys_nbm[2]");
  }

}


void outputHistograms(std::vector<sfeats>& Samples, std::string variation)
{
  std::cout << "outputHistograms(): " << variation << std::endl;

  std::string plotVar("nbm");
  std::vector<std::string> cuts = {"nbm>0&&ht>1500&&njets>=4&&njets<=5&&(nmus+nels)==0&&mj>=500&&mj<800","nbm>0&&ht>1500&&njets>=6&&njets<=7&&(nmus+nels)==0&&mj>=500&&mj<800",
				   "nbm>0&&ht>1200&&njets>=4&&njets<=5&&(nmus+nels)==1&&mj>=500&&mj<800", 
  				   "nbm>0&&ht>1500&&njets>=4&&njets<=5&&(nmus+nels)==0&&mj>=800","nbm>0&&ht>1500&&njets>=6&&njets<=7&&(nmus+nels)==0&&mj>=800",
				   "nbm>0&&ht>1200&&njets>=4&&njets<=5&&(nmus+nels)==1&&mj>=800",
				   // low MJ control regions
				   "nbm>0&&ht>1500&&njets>=4&&njets<=5&&(nmus+nels)==0&&mj>=300&&mj<500",
				   "nbm>0&&ht>1500&&njets>=6&&njets<=7&&(nmus+nels)==0&&mj>=300&&mj<500",
				   "nbm>0&&ht>1500&&njets>=8&&njets<=9&&(nmus+nels)==0&&mj>=300&&mj<500",
				   "nbm>0&&ht>1500&&njets>=10&&(nmus+nels)==0&&mj>=300&&mj<500",
				   // signal regions, low mj
				   "nbm>0&&ht>1500&&njets>=10&&(nmus+nels)==0&&mj>=500&&mj<800",
				   "nbm>0&&ht>1200&&njets>=6&&njets<=7&&(nmus+nels)==1&&mj>=500&&mj<800",
				   "nbm>0&&ht>1200&&njets>=8&&(nmus+nels)==1&&mj>=500&&mj<800",
				   // signal regions, high mj
				   "nbm>0&&ht>1500&&njets>=10&&(nmus+nels)==0&&mj>=800",
				   "nbm>0&&ht>1200&&njets>=6&&njets<=7&&(nmus+nels)==1&&mj>=800",
				   "nbm>0&&ht>1200&&njets>=8&&(nmus+nels)==1&&mj>=800",
  };

  // maximum number of b-tagged jets
  int nBBins=4;
  
  for(unsigned int icut=0; icut<cuts.size(); icut++) {
    // need to make temporary variable because some systematics can change cuts or plot variables
    TString tempCut=cuts.at(icut).c_str();
    TString tempPlotVar=plotVar.c_str();
    jetVariations(tempPlotVar, tempCut, variation);

    gDirectory->cd("/");
    TString directory(Form("bin%d", icut));
    if(!gDirectory->GetDirectory(directory)) gDirectory->mkdir(directory);
    gDirectory->cd(directory);
    for(unsigned int i=0; i<Samples.size(); i++) {
      TChain *ch = new TChain("tree");
      for(auto files : Samples.at(i).file) ch->Add(files.Data());
      std::string histname(prettySampleName[Samples.at(i).label.Data()]);
      std::cout << "Processing sample type: " << histname << std::endl;
      if(histname.find("data_obs")==std::string::npos ) {
	if(variation.size()>0&& variation.find("nominal")==std::string::npos) {
	  histname += "_";
	  histname += variation.c_str();
	}
      }
      else {
	if(variation.find("nominal")==std::string::npos) continue;
      }
      // erase brackets for PDF weights
      if(variation.find("pdf")!=std::string::npos) {
	histname.erase(histname.find('['), 1);
	histname.erase(histname.find(']'), 1);
      }
      TH1F * hist = new TH1F(histname.c_str(), histname.c_str(), nBBins+1, 0, nBBins+1);
      TString fullCut(Form("%s&&%s)", Samples.at(i).cut.Data(), tempCut.Data()));

      std::cout << fullCut << std::endl;
      ch->Project(histname.c_str(), tempPlotVar.Data(), fullCut.Data());
      setOverflow(hist);
      protectFromZero(hist);
      hist->Write();
      hist->Delete();
    }
  }
}


int main(int argc, char* argv[])
{
  std::vector<std::string> variations;
  std::vector<std::string> valid_variations = {"btag_bc", "btag_udsg", "gs",
					       "qcd_mur", "qcd_muf", "qcd_murf",
					       "ttbar_mur", "ttbar_muf", "ttbar_murf"
					       "wjets_mur", "wjets_muf", "wjets_murf",
  					       "other_mur", "other_muf", "other_murf"};
  if(argc<2) {
    variations.push_back("");
    std::cout << "Performing nominal variation" << std::endl;
  }
  else if(argc==2){
    variations.push_back(argv[1]);
    std::cout << "Performing variation " << argv[1] << std::endl;
  }
  else if(argc==3){
    variations.push_back(argv[1]);
    variations.at(0).append(argv[2]);
    std::cout << "Performing variation " << argv[1] << std::endl;
  }
  else {
    std::cout << "Too many arguments!" << std::endl;
  }

  if(variations.at(0).size()==0) variations.at(0)="nominal";
  f = new TFile(Form("variations/output_%s.root", variations.at(0).c_str()), "recreate");
  std::vector<std::string> types = {"Up", "Down"};
  // the nominal variations only have one sign
  if(variations.size()==0) {
    types.clear();
    types.push_back("");
  }
  for(auto ivariation : variations) {
    for(auto itype : types) {
      std::string variationName = ivariation + itype;
      std::cout << "Making variation " << variationName << std::endl;
      makeVariations(variationName);
    }
  }

  f->Close();
  return 0;
}

void makeVariations(std::string &syst){ 

  std::string extraWeight("1");  
  std::string qcdWeight("1");  
  std::string ttbarWeight("1");
  std::string wjetsWeight("1");
  std::string otherWeight("1");
  std::string signalWeight("1");


  std::vector<double> gs_dmc(4);
  //Get values for GS syst
  if(std::string::npos != syst.find("gs")){

    TFile *gs_file = TFile::Open("syst_gs.root");
    TGraphErrors* h_gs_dmc = static_cast<TGraphErrors*>(gs_file->Get("dmc_ldrbb_allmj"));
    
    double temp_val;
    for(unsigned int ibin=0; ibin<4; ibin++)
      h_gs_dmc->GetPoint(ibin,temp_val,gs_dmc[ibin]);   
  }

  // weights directly affecting b-tagging in all samples
  if(syst=="btag_bcUp") extraWeight="sys_bctag[0]";
  if(syst=="btag_bcDown") extraWeight="sys_bctag[1]";
  if(syst=="btag_udsgUp") extraWeight="sys_udsgtag[0]";
  if(syst=="btag_udsgDown") extraWeight="sys_udsgtag[1]";
  if(syst=="gsUp") extraWeight="(1+0.2*fromGS)";
  if(syst=="gsDown") extraWeight="(1-0.2*fromGS)";
  if(syst=="gs45Up") extraWeight="(1+(1-"+std::to_string(gs_dmc[0])+")*fromGS*(njets==4 || njets==5))";
  if(syst=="gs45Down") extraWeight="(1-(1-"+std::to_string(gs_dmc[0])+")*fromGS*(njets==4 || njets==5))";
  if(syst=="gs67Up") extraWeight="(1+(1-"+std::to_string(gs_dmc[1])+")*fromGS*(njets==6 || njets==7))";
  if(syst=="gs67Down") extraWeight="(1-(1-"+std::to_string(gs_dmc[1])+")*fromGS*(njets==6 || njets==7))";
  if(syst=="gs89Up") extraWeight="(1+(1-"+std::to_string(gs_dmc[2])+")*fromGS*(njets==8 || njets==9))";
  if(syst=="gs89Down") extraWeight="(1-(1-"+std::to_string(gs_dmc[2])+")*fromGS*(njets==8 || njets==9))";
  if(syst=="gs10InfUp") extraWeight="(1+(1-"+std::to_string(gs_dmc[3])+")*fromGS*(njets>=10))";
  if(syst=="gs10InfDown") extraWeight="(1-(1-"+std::to_string(gs_dmc[3])+")*fromGS*(njets>=10))";

  // other weights affecting all samples
  if(syst=="lep_effUp") extraWeight="w_lep";
  if(syst=="lep_effDown") extraWeight="(2-w_lep)";
  if(syst=="pileupUp") extraWeight="sys_pu_rpv[0]";
  if(syst=="pileupDown") extraWeight="sys_pu_rpv[1]";

  // pdf weights
  if(syst.find("w_pdf")!=std::string::npos) {
    if(syst.find("Up")!=std::string::npos) {
      size_t upLength=2;
      syst.erase(syst.find("Up"), upLength);
      syst.insert(5, "[");
      syst.append("]");
      extraWeight=syst;
      syst.append("Up");
    }
    // negative PDF variations have the same difference from 1  but with opposite sign 1+(1-weight)=2-weight
    else {
      size_t downLength=4;
      syst.erase(syst.find("Down"), downLength);
      syst.insert(5, "[");
      syst.append("]");
      extraWeight=syst;
      extraWeight="(2-";
      extraWeight+=syst;
      extraWeight+=")";
      syst.append("Down");
    }
  }

  // for the variations that do not depend on sample type
  signalWeight=qcdWeight=otherWeight=wjetsWeight=ttbarWeight=extraWeight;

  TFile *csv_weight_file = TFile::Open("data/low_njet.root");
  TH1F *csv_weight = static_cast<TH1F*>(csv_weight_file->Get("csv_weight"));
  float bflavorValCentral = csv_weight->GetBinContent(1);
  float bflavorValError = csv_weight->GetBinError(1);
  float cflavorValCentral = csv_weight->GetBinContent(2);
  // negative sign implements anticorrelation between b and c reweightings
  float cflavorValError = -csv_weight->GetBinError(2);
  float lflavorValCentral = csv_weight->GetBinContent(3);
  float lflavorValError = csv_weight->GetBinError(3);
  csv_weight_file->Close();
  f->cd();
  
  std::cout << "Reweight b jets by " << bflavorValCentral << " +/ " << bflavorValError << std::endl;
  std::cout << "Reweight c jets by " << cflavorValCentral << " +/ " << cflavorValError << std::endl;
  std::cout << "Reweight l jets by " << lflavorValCentral << " +/ " << lflavorValError << std::endl;

  // To provide QCD flavor reweighting
  double bflavorValUp, bflavorValDown;
  bflavorValUp=bflavorValCentral+bflavorValError;
  bflavorValDown=bflavorValCentral-bflavorValError;
  double cflavorValUp, cflavorValDown;
  cflavorValUp=cflavorValCentral+cflavorValError;
  cflavorValDown=cflavorValCentral-cflavorValError;
  double lflavorValUp, lflavorValDown;
  lflavorValUp=lflavorValCentral=lflavorValDown=1.0;
  
  TString bflavor("(Sum$(abs(jets_hflavor)==5)>0)");
  TString cflavor("(Sum$(abs(jets_hflavor)==5)==0 && Sum$(abs(jets_hflavor)==4)>0)");
  TString lflavor("(Sum$(abs(jets_hflavor)==5)==0 && Sum$(abs(jets_hflavor)==4)==0)");
  TString flavorWeightUp(Form("(%s*%f + %s*%f + %s*%f)", bflavor.Data(), bflavorValUp, cflavor.Data(), cflavorValUp, lflavor.Data(), lflavorValUp));
  TString flavorWeightCentral(Form("(%s*%f + %s*%f + %s*%f)", bflavor.Data(), bflavorValCentral, cflavor.Data(), cflavorValCentral, lflavor.Data(), lflavorValCentral));
  TString flavorWeightDown(Form("(%s*%f + %s*%f + %s*%f)", bflavor.Data(), bflavorValDown, cflavor.Data(), cflavorValDown, lflavor.Data(), lflavorValDown));
  std::string qcdFlavorWeight=flavorWeightCentral.Data();
  
  if(syst=="qcd_flavorUp") {
    qcdFlavorWeight=flavorWeightUp.Data();
    std::cout << qcdFlavorWeight << "\n" << std::endl;
  }
  if(syst=="qcd_flavorDown") {
    qcdFlavorWeight=flavorWeightDown.Data();
    std::cout << qcdFlavorWeight << "\n" << std::endl;
  }
  if(syst=="qcd_mufUp") qcdWeight="sys_muf[0]";
  if(syst=="qcd_mufDown") qcdWeight="sys_muf[1]";
  if(syst=="qcd_murUp") qcdWeight="sys_mur[0]";
  if(syst=="qcd_murDown") qcdWeight="sys_mur[1]";
  if(syst=="qcd_murfUp") qcdWeight="sys_murf[0]";
  if(syst=="qcd_murfDown") qcdWeight="sys_murf[1]";

  if(syst=="ttbar_mufUp") ttbarWeight="sys_muf[0]";
  if(syst=="ttbar_mufDown") ttbarWeight="sys_muf[1]";
  if(syst=="ttbar_murUp") ttbarWeight="sys_mur[0]";
  if(syst=="ttbar_murDown") ttbarWeight="sys_mur[1]";
  if(syst=="ttbar_murfUp") ttbarWeight="sys_murf[0]";
  if(syst=="ttbar_murfDown") ttbarWeight="sys_murf[1]";

  if(syst=="ttbar_ptUp") ttbarWeight="w_toppt";
  if(syst=="ttbar_ptDown") ttbarWeight="(2-w_toppt)";

  if(syst=="other_mufUp") otherWeight="sys_muf[0]";
  if(syst=="other_mufDown") otherWeight="sys_muf[1]";
  if(syst=="other_murUp") otherWeight="sys_mur[0]";
  if(syst=="other_murDown") otherWeight="sys_mur[1]";
  if(syst=="other_murfUp") otherWeight="sys_murf[0]";
  if(syst=="other_murfDown") otherWeight="sys_murf[1]";

  if(syst=="signal_mufUp") signalWeight="sys_muf[0]";
  if(syst=="signal_mufDown") signalWeight="sys_muf[1]";
  if(syst=="signal_murUp") signalWeight="sys_mur[0]";
  if(syst=="signal_murDown") signalWeight="sys_mur[1]";
  if(syst=="signal_murfUp") signalWeight="sys_murf[0]";
  if(syst=="signal_murfDown") signalWeight="sys_murf[1]";
  // only apply ISR systematic to signal
  if(syst=="isrUp") signalWeight="sys_isr[0]";
  if(syst=="isrDown") signalWeight="sys_isr[1]";

  if(syst=="wjets_mufUp") wjetsWeight="sys_muf[0]";
  if(syst=="wjets_mufDown") wjetsWeight="sys_muf[1]";
  if(syst=="wjets_murUp") wjetsWeight="sys_mur[0]";
  if(syst=="wjets_murDown") wjetsWeight="sys_mur[1]";
  if(syst=="wjets_murfUp") wjetsWeight="sys_murf[0]";
  if(syst=="wjets_murfDown") wjetsWeight="sys_murf[1]";

  TString folder_links="/homes/cawest/links/";

  std::vector<TString> s_rpv_1000;
  s_rpv_1000.push_back("/homes/cawest/babymaker/CMSSW_7_4_14/src/babymaker/RPV_M1000.root");
  std::vector<TString> s_rpv_1100;
  s_rpv_1100.push_back("/homes/cawest/babymaker/CMSSW_7_4_14/src/babymaker/RPV_M1100.root");
  std::vector<TString> s_rpv_1200;
  s_rpv_1200.push_back("/homes/cawest/babymaker/CMSSW_7_4_14/src/babymaker/RPV_M1200.root");
  std::vector<TString> s_rpv_1300;
  s_rpv_1300.push_back("/homes/cawest/babymaker/CMSSW_7_4_14/src/babymaker/RPV_M1300.root");
  std::vector<TString> s_rpv_1400;
  s_rpv_1400.push_back("/homes/cawest/babymaker/CMSSW_7_4_14/src/babymaker/RPV_M1400.root");
  std::vector<TString> s_tt;
  //  s_tt.push_back(filestring("TTJets_TuneCUETP8M1_13TeV-madgraphMLM"));
  //  s_tt.push_back(filestring("TT_TuneCUETP8M1_13TeV-powheg-pythia8"));
  s_tt.push_back(filestring("TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_tt.push_back(filestring("TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
  s_tt.push_back(filestring("TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_tt.push_back(filestring("TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
  s_tt.push_back(filestring("TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_tt.push_back(filestring("TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));

  std::vector<TString> s_qcd;
  s_qcd.push_back(filestring("QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_qcd.push_back(filestring("QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
  s_qcd.push_back(filestring("QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_qcd.push_back(filestring("QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
  s_qcd.push_back(filestring("QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_qcd.push_back(filestring("QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
  std::vector<TString> s_wjets;
  s_wjets.push_back(filestring("WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  s_wjets.push_back(filestring("WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
  std::vector<TString> s_other;
  s_other.push_back(filestring("ZJetsToQQ_HT600toInf_13TeV-madgraph"));
  s_other.push_back(filestring("ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
  s_other.push_back(filestring("DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
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
  std::string blinding("((njets<10 && (nmus+nels)==0) || (nmus+nels==1 && njets<6))");
  std::vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_rpv_1000, "#tilde{g}(1000)", ra4::c_t1tttt, 1,cutandweightForVariations("1",signalWeight)));
  Samples.push_back(sfeats(s_rpv_1100, "#tilde{g}(1100)", ra4::c_t1tttt, 1,cutandweightForVariations("1",signalWeight)));
  Samples.push_back(sfeats(s_rpv_1200, "#tilde{g}(1200)", ra4::c_t1tttt, 1,cutandweightForVariations("1",signalWeight)));
  Samples.push_back(sfeats(s_rpv_1300, "#tilde{g}(1300)", ra4::c_t1tttt, 1,cutandweightForVariations("1",signalWeight)));
  Samples.push_back(sfeats(s_rpv_1400, "#tilde{g}(1400)", ra4::c_t1tttt, 1,cutandweightForVariations("1",signalWeight)));
  Samples.push_back(sfeats(s_wjets, "W+jets", kTeal, 1,cutandweightForVariations("1", wjetsWeight)));
  Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1,cutandweightForVariationsQCD("1",qcdWeight, qcdFlavorWeight))); 
  Samples.push_back(sfeats(s_tt, "t#bar{t}", kTeal, 1,cutandweightForVariations("1", ttbarWeight)));
  Samples.push_back(sfeats(s_other, "Other", ra4::c_other, 1, cutandweightForVariations("1", otherWeight)));
  Samples.push_back(sfeats(s_jetht, "Data",kBlack,1,cutandweightForVariationsdata("trig[12] && pass && " + blinding, "1")));
  Samples.back().isData = true;
  Samples.back().doStack = false;

  // convert pretty sample name to the name used in the datacard
  prettySampleName["Data"] = "data_obs";
  prettySampleName["#tilde{g}(1000)"] = "signal_M1000";
  prettySampleName["#tilde{g}(1100)"] = "signal_M1100";
  prettySampleName["#tilde{g}(1200)"] = "signal_M1200";
  prettySampleName["#tilde{g}(1300)"] = "signal_M1300";
  prettySampleName["#tilde{g}(1400)"] = "signal_M1400";
  prettySampleName["QCD"] = "qcd";
  prettySampleName["W+jets"] = "wjets";
  prettySampleName["t#bar{t}"] = "ttbar";
  prettySampleName["Other"] = "other";

  outputHistograms(Samples, syst);

}

