// makes a datacard for RPV analysis with or without PDF uncertainties
#include "TH1.h"
#include "TString.h"
#include "TFile.h"
#include "TList.h"

#include <fstream>
#include <string>

void outputShapeSystematics(std::ofstream &file, const std::vector<std::string> shapeSysts);
void outputLognormalSystematics(std::ofstream &file);

unsigned int nbins;
unsigned int nprocesses;

int main()
{
  bool includePDFUncert = true;

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

  TList *bins = gDirectory->GetListOfKeys();
  nbins = bins->GetSize();
  // hack to set bin number to some other number
  nbins=6;

  // output header
  file << "imax " << nbins << " number of bins" << std::endl;
  file << "jmax " << nprocesses-1 << std::endl;
  file << "kmax * number of nuisance parameters" << std::endl;
  file << "------------------------------------" << std::endl;

  for(unsigned int ibin=0; ibin<nbins; ibin++) {
    file << "shapes * " << bins->At(ibin)->GetName() << " " << dataCardPath << " " << bins->At(ibin)->GetName() 
	 << "/$PROCESS " << bins->At(ibin)->GetName() << "/$PROCESS_$SYSTEMATIC" << std::endl;
  }
  file << "------------------------------------" << std::endl;
  file << "bin          ";

  for(unsigned int ibin=0; ibin<nbins; ibin++) {
    variations->cd(bins->At(ibin)->GetName());
    file << bins->At(ibin)->GetName() << " ";
  }
  file << "\n";
  file << "observation  ";
  for(unsigned int ibin=0; ibin<nbins; ibin++) {
    TString binName(bins->At(ibin)->GetName());
    TH1F *hist = static_cast<TH1F*>(variations->Get(Form("%s/data_obs",binName.Data())));
    file << hist->Integral() << " ";
    hist->Delete();
  }
  file << "\n";
  file << "------------------------------------" << std::endl;
  file << "bin  ";
  for(unsigned int ibin=0; ibin<nbins; ibin++) {
    for(unsigned int iprocess=0; iprocess<nprocesses; iprocess++) {
      file << bins->At(ibin)->GetName() << " ";
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
      TString histName(Form("%s/%s", bins->At(ibin)->GetName(), processes.at(iprocess).c_str()));
      TH1F *hist = static_cast<TH1F*>(variations->Get(histName));
      file << hist->Integral() << "  ";
    }
  }
  file << "\n------------------------------------" << std::endl;

  // output shape systematics
  outputShapeSystematics(file, shapeSysts);
  
  // output lognormal lumi uncertainties for signal, wjets and other
  outputLognormalSystematics(file);

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
