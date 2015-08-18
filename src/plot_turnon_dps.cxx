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
#include "TEfficiency.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace  {
  bool do_ht = true;
  bool do_met = false;
  TString plot_type = ".pdf";
}

using namespace std;

Double_t errorFun(Double_t *x, Double_t *par) {
  double value(0.5*par[0]*(1. + TMath::Erf( (x[0] - par[1]) / (sqrt(2.)*par[2]) )));
  // if(x[0] > par[1]) value = 0.5*par[0]*(1. + TMath::Erf( (x[0] - par[1]) / (sqrt(2.)*par[2]) ));
  // else value = 0.5*par[0]*(1. + TMath::Erf( (x[0] - par[1]) / (sqrt(2.)*par[3]) ));
  return value;
}

void PlotTurnOn(TChain *data, TString var, int nbins, double minx, double maxx, TString xtitle, 
		TString den, TString num, TString title="", TString ytitle="", float minfit=-1.);
TString Efficiency(TChain *data, TString den, TString num);

int main(){ 
  styles style("HLTStyle"); 
  style.setDefaultStyle();

  TString folder("/net/cms2/cms2r0/ald77/archive/2015_08_17/");

  TChain c_jetht("tree"); c_jetht.Add(folder+"*JetHT*root");
  TChain c_met("tree"); c_met.Add(folder+"*MET*root");
  TChain c_mu("tree"); c_mu.Add(folder+"*SingleMuon*");
  TChain c_el("tree"); c_el.Add(folder+"*SingleElectron*");
  TChain c_lep("tree"); c_lep.Add(folder+"*Single*");


   ///////////////////////////////////////// MET /////////////////////////////////////////
  if(do_met){
    float metmin(0), metmax(500);
    int metbins(static_cast<int>((metmax-metmin)/20));
    TString vvvl_or("(HT400_Btag || HT600)");

    // MET, no cuts
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "Offline E_{T}^{miss} [GeV]",
    	       "(trig[6]||trig[7])", "trig[0]", "Ele15_"+vvvl_or+"", "HT350_MET100");
    PlotTurnOn(&c_mu, "met", metbins,metmin,metmax, "Offline E_{T}^{miss} [GeV]",
    	       "(trig[2]||trig[3])", "trig[0]",  "Mu15_"+vvvl_or+"", "HT350_MET100");
    // MET, njets >= 4
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "Offline E_{T}^{miss} [GeV]",
    	       "(trig[6]||trig[7])&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_mu, "met", metbins,metmin,metmax, "Offline E_{T}^{miss} [GeV]",
    	       "(trig[2]||trig[3])&&njets>=4", "trig[0]",
    	       "Mu15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_lep, "met", metbins,metmin,metmax, "Offline E_{T}^{miss} [GeV]",
    	       "(trig[2]||trig[3]||trig[6]||trig[7])&&njets>=4", "trig[0]",
    	       "Lep15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");

    // MET no HF, njets >= 4
    PlotTurnOn(&c_el, "met_nohf", metbins,metmin,metmax, "Offline E_{T}^{miss} no HF [GeV]",
    	       "(trig[6]||trig[7])&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_mu, "met_nohf", metbins,metmin,metmax, "Offline E_{T}^{miss} no HF [GeV]",
    	       "(trig[2]||trig[3])&&njets>=4", "trig[0]",
    	       "Mu15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_lep, "met_nohf", metbins,metmin,metmax, "Offline E_{T}^{miss} no HF [GeV]",
    	       "(trig[2]||trig[3]||trig[6]||trig[7])&&njets>=4", "trig[0]",
    	       "Lep15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");

    // MHT, njets >= 4
    PlotTurnOn(&c_el, "mht_ra2b", metbins,metmin,metmax, "Offline H_{T}^{miss} [GeV]",
    	       "(trig[6]||trig[7])&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_el, "mht_ra2b", metbins,metmin,metmax, "Offline H_{T}^{miss} [GeV]",
    	       "(trig[6]||trig[7])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[0]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_mu, "mht_ra2b", metbins,metmin,metmax, "Offline H_{T}^{miss} [GeV]",
    	       "(trig[2]||trig[3])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[0]",
    	       "Mu15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");
    PlotTurnOn(&c_lep, "mht_ra2b", metbins,metmin,metmax, "Offline H_{T}^{miss} [GeV]",
    	       "(trig[2]||trig[3]||trig[6]||trig[7])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[0]",
    	       "Lep15_"+vvvl_or+", n_{j}#geq4", "HT350_MET100");

    // MET70, njets >= 4
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "Offline E_{T}^{miss} [GeV]",
    	       "(trig[6]||trig[7])&&njets>=4", "trig[5]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "Ele15_HT350_MET70");
    PlotTurnOn(&c_mu, "met", metbins,metmin,metmax, "Offline E_{T}^{miss} [GeV]",
    	       "(trig[2]||trig[3])&&njets>=4", "trig[1]",
    	       "Mu15_"+vvvl_or+", n_{j}#geq4", "Mu15_HT350_MET70");
    PlotTurnOn(&c_lep, "met", metbins,metmin,metmax, "Offline E_{T}^{miss} [GeV]",
    	       "(trig[2]||trig[3]||trig[6]||trig[7])&&njets>=4", "trig[1]||trig[5]",
    	       "Lep15_"+vvvl_or+", n_{j}#geq4", "Lep15_HT350_MET70");
    PlotTurnOn(&c_el, "mht_ra2b", metbins,metmin,metmax, "Offline H_{T}^{miss} [GeV]",
    	       "(trig[6]||trig[7])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[5]",
    	       "Ele15_"+vvvl_or+", n_{j}#geq4", "Ele15_HT350_MET70");
    PlotTurnOn(&c_mu, "mht_ra2b", metbins,metmin,metmax, "Offline H_{T}^{miss} [GeV]",
    	       "(trig[2]||trig[3])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", "trig[1]",
    	       "Mu15_"+vvvl_or+", n_{j}#geq4", "Mu15_HT350_MET70");
    PlotTurnOn(&c_lep, "mht_ra2b", metbins,metmin,metmax, "Offline H_{T}^{miss} [GeV]",
    	       "(trig[2]||trig[3]||trig[6]||trig[7])&&mht_ra2b/met>0.5&&mht_ra2b/met<2&&njets>=4", 
	       "trig[1]||trig[5]", "Lep15_"+vvvl_or+", n_{j}#geq4", "Lep15_HT350_MET70");
  }

  ///////////////////////////////////////// HT /////////////////////////////////////////
 
  if(do_ht){
    float htmin(175), htmax(1000);
    int htbins(static_cast<int>((htmax-htmin)/12.5));
    PlotTurnOn(&c_met, "ht", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "trig[14]&&nvleps==0", "trig[0]","MET170, no leptons", "HT350_MET100");
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "trig[14]&&nvleps==0", "trig[0]","MET170, no leptons", "HT350_MET100");
    PlotTurnOn(&c_met, "ht", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "trig[14]&&nvleps==0", "trig[0]","MET170, no leptons", "HT350_MET100",350);
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "trig[14]&&nvleps==0", "trig[0]","MET170, no leptons", "HT350_MET100",350);

    htmin = 600; htmax = 1600; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_jetht, "ht", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "trig[11]&&nvleps==0", "trig[12]","HT475, no leptons", "HT800");
    PlotTurnOn(&c_jetht, "ht_hlt", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "trig[11]&&nvleps==0", "trig[12]","HT475, no leptons", "HT800");
    PlotTurnOn(&c_jetht, "ht", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "trig[11]&&nvleps==0", "trig[12]","HT475, no leptons", "HT800",825);
    PlotTurnOn(&c_jetht, "ht_hlt", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "trig[11]&&nvleps==0", "trig[12]","HT475, no leptons", "HT800",825);


    htmin = 125; htmax = 1000; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "(trig[14])&&nels>=1&&onmaxel>15", "trig[5]","MET170, n_{e} #geq 1",
    	       "Ele15_HT350_MET70");
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "(trig[13])&&(nmus)>=1&&onmaxmu>15", "trig[1]","MET120_Mu5, n_{#mu} #geq 1",
    	       "Mu15_HT350_MET70");
    htmin = 400; htmax = 1300; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_mu, "ht_hlt", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "(trig[1])", "trig[2]","Mu15_HT350_MET70", "Mu15_HT600");
    PlotTurnOn(&c_el, "ht_hlt", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "(trig[5])", "trig[6]","Ele15_HT350_MET70", "Ele15_HT600");
    PlotTurnOn(&c_mu, "ht", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "(trig[1])", "trig[2]","Mu15_HT350_MET70", "Mu15_HT600");
    PlotTurnOn(&c_el, "ht", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "(trig[5])", "trig[6]","Ele15_HT350_MET70", "Ele15_HT600");
    PlotTurnOn(&c_mu, "ht", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "(trig[1])", "trig[2]","Mu15_HT350_MET70", "Mu15_HT600",600);
    PlotTurnOn(&c_el, "ht", htbins,htmin,htmax, "Offline H_{T} [GeV]",
    	       "(trig[5])", "trig[6]","Ele15_HT350_MET70", "Ele15_HT600",600);


  } // if(do_ht)

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
		TString den, TString num, TString title, TString ytitle, float minfit){
  TCanvas can;
  can.SetGrid();
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

  TGraphAsymmErrors heff(histo[1], histo[0]);
  //TEfficiency heff(*histo[1], *histo[0]);
  heff.SetMarkerStyle(20); heff.SetMarkerSize(0.9);
  heff.SetMarkerColor(4); heff.SetLineColor(4);

  TString epsi("#scale[1.3]{#font[122]{e}}");
  // Ploting denominator
  float hfactor(0.3/histo[1]->GetMaximum());
  histo[1]->Scale(hfactor);
  //histo[1]->SetFillStyle(3344);
  histo[1]->SetFillColor(kGray);
  histo[1]->SetLineStyle(0);
  //histo[1]->SetTitle("Denom: "+title);
  histo[1]->GetXaxis()->SetTitle(xtitle);
  histo[1]->GetYaxis()->SetTitle(epsi+"  ["+ytitle+"]");
  histo[1]->GetYaxis()->SetRangeUser(0,1.35);
  histo[1]->Draw();

  histo[0]->Scale(hfactor);
  histo[0]->SetLineColor(kGray+1);
  histo[0]->SetLineStyle(2);
  histo[0]->SetLineWidth(2);
  histo[0]->Draw("same");

  pname = "plots/turnon_"+format_tag(var)+"_"+format_tag(num)+"_"+format_tag(den);
  if(minfit>0) {pname += "_min"; pname += minfit; }
  pname += plot_type;
  pname.ReplaceAll("_json_golden_pass","");

  
  // Fitting turn on curve
  if(minfit<0) minfit = minx;
  TF1 *fitCurve = new TF1("fitCurve",errorFun,minx,maxx,3);
  Double_t params[] = {0.99,minx+(maxx-minx)/4.,50., 50.};    
  //Double_t params[] = {0.99,880,50.};    
  fitCurve->SetParameters(params);
  fitCurve->SetParNames("#epsilon","#mu","#sigma");
  fitCurve->SetLineColor(2);
  fitCurve->SetLineWidth(2);
  heff.Fit("fitCurve","EM+","",minfit, maxx);
  //heff.Fit(fitCurve,"QR+");
  
  heff.Draw("p");
  histo[1]->Draw("axis same");

  // 98th percentile of Gaussian from Wolfram Alpha
  float p95(fitCurve->GetParameter(1)+2.05*fitCurve->GetParameter(2));
  float eplateau(fitCurve->GetParError(0)*100);
  if(eplateau<0.1) cout<<"Error on plateau "<<eplateau<<"%"<<endl;
  TString fitpar("#splitline{Plateau "+epsi+"  = ("+
		 RoundNumber(fitCurve->GetParameter(0)*100,1)+" #pm "+
		 RoundNumber(eplateau,1)+
		 ")%}{98% of plateau at "+RoundNumber(p95,0)+" GeV}");
  TLatex label; label.SetTextSize(0.042); 
  label.SetTextAlign(33); //label.SetNDC(); 
  float range(maxx-minx);
  float x1(maxx-0.57*range), y1(1.05), x2(maxx-0.03*range), y2(1.22);
  TBox box(x1,y1,x2,y2); box.SetFillColor(10); box.SetFillStyle(1001); 
  box.Draw();
  label.DrawLatex(x2-0.01*range,y2-0.01,fitpar);
  // Drowing selection
  x1 = maxx-0.7*range; y1 = 1.22; x2 = maxx-0.03*range; y2 = 1.31;
  box.DrawBox(x1,y1,x2,y2);
  label.SetTextAlign(33); //label.SetTextSize(0.04); 
  label.DrawLatex(x2-0.01*range, y2-0.01, "Denom: #font[52]{"+title+"}");
  // Drawing CMS preliminary
  label.SetNDC();  label.SetTextAlign(11); label.SetTextSize(0.045); 
  label.DrawLatex(0.2, 0.93, "#font[62]{CMS} #scale[0.8]{#font[52]{Preliminary}}");
  // Drawing luminosity
  label.SetTextAlign(31); 
  label.DrawLatex(0.94, 0.93, "42 pb^{-1} (13 TeV)");

  can.SaveAs(pname);
  
  for(unsigned his(0); his<2; his++)
    histo[his]->Delete();
  fitCurve->Delete();
}

