#include <iostream>
#include <vector>

#include "TString.h"
#include "TGraphErrors.h"

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TColor.h"
#include "TMath.h"
#include "TF1.h"
#include "TLatex.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;

double GetWeight(sfeats Sample, TString luminosity);
void GetYield(double& yield, double& yield_err, TString baseline,TString external,TString mj_cut,TString mt_cut, sfeats Sample,TString luminosity);
void GetRatio(double& ratio, double& error, double num, double den, double num_err, double den_err, double weight);
void GetKappa(double& kappa, double& error, double n1, double n2, double n3, double n4, double n1_err, double n2_err, double n3_err, double n4_err, double weight);
void MakeGraphs(double rmj1[],double rmj2[],double rmj1_err[],double rmj2_err[],double rmt1[],double rmt2[],double rmt1_err[],double rmt2_err[],double kappa[],double kappa_err[], const vector<TString> binlabels,TString sampleName, TString baseline, TString mj_cut, TString mt_cut);

int main(){
  TString luminosity = "10";
  //  TString folder="/cms5r0/ald77/archive/2015_05_25/skim/";
  //  TString folder_noskim="/cms5r0/ald77/archive/2015_05_25/";
  TString folder="/cms5r0/rohan/2015_05_25/skim_met100/";

  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  //  vector<TString> s_tt_noskim;
  //  s_tt_noskim.push_back(folder_noskim+"*_TTJet*12.root");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*WJetsToLNu_HT*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder+"*_T*channel*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder+"*TTW*");
  s_ttv.push_back(folder+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD_HT*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");

  //TString baseline = "ht>500&&met>100&&nbm==1&&(nmus+nels)==1&&njets>=7";                // MET 100, Nb=1 
  //  TString baseline = "ht>500&&met>100&&nbm==1&&(nmus+nels)==1&&njets>=7&&ntks_chg==0";     // ITV, MET 100, Nb=1
  
  TString baseline = "ht>500&&met>100&&nbm>=2&&(nmus+nels)==1&&njets>=7";                // MET 100, Nb>=2
  //TString baseline = "ht>500&&met>100&&nbm>=2&&(nmus+nels)==1&&njets>=7&&ntks_chg==0";   // ITV, MET 100, Nb>=2

  //TString baseline = "ht>500&&met>200&&nbm>=2&&(nmus+nels)==1&&njets>=7";                // MET 200, Nb>=2
  //TString baseline = "ht>500&&met>200&&nbm>=2&&(nmus+nels)==1&&njets>=7&&ntks_chg==0";   // ITV, MET 200, Nb>=2
  

  vector<sfeats> samples_MJ;
  //samples_MJ.push_back(sfeats(s_tt, "t#bar{t}", 31, 1));
  //samples_MJ.push_back(sfeats(s_tt, "2#it{l} t#bar{t}", 31,1,"ntruleps>=2"));
  samples_MJ.push_back(sfeats(s_tt, "1#it{l} t#bar{t}", 31,1,"ntruleps<=1"));

  // METHOD 1
  TString mj_cut = "mj>600";   
  TString mt_cut = "mt>140";
  TString not_mt_cut = invertcut(mt_cut);
  TString not_mj_cut = invertcut(mj_cut);

  const int nExt = 6; // Must match size of external vector
  vector<TString> external_meth1;
  external_meth1.push_back("njets>=7&&njets<=8&&met>100&&met<=200");
  external_meth1.push_back("njets>=7&&njets<=8&&met>200&&met<=400");
  external_meth1.push_back("njets>=7&&njets<=8&&met>400");
  external_meth1.push_back("njets>=9&&met>100&&met<=200");
  external_meth1.push_back("njets>=9&&met>200&&met<=400");
  external_meth1.push_back("njets>=9&&met>400");

  double weight = GetWeight(samples_MJ.at(0), luminosity);

  double N1_meth1[nExt],N2_meth1[nExt],N3_meth1[nExt],N4_meth1[nExt];
  double N1_meth1_err[nExt],N2_meth1_err[nExt],N3_meth1_err[nExt],N4_meth1_err[nExt];
  for(int iR=0; iR<nExt; iR++){
    GetYield(N1_meth1[iR], N1_meth1_err[iR], baseline, external_meth1.at(iR), not_mj_cut, not_mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N2_meth1[iR], N2_meth1_err[iR], baseline, external_meth1.at(iR), mj_cut, not_mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N3_meth1[iR], N3_meth1_err[iR], baseline, external_meth1.at(iR), not_mj_cut, mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N4_meth1[iR], N4_meth1_err[iR], baseline, external_meth1.at(iR), mj_cut, mt_cut, samples_MJ.at(0), luminosity);
  }

  double rmj43[nExt], rmj21[nExt], rmj43_err[nExt], rmj21_err[nExt];
  double rmt42[nExt], rmt31[nExt], rmt42_err[nExt], rmt31_err[nExt]; 
  for(int iR=0; iR<nExt; iR++){
    GetRatio(rmj21[iR], rmj21_err[iR], N2_meth1[iR], N1_meth1[iR], N2_meth1_err[iR], N1_meth1_err[iR], weight);
    GetRatio(rmj43[iR], rmj43_err[iR], N4_meth1[iR], N3_meth1[iR], N4_meth1_err[iR], N3_meth1_err[iR], weight);
    GetRatio(rmt31[iR], rmt31_err[iR], N3_meth1[iR], N1_meth1[iR], N3_meth1_err[iR], N1_meth1_err[iR], weight);
    GetRatio(rmt42[iR], rmt42_err[iR], N4_meth1[iR], N2_meth1[iR], N4_meth1_err[iR], N2_meth1_err[iR], weight);
  }

  double kappa[nExt], kappa_err[nExt];
  for(int iR=0; iR<nExt; iR++){
    GetKappa(kappa[iR], kappa_err[iR], N1_meth1[iR], N2_meth1[iR], N3_meth1[iR], N4_meth1[iR], N1_meth1_err[iR], N2_meth1_err[iR], N3_meth1_err[iR], N4_meth1_err[iR], weight);
  }  

  vector<TString> binlabels; //push back twice for each njets bin
  binlabels.push_back("low MET");binlabels.push_back("med MET");binlabels.push_back("high MET");
  binlabels.push_back("low MET");binlabels.push_back("med MET");binlabels.push_back("high MET");
  TString binName = "method1";
  if (baseline.Contains("met>100")) binName.Append("_met100");
  if (baseline.Contains("nbm==1")) binName.Append("_nb1");
  binlabels.push_back(binName); // Last bin is for method name.

  MakeGraphs(rmj43, rmj21, rmj43_err, rmj21_err, rmt42, rmt31, rmt42_err, rmt31_err, kappa, kappa_err, binlabels, samples_MJ.at(0).label, baseline, mj_cut, mt_cut);

  // METHOD 2
  mj_cut = "mj>400";   
  mt_cut = "mt>140";
  not_mt_cut = invertcut(mt_cut);
  not_mj_cut = invertcut(mj_cut);

  const int nExt_2 = 6; // Must match size of external vector
  vector<TString> externalA_meth2;
  externalA_meth2.push_back("met>100&&met<=200");
  externalA_meth2.push_back("met>200&&met<=400");
  externalA_meth2.push_back("met>400");
  externalA_meth2.push_back("met>100&&met<=200");
  externalA_meth2.push_back("met>200&&met<=400");
  externalA_meth2.push_back("met>400");
  vector<TString> externalB_meth2;
  externalB_meth2.push_back("njets>=7&&njets<=8&&met>100&&met<=200");
  externalB_meth2.push_back("njets>=7&&njets<=8&&met>200&&met<=400");
  externalB_meth2.push_back("njets>=7&&njets<=8&&met>400");
  externalB_meth2.push_back("njets>=9&&met>100&&met<=200");
  externalB_meth2.push_back("njets>=9&&met>200&&met<=400");
  externalB_meth2.push_back("njets>=9&&met>400");

  double N1_meth2[nExt_2],N2_meth2[nExt_2],N3_meth2[nExt_2],N4_meth2[nExt_2];
  double N1_meth2_err[nExt_2],N2_meth2_err[nExt_2],N3_meth2_err[nExt_2],N4_meth2_err[nExt_2];
  for(int iR=0; iR<nExt_2; iR++){
    GetYield(N1_meth2[iR], N1_meth2_err[iR], baseline, externalA_meth2.at(iR), not_mj_cut, not_mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N2_meth2[iR], N2_meth2_err[iR], baseline, externalB_meth2.at(iR), mj_cut, not_mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N3_meth2[iR], N3_meth2_err[iR], baseline, externalA_meth2.at(iR), not_mj_cut, mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N4_meth2[iR], N4_meth2_err[iR], baseline, externalB_meth2.at(iR), mj_cut, mt_cut, samples_MJ.at(0), luminosity);
  }

  double rmj43_2[nExt_2], rmj21_2[nExt_2], rmj43_err_2[nExt_2], rmj21_err_2[nExt_2];
  double rmt42_2[nExt_2], rmt31_2[nExt_2], rmt42_err_2[nExt_2], rmt31_err_2[nExt_2]; 
  for(int iR=0; iR<nExt_2; iR++){
    GetRatio(rmj21_2[iR], rmj21_err_2[iR], N2_meth2[iR], N1_meth2[iR], N2_meth2_err[iR], N1_meth2_err[iR], weight);
    GetRatio(rmj43_2[iR], rmj43_err_2[iR], N4_meth2[iR], N3_meth2[iR], N4_meth2_err[iR], N3_meth2_err[iR], weight);
    GetRatio(rmt31_2[iR], rmt31_err_2[iR], N3_meth2[iR], N1_meth2[iR], N3_meth2_err[iR], N1_meth2_err[iR], weight);
    GetRatio(rmt42_2[iR], rmt42_err_2[iR], N4_meth2[iR], N2_meth2[iR], N4_meth2_err[iR], N2_meth2_err[iR], weight);
  }

  double kappa_2[nExt_2], kappa_err_2[nExt_2];
  for(int iR=0; iR<nExt_2; iR++){
    GetKappa(kappa_2[iR], kappa_err_2[iR], N1_meth2[iR], N2_meth2[iR], N3_meth2[iR], N4_meth2[iR], N1_meth2_err[iR], N2_meth2_err[iR], N3_meth2_err[iR], N4_meth2_err[iR], weight);
  }  

  vector<TString> binlabels_2; //push back twice for each njets bin
  binlabels_2.push_back("low MET");binlabels_2.push_back("med MET");binlabels_2.push_back("high MET");
  binlabels_2.push_back("low MET");binlabels_2.push_back("med MET");binlabels_2.push_back("high MET");

  TString binName_2 = "method2";
  if (baseline.Contains("met>100")) binName_2.Append("_met100");
  if (baseline.Contains("nbm==1")) binName_2.Append("_nb1");
  binlabels_2.push_back(binName_2); // Last bin is for method name.

  MakeGraphs(rmj43_2, rmj21_2, rmj43_err_2, rmj21_err_2, rmt42_2, rmt31_2, rmt42_err_2, rmt31_err_2, kappa_2, kappa_err_2, binlabels_2, samples_MJ.at(0).label, baseline, mj_cut, mt_cut);

  /*  // METHOD 3
  mj_cut = "mj>400";   
  mt_cut = "mt>140";
  not_mt_cut = invertcut(mt_cut);
  not_mj_cut = invertcut(mj_cut);

  const int nExt_3 = 6; // Must match size of largest external vector
  vector<TString> externalA_meth3;
  externalA_meth3.push_back("met>200&&met<=400&&nbm==2");
  externalA_meth3.push_back("met>200&&met<=400&&nbm>=3");
  externalA_meth3.push_back("met>400");
  externalA_meth3.push_back("met>200&&met<=400&&nbm==2");
  externalA_meth3.push_back("met>200&&met<=400&&nbm>=3");
  externalA_meth3.push_back("met>400");
  vector<TString> externalB_meth3;
  externalB_meth3.push_back("njets>=7&&njets<=8&&met>200&&met<=400&&nbm==2");
  externalB_meth3.push_back("njets>=7&&njets<=8&&met>200&&met<=400&&nbm>=3");
  externalB_meth3.push_back("njets>=7&&njets<=8&&met>400");
  externalB_meth3.push_back("njets>=9&&met>200&&met<=400&&nbm==2");
  externalB_meth3.push_back("njets>=9&&met>200&&met<=400&&nbm>=3");
  externalB_meth3.push_back("njets>=9&&met>400");

  double N1_meth3[nExt_3],N2_meth3[nExt_3],N3_meth3[nExt_3],N4_meth3[nExt_3];
  double N1_meth3_err[nExt_3],N2_meth3_err[nExt_3],N3_meth3_err[nExt_3],N4_meth3_err[nExt_3];
  for(int iR=0; iR<nExt_3; iR++){
    GetYield(N1_meth3[iR], N1_meth3_err[iR], baseline, externalA_meth3.at(iR), not_mj_cut, not_mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N2_meth3[iR], N2_meth3_err[iR], baseline, externalB_meth3.at(iR), mj_cut, not_mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N3_meth3[iR], N3_meth3_err[iR], baseline, externalA_meth3.at(iR), not_mj_cut, mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N4_meth3[iR], N4_meth3_err[iR], baseline, externalB_meth3.at(iR), mj_cut, mt_cut, samples_MJ.at(0), luminosity);
  }

  double rmj43_3[nExt_3], rmj21_3[nExt_3], rmj43_err_3[nExt_3], rmj21_err_3[nExt_3];
  double rmt42_3[nExt_3], rmt31_3[nExt_3], rmt42_err_3[nExt_3], rmt31_err_3[nExt_3]; 
  for(int iR=0; iR<nExt_3; iR++){
    GetRatio(rmj21_3[iR], rmj21_err_3[iR], N2_meth3[iR], N1_meth3[iR], N2_meth3_err[iR], N1_meth3_err[iR], weight);
    GetRatio(rmj43_3[iR], rmj43_err_3[iR], N4_meth3[iR], N3_meth3[iR], N4_meth3_err[iR], N3_meth3_err[iR], weight);
    GetRatio(rmt31_3[iR], rmt31_err_3[iR], N3_meth3[iR], N1_meth3[iR], N3_meth3_err[iR], N1_meth3_err[iR], weight);
    GetRatio(rmt42_3[iR], rmt42_err_3[iR], N4_meth3[iR], N2_meth3[iR], N4_meth3_err[iR], N2_meth3_err[iR], weight);
  }

  double kappa_3[nExt_3], kappa_err_3[nExt_3];
  for(int iR=0; iR<nExt_3; iR++){
    GetKappa(kappa_3[iR], kappa_err_3[iR], N1_meth3[iR], N2_meth3[iR], N3_meth3[iR], N4_meth3[iR], N1_meth3_err[iR], N2_meth3_err[iR], N3_meth3_err[iR], N4_meth3_err[iR], weight);
  }  

  vector<TString> binlabels_3; //push back twice for each njets bin
  binlabels_3.push_back("low MET, nb=2");binlabels_3.push_back("low MET, nb#geq3");binlabels_3.push_back("high MET");
  binlabels_3.push_back("low MET, nb=2");binlabels_3.push_back("low MET, nb#geq3");binlabels_3.push_back("high MET");

  TString binName_3 = "method3";
  if (baseline.Contains("met>100")) binName_3.Append("_met100");
  if (baseline.Contains("nbm==1")) binName_3.Append("_nb1");
  binlabels_3.push_back(binName_3); // Last bin is for method name.

  MakeGraphs(rmj43_3, rmj21_3, rmj43_err_3, rmj21_err_3, rmt42_3, rmt31_3, rmt42_err_3, rmt31_err_3, kappa_3, kappa_err_3, binlabels_3, samples_MJ.at(0).label, baseline, mj_cut, mt_cut);
  */ //METHOD 3

  // METHOD 0
  mj_cut = "mj>400";   
  mt_cut = "mt>140";
  not_mt_cut = invertcut(mt_cut);
  not_mj_cut = invertcut(mj_cut);

  const int nExt_0 = 6; // Must match size of external vector
  vector<TString> externalA_meth0;
  externalA_meth0.push_back("met>100");
  externalA_meth0.push_back("met>100");
  externalA_meth0.push_back("met>100");
  externalA_meth0.push_back("met>100");
  externalA_meth0.push_back("met>100");
  externalA_meth0.push_back("met>100");
  vector<TString> externalB_meth0;
  externalB_meth0.push_back("njets>=7&&njets<=8&&met>100&&met<=200");
  externalB_meth0.push_back("njets>=7&&njets<=8&&met>200&&met<=400");
  externalB_meth0.push_back("njets>=7&&njets<=8&&met>400");
  externalB_meth0.push_back("njets>=9&&met>100&&met<=200");
  externalB_meth0.push_back("njets>=9&&met>200&&met<=400");
  externalB_meth0.push_back("njets>=9&&met>400");

  double N1_meth0[nExt_0],N2_meth0[nExt_0],N3_meth0[nExt_0],N4_meth0[nExt_0];
  double N1_meth0_err[nExt_0],N2_meth0_err[nExt_0],N3_meth0_err[nExt_0],N4_meth0_err[nExt_0];
  for(int iR=0; iR<nExt_0; iR++){
    GetYield(N1_meth0[iR], N1_meth0_err[iR], baseline, externalA_meth0.at(iR), not_mj_cut, not_mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N2_meth0[iR], N2_meth0_err[iR], baseline, externalB_meth0.at(iR), mj_cut, not_mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N3_meth0[iR], N3_meth0_err[iR], baseline, externalA_meth0.at(iR), not_mj_cut, mt_cut, samples_MJ.at(0), luminosity);
    GetYield(N4_meth0[iR], N4_meth0_err[iR], baseline, externalB_meth0.at(iR), mj_cut, mt_cut, samples_MJ.at(0), luminosity);
  }

  double rmj43_0[nExt_0], rmj21_0[nExt_0], rmj43_err_0[nExt_0], rmj21_err_0[nExt_0];
  double rmt42_0[nExt_0], rmt31_0[nExt_0], rmt42_err_0[nExt_0], rmt31_err_0[nExt_0]; 
  for(int iR=0; iR<nExt_0; iR++){
    GetRatio(rmj21_0[iR], rmj21_err_0[iR], N2_meth0[iR], N1_meth0[iR], N2_meth0_err[iR], N1_meth0_err[iR], weight);
    GetRatio(rmj43_0[iR], rmj43_err_0[iR], N4_meth0[iR], N3_meth0[iR], N4_meth0_err[iR], N3_meth0_err[iR], weight);
    GetRatio(rmt31_0[iR], rmt31_err_0[iR], N3_meth0[iR], N1_meth0[iR], N3_meth0_err[iR], N1_meth0_err[iR], weight);
    GetRatio(rmt42_0[iR], rmt42_err_0[iR], N4_meth0[iR], N2_meth0[iR], N4_meth0_err[iR], N2_meth0_err[iR], weight);
  }

  double kappa_0[nExt_0], kappa_err_0[nExt_0];
  for(int iR=0; iR<nExt_0; iR++){
    GetKappa(kappa_0[iR], kappa_err_0[iR], N1_meth0[iR], N2_meth0[iR], N3_meth0[iR], N4_meth0[iR], N1_meth0_err[iR], N2_meth0_err[iR], N3_meth0_err[iR], N4_meth0_err[iR], weight);
  }  

  vector<TString> binlabels_0; //push back twice for each njets bin
  binlabels_0.push_back("low MET");binlabels_0.push_back("med MET");binlabels_0.push_back("high MET");
  binlabels_0.push_back("low MET");binlabels_0.push_back("med MET");binlabels_0.push_back("high MET");

  TString binName_0 = "method0";
  if (baseline.Contains("met>100")) binName_0.Append("_met100");
  if (baseline.Contains("nbm==1")) binName_0.Append("_nb1");
  binlabels_0.push_back(binName_0); // Last bin is for method name.

  MakeGraphs(rmj43_0, rmj21_0, rmj43_err_0, rmj21_err_0, rmt42_0, rmt31_0, rmt42_err_0, rmt31_err_0, kappa_0, kappa_err_0, binlabels_0, samples_MJ.at(0).label, baseline, mj_cut, mt_cut);
}

void MakeGraphs(double rmj1[],double rmj2[],double rmj1_err[],double rmj2_err[],double rmt1[],double rmt2[],double rmt1_err[],double rmt2_err[],double kappa[],double kappa_err[], const vector<TString> binlabels,TString sampleName, TString baseline, TString mj_cut, TString mt_cut){

  TString sampleNameText = sampleName;
  if(sampleName.Contains("2#it{l} t#bar{t}")) { sampleNameText = sampleName; sampleNameText.ReplaceAll("2#it{l} t#bar{t}","ttbar_2l"); }
  else if(sampleName.Contains("1#it{l} t#bar{t}")) { sampleNameText = sampleName; sampleNameText.ReplaceAll("1#it{l} t#bar{t}","ttbar_1l"); }
  else if(sampleName.Contains("t#bar{t}")) { sampleNameText = sampleName; sampleNameText.ReplaceAll("t#bar{t}","ttbar"); }
  

  vector<TGraphErrors*> graphsMJ;
  vector<TGraphErrors*> graphsMT;
  TGraphErrors* kappas;
  const int nExt = binlabels.size()-1;

  vector<double> x, x1, x2, x_err;
  for(int i=0; i<nExt; i++){
    x.push_back(i);
    x1.push_back(i-0.075);
    x2.push_back(i+0.075);
    x_err.push_back(0);
  }

  TGraphErrors *mj1 = new TGraphErrors(nExt,&x1[0],rmj1,&x_err[0],rmj1_err);  // &foo[0] convert vector "foo" to an array because TGraphErrors only accepts arrays
  graphsMJ.push_back(mj1);
    
  TGraphErrors *mj2 = new TGraphErrors(nExt,&x2[0],rmj2,&x_err[0],rmj2_err); // &foo[0] converts vector "foo" to an array because TGraphErrors only accepts arrays
  graphsMJ.push_back(mj2);

  float maxMJ = 0.01;  
  for(int imax = 0;imax<nExt;imax++){
    if(rmj1[imax]>maxMJ) maxMJ=rmj1[imax];
    if(rmj2[imax]>maxMJ) maxMJ=rmj2[imax];
  }

  TGraphErrors *mt1 = new TGraphErrors(nExt,&x1[0],rmt1,&x_err[0],rmt1_err);  // &foo[0] convert vector "foo" to an array because TGraphErrors only accepts arrays
  graphsMT.push_back(mt1);
    
  TGraphErrors *mt2 = new TGraphErrors(nExt,&x2[0],rmt2,&x_err[0],rmt2_err); // &foo[0] converts vector "foo" to an array because TGraphErrors only accepts arrays
  graphsMT.push_back(mt2);

  float maxMT = 0.01;  
  for(int imax = 0;imax<nExt;imax++){
    if(rmt1[imax]>maxMT) maxMT=rmt1[imax];
    if(rmt2[imax]>maxMT) maxMT=rmt2[imax];
  }
  
  kappas = new TGraphErrors(nExt,&x[0],kappa,&x_err[0],kappa_err);
  
  styles style("RA4"); style.setDefaultStyle();
  TCanvas can;

  //make hist to define pad with labels on x-axis
  TH1F *h = new TH1F("for_axis_label",cuts2title(baseline),nExt,-0.5,nExt-0.5);
  for (int ih=1;ih<=nExt;ih++){ 
    h->GetXaxis()->SetBinLabel(ih,binlabels[ih-1]);
  }

  h->SetMaximum(1.5*maxMJ);
  h->SetMinimum(0.0);

  h->GetYaxis()->SetTitle("R_{MJ}");
  h->GetXaxis()->SetLabelSize(0.045);
  h->Draw();

  double legX = 0.65, legY = 0.89, legSingle = 0.14;
  double legW = 0.22, legH = legSingle;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.057); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);
  
  graphsMJ.at(0)->SetMarkerStyle(20);
  graphsMJ.at(0)->SetMarkerSize(1.2);
  graphsMJ.at(0)->SetMarkerColor(31);
  graphsMJ.at(0)->SetLineColor(31);
  graphsMJ.at(0)->Draw("P");
  leg.AddEntry(graphsMJ.at(0), sampleName+" "+cuts2title(mt_cut),"p");

  graphsMJ.at(1)->SetMarkerStyle(22);
  graphsMJ.at(1)->SetMarkerSize(1.2);
  graphsMJ.at(1)->SetMarkerColor(46);
  graphsMJ.at(1)->SetLineColor(46);
  graphsMJ.at(1)->Draw("P"); 
  leg.AddEntry(graphsMJ.at(1), sampleName+" "+cuts2title(invertcut(mt_cut)),"p");

  leg.Draw("p");
  
  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
  line.DrawLine((nExt-1)/2., 0, (nExt-1)/2., 1.5*maxMJ);
  TLatex *text78 = new TLatex(0.35,0.03,"n_{jets}= 7-8");
  text78->SetNDC();
  text78->SetTextSize(0.04);
  text78->SetLineWidth(2);
  text78->Draw();
  TLatex *text9 = new TLatex(0.7,0.03,"n_{jets}#geq 9");
  text9->SetNDC();
  text9->SetTextSize(0.04);
  text9->SetLineWidth(2);
  text9->Draw();    
  
  TString pname1 = "plots/closure/"+binlabels.at(nExt)+"_"+sampleNameText+"_rmj.pdf";
  TString pname1root = "plots/closure/"+binlabels.at(nExt)+"_"+sampleNameText+"_rmj.root";
  if(baseline.Contains("ntks_chg==0")) { pname1.ReplaceAll("_rmj","_rmj_ITV"); pname1root.ReplaceAll("_rmj","_rmj_ITV"); }
  can.SaveAs(pname1);
  can.SaveAs(pname1root);

  
  graphsMJ.at(0)->Delete();
  graphsMJ.at(1)->Delete();

  TCanvas can2;

  h->SetMaximum(1.5*maxMT);
  h->SetMinimum(0.0);

  h->GetYaxis()->SetTitle("R_{mT}");
  h->GetXaxis()->SetLabelSize(0.045);
  h->Draw();

  TLegend leg2(legX, legY-legH, legX+legW, legY);
  leg2.SetTextSize(0.057); leg2.SetFillColor(0); leg2.SetFillStyle(0); leg2.SetBorderSize(0);
  leg2.SetTextFont(132);

  graphsMT.at(0)->SetMarkerStyle(20);
  graphsMT.at(0)->SetMarkerSize(1.2);
  graphsMT.at(0)->SetMarkerColor(31);
  graphsMT.at(0)->SetLineColor(31);
  graphsMT.at(0)->Draw("P");
  leg2.AddEntry(graphsMT.at(0), sampleName+" "+cuts2title(mj_cut),"p");

  graphsMT.at(1)->SetMarkerStyle(22);
  graphsMT.at(1)->SetMarkerSize(1.2);
  graphsMT.at(1)->SetMarkerColor(46);
  graphsMT.at(1)->SetLineColor(46);
  graphsMT.at(1)->Draw("P");
  leg2.AddEntry(graphsMT.at(1), sampleName+" "+cuts2title(invertcut(mj_cut)),"p");

  leg2.Draw();

  text78->Draw();
  text9->Draw();
  line.DrawLine((nExt-1)/2., 0, (nExt-1)/2., 1.5*maxMT);

  TString pname2 = "plots/closure/"+binlabels.at(nExt)+"_"+sampleNameText+"_rmt.pdf";
  TString pname2root = "plots/closure/"+binlabels.at(nExt)+"_"+sampleNameText+"_rmt.root";
  if(baseline.Contains("ntks_chg==0")) { pname2.ReplaceAll("_rmt","_rmt_ITV"); pname2root.ReplaceAll("_rmt","_rmt_ITV");}
  can2.SaveAs(pname2);
  can2.SaveAs(pname2root);

  graphsMT.at(0)->Delete();
  graphsMT.at(1)->Delete();
  
  //now plot Kappa
  TCanvas can3;
  
  //make hist to define pad with labels on x-axis
  h->SetMaximum(2.0);
  h->GetYaxis()->SetTitle("\\kappa");
  h->Draw();

  kappas->SetMarkerStyle(20);
  kappas->SetMarkerSize(1.2);
  kappas->SetMarkerColor(kBlack);
  kappas->SetLineColor(kBlack);
  kappas->Draw("P");
  line.DrawLine(h->GetBinLowEdge(1), 1, h->GetBinLowEdge(h->GetNbinsX()+1), 1);

  //  line.DrawLine((nExt-1)/2., 0, (nExt-1)/2., 1.5*maxMT); //vertical line
  text78->Draw();
  text9->Draw();    
  
  TString pname3 = "plots/closure/"+binlabels.at(nExt)+"_"+sampleNameText+"_kappa.pdf";
  TString pname3root = "plots/closure/"+binlabels.at(nExt)+"_"+sampleNameText+"_kappa.root";
  if(baseline.Contains("ntks_chg==0")) { pname3.ReplaceAll("_kappa","_kappa_ITV"); pname3root.ReplaceAll("_kappa","_kappa_ITV");}
  can3.SaveAs(pname3);
  can3.SaveAs(pname3root);

  kappas->Delete();
  h->Delete();
}

