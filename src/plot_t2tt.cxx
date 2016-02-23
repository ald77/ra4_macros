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

  TString foldert2(bfolder+"/cms2r0/babymaker/babies/2016_02_09/mc/T2tt/");

  vector<TString> s_t2vh;
  s_t2vh.push_back(foldert2+"*T2tt*250_*-0_*");
  vector<TString> s_t2h;
  s_t2h.push_back(foldert2+"*T2tt*175_*-0_*");
  vector<TString> s_t2m;
  s_t2m.push_back(foldert2+"*T2tt*150_*-0_*");
  vector<TString> s_t2l;
  s_t2l.push_back(foldert2+"*T2tt*100_*-0_*");

  // vector<TString> s_t22vh;
  // s_t22vh.push_back(foldert2+"*T2tt*250_*-0_*");
  // vector<TString> s_t22mh;
  // s_t22mh.push_back(foldert2+"*T2tt*950_*-0_*");
  // vector<TString> s_t22h;
  // s_t22h.push_back(foldert2+"*T2tt*600_*-0_*");
  // vector<TString> s_t22m;
  // s_t22m.push_back(foldert2+"*T2tt*300_*-0_*");
  // vector<TString> s_t22ml;
  // s_t22ml.push_back(foldert2+"*T2tt*200_*-0_*");
  // vector<TString> s_t22l;
  // s_t22l.push_back(foldert2+"*T2tt*100_*-0_*");

  // vector<TString> s_t22vh;
  // s_t22vh.push_back(foldert2+"*T2tt*250_*-0_*");
  // vector<TString> s_t22mh;
  // s_t22mh.push_back(foldert2+"*T2tt*200_*-0_*");
  // vector<TString> s_t22h;
  // s_t22h.push_back(foldert2+"*T2tt*175_*-0_*");
  // vector<TString> s_t22m;
  // s_t22m.push_back(foldert2+"*T2tt*150_*-0_*");
  // vector<TString> s_t22ml;
  // s_t22ml.push_back(foldert2+"*T2tt*125_*-0_*");
  // vector<TString> s_t22l;
  // s_t22l.push_back(foldert2+"*T2tt*100_*-0_*");

  vector<TString> s_t22vh;
  s_t22vh.push_back(foldert2+"*T2tt*350_*-100_*");
  vector<TString> s_t22mh;
  s_t22mh.push_back(foldert2+"*T2tt*300_*-100_*");
  vector<TString> s_t22h;
  s_t22h.push_back(foldert2+"*T2tt*275_*-100_*");
  vector<TString> s_t22m;
  s_t22m.push_back(foldert2+"*T2tt*250_*-100_*");
  vector<TString> s_t22ml;
  s_t22ml.push_back(foldert2+"*T2tt*225_*-100_*");
  vector<TString> s_t22l;
  s_t22l.push_back(foldert2+"*T2tt*200_*-100_*");

  // vector<TString> s_t22vh;
  // s_t22vh.push_back(foldert2+"*T2tt*550_*-300_*");
  // vector<TString> s_t22mh;
  // s_t22mh.push_back(foldert2+"*T2tt*500_*-300_*");
  // vector<TString> s_t22h;
  // s_t22h.push_back(foldert2+"*T2tt*475_*-300_*");
  // vector<TString> s_t22m;
  // s_t22m.push_back(foldert2+"*T2tt*450_*-300_*");
  // vector<TString> s_t22ml;
  // s_t22ml.push_back(foldert2+"*T2tt*425_*-300_*");
  // vector<TString> s_t22l;
  // s_t22l.push_back(foldert2+"*T2tt*400_*-300_*");


  // Reading ntuples
  // TString lsp = "#tilde{#chi}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  // TString t1t_label = "#scale[0.92]{#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#bar{t}"+lsp;
  vector<sfeats> Samples; 
  // Samples.push_back(sfeats(s_t2vh, "T2(250,0)", kGreen+1, 1));
  // Samples.push_back(sfeats(s_t2h,  "T2(175,0)", kAzure+2, 1));
  // Samples.push_back(sfeats(s_t2m,  "T2(150,0)", kRed, 1));
  // Samples.push_back(sfeats(s_t2l,  "T2(100,0)", kGreen+3, 1));

  Samples.push_back(sfeats(s_t22vh, "T2(350,100)", kGreen, 1));
  Samples.push_back(sfeats(s_t22mh, "T2(300,100)", kAzure+2, 1));
  Samples.push_back(sfeats(s_t22h,  "T2(275,100)", kBlue, 1));
  Samples.push_back(sfeats(s_t22m,  "T2(250,100)", kRed, 1));
  Samples.push_back(sfeats(s_t22ml, "T2(225,100)", kMagenta, 1));

  //  Samples.push_back(sfeats(s_t22vh, "T2(250,0)", kGreen+1, 1));
  // Samples.push_back(sfeats(s_t22mh, "T2tt(200,0)", kAzure, 1));
  // Samples.push_back(sfeats(s_t22h,  "T2tt(175,0)", kBlue+4, 1));
  // Samples.push_back(sfeats(s_t22m,  "T2tt(150,0)", kRed, 1));
  // Samples.push_back(sfeats(s_t22ml, "T2tt(125,0)", kOrange+10, 1));
  // Samples.push_back(sfeats(s_t22l,  "T2tt(125,0)", kOrange+10, 1));

  //  Samples.push_back(sfeats(s_t22vh, "T2(250,0)", kGreen+1, 1));
  //Samples.push_back(sfeats(s_t22ml, "T2tt(200,0)", kOrange+10, 1));
  // Samples.push_back(sfeats(s_t22l,  "T2tt(100,0)", kOrange+8, 1));
  // Samples.push_back(sfeats(s_t22m,  "T2tt(300,0)", kRed-1, 1));
  // Samples.push_back(sfeats(s_t22h,  "T2tt(600,0)", kBlue, 1));
  // Samples.push_back(sfeats(s_t22mh, "T2tt(950,0)", kGreen+1, 1));

  vector<int> all_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    all_sam.push_back(sam);
  } // Loop over samples


  vector<hfeats> vars;

  vars.push_back(hfeats("mc_pt",60,0,300, all_sam, "LSP p_{T} [GeV]","abs(mc_id)==1000022"));
  vars.back().whichPlots = "34";

  // vars.push_back(hfeats("met",60,0,300, all_sam, "MET [GeV]","1",-1,"mLSP-0"));
  // vars.back().whichPlots = "34";

  // vars.push_back(hfeats("mc_pt",50,0,1000, all_sam, "Stop p_{T} [GeV]","abs(mc_id)==1000006"));
  // vars.back().whichPlots = "34";

  // vars.push_back(hfeats("mc_pt",40,0,900, all_sam, "Stop p_{T} [GeV]","nvleps==0&&ht>500&&njets>=4&&abs(mc_id)==1000006"));
  // vars.back().whichPlots = "34";

  vars.push_back(hfeats("met",40,0,500, all_sam, "MET [GeV]","nvleps==0&&ht>500&&njets>=4"));
  vars.back().whichPlots = "34";
  vars.push_back(hfeats("ht",40,0,2000, all_sam, "H_{T} [GeV]","nvleps==0&&met>200&&njets>=4"));
  vars.back().whichPlots = "34";
  vars.push_back(hfeats("nbm",5,-0.5,4.5, all_sam, "n_{b}","nvleps==0&&ht>500&&met>200&&njets>=4"));
  vars.back().whichPlots = "34";
  vars.push_back(hfeats("njets",12,0.5,12.5, all_sam, "n_{jets}","nvleps==0&&ht>500&&met>200"));
  vars.back().whichPlots = "34";

  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "t2tt",false, true);

}
