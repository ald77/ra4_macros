// plot_cuts: Macro that plots significance and other S,B quantities

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TColor.h"
#include "RooStats/NumberCountingUtils.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace  {
  TString ntuple_date("2015_05_25");
  TString minjets("7"), midjets("9");
  TString mjthresh("400");
  TString luminosity="10";
  TString plot_type=".pdf";
  TString plot_style="RA4";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  styles style(plot_style); style.setDefaultStyle();
  vector<hfeats> vars;
  TCanvas can;

  TString folder="/cms5r0/ald77/archive/"+ntuple_date+"/skim100/";
  folder="/afs/cern.ch/user/m/manuelf/work/ucsb/2015_05_25/skim/";
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*WJetsToLNu_HT*");
  vector<TString> s_single;
  s_single.push_back(folder+"*_T*channel*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder+"*TTW*");
  s_ttv.push_back(folder+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD_HT*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");

  // Reading ntuples
  vector<TChain *> chain;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_other, "Other", 1001));
  Samples.push_back(sfeats(s_ttv, "ttV", 1002));
  Samples.push_back(sfeats(s_single, "Single top", 1005));
  Samples.push_back(sfeats(s_wjets, "W + jets", 1004));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", 1006,1,"ntruleps>=2"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", 1000,1,"ntruleps<=1"));
  Samples.push_back(sfeats(s_t1t, "T1tttt(1500,100)", 2));
  Samples.push_back(sfeats(s_t1tc, "T1tttt(1200,800)", 4));

  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  vector<int> ra4_sam;
  ra4_sam.push_back(0);
  ra4_sam.push_back(1);
  ra4_sam.push_back(2);
  ra4_sam.push_back(3);
  ra4_sam.push_back(4);
  ra4_sam.push_back(5);
  ra4_sam.push_back(6);
  ra4_sam.push_back(7);

  const int scanbins(100);
  // vars.push_back(hfeats("met",scanbins,100,1200, ra4_sam, "Cut on MET (GeV)",
  // 			"ht>500&&nbm>=2&&njets>="+minjets+"&&mt>140&&(nmus+nels)==1",200));
  // vars.push_back(hfeats("njets",18,-0.5,17.5, ra4_sam, "Cut on n_{jets}",
  // 			"ht>500&&met>200&&nbm>=2&&mt>140&&(nmus+nels)==1",minjets.Atof()));
  // vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Cut on n_{b}",
  // 			"ht>500&&met>200&&njets>="+minjets+"&&mt>140&&(nmus+nels)==1",2));


  // vars.push_back(hfeats("mt",scanbins,0,600, ra4_sam, "Cut on m_{T} (GeV)",
  // 			"ht>500&&met>400&&nbm>=2&&njets>=6&&(nmus+nels)==1",140));
  // vars.push_back(hfeats("mj",scanbins,0,1600, ra4_sam, "Cut on M_{J} (GeV)",
  // 			"ht>500&&met>400&&nbm>=2&&njets>=6&&mt>140&&(nmus+nels)==1",600));


  vars.push_back(hfeats("mj",scanbins,0,1600, ra4_sam, "Cut on M_{J} (GeV)",
                        "ht>500&&met>200&&nbm>=2&&njets>=7&&mt>140&&(nmus+nels)==1"));
  vars.push_back(hfeats("ht",scanbins,500,3000, ra4_sam, "Cut on H_{T} (GeV)",
                        "ht>500&&met>200&&nbm>=2&&njets>=7&&mt>140&&(nmus+nels)==1"));


  TString plot_tag("_lumi"+luminosity+plot_type);
  double Syserr(pow(0.3,2));
  double legX = 0.66, legY = 0.9, legSingle = 0.061;
  double legW = 0.12, legH = legSingle*3;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.052); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
  const unsigned Nhis(4);
  vector< vector<TH1D*> > histo[Nhis];
  vector<TH1D*> varhisto;
  vector<float> nentries;
  TString hname, pname, variable, leghisto, totCut, title;
  for(unsigned var(0); var<vars.size(); var++){
    cout<<endl;
    // Generating vector of histograms
    title = vars[var].cuts; if(title=="1") title = "";
    title.ReplaceAll("nvmus==1&&nmus==1&&nvels==0","1 #mu");
    title.ReplaceAll("nvmus10==0&&nvels10==0", "0 leptons");  
    title.ReplaceAll("(nmus+nels)", "n_{lep}");  title.ReplaceAll("njets30","n_{jets}^{30}"); 
    title.ReplaceAll("els_pt","p^{e}_{T}");title.ReplaceAll("mus_pt","p^{#mu}_{T}");
    title.ReplaceAll("mus_reliso","RelIso"); title.ReplaceAll("els_reliso","RelIso");
    title.ReplaceAll("mus_miniso_tr15","MiniIso"); title.ReplaceAll("els_miniso_tr15","MiniIso");
    title.ReplaceAll("njets","n_{jets}");title.ReplaceAll("abs(lep_id)==13&&","");
    title.ReplaceAll(">=", " #geq "); title.ReplaceAll(">", " > "); title.ReplaceAll("&&", ", "); 
    title.ReplaceAll("met", "MET"); title.ReplaceAll("ht", "H_{T}");  title.ReplaceAll("mt", "m_{T}"); 
    title.ReplaceAll("nleps==1", "1 lepton");  title.ReplaceAll("nbm","n_{b}"); title.ReplaceAll("==", " = "); 
    title.ReplaceAll("nbl[1]","n_{b,l}");
    for(unsigned his(0); his < Nhis; his++){
      varhisto.resize(0);
      for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
	int isam = vars[var].samples[sam];
	hname = "histo"; hname += var; hname += his; hname += sam;
	varhisto.push_back(new TH1D(hname, title, vars[var].nbins, vars[var].minx, vars[var].maxx));
	varhisto[sam]->SetXTitle(vars[var].title);
	varhisto[sam]->SetLineColor(Samples[isam].color);
	varhisto[sam]->SetLineStyle(Samples[isam].style);
	varhisto[sam]->SetLineWidth(3);
      }
      histo[his].push_back(varhisto);
    }

    //// Plotting Zbi in histo[0], S/B in histo[1], B in histo[2], S in hisot[3] ///
    leg.Clear();
    nentries.resize(0);
    variable = vars[var].varname;
    int bkgind(-1);
    for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
      int isam = vars[var].samples[sam];
      bool isSig = Samples[isam].isSig;
      totCut = Samples[isam].factor+"*"+luminosity+"*weight*("+vars[var].cuts+"&&"+Samples[isam].cut+")"; 
      //cout<<totCut<<endl;
      histo[0][var][sam]->Sumw2();
      chain[isam]->Project(histo[0][var][sam]->GetName(), variable, totCut);
      histo[0][var][sam]->SetBinContent(vars[var].nbins,
					  histo[0][var][sam]->GetBinContent(vars[var].nbins)+
					  histo[0][var][sam]->GetBinContent(vars[var].nbins+1));
      histo[0][var][sam]->SetBinError(vars[var].nbins,
				      sqrt(pow(histo[0][var][sam]->GetBinError(vars[var].nbins),2)+
					   pow(histo[0][var][sam]->GetBinError(vars[var].nbins+1),2)));
      nentries.push_back(histo[0][var][sam]->Integral(1,vars[var].nbins));
      histo[0][var][sam]->SetYTitle("Z_{bi} (#sigma)");
      histo[1][var][sam]->SetYTitle("S/#sqrt{B} (#sigma)");
      histo[2][var][sam]->SetYTitle("Events");
      histo[2][var][sam]->SetLineColor(1);
      if(!isSig){ // Adding previous bkg histos
	for(int bsam(sam-1); bsam >= 0; bsam--){
	  histo[0][var][sam]->Add(histo[0][var][bsam]);
	  break;
	}
	bkgind = sam;
      } 
    } // First loop over samples

    for(int sam(vars[var].samples.size()-1); sam >= 0; sam--){
      int isam = vars[var].samples[sam];
      if(!(Samples[isam].isSig)) continue;

      double maxhisto[Nhis];
      for(unsigned his(0); his<Nhis; his++) maxhisto[his] = -1;
      for(int bin(1); bin<=vars[var].nbins; bin++){
	double Nerr(0);
	const double Nsig(histo[0][var][sam]->Integral(bin,vars[var].nbins+1));
	const double Nbkg(histo[0][var][bkgind]->IntegralAndError(bin,vars[var].nbins+1,Nerr)); 
	histo[2][var][sam]->SetBinContent(bin,Nbkg);
	histo[3][var][sam]->SetBinContent(bin,Nsig);
	if(Nbkg==0){
	  histo[0][var][sam]->SetBinContent(bin,0);
	  histo[1][var][sam]->SetBinContent(bin,0);
	} else {
	  const double Zbi(RooStats::NumberCountingUtils::BinomialExpZ(Nsig, Nbkg, sqrt(pow(Nerr/Nbkg,2)+Syserr)));
	  histo[0][var][sam]->SetBinContent(bin,Zbi>0?Zbi:0);
	  histo[1][var][sam]->SetBinContent(bin,Nsig/sqrt(Nbkg));
	}
	for(unsigned his(0); his<Nhis; his++) 
	  if(maxhisto[his] < histo[his][var][sam]->GetBinContent(bin)) 
	    maxhisto[his] = histo[his][var][sam]->GetBinContent(bin);
      }
    } // Loop over samples

    unsigned sam_nc(vars[var].samples.size()-2);
    unsigned isam_nc = vars[var].samples[sam_nc];
    unsigned sam_c(vars[var].samples.size()-1);
    unsigned isam_c = vars[var].samples[sam_c];

    float maxleg = 1.35;
    leg.Clear();
    legH = legSingle*3;
    leg.SetY1NDC(legY-legH);
    leg.SetHeader("#font[22]{   L = "+luminosity+" fb^{-1}}");
    leg.AddEntry(histo[0][var][sam_nc], Samples[isam_nc].label);
    leg.AddEntry(histo[0][var][sam_c], Samples[isam_c].label);
    float hismax = max(histo[0][var][sam_nc]->GetMaximum(), histo[0][var][sam_c]->GetMaximum())*maxleg;
    histo[0][var][sam_nc]->SetMaximum(hismax);
    histo[0][var][sam_nc]->SetMaximum(3.7);
    histo[0][var][sam_nc]->SetMinimum(0);
    histo[0][var][sam_nc]->Draw("l hist");
    histo[0][var][sam_c]->Draw("l hist same");
    style.moveYAxisLabel(histo[0][var][sam_nc], hismax, false);
    if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, hismax);
    leg.Draw();
    pname = "plots/zbi_"+vars[var].tag+plot_tag;
    can.SaveAs(pname);
    histo[1][var][sam_nc]->SetMaximum(max(histo[1][var][sam_nc]->GetMaximum(),
      histo[1][var][sam_c]->GetMaximum())*maxleg);
    histo[1][var][sam_nc]->SetMinimum(0);
    histo[1][var][sam_nc]->Draw("l hist");
    histo[1][var][sam_c]->Draw("l hist same");
    if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, histo[1][var][sam_nc]->GetMaximum()*1.05);
    pname = "plots/s_sqrtb_"+vars[var].tag+plot_tag;
    leg.Draw();
    can.SaveAs(pname);
    hismax = max(histo[3][var][sam_nc]->GetMaximum(), histo[3][var][sam_c]->GetMaximum())*maxleg;
    histo[2][var][sam_nc]->SetMaximum(hismax);
    histo[2][var][sam_nc]->Draw("l hist");
    histo[3][var][sam_c]->Draw("l hist same");
    histo[3][var][sam_nc]->Draw("l hist same");
    if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, hismax);
    style.moveYAxisLabel(histo[0][var][sam_nc], hismax, false);
    leg.AddEntry(histo[2][var][sam_nc], "Total bkg.");
    legH = legSingle*4;
    leg.SetY1NDC(legY-legH);
    leg.Draw();
    pname = "plots/sb_"+vars[var].tag+plot_tag;
    can.SaveAs(pname);
    
  }// Loop over variables

  for(unsigned his(0); his < Nhis; his++){
    for(unsigned var(0); var<vars.size(); var++){
      for(unsigned sam(0); sam < vars[var].samples.size(); sam++)
	if(histo[his][var][sam]) histo[his][var][sam]->Delete();
    }
  }
}

