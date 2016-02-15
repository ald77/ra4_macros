// plot_t5tttt: Compares kinematic distributions of T1tttt and T5tttt

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TColor.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace {
  TString luminosity="2.25";
  TString plot_type=".pdf";
  TString plot_style="CMSPaperNoRatio";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 

  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder

  TString foldert1(bfolder+"/cms2r0/babymaker/babies/2016_01_11/mc/scan/");
  TString foldert5(bfolder+"/cms2r0/babymaker/babies/2016_02_09/mc/T5tttt/");

  vector<TString> s_t1m0;
  s_t1m0.push_back(foldert1+"*T1tttt*1400_*-0_*");
  vector<TString> s_t1m300;
  s_t1m300.push_back(foldert1+"*T1tttt*1400_*-300_*");
  vector<TString> s_t1m1100;
  s_t1m1100.push_back(foldert1+"*T1tttt*1400_*-1100_*");

  vector<TString> s_t5m0;
  s_t5m0.push_back(foldert5+"*T5tttt*1400_*-0_*");
  vector<TString> s_t5m300;
  s_t5m300.push_back(foldert5+"*T5tttt*1400_*-300_*");
  vector<TString> s_t5m1100;
  s_t5m1100.push_back(foldert5+"*T5tttt*1400_*-1100_*");


  // Reading ntuples
  // TString lsp = "#tilde{#chi}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  // TString t1t_label = "#scale[0.92]{#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#bar{t}"+lsp;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_t1m0,    "T1(1400,0)", kGreen+1, 1));
  Samples.push_back(sfeats(s_t1m300,  "T1(1400,300)", kAzure+2, 1));
  Samples.push_back(sfeats(s_t1m1100, "T1(1400,1100)", kRed, 1));
  Samples.push_back(sfeats(s_t5m0,    "T5(1400,0)", kGreen+1, 2));
  Samples.push_back(sfeats(s_t5m300,  "T5(1400,300)", kAzure+2, 2));
  Samples.push_back(sfeats(s_t5m1100, "T5(1400,1100)", kRed, 2));

  vector<int> all_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    all_sam.push_back(sam);
  } // Loop over samples


  vector<hfeats> vars;

  vars.push_back(hfeats("met",40,0,1200, all_sam, "MET [GeV]","nleps==1&&ht>500&&njets>=6"));
  vars.back().whichPlots = "34";
  vars.push_back(hfeats("ht",40,0,3000, all_sam, "H_{T} [GeV]","nleps==1&&met>200&&njets>=6"));
  vars.back().whichPlots = "34";
  vars.push_back(hfeats("mj",40,0,1500, all_sam, "M_{J} [GeV]","nleps==1&&ht>500&&met>200&&njets>=6"));
  vars.back().whichPlots = "34";
  vars.push_back(hfeats("nbm",6,-0.5,5.5, all_sam, "n_{b}","nleps==1&&ht>500&&met>200&&njets>=6"));
  vars.back().whichPlots = "34";
  vars.push_back(hfeats("njets",13,1.5,14.5, all_sam, "n_{jets}","nleps==1&&ht>500&&met>200"));
  vars.back().whichPlots = "34";

  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "t5tttt",false, true);

}
