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
#include "TRandom3.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;

void plot_distributions(vector<sfeats> Samples, vector<hfeats> vars, TString luminosity, 
			TString filetype, TString namestyle, TString dir){
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
  double legY(0.902), legSingle = 0.052;
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
  TString hname, pname, variable, samVariable, leghisto, totCut, title, ytitle, lumilabel, cmslabel;
  for(unsigned var(0); var<vars.size(); var++){
    const unsigned Nsam(vars[var].samples.size());
    if(Nsam>=4) {
      leg[0].SetX1NDC(legX1[0]);
      leg[0].SetX2NDC(legX1[0]+legW);
    } else {
      leg[0].SetX1NDC(0.5);
      leg[0].SetX2NDC(0.5+legW);
    }
    legH = (Nsam<=3?legSingle*Nsam:legSingle*(Nsam+1)/2);
    fracLeg = legH/(1-style.PadTopMargin-style.PadBottomMargin)*1.25;
    for(int ileg(0); ileg<nLegs; ileg++) leg[ileg].SetY1NDC(legY-legH); 
    cout<<endl;
    // Generating vector of histograms
    title = cuts2title(vars[var].cuts); 
    if(namestyle=="CMSPaper") title = "";
    for(unsigned his(0); his < 2; his++){
      varhisto.resize(0);
      for(unsigned sam(0); sam < Nsam; sam++){
	hname = "histo"; hname += var; hname += his; hname += sam;
	varhisto.push_back(new TH1D(hname, title, vars[var].nbins, vars[var].minx, vars[var].maxx));
      }
      histo[his].push_back(varhisto);
    }
    nentries.resize(Nsam);
    variable = vars[var].varname;
    float maxhisto(-999);
    int nbkg(0);
    for(unsigned sam(Nsam-1); sam < Nsam; sam--){
      int isam = vars[var].samples[sam];
      if(!Samples[isam].isSig) nbkg++;
      samVariable = Samples[isam].samVariable;
      totCut = Samples[isam].factor+"*"+luminosity+"*weight*("+vars[var].cuts+"&&"+Samples[isam].cut+")"; 
      //cout<<totCut<<endl;
      histo[0][var][sam]->Sumw2();
      if(samVariable=="noPlot") chain[isam]->Project(histo[0][var][sam]->GetName(), variable, totCut);
      else chain[isam]->Project(histo[0][var][sam]->GetName(), samVariable, totCut);
      histo[0][var][sam]->SetBinContent(vars[var].nbins,
					histo[0][var][sam]->GetBinContent(vars[var].nbins)+
					histo[0][var][sam]->GetBinContent(vars[var].nbins+1));
      nentries[sam] = histo[0][var][sam]->Integral(1,vars[var].nbins);
      if(nentries[sam]<0) nentries[sam]=0;
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
      for(int bin(0); bin<=histo[0][var][sam]->GetNbinsX()+1; bin++){
	histo[1][var][sam]->SetBinContent(bin, histo[0][var][sam]->GetBinContent(bin));
	histo[1][var][sam]->SetBinError(bin, histo[0][var][sam]->GetBinError(bin));
      }
    }
    if(nbkg>0){
      //// Plotting lumi-weighted distributions in histo[0], and then area-normalized in histo[1] ///
      int bkgind(-1);
      for(unsigned sam(Nsam-1); sam < Nsam; sam--){
	int isam = vars[var].samples[sam];
	bool isSig = Samples[isam].isSig;
	if(!isSig){ // Adding previous bkg histos
	  for(unsigned bsam(sam+1); bsam < Nsam; bsam++){
	    histo[0][var][sam]->Add(histo[0][var][bsam]);
	    break;
	  }
	  histo[0][var][sam]->SetFillColor(Samples[isam].color);
	  histo[0][var][sam]->SetFillStyle(1001);
	  histo[0][var][sam]->SetLineColor(1);
	  histo[0][var][sam]->SetLineWidth(1);
	  bkgind = sam;
	} else {
	  histo[0][var][sam]->SetLineColor(Samples[isam].color);
	  histo[0][var][sam]->SetLineStyle(abs(Samples[isam].style));
	  histo[0][var][sam]->SetLineWidth(6);
	  if(Samples[isam].doStack)  histo[0][var][sam]->Add(histo[0][var][bkgind]);
	}
	if(maxhisto < histo[0][var][sam]->GetMaximum()) maxhisto = histo[0][var][sam]->GetMaximum();
      } // First loop over samples
      for(int ileg(0); ileg<nLegs; ileg++) leg[ileg].Clear();
      unsigned legcount(0);
      int firstplotted(-1);
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
	    histo[0][var][sam]->Draw("hist");
	    firstplotted = sam;
	    style.setTitles(histo[0][var][sam],vars[var].title, ytitle, cmslabel, lumilabel);
	  } else histo[0][var][sam]->Draw("hist same");
	} else {
	  leg[ileg].AddEntry(histo[0][var][sam], leghisto,"l");
	  legcount++;
	}
      }    
      for(int sam(Nsam-1); sam >= 0; sam--){
	int isam = vars[var].samples[sam];
	bool isSig = Samples[isam].isSig;
	if(isSig) histo[0][var][sam]->Draw("hist same");
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
      pname = "plots/"+dir+"/log_lumi_"+vars[var].tag+plot_tag;
      if(!vars[var].skiplog) can.SaveAs(pname);
      can.SetLogy(0);
      float maxpad(maxhisto + fracLeg*(maxhisto-minLog)/(1-fracLeg));
      if(vars[var].maxYaxis > 0) maxpad = vars[var].maxYaxis;
      histo[0][var][firstplotted]->SetMinimum(0);
      histo[0][var][firstplotted]->SetMaximum(maxpad);
      pad = static_cast<TPad *>(can.cd(1));
      style.moveYAxisLabel(histo[0][var][firstplotted], maxpad, false);
      pname = "plots/"+dir+"/lumi_"+vars[var].tag+plot_tag;
      can.SaveAs(pname);
    }
    //////////// Plotting area-normalized distributions ////////////
    maxhisto = -999;
    for(int ileg(0); ileg<nLegs; ileg++) leg[ileg].Clear();
    unsigned legcount = 0;
    for(unsigned sam(0); sam < Nsam; sam++){
      int isam = vars[var].samples[sam];
      histo[1][var][sam]->SetLineColor(Samples[isam].color);
      histo[1][var][sam]->SetMarkerColor(Samples[isam].color);
      histo[1][var][sam]->SetMarkerStyle(20);
      histo[1][var][sam]->SetLineStyle(abs(Samples[isam].style));
      histo[1][var][sam]->SetLineWidth(4);
      if(nentries[sam]) histo[1][var][sam]->Scale(100./nentries[sam]);
      if(maxhisto < histo[1][var][sam]->GetMaximum()) maxhisto = histo[1][var][sam]->GetMaximum();
      if(sam==0){
	histo[1][var][sam]->SetXTitle(vars[var].title);
	histo[1][var][sam]->SetYTitle("Entries (%)");
	if(Samples[isam].style>0) histo[1][var][sam]->Draw("hist");
	else histo[1][var][sam]->Draw("e1 x0");
      } else {
	if(Samples[isam].style>0) histo[1][var][sam]->Draw("hist same");
	else histo[1][var][sam]->Draw("e1 x0 same");
      }
      leghisto = Samples[isam].label;
      unsigned ileg = (Nsam<=3?0:legcount>=(Nsam+1)/2);
      if(vars[var].nevents.at(sam)<0 && namestyle!="CMSPaper") {
	leghisto += " [#mu=";
	int digits(0);
	if(histo[1][var][sam]->GetMean()<30) digits = 1;
	leghisto += RoundNumber(histo[1][var][sam]->GetMean(),digits) + "]";
      }
      else{
	leg[ileg].SetX1NDC(0.24); leg[ileg].SetX2NDC(0.7);
	leg[ileg].SetTextSize(0.75*style.LegendSize);
	leghisto +=  "[N_{tks} = " + RoundNumber(nentries[sam],1) + ", from N_{events} = "+RoundNumber(vars[var].nevents.at(sam),1)+"]";
      }
      
      if(Samples[isam].style>0) leg[ileg].AddEntry(histo[1][var][sam], leghisto, "l");
      else leg[ileg].AddEntry(histo[1][var][sam], leghisto, "p");
      legcount++;
    } // Loop over samples
    for(int ileg(0); ileg<nLegs; ileg++) leg[ileg].Draw(); 
    if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, maxhisto);
    float maxpad = maxhisto + fracLeg*maxhisto/(1-fracLeg);
    histo[1][var][0]->SetMaximum(maxpad);
    histo[1][var][0]->Draw("axis same");
    style.moveYAxisLabel(histo[1][var][0], maxpad, false);
    can.SetLogy(0);
    pname = "plots/"+dir+"/shapes_"+vars[var].tag+plot_tag;
    can.SaveAs(pname);
    float maxpadLog = maxhisto*exp(fracLeg*log(maxhisto/minLog)/(1-fracLeg));
    histo[1][var][0]->SetMaximum(maxpadLog);
    style.moveYAxisLabel(histo[1][var][0], maxpadLog, true);
    can.SetLogy(1);
    pname = "plots/"+dir+"/log_shapes_"+vars[var].tag+plot_tag;
    if(!vars[var].skiplog) can.SaveAs(pname);
  }// Loop over variables

  for(unsigned his(0); his < 2; his++){
    for(unsigned var(0); var<vars.size(); var++){
      for(unsigned sam(0); sam < vars[var].samples.size(); sam++)
	if(histo[his][var][sam]) histo[his][var][sam]->Delete();
    }
  }
}

