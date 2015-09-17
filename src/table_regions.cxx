// table_yields: Generates a LaTeX file with a cutflow table for RA4

#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
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
  TString ntuple_date("2015_05_25");
  TString luminosity="10";
  bool do_1b=false;
  bool do_2l=false;
  bool do_ttbaronly = true;
  bool do_note=true;
  bool do_zbi=false;

  int method = 1; // Only methods 1 and 3 currently supported
}

using namespace std;
using std::cout;
using std::endl;

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig);

int main(){

  // Reading ntuples
  TString folder, folder_ns;  
  folder="/cms5r0/ald77/archive/"+ntuple_date+"/skim/";   
  folder_ns="/cms5r0/ald77/archive/"+ntuple_date+"/";
  vector<TString> s_tt;
  if(!(ntuple_date=="Spring15_Pow")) s_tt.push_back(folder+"*_TTJet*");
  else s_tt.push_back(folder+"*_TT*");
  vector<TString> s_wjets;
  vector<TString> s_single;
  s_single.push_back(folder+"*_T*channel*");
  vector<TString> s_ttv;
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");
  s_other.push_back(folder+"*TTW*");
  s_other.push_back(folder+"*TTZ*");
  s_other.push_back(folder+"*_WJets*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");

  vector<TChain *> chain;
  vector<sfeats> Samples; 
  if(!do_ttbaronly){
    Samples.push_back(sfeats(s_other, "Other", 1001));
    Samples.push_back(sfeats(s_ttv, "$t\\bar{t}V$", 1002));
    Samples.push_back(sfeats(s_single, "Single $t$", 1005));
    Samples.push_back(sfeats(s_wjets, "W+jets", 1004));
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

  TString minjets("7"), midjets("8"), mjthresh("600"), highmet("400"); bool do_nb_binning=false;
  if(method==1) {  minjets = "7"; midjets="8"; mjthresh="600"; highmet="400"; do_nb_binning=false; } 
  if(method==3) {  minjets ="7"; midjets="8"; mjthresh="400"; highmet="400"; do_nb_binning=true; }  


  TString minjets_2l(""), midjets_2l("");
  minjets_2l += (minjets.Atoi()-1); midjets_2l += (midjets.Atoi()-1); 
  TString fom("$Z_{\\rm bi}$");
  if(!do_zbi) fom = "S/B";
  TString outname = "txt/yields_mj"+mjthresh+"_met"+highmet+"_njets"+minjets+midjets+"_lumi"+luminosity+"_"+ntuple_date+".tex";
  if(do_1b) outname.ReplaceAll("yields","yields_1b");
  if(do_2l) outname.ReplaceAll("yields","yields_2l");
  if(do_zbi) outname.ReplaceAll("yields","yields_zbi");

  TString baseline("ht>500&&met>200");
  TString cuts_1l(baseline+"&&(nmus+nels)==1&&nbm>=2&&njets>="+minjets+"");
  TString cuts_1l1b(baseline+"&&(nmus+nels)==1&&nbm==1&&njets>="+minjets+"");
  TString cuts_2l(baseline+"&&(nmus+nels)==2&&nbm==1&&njets>="+minjets_2l+"");
  TString cuts_2lbb(baseline+"&&(nmus+nels)==2&&nbm==2&&njets>="+minjets_2l+"");
  TString cuts_1ltex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq "+minjets+", n_b\\geq 2, n_{\\rm lep}=1$");
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
  for(int sam(0); sam < nsig; sam++) file<<"| r ";
  file<<"}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{${\\cal L} = "<<luminosity<<"$ fb$^{-1}$} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label;//<< " & "+fom;
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
  TString higjets(""); higjets += (midjets.Atoi()+1);
  TString lownj("$n_j\\leq "+midjets+"$");
  TString hignj("$n_j\\geq "+higjets+"$");
  TString lownj_2l("$n_j\\leq "+midjets_2l+"$");
  TString higjets_2l(""); higjets_2l += (midjets_2l.Atoi()+1);
  TString hignj_2l("$n_j\\geq "+higjets_2l+"$");
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

    mjcut="mj<="+mjthresh; mtcut="mt<=140"; 
    file << YieldsCut(regions[0]+": $m_T  \\leq 140,M_J\\leq "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file <<"\\hline\n";
    if(!do_nb_binning){
      file << YieldsCut(indent+lownj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
    } else {
      file << YieldsCut(indent+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+higmet, mjcut+"&&"+mtcut+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
    }
    file <<"\\hline\n";

    mjcut="mj>"+mjthresh; mtcut="mt<=140"; 
    file << YieldsCut(regions[1]+": $m_T  \\leq 140,M_J> "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file <<"\\hline\n";

    if(!do_nb_binning){
      file << YieldsCut(indent+lownj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
    } else {
      file << YieldsCut(indent+lownj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lownj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm==2&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm>=3&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
    }
    file <<"\\hline\n";

    mjcut="mj<="+mjthresh; mtcut="mt>140"; 
    file << YieldsCut(regions[2]+": $m_T  > 140,M_J \\leq "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file <<"\\hline\n";

    if(!do_nb_binning){
      file << YieldsCut(indent+lownj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
    } else {
      file << YieldsCut(indent+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(indent+higmet, mjcut+"&&"+mtcut+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
    }
    file <<"\\hline\n";
    file <<"\\hline\n";


    mjcut="mj>"+mjthresh; mtcut="mt>140"; 
    file << YieldsCut(regions[3]+": $m_T  > 140,M_J > "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file <<"\\hline\n";

    if(!do_nb_binning){
      file << YieldsCut(binnames[0]+lownj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[1]+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[2]+hignj+", "+lowmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[3]+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
    } else {
      file << YieldsCut(binnames[0]+lownj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[1]+lownj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met<="+highmet+"&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[2]+lownj+", "+higmet, mjcut+"&&"+mtcut+"&&njets<="+midjets+"&&met>"+highmet+"&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[3]+hignj+", "+lowmet+", $n_b=2$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm==2&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[4]+hignj+", "+lowmet+", $n_b\\geq 3$", mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met<="+highmet+"&&nbm>=3&&" +cuts_1l, chain, Samples, nsig);
      file << YieldsCut(binnames[5]+hignj+", "+higmet, mjcut+"&&"+mtcut+"&&njets>"+midjets+"&&met>"+highmet+"&&"  +cuts_1l, chain, Samples, nsig);
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
    file << " & "<<Samples[sam].label;//<< " & "+fom;
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file<<footer.rdbuf();
  file.close();
  cout<<endl<<"Written "<<outname<<endl;
}

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig){
  TString totCut, Hname = "histo", out;
  vector<double> yield, error;
  double bkg(0), bkg_err(0), err, yield_sam;
  int nsam(chain.size()), entries(0);
  for(int sam(0); sam < nsam; sam++){
    totCut = luminosity+"*weight*("+cuts+"&&"+Samples[sam].cut+")";
    entries = getYieldErr(*chain[sam], totCut, yield_sam, err);
    yield.push_back(yield_sam);
    error.push_back(err);
    if(sam<nsam-nsig) {
      if(yield[sam]>0) bkg += yield[sam];
      bkg_err = sqrt(pow(bkg_err,2)+pow(err,2));
    }
    cout<<sam<<": yield "<<Samples[sam].label<<" "<<yield[sam]<<", n "<<entries<<" \t "<<totCut<<endl;
  }
  
  cout<<title<<": B = "<<(RoundNumber(bkg,1)+" +- "+RoundNumber(bkg_err,1));
  out = title;
  for(int sam(0); sam < nsam-nsig; sam++) out += (" \t & " + RoundNumber(yield[sam],1));
  out += (" \t &  $"+RoundNumber(bkg,1))+" \\pm "+RoundNumber(bkg_err,1)+"$";
  for(int sam(nsam-nsig); sam < nsam; sam++) {
    float fracerr(sqrt(pow(bkg_err/bkg,2)+0.3*0.3+0.24*0.24));
    out += (" \t& $" + RoundNumber(yield[sam],1)+" \\pm "+RoundNumber(error[sam],1))+"$";// + "$ \t& ");
    // if(do_zbi) out += RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2);
    // else out += RoundNumber(yield[sam],2,bkg);
	    
    cout<<", S = "<<RoundNumber(yield[sam],1)+" +- "+RoundNumber(error[sam],1)<<" with Zbi = ";
    cout<<RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2);
  }
  out += " \\\\ \n";
  cout<<endl;
  return out;
}
