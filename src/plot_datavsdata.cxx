// plot_dps: Macro that plots variables both for comissioning DPS

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
  TString luminosity="1.";
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 

  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder
  
  TString folder1l(bfolder+"/cms2r0/babymaker/babies/2016_02_04/data/singlelep/combined/skim_1lht500met200/");
  TString foldermc(bfolder+"/cms2r0/babymaker/babies/2015_11_28/mc/skim_1lht500met200/");

  vector<TString> s_2016;
  s_2016.push_back("/net/cms2/cms2r0/babymaker/babies/2016_06_14/data/skim_standard/*.root");
  vector<TString> s_2015;
  s_2015.push_back("/net/cms2/cms2r0/babymaker/babies/2016_04_29/data/merged_1lht500met200/*.root");
  //s_2015.push_back("/net/cms2/cms2r0/babymaker/babies/2016_02_04/data/singlelep/combined/skim_1lht500met200/*root"); 

  vector<sfeats> Samples;

  int imj(Samples.size());
  Samples.push_back(sfeats(s_2016, "2016 data (2.1 fb^{-1})", kBlack,1,"(trig[4]||trig[8]||trig[13]||trig[33])&&pass")); Samples.back().isData=true;
  Samples.push_back(sfeats(s_2015, "2015 data (2.3 fb^{-1})", kBlue+2,1,"(trig[4]||trig[8])&&pass")); //Samples.back().isData=true; //
  Samples.back().doBand = true;

   Samples.push_back(sfeats(s_2016, "Dilepton events", kBlack,1,"nonblind&&((nleps==2&&nbm>=0&&nbm<=2&&njets>=5)||(nleps==1&&nveto==1&&nbm>=1&&nbm<=2&&njets>=6&&mt>140))&&(trig[4]||trig[8]||trig[13]||trig[33])&&pass")); Samples.back().isData=true;
    Samples.push_back(sfeats(s_2016, "Single lepton events, m_{T} < 140", kBlue+2,1,"nonblind&&nleps==1&&nveto==0&&nbm>=1&&njets>=6&&mt<=140&&(trig[4]||trig[8]||trig[13]||trig[33])&&pass"));   Samples.back().doBand = true;
  
  vector<int> mj_sam;
  mj_sam.push_back(imj);
  mj_sam.push_back(imj+1);

  vector<int> dilep_comp;
  dilep_comp.push_back(imj+2);
  dilep_comp.push_back(imj+3);

  vector<hfeats> vars;
  plot_style="CMSPaperNoRatio";
  vars.push_back(hfeats("mj14",10,25,775, mj_sam, "M_{J} [GeV]",
			"nleps==1&&ht>500&&met>200&&njets>=6&&nbm>=1&&mt<140",400,"results"));

  vars.push_back(hfeats("mj14",10,25,775, dilep_comp, "M_{J} [GeV]",
			"ht>500&&met>200&&met<500",400,"results"));
  vars.back().normalize=true;

  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "datavsdata", false, true);
  

}