TString cuts2title(TString title){
  if(title=="1") title = "";
  title.ReplaceAll("Sum$(abs(mc_id)==11)","n^{true}_{e}");
  title.ReplaceAll("Sum$(abs(mc_id)==13)","n^{true}_{#mu}");
  title.ReplaceAll("Sum$(genels_pt>0)", "n^{true}_{e}");
  title.ReplaceAll("Sum$(genmus_pt>0)", "n^{true}_{#mu}");
  title.ReplaceAll("ntruleps", "n^{true}_{l}");
  title.ReplaceAll("onmet", "MET"); title.ReplaceAll("onht", "H_{T}");  
  title.ReplaceAll("nvmus==1&&nmus==1&&nvels==0","1 #mu");
  title.ReplaceAll("nvmus10==0&&nvels10==0", "0 leptons");  
  title.ReplaceAll("(nmus+nels)", "n_{lep}");  title.ReplaceAll("njets30","n_{jets}^{30}"); 
  title.ReplaceAll("els_pt","p^{e}_{T}");title.ReplaceAll("mus_pt","p^{#mu}_{T}");
  title.ReplaceAll("mus_reliso","RelIso"); title.ReplaceAll("els_reliso","RelIso");
  title.ReplaceAll("mus_miniso_tr15","MiniIso"); title.ReplaceAll("els_miniso_tr15","MiniIso");
  title.ReplaceAll("njets","n_{jets}");title.ReplaceAll("abs(lep_id)==13&&","");
  title.ReplaceAll(">=", " #geq "); title.ReplaceAll(">", " > "); 
  title.ReplaceAll("<=", " #leq "); title.ReplaceAll("<", " < "); 
  title.ReplaceAll("&&", ", "); 
  title.ReplaceAll("met", "MET"); title.ReplaceAll("ht", "H_{T}");  title.ReplaceAll("mt", "m_{T}"); 
  title.ReplaceAll("ntks_chg==0", " ITV");
  title.ReplaceAll("nleps==1", "1 lepton");  title.ReplaceAll("nbm","n_{b}"); title.ReplaceAll("==", " = "); 
  title.ReplaceAll("nbl[1]","n_{b,l}");
  title.ReplaceAll("mj", " M_{J}");

  return title;
}