double GetWeight(sfeats Sample, TString luminosity){

  TChain * chain = new TChain("tree");
  for(unsigned insam(0); insam < Sample.file.size(); insam++) 
    chain->Add(Sample.file[insam]);
  
  TH1D h_weight("weight", "",2000, 0, 2);
  h_weight.Sumw2();

  chain->Project("weight", "weight*"+luminosity);

  double weight = h_weight.GetMean(); // This method could be improved. Only works if all the weights are the same.
  
  return weight;
}

void GetYield(double& yield, double& yield_err, TString baseline,TString external,TString mj_cut,TString mt_cut, sfeats Sample,TString luminosity){

  TChain * chain = new TChain("tree");
  for(unsigned insam(0); insam < Sample.file.size(); insam++) 
    chain->Add(Sample.file[insam]);
  
  TH1D h_yield("yields", "",100, 0, 10000);
  h_yield.Sumw2();
  TH1D h_weight("weight", "",2000, 0, 2);
  h_weight.Sumw2();

  TString yieldCut = luminosity+"*weight*("+baseline+"&&"+external+"&&"+mj_cut+"&&"+mt_cut+"&&"+Sample.cut+")";
  
  chain->Project("yields", "met", yieldCut);
  chain->Project("weight", "weight*"+luminosity);

  double weight = h_weight.GetMean(); // This method could be improved. Only works if all the weights are the same.

  yield = h_yield.IntegralAndError(0,101,yield_err);
  
  if(yield==0) yield_err = weight;
}

