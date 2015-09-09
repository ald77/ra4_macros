// plot_trigger: Plots for the trigger section of the RA4 note

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "TChain.h"
#include "TH1D.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLine.h"
#include "TGaxis.h"
#include "TBox.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TString.h"
#include "TStyle.h"
#include "TGraphAsymmErrors.h"
#include "TEfficiency.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace  {
  bool do_si = true;
  bool do_lep = false;
  bool do_dps = false;
  bool do_ht = false;
  bool do_met = false;
  TString plot_type = ".pdf";
}

using namespace std;

Double_t errorFun(Double_t *x, Double_t *par) {
  double value(0.5*par[0]*(1. + TMath::Erf( (x[0] - par[1]) / (sqrt(2.)*par[2]) )));
  return value;
}

void PlotTurnOn(TChain *data, TString var, int nbins, double minx, double maxx, TString xtitle, 
		TString den, TString num, TString title="", TString ytitle="", float minfit=-1., bool isData=true);
TString Efficiency(TChain *data, TString den, TString num, float &effic, float &errup, float &errdown);

int main(){ 

  styles style("HLTStyle"); style.setDefaultStyle();
  gStyle->SetPadTickY(0);

  TString folder("/net/cms2/cms2r0/ald77/archive/2015_08_25/");
  //folder = "root/";

  TChain c_tt("tree"); c_tt.Add("/cms2r0/ald77/archive/2015_08_13/small_quick_TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1_MINIAODSIM_UCSB2650_v82_files20_batch*.root");
  TChain c_jetht("tree"); c_jetht.Add(folder+"*JetHT*root");
  TChain c_met("tree"); c_met.Add(folder+"*MET*root");
  TChain c_mu("tree"); c_mu.Add(folder+"*SingleMuon*");
  TChain c_el("tree"); c_el.Add(folder+"*SingleElectron*");
  TChain c_lep("tree"); c_lep.Add(folder+"*Single*");
  TChain c_all("tree"); c_all.Add(folder+"alldata/*root");

  if(do_si){
    float lmin(25), lmax(300);
    int lbins(static_cast<int>((lmax-lmin)/12.5));

    // TM turn-ons IsoMu20
    lmin = 0; lmax = 100; lbins = static_cast<int>((lmax-lmin)/2);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2&&mus_tru_tm))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium, miniso} p_{T}","1", "trig[18]","TM medium #mu, I_{mini} < 0.2", 
	       "IsoMu20",-50,false);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_reliso<0.12&&mus_tru_tm))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium, reliso} p_{T}","1", "trig[18]","TM medium #mu, I_{R=0.4} < 0.12", 
	       "IsoMu20",-50,false);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_reliso<0.2&&mus_tru_tm))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium, reliso} p_{T}","1", "trig[18]","TM medium #mu, I_{R=0.4} < 0.2", 
	       "IsoMu20",-50,false);

    float effic, errup, errdown;
    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_miniso<0.2))>50","trig[18]",effic, errup, errdown);
    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_miniso<0.2))>50&&ht>500","trig[18]",effic, errup, errdown);
    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_miniso<0.2))>50&&ht>1000","trig[18]",effic, errup, errdown);
    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_miniso<0.2))>50&&ht>1500","trig[18]",effic, errup, errdown);

    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.2))>50","trig[18]",effic, errup, errdown);
    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.2))>50&&ht>500","trig[18]",effic, errup, errdown);
    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.2))>50&&ht>1000","trig[18]",effic, errup, errdown);
    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.2))>50&&ht>1500","trig[18]",effic, errup, errdown);

    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.12))>50","trig[18]",effic, errup, errdown);
    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.12))>50&&ht>500","trig[18]",effic, errup, errdown);
    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.12))>50&&ht>1000","trig[18]",effic, errup, errdown);
    Efficiency(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.12))>50&&ht>1500","trig[18]",effic, errup, errdown);

    float htmin(175), htmax(1000);
    int htbins(static_cast<int>((htmax-htmin)/12.5));
    htmin = 0; htmax = 2400; htbins = static_cast<int>((htmax-htmin)/200);
    PlotTurnOn(&c_tt, "ht_reliso", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.12))>50", 
	       "trig[18]","n_{#mu,50,reliso} #geq 1", "IsoMu20", -500,false);
    PlotTurnOn(&c_tt, "ht", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_miniso<0.2))>50", 
	       "trig[18]","n_{#mu,50,miniso} #geq 1", "IsoMu20", -500,false);
    PlotTurnOn(&c_tt, "ht_reliso", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.2))>50", 
	       "trig[18]","n_{#mu,50,reliso} #geq 1", "IsoMu20", -500,false);


  }

  if(do_lep){
    float lmin(25), lmax(300);
    int lbins(static_cast<int>((lmax-lmin)/12.5));

    // Truth-matched plateaus
    lmin = 0; lmax = 300; lbins = static_cast<int>((lmax-lmin)/20);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_reliso<0.12&&mus_tru_tm))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium, reliso} p_{T}","1", "trig[18]","TM medium #mu, I_{R=0.4} < 0.12", "IsoMu20",-40,false);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_reliso<0.2&&mus_tru_tm))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium, reliso} p_{T}","1", "trig[18]","TM medium #mu, I_{R=0.4} < 0.2", "IsoMu20",-40,false);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2&&mus_tru_tm))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium, miniso} p_{T}","1", "trig[18]","TM medium #mu, I_{mini} < 0.2", "IsoMu20",-40,false);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_reliso<0.12&&mus_tru_tm))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium, reliso} p_{T}","1", "trig[21]","TM medium #mu, I_{R=0.4} < 0.12", "Mu50",-60,false);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2&&mus_tru_tm))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium, miniso} p_{T}","1", "trig[21]","TM medium #mu, I_{mini} < 0.2", "Mu50",-60,false);

    PlotTurnOn(&c_tt, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&els_tru_tm&&abs(els_eta)<1.4))-0.1", 
	       lbins,lmin,lmax, "e_{medium}^{|#eta|<1.4} p_{T}","1", "trig[24]",
	       "TM medium e, I_{mini} < 0.1", "Ele105",-120,false);
    PlotTurnOn(&c_tt, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&els_tru_tm&&abs(els_eta)>1.5))-0.1", 
	       lbins,lmin,lmax, "e_{medium}^{|#eta|>1.5} p_{T}","1", "trig[24]",
	       "TM medium e, I_{mini} < 0.1", "Ele105",-120,false);

    // Truth-matched turn-ons
    lmin = 5; lmax = 50; lbins = static_cast<int>((lmax-lmin)/1);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_reliso<0.12&&mus_tru_tm))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium, reliso} p_{T}","1", "trig[18]","TM medium #mu, I_{R=0.4} < 0.12", 
	       "IsoMu20",-1,false);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_reliso<0.2&&mus_tru_tm))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium, reliso} p_{T}","1", "trig[18]","TM medium #mu, I_{R=0.4} < 0.2", 
	       "IsoMu20",-1,false);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2&&mus_tru_tm))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium, reliso} p_{T}","1", "trig[18]","TM medium #mu, I_{mini} < 0.2", 
	       "IsoMu20",-1,false);
    lmin = 25; lmax = 100; lbins = static_cast<int>((lmax-lmin)/1);
    PlotTurnOn(&c_tt, "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, 
	       "#mu_{medium} p_{T}","1", "trig[21]","TM medium #mu, I_{mini} < 0.2", "Mu50",48,false);
    lmin = 70; lmax = 160; lbins = static_cast<int>((lmax-lmin)/2.5);
    PlotTurnOn(&c_tt, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&els_tru_tm&&abs(els_eta)<1.4))-0.1", 
	       lbins,lmin,lmax, "e_{medium}^{|#eta|<1.4} p_{T}",
    	       "1", "trig[24]","TM medium e, I_{mini} < 0.1", "Ele105",-1,false);
    PlotTurnOn(&c_tt, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&els_tru_tm&&abs(els_eta)>1.5))-0.1", 
	       lbins,lmin,lmax, "e_{medium}^{|#eta|>1.5} p_{T}",
    	       "1", "trig[24]","TM medium e, I_{mini} < 0.1", "Ele105",-1,false);


    // Turn-ons Ele105/Mu50
    lmin = 60; lmax = 160; lbins = static_cast<int>((lmax-lmin)/5);
    PlotTurnOn(&c_all, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&abs(els_eta)<1.4))-0.1", lbins,lmin,lmax, 
    	       "e_{medium}^{|#eta|<1.4} p_{T}",
    	       "(trig[0]||trig[12])", "trig[24]","HT800 || HT350_MET100", "Ele105");
    PlotTurnOn(&c_all, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&abs(els_eta)>1.5))-0.1", lbins,lmin,lmax, 
    	       "e_{medium}^{|#eta|>1.5} p_{T}",
    	       "(trig[0]||trig[12])", "trig[24]","HT800 || HT350_MET100", "Ele105");

    lmin = 0; lmax = 100; lbins = static_cast<int>((lmax-lmin)/2.5);
    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, "#mu_{medium} p_{T}",
    	       "(trig[0]||trig[12])", "trig[21]","HT800 || HT350_MET100", "Mu50");

    // Turn-ons IsoMu20, VVVL
    lmin = 0; lmax = 60; lbins = static_cast<int>((lmax-lmin)/2.5);
    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, "#mu_{medium} p_{T}",
    	       "(trig[0]||trig[12])", "trig[18]","HT800 || HT350_MET100", "IsoMu20");
    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, "#mu_{medium} p_{T}",
    	       "(trig[0]||trig[12])", "trig[1]||trig[2]","HT800 || HT350_MET100", "Mu15_(HT350_MET70 || HT600)");
    PlotTurnOn(&c_all, "Max$(els_pt*(els_sigid&&els_miniso<0.1))-0.1", lbins,lmin,lmax, "e_{medium} p_{T}",
    	       "(trig[0]||trig[12])", "trig[5]||trig[6]","HT800 || HT350_MET100", "Ele15_(HT350_MET70 || HT600)");

    // Plateaus
    lmin = 0; lmax = 300; lbins = static_cast<int>((lmax-lmin)/20);
    PlotTurnOn(&c_el, "Max$(mus_pt*(mus_tight&&mus_reliso<0.12))-0.1", lbins,lmin,lmax, 
    	       "#mu_{tight} p_{T}",
    	       "(trig[22]||trig[26])", "trig[18]","Ele27 || Ele24_22", "IsoMu20");
    PlotTurnOn(&c_el, "Max$(mus_pt*(mus_tight&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, 
    	       "#mu_{tight} p_{T}",
    	       "(trig[22]||trig[26])", "trig[18]","Ele27 || Ele24_22", "IsoMu20");
    PlotTurnOn(&c_el, "Max$(mus_pt*(mus_tight&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, 
    	       "#mu_{tight} p_{T}",
    	       "(trig[22]||trig[26])", "trig[1]||trig[2]","Ele27 || Ele24_22", "Mu15_(HT350_MET70 || HT600)");
    PlotTurnOn(&c_el, "Max$(mus_pt*(mus_tight&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, 
    	       "#mu_{tight} p_{T}",
    	       "(trig[22]||trig[26])", "trig[21]","Ele27 || Ele24_22", "Mu50");

    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_tight&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, 
    	       "#mu_{tight} p_{T}",
    	       "(trig[0]||trig[12])", "trig[18]","HT800 || HT350_MET100", "IsoMu20");
    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_tight&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, 
    	       "#mu_{tight} p_{T}",
    	       "(trig[0]||trig[12])", "trig[1]||trig[2]","HT800 || HT350_MET100", "Mu15_(HT350_MET70 || HT600)");
    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_tight&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, 
    	       "#mu_{tight} p_{T}",
    	       "(trig[0]||trig[12])", "trig[21]","HT800 || HT350_MET100", "Mu50");

    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium} p_{T}",
    	       "(trig[0]||trig[12])", "trig[18]","HT800 || HT350_MET100", "IsoMu20");
    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium} p_{T}",
    	       "(trig[0]||trig[12])", "trig[1]||trig[2]","HT800 || HT350_MET100", "Mu15_(HT350_MET70 || HT600)");
    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium} p_{T}",
    	       "(trig[0]||trig[12])", "trig[21]","HT800 || HT350_MET100", "Mu50");

    lmin = 0; lmax = 300; lbins = static_cast<int>((lmax-lmin)/20);
    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2&&abs(mus_eta)<1.4))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium}^{|#eta|<1.4} p_{T}",
    	       "(trig[0]||trig[12])", "trig[1]||trig[2]","HT800 || HT350_MET100", "Mu15_(HT350_MET70 || HT600)");
    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2&&abs(mus_eta)>1.5))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium}^{|#eta|>1.5} p_{T}",
    	       "(trig[0]||trig[12])", "trig[1]||trig[2]","HT800 || HT350_MET100", "Mu15_(HT350_MET70 || HT600)");

    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2&&abs(mus_eta)<1.4))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium}^{|#eta|<1.4} p_{T}",
    	       "(trig[0]||trig[12])", "trig[21]","HT800 || HT350_MET100", 
	       "Mu50");
    PlotTurnOn(&c_all, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2&&abs(mus_eta)>1.5))-0.1", lbins,lmin,lmax, 
    	       "#mu_{medium}^{|#eta|>1.5} p_{T}",
    	       "(trig[0]||trig[12])", "trig[21]","HT800 || HT350_MET100", 
	       "Mu50");

    PlotTurnOn(&c_all, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&abs(els_eta)<1.4))-0.1", lbins,lmin,lmax, 
    	       "e_{medium}^{|#eta|<1.4} p_{T}",
    	       "(trig[0]||trig[12])", "trig[5]||trig[6]","HT800 || HT350_MET100", "Ele15_(HT350_MET70 || HT600)");
    PlotTurnOn(&c_all, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&abs(els_eta)>1.5))-0.1", lbins,lmin,lmax, 
    	       "e_{medium}^{|#eta|>1.5} p_{T}",
    	       "(trig[0]||trig[12])", "trig[5]||trig[6]","HT800 || HT350_MET100", "Ele15_(HT350_MET70 || HT600)");

    PlotTurnOn(&c_all, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&abs(els_eta)<1.4))-0.1", lbins,lmin,lmax, 
    	       "e_{medium}^{|#eta|<1.4} p_{T}",
    	       "(trig[0]||trig[12])", "trig[24]","HT800 || HT350_MET100", 
	       "Ele105");
    PlotTurnOn(&c_all, "Max$(els_pt*(els_sigid&&els_miniso<0.1&&abs(els_eta)>1.5))-0.1", lbins,lmin,lmax, 
    	       "e_{medium}^{|#eta|>1.5} p_{T}",
    	       "(trig[0]||trig[12])", "trig[24]","HT800 || HT350_MET100", 
	       "Ele105");

    float htmin(175), htmax(1000);
    int htbins(static_cast<int>((htmax-htmin)/12.5));
    htmin = 0; htmax = 2400; htbins = static_cast<int>((htmax-htmin)/200);
    PlotTurnOn(&c_tt, "ht_reliso", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.12))>40", 
	       "trig[18]","n_{#mu,40,reliso} #geq 1", "IsoMu20", -500,false);
    PlotTurnOn(&c_tt, "ht_reliso", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.12))>25", 
	       "trig[18]","n_{#mu,25,reliso} #geq 1", "IsoMu20", -500,false);
    PlotTurnOn(&c_tt, "ht", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_miniso<0.2))>25", 
	       "trig[18]","n_{#mu,25,miniso} #geq 1", "IsoMu20", -500,false);
    PlotTurnOn(&c_tt, "ht_reliso", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.2))>25", 
	       "trig[18]","n_{#mu,25,reliso} #geq 1", "IsoMu20", -500,false);

    PlotTurnOn(&c_tt, "ht", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm))>25", 
	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_miniso<0.2))>25","n_{#mu,25} #geq 1", 
	       "n_{#mu,25,miniso} #geq 1", -500,false);
    PlotTurnOn(&c_tt, "ht_reliso", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm))>25", 
	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.2))>25","n_{#mu,25} #geq 1", 
	       "n_{#mu,25,reliso} #geq 1", -500,false);
    PlotTurnOn(&c_tt, "ht_reliso", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm))>25", 
	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.12))>25","n_{#mu,25} #geq 1", 
	       "n_{#mu,25,reliso} #geq 1", -500,false);
    PlotTurnOn(&c_tt, "ht", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm))>25", 
	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_miniso<0.2))>25&&trig[18]","n_{#mu,25} #geq 1", 
	       "n_{#mu,25,miniso} #geq 1, IsoMu20", -500,false);
    PlotTurnOn(&c_tt, "ht_reliso", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm))>25", 
	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.2))>25&&trig[18]","n_{#mu,25} #geq 1", 
	       "n_{#mu,25,reliso} #geq 1, IsoMu20", -500,false);
    PlotTurnOn(&c_tt, "ht_reliso", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm))>25", 
	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.12))>25&&trig[18]","n_{#mu,25} #geq 1", 
	       "n_{#mu,25,reliso} #geq 1, IsoMu20", -500,false);
    PlotTurnOn(&c_tt, "ht_reliso", htbins,htmin,htmax, "H_{T}",
    	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm))>40", 
	       "Max$(mus_pt*(mus_sigid&&mus_tru_tm&&mus_reliso<0.12))>40&&trig[18]","n_{#mu,40} #geq 1", 
	       "n_{#mu,40,reliso} #geq 1, IsoMu20", -500,false);

    float ltmin(0), ltmax(600);
    int ltbins(static_cast<int>((ltmax-ltmin)/25));
    PlotTurnOn(&c_jetht, "met_nohf+Sum$(mus_pt*(mus_sigid&&mus_miniso<0.2))", ltbins, ltmin, ltmax, "L_{T}",
	       "Sum$((mus_pt>25&&mus_sigid&&mus_miniso<0.2))==1&&ht>500", "trig[1]||trig[21]",
	       "JetHT, n_{#mu}^{25} = 1, H_{T} > 500","Mu15_HT350_MET70 || Mu50");

    PlotTurnOn(&c_jetht, "met_nohf+Sum$(els_pt*(els_sigid&&els_miniso<0.1))", ltbins, ltmin, ltmax, "L_{T}",
	       "Sum$((els_pt>25&&els_sigid&&els_miniso<0.1))==1&&ht>500", "trig[5]||trig[24]",
	       "JetHT, n_{e}^{25} = 1, H_{T} > 500","Ele15_HT350_MET70 || Ele105");
  } // if(do_lep)


  if(do_dps){
    float metmin(0), metmax(460);
    int metbins(static_cast<int>((metmax-metmin)/20));
    TString vvvl_or("(HT400_Btag || HT600)");
    PlotTurnOn(&c_all, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[22]||trig[10])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets_ra2b>=4&&ht_ra2b>500&&onht>350", "trig[0]",      
    	       "Ele27 || DoubleEle8, n_{j} #geq 4, H_{T} > 500", "HT350_MET100");
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[6]||trig[7])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[5]",
    	       "Ele15_"+vvvl_or+", n_{j} #geq 4", "Ele15_HT350_MET70");
    PlotTurnOn(&c_all, "mht_ra2b", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[22]||trig[10])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets_ra2b>=4&&ht_ra2b>500&&onht>350", "trig[0]",
    	       "Ele27 || DoubleEle8, n_{j} #geq 4, H_{T} > 500", "HT350_MET100");


    float htmin(175), htmax(850);
    int htbins(static_cast<int>((htmax-htmin)/12.5));
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "trig[14]&&nvleps==0", "trig[0]","MET170, no leptons", "HT350_MET100");
    htmin = 575; htmax = 1450; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_jetht, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "trig[11]&&nvleps==0", "trig[12]","HT475, no leptons", "HT800");
    htmin = 125; htmax = 850; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "(trig[13])&&(nmus)>=1&&onmaxmu>15", "trig[1]","MET120_Mu5, n_{#mu} #geq 1",
    	       "Mu15_HT350_MET70");
    htmin = 375; htmax = 1250; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_el, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "(trig[5])", "trig[6]","Ele15_HT350_MET70", "Ele15_HT600");

  }

  ///////////////////////////////////////// MET /////////////////////////////////////////
  if(do_met){
    float metmin(0), metmax(500);
    int metbins(static_cast<int>((metmax-metmin)/20));
    TString vvvl_or("(HT400_Btag || HT600)");

    // MET, njets >= 4
    vvvl_or = "HT400_Btag";
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[7])&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_el, "mht_ra2b", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[7])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    // MET, njets >= 4
    vvvl_or = "HT600";
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[6])&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_el, "mht_ra2b", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[6])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_all, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[22]||trig[26])&&njets_ra2b>=4&&ht_ra2b>500", "trig[0]",    	       
	       "(Ele27 || Ele24_22), n_{j}#geq4, H_{T}>500", "HT350_MET100");
    PlotTurnOn(&c_all, "mht_ra2b", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[22]||trig[26])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets_ra2b>=4&&ht_ra2b>500", "trig[0]",
    	       "(Ele27 || Ele24_22), n_{j}#geq4, H_{T}>500", "HT350_MET100");


    // MET, no cuts
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[6]||trig[7])", "trig[0]", "Ele15_"+vvvl_or+"", "HT350_MET100");
    PlotTurnOn(&c_mu, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[2]||trig[3])", "trig[0]",  "Mu15_"+vvvl_or+"", "HT350_MET100");
    // MET, njets >= 4
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[6]||trig[7])&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_mu, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[2]||trig[3])&&njets>=4", "trig[0]",
    	       "Mu15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_lep, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[2]||trig[3]||trig[6]||trig[7])&&njets>=4", "trig[0]",
    	       "Lep15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");

    // MET no HF, njets >= 4
    PlotTurnOn(&c_el, "met_nohf", metbins,metmin,metmax, "E_{T}^{miss} no HF",
    	       "(trig[6]||trig[7])&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_mu, "met_nohf", metbins,metmin,metmax, "E_{T}^{miss} no HF",
    	       "(trig[2]||trig[3])&&njets>=4", "trig[0]",
    	       "Mu15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_lep, "met_nohf", metbins,metmin,metmax, "E_{T}^{miss} no HF",
    	       "(trig[2]||trig[3]||trig[6]||trig[7])&&njets>=4", "trig[0]",
    	       "Lep15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");

    // MHT, njets >= 4
    PlotTurnOn(&c_el, "mht_ra2b", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[6]||trig[7])&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_el, "mht_ra2b", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[6]||trig[7])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_mu, "mht_ra2b", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[2]||trig[3])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[0]",
    	       "Mu15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_lep, "mht_ra2b", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[2]||trig[3]||trig[6]||trig[7])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[0]",
    	       "Lep15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");

    // MET70, njets >= 4
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[6]||trig[7])&&njets>=4", "trig[5]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "Ele15_HT350_MET70");
    PlotTurnOn(&c_mu, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[2]||trig[3])&&njets>=4", "trig[1]",
    	       "Mu15_"+vvvl_or+", n_{j}#geq4", "Mu15_HT350_MET70");
    PlotTurnOn(&c_lep, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[2]||trig[3]||trig[6]||trig[7])&&njets>=4", "trig[1]||trig[5]",
    	       "Lep15_"+vvvl_or+", n_{j}#geq4", "Lep15_HT350_MET70");
    PlotTurnOn(&c_el, "mht_ra2b", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[6]||trig[7])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[5]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "Ele15_HT350_MET70");
    PlotTurnOn(&c_mu, "mht_ra2b", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[2]||trig[3])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[1]",
    	       "Mu15_"+vvvl_or+", n_{j}#geq4", "Mu15_HT350_MET70");
    PlotTurnOn(&c_lep, "mht_ra2b", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[2]||trig[3]||trig[6]||trig[7])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", 
	       "trig[1]||trig[5]", "Lep15_"+vvvl_or+", n_{j}#geq4", "Lep15_HT350_MET70");
  }

  ///////////////////////////////////////// HT /////////////////////////////////////////
 
  if(do_ht){
    float htmin(175), htmax(1000);
    int htbins(static_cast<int>((htmax-htmin)/12.5));
    PlotTurnOn(&c_met, "ht", htbins,htmin,htmax, "H_{T}",
    	       "trig[14]&&nvleps==0", "trig[0]","MET170, no leptons", "HT350_MET100");
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "trig[14]&&nvleps==0", "trig[0]","MET170, no leptons", "HT350_MET100");
    PlotTurnOn(&c_met, "ht", htbins,htmin,htmax, "H_{T}",
    	       "trig[14]&&nvleps==0", "trig[0]","MET170, no leptons", "HT350_MET100",350);
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "trig[14]&&nvleps==0", "trig[0]","MET170, no leptons", "HT350_MET100",350);

    htmin = 600; htmax = 1600; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_jetht, "ht", htbins,htmin,htmax, "H_{T}",
    	       "trig[11]&&nvleps==0", "trig[12]","HT475, no leptons", "HT800");
    PlotTurnOn(&c_jetht, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "trig[11]&&nvleps==0", "trig[12]","HT475, no leptons", "HT800");
    PlotTurnOn(&c_jetht, "ht", htbins,htmin,htmax, "H_{T}",
    	       "trig[11]&&nvleps==0", "trig[12]","HT475, no leptons", "HT800",825);
    PlotTurnOn(&c_jetht, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "trig[11]&&nvleps==0", "trig[12]","HT475, no leptons", "HT800",825);


    htmin = 125; htmax = 1000; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "(trig[14])&&nels>=1&&onmaxel>15", "trig[5]","MET170, n_{e} #geq 1",
    	       "Ele15_HT350_MET70");
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "(trig[13])&&(nmus)>=1&&onmaxmu>15", "trig[1]","MET120_Mu5, n_{#mu} #geq 1",
    	       "Mu15_HT350_MET70");
    htmin = 400; htmax = 1300; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_mu, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "(trig[1])", "trig[2]","Mu15_HT350_MET70", "Mu15_HT600");
    PlotTurnOn(&c_el, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "(trig[5])", "trig[6]","Ele15_HT350_MET70", "Ele15_HT600");
    PlotTurnOn(&c_mu, "ht", htbins,htmin,htmax, "H_{T}",
    	       "(trig[1])", "trig[2]","Mu15_HT350_MET70", "Mu15_HT600");
    PlotTurnOn(&c_el, "ht", htbins,htmin,htmax, "H_{T}",
    	       "(trig[5])", "trig[6]","Ele15_HT350_MET70", "Ele15_HT600");
    PlotTurnOn(&c_mu, "ht", htbins,htmin,htmax, "H_{T}",
    	       "(trig[1])", "trig[2]","Mu15_HT350_MET70", "Mu15_HT600",600);
    PlotTurnOn(&c_el, "ht", htbins,htmin,htmax, "H_{T}",
    	       "(trig[5])", "trig[6]","Ele15_HT350_MET70", "Ele15_HT600",600);


  } // if(do_ht)

}

