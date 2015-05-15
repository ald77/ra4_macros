//----------------------------------------------------------------------------
// utilities_macros - Various functions used accross the code
//----------------------------------------------------------------------------

#ifndef INT_ROOT
#include "utilities_macros.hpp"
#endif

#include <cmath>
#include <vector>

#include <iostream>
#include <string>
#include <stdexcept>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TColor.h"
#include "TMath.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;

void plot_distributions(vector<sfeats> Samples, vector<hfeats> vars, TString luminosity, 
		       TString filetype, TString namestyle){
  styles style(namestyle); style.setDefaultStyle();
  TCanvas can;
  TPad *pad = static_cast<TPad *>(can.cd());


  // Reading ntuples
  vector<TChain *> chain;
  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }


  TString plot_tag("_lumi"+luminosity+filetype);
  float minLog = 0.04, fracLeg = 0.36; // Fraction of the histo pad devoted to the legend

  double legLeft(style.PadLeftMargin+0.03), legRight(1-style.PadRightMargin-0.02);
  double legY(0.905), legSingle = 0.052;
  double legW = 0.13, legH = legSingle*(vars[0].samples.size()+1)/2;
  double legX1[] = {legLeft, legLeft+(legRight-legLeft)/2.*1.15};
  TLegend leg[2]; int nLegs(2);
  for(int ileg(0); ileg<nLegs; ileg++){
    leg[ileg].SetX1NDC(legX1[ileg]); leg[ileg].SetX2NDC(legX1[ileg]+legW); 
    leg[ileg].SetY1NDC(legY-legH); leg[ileg].SetY2NDC(legY); 
    leg[ileg].SetTextSize(style.LegendSize); leg[ileg].SetFillColor(0); 
    leg[ileg].SetFillStyle(0); leg[ileg].SetBorderSize(0);
    leg[ileg].SetTextFont(style.nFont); 
  }
  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
  vector< vector<TH1D*> > histo[2];
  vector<TH1D*> varhisto;
  vector<float> nentries;
  TString hname, pname, variable, leghisto, totCut, title, ytitle, lumilabel, cmslabel;
  for(unsigned var(0); var<vars.size(); var++){
    const unsigned Nsam(vars[var].samples.size());
    legH = (Nsam<=3?legSingle*Nsam:legSingle*(Nsam+1)/2);
    fracLeg = legH/(1-style.PadTopMargin-style.PadBottomMargin)*1.15;
    for(int ileg(0); ileg<nLegs; ileg++) leg[ileg].SetY1NDC(legY-legH); 
    cout<<endl;
    // Generating vector of histograms
    title = vars[var].cuts; if(title=="1") title = "";
    title.ReplaceAll("Sum$(abs(mc_id)==11)>0","");title.ReplaceAll("Sum$(abs(mc_id)==13)>0","");
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
    if(namestyle=="CMSPaper") title = "";
    for(unsigned his(0); his < 2; his++){
      varhisto.resize(0);
      for(unsigned sam(0); sam < Nsam; sam++){
	hname = "histo"; hname += var; hname += his; hname += sam;
	varhisto.push_back(new TH1D(hname, title, vars[var].nbins, vars[var].minx, vars[var].maxx));
      }
      histo[his].push_back(varhisto);
    }

    //// Plotting lumi-weighted distributions in histo[0], and then area-normalized in histo[1] ///
    nentries.resize(Nsam);
    variable = vars[var].varname;
    float maxhisto(-999);
    for(unsigned sam(Nsam-1); sam < Nsam; sam--){
      int isam = vars[var].samples[sam];
      bool isSig = Samples[isam].isSig;
      totCut = Samples[isam].factor+"*"+luminosity+"*weight*("+vars[var].cuts+"&&"+Samples[isam].cut+")"; 
      //cout<<totCut<<endl;
      chain[isam]->Project(histo[0][var][sam]->GetName(), variable, totCut);
      histo[0][var][sam]->SetBinContent(vars[var].nbins,
					  histo[0][var][sam]->GetBinContent(vars[var].nbins)+
					  histo[0][var][sam]->GetBinContent(vars[var].nbins+1));
      nentries[sam] = histo[0][var][sam]->Integral(1,vars[var].nbins);
      if(namestyle!="CMSPaper") {
	ytitle = "Entries for "+luminosity+" fb^{-1}";
	lumilabel = "";
	cmslabel = "";
      } else {
	ytitle = "Entries";
	lumilabel = luminosity+" fb^{-1} (13 TeV)";
	cmslabel = "#font[62]{CMS}";
      }
      if(vars[var].unit!="") {
	int digits(0);
	float binwidth((vars[var].maxx-vars[var].minx)/static_cast<float>(vars[var].nbins));
	if(binwidth<1) digits = 1;
	ytitle += ("/("+RoundNumber(binwidth,digits) +" "+vars[var].unit+")");
      }
      histo[0][var][sam]->SetYTitle(ytitle);
      // Cloning histos for later
      for(int bin(0); bin<=histo[0][var][sam]->GetNbinsX()+1; bin++)
	histo[1][var][sam]->SetBinContent(bin, histo[0][var][sam]->GetBinContent(bin));

      if(!isSig){ // Adding previous bkg histos
	for(unsigned bsam(sam+1); bsam < Nsam; bsam++){
	  histo[0][var][sam]->Add(histo[0][var][bsam]);
	  break;
	}
	histo[0][var][sam]->SetFillColor(Samples[isam].color);
	histo[0][var][sam]->SetFillStyle(1001);
	histo[0][var][sam]->SetLineColor(1);
	histo[0][var][sam]->SetLineWidth(1);
      } else {
	histo[0][var][sam]->SetLineColor(Samples[isam].color);
	histo[0][var][sam]->SetLineStyle(Samples[isam].style);
	histo[0][var][sam]->SetLineWidth(4);
      }
      if(maxhisto < histo[0][var][sam]->GetMaximum()) maxhisto = histo[0][var][sam]->GetMaximum();
    } // First loop over samples
    int firstplotted(-1);
    for(int ileg(0); ileg<nLegs; ileg++) leg[ileg].Clear();
    unsigned legcount(0);
    for(unsigned sam(0); sam < Nsam; sam++){
      int isam = vars[var].samples[sam];
      leghisto = Samples[isam].label;
          if(namestyle!="CMSPaper") leghisto += " [N=" + RoundNumber(nentries[sam],0) + "]";
      bool isSig = Samples[isam].isSig;
      unsigned ileg = (Nsam<=3?0:legcount>=(Nsam+1)/2);
       if(!isSig){
	leg[ileg].AddEntry(histo[0][var][sam], leghisto,"f");
	legcount++;
	if(firstplotted < 0) {
	  histo[0][var][sam]->Draw();
	  firstplotted = sam;
	  style.setTitles(histo[0][var][sam],vars[var].title, ytitle, cmslabel, lumilabel);
 	} else histo[0][var][sam]->Draw("same");
      } else {
	leg[ileg].AddEntry(histo[0][var][sam], leghisto,"l");
	legcount++;
      }
    }    
    for(int sam(Nsam-1); sam >= 0; sam--){
      int isam = vars[var].samples[sam];
      bool isSig = Samples[isam].isSig;
      if(isSig) histo[0][var][sam]->Draw("same");
    }
    for(int ileg(0); ileg<nLegs; ileg++) leg[ileg].Draw(); 
    if(histo[0][var][firstplotted]->GetMinimum() > minLog) histo[0][var][firstplotted]->SetMinimum(minLog);
    float maxpadLog(maxhisto*exp(fracLeg*log(maxhisto/minLog)/(1-fracLeg)));
    histo[0][var][firstplotted]->SetMinimum(minLog);
    histo[0][var][firstplotted]->SetMaximum(maxpadLog);
    style.moveYAxisLabel(histo[0][var][firstplotted], maxpadLog, true);
    histo[0][var][firstplotted]->Draw("axis same");
    if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, maxhisto);
    can.SetLogy(1);
    pname = "plots/1d/log_lumi_"+vars[var].tag+plot_tag;
    can.SaveAs(pname);
    can.SetLogy(0);
    float maxpad(maxhisto + fracLeg*(maxhisto-minLog)/(1-fracLeg));
    histo[0][var][firstplotted]->SetMinimum(0);
    histo[0][var][firstplotted]->SetMaximum(maxpad);
    pad = static_cast<TPad *>(can.cd(1));
    style.moveYAxisLabel(histo[0][var][firstplotted], maxpad, false);
    pname = "plots/1d/lumi_"+vars[var].tag+plot_tag;
    can.SaveAs(pname);

    //////////// Plotting area-normalized distributions ////////////
    maxhisto = -999;
    for(int ileg(0); ileg<nLegs; ileg++) leg[ileg].Clear();
    legcount = 0;
    for(unsigned sam(0); sam < Nsam; sam++){
      int isam = vars[var].samples[sam];
      histo[1][var][sam]->SetLineColor(Samples[isam].color);
      histo[1][var][sam]->SetLineStyle(Samples[isam].style);
      histo[1][var][sam]->SetLineWidth(3);
      if(nentries[sam]) histo[1][var][sam]->Scale(100./nentries[sam]);
      if(maxhisto < histo[1][var][sam]->GetMaximum()) maxhisto = histo[1][var][sam]->GetMaximum();
      if(sam==0){
	histo[1][var][sam]->SetXTitle(vars[var].title);
	histo[1][var][sam]->SetYTitle("Entries (%)");
	histo[1][var][sam]->Draw();
      } else histo[1][var][sam]->Draw("same");
      leghisto = Samples[isam].label;
      if(namestyle!="CMSPaper") {
	leghisto += " [#mu=";
	int digits(0);
	if(histo[1][var][sam]->GetMean()<50) digits = 1;
	leghisto += RoundNumber(histo[1][var][sam]->GetMean(),digits) + "]";
      }
      unsigned ileg = (Nsam<=3?0:legcount>=(Nsam+1)/2);
      leg[ileg].AddEntry(histo[1][var][sam], leghisto);
      legcount++;
    } // Loop over samples
    for(int ileg(0); ileg<nLegs; ileg++) leg[ileg].Draw(); 
    if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, maxhisto);
    maxpad = maxhisto + fracLeg*maxhisto/(1-fracLeg);
    histo[1][var][0]->SetMaximum(maxpad);
    histo[1][var][0]->Draw("axis same");
    style.moveYAxisLabel(histo[1][var][0], maxpad, false);
    can.SetLogy(0);
    pname = "plots/1d/shapes_"+vars[var].tag+plot_tag;
    can.SaveAs(pname);
    maxpadLog = maxhisto*exp(fracLeg*log(maxhisto/minLog)/(1-fracLeg));
    histo[1][var][0]->SetMaximum(maxpadLog);
    style.moveYAxisLabel(histo[1][var][0], maxpadLog, true);
    can.SetLogy(1);
    pname = "plots/1d/log_shapes_"+vars[var].tag+plot_tag;
    can.SaveAs(pname);
  }// Loop over variables

  for(unsigned his(0); his < 2; his++){
    for(unsigned var(0); var<vars.size(); var++){
      for(unsigned sam(0); sam < vars[var].samples.size(); sam++)
	if(histo[his][var][sam]) histo[his][var][sam]->Delete();
    }
  }
}