TString invertcut(TString cut){ //For only one cut
  if (cut.Contains("<"))
    cut.ReplaceAll("<",">="); 
  else if (cut.Contains(">"))
    cut.ReplaceAll(">","<=");
  else if (cut.Contains("<="))
    cut.ReplaceAll("<=",">");
  else if (cut.Contains(">="))
    cut.ReplaceAll(">=","<");

  return cut;
}

pfeats::pfeats(const vector<int> &isamples, const TString &icut, const TString &itagname):
  samples(isamples),
  cut(icut),
  tagname(itagname){
  }

hfeats::hfeats(TString ivarname, int inbins, float iminx, float imaxx, vector<int> isamples,
               TString ititle, TString icuts, float icut, TString itagname,bool iskiplog, vector<double> inevents):
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
  maxYaxis = -1.;
  skiplog=iskiplog;
  if(inevents.at(0)<0) nevents = vector<double>(isamples.size(),-1);
  else nevents = inevents;
  if(nevents.size() != samples.size() ) cout<<"hfeats samples/nevents size mismatch: "<<ititle<<endl;
  
  
  string ctitle(title.Data()); // Needed because effing TString can't handle square brackets
  if(!(ctitle.find("GeV")==std::string::npos)) unit = "GeV";
  if(!(ctitle.find("phi")==std::string::npos)) unit = "rad";
  }

