// makes a datacard for RPV analysis with or without PDF uncertainties
#include "TH1.h"
#include "TString.h"
#include "TFile.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

void outputShapeSystematics(std::ofstream &file, const std::vector<std::string> shapeSysts);
void outputLognormalSystematics(std::ofstream &file);
void outputMCStatisticsSyst(std::ofstream &file, const std::vector<std::string> &bins, const std::string & signalBinName);
// determine if a histogram has an entry for a given nB
bool hasEntry(const std::string &sample, const std::string &bin, const int nB);

namespace {
  unsigned int nbins;
  unsigned int nprocesses;
}

int main(int argc, char *argv[])
{
  bool includePDFUncert = true;
  bool includeLowMJ = false;
  bool includeSignalRegion = true;

  // signal is added later
  std::vector<std::string> processes = { "qcd", "ttbar", "wjets", "other"};
  std::vector<std::string> shapeSysts = {"btag_bc", "btag_udsg",
					 "gs45", "gs67", "gs89", "gs10Inf",
					 "jes", "jer",
					 "pileup","lep_eff", "ttbar_pt",
					 "qcd_flavor",
					 "qcd_muf", "qcd_mur", "qcd_murf", 
					 "isr",
					 "ttbar_muf", "ttbar_mur", "ttbar_murf",
					 "wjets_muf", "wjets_mur", "wjets_murf",
					 "other_muf", "other_mur", "other_murf"};

  std::string gluinoMass;
  std::string signalBinName;
  if(argc<2) {
    std::cout << "Syntax: make_rpv_datacard.exe [gluino mass, in GeV]" << std::endl;
    return 1;
  }
  else {
    std::stringstream ss;
    gluinoMass = argv[1];
    ss << "signal_M" << gluinoMass;
    signalBinName = ss.str();
    // this is supposed to be the first entry in the process list
    processes.insert(processes.begin(), signalBinName);
  }

  nprocesses=processes.size();

  std::vector<std::string> bins = {"bin0", "bin1", "bin2",
				   "bin3", "bin4", "bin5"};

  if(includeLowMJ) {
    bins.push_back("bin6");
    bins.push_back("bin7");
    bins.push_back("bin8");
    bins.push_back("bin9");
  }
  if(includeSignalRegion) {
    bins.push_back("bin10");
    bins.push_back("bin11");
    bins.push_back("bin12");
    bins.push_back("bin13");
    bins.push_back("bin14");
    bins.push_back("bin15");
  }
  nbins = bins.size();

  std::string dataCardPath("/homes/cawest/rpv_2016/ra4_macros/variations/sum_rescaled.root");
  TFile *variations = TFile::Open(dataCardPath.c_str());
  std::ofstream file;
  std::string filename("datacard_M");
  filename+=gluinoMass;
  if(!includeSignalRegion) filename+="_control";

  if(includePDFUncert) {
    for(unsigned int i=0; i<100; i++) {
      TString pdf(Form("w_pdf%d", i));
      shapeSysts.push_back(pdf.Data());
    }
  }
  else {
    filename+="_nopdf";
  }
  filename+=".dat";
  file.open(filename);

  // output header
  file << "imax " << nbins << " number of bins" << std::endl;
  file << "jmax " << nprocesses-1 << std::endl;
  file << "kmax * number of nuisance parameters" << std::endl;
  file << "------------------------------------" << std::endl;

  for(unsigned int ibin=0; ibin<nbins; ibin++) {
    file << "shapes * " << bins.at(ibin) << " " << dataCardPath << " " << bins.at(ibin)
	 << "/$PROCESS " << bins.at(ibin) << "/$PROCESS_$SYSTEMATIC" << std::endl;
  }
  file << "------------------------------------" << std::endl;
  file << "bin          ";

  for(unsigned int ibin=0; ibin<nbins; ibin++) {
    variations->cd(bins.at(ibin).c_str());
    file << bins.at(ibin) << " ";
  }
  file << "\n";
  file << "observation  ";
  for(unsigned int ibin=0; ibin<nbins; ibin++) {
    TString binName(bins.at(ibin));
    TH1F *hist = static_cast<TH1F*>(variations->Get(Form("%s/data_obs",binName.Data())));
    file << hist->Integral() << " ";
    hist->Delete();
  }
  file << "\n";
  file << "------------------------------------" << std::endl;
  file << "bin  ";
  for(unsigned int ibin=0; ibin<nbins; ibin++) {
    for(unsigned int iprocess=0; iprocess<nprocesses; iprocess++) {
      file << bins.at(ibin) << " ";
    }
  }
  file << "\n";
  file << "process  ";
  for(unsigned int index=0; index<nbins*nprocesses; index++) file << processes.at(index%nprocesses) << " ";
  file << "\n";
  file << "process  ";
  for(unsigned int index=0; index<nbins*nprocesses; index++) file << index%nprocesses << " ";

  file << "\n";
  file << "rate  ";
  for(unsigned int ibin=0; ibin<nbins; ibin++) {
    for(unsigned int iprocess=0; iprocess<nprocesses; iprocess++) {
      TString histName(Form("%s/%s", bins.at(ibin).c_str(), processes.at(iprocess).c_str()));
      TH1F *hist = static_cast<TH1F*>(variations->Get(histName));
      file << hist->Integral() << "  ";
    }
  }
  file << "\n------------------------------------" << std::endl;

  // output shape systematics
  outputShapeSystematics(file, shapeSysts);
  
  // output lognormal lumi uncertainties for signal, wjets and other
  outputLognormalSystematics(file);

  // output MC statistics nuisance parameters
  outputMCStatisticsSyst(file, bins, signalBinName);

  file.close();

}

