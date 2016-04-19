// plot_mj_roc_aux: Macro that plots ROC curves

#include <stdexcept>
#include <iostream>

#include "TChain.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TDirectory.h"
#include "TMarker.h"
#include "TStyle.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "plot_mj_roc_aux.hpp"
#include "utilities_macros.hpp"

using namespace std;

int main(){
  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder
  
  TString folder=bfolder+"/cms2r0/babymaker/babies/2015_01_30/small_tree/skim_ht500met200/";

  // NTUPLES
  vector<TString> v_t1t;
  v_t1t.push_back(folder+"*T1tttt*1500_*PU20*");

  vector<TString> v_t1tc;
  v_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");

  vector<TString> v_tt;
  v_tt.push_back(folder+"*TTJet*");

  ////////////////////////// SAMPLES for the axes /////////////////////////
  TString lsp = "{#lower[-0.1]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}}";
  TString t1t_label = "#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#kern[0.18]{#bar{t}}#kern[0.18]"+lsp;
  vector<sample_class> tt_t1t; 
  tt_t1t.push_back(sample_class(t1t_label+" (1500,100)", v_t1t));
  tt_t1t.push_back(sample_class("t#bar{t}", v_tt));

  vector<sample_class> tt_t1tc; 
  tt_t1tc.push_back(sample_class(t1t_label+" (1200,800)", v_t1tc));
  tt_t1tc.push_back(sample_class("t#bar{t}", v_tt));



  ///////////////////// VARIABLES for each ROC /////////////////////
  int ht_col(1), mj_col(kAzure+2), col2(kGreen+1), col3(kRed+1), col4(kOrange+7);
  int mj_style(8); float mj_size(2.);
  vector<marker_class> mj_points, ht_points, met_points, nj_points;
  mj_points.push_back(marker_class(200,  mj_size, mj_col, mj_style));
  mj_points.push_back(marker_class(400,  4, mj_col, 29));
  mj_points.push_back(marker_class(600,  mj_size, mj_col, mj_style));
  //mj_points.push_back(marker_class(800,  mj_size, mj_col, mj_style));
  //mj_points.push_back(marker_class(1000,  mj_size, mj_col, mj_style));

  ht_points.push_back(marker_class(1000,  4, ht_col, 29));
  ht_points.push_back(marker_class(1500,  mj_size, ht_col, mj_style));
  //ht_points.push_back(marker_class(2000,  mj_size, ht_col, mj_style));
  //ht_points.push_back(marker_class(2500,  mj_size, ht_col, mj_style));

  met_points.push_back(marker_class(400,  4, col2, 29));
  met_points.push_back(marker_class(600,  mj_size, col2, mj_style));
  //met_points.push_back(marker_class(800,  mj_size, col2, mj_style));

  nj_points.push_back(marker_class(6,  mj_size, col3, mj_style));
  nj_points.push_back(marker_class(9,  4, col3, 29));
  nj_points.push_back(marker_class(11,  mj_size, col3, mj_style));

  vector<var_class> mj_general;
  mj_general.push_back(var_class("ht",4000,0,"H_{T}",ht_col,2,ht_points));
  mj_general.push_back(var_class("met",1500,0,"E_{T}^{miss}",col2,1,met_points));
  mj_general.push_back(var_class("njets30",15,0,"N_{jets}",col3,1,nj_points));
  mj_general.push_back(var_class("mj_30",2000,0,"M_{J}",mj_col,1,mj_points));

  vector<var_class> mj_cands;
  //mj_cands.push_back(var_class("ht",4000,0,"H_{T}",ht_col,2,ht_points));
  mj_cands.push_back(var_class("mj_cands",2200,0,"M_{J} PFcands",col2,1));
  mj_cands.push_back(var_class("mj_cands_trim",2200,0,"M_{J} PFcands trimmed",col3,1));
  mj_cands.push_back(var_class("mj_10",2200,0,"M_{J} 10 GeV jets",col4,1));
  mj_cands.push_back(var_class("mj_30", 2200,0,"M_{J} 30 GeV jets",mj_col,1,mj_points));

  vector<var_class> mj_sizes;
  //mj_sizes.push_back(var_class("ht",4000,0,"H_{T}",ht_col,2,ht_points));
  mj_sizes.push_back(var_class("mj_r08",2200,0,"M_{J} R=0.8",col2,1));
  mj_sizes.push_back(var_class("mj_r10",2200,0,"M_{J} R=1.0",col3,1));
  mj_sizes.push_back(var_class("mj_30", 2200,0,"M_{J} R=1.2",mj_col,1,mj_points));
  mj_sizes.push_back(var_class("mj_r14",2200,0,"M_{J} R=1.4",col4,1));

  ///////////////////// ROCs to plot /////////////////////
  vector<TString> vs_sam, vs_vars;
  vector<vector<sample_class>*> v_sam; 
  v_sam.push_back(&tt_t1t); vs_sam.push_back("tt_t1t");
  v_sam.push_back(&tt_t1tc); vs_sam.push_back("tt_t1tc");

  vector<vector<var_class>*> v_vars;
  //v_vars.push_back(&mj_general); vs_vars.push_back("general");
  v_vars.push_back(&mj_cands); vs_vars.push_back("cands");
  v_vars.push_back(&mj_sizes); vs_vars.push_back("size");

  vector<TString> v_cuts;
  v_cuts.push_back("ht>500&&met>200");
 
  for(unsigned ivar(0); ivar<v_vars.size(); ivar++){
    for(unsigned icut(0); icut<v_cuts.size(); icut++){
      for(unsigned isam(0); isam<v_sam.size(); isam++){
  	DrawROC(*(v_sam[isam]), *(v_vars[ivar]), v_cuts[icut], "mj_"+vs_sam[isam]+"_"+vs_vars[ivar]);
      } // Loop over samples
    } // Loop over cuts
  } // Loop over variables

}



