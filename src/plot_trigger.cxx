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
  bool do_rund = true;
  bool do_dps = false;
  bool do_rpv = false;
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

  TString folder("/cms2r0/babymaker/babies/2015_09_28/skim_json/");

  TChain c_htmht("tree"); c_htmht.Add(folder+"*HTMHT*root");
  TChain c_jetht("tree"); c_jetht.Add(folder+"*JetHT*root");
  TChain c_met("tree"); c_met.Add(folder+"*MET*root");
  TChain c_mu("tree"); c_mu.Add(folder+"*SingleMuon*");
  TChain c_el("tree"); c_el.Add(folder+"*SingleElectron*");
  TChain c_lep("tree"); c_lep.Add(folder+"singlelep/*Single*");
  TChain c_all("tree"); c_all.Add(folder+"alldata/*root");
  TChain c_had("tree"); c_had.Add(folder+"hadronic/*root");
  TChain c_jetht_rpv("tree");
  c_jetht_rpv.Add("/homes/cawest/links/JetHT_Run2015C_25ns-05Oct2015-v1/*root");
  c_jetht_rpv.Add("/homes/cawest/links/JetHT_Run2015D-05Oct2015-v1/*root");
  c_jetht_rpv.Add("/homes/cawest/links/JetHT_Run2015D-PromptReco-v4/*root");
  TChain c_mu_rpv("tree");
  c_mu_rpv.Add("/homes/cawest/links/SingleMuon_Run2015C-23Sep2015-v1/*root");
  c_mu_rpv.Add("/homes/cawest/links/SingleMuon_Run2015D-05Oct2015-v1/*root");
  c_mu_rpv.Add("/homes/cawest/links/SingleMuon_Run2015D-PromptReco-v4/*root");

  if(do_rpv){
    float lmin(500), lmax(2000);
    int lbins(static_cast<int>((lmax-lmin)/25));
    PlotTurnOn(&c_mu_rpv, "ht", lbins,lmin,lmax, "H_{T}",
               "(trig[20]&&nmus==1&&mus_pt[0]>35)", "trig[12]","HLT_IsoMu27 && p_{T,#mu}>35 GeV", "PFHT800",700);
    PlotTurnOn(&c_jetht_rpv, "ht", lbins,lmin,lmax, "H_{T}",
               "(trig[11])", "trig[12]","PFHT450", "PFHT800",800);
  }

  if(do_rund){
    float lmin(25), lmax(300);
    int lbins(static_cast<int>((lmax-lmin)/12.5));

    lmin = 10; lmax = 80; lbins = static_cast<int>((lmax-lmin)/2.5);
    PlotTurnOn(&c_htmht, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, "#mu_{medium} p_{T}",
    	       "(trig[0])&&met>150", "trig[4]","HT350_MET100, MET > 150", "Mu15_HT350");
    PlotTurnOn(&c_htmht, "Max$(mus_pt*(mus_sigid&&mus_miniso<0.2))-0.1", lbins,lmin,lmax, "#mu_{medium} p_{T}",
    	       "(trig[0])&&met>150", "trig[17]","HT350_MET100, MET > 150", "IsoMu18");

    PlotTurnOn(&c_htmht, "Max$(els_pt*(els_sigid&&els_miniso<0.1))-0.1", lbins,lmin,lmax, "e_{medium} p_{T}",
    	       "(trig[0])&&met>150", "trig[8]","HT350_MET100, MET > 150", "Ele15_HT350");
    PlotTurnOn(&c_htmht, "Max$(els_pt*(els_sigid&&els_miniso<0.1))-0.1", lbins,lmin,lmax, "e_{medium} p_{T}",
    	       "(trig[0])&&met>150", "trig[23]","HT350_MET100, MET > 150", "Ele23_WPLoose");
 
    float htmin(175), htmax(850);
    int htbins(static_cast<int>((htmax-htmin)/12.5));
    htmin = 175; htmax = 850; htbins = static_cast<int>((htmax-htmin)/12.5);
    PlotTurnOn(&c_met, "ht", htbins,htmin,htmax, "H_{T}",
    	       "trig[14]&&nvleps>=1", "trig[0]","MET170, n_{l} #geq 1", "HT350_MET100", 300);
    htmin = 100; htmax = 850; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_met, "ht", htbins,htmin,htmax, "H_{T}",
    	       "(trig[13])&&(nmus)>=1&&onmaxmu>15", "trig[1]","MET120_Mu5, n_{#mu} #geq 1",
    	       "Mu15_HT350_MET50", 200);

    float metmin(0), metmax(460);
    int metbins(static_cast<int>((metmax-metmin)/20));
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[8])&&njets_ra2>=4&&ht_ra2>450&&ht_ra2<750", "trig[5]",      
    	       "Ele15_HT350, n_{j} #geq 4, 450 < H_{T} < 750", "Ele15_HT350_MET50");
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[8])&&njets_ra2>=4&&ht_ra2>750", "trig[5]",      
    	       "Ele15_HT350, n_{j} #geq 4, H_{T} > 750", "Ele15_HT350_MET50");
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[8])&&njets_ra2>=4&&ht_ra2>450", "trig[5]",      
    	       "Ele15_HT350, n_{j} #geq 4, H_{T} > 450", "Ele15_HT350_MET50");

  } // if(do_rund)

  if(do_dps){
    float htmin(175), htmax(850);
    int htbins(static_cast<int>((htmax-htmin)/12.5));
    htmin = 175; htmax = 850; htbins = static_cast<int>((htmax-htmin)/12.5);
    PlotTurnOn(&c_met, "ht", htbins,htmin,htmax, "H_{T}",
    	       "trig[14]&&nvleps==0", "trig[0]","MET170, no leptons", "HT350_MET100", 300);
    htmin = 100; htmax = 850; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_met, "ht", htbins,htmin,htmax, "H_{T}",
    	       "(trig[13])&&(nmus)>=1&&onmaxmu>15", "trig[1]","MET120_Mu5, n_{#mu} #geq 1",
    	       "Mu15_HT350_MET50", 200);

    htmin = 575; htmax = 1450; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_jetht, "ht", htbins,htmin,htmax, "H_{T}",
    	       "trig[11]&&nvleps==0", "trig[12]","HT475, no leptons", "HT800");
    htmin = 375; htmax = 1250; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_el, "ht", htbins,htmin,htmax, "H_{T}",
    	       "(trig[5])", "trig[6]","Ele15_HT350_MET50", "Ele15_HT600", 550);
    PlotTurnOn(&c_el, "ht", htbins,htmin,htmax, "H_{T}",
    	       "(trig[23])&&nels>=1&&onmaxel>15", "trig[6]","Ele23", "Ele15_HT600", 550);

    htmin = 175; htmax = 850; htbins = static_cast<int>((htmax-htmin)/12.5);
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "trig[14]&&nvleps==0", "trig[0]","MET170, no leptons", "HT350_MET100", 300);
    PlotTurnOn(&c_met, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "(trig[13])&&(nmus)>=1&&onmaxmu>15", "trig[1]","MET120_Mu5, n_{#mu} #geq 1",
    	       "Mu15_HT350_MET50", 300);

    htmin = 575; htmax = 1450; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_jetht, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "trig[11]&&nvleps==0", "trig[12]","HT475, no leptons", "HT800");
    htmin = 375; htmax = 1250; htbins = static_cast<int>((htmax-htmin)/25);
    PlotTurnOn(&c_el, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "(trig[5])", "trig[6]","Ele15_HT350_MET50", "Ele15_HT600", 550);
    PlotTurnOn(&c_el, "ht_hlt", htbins,htmin,htmax, "H_{T}",
    	       "(trig[23])&&nels>=1&&onmaxel>15", "trig[6]","Ele23", "Ele15_HT600", 550);

    float metmin(0), metmax(460);
    int metbins(static_cast<int>((metmax-metmin)/20));
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[8])&&njets_ra2>=4&&ht_ra2>450&&ht_ra2<750", "trig[5]",      
    	       "Ele15_HT350, n_{j} #geq 4, 450 < H_{T} < 750", "Ele15_HT350_MET50");
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[8])&&njets_ra2>=4&&ht_ra2>750", "trig[5]",      
    	       "Ele15_HT350, n_{j} #geq 4, H_{T} > 750", "Ele15_HT350_MET50");
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[8])&&njets_ra2>=4&&ht_ra2>450", "trig[5]",      
    	       "Ele15_HT350, n_{j} #geq 4, H_{T} > 450", "Ele15_HT350_MET50");
    PlotTurnOn(&c_el, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[8])&&njets_ra2>=4&&ht_ra2>450", "trig[0]",      
    	       "Ele15_HT350, n_{j} #geq 4, H_{T} > 450", "HT350_MET100");
    PlotTurnOn(&c_mu, "met", metbins,metmin,metmax, "E_{T}^{miss}",
    	       "(trig[4])&&njets_ra2>=4&&ht_ra2>450", "trig[0]",      
    	       "Mu15_HT350, n_{j} #geq 4, H_{T} > 450", "HT350_MET100");

    PlotTurnOn(&c_el, "mht_ra2", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[8])&&njets_ra2>=4&&ht_ra2>450", "trig[0]",      
    	       "Ele15_HT350, n_{j} #geq 4, H_{T} > 450", "HT350_MET100");
    PlotTurnOn(&c_el, "mht_ra2", metbins,metmin,metmax, "H_{T}^{miss}",
    	       "(trig[8])&&njets_ra2>=4&&ht_ra2>450&&mht_ra2/met>0.5&&mht_ra2/met<2", "trig[0]",      
    	       "Ele15_HT350, n_{j} #geq 4, H_{T} > 450", "HT350_MET100");


  }

}

