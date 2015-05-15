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

namespace ra4 {
  TString luminosity="10";
  bool do_zbi=false;
  bool do_8j=false;
}

using namespace ra4;

using namespace std;
using std::cout;
using std::endl;

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig);

int main(){

  // Reading ntuples
  TString folder="/cms5r0/ald77/archive/2015_05_10/skim/";
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
  Samples.push_back(sfeats(s_other, "Other", 1001));
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

  TString fom("$Z_{\\rm bi}$");
  if(!do_zbi) fom = "S/B";
  TString name = "txt/ra4_regions_lumi"+luminosity+".tex";
  TString cuts_1l("(nmus+nels)==1&&ht>500&&met>200&&nbm>=2&&njets>=6");
  TString cuts_1l1b("(nmus+nels)==1&&ht>500&&met>200&&nbm==1&&njets>=6");
  TString cuts_2l("(nmus+nels)==2&&ht>500&&met>200&&nbm==1&&njets>=5");
  TString cuts_2lbb("(nmus+nels)==2&&ht>500&&met>200&&nbm==2&&njets>=5");
  TString cuts_1ltex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq 6, n_b\\geq 2, n_{\\rm lep}=1$");
  TString cuts_1l1btex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq 6, n_b=1, n_{\\rm lep}=1$");
  TString cuts_2ltex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq 5, n_b=1, n_{\\rm lep}=2$");
  TString cuts_2lbbtex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq 5, n_b=2, n_{\\rm lep}=2$");
  if(do_8j){
    name.ReplaceAll("regions","regions_8j");
    cuts_1l.ReplaceAll(">=6",">=8");
    cuts_2l.ReplaceAll(">=5",">=7");
    cuts_2lbb.ReplaceAll(">=5",">=7");
    cuts_1ltex.ReplaceAll("6,","8,");
    cuts_2ltex.ReplaceAll("5,","7,");
    cuts_2lbbtex.ReplaceAll("5,","7,");
  }
  if(do_zbi){
    name.ReplaceAll("regions","regions_zbi");
  }

  ifstream header("txt/header.tex");
  ifstream footer("txt/footer.tex");
  ofstream file(name);
  file<<header.rdbuf();
  file<<"\\vspace{80 mm}\n";
  file << "\n\\begin{tabular}[!htb]{ l | ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++) file << "r";
  file<<" | r ";
  for(int sam(0); sam < nsig; sam++) file<<"| rr ";
  file<<"}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{${\\cal L} = "<<luminosity<<"$ fb$^{-1}$} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & "+fom;
  file << "\\\\ \\hline \n ";

  //////////////////////////////////// RA4 regions //////////////////////////////////
  file << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << cuts_1ltex  <<"} \\\\ \\hline \\hline\n";
  file << YieldsCut("R1: $m_T  \\leq 140,m_J\\leq 500$      ", "mj<=500&&mt<=140&&"+cuts_1l, chain, Samples, nsig);
  file << YieldsCut("R2: $m_T  \\leq 140,m_J> 500$  ", "mj>500&&mt<=140&&"+cuts_1l, chain, Samples, nsig);
  file <<"\\hline\n";
  file << YieldsCut("\\hspace{5 mm} low nj  ", "mj>500&&mt<=140&&njets<=7&&"+cuts_1l, chain, Samples, nsig);
  file << YieldsCut("\\hspace{5 mm} high nj  ", "mj>500&&mt<=140&&njets>=8&&"+cuts_1l, chain, Samples, nsig);
  file <<"\\hline\n";
  file << YieldsCut("R3: $m_T > 140,m_J \\leq 500$    ", "mj<=500&&mt>140&&"+cuts_1l, chain, Samples, nsig);
  file << YieldsCut("\\hspace{5 mm} low nb  ", "mj<=500&&mt>140&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
  file << YieldsCut("\\hspace{5 mm} high nb  ", "mj<=500&&mt>140&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
  file <<"\\hline\n";
  file << YieldsCut("R4: $m_T > 140,m_J > 500$", "mj>500&&mt>140&&"+cuts_1l, chain, Samples, nsig);
  
  file <<"\\hline\n";
  file <<"\\hline\n";

  file << YieldsCut("Bin 0: R4, low nj, low MET, low nb", "mj>500&&mt>140&&njets<=7&&met<=400&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
  file << YieldsCut("Bin 1: R4, low nj, low MET, high nb", "mj>500&&mt>140&&njets<=7&&met<=400&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
  file << YieldsCut("Bin 0+1: R4, low nj, low MET, all nb", "mj>500&&mt>140&&njets<=7&&met<=400&&"+cuts_1l, chain, Samples, nsig);
  file << YieldsCut("Bin 2: R4, low nj, high MET, all nb", "mj>500&&mt>140&&njets<=7&&met>400&&"+cuts_1l, chain, Samples, nsig);
  file<<"\\hline\n";
  file << YieldsCut("\\hspace{5 mm} low nb", "mj>500&&mt>140&&njets<=7&&met>400&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
  file << YieldsCut("\\hspace{5 mm} high nb", "mj>500&&mt>140&&njets<=7&&met>400&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);
  file<<"\\hline\n";
  file << YieldsCut("Bin 3: R4, high nj, low MET, all nb", "mj>500&&mt>140&&njets>=8&&met<=400&&"+cuts_1l, chain, Samples, nsig);
  file << YieldsCut("Bin 4: R4, high nj, high MET, all nb", "mj>500&&mt>140&&njets>=8&&met>400&&"+cuts_1l, chain, Samples, nsig);
  file << "\\hline \n";
  file << YieldsCut("\\hspace{5 mm} low nb", "mj>500&&mt>140&&njets>=8&&met>400&&nbm==2&&"+cuts_1l, chain, Samples, nsig);
  file << YieldsCut("\\hspace{5 mm} high nb", "mj>500&&mt>140&&njets>=8&&met>400&&nbm>=3&&"+cuts_1l, chain, Samples, nsig);

  file << "\\hline \\hline\\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       <<cuts_2ltex <<"} \\\\ \\hline \\hline\n";
  file << YieldsCut("$m_J \\leq 500$", "mj <= 500&&"+cuts_2l, chain, Samples, nsig);
  file << YieldsCut("$m_J > 500$", "mj>500&&"+cuts_2l, chain, Samples, nsig);
  file <<"\\hline\n";
  file << YieldsCut("\\hspace{5 mm} low nj", "mj>500&&njets<=6&&"+cuts_2l, chain, Samples, nsig);
  file << YieldsCut("\\hspace{5 mm} high nj", "mj>500&&njets>=7&&"+cuts_2l, chain, Samples, nsig);

  file << "\\hline \\hline\\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << cuts_2lbbtex<<"} \\\\ \\hline \\hline\n";
  file << YieldsCut("$m_J \\leq 500$", "mj<=500&&"+cuts_2lbb, chain, Samples, nsig);
  file << YieldsCut("$m_J > 500$", "mj>500&&"+cuts_2lbb, chain, Samples, nsig);
  file <<"\\hline\n";
  file << YieldsCut("\\hspace{5 mm} low nj", "mj>500&&njets<=6&&"+cuts_2lbb, chain, Samples, nsig);
  file << YieldsCut("\\hspace{5 mm} high nj", "mj>500&&njets>=7&&"+cuts_2lbb, chain, Samples, nsig);


  file << "\\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & "+fom;
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file<<footer.rdbuf();
  file.close();
  cout<<"Written "<<name<<endl;
}

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig){
  TString totCut, Hname = "histo", out;
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
    float fracerr(sqrt(pow(bkg_err/bkg,2)+0.3*0.3+0.24*0.24));
    out += (" \t& $" + RoundNumber(yield[sam],1)+" \\pm "+RoundNumber(error[sam],1) + "$ \t& ");
    if(do_zbi) out += RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2);
    else out += RoundNumber(yield[sam],2,bkg);
	    
    cout<<", S = "<<RoundNumber(yield[sam],1)+" +- "+RoundNumber(error[sam],1)<<" with Zbi = ";
    cout<<RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2);
  }
  out += " \\\\ \n";
  cout<<endl;
  return out;
}