void DrawROC(vector<sample_class> samples, vector<var_class> vars, TString cuts, TString tag){
  styles style("RA4"); style.setDefaultStyle();
  TCanvas can;
  const int nbins(1000);
  vector<vector<TH1D> > histos;
  TString hname, totcut;
  TChain *chain[2];

  for(unsigned sam(0); sam < samples.size(); sam++){
    // Loading chains
    for(unsigned isam(0); isam < samples[sam].files.size(); isam++){
      chain[sam] = new TChain("tree");
      int nfiles = chain[sam]->Add(samples[sam].files[isam]);
      if(nfiles==0) cout<<samples[sam].files[isam]<<" not found"<<endl;
    }
    histos.push_back(vector<TH1D>());

    // Projecting variables
    for(unsigned var(0); var<vars.size(); var++){
      float minh(vars[var].minx), maxh(vars[var].maxx);
      if(minh > maxh){
	minh = maxh;
	maxh = vars[var].minx;
      }
      hname = "histo"; hname += sam; hname += var;
      totcut = "weight*("+cuts+"&&"+samples[sam].cut+")";
      histos[sam].push_back(TH1D(hname,"",nbins,minh,maxh));
      chain[sam]->Project(hname, vars[var].varname, totcut);
    } // Loop over variables
  } // Loop over samples

  TH1D base_histo("base","",1,0.03,1.0);
  base_histo.SetMinimum(0.0);
  base_histo.SetMaximum(1.0);
  base_histo.SetDirectory(0);
  base_histo.Draw();
  style.setTitles(&base_histo, samples[0].label+" efficiency", samples[1].label+" efficiency", 
		  "#scale[0.8]{#font[62]{CMS}} #scale[0.6]{#font[52]{Supplementary (Simulation)}}", "#scale[0.8]{13 TeV}");

  // Legend
  double legX = style.PadLeftMargin+0.03, legY = 1-style.PadTopMargin-0.02, legSingle = 0.064;
  double legW = 0.2, legH = legSingle*vars.size();
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.057); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  // Making individual graphs
  TGraph graphs[100]; // Had to make it an array because a vector<TGraph> kept crashing
  for(unsigned var(0); var<vars.size(); var++){
    graphs[var] = MakeROC(histos[0][var], histos[1][var], vars[var].minx < vars[var].maxx, vars[var].cuts);
    graphs[var].SetLineColor(vars[var].color);
    graphs[var].SetLineStyle(vars[var].style);
    graphs[var].SetLineWidth(5);
    leg.AddEntry(&(graphs[var]), vars[var].title, "l");
    graphs[var].Draw("lsame");
  } // Loop over variables
  leg.Draw();

  cuts.ReplaceAll(".",""); 
  cuts.ReplaceAll("(",""); cuts.ReplaceAll("$","");  cuts.ReplaceAll(")",""); 
  cuts.ReplaceAll("[",""); cuts.ReplaceAll("]",""); 
  cuts.ReplaceAll("/","_"); cuts.ReplaceAll("*",""); cuts.ReplaceAll("&&","_");
  cuts.ReplaceAll(">=","ge"); cuts.ReplaceAll("<=","se"); 
  cuts.ReplaceAll(">","g"); cuts.ReplaceAll("<","s"); cuts.ReplaceAll("=","");
  cuts.ReplaceAll("+",""); 
  TString pname("plots/roc_"+tag+"_"+cuts+".pdf");  
  //can.Print(pname);
  can.SetLogx(1);
  can.SetLogy(1);
  pname.ReplaceAll("roc_","log_roc_");
  base_histo.SetMinimum(3e-4);
  can.Print(pname);

  for(unsigned sam(0); sam < samples.size(); sam++)
    chain[sam]->Delete();
}

