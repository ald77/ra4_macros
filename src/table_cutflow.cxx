// table_cutflow: Generates a LaTeX file with a cutflow table for RA4

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
  TString ntuple_date("2015_05_25");
  TString minjets("7"), midjets("9");
  TString mjthresh("400");
  TString luminosity="10";
  bool do_2l=false;
}

using namespace ra4;

using namespace std;
using std::cout;
using std::endl;

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig);

int main(){

  // Reading ntuples
  TString folder="/cms5r0/ald77/archive/"+ntuple_date+"/skim/";
  TString folder_ns="/cms5r0/ald77/archive/"+ntuple_date+"/";
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*_WJets*");
  vector<TString> s_single;
  s_single.push_back(folder+"*_T*channel*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder+"*TTW*");
  s_ttv.push_back(folder+"*TTZ*");
  vector<TString> s_qcd;
  s_qcd.push_back(folder+"*QCD_Pt*");
  s_qcd.push_back(folder+"*_TTJet*");
  vector<TString> s_other;
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");

  vector<TChain *> chain, chain_ns;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_other, "Other", 1001));
  Samples.push_back(sfeats(s_qcd, "QCD", 1005,1,"ntruleps==0")); // It includes hadronic ttbar
  Samples.push_back(sfeats(s_ttv, "$t\\bar{t}V$", 1002));
  Samples.push_back(sfeats(s_single, "Single $t$", 1005));
  Samples.push_back(sfeats(s_wjets, "W+jets", 1004));
  Samples.push_back(sfeats(s_tt, "$t\\bar{t}$ (1$\\ell$)", 1000,1,"ntruleps==1"));
  Samples.push_back(sfeats(s_tt, "$t\\bar{t}$ ($2\\ell$)", 1006,1,"ntruleps>=2"));
  int nsig(2);
  Samples.push_back(sfeats(s_t1t, "T1tttt NC", 2));
  Samples.push_back(sfeats(s_t1tc, "T1tttt C", 2,2));

  TString file_ns;
  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    chain_ns.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++){
      chain[sam]->Add(Samples[sam].file[insam]);
      file_ns = Samples[sam].file[insam];
      file_ns.ReplaceAll(folder, folder_ns);
      chain_ns[sam]->Add(file_ns);
    }
  }

  bool do_zbi=false;
  TString minjets_2l(""), midjets_2l("");
  minjets_2l += (minjets.Atoi()-1); midjets_2l += (midjets.Atoi()-1); 
  TString fom("$Z_{\\rm bi}$");
  if(!do_zbi) fom = "S/B";
  TString outname = "txt/cutflow_mj"+mjthresh+"_njets"+minjets+midjets+"_lumi"+luminosity+"_"+ntuple_date+".tex";
  if(do_2l) outname.ReplaceAll("yields","yields_2l");
  if(do_zbi) outname.ReplaceAll("yields","yields_zbi");

  ifstream header("txt/header.tex");
  ifstream footer("txt/footer.tex");
  ofstream file(outname);
  file<<header.rdbuf();
  file<<"\\vspace{80 mm}\n";
  file << "\n\\begin{tabular}[!htb]{ l | ";
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

  //////////////////////////////////////////  Cutflow   /////////////////////////////////////////////////////
  TString cuts("1");
  file << YieldsCut("No selection", cuts, chain_ns, Samples, nsig);
  cuts += "&&(nmus+nels)==1";
  file << YieldsCut("$1\\ell$", cuts, chain_ns, Samples, nsig);
  cuts += "&ht>500";
  file << YieldsCut("$H_T>500$ GeV", cuts, chain_ns, Samples, nsig);
  cuts += "&&met>200";
  file << YieldsCut("${\\rm MET}>200$ GeV", cuts, chain, Samples, nsig);
  cuts += "&&njets>="+minjets;
  file << YieldsCut("$n_{\\rm jets}\\geq "+minjets+"$", cuts, chain, Samples, nsig);
  cuts += "&&nbm>=2";
  file << YieldsCut("$n_{\\rm b}\\geq 2$", cuts, chain, Samples, nsig);
  cuts += "&&mt>140";
  file << YieldsCut("$m_T>140$ GeV", cuts, chain, Samples, nsig);
  cuts += "&&mj>"+mjthresh;
  file << YieldsCut("$M_J>"+mjthresh+"$ GeV", cuts, chain, Samples, nsig);
  cuts += "&&met>400";
  file << YieldsCut("${\\rm MET}>400$ GeV", cuts, chain, Samples, nsig);
  cuts += "&&njets>="+midjets;
  file << YieldsCut("$n_{\\rm jets}\\geq "+midjets+"$", cuts, chain, Samples, nsig);


  //////////////////////////////////////////  Cutflow   /////////////////////////////////////////////////////


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
      if(yield[sam]>0) bkg += yield[sam];
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
