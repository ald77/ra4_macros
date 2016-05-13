///// plot_eff_scan_1d.cxx: Takes a 2D efficiency map, and find 1D efficiency for constant mglu+mlsp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "TCanvas.h"
#include "TGraph2D.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TFile.h"

#include "utilities.hpp"
#include "styles.hpp"

using namespace std;

int main(){
  styles style("RA4"); style.setDefaultStyle();
  TCanvas can;


  double xmin = 600;
  double xmax = 2000;
  double ymin = 0;
  double ymax = 1400;
  double bin_size = 10;
  int nxbins = max(1, min(500, static_cast<int>(ceil((xmax-xmin)/bin_size))));
  int nybins = max(1, min(500, static_cast<int>(ceil((ymax-ymin)/bin_size))));
  
  TFile gfile("t1tttt_eff.root");

  TGraph2D* geff = static_cast<TGraph2D*>(gfile.Get("effmap"));
  geff->SetNpx(nxbins);
  geff->SetNpy(nybins);

  
  TH2D *heff = geff->GetHistogram();
  if(heff == nullptr) {
    cout<<endl<<"Could not retrieve histogram"<<endl<<endl;
    return 1;
  }
  heff->SetTitle(";m_{gluino} [GeV];m_{LST} [GeV]; Efficiency");
  heff->Draw("cont4z");

  int nbins=64;
  float hmin=200, hmax=1800;
  vector<float> glus = {1800, 1400, 1000};
  vector<int> colors = {1,2,4};
  vector<TH1D*> histos;
  float eff_1l = 0.37;

  // Legend
  double legSingle = 0.06;
  double legX=style.PadLeftMargin+0.02, legY=1-style.PadTopMargin-0.02, legW=0.1, legH=legSingle*glus.size();
  TLegend leg(legX,legY-legH, legX+legW, legY);
  leg.SetTextSize(0.05); leg.SetFillColor(0); leg.SetBorderSize(0); leg.SetFillStyle(0);

  for(size_t iglu=0; iglu<glus.size(); iglu++){
    TString hname = "histo"; hname += iglu;
    TH1D* histo = new TH1D(hname,"",nbins,hmin,hmax);
    histo->SetLineWidth(3); histo->SetLineColor(colors[iglu]);
    histo->SetXTitle("m_{#tilde{g}} - m_{#chi^{0}_{1}} [GeV]");
    histo->SetYTitle("Efficiency in R4 [%]");
    for(int bin=1; bin<nbins; bin++){
      float dm = histo->GetBinCenter(bin);
      float mglu = (glus[iglu]+dm)/2., mlsp = (glus[iglu]-dm)/2.;
      if(mlsp<0) continue;
      float eff = 100*heff->GetBinContent(heff->FindBin(mglu, mlsp))/eff_1l;
      //cout<<"Efficiency for mglu = "<<mglu<<", mlsp = "<<mlsp<<" is "<<eff<<" %"<<endl;
      histo->SetBinContent(bin, eff);
    }
    
    histos.push_back(histo);
    leg.AddEntry(histo, "m_{#tilde{g}} + m_{#chi^{0}_{1}} = "+RoundNumber(glus[iglu],0)+" GeV");
    if(iglu==0) histos[iglu]->Draw("");
    else histos[iglu]->Draw("same");
  }
  leg.Draw();

  can.SaveAs("plots/eff_1d.pdf");

  leg.SetX1NDC(0.56); leg.SetX2NDC(0.56+legW);
  float eff_step = 50;
  for(size_t iglu=0; iglu<glus.size(); iglu++){
    histos[iglu]->SetBinContent(0,0);
    for(int bin=1; bin<nbins; bin++){
      float dm0 = histos[iglu]->GetBinCenter(bin);
      float dm1 = histos[iglu]->GetBinCenter(bin+1);
      float valbin0 = histos[iglu]->GetBinContent(bin);
      float valbin1 = histos[iglu]->GetBinContent(bin+1);
      histos[iglu]->SetBinContent(bin, eff_step*(valbin1-valbin0)/(dm1-dm0));
      float valbinm1 = histos[iglu]->GetBinContent(bin-1);
      valbin0 = histos[iglu]->GetBinContent(bin);
      histos[iglu]->SetBinContent(bin, (valbinm1+valbin0)/2);
      
    }
    histos[iglu]->SetYTitle("Change in efficiency [%/("+RoundNumber(eff_step,0)+" GeV)]");
    histos[iglu]->SetMinimum(-0.002*eff_step);
    //histos[iglu]->SetMaximum(0.05*eff_step);
    if(iglu==0) histos[iglu]->Draw("");
    else histos[iglu]->Draw("same");
   }
  leg.Draw();

  can.SaveAs("plots/deff_1d.pdf");

  
}
