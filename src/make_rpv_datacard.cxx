// makes a datacard for RPV analysis with or without PDF uncertainties
#include "TH1.h"
#include "TString.h"
#include "TFile.h"

#include <fstream>
#include <string>

void outputShapeSystematics(std::ofstream &file, const std::vector<std::string> shapeSysts);
void outputLognormalSystematics(std::ofstream &file);
void outputMCStatisticsSyst(std::ofstream &file, const std::vector<std::string> &bins);
// determine if a histogram has an entry for a given nB
bool hasEntry(const std::string &sample, const std::string &bin, const int nB);

namespace {
  unsigned int nbins;
  unsigned int nprocesses;
}

int main()
{
  bool includePDFUncert = true;
  bool includeLowMJ = false;
  bool includeSignalRegion = false;

  std::vector<std::string> processes = {"signal", "qcd", "ttbar", "wjets", "other"};
  std::vector<std::string> shapeSysts = {"btag_bc", "btag_udsg", "gs",
					 "jes", "lep_eff",
					 "ttbar_pt", 
					 "qcd_flavor",
					 "qcd_muf", "qcd_mur", "qcd_murf", 
					 "ttbar_muf", "ttbar_mur", "ttbar_murf",
					 "wjets_muf", "wjets_mur", "wjets_murf",
					 "other_muf", "other_mur", "other_murf"};

  nprocesses=processes.size();

  std::vector<std::string> bins = {"bin0", "bin1", "bin2",
				   "bin3", "bin4", "bin5"};

  if(includeLowMJ) {
    bins.push_back("bins6");
    bins.push_back("bins7");
    bins.push_back("bins8");
    bins.push_back("bins9");
  }
  if(includeSignalRegion) {
    bins.push_back("bins10");
    bins.push_back("bins11");
    bins.push_back("bins12");
    bins.push_back("bins13");
    bins.push_back("bins14");
    bins.push_back("bins15");
  }
  nbins = bins.size();

  std::string dataCardPath("/homes/cawest/rpv_2016/ra4_macros/variations/sum_rescaled.root");
  TFile *variations = TFile::Open(dataCardPath.c_str());
  std::ofstream file;
  std::string filename("datacard.dat");
  if(includePDFUncert) {
    for(unsigned int i=0; i<100; i++) {
      TString pdf(Form("w_pdf%d", i));
      shapeSysts.push_back(pdf.Data());
    }
  }
  else {
    filename="datacard_nopdf.dat";
  }
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
  outputMCStatisticsSyst(file, bins);

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
      // signal histograms currently do not contain PDF uncertainties
      for(unsigned int index=0; index<nbins; index++) file << "- 0.1 0.1 0.1 0.1 ";
    }
    else {
      for(unsigned int index=0; index<nbins*nprocesses; index++) file << 1.0 << " ";
    }
    file << "\n";
  }
}

// outputs MC statistical uncertainties
void outputMCStatisticsSyst(std::ofstream &file, const std::vector<std::string> &bins)
{
  //  unsigned int nbins=bins.size();
  const unsigned int maxB=4;

  // only signal, qcd, and ttbar have non-negligible MC statistics uncertainties
  std::vector<std::string> samples = {"signal", "qcd", "ttbar"};
  for(auto isample : samples) {
    for(unsigned int ibin = 0; ibin<bins.size(); ibin++) {
      for(unsigned int ibbin=1; ibbin<maxB+1; ibbin++) {
	if(!hasEntry(isample, bins.at(ibin), ibbin)) continue;
	file << "mcstat_" << isample << "_" << bins.at(ibin) << "_nb" << ibbin << " shape ";
	for(unsigned int ientry = 0; ientry<bins.size(); ientry++) {
	  if(ientry == ibin ) {
	    if( isample == "signal") file << "1 - - - - ";
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
  if(sample=="signal") {
    if(bin=="bin0" && nB==3) return false;
    if(bin=="bin2" && nB==4) return false;
    if(bin=="bin3" && nB==4) return false;
    if(bin=="bin5" && nB==4) return false;
  }
  if(sample=="ttbar") {
    if(bin=="bin0" && nB==4) return false;
    if(bin=="bin5" && nB==4) return false;
  }
  if(sample=="qcd") {
    if(bin=="bin5" && nB==3) return false;
    if(bin=="bin5" && nB==4) return false;
  }
  return true;
}