pfeats::pfeats(const vector<int> &isamples, const TString &icut, const TString &itagname):
  samples(isamples),
  cut(icut),
  tagname(itagname){
  }

hfeats::hfeats(TString ivarname, int inbins, float iminx, float imaxx, vector<int> isamples,
               TString ititle, TString icuts, float icut, TString itagname):
  title(ititle),
  varname(ivarname),
  cuts(icuts),
  nbins(inbins),
  minx(iminx),
  maxx(imaxx),
  cut(icut),
  samples(isamples),
  tagname(itagname){
  format_tag();
  unit = "";
  string ctitle(title.Data()); // Needed because effing TString can't handle square brackets
  if(!(ctitle.find("GeV")==std::string::npos)) unit = "GeV";
  if(!(ctitle.find("phi")==std::string::npos)) unit = "rad";
  }

hfeats::hfeats(TString ivarname, int inbins, float *ibinning, vector<int> isamples,
               TString ititle, TString icuts, float icut, TString itagname):
  title(ititle),
  varname(ivarname),
  cuts(icuts),
  nbins(inbins),
  binning(ibinning),
  cut(icut),
  samples(isamples),
  tagname(itagname) {
  minx = binning[0]; maxx = binning[nbins];
  format_tag();
  unit = "";
  string ctitle(title.Data()); // Needed because effing TString can't handle square brackets
  if(!(ctitle.find("GeV")==std::string::npos)) unit = "GeV";
  if(!(ctitle.find("phi")==std::string::npos)) unit = "rad";
  }