void PlotTurnOn(TChain *data, TString var, int nbins, double minx, double maxx, TString xtitle, 
		TString den, TString num, TString title, TString ytitle, float minfit, bool isData){
  styles style("HLTStyle"); gStyle->SetPadTickY(0);
  bool dofit(minfit>=-1);
  TCanvas can;
  //can.SetGrid();
  TH1D* histo[2];
  TString hname, totCut, pname;
  den = "("+den+")&&json_golden&&pass"; 
  hname = "den"; totCut = den;
  histo[0] = new TH1D(hname, "", nbins, minx, maxx);
  data->Project(hname, var, totCut);

  hname = "num"; totCut = "("+den+")&&("+num+")";
  histo[1] = new TH1D(hname, "", nbins, minx, maxx);
  data->Project(hname, var, totCut);

  // Adding overflow bins
  for(unsigned his(0); his<2; his++)
    histo[his]->SetBinContent(nbins, histo[his]->GetBinContent(nbins)+histo[his]->GetBinContent(nbins+1));

  TGraphAsymmErrors heff(histo[1], histo[0], "cp");
  //TEfficiency heff(*histo[1], *histo[0]);
  heff.SetMarkerStyle(20); heff.SetMarkerSize(0.9);
  heff.SetMarkerColor(1); heff.SetLineColor(1);

  TString epsi("#scale[1.3]{#font[122]{e}}");
  //epsi = "Efficiency";
  // Ploting denominator
  float hscaled(0.3), maxeff(1.42);
  float hfactor(hscaled/histo[1]->GetMaximum()), hmax(histo[1]->GetMaximum());
  float axismax(hmax*maxeff/hscaled);
  histo[1]->Scale(hfactor);
  //histo[1]->SetFillStyle(3344);
  histo[1]->SetFillColor(kBlue-9);
  histo[1]->SetLineStyle(0);
  //histo[1]->SetTitle("Denom: "+title);
  histo[1]->GetXaxis()->SetTitle("Offline "+xtitle+" [GeV]");
  histo[1]->GetYaxis()->SetTitle(epsi+"  ["+ytitle+"]");
  histo[1]->GetYaxis()->SetTitle("Efficiency  ["+ytitle+"]");
  histo[1]->GetYaxis()->SetRangeUser(0,maxeff);
  histo[1]->GetYaxis()->CenterTitle(true);
  histo[1]->Draw();

  TLine line; line.SetLineStyle(3);
  line.DrawLine(minx, 1, maxx, 1);

  histo[0]->Scale(hfactor);
  histo[0]->SetLineColor(kBlue+2);
  histo[0]->SetLineStyle(2);
  histo[0]->SetLineWidth(2);
  histo[0]->Draw("same");

  pname = "plots/turnon_"+format_tag(var)+"_";
  pname += maxx; pname += "_"+format_tag(num)+"_"+format_tag(den);
  if(minfit>0) {pname += "_min"; pname += minfit; }
  if(isData) pname += "_data";
  else pname += "_mc";
  pname += plot_type;
  pname.ReplaceAll("_json_golden_pass","");

  
  // Fitting turn on curve
  if(minfit<0 && dofit) minfit = minx;
  TF1 *fitCurve = new TF1("fitCurve",errorFun,minx,maxx,3);
  Double_t params[] = {0.99,minx+(maxx-minx)/4.,50., 50.};    
  //Double_t params[] = {0.99,880,50.};    
  fitCurve->SetParameters(params);
  fitCurve->SetParNames("#epsilon","#mu","#sigma");
  fitCurve->SetLineColor(2);
  fitCurve->SetLineWidth(2);
  if(dofit) heff.Fit("fitCurve","QEM+","",minfit, maxx);
  
  heff.Draw("p");
  histo[1]->Draw("axis same");

  float binw((maxx-minx)/static_cast<float>(nbins));
  int digits((binw-floor(binw))>0?1:0);
  
  TString ntitle("Events/("+RoundNumber(maxx-minx,digits,nbins)+" GeV)");
  TGaxis *axis = new TGaxis(maxx,0, maxx, maxeff,0,axismax,508,"+L");
  axis->SetLineColor(kBlue+2);
  axis->SetTextColor(kBlue+2); axis->SetLabelColor(kBlue+2);
  axis->SetTextFont(style.nFont); axis->SetLabelFont(style.nFont); 
  axis->SetTitleSize(style.LabelSize); axis->SetLabelSize(style.LabelSize); 
  if(axismax>=1000) axis->SetTitleOffset(style.yTitleOffset+0.34);
  else axis->SetTitleOffset(style.yTitleOffset+0.22);
  axis->SetTitle(ntitle); axis->CenterTitle(true);
  axis->Draw();


  float effic, errup, errdown;
  float mu(fitCurve->GetParameter(1)), sigma(fitCurve->GetParameter(2));
  float rgev(mu>200?10:5);
  float var_plateau_f(floor((mu+3*sigma+5)/rgev)*rgev);
  if(!dofit) var_plateau_f = fabs(minfit);
  TString den_plateau(den), var_plateau(RoundNumber(var_plateau_f, 0));
  den_plateau += ("&&"+var+">"+var_plateau);
  Efficiency(data, den_plateau, num, effic, errup, errdown);

  // 98th percentile of Gaussian from Wolfram Alpha
  float p98(fitCurve->GetParameter(1)+2.05*fitCurve->GetParameter(2));
  float eplateau(fitCurve->GetParError(0)*100);
  if(eplateau<0.1 && dofit) cout<<"Error on plateau "<<eplateau<<"%"<<endl;
  epsi.ToLower();
  // TString fitpar("Plateau "+epsi+"  = ("+RoundNumber(fitCurve->GetParameter(0)*100,1)+" #pm "+
  // 		 RoundNumber(eplateau,1)+")%");
  // TString fitpar("Plateau "+epsi+"  = "+RoundNumber(effic*100,1)+"^{+"+RoundNumber(errup*100,1)+
  // 		 "}_{-"+RoundNumber(errdown*100,1)+"} %");
  TString fitpar(epsi+"("+xtitle+" > "+var_plateau+" GeV) = "+RoundNumber(effic*100,1)+"^{+"+RoundNumber(errup*100,1)+
		 "}_{-"+RoundNumber(errdown*100,1)+"} %");
  TLatex label; label.SetTextSize(0.042); 
  label.SetTextAlign(33); //label.SetNDC(); 
  float range(maxx-minx);
  float x2(maxx-0.04*range), y2(maxeff-0.07), ysingle(0.1);
  label.DrawLatex(x2, y2, "Denom: #font[52]{"+title+"}");
  label.DrawLatex(x2, y2-ysingle,fitpar);
  fitpar = "98% of plateau at "+RoundNumber(p98,0)+" GeV";
  if(dofit) label.DrawLatex(x2, y2-2.3*ysingle,fitpar);

  // Drawing CMS preliminary
  label.SetNDC();  label.SetTextAlign(11); label.SetTextSize(0.045); 
  if(isData) label.DrawLatex(0.13, 0.93, "#font[61]{CMS} #scale[0.8]{#font[52]{Preliminary}}");
  else label.DrawLatex(0.13, 0.93, "#font[61]{CMS} #scale[0.8]{#font[52]{Simulation}}");
  // Drawing luminosity
  label.SetTextAlign(31); 
  if(isData) label.DrawLatex(0.85, 0.93, "42 pb^{-1} (13 TeV)");
  else label.DrawLatex(0.85, 0.93, "Spring15 t#bar{t}");

  can.SaveAs(pname);
  
  for(unsigned his(0); his<2; his++)
    histo[his]->Delete();
  fitCurve->Delete();
}

