// plot_trigger: Plots for the trigger section of the RA4 note

#include <iostream>
#include <fstream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TBox.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TString.h"
#include "TStyle.h"
#include "TGraphAsymmErrors.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace  {
  bool do_leptons = false;
  bool do_ht = false;
  bool do_met = true;
}

using namespace std;

Double_t errorFun(Double_t *x, Double_t *par) {
  return 0.5*par[0]*(1. + TMath::Erf( (x[0] - par[1]) / (sqrt(2.)*par[2]) )) ;
}

void PlotTurnOn(TChain *data, TString var, int nbins, double minx, double maxx, TString xtitle, 
		TString den, TString num, TString title="", TString ytitle="");
TString Efficiency(TChain *data, TString den, TString num);

int main(){ 
  TString namestyle("RA4");
  styles style(namestyle); style.yTitleOffset = 1.2;
  style.LabelSize = 0.05;
  style.setDefaultStyle();

  TString alldir("/cms2r0/ald77/archive/2015_07_26/alldata/");
  alldir = "/cms2r0/ald77/archive/2015_08-03/alldata/skim_json/";
  TChain alldata("tree");
  alldata.Add(alldir+"*.root");
  TChain mu_jack("reduced_tree"), el_jack("reduced_tree"), jack("reduced_tree");
  mu_jack.Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/SingleMuon_Run2015B-PromptReco-v1_MINIAOD_UCSB2466_v80/SingleMuon_Run2015B-PromptReco-v1_MINIAOD_UCSB2466_v80_defaultJEC.root");
  el_jack.Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/SingleElectron_Run2015B-PromptReco-v1_MINIAOD_UCSB2470_v80/SingleElectron_Run2015B-PromptReco-v1_MINIAOD_UCSB2470_v80_defaultJEC.root");
  jack.Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/SingleMuon_Run2015B-PromptReco-v1_MINIAOD_UCSB2466_v80/SingleMuon_Run2015B-PromptReco-v1_MINIAOD_UCSB2466_v80_defaultJEC.root");
  jack.Add("/cms8r0/johnbr/2014/csa14/reduced_trees/13TeV/SingleElectron_Run2015B-PromptReco-v1_MINIAOD_UCSB2470_v80/SingleElectron_Run2015B-PromptReco-v1_MINIAOD_UCSB2470_v80_defaultJEC.root");

  if(do_met){
    // Table comparing MET/MHT efficiencies
    TString metcut("(trig[2]||trig[3]||trig[6]||trig[7])&&abs(mht_ra2b-met)<150");
    TString outname("txt/trig_met_mht.tex");
    ifstream header("txt/header.tex");
    ifstream footer("txt/footer.tex");
    ofstream file(outname);
    file << header.rdbuf();
    file << "\\vspace{80 mm}\n";
    file << "\n\\begin{tabular}[tbp!]{ l | ccc | ccc}\\hline\\hline\n";
    file << "\\multirow{2}{c}{MET/MHT $\\epsilon$ [\\%]}&\\multicolumn{3}{c|}{MET} & \\multicolumn{3}{c}{MHT} \\\\ \n";
    file << " & 150-200 & 200-300 & 300+ & 150-200 & 200-300 & 300+\\\\ \\hline\n";
    file << "Lep15\\_HT350\\_MET70 & $";
    file << Efficiency(&alldata, metcut+"&&met>150&&met<=200", "trig[1]||trig[5]")<<"$ & $";
    file << Efficiency(&alldata, metcut+"&&met>200&&met<=300", "trig[1]||trig[5]")<<"$ & $";
    file << Efficiency(&alldata, metcut+"&&met>300", "trig[1]||trig[5]")<<"$ & $";
    file << Efficiency(&alldata, metcut+"&&mht_ra2b>150&&mht_ra2b<=200", "trig[1]||trig[5]")<<"$ & $";
    file << Efficiency(&alldata, metcut+"&&mht_ra2b>200&&mht_ra2b<=300", "trig[1]||trig[5]")<<"$ & $";
    file << Efficiency(&alldata, metcut+"&&mht_ra2b>300", "trig[1]||trig[5]")<<"$ \\\\ \n";
    file << "HT350\\_MET100 & $";
    file << Efficiency(&alldata, metcut+"&&met>150&&met<=200", "trig[0]")<<"$ & $";
    file << Efficiency(&alldata, metcut+"&&met>200&&met<=300", "trig[0]")<<"$ & $";
    file << Efficiency(&alldata, metcut+"&&met>300", "trig[0]")<<"$ & $";
    file << Efficiency(&alldata, metcut+"&&mht_ra2b>150&&mht_ra2b<=200", "trig[0]")<<"$ & $";
    file << Efficiency(&alldata, metcut+"&&mht_ra2b>200&&mht_ra2b<=300", "trig[0]")<<"$ & $";
    file << Efficiency(&alldata, metcut+"&&mht_ra2b>300", "trig[0]")<<"$ \\\\ \n";

    file << "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
    file << footer.rdbuf();
    file.close();
    cout<<"Written "<<outname<<endl;

    ///////////////////////////////////////// MET /////////////////////////////////////////
    // Plottting MET-MHT resolution
    gStyle->SetOptStat("emr");  // Entries, Mean, RMS
    TCanvas can;
    int bins(100);
    TH1D hmet("hmet", "(Lep15_HT400_Btag || Lep15_HT600) && GoodJets", bins, -200, 200);
    hmet.SetLineColor(2); hmet.SetLineWidth(3); 
    hmet.SetXTitle("MHT - MET [GeV]"); hmet.SetYTitle("Events in 40 pb^{-1}");
    alldata.Project("hmet", "mht_ra2b-met", "(trig[2]||trig[3]||trig[6]||trig[7])");
    hmet.SetBinContent(1, hmet.GetBinContent(0)+hmet.GetBinContent(1));
    hmet.SetBinContent(bins, hmet.GetBinContent(bins)+hmet.GetBinContent(bins+1));
    hmet.Draw();
    can.SaveAs("plots/resolution_mht_gjets.eps");
    alldata.Project("hmet", "mht_ra2b-met", "(trig[2]||trig[3]||trig[6]||trig[7])&&trig[0]");
    hmet.SetBinContent(1, hmet.GetBinContent(0)+hmet.GetBinContent(1));
    hmet.SetBinContent(bins, hmet.GetBinContent(bins)+hmet.GetBinContent(bins+1));
    hmet.SetTitle("(Lep15_HT400_Btag || Lep15_HT600) && HT350_MET100 && GoodJets");
    hmet.Draw();
    can.SaveAs("plots/resolution_mht_trig0_gjets.eps");

    jack.Project("hmet", "mht30-met", "(HLT_Ele15_IsoVVVL_PFHT600||HLT_Mu15_IsoVVVL_PFHT600)&&passesJSONCut&&passesBadJetFilter");
    //jack.Project("hmet", "mht30-met", "(HLT_Ele15_IsoVVVL_PFHT600||HLT_Mu15_IsoVVVL_PFHT600)&&passesJSONCut");
    hmet.SetBinContent(1, hmet.GetBinContent(0)+hmet.GetBinContent(1));
    hmet.SetBinContent(bins, hmet.GetBinContent(bins)+hmet.GetBinContent(bins+1));
    hmet.SetTitle("(Lep15_HT400_Btag || Lep15_HT600) && GoodJets");
    hmet.Draw();
    can.SaveAs("plots/resolution_mht_jack_gjets.eps");
    jack.Project("hmet", "mht30-met", "(HLT_Ele15_IsoVVVL_PFHT600||HLT_Mu15_IsoVVVL_PFHT600)&&passesJSONCut&&passesBadJetFilter&&HLT_PFHT350_PFMET100_NoiseCleaned");
    //jack.Project("hmet", "mht30-met", "(HLT_Ele15_IsoVVVL_PFHT600||HLT_Mu15_IsoVVVL_PFHT600)&&passesJSONCut&&HLT_PFHT350_PFMET100_NoiseCleaned");
    hmet.SetBinContent(1, hmet.GetBinContent(0)+hmet.GetBinContent(1));
    hmet.SetBinContent(bins, hmet.GetBinContent(bins)+hmet.GetBinContent(bins+1));
    hmet.SetTitle("(Lep15_HT400_Btag || Lep15_HT600) && HT350_MET100 && GoodJets");
    hmet.Draw();
    can.SaveAs("plots/resolution_mht_trig0_jack_gjets.eps");
    gStyle->SetOptStat(0);  // No stat box


    float metmin(0), metmax(500);
    int metbins(static_cast<int>((metmax-metmin)/12.5));

    PlotTurnOn(&alldata, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(trig[2]||trig[3]||trig[6]||trig[7])&&abs(mht_ra2b-met)<150", "trig[0]",
	       "Lep15_HT400_Btag || Lep15_HT600", 
	       "HT350_MET100");
    PlotTurnOn(&alldata, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(trig[2]||trig[3]||trig[6]||trig[7])&&abs(mht_ra2b-met)<150", "trig[1]||trig[5]",
	       "Lep15_HT400_Btag || Lep15_HT600", 
	       "Lep15_HT350_MET70");

    PlotTurnOn(&alldata, "mht_ra2b", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "(trig[2]||trig[3]||trig[6]||trig[7])&&abs(mht_ra2b-met)<150", "trig[1]||trig[5]",
	       "Lep15_HT400_Btag || Lep15_HT600", 
	       "Lep15_HT350_MET70");
    PlotTurnOn(&alldata, "mht_ra2b", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "(trig[2]||trig[3]||trig[6]||trig[7])&&abs(mht_ra2b-met)<150", "trig[0]",
	       "Lep15_HT400_Btag || Lep15_HT600", 
	       "HT350_MET100");

    // Best MET
    PlotTurnOn(&alldata, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(trig[2]||trig[3]||trig[6]||trig[7])", "trig[0]",
	       "Lep15_HT400_Btag || Lep15_HT600", 
	       "HT350_MET100");
    PlotTurnOn(&alldata, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(trig[2]||trig[3]||trig[6]||trig[7])", "trig[1]||trig[5]",
	       "Lep15_HT400_Btag || Lep15_HT600", 
	       "Lep15_HT350_MET70");

    PlotTurnOn(&alldata, "mht_ra2b", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "(trig[2]||trig[3]||trig[6]||trig[7])", "trig[0]",
	       "Lep15_HT400_Btag || Lep15_HT600", 
	       "HT350_MET100");
    PlotTurnOn(&alldata, "mht_ra2b", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "(trig[2]||trig[3]||trig[6]||trig[7])", "trig[1]||trig[5]",
	       "Lep15_HT400_Btag || Lep15_HT600", 
	       "Lep15_HT350_MET70");

    // Comparison with Jack
    PlotTurnOn(&jack, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(HLT_Ele15_IsoVVVL_PFHT600||HLT_Mu15_IsoVVVL_PFHT600)&&passesJSONCut&&passesBadJetFilter", 
	       "HLT_Ele15_IsoVVVL_PFHT350_PFMET70||HLT_Mu15_IsoVVVL_PFHT350_PFMET70", "Lep15_HT600", "Lep15_HT350_MET70");
    PlotTurnOn(&jack, "mht30", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "(HLT_Ele15_IsoVVVL_PFHT600||HLT_Mu15_IsoVVVL_PFHT600)&&passesJSONCut&&passesBadJetFilter", 
	       "HLT_Ele15_IsoVVVL_PFHT350_PFMET70||HLT_Mu15_IsoVVVL_PFHT350_PFMET70", "Lep15_HT600", "Lep15_HT350_MET70");
    PlotTurnOn(&jack, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(HLT_Ele15_IsoVVVL_PFHT600||HLT_Mu15_IsoVVVL_PFHT600)&&passesJSONCut&&passesBadJetFilter", 
	       "HLT_PFHT350_PFMET100_NoiseCleaned", "Lep15_HT600", "HT350_MET100");
    PlotTurnOn(&jack, "mht30", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "(HLT_Ele15_IsoVVVL_PFHT600||HLT_Mu15_IsoVVVL_PFHT600)&&passesJSONCut&&passesBadJetFilter", 
	       "HLT_PFHT350_PFMET100_NoiseCleaned", "Lep15_HT600", "HT350_MET100");
    PlotTurnOn(&el_jack, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "HLT_Ele15_IsoVVVL_PFHT600&&passesJSONCut&&passesBadJetFilter", "HLT_PFHT350_PFMET100_NoiseCleaned",
	       "Ele15_HT600", "HT350_MET100");
    PlotTurnOn(&el_jack, "mht30", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "HLT_Ele15_IsoVVVL_PFHT600&&passesJSONCut&&passesBadJetFilter", "HLT_PFHT350_PFMET100_NoiseCleaned",
	       "Ele15_HT600", "HT350_MET100");
    PlotTurnOn(&mu_jack, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "HLT_Mu15_IsoVVVL_PFHT600&&passesJSONCut&&passesBadJetFilter", "HLT_PFHT350_PFMET100_NoiseCleaned",
	       "Mu15_HT600", "HT350_MET100");
    PlotTurnOn(&mu_jack, "mht30", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "HLT_Mu15_IsoVVVL_PFHT600&&passesJSONCut&&passesBadJetFilter", "HLT_PFHT350_PFMET100_NoiseCleaned",
	       "Mu15_HT600", "HT350_MET100");

    PlotTurnOn(&alldata, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(trig[2])", "trig[0]",
	       "Mu15_HT600", "HT350_MET100");
    PlotTurnOn(&alldata, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(trig[3])", "trig[0]",
	       "Mu15_HT400_Btag", "HT350_MET100");
    PlotTurnOn(&alldata, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(trig[5])", "trig[0]",
	       "Ele15_HT600", "HT350_MET100");
    PlotTurnOn(&alldata, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(trig[6])", "trig[0]",
	       "Ele15_HT400_Btag", "HT350_MET100");

    PlotTurnOn(&alldata, "mht_ra2b", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "(trig[2])", "trig[0]",
	       "Mu15_HT600", "HT350_MET100");
    PlotTurnOn(&alldata, "mht_ra2b", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "(trig[3])", "trig[0]",
	       "Mu15_HT400_Btag", "HT350_MET100");
    PlotTurnOn(&alldata, "mht_ra2b", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "(trig[5])", "trig[0]",
	       "Ele15_HT600", "HT350_MET100");
    PlotTurnOn(&alldata, "mht_ra2b", metbins,metmin,metmax, "Reco MHT [GeV]",
	       "(trig[6])", "trig[0]",
	       "Ele15_HT400_Btag", "HT350_MET100");

    // MET with HT>500, njets>3
    PlotTurnOn(&alldata, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(trig[2]||trig[3]||trig[6]||trig[7])&&ht_ra2b>500&&njets_ra2b>3", "trig[0]",
	       "Lep15_HT400_Btag || Lep15_HT600, H_{T} > 500, n_{jets} > 3", 
	       "HT350_MET100");
    PlotTurnOn(&alldata, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(trig[2]||trig[3]||trig[6]||trig[7])&&ht_ra2b>500&&njets_ra2b>3", "trig[1]||trig[5]",
	       "Lep15_HT400_Btag || Lep15_HT600, H_{T} > 500, n_{jets} > 3", 
	       "Lep15_HT350_MET70");

    // MET with other triggers
    PlotTurnOn(&alldata, "met", metbins,metmin,metmax, "Reco MET [GeV]",
	       "(trig[22]||trig[10]||trig[9]||trig[18])&&ht_ra2b>500&&njets_ra2b>3", "trig[0]",
	       "(IsoMu20 || Ele27 || DoubleLep8), H_{T} > 500, n_{jets} > 3", 
	       "HT350_MET100");

    PlotTurnOn(&alldata, "met", metbins/2,metmin,metmax, "Reco MET [GeV]",
	       "(trig[22]||trig[10])&&ht_ra2b>500&&njets_ra2b>3", "trig[0]",
	       "(Ele27 || DoubleEle8), H_{T} > 500, n_{jets} > 3", 
	       "HT350_MET100");
    PlotTurnOn(&alldata, "mht_ra2b", metbins/2,metmin,metmax, "Reco MHT [GeV]",
	       "(trig[22]||trig[10])&&ht_ra2b>500&&njets_ra2b>3", "trig[0]",
	       "(Ele27 || DoubleEle8), H_{T} > 500, n_{jets} > 3", 
	       "HT350_MET100");

  } // if(do_met)



  ///////////////////////////////////////// HT /////////////////////////////////////////
  if(do_ht){
    float htmin(175), htmax(1000);
    int htbins(static_cast<int>((htmax-htmin)/12.5));
    PlotTurnOn(&alldata, "ht_ra2b", htbins,htmin+150,htmax+150, "Reco H_{T} [GeV]",
	       "(trig[1])", "trig[2]","Mu15_HT350_MET70",
	       "Mu15_HT600");
    PlotTurnOn(&alldata, "ht_ra2b", htbins,htmin+150,htmax+150, "Reco H_{T} [GeV]",
	       "(trig[1]||trig[5])", "trig[2]||trig[6]","Lep15_HT350_MET70",
	       "Lep15_HT600");
    PlotTurnOn(&alldata, "ht_ra2b", htbins,htmin,htmax, "Reco H_{T} [GeV]",
	       "(trig[14])&&(nvmus+nvels)==0", "trig[0]","MET170, no leptons",
	       "HT350_MET100");
  } // if(do_ht)

  ///////////////////////////////////// LEPTONS ////////////////////////////////////////
  if(do_leptons){
    float lmin(10), lmax(60);
    int lbins(static_cast<int>((lmax-lmin)/1.25));

    PlotTurnOn(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))", lbins,lmin,lmax, 
	       "Max Medium #mu_{reco} p_{T} [GeV]",
	       "(trig[0]||trig[12])", "trig[1]||trig[2]","HT800 || HT350_MET100", "Mu15_(HT350_MET70 || HT600)");
    PlotTurnOn(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1))", lbins,lmin,lmax, 
	       "Max Medium e_{reco} p_{T} [GeV]",
	       "(trig[0]||trig[12])", "trig[5]||trig[6]","HT800 || HT350_MET100", "Ele15_(HT350_MET70 || HT600)");
    PlotTurnOn(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))", lbins,lmin,lmax, 
	       "Max Medium #mu_{reco} p_{T} [GeV]",
	       "(trig[0]||trig[12])", "trig[18]","HT800 || HT350_MET100", "IsoMu20");
    PlotTurnOn(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1))", lbins,lmin,lmax, 
	       "Max Medium e_{reco} p_{T} [GeV]",
	       "(trig[0]||trig[12])", "trig[22]","HT800 || HT350_MET100", "Ele27_eta2p1_WPLoose_Gsf");
    PlotTurnOn(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&abs(els_eta)<2.1))", lbins,lmin,lmax, 
	       "Max Medium e_{reco}^{|#eta|<2.1} p_{T} [GeV]",
	       "(trig[0]||trig[12])", "trig[5]||trig[6]","HT800 || HT350_MET100", "Ele15_(HT350_MET70 || HT600)");
    PlotTurnOn(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&abs(els_eta)<2.1))", lbins,lmin,lmax, 
	       "Max Medium e_{reco}^{|#eta|<2.1} p_{T} [GeV]",
	       "(trig[0]||trig[12])", "trig[22]","HT800 || HT350_MET100", "Ele27_eta2p1_WPLoose_Gsf");

    PlotTurnOn(&alldata, "Max$(mus_pt*(mus_miniso<0.2))", lbins,lmin,lmax, "Max Loose #mu_{reco} p_{T} [GeV]",
	       "(trig[0]||trig[12])", "trig[1]||trig[2]","HT800 || HT350_MET100", 
	       "Mu15_(HT350_MET70 || HT600)");
    PlotTurnOn(&alldata, "Max$(els_pt*(els_miniso<0.1))", lbins,lmin,lmax, "Max Veto e_{reco} p_{T} [GeV]",
	       "(trig[0]||trig[12])", "trig[5]||trig[6]","HT800 || HT350_MET100", 
	       "Ele15_(HT350_MET70 || HT600)");
    PlotTurnOn(&alldata, "Max$(mus_pt*(mus_miniso<0.2))", lbins,lmin,lmax, "Max Loose #mu_{reco} p_{T} [GeV]",
	       "(trig[0]||trig[12])", "trig[18]","HT800 || HT350_MET100", 
	       "IsoMu20");
    PlotTurnOn(&alldata, "Max$(els_pt*(els_miniso<0.1))", lbins,lmin,lmax, "Max Veto e_{reco} p_{T} [GeV]",
	       "(trig[0]||trig[12])", "trig[22]","HT800 || HT350_MET100", 
	       "Ele27_eta2p1_WPLoose_Gsf");
    PlotTurnOn(&alldata, "Max$(els_pt*(els_miniso<0.1&&abs(els_eta)<2.1))", lbins,lmin,lmax, 
	       "Max Veto e_{reco} p_{T}^{|#eta|<2.1} [GeV]",
	       "(trig[0]||trig[12])", "trig[5]||trig[6]","HT800 || HT350_MET100", "Ele15_(HT350_MET70 || HT600)");
    PlotTurnOn(&alldata, "Max$(els_pt*(els_miniso<0.1&&abs(els_eta)<2.1))", lbins,lmin,lmax, 
	       "Max Veto e_{reco}^{|#eta|<2.1} p_{T} [GeV]",
	       "(trig[0]||trig[12])", "trig[22]","HT800 || HT350_MET100", "Ele27_eta2p1_WPLoose_Gsf");

  } // if(do_leptons)


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////


  // //////////////////////// Leptons ///////////////////////////
  // PlotTurnOn(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))", 19,10,200, "Max #mu_{reco} p_{T} [GeV]",
  // 	     "(trig[0]||trig[12])", "trig[1]||trig[2]||trig[21]","HT800 || HT350_MET100", 
  // 	     "Mu15_(HT350_MET70 || HT600) || Mu50");
  // PlotTurnOn(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))", 19,10,200, "Max #mu_{reco} p_{T} [GeV]",
  // 	     "(trig[0]||trig[12])", "trig[1]||trig[2]","HT800 || HT350_MET100", 
  // 	     "Mu15_(HT350_MET70 || HT600)");
  // PlotTurnOn(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1))", 19,10,200, "Max e_{reco} p_{T} [GeV]",
  // 	     "(trig[0]||trig[12])", "trig[5]||trig[6]","HT800 || HT350_MET100", 
  // 	     "Ele15_(HT350_MET70 || HT600)");
  // PlotTurnOn(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))", 19,10,200, "Max #mu_{reco} p_{T} [GeV]",
  // 	     "(trig[0]||trig[12])", "trig[18]","HT800 || HT350_MET100", 
  // 	     "IsoMu20");
  // PlotTurnOn(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1))", 19,10,200, "Max e_{reco} p_{T} [GeV]",
  // 	     "(trig[0]||trig[12])", "trig[22]","HT800 || HT350_MET100", 
  // 	     "Ele27_eta2p1_WPLoose_Gsf");

  // cout<<endl<<"Efficiencies for leptons"<<endl;
  // Efficiency(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))>30&&(trig[0]||trig[12])", 
  // 	     "trig[1]||trig[2]");
  // Efficiency(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))>30&&(trig[0]||trig[12])", 
  // 	     "trig[1]||trig[2]||trig[21]");
  // Efficiency(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1))>30&&(trig[0]||trig[12])", 
  // 	     "trig[5]||trig[6]");
  // Efficiency(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))>30&&(trig[0]||trig[12])", 
  // 	     "trig[18]");
  // Efficiency(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1))>30&&(trig[0]||trig[12])", 
  // 	     "trig[22]");
 
  // //////////////////////// b-tagging ///////////////////////////
  // cout<<endl<<"Efficiencies for b-tagging in muon triggers"<<endl;
  // Efficiency(&alldata, "nbl>=1&&(trig[2])", "trig[3]");
  // Efficiency(&alldata, "nbm>=1&&(trig[2])", "trig[3]");
  // Efficiency(&alldata, "nbt>=1&&(trig[2])", "trig[3]");
  // Efficiency(&alldata, "nbl>=2&&(trig[2])", "trig[3]");
  // Efficiency(&alldata, "nbm>=2&&(trig[2])", "trig[3]");
  // Efficiency(&alldata, "nbt>=2&&(trig[2])", "trig[3]");

  // cout<<endl<<"Efficiencies for b-tagging in electron triggers"<<endl;
  // Efficiency(&alldata, "nbl>=1&&(trig[6])", "trig[7]");
  // Efficiency(&alldata, "nbm>=1&&(trig[6])", "trig[7]");
  // Efficiency(&alldata, "nbt>=1&&(trig[6])", "trig[7]");
  // Efficiency(&alldata, "nbl>=2&&(trig[6])", "trig[7]");
  // Efficiency(&alldata, "nbm>=2&&(trig[6])", "trig[7]");
  // Efficiency(&alldata, "nbt>=2&&(trig[6])", "trig[7]");


  // /////////////////////////// HT ///////////////////////////
  // PlotTurnOn(&alldata, "ht", 20,200,700, "Reco H_{T} [GeV]",
  //  	     "(trig[14])&&nmus>=1", "trig[1]","MET170 && n_{#mu,reco} #geq 1",
  // 	     "Mu15_HT350_MET70");
  // PlotTurnOn(&alldata, "ht", 20,200,700, "Reco H_{T} [GeV]",
  //  	     "(trig[14])&&nels>=1", "trig[5]","MET170 && n_{e,reco} #geq 1",
  // 	     "Ele15_HT350_MET70");
  // PlotTurnOn(&alldata, "ht", 20,200,700, "Reco H_{T} [GeV]",
  //  	     "(trig[14])", "trig[0]","MET170",
  // 	     "HT350_MET100");
  // PlotTurnOn(&alldata, "ht_hlt", 20,200,700, "Reco H_{T}^{HLT} [GeV]",
  //  	     "(trig[14])", "trig[0]","MET170",
  // 	     "HT350_MET100");


  // //////////////////////// MET ///////////////////////////
  // PlotTurnOn(&alldata, "met", 20,0,500, "Reco MET [GeV]",
  // 	     "(trig[2]||trig[6])", "trig[1]||trig[5]","Mu15_HT600 || Ele15_HT600", 
  //  	     "(Mu15 || Ele15)_HT350_MET70");
  // PlotTurnOn(&alldata, "met", 20,0,500, "Reco MET [GeV]",
  // 	     "(trig[2]||trig[6])", "trig[0]","Mu15_HT600 || Ele15_HT600", 
  //  	     "HT350_MET100");
  // PlotTurnOn(&alldata, "mht_ra2b", 20,0,500, "Reco MHT [GeV]",
  // 	     "(trig[2]||trig[6])", "trig[1]||trig[5]","Mu15_HT600 || Ele15_HT600", 
  //  	     "(Mu15 || Ele15)_HT350_MET70");
  // PlotTurnOn(&alldata, "mht_ra2b", 20,0,500, "Reco MHT [GeV]",
  // 	     "(trig[2]||trig[6])", "trig[0]","Mu15_HT600 || Ele15_HT600", 
  //  	     "HT350_MET100");

  // PlotTurnOn(&alldata, "mht_ra2b", 20,0,500, "Reco MHT [GeV]",
  //  	     "(trig[22]&&ht>500&&njets_ra2b>=4)", "trig[0]","Ele27 && H_{T} > 500 && n_{jets} #geq 4", 
  //   	     "HT350_MET100");
  // PlotTurnOn(&alldata, "met", 20,0,500, "Reco MET [GeV]",
  // 	     "(trig[22]&&ht>500&&njets_ra2b>=4)", "trig[0]","Ele27 && H_{T} > 500 && n_{jets} #geq 4", 
  //  	     "HT350_MET100");

  // PlotTurnOn(&alldata, "mht_ra2b", 20,0,500, "Reco MHT [GeV]",
  //  	     "(trig[22]&&ht>500)", "trig[0]","Ele27 && H_{T} > 500", 
  //   	     "HT350_MET100");
  // PlotTurnOn(&alldata, "met", 20,0,500, "Reco MET [GeV]",
  // 	     "(trig[22]&&ht>500)", "trig[0]","Ele27 && H_{T} > 500", 
  //  	     "HT350_MET100");

  // cout<<endl<<"Efficiencies for MET"<<endl;
  // Efficiency(&alldata, "met>150&&met<200&&(trig[2]||trig[6])", "trig[1]||trig[5]");
  // Efficiency(&alldata, "met>200&&met<300&&(trig[2]||trig[6])", "trig[1]||trig[5]");
  // Efficiency(&alldata, "met>150&&met<200&&(trig[2]||trig[6])", "trig[0]");
  // Efficiency(&alldata, "met>200&&met<300&&(trig[2]||trig[6])", "trig[0]");


}