void outputLognormalSystematics(std::ofstream &file)
{
  // luminosity uncertainty is 2.7%
  file << "lumi  lnN  ";
  for(unsigned int ibin=0; ibin<nbins; ibin++) {
    file << "1.027 - - 1.027 1.027 ";
  }
  file << std::endl;

}

void outputShapeSystematics(std::ofstream &file, const std::vector<std::string> shapeSysts)
{
  for(unsigned int isyst=0; isyst<shapeSysts.size(); isyst++) {
    file << shapeSysts.at(isyst) << "     shape     ";
    if(shapeSysts.at(isyst).find("pdf")!=std::string::npos) {
      // there are 100 NNPDF variations and so each needs to be scaled down by a factor 1/sqrt(100)
      for(unsigned int index=0; index<nbins; index++) file << "0.1 0.1 0.1 0.1 0.1 ";
    }
    else {
      for(unsigned int index=0; index<nbins*nprocesses; index++) file << 1.0 << " ";
    }
    file << "\n";
  }
}

// outputs MC statistical uncertainties
void outputMCStatisticsSyst(std::ofstream &file, const std::vector<std::string> &bins, const std::string & signalBinName)
{
  //  unsigned int nbins=bins.size();
  const unsigned int maxB=4;

  // only signal, qcd, and ttbar have non-negligible MC statistics uncertainties
  std::vector<std::string> samples = {signalBinName, "qcd", "ttbar"};
  for(auto isample : samples) {
    for(unsigned int ibin = 0; ibin<bins.size(); ibin++) {
      for(unsigned int ibbin=1; ibbin<maxB+1; ibbin++) {
	if(!hasEntry(isample, bins.at(ibin), ibbin)) continue;
	file << "mcstat_" << isample << "_" << bins.at(ibin) << "_nb" << ibbin << " shape ";
	for(unsigned int ientry = 0; ientry<bins.size(); ientry++) {
	  if(ientry == ibin ) {
	    if( isample.find("signal")!=std::string::npos) file << "1 - - - - ";
	    else if( isample == "qcd") file << "- 1 - - - ";
	    else if( isample == "ttbar" ) file << "- - 1 - - ";
	    else if( isample == "wjets" ) file << "- - - 1 - ";
	    else if( isample == "other" ) file << "- - - - 1 ";
	  }
	  else file << "- - - - - ";
	}
	file << "\n";
      }
    }
  }
}

// exclude by hand following bins that have no entries:
// signal_mcstat_signal_bin0_nb3
// signal_mcstat_signal_bin2_nb4
// signal_mcstat_signal_bin3_nb4
// signal_mcstat_signal_bin5_nb4
// ttbar_mcstat_ttbar_bin0_nb4
// ttbar_mcstat_ttbar_bin5_nb4
// qcd_mcstat_qcd_bin5_nb3
// qcd_mcstat_qcd_bin5_nb4
// should do this from the histograms themselves!
bool hasEntry(const std::string &sample, const std::string &bin, const int nB)
{
  if(sample.find("signal_M1000")!=std::string::npos) {
    if(bin=="bin2" && nB==4) return false;
    if(bin=="bin3" && nB==1) return false;
    if(bin=="bin3" && nB==3) return false;
    if(bin=="bin3" && nB==4) return false;
  }
  if(sample.find("signal_M1100")!=std::string::npos) {
    if(bin=="bin3" && nB==4) return false;
    if(bin=="bin5" && nB==4) return false;
  }
  if(sample.find("signal_M1200")!=std::string::npos) {
    if(bin=="bin0" && nB==4) return false;
    if(bin=="bin2" && nB==4) return false;
    if(bin=="bin3" && nB==4) return false;
    if(bin=="bin5" && nB==4) return false;
  }
  if(sample.find("signal_M1300")!=std::string::npos) {
    if(bin=="bin2" && nB==4) return false;
    if(bin=="bin3" && nB==1) return false;
  }
  if(sample.find("signal_M1400")!=std::string::npos) {
    if(bin=="bin0" && nB==4) return false;
  }
  if(sample=="ttbar") {
    if(bin=="bin0" && nB==4) return false;
    if(bin=="bin5" && nB==4) return false;
  }
  if(sample=="qcd") {
    if(bin=="bin5" && nB==3) return false;
    if(bin=="bin5" && nB==4) return false;
    if(bin=="bin11" && nB==4) return false;
    if(bin=="bin14" && nB==4) return false;
    if(bin=="bin15" && nB==4) return false;
  }
  return true;
}
