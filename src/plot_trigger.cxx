// plot_trigger: Plots for the trigger section of the RA4 note

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;

void PlotTurnOn(TChain *data, TString var, int nbins, double minx, double maxx, TString xtitle, 
		TString den, TString num, TString title="", TString ytitle="");
void Efficiency(TChain *data, TString den, TString num);

int main(){ 

  TString namestyle("RA4");
  styles style(namestyle); style.yTitleOffset = 1.2;
  style.LabelSize = 0.05;
  style.setDefaultStyle();

  TString htdir("/cms2r0/manuelf/root/july/htmht_met/");
  TString alldir("/cms2r0/manuelf/root/july/all/");
  TChain alldata("tree"), htdata("tree");
  alldata.Add(alldir+"*.root");
  htdata.Add(htdir+"*.root");

  //////////////////////// Leptons ///////////////////////////
  PlotTurnOn(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))", 19,10,200, "Max #mu_{reco} p_{T} [GeV]",
  	     "(trig[0]||trig[12])&&json_dcs", "trig[1]||trig[2]","HT800 || HT350_MET100", 
  	     "Mu15_(HT350_MET70 || HT600)");
  PlotTurnOn(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1))", 19,10,200, "Max e_{reco} p_{T} [GeV]",
  	     "(trig[0]||trig[12])&&json_dcs", "trig[5]||trig[6]","HT800 || HT350_MET100", 
  	     "Ele15_(HT350_MET70 || HT600)");
  PlotTurnOn(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))", 19,10,200, "Max #mu_{reco} p_{T} [GeV]",
  	     "(trig[0]||trig[12])&&json_dcs", "trig[18]","HT800 || HT350_MET100", 
  	     "IsoMu20");
  PlotTurnOn(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1))", 19,10,200, "Max e_{reco} p_{T} [GeV]",
  	     "(trig[0]||trig[12])&&json_dcs", "trig[22]","HT800 || HT350_MET100", 
  	     "Ele27_eta2p1_WPLoose_Gsf");

  cout<<endl<<"Efficiencies for leptons"<<endl;
  Efficiency(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))>30&&(trig[0]||trig[12])&&json_dcs", 
  	     "trig[1]||trig[2]");
  Efficiency(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1))>30&&(trig[0]||trig[12])&&json_dcs", 
  	     "trig[5]||trig[6]");
  Efficiency(&alldata, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))>30&&(trig[0]||trig[12])&&json_dcs", 
  	     "trig[18]");
  Efficiency(&alldata, "Max$(els_pt*(els_sigid&&els_miniso<0.1))>30&&(trig[0]||trig[12])&&json_dcs", 
  	     "trig[22]");
 
  //////////////////////// b-tagging ///////////////////////////
  cout<<endl<<"Efficiencies for b-tagging in muon triggers"<<endl;
  Efficiency(&alldata, "nbl>=1&&(trig[2])&&json_dcs", "trig[3]");
  Efficiency(&alldata, "nbm>=1&&(trig[2])&&json_dcs", "trig[3]");
  Efficiency(&alldata, "nbt>=1&&(trig[2])&&json_dcs", "trig[3]");
  Efficiency(&alldata, "nbl>=2&&(trig[2])&&json_dcs", "trig[3]");
  Efficiency(&alldata, "nbm>=2&&(trig[2])&&json_dcs", "trig[3]");
  Efficiency(&alldata, "nbt>=2&&(trig[2])&&json_dcs", "trig[3]");

  cout<<endl<<"Efficiencies for b-tagging in electron triggers"<<endl;
  Efficiency(&alldata, "nbl>=1&&(trig[6])&&json_dcs", "trig[7]");
  Efficiency(&alldata, "nbm>=1&&(trig[6])&&json_dcs", "trig[7]");
  Efficiency(&alldata, "nbt>=1&&(trig[6])&&json_dcs", "trig[7]");
  Efficiency(&alldata, "nbl>=2&&(trig[6])&&json_dcs", "trig[7]");
  Efficiency(&alldata, "nbm>=2&&(trig[6])&&json_dcs", "trig[7]");
  Efficiency(&alldata, "nbt>=2&&(trig[6])&&json_dcs", "trig[7]");


  /////////////////////////// HT ///////////////////////////
  PlotTurnOn(&alldata, "ht", 20,200,600, "Reco H_{T} [GeV]",
   	     "json_dcs&&(trig[14])&&nmus>=1", "trig[1]","MET170 && n_{#mu,reco} #geq 1",
	     "Mu15_HT350_MET70");
  PlotTurnOn(&alldata, "ht", 20,200,600, "Reco H_{T} [GeV]",
   	     "json_dcs&&(trig[14])&&nels>=1", "trig[5]","MET170 && n_{e,reco} #geq 1",
	     "Ele15_HT350_MET70");
  PlotTurnOn(&alldata, "ht", 20,200,600, "Reco H_{T} [GeV]",
   	     "json_dcs&&(trig[14])", "trig[0]","MET170",
	     "HT350_MET100");


  //////////////////////// MET ///////////////////////////
  PlotTurnOn(&alldata, "met", 15,0,300, "Reco MET [GeV]",
  	     "json_dcs&&(trig[2]||trig[6])", "trig[1]||trig[5]","Mu15_HT600 || Ele15_HT600", 
   	     "(Mu15 || Ele15)_HT350_MET70");
  PlotTurnOn(&alldata, "met", 15,0,300, "Reco MET [GeV]",
  	     "json_dcs&&(trig[2]||trig[6])", "trig[0]","Mu15_HT600 || Ele15_HT600", 
   	     "HT350_MET100");

  cout<<endl<<"Efficiencies for MET"<<endl;
  Efficiency(&alldata, "met>150&&met<200&&(trig[2]||trig[6])&&json_dcs", "trig[1]||trig[5]");
  Efficiency(&alldata, "met>200&&met<300&&(trig[2]||trig[6])&&json_dcs", "trig[1]||trig[5]");
  Efficiency(&alldata, "met>150&&met<200&&(trig[2]||trig[6])&&json_dcs", "trig[0]");
  Efficiency(&alldata, "met>200&&met<300&&(trig[2]||trig[6])&&json_dcs", "trig[0]");


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
  heff.SetMarkerColor(2); heff.SetLineColor(2);
  heff.SetTitle(title);
  heff.GetXaxis()->SetTitle(xtitle);
  heff.GetYaxis()->SetTitle("#epsilon ["+ytitle+"]");
  heff.GetYaxis()->SetRangeUser(0,1.2);
  heff.Draw("ap");

  pname = "plots/turnon_"+format_tag(var)+"_"+format_tag(den)+"_"+format_tag(num)+filetype;
  can.SaveAs(pname);
  
 cout<<"Error bin 1 is "<<"+"<<RoundNumber(heff.GetErrorYhigh(1)*100,1)
     <<"-"<<RoundNumber(heff.GetErrorYlow(1)*100,1)
     <<". Error bin n is "<<"+"<<RoundNumber(heff.GetErrorYhigh(nbins)*100,1)
     <<"-"<<RoundNumber(heff.GetErrorYlow(nbins)*100,1)<<endl;
  for(unsigned his(0); his<2; his++)
    histo[his]->Delete();
}

void Efficiency(TChain *data, TString den, TString num){
  TCanvas can;
  can.SetGrid();
  TH1D* histo[2];
  TString filetype(".eps"), hname, totCut, pname;
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

  den.ReplaceAll("&&json_dcs","");
  if(denom) cout<<"Eff for "<<num<<" and "<<den<<" is "<<RoundNumber(numer*100,1,denom)
		<<"+"<<RoundNumber(heff.GetErrorYhigh(1)*100,1)
		<<"-"<<RoundNumber(heff.GetErrorYlow(1)*100,1)<<" with "<<denom<<" denominator"<<endl;
  else cout<<"Denominator is zero"<<endl;

  for(unsigned his(0); his<2; his++)
    histo[his]->Delete();
}