hfeats::hfeats(TString ivarname, int inbins, float *ibinning, vector<int> isamples,
               TString ititle, TString icuts, float icut, TString itagname,bool iskiplog, vector<double> inevents):
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
  maxYaxis = -1.;
  skiplog =iskiplog;
  if(inevents.at(0)<0) nevents = vector<double>(isamples.size(),-1);
  else nevents = inevents;
  if(nevents.size() != samples.size() ) cout<<"hfeats samples/nevents size mismatch: "<<ititle<<endl;
  
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
  tag.ReplaceAll("#",""); tag.ReplaceAll("{",""); tag.ReplaceAll("}","");

  tag.ReplaceAll("htg500_metg200_nbmge1_mtg140_njetsge7_nmusnels1_","baseline1B_");
  tag.ReplaceAll("htg500_metg200_nbmge1_mtg140_njetsge7_nmusnels1_","baseline_");

  tag.ReplaceAll("tks_idlep_chargeg0_nottks_is_primary_tks_idtks_id121_","os_els_");
  tag.ReplaceAll("tks_idlep_chargeg0_nottks_is_primary_tks_idtks_id169_","os_mus_");
  tag.ReplaceAll("tks_idlep_charges0_nottks_is_primary_nottks_idtks_id169_tks_idtks_id121_","os_had_");

  tag.ReplaceAll("nottks_from_w_","fakes_");
  tag.ReplaceAll("tks_from_w_","prompt_");

  tag.ReplaceAll("tks_ptmintks_mini_netks_mini_ch,tks_r02_netks_r02_ch","abs_mini_iso_chgneu");
  tag.ReplaceAll("tks_ptmintks_mini_netks_mini_ch,tks_r05_netks_r05_ch","abs_r_05_mini_iso_chgneu");
  tag.ReplaceAll("tks_pttks_mini_netks_mini_ch","abs_untruncated_mini_iso_chgneu");
  tag.ReplaceAll("mintks_mini_netks_mini_ch,tks_r02_netks_r02_ch","rel_02_mini_iso_chgneu");
  tag.ReplaceAll("mintks_mini_netks_mini_ch,tks_r05_netks_r05_ch","rel_05_mini_iso_chgneu");
  tag.ReplaceAll("tks_mini_netks_mini_ch","rel_untruncated_mini_iso_chgneu");
  
  tag.ReplaceAll("tks_ptmintks_mini_ch,tks_r02_ch","abs_mini_iso_chg");
  tag.ReplaceAll("tks_ptmintks_mini_ch,tks_r05_ch","abs_r05_mini_iso_chg");
  tag.ReplaceAll("tks_pttks_mini_ch","abs_untruncated_mini_iso_chg");
  tag.ReplaceAll("mintks_mini_ch,tks_r02_ch","rel_02_mini_iso_chg");
  tag.ReplaceAll("mintks_mini_ch,tks_r05_ch","rel_05_mini_iso_chg");
  tag.ReplaceAll("tks_mini_ch","rel_untruncated_mini_iso_chg");
  
  
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