void GetRatio(double& ratio, double& error, double num, double den, double num_err, double den_err, double weight){

  if(num==0 && den==0){
    ratio = 0;
    error = 0;
  }
  else if(num==0){ // Error on zero is ~1 event
    num = 0;
    num_err = weight;
  }
  else if(den==0){ //Pretend as if you saw 1 event.
    den = weight*1.;
    den_err = weight;
  }
  ratio = num/den;
  error = sqrt(pow(num_err/den,2)+pow(num/pow(den,2)*den_err,2));  
}

void GetKappa(double& kappa, double& error, double n1, double n2, double n3, double n4, double n1_err, double n2_err, double n3_err, double n4_err, double weight){

  int nzeroes = 0;
  if(n1==0) nzeroes++;
  if(n2==0) nzeroes++;
  if(n3==0) nzeroes++;
  if(n4==0) nzeroes++;

  if(nzeroes>=2){ // If more than 1 zero, don't calculate since one R-factor is indeterminate
    kappa = 0;
    error = 0;
  }
  else if(n1==0){ // Error on zero is ~1 event
    n1 = 0;
    n1_err = weight;
  }
  else if(n4==0){ // Error on zero is ~1 event
    n4 = 0;
    n4_err = weight;
  }
  else if(n2==0){ // Pretend as if you saw 1 event
    n2 = weight*1;
    n2_err = weight;
  }
  else if(n3==0){ // Pretend as if you saw 1 event
    n3 = weight*1;
    n3_err = weight;
  }
  kappa = (n4*n1)/(n3*n2);
  error = sqrt(pow((n4/(n2*n3))*n1_err,2)+pow((n1/(n2*n3))*n4_err,2)+pow(((n1*n4)/(pow(n2,2)*n3))*n2_err,2)+pow(((n1*n4)/(n2*pow(n3,2)))*n3_err,2));
}
