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
  TString MJcut = "400";
  bool doRmj = false;
  bool doRmt = true;
}

using namespace ra4;

using namespace std;
using std::cout;
using std::endl;

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples);
TString GetRatio(TString baseline,TString region_cut,TString ratio_cut, sfeats Sample);
void GetRatio(double &ratio,double &error, TString baseline,TString region_cut, TString ratio_cut,  sfeats Sample);
TString GetKappa(TString baseline,TString region_cut1,TString region_cut2, TString ratio_cut,  sfeats Sample);

int main(){

  // Reading ntuples
  TString folder="/cms5r0/ald77/archive/2015_05_21/skim/";
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");

  vector<TChain *> chain;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_tt, "$t\\bar{t}$ (1$\\ell$)", 1000,1,"ntruleps<=1"));
  Samples.push_back(sfeats(s_tt, "$t\\bar{t}$ ($2\\ell$)", 1006,1,"ntruleps>=2"));
  
  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  TString name = "txt/ra4_Rfactors_MJ"+MJcut+"_lumi"+luminosity+".tex";
  if(doRmj) name.ReplaceAll("Rfactors","Rfactors_Rmj");
  if(doRmt) name.ReplaceAll("Rfactors","Rfactors_Rmt");

  // CUTS
  TString cuts_1l("(nmus+nels)==1&&ht>500&&met>200&&nbm>=2&&njets>=6");
  TString cuts_1l1b("(nmus+nels)==1&&ht>500&&met>200&&nbm==1&&njets>=6");
  TString cuts_2l("(nmus+nels)==2&&ht>500&&met>200&&nbm==1&&njets>=5");
  TString cuts_2lbb("(nmus+nels)==2&&ht>500&&met>200&&nbm==2&&njets>=5");

  TString cuts_1ltex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq 6, n_b\\geq 2, n_{\\rm lep}=1$");
  TString cuts_1l1btex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq 6, n_b=1, n_{\\rm lep}=1$");
  TString cuts_2ltex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq 5, n_b=1, n_{\\rm lep}=2$");
  TString cuts_2lbbtex("$H_T>500, \\mathrm{MET}>200, n_{\\rm jets}\\geq 5, n_b=2, n_{\\rm lep}=2$");

  TString cuts_R1("mj<="+MJcut+"&&mt<=140&&");
  TString cuts_R2("mj>"+MJcut+"&&mt<=140&&");
  TString cuts_R3("mj<="+MJcut+"&&mt>140&&");
  TString cuts_R4("mj>"+MJcut+"&&mt>140&&");
  TString cuts_highMJ("mj>"+MJcut+"&&");
  TString cuts_lowMJ("mj<="+MJcut+"&&");

  TString cuts_R1tex("R1: $m_T \\leq 140,M_J \\leq "+MJcut+"$");
  TString cuts_R2tex("R2: $m_T  \\leq 140,M_J > "+MJcut+"$");
  TString cuts_R3tex("R3: $m_T  > 140,M_J \\leq "+MJcut+"$");
  TString cuts_R4tex("R4: $m_T  > 140,M_J > "+MJcut+"$");
  TString cuts_highMJtex("$M_J > "+MJcut+"$");
  TString cuts_lowMJtex("$M_J \\leq "+MJcut+"$");
  // 

  ifstream header("txt/header.tex");
  ifstream footer("txt/footer.tex");
  ofstream file(name);
  file<<header.rdbuf();
  file<<"\\vspace{80 mm}\n";
  file << "\n\\begin{tabular}[!htb]{ l | ";
  for(unsigned sam(0); sam < Samples.size(); sam++) 
    file << "r";
  file << "| r r | r r"; //For R and K factors
  file<<"}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{${\\cal L} = "<<luminosity<<"$ fb$^{-1}$} ";
  for(unsigned sam(0); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label;
  if(doRmj) file << "& $R_{m_T}^{1\\ell}$ & $\\kappa_{m_T}^{1\\ell}$ & $R_{m_T}^{2\\ell}$ & $\\kappa_{m_T}^{2\\ell}$";
  if(doRmt) file << "& $R_{m_T}^{1\\ell}$ & $\\kappa_{m_T}^{1\\ell}$ & $R_{m_T}^{2\\ell}$ & $\\kappa_{m_T}^{2\\ell}$";
  file << "\\\\ \\hline \n ";

  //////////////////////////////////// RA4Regions //////////////////////////////////
  file << " \\multicolumn{"<< Samples.size()+5<<"}{c}{" << cuts_1ltex  <<"} \\\\ \\hline \\hline\n";
  
  // Do Rmj
  if(doRmj){
    file << YieldsCut(cuts_R1tex, cuts_R1+cuts_1l, chain, Samples)+"&&&&"+" \\\\ \n";
    file << YieldsCut(cuts_R2tex, cuts_R2+cuts_1l, chain, Samples)+"& \\multirow{-2}{*}{"+GetRatio(cuts_1l,"mt<=140","mj>"+MJcut,Samples[0])+"} && \\multirow{-2}{*}{"+GetRatio(cuts_1l,"mt<=140","mj>"+MJcut,Samples[1])+"} &"+" \\\\ \n";
    file << YieldsCut(cuts_R3tex, cuts_R3+cuts_1l, chain, Samples)+"&&&&"+" \\\\ \n";
    file << YieldsCut(cuts_R4tex, cuts_R4+cuts_1l, chain, Samples)+ " & \\multirow{-2}{*}{"+GetRatio(cuts_1l,"mt>140","mj>"+MJcut,Samples[0])+"} & \\multirow{-4}{*}{"+GetKappa(cuts_1l,"mt<=140","mt>140","mj>"+MJcut,Samples[0])+"} & \\multirow{-2}{*}{"+GetRatio(cuts_1l,"mt>140","mj>"+MJcut,Samples[1])+"} & \\multirow{-4}{*}{"+GetKappa(cuts_1l,"mt<=140","mt>140","mj>"+MJcut,Samples[1])+"}"+" \\\\ \n";
  }

  // Do Rmt
  if(doRmt){
    file << YieldsCut(cuts_R1tex, cuts_R1+cuts_1l, chain, Samples)+"&&&&"+" \\\\ \n";
    file << YieldsCut(cuts_R3tex, cuts_R3+cuts_1l, chain, Samples)+"& \\multirow{-2}{*}{"+GetRatio(cuts_1l,"mj<="+MJcut,"mt>140",Samples[0])+"} && \\multirow{-2}{*}{"+GetRatio(cuts_1l,"mj<="+MJcut,"mt>140",Samples[1])+"} &"+" \\\\ \n";
    file << YieldsCut(cuts_R2tex, cuts_R2+cuts_1l, chain, Samples)+"&&&&"+" \\\\ \n";
    file << YieldsCut(cuts_R4tex, cuts_R4+cuts_1l, chain, Samples)+ " & \\multirow{-2}{*}{"+GetRatio(cuts_1l,"mj>"+MJcut,"mt>140",Samples[0])+"} & \\multirow{-4}{*}{"+GetKappa(cuts_1l,"mj<="+MJcut,"mj>"+MJcut,"mt>140",Samples[0])+"} & \\multirow{-2}{*}{"+GetRatio(cuts_1l,"mj>"+MJcut,"mt>140",Samples[1])+"} & \\multirow{-4}{*}{"+GetKappa(cuts_1l,"mj<="+MJcut,"mj>"+MJcut,"mt>140",Samples[1])+"}"+" \\\\ \n";
  }

  file << "\\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size(); sam++)
    file << " & ";
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file<<footer.rdbuf();
  file.close();
  cout<<"Written "<<name<<endl;
}

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples){
  TString totCut, Hname = "histo", out;
  vector<double> yield, error;
  double err;
  int nsam(chain.size());
  TH1D histo(Hname, "",100, 0, 10000);
  histo.Sumw2();
  for(int sam(0); sam < nsam; sam++){
    totCut = luminosity+"*weight*("+cuts+"&&"+Samples[sam].cut+")";
    chain[sam]->Project(Hname, "met", totCut);
    yield.push_back(histo.IntegralAndError(0,101,err));
    error.push_back(err);
  }
  
  cout<<title;
  out = title;
  for(int sam(0); sam < nsam; sam++)
    out += (" \t &  $"+RoundNumber(yield[sam],1))+" \\pm "+RoundNumber(error[sam],1)+"$";
  //  out += " \\\\ \n";
  cout<<endl;
  return out;
}

