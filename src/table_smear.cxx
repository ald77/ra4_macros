// table_yields: Generates a LaTeX file with a cutflow table for RA4

#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <unistd.h> // getopt in Macs

#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH1D.h"
#include "TMath.h"
#include "RooStats/NumberCountingUtils.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace  {
  TString ntuple_date("2015_10_19");
  TString luminosity="3";
  TString tag = "";
  bool do_1b=false;
  bool do_2l=false;
  bool do_ttbaronly = false;
  bool do_note=true;
  bool do_zbi=false;
  bool do_soverb=false;
  bool print_full(true);

  int method = 0; // Only methods 1 and 2 currently supported

  TString minjets("6");
  TString highjets("9");
  TString mjthresh("400");
  TString highmet("400"); 
  int do_nb_binning=0; // 0 = No Nb binning. 1 = Nb binning at low MET. 2 = Nb binning at all MET. 
}

using namespace std;
using std::cout;
using std::endl;

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig);

int main(int argc, char *argv[]){

  int c(0);
  while((c=getopt(argc, argv, "m:oz:t:j:h:f:e:b:p:"))!=-1){
    switch(c){
    case 'm':
      method=atoi(optarg);
      break;
    case 'o':
      do_ttbaronly = true;
      break; 
    case 'z':
      if(0==atoi(optarg)) do_zbi=false;
      if(1==atoi(optarg)) do_zbi=true;
      break;
    case 'j':
      minjets = optarg;
      break;
    case 'h':
      highjets = optarg;
      break;
    case 'f':
      mjthresh = optarg;
      break;
    case 'e':
      highmet = optarg;
      break;
    case 'b':
      do_nb_binning = atoi(optarg);
      break;
    case 't':
      tag = optarg;
      break;
    case 'p':
      print_full = true;
      break; 
  default:
      break;
    }
  }

  // Reading ntuples
  TString folder ="/afs/cern.ch/work/m/manuelf/public/babies/2015_10_19/mc/skim_1lht500met200/";
  // TString folder_ttv = "~manuelf/work/babies/2015_10_19/mc/skim_1lht400/";
  TString folder_other="/afs/cern.ch/user/a/ana/public/151028_230803/";
  // folder = "archive/2015_10_19/mc/skim_1lht500met200/";

  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJets*Lept*");
  s_tt.push_back(folder+"*_TTJets_HT*");
  vector<TString> s_other;
  s_other.push_back(folder_other+"*_TTJets*Lept*");
  s_other.push_back(folder_other+"*_TTJets_HT*");
  // vector<TString> s_wjets;
  // s_wjets.push_back(folder+"*_WJetsToLNu*");
  // vector<TString> s_ttv;
  // s_ttv.push_back(folder_ttv+"*_TTWJets*");
  // s_ttv.push_back(folder_ttv+"*_TTZTo*");
  // vector<TString> s_single;
  // s_single.push_back(folder+"*_ST_*");
  // vector<TString> s_other;
  // s_other.push_back(folder+"*DYJetsToLL*");
  // s_other.push_back(folder+"*_QCD_HT*");
  // s_other.push_back(folder+"*_ZJet*");
  // s_other.push_back(folder+"*_WWTo*");
  // s_other.push_back(folder+"*ggZH_HToBB*");
  // s_other.push_back(folder+"*ttHJetTobb*");
  // s_other.push_back(folder_ttv+"*_TTWJets*");
  // s_other.push_back(folder_ttv+"*_TTZTo*");
  // s_other.push_back(folder+"*_WJetsToLNu*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*");

  vector<TChain *> chain;
  vector<sfeats> Samples; 
  if(!do_ttbaronly){
    // Samples.push_back(sfeats(s_other, "Smeared $t\\bar{t}$ (1$\\ell$)", 1001,1,"(ntruleps==1)*(1+0.5*((Max$(jets_pt_res)>1.276)||(Min$(jets_pt_res)<0.824)))"));
    // Samples.push_back(sfeats(s_other, "Smeared $t\\bar{t}$ (2$\\ell$)", 1001,1,"(ntruleps==2)*(1+0.5*((Max$(jets_pt_res)>1.276)||(Min$(jets_pt_res)<0.824)))"));
    Samples.push_back(sfeats(s_other, "Smeared $t\\bar{t}$ (1$\\ell$)", 1001,1,"ntruleps==1"));
    Samples.push_back(sfeats(s_other, "Smeared $t\\bar{t}$ (2$\\ell$)", 1001,1,"ntruleps==2"));
    // Samples.push_back(sfeats(s_ttv, "$t\\bar{t}V$", 1002));
    // Samples.push_back(sfeats(s_single, "Single $t$", 1005));
    // Samples.push_back(sfeats(s_wjets, "W+jets", 1004));
  }
  Samples.push_back(sfeats(s_tt, "$t\\bar{t}$ (1$\\ell$)", 1000,1,
			   "ntruleps<=1"));
  Samples.push_back(sfeats(s_tt, "$t\\bar{t}$ ($2\\ell$)", 1006,1,
			   "ntruleps>=2"));
  int nsig(2);
  Samples.push_back(sfeats(s_t1t, "T1tttt NC", 2));
  Samples.push_back(sfeats(s_t1tc, "T1tttt C", 2,2));

  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  if(method==1) {  minjets = "6"; highjets="9"; mjthresh="600"; highmet="400"; do_nb_binning=0; } 
  if(method==2) {  minjets ="6"; highjets="9"; mjthresh="400"; highmet="400"; do_nb_binning=1; }  


  TString midjets(""); midjets += (highjets.Atoi()-1); 
  TString minjets_2l(""), midjets_2l("");
  minjets_2l += (minjets.Atoi()-1); midjets_2l += (midjets.Atoi()-1); 
  TString fom("$Z_{\\rm bi}$");
  if(do_soverb) fom = "S/B";
  TString lumi_s(luminosity); lumi_s.ReplaceAll(".","p");
  TString outname = "txt/yields_mj"+mjthresh+"_met"+highmet+"_njets"+minjets+highjets+"_lumi"+lumi_s+"_"+ntuple_date+tag+".tex";
  if(do_1b) outname.ReplaceAll("yields","yields_1b");
  if(do_2l) outname.ReplaceAll("yields","yields_2l");
  if(do_zbi) outname.ReplaceAll("yields","yields_zbi");

  TString baseline("ht>500&&met>200");
  TString cuts_1l(baseline+"&&(nmus+nels)==1&&nbm>=1&&njets>="+minjets+"");
  TString cuts_1l1b(baseline+"&&(nmus+nels)==1&&nbm==1&&njets>="+minjets+"");
  TString cuts_2l(baseline+"&&(nmus+nels)==2&&nbm==1&&njets>="+minjets_2l+"");
  TString cuts_2lbb(baseline+"&&(nmus+nels)==2&&nbm==2&&njets>="+minjets_2l+"");
  TString cuts_1ltex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq "+minjets+", n_b\\geq 1, n_{\\rm lep}=1$");
  TString cuts_1l1btex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq "+minjets+", n_b=1, n_{\\rm lep}=1$");
  TString cuts_2ltex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq "+minjets_2l+", n_b=1, n_{\\rm lep}=2$");
  TString cuts_2lbbtex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq "+minjets_2l+", n_b=2, n_{\\rm lep}=2$");
  if(do_1b) {
    cuts_1l.ReplaceAll("nbm>=2","nbm==1");
    cuts_1ltex.ReplaceAll("n_b\\geq 2", "n_b=1");
  }
  ifstream header("txt/header.tex");
  ifstream footer("txt/footer.tex");
  ofstream file(outname);
  file<<header.rdbuf();
  file<<"\\vspace{80 mm}\n";
  file << "\n\\begin{tabular}[tbp!]{ l | ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++) file << "r";
  file<<" | r ";
  for(int sam(0); sam < nsig; sam++) {
    if (do_zbi || do_soverb) file<<"| r | r";
    else  file<<"| r ";
  }
  file<<"}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{${\\cal L} = "<<luminosity<<"$ fb$^{-1}$} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++) {
    file << " & "<<Samples[sam].label;
    if (do_zbi || do_soverb) file << " & "+fom;
  }
  file << "\\\\ \\hline \n ";

  TString mtcut, mjcut;
  TString indent("\\hspace{5 mm} ");
  if(do_note) indent = "";
  vector<TString> binnames;
  for(unsigned bin(1); bin <= 6; bin++)
    if(do_note) binnames.push_back("");
    else {
      TString name("Bin "); name += bin; name += ": ";
      binnames.push_back(name);
    }
  TString lownj("$n_j\\leq "+midjets+"$");
  TString hignj("$n_j\\geq "+highjets+"$");
  TString lownj_2l("$n_j\\leq "+midjets_2l+"$");
  TString highjets_2l(""); highjets_2l += (midjets_2l.Atoi()+1);
  TString hignj_2l("$n_j\\geq "+highjets_2l+"$");
  TString lowmet("$\\text{MET}\\leq "+highmet+"$");
  TString higmet("$\\text{MET}> "+highmet+"$");
  TString letter("R"); 
  if(do_1b) letter = "B"; 
  if(do_2l) letter = "D";
  TString regions[4];
  for(int ind(0); ind<4; ind++) {regions[ind] = letter; regions[ind] += (ind+1);}
  //////////////////////////////////// RA4 regions //////////////////////////////////
  if(!do_2l){
    file << " \\multicolumn{"<< Samples.size()+1<<"}{c}{"<< cuts_1ltex  <<"} \\\\ \\hline \\hline\n";

    // R1 region
    mjcut="mj<="+mjthresh; mtcut="mt<=140"; 
    file << YieldsCut(regions[0]+": $m_T  \\leq 140,M_J\\leq "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file <<"\\hline\n";
    if(print_full){
      if(do_nb_binning==0){
	file << YieldsCut(indent+lownj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&"+cuts_1l, chain, Samples, nsig);
	file << YieldsCut(indent+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
	file << YieldsCut(indent+hignj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&" +cuts_1l, chain, Samples, nsig);
	file << YieldsCut(indent+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
      } 
      else if(do_nb_binning==1) {
	file << YieldsCut(indent+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
	file << YieldsCut(indent+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
	file << YieldsCut(indent+higmet, mjcut+"&&"+mtcut+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      }
      else if(do_nb_binning==2) {
	file << YieldsCut(indent+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
	file << YieldsCut(indent+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
	file << YieldsCut(indent+higmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&met>"+highmet+"&&nbm==2&&" +cuts_1l, chain, Samples, nsig);
	file << YieldsCut(indent+higmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&met>"+highmet+"&&nbm>=3&&" +cuts_1l, chain, Samples, nsig);
      }
      file <<"\\hline\n";
    } // if(print_full)

    // R2 region
    mjcut="mj>"+mjthresh; mtcut="mt<=140"; 
    file << YieldsCut(regions[1]+": $m_T  \\leq 140,M_J> "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file <<"\\hline\n";

    if(print_full){
    if(do_nb_binning==0){
      file << YieldsCut(indent+lownj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
    } 
    else if(do_nb_binning==1) {
      file << YieldsCut(indent+lownj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lownj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm==2&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm>=3&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
    }
    else if(do_nb_binning==2) {
      file << YieldsCut(indent+lownj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lownj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lownj+", "+higmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&nbm==2&&" +cuts_1l, chain, Samples, nsig);   
      file << YieldsCut(indent+lownj+", "+higmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&nbm>=3&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm==2&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm>=3&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+higmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&nbm==2&&"  +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+higmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&nbm>=3&&"  +cuts_1l, chain, Samples, nsig);
    }

    file <<"\\hline\n";
    } // if(print_full)

    // R3 region
    mjcut="mj<="+mjthresh; mtcut="mt>140"; 
    file << YieldsCut(regions[2]+": $m_T  > 140,M_J \\leq "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file <<"\\hline\n";

    if(do_nb_binning==0){
      file << YieldsCut(indent+lownj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
    } 
    else if(do_nb_binning==1) {
      file << YieldsCut(indent+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+higmet, mjcut+"&&"+mtcut+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
    }
    else if(do_nb_binning==2) {
      file << YieldsCut(indent+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+higmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&met>"+highmet+"&&nbm==2&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+higmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&met>"+highmet+"&&nbm>=3&&" +cuts_1l, chain, Samples, nsig);
    }

    file <<"\\hline\n";
    file <<"\\hline\n";


    // R4 region
    mjcut="mj>"+mjthresh; mtcut="mt>140"; 
    file << YieldsCut(regions[3]+": $m_T  > 140,M_J > "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file <<"\\hline\n";

    if(do_nb_binning==0){
      file << YieldsCut(binnames[0]+lownj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[1]+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[2]+hignj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[3]+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
    } 
    else if(do_nb_binning==1){
      file << YieldsCut(binnames[0]+lownj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[1]+lownj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[2]+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[3]+hignj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm==2&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[4]+hignj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm>=3&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[5]+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
    }
    else if(do_nb_binning==2){
      file << YieldsCut(binnames[0]+lownj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[1]+lownj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[2]+lownj+", "+higmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&nbm==2&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[2]+lownj+", "+higmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&nbm>=3&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[3]+hignj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm==2&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[4]+hignj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm>=3&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[5]+hignj+", "+higmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&nbm==2&&"  +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[5]+hignj+", "+higmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&nbm>=3&&"  +cuts_1l, chain, Samples, nsig);

    }

    file <<"\\hline\n";

  } else {
    file << "\\hline \\hline\\multicolumn{"<< Samples.size()+1<<"}{c}{"
	 <<cuts_2ltex <<"} \\\\ \\hline \\hline\n";

    mjcut="mj<="+mjthresh; 
    file << YieldsCut(regions[2]+": $M_J \\leq "+mjthresh+"$", "mj <= "+mjthresh+"&&"+cuts_2l, chain, Samples, nsig);
    file <<"\\hline\n";
    file << YieldsCut(indent+lownj_2l+", "+lowmet, mjcut+"&&njets<="+midjets_2l+"&&met<="+highmet+"&&"+cuts_2l, chain, Samples, nsig);
    file << YieldsCut(indent+lownj_2l+", "+higmet, mjcut+"&&njets<="+midjets_2l+"&&met>"+highmet+"&&" +cuts_2l, chain, Samples, nsig);
    file << YieldsCut(indent+hignj_2l+", "+lowmet, mjcut+"&&njets>"+midjets_2l+"&&met<="+highmet+"&&" +cuts_2l, chain, Samples, nsig);
    file << YieldsCut(indent+hignj_2l+", "+higmet, mjcut+"&&njets>"+midjets_2l+"&&met>"+highmet+"&&"  +cuts_2l, chain, Samples, nsig);
    file <<"\\hline\n";


    mjcut="mj>"+mjthresh; 
    file << YieldsCut(regions[3]+": $M_J > "+mjthresh+"$", "mj>"+mjthresh+"&&"+cuts_2l, chain, Samples, nsig);
    file <<"\\hline\n";
    file << YieldsCut(indent+lownj_2l+", "+lowmet, mjcut+"&&njets<="+midjets_2l+"&&met<="+highmet+"&&"+cuts_2l, chain, Samples, nsig);
    file << YieldsCut(indent+lownj_2l+", "+higmet, mjcut+"&&njets<="+midjets_2l+"&&met>"+highmet+"&&" +cuts_2l, chain, Samples, nsig);
    file << YieldsCut(indent+hignj_2l+", "+lowmet, mjcut+"&&njets>"+midjets_2l+"&&met<="+highmet+"&&" +cuts_2l, chain, Samples, nsig);
    file << YieldsCut(indent+hignj_2l+", "+higmet, mjcut+"&&njets>"+midjets_2l+"&&met>"+highmet+"&&"  +cuts_2l, chain, Samples, nsig);

    file << "\\hline \\hline\\multicolumn{"<< Samples.size()+1<<"}{c}{"
	 << cuts_2lbbtex<<"} \\\\ \\hline \\hline\n";

    mjcut="mj<="+mjthresh; 
    file << YieldsCut(regions[2]+": $M_J \\leq "+mjthresh+"$", "mj<="+mjthresh+"&&"+cuts_2lbb, chain, Samples, nsig);
    file <<"\\hline\n";
    file << YieldsCut(indent+lownj_2l+", "+lowmet, mjcut+"&&njets<="+midjets_2l+"&&met<="+highmet+"&&"+cuts_2lbb, chain, Samples, nsig);
    file << YieldsCut(indent+lownj_2l+", "+higmet, mjcut+"&&njets<="+midjets_2l+"&&met>"+highmet+"&&" +cuts_2lbb, chain, Samples, nsig);
    file << YieldsCut(indent+hignj_2l+", "+lowmet, mjcut+"&&njets>"+midjets_2l+"&&met<="+highmet+"&&" +cuts_2lbb, chain, Samples, nsig);
    file << YieldsCut(indent+hignj_2l+", "+higmet, mjcut+"&&njets>"+midjets_2l+"&&met>"+highmet+"&&"  +cuts_2lbb, chain, Samples, nsig);
    file <<"\\hline\n";

    mjcut="mj>"+mjthresh; 
    file << YieldsCut(regions[3]+": $M_J > "+mjthresh+"$", "mj>"+mjthresh+"&&"+cuts_2lbb, chain, Samples, nsig);
    file <<"\\hline\n";
    file << YieldsCut(indent+lownj_2l+", "+lowmet, mjcut+"&&njets<="+midjets_2l+"&&met<="+highmet+"&&"+cuts_2lbb, chain, Samples, nsig);
    file << YieldsCut(indent+lownj_2l+", "+higmet, mjcut+"&&njets<="+midjets_2l+"&&met>"+highmet+"&&" +cuts_2lbb, chain, Samples, nsig);
    file << YieldsCut(indent+hignj_2l+", "+lowmet, mjcut+"&&njets>"+midjets_2l+"&&met<="+highmet+"&&" +cuts_2lbb, chain, Samples, nsig);
    file << YieldsCut(indent+hignj_2l+", "+higmet, mjcut+"&&njets>"+midjets_2l+"&&met>"+highmet+"&&"  +cuts_2lbb, chain, Samples, nsig);
  }

  file << "\\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label;
  if (do_zbi || do_soverb) file << " & "+fom;
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file<<footer.rdbuf();
  file.close();
  cout<<endl<<" pdflatex "<<outname<<endl<<endl;
}

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig){
  TString totCut, Hname = "histo", out;
  vector<double> yield, error;
  double bkg(0), bkg_err(0), err, yield_sam;
  int nsam(chain.size()), entries(0);
  cout<<entries<<endl; // So it compiles
  for(int sam(0); sam < nsam; sam++){
    if (Samples[sam].label.Contains("Smeared"))
      totCut = luminosity+"*(1/1.0287)*weight*(1+0.5*((Max$(jets_pt_res)>1.391)||Min$(jets_pt_res)<0.675))*("+cuts+"&&"+Samples[sam].cut+")";
      //totCut = luminosity+"*weight*(1+0.5*((Max$(jets_pt_res)>1.334)||Min$(jets_pt_res)<0.777))*("+cuts+"&&"+Samples[sam].cut+")";
    //totCut = luminosity+"*weight*("+cuts+"&&"+Samples[sam].cut+")";
    else
      totCut = luminosity+"*weight*("+cuts+"&&"+Samples[sam].cut+")";
    entries = getYieldErr(*chain[sam], totCut, yield_sam, err);
    yield.push_back(yield_sam);
    error.push_back(err);
    if(sam<nsam-nsig) {
      if(yield[sam]>0) bkg += yield[sam];
      bkg_err = sqrt(pow(bkg_err,2)+pow(err,2));
    }
    //    cout<<sam<<": yield "<<Samples[sam].label<<" "<<yield[sam]<<", n "<<entries<<" \t "<<totCut<<endl;
  }
  
  int digits(1);
  cout<<title<<": B = "<<(RoundNumber(bkg,digits)+" +- "+RoundNumber(bkg_err,digits));
  out = title;
  for(int sam(0); sam < nsam-nsig; sam++) out += (" \t & " + RoundNumber(yield[sam],digits));
  out += (" \t &  $"+RoundNumber(bkg,digits))+" \\pm "+RoundNumber(bkg_err,digits)+"$";
  for(int sam(nsam-nsig); sam < nsam; sam++) {
    float fracerr(sqrt(pow(bkg_err/bkg,2)+0.3*0.3+0.24*0.24));
    out += (" \t& $" + RoundNumber(yield[sam],digits)+" \\pm "+RoundNumber(error[sam],digits) + "$");
    if(do_zbi) out += " \t& " + RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2);
    else if (do_soverb) out += " \t& " + RoundNumber(yield[sam],2,bkg);   
    
    cout<<", S = "<<RoundNumber(yield[sam],digits)+" +- "+RoundNumber(error[sam],digits)<<" with Zbi = ";
    cout<<RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2);
  }
  out += " \\\\ \n";
  cout<<endl;
  return out;
}