TString format_tag(TString tag){
  tag.ReplaceAll(".","");
  tag.ReplaceAll("(",""); tag.ReplaceAll("$","");  tag.ReplaceAll(")","");
  tag.ReplaceAll("[",""); tag.ReplaceAll("]",""); tag.ReplaceAll("||","_");
  tag.ReplaceAll("/","_"); tag.ReplaceAll("*",""); tag.ReplaceAll("&&","_");
  tag.ReplaceAll(">=","ge"); tag.ReplaceAll("<=","se");
  tag.ReplaceAll(">","g"); tag.ReplaceAll("<","s"); tag.ReplaceAll("=","");
  tag.ReplaceAll("+",""); tag.ReplaceAll("&","");
  tag.ReplaceAll("!","not");
  tag.ReplaceAll("#",""); tag.ReplaceAll("{",""); tag.ReplaceAll("}","");

  return tag;
}

void PlotTurnOn(TChain *data, TString var, int nbins, double minx, double maxx, TString xtitle, 
		TString den, TString num, TString title, TString ytitle){
  TCanvas can;
  can.SetGrid();
  TH1D* histo[2];
  TString filetype(".eps"), hname, totCut, pname;
  // den = "("+den+")&&json_golden&&pass_jets"; // Already in the skim
  hname = "den"; totCut = den;
  histo[0] = new TH1D(hname, "", nbins, minx, maxx);
  data->Project(hname, var, totCut);

  hname = "num"; totCut = "("+den+")&&("+num+")";
  histo[1] = new TH1D(hname, "", nbins, minx, maxx);
  data->Project(hname, var, totCut);
   for(unsigned his(0); his<2; his++)
     histo[his]->SetBinContent(nbins, histo[his]->GetBinContent(nbins)+histo[his]->GetBinContent(nbins+1));

  TGraphAsymmErrors heff;
  heff = TGraphAsymmErrors(histo[1], histo[0]);
  heff.SetMarkerStyle(20); heff.SetMarkerSize(0.9);
  heff.SetMarkerColor(4); heff.SetLineColor(4);


  // Ploting denominator
  float hfactor(0.3/histo[1]->GetMaximum());
  histo[1]->Scale(hfactor);
  //histo[1]->SetFillStyle(3344);
  histo[1]->SetFillColor(kGray);
  histo[1]->SetLineStyle(0);
  histo[1]->SetTitle("Denom: "+title);
  histo[1]->GetXaxis()->SetTitle(xtitle);
  histo[1]->GetYaxis()->SetTitle("#epsilon ["+ytitle+"]");
  histo[1]->GetYaxis()->SetRangeUser(0,1.3);
  histo[1]->Draw();

  histo[0]->Scale(hfactor);
  histo[0]->SetLineColor(kGray+1);
  histo[0]->SetLineStyle(2);
  histo[0]->SetLineWidth(2);
  histo[0]->Draw("same");


  
  // Fitting turn on curve
  TF1 *fitCurve = new TF1("fitCurve",errorFun,minx,maxx,3);
  Double_t params[3] = {0.98,maxx/3.,40.};    
  fitCurve->SetParameters(params);
  fitCurve->SetParNames("#epsilon","#mu","#sigma");
  fitCurve->SetLineColor(2);
  fitCurve->SetLineWidth(2);
  heff.Fit("fitCurve","QR+");
  
  heff.Draw("p");
  histo[1]->Draw("axis same");

  // 95th percentile of Gaussian from Wolfram Alpha
  float p95(fitCurve->GetParameter(1)+1.64485*fitCurve->GetParameter(2));
  float eplateau(fitCurve->GetParError(0)*100);
  if(eplateau<0.1) cout<<"Error on plateau "<<eplateau<<"%"<<endl;
  TString fitpar("#splitline{Plateau #epsilon = ("+
		 RoundNumber(fitCurve->GetParameter(0)*100,1)+" #pm "+
		 RoundNumber(eplateau,1)+
		 ")%}{95% of plateau at "+RoundNumber(p95,0)+" GeV}");
  TLatex label; label.SetTextSize(0.045); 
  label.SetTextAlign(33); //label.SetNDC(); 
  float range(maxx-minx);
  float x1(maxx-0.57*range), y1(1.08), x2(maxx-0.05*range), y2(1.25);
  TBox box(x1,y1,x2,y2); box.SetFillColor(10); box.SetFillStyle(1001); 
  box.Draw();
  label.DrawLatex(x2-0.01*range,y2-0.01,fitpar);

  // TPaveText label(x1,y1,x2,y2,"NDC NB"); label.SetTextSize(0.045); 
  // label.SetFillColor(10); label.SetFillStyle(1001); 
  // label.SetTextAlign(33); label.AddText(fitpar);
  // label.Draw();

  pname = "plots/turnon_"+format_tag(var)+"_"+format_tag(den)+"_"+format_tag(num)+filetype;
  can.SaveAs(pname);
  
 // cout<<"Error bin 1 is "<<"+"<<RoundNumber(heff.GetErrorYhigh(1)*100,1)
 //     <<"-"<<RoundNumber(heff.GetErrorYlow(1)*100,1)
 //     <<". Error bin n is "<<"+"<<RoundNumber(heff.GetErrorYhigh(nbins)*100,1)
 //     <<"-"<<RoundNumber(heff.GetErrorYlow(nbins)*100,1)<<endl;
  for(unsigned his(0); his<2; his++)
    histo[his]->Delete();
  fitCurve->Delete();
}