void PlotTurnOn(TChain *data, TString var, int nbins, double minx, double maxx, TString xtitle, 
		TString den, TString num, TString title, TString ytitle, float minfit, bool isData){
  styles style("HLTStyle"); gStyle->SetPadTickY(0);
  bool dofit(minfit>=-1);
  TCanvas can;
  //can.SetGrid();
  TH1D* histo[2];
  TString hname, totCut, pname;
  //  den = "("+den+")&&json&&pass"; 
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
  pname.ReplaceAll("_json_pass","");

  
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
  if(isData) {
    if(do_rpv) label.DrawLatex(0.85, 0.93, "2.69 fb^{-1} (13 TeV)");
    else label.DrawLatex(0.85, 0.93, "116 pb^{-1} (13 TeV)");
  }
  else label.DrawLatex(0.85, 0.93, "Spring15 t#bar{t}");

  can.SaveAs(pname);
  
  for(unsigned his(0); his<2; his++)
    histo[his]->Delete();
  fitCurve->Delete();
}

TString Efficiency(TChain *data, TString den, TString num, float &effic, float &errup, float &errdown){
  TH1D* histo[2];
  TString hname, totCut, pname;
  //  den = "("+den+")&&json&&pass";
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

  den.ReplaceAll("&&json","");
  if(denom) cout<<endl<<"Eff = "<<RoundNumber(numer*100,1,denom)<<"+"<<RoundNumber(errup*100,1)
  		<<"-"<<RoundNumber(errdown*100,1)<<" for num "<<num<<" and "<<den<<" with "<<denom<<" entries"<<endl;
  else cout<<"Denominator is zero"<<endl;
  
  TString efficiency(RoundNumber(numer*100,1,denom)+"^{+"+RoundNumber(errup*100,1)+
		     "}_{-"+RoundNumber(errdown*100,1)+"}");

  for(unsigned his(0); his<2; his++)
    histo[his]->Delete();

  return efficiency;

}