TString Efficiency(TChain *data, TString den, TString num, float &effic, float &errup, float &errdown){
  TH1D* histo[2];
  TString hname, totCut, pname;
  den = "("+den+")&&json_golden&&pass";
  hname = "eden"; totCut = den;
  histo[0] = new TH1D(hname, "", 1, 0, 1);
  float denom(data->GetEntries(totCut));
  histo[0]->SetBinContent(1,denom);

  hname = "enum"; totCut = "("+den+")&&("+num+")";
  histo[1] = new TH1D(hname, "", 1, 0, 1);
  float numer(data->GetEntries(totCut));
  histo[1]->SetBinContent(1,numer);

  TGraphAsymmErrors heff(histo[1], histo[0], "cp");
  //TEfficiency heff(*histo[1], *histo[0]);

  effic = numer/denom;
  errup = heff.GetErrorYhigh(0); errdown = heff.GetErrorYlow(0);
  //float errup(heff.GetEfficiencyErrorUp(0)), errdown(heff.GetEfficiencyErrorLow(0));

  den.ReplaceAll("&&json_golden","");
  if(denom) cout<<endl<<"Eff = "<<RoundNumber(numer*100,1,denom)<<"+"<<RoundNumber(errup*100,1)
  		<<"-"<<RoundNumber(errdown*100,1)<<" for num "<<num<<" and "<<den<<" with "<<denom<<" entries"<<endl;
  else cout<<"Denominator is zero"<<endl;
  
  TString efficiency(RoundNumber(numer*100,1,denom)+"^{+"+RoundNumber(errup*100,1)+
		     "}_{-"+RoundNumber(errdown*100,1)+"}");

  for(unsigned his(0); his<2; his++)
    histo[his]->Delete();

  return efficiency;

}

