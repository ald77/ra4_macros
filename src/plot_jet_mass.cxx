// plot_jet_mass: Plots average jet mass versus pT
// and jet mass distributions for representative bins

#include <iostream>
#include <vector>

#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TFile.h"
#include "TH2.h"
#include "TGraphErrors.h"
#include "TProfile.h"

#include "styles.hpp"
#include "utilities_macros.hpp"

namespace {
  TString metcut("met<50");
  TString luminosity="2.25"; // in ifb
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
}

using namespace std;

int main(){ 

  //TString folder_mc="/net/cms2/cms2r0/ald77/archive/2015_07_22/skim_ht1000/";
  //TString folder_data="/net/cms2/cms2r0/ald77/archive/2015_07_26/skim_ht1000/";
  //folder_mc   = "/net/cms2/cms2r0/babymaker/babies/2015_11_28/mc/";
  TString folder_mc     = "/net/cms2/cms2r0/jaehyeok/babies/2015_11_28/mc/skim_ht1000/";
  TString folder_data = "/net/cms2/cms2r0/babymaker/babies/2015_11_20/data/hadronic/";
 
  vector<TString> s_data_ns;
  s_data_ns.push_back(folder_data+"*JetHT*");
  vector<TString> s_tt;
  s_tt.push_back(folder_mc+"*_TTJets*Lept*");
  s_tt.push_back(folder_mc+"*_TTJets_HT*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder_mc+"*ST*");
  vector<TString> s_qcd;
  s_qcd.push_back(folder_mc+"*QCD_HT*");
  //  s_qcd.push_back("/net/cms5/cms5r0/ald77/archive/2015_06_05/*QCD_Pt*");
  vector<TString> s_other;
  s_other.push_back(folder_mc+"*TTW*");
  s_other.push_back(folder_mc+"*TTZ*");
  s_other.push_back(folder_mc+"*_ZJet*");
  s_other.push_back(folder_mc+"*DY*");
  s_other.push_back(folder_mc+"*WJetsToLNu_HT*");
  s_other.push_back(folder_mc+"*WH_HToBB*");
  s_other.push_back(folder_mc+"*ggZH_HToBB*");
  s_other.push_back(folder_mc+"*ttHJetTobb*");

  // Reading ntuples
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_data_ns, "Data", 1, -1, "trig[12]")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_qcd, "QCD", dps::c_qcd));
  Samples.push_back(sfeats(s_tt, "t#bar{t}", dps::c_tt_1l, 1));
  Samples.push_back(sfeats(s_singlet, "Single top", dps::c_singlet));
  Samples.push_back(sfeats(s_other, "Other", dps::c_other));

  vector<int> ra4_sam;
  ra4_sam.push_back(0);
  ra4_sam.push_back(1);
  ra4_sam.push_back(2);
  ra4_sam.push_back(3);
  ra4_sam.push_back(4);

  vector<hfeats> vars;

  vars.push_back(hfeats("fjets_m",50,0,500, ra4_sam, "m(J) [GeV]",
    			"pass&&ht>1000&&"+metcut+"&&(nvmus+nvels)==0&&njets>=7&&njets<=9",0));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
  
  vars.push_back(hfeats("fjets_m",50,0,500, ra4_sam, "m(J) [GeV]",
    			"pass&&ht>1000&&"+metcut+"&&(nvmus+nvels)==0&&njets>=10",0));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
  
  vars.push_back(hfeats("mj",50,0,1500, ra4_sam, "M_{J} [GeV]",
    			"pass&&ht>1000&&"+metcut+"&&(nvmus+nvels)==0&&njets>=7&&njets<=9",0));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
  
  vars.push_back(hfeats("mj",50,0,1500, ra4_sam, "M_{J} [GeV]",
    			"pass&&ht>1000&&"+metcut+"&&(nvmus+nvels)==0&&njets>=10",0));
  vars.back().whichPlots = "1"; vars.back().normalize = true;
  
  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "1d_2015d",true);