void hfeats::format_tag(){
  tag = varname;
  if(cuts!="1")   tag+="_"+cuts;
  if(tagname!="") tag+="_"+tagname;

  tag.ReplaceAll(".","");
  tag.ReplaceAll("(",""); tag.ReplaceAll("$","");  tag.ReplaceAll(")","");
  tag.ReplaceAll("[",""); tag.ReplaceAll("]",""); tag.ReplaceAll("||","_");
  tag.ReplaceAll("/","_"); tag.ReplaceAll("*",""); tag.ReplaceAll("&&","_");
  tag.ReplaceAll(">=","ge"); tag.ReplaceAll("<=","se");
  tag.ReplaceAll(">","g"); tag.ReplaceAll("<","s"); tag.ReplaceAll("=","");
  tag.ReplaceAll("+",""); tag.ReplaceAll("&","");
  tag.ReplaceAll("!","not");
}

sfeats::sfeats(vector<TString> ifile, TString ilabel, int icolor, int istyle, TString icut){
  file = ifile; label = ilabel; cut = icut;
  color = icolor; style = istyle;
  isSig = ifile[0].Contains("T1tttt");
  factor = "1";
  tag = label;
  tag.ReplaceAll("(",""); tag.ReplaceAll(",","_");  tag.ReplaceAll(")","");
  tag.ReplaceAll("{",""); tag.ReplaceAll("#,","");  tag.ReplaceAll("}","");
}

