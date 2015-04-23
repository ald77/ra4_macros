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

using namespace std;
using std::cout;
using std::endl;

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig);

int main(){

  // Reading ntuples
  TString folder="archive/15-04-07/skims/";
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*_WJets*");
  vector<TString> s_single;
  s_single.push_back(folder+"*_T*channel*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder+"*TTW*");
  s_ttv.push_back(folder+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD_HT*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");

  vector<TChain *> chain;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_other, "QCD+other", 1001));
  Samples.push_back(sfeats(s_ttv, "$t\\bar{t}V$", 1002));
  Samples.push_back(sfeats(s_single, "Single $t$", 1005));
  Samples.push_back(sfeats(s_wjets, "W+jets", 1004));
  Samples.push_back(sfeats(s_tt, "$t\\bar{t}$ (1$\\ell$)", 1000,1,
			   "((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)<=1"));
  Samples.push_back(sfeats(s_tt, "$t\\bar{t}$ ($2\\ell$)", 1006,1,
			   "((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)>=2"));
  int nsig(2);
  Samples.push_back(sfeats(s_t1t, "T1tttt NC", 2));
  Samples.push_back(sfeats(s_t1tc, "T1tttt C", 2,2));

  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  
  TString name = "txt/ra4_cutflow.tex";
  ofstream file(name);
  TString cuts("(nmus+nels)==1&&ht>500&&met>250&&nbm>=1&&njets>=4");
  
  file << "\n\\begin{tabular}{ l | rrrrrr | r | rr | rr}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{Cuts} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & S/B ";
  file << "\\\\ \\hline \n ";

  //////////////////////////////////// Cutflow for Jae //////////////////////////////////
  //  file << " \\multicolumn{"<< Samples.size()+nsig*2<<"}{c}{"<< "$H_T>500, \\mathrm{MET}>200, m_T>150, n_{\\rm jets}\\geq 6, " <<"n_b=1, n_{\\rm lep}=1$"<<"} \\\\ \\hline\n";
  //file << YieldsCut("1 lepton, $H_T>500,{\\rm MET}>250,n_{\\rm jets}\\geq 4, n_b\\geq 1$", cuts, 
  file << YieldsCut("Baseline", cuts, 
		    chain, Samples, nsig);
  file << YieldsCut("+ $n_{\\rm jets}\\geq 6$", cuts+"&&njets>=6", 
		    chain, Samples, nsig);
  file << YieldsCut("+ $n_{b}\\geq 2$", cuts+"&&njets>=6&&nbm>=2", 
		    chain, Samples, nsig);
  file << YieldsCut("+ $m_T \\geq 150$", cuts+"&&njets>=6&&nbm>=2&&mt>=150", 
		    chain, Samples, nsig);
  file << YieldsCut("+ $M_J \\geq 600$", cuts+"&&njets>=6&&nbm>=2&&mt>=150&&mj>=600", 
		    chain, Samples, nsig);
  file << YieldsCut("+ MET$ \\geq 400$", cuts+"&&njets>=6&&nbm>=2&&mt>=150&&mj>=600&&met>=400", 
		    chain, Samples, nsig);
  //////////////////////////////////// Cutflow for Jae //////////////////////////////////






  file << " \\hline\\multicolumn{1}{c|}{Cuts} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & S/B ";
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file.close();
  cout<<"Written "<<name<<endl;
}

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig){
  TString totCut, luminosity="4", Hname = "histo", out;
  vector<double> yield, error;
  double bkg(0), bkg_err(0), err;
  int nsam(chain.size());
  TH1D histo(Hname, "",100, 0, 10000);
  histo.Sumw2();
  for(int sam(0); sam < nsam; sam++){
    totCut = luminosity+"*weight*("+cuts+"&&"+Samples[sam].cut+")";
    chain[sam]->Project(Hname, "met", totCut);
    yield.push_back(histo.IntegralAndError(0,101,err));
    error.push_back(err);
    if(sam<nsam-nsig) {
      bkg += yield[sam];
      bkg_err = sqrt(pow(bkg_err,2)+pow(err,2));
    }
    //cout<<sam<<": yield "<<Samples[sam].label<<" "<<yield[sam]<<" \t "<<totCut<<endl;
  }
  
  cout<<title<<": B = "<<(RoundNumber(bkg,1)+" +- "+RoundNumber(bkg_err,1));
  out = title;
  for(int sam(0); sam < nsam-nsig; sam++) out += (" \t & " + RoundNumber(yield[sam],1));
  out += (" \t &  $"+RoundNumber(bkg,1))+" \\pm "+RoundNumber(bkg_err,1)+"$";
  for(int sam(nsam-nsig); sam < nsam; sam++) {
    out += (" \t& $" + RoundNumber(yield[sam],1)+" \\pm "+RoundNumber(error[sam],1) + "$ \t& " +
	    RoundNumber(yield[sam],2,bkg));
    cout<<", S = "<<RoundNumber(yield[sam],1)+" +- "+RoundNumber(error[sam],1)<<" with Zbi = ";
    cout<<RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, 
								  sqrt(pow(bkg_err/bkg,2)+0.3*0.3+0.24*0.24)),2);
  }
  out += " \\\\ \n";
  cout<<endl;
  return out;
}