TString Efficiency(TChain *data, TString den, TString num){
  TCanvas can;
  can.SetGrid();
  TH1D* histo[2];
  TString filetype(".eps"), hname, totCut, pname;
  //den = "("+den+")&&json_golden";
  hname = "den"; totCut = den;
  histo[0] = new TH1D(hname, "", 1, 0, 1);
  float denom(data->GetEntries(totCut));
  histo[0]->SetBinContent(1,denom);

  hname = "num"; totCut = "("+den+")&&("+num+")";
  histo[1] = new TH1D(hname, "", 1, 0, 1);
  float numer(data->GetEntries(totCut));
  histo[1]->SetBinContent(1,numer);

  TGraphAsymmErrors heff;
  heff = TGraphAsymmErrors(histo[1], histo[0]);

  den.ReplaceAll("&&json_golden","");
  if(denom) cout<<"Eff for "<<num<<" and "<<den<<" is "<<RoundNumber(numer*100,1,denom)
		<<"+"<<RoundNumber(heff.GetErrorYhigh(0)*100,1)
		<<"-"<<RoundNumber(heff.GetErrorYlow(0)*100,1)<<" with "<<denom<<" denominator"<<endl;
  else cout<<"Denominator is zero"<<endl;
  
  TString efficiency(RoundNumber(numer*100,1,denom)+"^{+"+RoundNumber(heff.GetErrorYhigh(0)*100,1)+
		     "}_{-"+RoundNumber(heff.GetErrorYlow(0)*100,1)+"}");

  for(unsigned his(0); his<2; his++)
    histo[his]->Delete();

  return efficiency;

}

