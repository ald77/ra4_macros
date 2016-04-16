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

void makePlot(vector<sfeats> Samples, vector<TChain *> chain, TString cuts);

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
  Samples.push_back(sfeats(s_tt, "t#bar{t}", kRed+1));
  Samples.push_back(sfeats(s_t1t, t1t_label+" (1500,100)", kAzure+2));
  Samples.push_back(sfeats(s_t1tc, t1t_label+" (1200,800)", kGreen+1));

  // Reading ntuples
  vector<TChain *> chain;
  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++){
      chain[sam]->Add(Samples[sam].file[insam]);
    }   
  }

  TString mjcut("400");
  vector<var_class> mj_vars;
  mj_vars.push_back(var_class("mj_cands",2200,0,"M_{J} with PFcands",kMagenta,1));
  mj_vars.push_back(var_class("mj_cands_trim",2200,0,"M_{J} with PFcands trimmed",kRed,1));
  mj_vars.push_back(var_class("mj_10",2200,0,"M_{J} with 10 GeV jets",kGreen+1,1));
  mj_vars.push_back(var_class("mj_30", 2200,0,"M_{J} with 30 GeV jets",kAzure+2,1));
  TLatex label; label.SetNDC(kTRUE);
  label.SetTextSize(0.052);
  label.SetTextAlign(31);

  // Finding histograms
  TCanvas can;
  double legSingle = 0.08;
  double legX=style.PadLeftMargin+0.03, legY=1-style.PadTopMargin-0.02, legW=0.1, legH=legSingle*mj_vars.size();
  TLegend leg(legX,legY-legH, legX+legW, legY);
  leg.SetTextSize(0.05); leg.SetFillColor(0); leg.SetBorderSize(0); leg.SetFillStyle(0);
  float minh = 5, maxh = 40;
  int nbins = (maxh-minh)/5;
  TString hname;
  vector<TH1D*> histo[2];
  for(unsigned sam(0); sam < Samples.size(); sam++){
    for(unsigned var(0); var<mj_vars.size(); var++){
      for(size_t ind(0); ind<2; ind++){
	hname = "histo"; hname += sam;  hname += var;  hname += ind;
	histo[ind].push_back(new TH1D(hname, "", nbins, minh, maxh));
	histo[ind][var]->Sumw2();
	//histo[ind][var]->SetBinErrorOption(TH1::kPoisson);
	histo[ind][var]->SetLineColor(mj_vars[var].color);
	histo[ind][var]->SetLineWidth(4);
	style.setTitles(histo[ind][var], "True n_{PV}", "#epsilon^{norm} for M_{J} > "+mjcut+" GeV",
			"#scale[0.8]{#font[62]{CMS}} #scale[0.6]{#font[52]{Supplementary (Simulation)}}", "#scale[0.8]{13 TeV}");
	histo[ind][var]->SetBinContent(nbins, histo[ind][var]->GetBinContent(nbins)+
				       histo[ind][var]->GetBinContent(nbins+1));
      }
      chain[sam]->Project(histo[0][var]->GetName(), "ntrupv", "1*("+mj_vars[var].varname+">"+mjcut+")");
      chain[sam]->Project(histo[1][var]->GetName(), "ntrupv", "1");
      histo[0][var]->Divide(histo[1][var]);
      histo[0][var]->Scale(1/histo[0][var]->GetBinContent(4));
      histo[0][var]->SetMinimum(0.15);
      histo[0][var]->SetMaximum(2.1);
      leg.AddEntry(histo[0][var], mj_vars[var].title);
      if(var==0){
	histo[0][var]->Draw("");
	TLine line; line.SetLineStyle(2);
	line.DrawLine(minh, 1, maxh, 1);
      } else histo[0][var]->Draw("same");
    } // Loop over MJ types
    leg.Draw();
    label.DrawLatex(1-style.PadRightMargin-0.04, style.PadBottomMargin+0.04, "Sample: "+Samples[sam].label);  

    TString pname = "plots/eff_pu_"+format_tag(Samples[sam].label)+".pdf";
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