TString GetRatio(TString baseline,TString region_cut,TString ratio_cut, sfeats Sample){

  TChain * chain = new TChain("tree");
  for(unsigned insam(0); insam < Sample.file.size(); insam++) 
    chain->Add(Sample.file[insam]);

  TH1D h_numerator("num", "",100, 0, 10000);
  h_numerator.Sumw2();
  TH1D h_denominator("den", "",100, 0, 10000);
  h_denominator.Sumw2();

  TString numCut = luminosity+"*weight*("+baseline+"&&"+region_cut+"&&"+ratio_cut+"&&"+Sample.cut+")";
  TString denCut = luminosity+"*weight*("+baseline+"&&"+region_cut+"&&!("+ratio_cut+")&&"+Sample.cut+")";

  chain->Project("num", "met", numCut);
  chain->Project("den", "met", denCut);

  double numerator, denominator,numerator_err, denominator_err, ratio, error;
  numerator = h_numerator.IntegralAndError(0,101,numerator_err);
  denominator = h_denominator.IntegralAndError(0,101,denominator_err);

  ratio = numerator/denominator;
  error = ratio*sqrt(pow(numerator_err/numerator,2)+pow(denominator_err/denominator,2));
  
  TString out;
  out = "$"+RoundNumber(ratio,2)+" \\pm "+RoundNumber(error,2)+"$";

  return out;
}

