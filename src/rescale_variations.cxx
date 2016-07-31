// rescales selected nuisance parameters and adds variations corresponding to statistical uncertainties
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>

bool isBlinded(const std::string &binName, const std::vector<std::string>& blindBins);

int main()
{
    // Choose the type of cards to produce: mconly, control, and default
    //    For signal injection studies(mconly), only want to use MC as nuisance parameters
    //    are different for data in sideband regions and MC
    std::string cardType="mconly"; 
    cardType="data"; 

    std::string rootfile("variations/sum_rescaled.root");
    if(cardType=="mconly") rootfile = "variations/sum_rescaled_mconly.root";
    TFile *f = TFile::Open(rootfile.c_str(), "update");

    // samples for which MC statistics should be considered
    std::vector<std::string> mcStatisticsList = {
        "signal_M750", "signal_M1000", "signal_M1100", "signal_M1200", "signal_M1300", "signal_M1400", "signal_M1500", "qcd", "ttbar"};
    // systematics for which the template should be rescaled
    std::vector<std::string> rescaleList = {
        "qcd_flavor", "qcd_mur", "qcd_muf", "qcd_murf",
        "ttbar_pt", "jes", "lep_eff",
        "ttbar_mur", "ttbar_muf", "ttbar_murf"};
    
    // signal list
    std::vector<std::string> signalList = {
        "signal_M750", "signal_M1000", "signal_M1100", "signal_M1200", "signal_M1300", "signal_M1400", "signal_M1500"};
    std::vector<std::string> signalRescaleList = {};
        //"signal_mur", "signal_muf", "signal_murf"};
    std::vector<std::string> upAndDown = {"Up", "Down"}; 

    // Bins
    std::vector<std::string> binNames = { 
        "bin0", "bin1", "bin2",                     // bins for control region fit
        "bin3", "bin4", "bin5",                     // bins for control region fit
        "bin6", "bin7", "bin8", "bin9",             // lower mj bins
        "bin10", "bin11", "bin12",                  // signal bins
        "bin13", "bin14", "bin15","bin16","bin17"}; // signal bins
    std::vector<std::string> blindedBins={};        // bins where data_obs = sum of bkg mc
    if(cardType=="control") blindedBins = {
        "bin10", "bin11", "bin12",
        "bin13", "bin14", "bin15","bin16","bin17"}; 
    else if (cardType=="mconly") blindedBins=binNames; 

    unsigned int nbins=binNames.size();

    // only QCD and ttbar shapes should be rescaled
    // as only these processes have a floating normalization
    // in the fit
    for(unsigned int i=0; i<100; i++) {
        std::string qcd_pdf("qcd_w_pdf");
        qcd_pdf+=std::to_string(i);
        rescaleList.push_back(qcd_pdf);
        std::string ttbar_pdf("ttbar_w_pdf");
        ttbar_pdf+=std::to_string(i);    
        rescaleList.push_back(ttbar_pdf);
        //for(auto isignal : signalList) {
        //    std::string signal_pdf("w_pdf");
        //    signal_pdf+=std::to_string(i);
        //    signalRescaleList.push_back(signal_pdf);
        //}
    }

    for(unsigned int ibin=0; ibin<nbins; ibin++) { 

        // FIXME 
        if(ibin>=6 && ibin<=9) continue; 

        TString binname(binNames.at(ibin).c_str());
        f->cd(binname);
        for(unsigned int isyst=0; isyst<rescaleList.size(); isyst++) {
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
                //rescale->Write("",TObject::kOverwrite);
                rescale->Write();
            }
        }
        // rescale signal systematics
        for(auto isignal : signalList) {
            for(unsigned int isyst=0; isyst<signalRescaleList.size(); isyst++) {
                for(unsigned int idir=0; idir<upAndDown.size(); idir++) {
                    TString histnameNominal(Form("%s/%s", binNames.at(ibin).c_str(), isignal.c_str()));
                    std::cout << "Getting histogram " << histnameNominal << std::endl;
                    TString histnameRescale(Form("%s/%s_%s%s", binNames.at(ibin).c_str(), isignal.c_str(), signalRescaleList.at(isyst).c_str(), upAndDown.at(idir).c_str()));
                    //	  histnameRescale = Form("%s/%s%s", binNames.at(ibin).c_str(), signalRescaleList.at(isyst).c_str(), upAndDown.at(idir).c_str());
                    std::cout << "Getting signal histogram " << histnameRescale << std::endl;
                    TH1F *nominal = static_cast<TH1F*>(f->Get(histnameNominal));
                    TH1F *rescale = static_cast<TH1F*>(f->Get(histnameRescale));
                    if(rescale->Integral()!=0) {
                        rescale->Scale(nominal->Integral()/rescale->Integral());
                    }
                    //rescale->Write("",TObject::kOverwrite);
                    rescale->Write();
                }
            }
        } // end rescaling of signal systematics

        // add histograms for MC statistics so that Barlow-Beeston method can be implemented
        // ignore variations for samples with very small contributions
        // to avoid profusion of useless nuisance parameters
        for(auto isample : mcStatisticsList) {
            TString histnameNominal(Form("%s/%s", binNames.at(ibin).c_str(), isample.c_str()));
            TH1F *nominal = static_cast<TH1F*>(f->Get(histnameNominal));
            for(unsigned int ib=1; ib<=nbins; ib++) {
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
                //up->Write("",TObject::kOverwrite);
                //down->Write("",TObject::kOverwrite);
                up->Write();
                down->Write();
            }
        }

        if(isBlinded(binNames.at(ibin), blindedBins)) {
            // add fake data_obs histogram for blinded bins
            TH1F *data_obs = static_cast<TH1F*>(f->Get(Form("%s/data_obs", binNames.at(ibin).c_str())));
            TH1F *qcd = static_cast<TH1F*>(f->Get(Form("%s/qcd", binNames.at(ibin).c_str())));
            TH1F *ttbar = static_cast<TH1F*>(f->Get(Form("%s/ttbar", binNames.at(ibin).c_str())));
            TH1F *wjets = static_cast<TH1F*>(f->Get(Form("%s/wjets", binNames.at(ibin).c_str())));
            TH1F *other = static_cast<TH1F*>(f->Get(Form("%s/other", binNames.at(ibin).c_str())));
            for(int i=1; i<=data_obs->GetNbinsX(); i++) {
                data_obs->SetBinContent(i, static_cast<int>(qcd->GetBinContent(i)
                            + ttbar->GetBinContent(i)
                            + wjets->GetBinContent(i)
                            + other->GetBinContent(i)));
            }
            //data_obs->Write("",TObject::kOverwrite);
            data_obs->Write();
        }

        // go back to the top left directory to start processing next bin
        f->cd("/");
    }

    f->Write();
    f->Close();

    return 0;
}

bool isBlinded(const std::string &binName, const std::vector<std::string>& blindBins)
{
    for(auto iblind : blindBins) {
        if(binName == iblind) return true;
    }

    return false;
}
