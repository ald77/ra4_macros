// plot_dy_mj1: Macro that makes plots mJ1 for MJs with and without leptons, and also 
//              MJ/mJ1 for a DY selection for different numbers of jets


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
  TString luminosity="2.3";
  TString plot_type=".pdf";
  TString plot_style="CMSPaper_Supplementary";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 

  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder
  
  TString folder1l_zisr(bfolder+"/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/skim_dy_ht300/"); //skim doesn't exist
  TString foldermc_zisr(bfolder+"/cms2r0/babymaker/babies/2015_11_28/mc/skim_dy_ht300/");
  TString foldermj="/net/cms5/cms5r0/manuelf/root/archive/15-01-30/";
  TString foldermjnew(bfolder+"/cms2r0/babymaker/babies/2015_11_28/mc/");
  
  vector<TString> s_slep_dy;
  s_slep_dy.push_back(folder1l_zisr+"*root"); 

  vector<TString> s_tt_dy;
  s_tt_dy.push_back(foldermc_zisr+"*_TTJets*Lept*");
  s_tt_dy.push_back(foldermc_zisr+"*_TTJets_HT*");

  vector<TString> s_ttv_dy;
  s_ttv_dy.push_back(foldermc_zisr+"*_TTWJets*");
  s_ttv_dy.push_back(foldermc_zisr+"*_TTZTo*");
  
  vector<TString> s_dyjets;
  s_dyjets.push_back(foldermc_zisr+"*_DYJetsToLL*");

  vector<TString> s_other_dy;
  s_other_dy.push_back(foldermc_zisr+"*_WJetsToLNu*");
  s_other_dy.push_back(foldermc_zisr+"*_QCD_HT*");
  s_other_dy.push_back(foldermc_zisr+"*_ZJet*");
  s_other_dy.push_back(foldermc_zisr+"*_WWTo*");
  s_other_dy.push_back(foldermc_zisr+"*ggZH_HToBB*");
  s_other_dy.push_back(foldermc_zisr+"*ttHJetTobb*");
  s_other_dy.push_back(foldermc_zisr+"*_ST_*");


  // ttbar selection
  TString lsp = "{#lower[-0.1]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}}";
  TString t1t_label = "#scale[0.95]{#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#kern[0.18]{#bar{t}}#kern[0.18]"+lsp;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_slep_dy, "Data", kBlack,1,"(trig[4]||trig[8])&&pass")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_dyjets, "Z+jets", dps::c_wjets,1,"stitch"));
  Samples.push_back(sfeats(s_tt_dy, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  Samples.push_back(sfeats(s_tt_dy, "t#bar{t}, 1 true lepton", dps::c_tt_1l,1,"ntruleps<=1&&stitch"));
  Samples.push_back(sfeats(s_ttv_dy, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_other_dy, "Other", 2001, 1));

  vector<int> ra4_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
  } // Loop over samples


  //// Samples for m(J1) (old MJ ntuples)
  vector<TString> s_dyjetsmj;
  s_dyjetsmj.push_back(foldermj+"*_DYJetsToLL_M-50_HT-*");
  TString mj_s("M#lower[-.1]{_{J}}"), nj_s("N#lower[-.1]{_{jets}}"), ht_s("H#lower[-.1]{_{T}}");
  int idy(Samples.size());
  Samples.push_back(sfeats(s_dyjetsmj, mj_s+" without leptons", kRed+1,1,"1", "fjets_nolep_30_m[0]"));
  Samples.push_back(sfeats(s_dyjetsmj, mj_s+" with RA4 leptons", kGreen+1,1,"1", "fjets_siglep_30_m[0]"));
  Samples.push_back(sfeats(s_dyjetsmj, mj_s+" with jets-from-leptons", kAzure+2,1,"1", "fjets_30_m[0]"));

  vector<int> dy_sam, dy_mjs_sam;
  for(unsigned sam(idy); sam < Samples.size(); sam++){
    dy_mjs_sam.push_back(sam);
  } // Loop over samples

  idy = Samples.size();
  Samples.push_back(sfeats(s_dyjetsmj, "Z+jets ("+mj_s+" with jets-from-leptons)", kAzure+2,1,"1"));
  dy_sam.push_back(idy);

  //// Samples for m(J1) (new MJ ntuples)
  vector<TString> s_dyjetsmjnew;
  s_dyjetsmjnew.push_back(foldermjnew+"*_DYJetsToLL_M-50_HT-*");
  idy = Samples.size();
  Samples.push_back(sfeats(s_dyjetsmjnew, "Z+jets ("+mj_s+" with jets-from-leptons)", kAzure+2,1,"1"));

  vector<int> dynew_sam;
  for(unsigned sam(idy); sam < Samples.size(); sam++){
    dynew_sam.push_back(sam);
  } // Loop over samples


  vector<hfeats> vars;
  
  vars.push_back(hfeats("fjets_30_phi[0]-mc_phi*(mc_id==23)",64,-3.2,3.2, dy_sam, "#Delta#phi(J_{1},Z)", 
  			"fjets_30_m[0]>95&&fjets_30_m[0]<105",-1,"old_mj"));
  vars.back().whichPlots = "3"; vars.back().addOverflow = false;
  vars.push_back(hfeats("fjets_30_eta[0]-mc_eta*(mc_id==23)",64,-3.2,3.2, dy_sam, "#Delta#eta(J_{1},Z)", 
  			"fjets_30_m[0]>95&&fjets_30_m[0]<105",-1,"old_mj"));
  vars.back().whichPlots = "3"; vars.back().addOverflow = false;
  vars.push_back(hfeats("fjets_30_m[0]",75,65,140, dy_mjs_sam, "m(J_{1}) [GeV]", "1",-1,"old_mj"));
  vars.back().whichPlots = "3"; vars.back().addOverflow = false;

  vars.push_back(hfeats("fjets_nconst[0]",4,0.5,4.5, dynew_sam, "Number of AK4 jets in J_{1}", 
  			"fjets_m[0]>95&&fjets_m[0]<105",-1,"new_mj"));
  vars.back().whichPlots = "3"; 
  vars.push_back(hfeats("fjets_m[0]",75,65,140, dynew_sam, "m(J_{1}) [GeV]", "1",-1,"new_mj"));
  vars.back().whichPlots = "3"; vars.back().addOverflow = false;
  vars.push_back(hfeats("fjets_phi[0]-mc_phi*(mc_id==23)",64,-3.2,3.2, dynew_sam, "#Delta#phi(J_{1},Z)", 
  			"fjets_m[0]>95&&fjets_m[0]<105",-1,"new_mj"));
  vars.back().whichPlots = "3"; vars.back().addOverflow = false;
  vars.push_back(hfeats("fjets_eta[0]-mc_eta*(mc_id==23)",64,-3.2,3.2, dynew_sam, "#Delta#eta(J_{1},Z)", 
  			"fjets_m[0]>95&&fjets_m[0]<105",-1,"new_mj"));
  vars.back().whichPlots = "3"; vars.back().addOverflow = false;



  plot_distributions(Samples, vars, luminosity, plot_type, "RA4", "dy_mj1",false);

  vars.clear();
  TString mll("(mumu_m*(mumu_m>0&&mumu_pt1>20)+elel_m*(elel_m>0&&elel_pt1>20))>80&&(mumu_m*(mumu_m>0&&mumu_pt1>20)+elel_m*(elel_m>0&&elel_pt1>20))<100");

  vars.push_back(hfeats("fjets_m[0]",20,0,500, ra4_sam, "m(J_{1}) [GeV]",
			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",24,0,600, ra4_sam, "M_{J} [GeV]",
   			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_m[0]",20,0,500, ra4_sam, "m(J_{1}) [GeV]",
			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets==5&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",24,0,600, ra4_sam, "M_{J} [GeV]",
   			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets==5&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_m[0]",20,0,500, ra4_sam, "m(J_{1}) [GeV]",
			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets>=2&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",24,0,600, ra4_sam, "M_{J} [GeV]",
   			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets>=2&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_m[0]",20,0,500, ra4_sam, "m(J_{1}) [GeV]",
			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets==2&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",24,0,600, ra4_sam, "M_{J} [GeV]",
   			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets==2&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_m[0]",20,0,500, ra4_sam, "m(J_{1}) [GeV]",
			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets==3&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",24,0,600, ra4_sam, "M_{J} [GeV]",
   			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets==3&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("fjets_m[0]",20,0,500, ra4_sam, "m(J_{1}) [GeV]",
			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets==4&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",24,0,600, ra4_sam, "M_{J} [GeV]",
   			"pass&&ht>500&&(nmus>=2||nels>=2)&&njets==4&&"+mll));
  vars.back().whichPlots = "12"; vars.back().normalize = true;




  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "dy_mj1",true);
}