void GetRatio(double &ratio,double &error, TString baseline,TString region_cut, TString ratio_cut,  sfeats Sample){

  TChain * chain = new TChain("tree");
  for(unsigned insam(0); insam < Sample.file.size(); insam++) chain->Add(Sample.file[insam]);

  TH1D h_numerator("num", "",100, 0, 10000);
  h_numerator.Sumw2();
  TH1D h_denominator("den", "",100, 0, 10000);
  h_denominator.Sumw2();

  TString numCut = luminosity+"*weight*("+baseline+"&&"+region_cut+"&&"+ratio_cut+"&&"+Sample.cut+")";
  TString denCut = luminosity+"*weight*("+baseline+"&&"+region_cut+"&&!("+ratio_cut+")&&"+Sample.cut+")";

  chain->Project("num", "met", numCut);
  chain->Project("den", "met", denCut);

  double numerator, denominator,numerator_err, denominator_err;
  numerator = h_numerator.IntegralAndError(0,101,numerator_err);
  denominator = h_denominator.IntegralAndError(0,101,denominator_err);

  ratio = numerator/denominator;
  error = ratio*sqrt(pow(numerator_err/numerator,2)+pow(denominator_err/denominator,2));
}

TString GetKappa(TString baseline,TString region_cut1,TString region_cut2, TString ratio_cut,  sfeats Sample){
  
  double ratio1, ratio2, error1, error2, kappa, kappaerr;
  GetRatio(ratio1,error1,baseline,region_cut1,ratio_cut, Sample);
  GetRatio(ratio2,error2,baseline,region_cut2,ratio_cut, Sample);

  kappa = ratio2/ratio1;
  kappaerr = kappa*sqrt(pow(error2/ratio2,2)+pow(error1/ratio1,2));
  
  TString out;
  out = "$"+RoundNumber(kappa,2)+" \\pm "+RoundNumber(kappaerr,2)+"$";

  return out;
}