/*
  double ptBinEdges[30] = {50, 60, 70, 80, 90,
                        100, 110, 120, 130, 140,
                        150, 160, 170, 180, 190,
                        200, 210, 220, 230, 240,
                        250, 300, 350, 400, 450,
                        500, 550, 600, 650, 700};

  TH2D *jetmass_vs_pt_data = new TH2D("jetmass_vs_pt_data", "jetmass_vs_pt_data", 29, ptBinEdges, 100, 0.,100.); 
  jetmass_vs_pt_data->Sumw2();
  TH2D *jetmass_vs_pt_MC = new TH2D("jetmass_vs_pt_MC", "jetmass_vs_pt_MC", 29, ptBinEdges, 100, 0.,100.); 
  jetmass_vs_pt_MC->Sumw2();

  TChain *filesData = new TChain("tree");
  filesData->Add(s_data_ns.at(0));

  TChain *filesMC = new TChain("tree");
  filesMC->Add(s_tt.at(0));
  filesMC->Add(s_singlet.at(0));
  filesMC->Add(s_qcd.at(0));
  for(unsigned int i=0; i<s_other.size(); i++) filesMC->Add(s_other.at(i));

  // make 2-d histograms for projections
  TString cuts("ht>1000&&"+metcut+"&&(nvmus+nvels)==0");
  filesData->Project("jetmass_vs_pt_data", "jets_m:jets_pt", "weight*(trig[12] && json && "+cuts+")");
  filesMC->Project("jetmass_vs_pt_MC", "jets_m:jets_pt", "weight*("+cuts+")");

  styles style("CMSPaper"); 
  //some small modifications for ratios
  style.nDivisions = 706;
  style.LabelSize *=1.1;
  style.LegendSize *=1.2;
  style.TitleSize *=1.2;
  style.yTitleOffset /=1.3;
  style.setDefaultStyle();

  jetmass_vs_pt_MC->Print();
  jetmass_vs_pt_MC->ProfileY()->Print();
  jetmass_vs_pt_data->Print();
  jetmass_vs_pt_data->ProfileY()->Print();
  TH1D *data = new TH1D("hdata", "hdata", 29, ptBinEdges); data->Sumw2();
  TH1D *MC = new TH1D("hmc", "", 29, ptBinEdges); MC->Sumw2();
  for (int i=1; i<30; i++){
    TH1D* htempdata = static_cast<TH1D*>(jetmass_vs_pt_data->ProjectionY(TString::Format("hdata_%i",i),i,i));
    data->SetBinContent(i, htempdata->GetMean());
    data->SetBinError(i, htempdata->GetMeanError());
    TH1D* htempmc = static_cast<TH1D*>(jetmass_vs_pt_MC->ProjectionY(TString::Format("hmc_%i",i),i,i));
    MC->SetBinContent(i, htempmc->GetMean());
    MC->SetBinError(i, htempmc->GetMeanError());
  }

  TCanvas* can = new TCanvas();
  can->cd();
  TPad *pad(NULL), *bpad(NULL); //bpad (stands for bottom pad) is for ratio
  float bpadHeight = 0.3;
  pad = new TPad("tpad","tpad",0.,bpadHeight,1.,1.); // assign 
  pad->SetBottomMargin(0.02);
  pad->SetTopMargin(style.PadTopMargin+0.01);
  pad->Draw();   
  bpad = new TPad("bpad","bpad",0.,0.,1.,bpadHeight+0.005); // assign 
  bpad->SetTopMargin(0.);
  bpad->SetBottomMargin(2.35*style.PadBottomMargin);
  bpad->SetFillStyle(4000);
  bpad->Draw(); 

  pad->cd();
  MC->SetMinimum(0);
  MC->GetYaxis()->SetRangeUser(0.,70.);
  MC->GetXaxis()->SetLabelOffset(1.5);
  MC->SetMarkerStyle(kFullCircle);
  MC->SetMarkerColor(kRed);
  MC->SetMarkerSize(0.6);
  MC->SetLineWidth(2);
  // MC->SetTitle(cuts2title(cuts));
  // MC->GetYaxis()->SetTitle("Mean AK4 jet mass [GeV]"); 
  MC->Draw("e x0");
  data->SetMarkerStyle(kOpenSquare);
  data->SetMarkerColor(kBlack);
  data->SetMarkerSize(0.6);
  data->SetLineColor(kBlack);
  data->SetLineWidth(2);
  data->Draw("e x0 same");

  TString cmslabel = "#font[62]{CMS} #scale[0.8]{#font[52]{Preliminary}}";
  TString lumilabel = TString::Format("L = %1.f",luminosity.Atof()*1000.)+" pb^{-1} (13 TeV)";
  style.setTitles(MC, "", "Mean AK4 jet mass [GeV]", cmslabel, lumilabel);

  TLegend *leg = new TLegend(0.6, 0.3, 0.8, 0.5);
  leg->AddEntry(data, "Data", "P");
  leg->AddEntry(MC, "MC", "P");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->Draw();

  

  // // ------- RATIO ---------
  TH1D* hratio = static_cast<TH1D*>(data->Clone());
  hratio->SetTitle("");
  hratio->Divide(MC);
  hratio->GetYaxis()->SetRangeUser(0.925,1.075);
  hratio->GetXaxis()->SetLabelSize(style.LabelSize*2.2);
  hratio->GetYaxis()->SetLabelSize(style.LabelSize*2.1);
  hratio->GetYaxis()->SetTitle("Data / MC ");
  hratio->GetXaxis()->SetTitle("AK4 jet p_{T} [GeV]");
  hratio->GetYaxis()->SetTitleSize(style.TitleSize*3);
  hratio->GetYaxis()->SetTitleOffset(0.5); //can't use relative size, since somehow it changes between plots...
  hratio->GetXaxis()->SetTitleSize(style.TitleSize*3);
  hratio->GetXaxis()->SetTitleOffset(style.xTitleOffset*0.9);
  hratio->SetMarkerStyle(kFullCircle);
  hratio->SetMarkerColor(kBlack);
  hratio->SetMarkerSize(0.6);
  hratio->SetLineColor(kBlack);
  hratio->SetLineWidth(2);
  //line at 1
  bpad->cd();
  hratio->Draw("e x0");
  TLine* l1 = new TLine(data->GetXaxis()->GetXmin(), 1., data->GetXaxis()->GetXmax(), 1.);
  l1->SetLineStyle(2);
  l1->Draw("same");

  can->SaveAs("plots/1d_2015d/mass_versus_pt2.pdf");
*/
}
