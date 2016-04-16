// plot_dps: Macro that plots variables both for comissioning DPS

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TLatex.h"
#include "TF1.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

class marker_class {
public:
  marker_class(float icut, float isize, int icolor, int istyle);
  float cut, size;
  int color, style;
};

class var_class {
public:
  var_class(TString ivarname, float iminx, float imaxx, TString ititle, int icolor, 
	    int istyle=1, std::vector<marker_class> icuts=std::vector<marker_class>());
  TString title, varname;
  float minx, maxx;
  std::vector<marker_class> cuts;
  int color, style;
};

using namespace std;

int main(){ 

  styles style("RA4"); style.setDefaultStyle();

  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder
  
  TString folder=bfolder+"/cms2r0/babymaker/babies/2015_01_30/small_tree/";

  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_mLSP-100*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_mLSP-800*");
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJets*");


  // Reading ntuples
  TString lsp = "{#lower[-0.1]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}}";
  TString t1t_label = "#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#kern[0.18]{#bar{t}}#kern[0.18]"+lsp;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_t1t, t1t_label+" (1500,100)", kAzure+2));
  Samples.push_back(sfeats(s_t1tc, t1t_label+" (1200,800)", kGreen+1));
  Samples.push_back(sfeats(s_tt, "t#bar{t}", kRed+1));

  // Reading ntuples
  vector<TChain *> chain;
  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++){
      chain[sam]->Add(Samples[sam].file[insam]);
    }   
  }
  TString mj_s("M#lower[-.1]{_{J}}");
  vector<var_class> mj_vars;
  mj_vars.push_back(var_class("mj_cands",2200,0,mj_s+" with PFcands",kMagenta+1,1));
  mj_vars.push_back(var_class("mj_cands_trim",2200,0,mj_s+" with PFcands trim",kRed,1));
  mj_vars.push_back(var_class("mj_10",2200,0,mj_s+" with 10 GeV jets",kGreen+1,1));
  mj_vars.push_back(var_class("mj_30", 2200,0,mj_s+" with 30 GeV jets",kAzure+2,1));
  TLatex label; label.SetNDC(kTRUE);
  label.SetTextSize(0.052);
  label.SetTextAlign(31);

  // Finding histograms
  TCanvas can;
  double legSingle = 0.06;
  double legX=style.PadLeftMargin+0.03, legY=1-style.PadTopMargin-0.02, legW=0.1, legH=legSingle*mj_vars.size();
  TLegend leg(legX,legY-legH, legX+legW, legY);
  leg.SetTextSize(0.045); leg.SetFillColor(0); leg.SetBorderSize(0); leg.SetFillStyle(0);
  float binw=5., minh[] = {5,0}, maxh[] = {40,1000}; // 0 for ntrupv, 1 for MJ
  int nbins[] = {static_cast<int>((maxh[0]-minh[0])/binw), 400};
  TF1 linFit("linFit","[0]+[1]*x", minh[0], maxh[0]);

  TString hname;
  vector<TH1D*> histo[2];
  for(unsigned sam(0); sam < Samples.size(); sam++){
    for(unsigned var(0); var<mj_vars.size(); var++){
      for(size_t ind(0); ind<2; ind++){
	hname = "histo"; hname += sam;  hname += var;  hname += ind;
	histo[ind].push_back(new TH1D(hname, "", nbins[ind], minh[ind], maxh[ind]));
	histo[ind][var]->Sumw2();
	//histo[ind][var]->SetBinErrorOption(TH1::kPoisson);
	histo[ind][var]->SetLineColor(mj_vars[var].color);
	histo[ind][var]->SetLineWidth(4);
	style.setTitles(histo[ind][var], "True n_{PV}", "Average "+mj_s+" [GeV]",
			"#scale[0.8]{#font[62]{CMS}} #scale[0.6]{#font[52]{Supplementary (Simulation)}}", "#scale[0.8]{13 TeV}");
      }
      for(int bin=1; bin<=nbins[0]; bin++){
	TString cuts = "weight*(ntrupv>="+RoundNumber(minh[0]+(bin-1)*binw,1)+"&&ntrupv<"+RoundNumber(minh[0]+bin*binw,1)+")";
	chain[sam]->Project(histo[1][var]->GetName(), mj_vars[var].varname, cuts);
	histo[0][var]->SetBinContent(bin, histo[1][var]->GetMean());
	histo[0][var]->SetBinError(bin, histo[1][var]->GetMeanError());
      } // Loop over PV bins
      histo[0][var]->SetMinimum(0);
      histo[0][var]->SetMaximum(histo[0][var]->GetMaximum()*1.6);
      histo[0][var]->Fit(&linFit, "QEMN+");
      TString mean0 = RoundNumber(linFit.GetParameter(0),1), slope = RoundNumber(linFit.GetParameter(1),1);
      leg.AddEntry(histo[0][var], mj_vars[var].title+" (<"+mj_s+"> = "+mean0+" + "+slope+"n_{PV})");
      if(var==0){
	histo[0][var]->Draw("");
	TLine line; line.SetLineStyle(2);
	line.DrawLine(minh[0], 1, maxh[0], 1);
      } else histo[0][var]->Draw("same");
    } // Loop over MJ types
    leg.Draw();
    label.DrawLatex(1-style.PadRightMargin-0.03, style.PadBottomMargin+0.05, "Sample: "+Samples[sam].label);  

    TString pname = "plots/mj_pu_"+format_tag(Samples[sam].label)+".pdf";
    can.SaveAs(pname);
    leg.Clear();

    for(unsigned var(0); var<mj_vars.size(); var++){
      for(size_t ind(0); ind<2; ind++)
	histo[ind][var]->Delete();
      histo[0].clear();
      histo[1].clear();
    }

  } // Loop over samples
  
}

var_class::var_class(TString ivarname, float iminx, float imaxx, TString ititle, int icolor, 
	    int istyle, vector<marker_class> icuts){
  varname = ivarname; minx = iminx; maxx = imaxx; title = ititle;
  cuts = icuts; 
  color = icolor; style = istyle;
}

marker_class::marker_class(float icut, float isize, int icolor, int istyle){
  cut=icut; size=isize; color=icolor; style=istyle;
}