sfeats::sfeats(vector<TString> ifile, TString ilabel, int icolor, int istyle, TString icut, 
	       TString isamVariable){
  file = ifile; label = ilabel; cut = icut;
  color = icolor; style = istyle;
  isSig = ifile[0].Contains("T1tttt");
  samVariable = isamVariable;
  factor = "1";
  tag = label;
  tag.ReplaceAll("(",""); tag.ReplaceAll(",","_");  tag.ReplaceAll(")","");
  tag.ReplaceAll("{",""); tag.ReplaceAll("#,","");  tag.ReplaceAll("}","");
  doStack = false;
}

sysfeats::sysfeats(TString iname, TString ititle):
  name(iname),
  title(ititle){
  bincuts = vector<TString>();
  weights = vector<double>();
}
void sysfeats::push_back(TString ibincut, double iweight){
  bincuts.push_back(ibincut);
  weights.push_back(iweight);
}
TString sysfeats::bincut(unsigned i){
  if (i<bincuts.size()) return bincuts[i];
  else {cout<<"ERROR: bincut out of range"<<endl; return "";}
}
double sysfeats::weight(unsigned i){
  if (i<weights.size()) return weights[i];
  else {cout<<"ERROR: weight out of range"<<endl; return -1.;}
}
unsigned sysfeats::size(){
  return weights.size(); 
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

long getYieldErr(TChain& tree, TString cut, double& yield, double& uncertainty){
  const TString hist_name("temp");
  TH1D temp(hist_name, "", 1, -1.0, 1.0);
  temp.Sumw2();
  long entries = tree.Project(hist_name, "0.0", cut);
  yield = temp.IntegralAndError(0,2,uncertainty);
  return entries;
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

double intGaus(double mean, double sigma, double minX, double maxX){
  return (TMath::Erf((maxX-mean)/sigma/sqrt(2.))-TMath::Erf((minX-mean)/sigma/sqrt(2.)))/2.;
}


// Code from http://www.hongliangjie.com/2012/12/19/how-to-generate-gamma-random-variables/
// Parameter b could be theta...
double gsl_ran_gamma(const double a, const double b, TRandom3 &rand){
  if (a < 1){
    double u = rand.Uniform(1);
    return gsl_ran_gamma(1.0 + a, b, rand) * pow (u, 1.0 / a);
  }

  double x, v, u;
  double d = a - 1.0 / 3.0;
  double c = (1.0 / 3.0) / sqrt (d);
  
  while (1) {
    do {
      x = rand.Gaus(0, 1.0);
      v = 1.0 + c * x;
    }
    while (v <= 0);
      
    v = v * v * v;
    u = rand.Uniform(1);

    if (u < 1 - 0.0331 * x * x * x * x) 
      break;

    if (log (u) < 0.5 * x * x + d * (1 - v + log (v)))
      break;
  }
    
  return b * d * v;
}


// yields[Nobs][Nsam] has the entries for each sample for each observable going into kappa
// weights[Nobs][Nsam] has the average weight of each observable for each sample
// powers[Nobs] defines kappa = Product_obs{ Sum_sam{yields[sam][obs]*weights[sam][obs]}^powers[obs] }
double calcKappa(vector<vector<float> > &entries, vector<vector<float> > &weights,
		 vector<float> &powers, float &mSigma, float &pSigma, bool do_data,
		 bool verbose, bool do_plot, int nrep){
  TRandom3 rand(1234);
  styles style("RA4"); style.setDefaultStyle();
  int nbadk(0);
  TCanvas can;
  vector<float> fKappas;
  double mean(0.), bignum(1e10);
  // Doing kappa variations
  for(int rep(0), irep(0); rep < nrep; rep++) {
    fKappas.push_back(1.);
    bool Denom_is0(false);
    for(unsigned obs(0); obs < powers.size(); obs++) {
      float observed(0.);
      for(unsigned sam(0); sam < entries[obs].size(); sam++) {
	// With a flat prior, the expected average of the Poisson with N observed is Gamma(N+1,1)
	// Rounding the expected yield for data stats
	if(do_data) observed += entries[obs][sam]*weights[obs][sam];
	else observed += gsl_ran_gamma(entries[obs][sam]+1,1,rand)*weights[obs][sam];
      } // Loop over samples
      //if(do_data) observed = gsl_ran_gamma(static_cast<int>(0.5+observed)+1,1,rand);
      if(do_data) observed = gsl_ran_gamma(observed+1,1,rand);
      if(observed <= 0 && powers[obs] < 0) Denom_is0 = true;
      else fKappas[irep] *= pow(observed, powers[obs]);
    } // Loop over number of observables going into kappa
    if(Denom_is0 && fKappas[irep]==0) {
      fKappas.pop_back();
      nbadk++;
    }else {
      if(Denom_is0) fKappas[irep] = bignum;
      else mean += fKappas[irep];
      irep++;
    }
  } // Loop over fluctuations of kappa (repetitions)
  int ntot(nrep-nbadk);
  mean /= static_cast<double>(ntot);

  sort(fKappas.begin(), fKappas.end());
  double gSigma = intGaus(0,1,0,1);
  int iMedian((nrep-nbadk+1)/2-1);
  int imSigma(iMedian-static_cast<int>(gSigma*ntot)), ipSigma(iMedian+static_cast<int>(gSigma*ntot));
  float median(fKappas[iMedian]);
  mSigma = median-fKappas[imSigma]; pSigma = fKappas[ipSigma]-median;

  // Finding standard value
  float stdval(1.);
  bool infStd(false);
  for(unsigned obs(0); obs < powers.size(); obs++) {
    float stdyield(0.);
    if(verbose) cout<<obs<<": ";
    for(unsigned sam(0); sam < entries[obs].size(); sam++) {
      if(verbose) cout<<"Yield"<<sam<<" "<<entries[obs][sam]*weights[obs][sam]
		      <<", N"<<sam<<" "<<entries[obs][sam]
		      <<", avW"<<sam<<" "<<weights[obs][sam]<<". ";
      stdyield += entries[obs][sam]*weights[obs][sam];
    }
    if(verbose) cout<<"  ==> Total yield "<<stdyield<<endl;
    if(stdyield <= 0 && powers[obs] < 0) infStd = true;
    else stdval *= pow(stdyield, powers[obs]);
  } // Loop over number of observables going into kappa
  if(infStd) stdval = median;
  else {
    int istd(0);
    for(int rep(0); rep < ntot; rep++) 
      if(fKappas[rep]>stdval) {istd = rep; break;}
    imSigma = istd-static_cast<int>(gSigma*ntot);
    ipSigma = istd+static_cast<int>(gSigma*ntot);
    if(imSigma<0){ // Adjusting the length of the interval in case imSigma has less than 1sigma
      ipSigma += (-imSigma);
      imSigma = 0;
    }
    if(ipSigma>=ntot){ // Adjusting the length of the interval in case ipSigma has less than 1sigma
      imSigma -= (ipSigma-ntot+1);
      ipSigma = ntot-1;
    }
    mSigma = stdval-fKappas[imSigma]; pSigma = fKappas[ipSigma]-stdval;
  }

  int nbins(100);
  double minH(stdval-3*mSigma), maxH(stdval+3*pSigma);
  if(minH < fKappas[0]) minH = fKappas[0];
  if(maxH > fKappas[ntot-1]) maxH = fKappas[ntot-1];
  TH1D histo("h","",nbins, minH, maxH);
  for(int rep(0); rep < ntot; rep++) 
    histo.Fill(fKappas[rep]);   
  //histo.SetBinContent(1, histo.GetBinContent(1)+nbadk);
  //histo.SetBinContent(nbins, histo.GetBinContent(nbins)+histo.GetBinContent(nbins+1));
  histo.Scale(1/histo.Integral());
  histo.SetMaximum(histo.GetMaximum()*1.2);
  histo.SetLineWidth(3);
  histo.Draw();
  histo.SetXTitle("Expected value");
  histo.SetYTitle("Probability");
  histo.Draw();
  if(do_plot) can.SaveAs("test.eps");

  double mode(histo.GetBinLowEdge(histo.GetMaximumBin()));
  if(verbose) cout<<"Std kappa = "<<stdval<<"+"<<pSigma<<"-"<<mSigma<<".   Mean = "<<mean
		  <<". Mode = "<<mode<<". Median = "<<median<<endl;

  return stdval;
}

