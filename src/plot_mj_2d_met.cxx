// plot_mj_met: Macro that plots signal and bkg in the MJ-MET plane

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TColor.h"
#include "TMath.h"
#include "TStyle.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;
using std::cout;
using std::endl;


int main(){ 
  styles style("LargeLabels"); style.setDefaultStyle();
  TCanvas can;

  TString folder="archive/15-03-17/skims/";
  vector<TString> bkgfiles;
  bkgfiles.push_back(folder+"*_TTJet*");
  bkgfiles.push_back(folder+"*_WJets*");
  bkgfiles.push_back(folder+"*_T*channel*");
  bkgfiles.push_back(folder+"*TTW*");
  bkgfiles.push_back(folder+"*TTZ*");
  bkgfiles.push_back(folder+"*QCD_HT*");
  bkgfiles.push_back(folder+"*_ZJet*");
  bkgfiles.push_back(folder+"*DY*");
  bkgfiles.push_back(folder+"*WH_HToBB*");

  bool do_t1t(true), do_2b(true);
  int colors[] = {4,kGreen+2};
  TString leglab[] = {"Total bkg.", "T1tttt(1500,100)"}, sigfile("*T1tttt*1500_*PU20*"), sigtag("t1t");
  if(!do_t1t){
    leglab[1] = "T1tttt(1200,800)";
    sigfile = "*T1tttt*1200_*PU20*";
    sigtag = "t1tc";
    colors[1] = kMagenta+1;
  }

  vector<TChain *> chain;
  for(unsigned icha(0); icha<2; icha++){
    chain.push_back(new TChain("tree"));
    if(icha==0){
      for(unsigned sam(0); sam < bkgfiles.size(); sam++)
	chain[icha]->Add(bkgfiles[sam]);
    } else chain[icha]->Add(folder+sigfile);
  }

  double legX = 0.15, legY = 0.87, legSingle = 0.06;
  double legW = 0.3, legH = legSingle*chain.size();
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.052); //leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
  TString luminosity="4", totCut, title;
  TString cuts("ht>500&&met>200&&mt>=150&&nbm>=2&&njets>=6&&(nmus+nels)==1"), hname;
  if(!do_2b) cuts.ReplaceAll("nbm>=2","nbm==1");
  int mj_scat_nbins(20), met_scat_nbins(10), mj_nrebin(2), met_nrebin(1);
  // int mj_nbins(6), met_nbins(5);
  // float mj_binning[] = {0,200,300,400,500,600,1500};
  // float met_binning[] = {200,300,400,500,600,1500};
  float limits[2][2] = {{0,1000},{200,700}};
  vector<TH2D *>hscat, hsb;
  for(unsigned icha(0); icha<2; icha++){
    hname = "hscatter"; hname += icha;
    hscat.push_back(new TH2D(hname,"",mj_scat_nbins,limits[0][0],limits[0][1], 
			     met_scat_nbins,limits[1][0],limits[1][1]));
    totCut = luminosity+"*weight*("+cuts+")"; 
    chain[icha]->Project(hname, "met:mj",totCut);

    //cout<<icha<<": Integral "<< RoundNumber(hscat[icha]->Integral(),2)<<endl;
    title = cuts; if(title=="1") title = "";
    title.ReplaceAll("nvmus==1&&nmus==1&&nvels==0","1 #mu");
    title.ReplaceAll("(nmus+nels)","n_{lep}");
    title.ReplaceAll("nvmus10==0&&nvels10==0", "0 leptons");  
    title.ReplaceAll("njets30","n_{jets}^{30}"); 
    title.ReplaceAll("els_pt","p^{e}_{T}");title.ReplaceAll("mus_pt","p^{#mu}_{T}");
    title.ReplaceAll("njets","n_{jets}"); title.ReplaceAll("<", " < "); 
    title.ReplaceAll(">=", " #geq "); title.ReplaceAll(">", " > "); title.ReplaceAll("&&", ", ");     
    title.ReplaceAll("met", "MET"); title.ReplaceAll("ht", "H_{T}");  title.ReplaceAll("mt", "m_{T}"); 
    title.ReplaceAll("nleps==1", "1 lepton");  title.ReplaceAll("nbm","n_{b}"); title.ReplaceAll("==", " = "); 
    hscat[icha]->SetTitle(title);
    hscat[icha]->SetXTitle("M_{J} (GeV)");
    hscat[icha]->SetYTitle("MET (GeV)");
    hscat[icha]->SetMarkerColor(colors[icha]);
    hscat[icha]->SetLineColor(colors[icha]);
    hscat[icha]->SetMarkerStyle(20);
    hscat[icha]->SetMarkerSize(0.7);
    if(icha==0) hscat[icha]->Draw("scat=40");
    else hscat[icha]->Draw("scat=40 same");
    line.DrawLine(400,200,400,700); line.DrawLine(0,350,1000,350);

    hname = "hsb"; hname += icha;
    hsb.push_back(static_cast<TH2D*>(hscat[icha]->Rebin2D(mj_nrebin,met_nrebin,hname)));
    leg.AddEntry(hscat[icha],leglab[icha]);

  } // Loop over bkg and signal
  leg.Draw();
  TString pname("plots/scatter_"+sigtag+(do_2b?"2b":"1b")); pname += ".eps";
  can.SaveAs(pname);

  const unsigned num_conts = 50;
  const unsigned num_stops = 2;
  double stops[num_stops] = {0., 1.};
  double red[num_stops]   = {1., 0.23};
  double green[num_stops] = {1., 0.72};
  double blue[num_stops]  = {1., 0.52};
  if(!do_t1t){
    red[1] = 0.78;
    green[1] = 0.47;
    blue[1] = 0.63;
  }
  gStyle->SetNumberContours(num_conts); gStyle->SetPaintTextFormat("4.2f");
  TColor::CreateGradientColorTable(num_stops, stops, red, green, blue, num_conts);

  can.SetRightMargin(0.13);
  //hsb[1]->Divide(hsb[0]);
  for(int xbin(1); xbin <= hsb[1]->GetNbinsX(); xbin++){
    for(int ybin(1); ybin <= hsb[1]->GetNbinsY(); ybin++){
      double num(hsb[1]->GetBinContent(xbin,ybin)), den(hsb[0]->GetBinContent(xbin,ybin));
      double val(0);
      if(den==0){
	if(num>0) val = 9.99;
	else val = 0.;
      } else val = num/den;
      hsb[1]->SetBinContent(xbin,ybin,val);
    } // Loop over ybins
  } // Loop over xbins
  hsb[1]->SetMinimum(0);
  hsb[1]->SetMaximum(1);
  hsb[1]->SetMarkerSize(2.5);
  hsb[1]->SetMarkerColor(1);
  hsb[1]->SetZTitle("S/B");
  hsb[1]->SetTitleOffset(0.56,"Z");
  hsb[1]->Draw("colz");
  hsb[1]->Draw("text same");
  line.DrawLine(400,200,400,700); line.DrawLine(0,350,1000,350);
  pname.ReplaceAll("scatter","s_over_b");
  can.SaveAs(pname);
 

  for(unsigned icha(0); icha<2; icha++){
    hscat[icha]->Delete();
    hsb[icha]->Delete();
    chain[icha]->Delete();
  }
}

