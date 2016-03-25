// rescales selected nuisance parameters and adds variations corresponding to statistical uncertainties
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>

int main()
{
  TFile *f = TFile::Open("variations/sum_rescaled_test.root", "update");

  std::vector<std::string> mcStatisticsList = { "signal", "qcd", "ttbar"};
  std::vector<std::string> rescaleList = {"qcd_flavor", "qcd_mur", "qcd_muf", "qcd_murf",
					  "ttbar_pt", "jes", "lep_eff",
  					  "ttbar_mur", "ttbar_muf", "ttbar_murf"};
  std::vector<std::string> upAndDown = {"Up", "Down"};
  std::vector<std::string> binNames = {"bin0", "bin1", "bin2",
				       "bin3", "bin4", "bin5",
				       "bin6", "bin7", "bin8", "bin9"};
  
  int nbins=binNames.size();
  
  // only QCD and ttbar shapes should be rescaled
  // as only these processes have a floating normalization
  // in the fit
  for(int i=0; i<100; i++) {
    std::string qcd_pdf("qcd_w_pdf");
    qcd_pdf+=std::to_string(i);
    rescaleList.push_back(qcd_pdf);
    std::string ttbar_pdf("ttbar_w_pdf");
    ttbar_pdf+=std::to_string(i);    
    rescaleList.push_back(ttbar_pdf);
  }

  for(int ibin=0; ibin<nbins; ibin++) {
    TString binname(binNames.at(ibin).c_str());
    f->cd(binname);
    // ************************
    // the next line is a hack!
    // ************************
    for(unsigned int isyst=0; isyst<rescaleList.size() && nbins<6; isyst++) {
      //    for(int isyst=0; isyst<rescaleList.size(); isyst++) {
      for(unsigned int idir=0; idir<upAndDown.size(); idir++) {
	std::string process("qcd");
	if(rescaleList.at(isyst).find("ttbar")!=std::string::npos) process="ttbar";
	
	TString histnameNominal(Form("%s/%s", binNames.at(ibin).c_str(), process.c_str()));
	std::cout << "Getting histogram " << histnameNominal << std::endl;
	TString histnameRescale(Form("%s/%s_%s%s", binNames.at(ibin).c_str(), process.c_str(), rescaleList.at(isyst).c_str(), upAndDown.at(idir).c_str()));
	if(rescaleList.at(isyst).find("pdf")!=std::string::npos && 
	   (rescaleList.at(isyst).find("qcd")!=std::string::npos || 
	    rescaleList.at(isyst).find("ttbar")!=std::string::npos) ) {
	  histnameRescale = Form("%s/%s%s", binNames.at(ibin).c_str(), rescaleList.at(isyst).c_str(), upAndDown.at(idir).c_str());
	}
	std::cout << "Getting histogram " << histnameRescale << std::endl;
	TH1F *nominal = static_cast<TH1F*>(f->Get(histnameNominal));
	TH1F *rescale = static_cast<TH1F*>(f->Get(histnameRescale));
	if(rescale->Integral()!=0) {
	  rescale->Scale(nominal->Integral()/rescale->Integral());
	}
	rescale->Write();
      }
    }

    // add histograms for MC statistics so that Barlow-Beeston method can be implemented
    // ignore variations for samples with very small contributions
    // to avoid profusion of useless nuisance parameters
    for(auto isample : mcStatisticsList) {
      TString histnameNominal(Form("%s/%s", binNames.at(ibin).c_str(), isample.c_str()));
      TH1F *nominal = static_cast<TH1F*>(f->Get(histnameNominal));
      for(int ib=1; ib<=nbins; ib++) {
	// only add uncertainties for bins with non-zero entries
	if(nominal->GetBinContent(ib)==0) continue;
	TH1F *up = static_cast<TH1F*>(nominal->Clone());
	TH1F *down = static_cast<TH1F*>(nominal->Clone());
	up->SetBinContent(ib, nominal->GetBinContent(ib)+nominal->GetBinError(ib));
	float downvalue = nominal->GetBinContent(ib)-nominal->GetBinError(ib);
	if(downvalue<0) downvalue=0.0;
	down->SetBinContent(ib, downvalue);

	TString basename(nominal->GetName());
	TString upname(Form("%s_mcstat_%s_%s_nb%dUp", nominal->GetName(), isample.c_str(), binname.Data(), ib-1));
	TString downname(Form("%s_mcstat_%s_%s_nb%dDown", nominal->GetName(), isample.c_str(), binname.Data(), ib-1));
	up->SetName(upname);
	down->SetName(downname);
	up->Write();
	down->Write();
      }
    }

    // go back to the top left directory to start processing next bin
    f->cd("/");
  }

  return 0;
}