TGraph MakeROC(TH1D &good, TH1D &bad, const bool less_is_better, vector<marker_class> cuts){
  const int nbins = good.GetNbinsX();
  if(bad.GetNbinsX() != nbins) throw logic_error("Mismatched number of bins");

  TMarker marker;  

  TGraph graph(0);
  const double good_tot = good.Integral(0, nbins+1);
  const double bad_tot = bad.Integral(0, nbins+1);
  int inibin(0), endbin(nbins+1), dbin(1); unsigned icut(0);
  if(less_is_better){
    inibin = nbins+1;
    endbin = 0;
    dbin = -1;
  }
  for(int bin = inibin; bin*dbin<=endbin*dbin; bin+=dbin){
    const double good_pass = good.Integral(min(endbin,bin), max(endbin,bin));
    const double bad_pass = bad.Integral(min(endbin,bin), max(endbin,bin));
    const double x = good_pass/good_tot;
    const double y = bad_pass/bad_tot;
    graph.SetPoint(graph.GetN(), x, y);
 
    // Plotting the stars
    if(icut<cuts.size()){
      float edge(good.GetXaxis()->GetBinUpEdge(bin));
      if((edge>=cuts[icut].cut&&!less_is_better) || (edge<=cuts[icut].cut&&less_is_better)){
	marker.SetMarkerStyle(cuts[icut].style);marker.SetMarkerColor(cuts[icut].color);
	marker.SetMarkerSize(cuts[icut].size); 
	if(x>0.01 && y>0.0001) marker.DrawMarker(x,y);
	icut++;
      }
    }
  }
  TString name(good.GetName());
  name += "graph";
  graph.SetName(name);
  graph.SetTitle(name);

  return graph;
}

var_class::var_class(TString ivarname, float iminx, float imaxx, TString ititle, int icolor, 
	    int istyle, vector<marker_class> icuts){
  varname = ivarname; minx = iminx; maxx = imaxx; title = ititle;
  cuts = icuts; 
  color = icolor; style = istyle;
}

sample_class::sample_class(TString ilabel, vector<TString> ifiles, TString icut){
  files = ifiles; label = ilabel; cut = icut;
}

marker_class::marker_class(float icut, float isize, int icolor, int istyle){
  cut=icut; size=isize; color=icolor; style=istyle;
}