// Function that calculates the chi2 of a histogram with respect to the flat hypothesis
void calc_chi2(TH1D *histo, float &chi2, int &ndof, float &pvalue, float &average){
  vector<double> vals[2];
  double sumx_sig2(0), sum_sig2(0);
  ndof = 0;
  for(int bin(1); bin<=histo->GetNbinsX(); bin++){
    if(histo->GetBinError(bin) > 0){
      vals[0].push_back(histo->GetBinContent(bin));
      vals[1].push_back(histo->GetBinError(bin));

      sumx_sig2 += vals[0][ndof]/pow(vals[1][ndof],2);
      sum_sig2 += 1/pow(vals[1][ndof],2);
      ndof++;
    }
  }
  if(sum_sig2<=0){cout<<"All errors in histo are zero. Exiting."<<endl; return;}
  average = sumx_sig2/sum_sig2;
  chi2 = 0; ndof--;
  for(int ival(0); ival <= ndof; ival++){
    chi2 += pow((vals[0][ival]-average)/vals[1][ival],2);
  }
  pvalue = TMath::Prob(chi2,ndof);
}

// Function that calculates the chi2 from the difference of 2 histos, after normalizing their areas
void calc_chi2_diff(TH1D *histo1, TH1D *histo2, float &chi2, int &ndof, float &pvalue, float *average){
  TH1D *histos[] = {histo1, histo2};
  vector<double> vals[2][2];
  double sumx_sig2[] = {0,0}, sum_sig2[] = {0,0};
  int ndofs[] = {0,0};
  for(int his(0); his<2; his++){
    for(int bin(1); bin<=histos[his]->GetNbinsX(); bin++){
      if(histos[his]->GetBinError(bin) > 0){
        vals[his][0].push_back(histos[his]->GetBinContent(bin));
        vals[his][1].push_back(histos[his]->GetBinError(bin));

        sumx_sig2[his] += vals[his][0][ndofs[his]];
        sum_sig2[his] += 1;
        ndofs[his]++;
      }
    }
    if(sum_sig2[his]<=0){cout<<"All errors in histo are zero. Exiting."<<endl; return;}
    average[his] = sumx_sig2[his]/sum_sig2[his];
    ndofs[his]--;
  } // Loop over histos

  // In principle, we should use just bins where both histos have entries. To be modified
  if(ndofs[0] != ndofs[1]) {cout<<"First histo has "<<ndofs[0]<<" ndof and second "<<ndofs[1]<<endl; return;}
  else ndof = ndofs[0];
  chi2 = 0;
  double Raver = average[0]/average[1];
  for(int ival(0); ival <= ndof; ival++){
    double error(sqrt(pow(vals[0][1][ival],2)+pow(vals[1][1][ival]*Raver,2)));
    chi2 += pow((vals[0][0][ival]-vals[1][0][ival]*Raver)/error,2);
  }
  pvalue = TMath::Prob(chi2,ndof);
}

namespace  ra4 {
  TColor ucsb_blue(1000, 1/255.,57/255.,166/255.);
  TColor ucsb_gold(1001, 255/255.,200/255.,47/255);
  TColor penn_red(1002, 149/255.,0/255.,26/255.);
  TColor uf_orange(1003, 255/255.,74/255.,0/255.);
  TColor uo_green(1004, 0/255.,79/255.,39/255.);
  TColor tcu_purple(1005, 52/255.,42/255.,123/255.);
  TColor tar_heel_blue(1006, 86/255.,160/255.,211/255.);
  TColor sig_teal(1007, 96/255.,159/255.,128/255.);
  TColor sig_gold(1008, 215/255.,162/255.,50/255.);
  TColor seal_brown(1010, 89/255.,38/255.,11/255.);
}