TString Efficiency(TChain *data, TString den, TString num){
  TCanvas can;
  can.SetGrid();
  TH1D* histo[2];
  TString hname, totCut, pname;
  //den = "("+den+")&&json_golden";
  hname = "den"; totCut = den;
  histo[0] = new TH1D(hname, "", 1, 0, 1);
  float denom(data->GetEntries(totCut));
  histo[0]->SetBinContent(1,denom);

  hname = "num"; totCut = "("+den+")&&("+num+")";
  histo[1] = new TH1D(hname, "", 1, 0, 1);
  float numer(data->GetEntries(totCut));
  histo[1]->SetBinContent(1,numer);

  TGraphAsymmErrors heff(histo[1], histo[0]);
  //TEfficiency heff(*histo[1], *histo[0]);

  float errup(heff.GetErrorYhigh(0)), errdown(heff.GetErrorYlow(0));
  //float errup(heff.GetEfficiencyErrorUp(0)), errdown(heff.GetEfficiencyErrorLow(0));

  den.ReplaceAll("&&json_golden","");
  if(denom) cout<<"Eff for "<<num<<" and "<<den<<" is "<<RoundNumber(numer*100,1,denom)
		<<"+"<<RoundNumber(errup*100,1)
		<<"-"<<RoundNumber(errdown*100,1)<<" with "<<denom<<" denominator"<<endl;
  else cout<<"Denominator is zero"<<endl;
  
  TString efficiency(RoundNumber(numer*100,1,denom)+"^{+"+RoundNumber(errup*100,1)+
		     "}_{-"+RoundNumber(errdown*100,1)+"}");

  for(unsigned his(0); his<2; his++)
    histo[his]->Delete();

  return efficiency;

}

