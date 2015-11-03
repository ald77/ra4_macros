// table_yields: Generates a LaTeX file with a cutflow table for RA4

#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH1D.h"
#include "TObject.h"
#include "TObjString.h"
#include "TObjArray.h"
#include "TMath.h"
#include "RooStats/NumberCountingUtils.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace  {
  TString ntuple_date("2015_05_25");
  TString luminosity=/*"0.135"*/"0.832";
  bool do_1b=false;
  bool do_2l=false;
  bool do_ttbaronly = false;
  bool do_note=true;
  bool do_zbi=false;

  int method = 1; // Only methods 1 and 3 currently supported
}

using namespace std;
using std::cout;
using std::endl;

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig);
float GetNdata(TString oneline, bool doMC);

int main(){

  // Reading ntuples
  TString folder, folder_ns, folder_1ldata;  
  folder        = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/mc/skim_1lht400mc/";
  folder_1ldata = "/net/cms2/cms2r0/babymaker/babies/2015_10_19/data/singlelep/combined/skim_1lht400/";
  vector<TString> s_trig_sl;
  s_trig_sl.push_back(folder_1ldata+"/*Single*");

  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJets*Lept*");
  s_tt.push_back(folder+"*_TTJets_HT*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*_WJetsToLNu*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder+"*_TTWJets*");
  s_ttv.push_back(folder+"*_TTZTo*");
  vector<TString> s_single;
  s_single.push_back(folder+"*_ST_*");
  vector<TString> s_other;
  s_other.push_back(folder+"*DYJetsToLL*");
  s_other.push_back(folder+"*_QCD_HT*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*_WWTo*");
  s_other.push_back(folder+"*ggZH_HToBB*");
  s_other.push_back(folder+"*ttHJetTobb*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*");

  vector<TChain *> chain;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_trig_sl, "Data", 1003,1,"(trig[4]||trig[8])&&pass&&(mt<140||(mt>140&&mj<400))"));
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

  int nsig(0);

  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  // 2l
  vector<TChain *> chain_2l;
  vector<sfeats> Samples_2l; 
  Samples_2l.push_back(sfeats(s_trig_sl, "Data", 1003,1,"(trig[4]||trig[8])&&pass"));
  if(!do_ttbaronly){
    Samples_2l.push_back(sfeats(s_other, "Other", 1001));
    Samples_2l.push_back(sfeats(s_ttv, "$t\\bar{t}V$", 1002));
    Samples_2l.push_back(sfeats(s_single, "Single $t$", 1005));
    Samples_2l.push_back(sfeats(s_wjets, "W+jets", 1004));
  }
  Samples_2l.push_back(sfeats(s_tt, "$t\\bar{t}$ (1$\\ell$)", 1000,1,
			   "ntruleps<=1"));
  Samples_2l.push_back(sfeats(s_tt, "$t\\bar{t}$ ($2\\ell$)", 1006,1,
			   "ntruleps>=2"));

  for(unsigned sam(0); sam < Samples_2l.size(); sam++){
    chain_2l.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples_2l[sam].file.size(); insam++)
      chain_2l[sam]->Add(Samples_2l[sam].file[insam]);
  }

  TString minjets("7"), midjets("8"), mjthresh("600"), highmet("400");
  if(method==1) {  minjets = "6"; midjets="99"; mjthresh="400"; highmet="9999";} 
  if(method==3) {  minjets ="6"; midjets="8"; mjthresh="400"; highmet="400";}  


  TString minjets_2l(""), midjets_2l("");
  minjets_2l += (minjets.Atoi()-1); midjets_2l += (midjets.Atoi()-1); 
  TString fom("$Z_{\\rm bi}$");
  if(!do_zbi) fom = "S/B";
  TString outname = "txt/yields_mj"+mjthresh+"_met"+highmet+"_njets"+minjets+midjets+"_lumi"+luminosity+"_"+ntuple_date+".tex";
  if(do_1b) outname.ReplaceAll("yields","yields_1b");
  if(do_2l) outname.ReplaceAll("yields","yields_2l");
  if(do_zbi) outname.ReplaceAll("yields","yields_zbi");

  TString baseline("ht>450&&met>200");
  TString cuts_1l(baseline+"&&(nmus+nels)==1&&nbm>=1&&njets>="+minjets+"");
  TString cuts_1l1b(baseline+"&&(nmus+nels)==1&&nbm==1&&njets>="+minjets+"");
  TString cuts_2l("ht>450&&met>200&&(nmus+nels)==2&&njets>="+minjets_2l+"");
  TString cuts_2lbb("ht>500&&met>200&&(nmus+nels)==2&&njets>="+minjets_2l+"");
  TString cuts_1ltex("$H_T>450, \\mathrm{MET}>200, n_{\\rm jets}\\geq "+minjets+", n_b\\geq1, n_{\\rm lep}=1$");
  TString cuts_1l1btex("$H_T>450, \\mathrm{MET}>200, n_{\\rm jets}\\geq "+minjets+", n_b=1, n_{\\rm lep}=1$");
  TString cuts_2ltex("$H_T>450, \\mathrm{MET}>200, n_{\\rm jets}\\geq "+minjets_2l+", n_{\\rm lep}=2$");
  TString cuts_2lbbtex("$H_T>450, \\mathrm{MET}>200, n_{\\rm jets}\\geq "+minjets_2l+", n_{\\rm lep}=2$");
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
  file<< " & MC bkg. ";
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
  float Ndata[4];
  float Nmc[4];
  TString onelineyields[4];
  for(int ind(0); ind<4; ind++) {regions[ind] = letter; regions[ind] += (ind+1);}
  
  //////////////////////////////////// RA4 regions //////////////////////////////////
    file << " \\multicolumn{"<< Samples.size()+1<<"}{c}{"<< cuts_1ltex  <<"} \\\\ \\hline \\hline\n";

    mjcut="mj<="+mjthresh; mtcut="mt<=140"; 
    onelineyields[0] = YieldsCut(regions[0]+": $m_T  \\leq 140,M_J\\leq "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file << onelineyields[0];
    file <<"\\hline\n";

    mjcut="mj>"+mjthresh; mtcut="mt<=140"; 
    onelineyields[1] =  YieldsCut(regions[1]+": $m_T  \\leq 140,M_J> "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file << onelineyields[1];
    file <<"\\hline\n";

    mjcut="mj<="+mjthresh; mtcut="mt>140"; 
    onelineyields[2] = YieldsCut(regions[2]+": $m_T  > 140,M_J \\leq "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file << onelineyields[2];
    file <<"\\hline\n";

    mjcut="mj>"+mjthresh; mtcut="mt>140"; 
    onelineyields[3] = YieldsCut(regions[3]+": $m_T  > 140,M_J > "+mjthresh+"$", mjcut+"&&"+mtcut+"&&"+cuts_1l, chain, Samples, nsig);
    file << onelineyields[3];
    file <<"\\hline\n";

    // Do backgrond prediction 
    // data = N2*N3/N1 
    // mc   = (N4/N3/(N2/N1) : ignore for this bc negligible
    Ndata[0] = GetNdata(onelineyields[0], 0);
    Ndata[1] = GetNdata(onelineyields[1], 0);
    Ndata[2] = GetNdata(onelineyields[2], 0);  
    Ndata[3] = GetNdata(onelineyields[3], 0);  
    Nmc[0] = GetNdata(onelineyields[0], 1);
    Nmc[1] = GetNdata(onelineyields[1], 1);
    Nmc[2] = GetNdata(onelineyields[2], 1);  
    Nmc[3] = GetNdata(onelineyields[3], 1);  

    cout << Ndata[0] << " " << Ndata[1] << " " << Ndata[2] << " " << Ndata[3] << endl;
    cout << Nmc[0] << " " << Nmc[1] << " " << Nmc[2] << " " << Nmc[3] << endl;

    // mc
    float kappa = (Nmc[3]/Nmc[2])/(Nmc[1]/Nmc[0]);
    // data  
    vector<vector<float> > entries, weights;   
    for(int i=0; i<4; i++){ 
        vector<float> tmp; 
        if(i<3) tmp.push_back(Ndata[i]); 
        else tmp.push_back(1);
        vector<float> tmpweights; 
        tmpweights.push_back(1);  

        entries.push_back(tmp);
        weights.push_back(tmpweights);
    }
    vector<float> powersk; powersk.push_back(-1); powersk.push_back(1); powersk.push_back(1); //powersk.push_back(-1);
    float mSigma, pSigma;
    float kappaData =  calcKappa(entries, weights, powersk, mSigma, pSigma, true, true);

    cout << "MC   : " << kappa  << endl;
    cout << "Data : " << kappaData  << " " << mSigma << " "  << pSigma << endl;

    cout << "Precition : " << kappa*kappaData << " +"  << kappa*pSigma << " -" << kappa*mSigma << endl; 

    file << "Data driven prediction & \\multicolumn{"<<Samples.size()-1 << "}{c}{" << Form("$%.1f^{+%.1f}_{-%.1f}$", kappa*kappaData, kappa*pSigma, kappa*mSigma) << "} \\\\ \n"; 
 
    //////////////////////////////////// 2l regions //////////////////////////////////
    file << "\\hline \\hline\\multicolumn{"<< Samples.size()+1<<"}{c}{"
	 <<cuts_2ltex <<"} \\\\ \\hline \\hline\n";

    mjcut="mj<="+mjthresh; 
    onelineyields[2] =  YieldsCut("D3: $M_J \\leq "+mjthresh+"$", "mj <= "+mjthresh+"&&"+cuts_2l, chain_2l, Samples_2l, nsig);
    file << onelineyields[2];
    file <<"\\hline\n";


    mjcut="mj>"+mjthresh; 
    onelineyields[3] = YieldsCut("D4: $M_J > "+mjthresh+"$", "mj>"+mjthresh+"&&"+cuts_2l, chain_2l, Samples_2l, nsig);
    file << onelineyields[3];
    file <<"\\hline\n";
   
    // closure
    Ndata[2] = GetNdata(onelineyields[2], 0);  
    Ndata[3] = GetNdata(onelineyields[3], 0);  
    Nmc[2] = GetNdata(onelineyields[2], 1);  
    Nmc[3] = GetNdata(onelineyields[3], 1);  

    cout << Ndata[0] << " " << Ndata[1] << " " << Ndata[2] << " " << Ndata[3] << endl;
    cout << Nmc[0] << " " << Nmc[1] << " " << Nmc[2] << " " << Nmc[3] << endl;

    // mc
    kappa = (Nmc[3]/Nmc[2])/(Nmc[1]/Nmc[0]);
    // data   
    entries.pop_back(); entries.pop_back();
    weights.pop_back(); weights.pop_back();
    for(int i=2; i<4; i++){ 
        vector<float> tmp; 
        if(i<3) tmp.push_back(Ndata[i]); 
        else tmp.push_back(1);
        vector<float> tmpweights; 
        tmpweights.push_back(1);  

        entries.push_back(tmp);
        weights.push_back(tmpweights);
    }
    kappaData =  calcKappa(entries, weights, powersk, mSigma, pSigma, true, true);

    cout << "MC   : " << kappa  << endl;
    cout << "Data : " << kappaData  << " " << mSigma << " "  << pSigma << endl;

    file << "Data driven prediction & \\multicolumn{"<<Samples.size()-1 << "}{c}{" << Form("$%.1f^{+%.1f}_{-%.1f}$", kappa*kappaData, kappa*pSigma, kappa*mSigma) << "} \\\\ \n"; 

  file << "\\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & MC bkg. ";
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
    if(Samples[sam].label.Contains("Data")) totCut = "("+cuts+"&&"+Samples[sam].cut+")";
    else totCut = luminosity+"*weight*("+cuts+"&&"+Samples[sam].cut+")";
    entries = getYieldErr(*chain[sam], totCut, yield_sam, err);
    yield.push_back(yield_sam);
    error.push_back(err);
    if(sam<nsam-nsig) {
      if(!Samples[sam].label.Contains("Data"))  
      {
        if(yield[sam]>0) bkg += yield[sam];
        bkg_err = sqrt(pow(bkg_err,2)+pow(err,2)); 
      }
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

float GetNdata(TString oneline, bool doMC){   
    int index = 1; 
    if(doMC) index = 8;
    TObjArray* tokens = oneline.Tokenize("&");
    TString Ndata_str(dynamic_cast<TObjString *>(tokens->At(index))->GetString());  
    delete tokens;

    if(!doMC) return atof(Ndata_str); 
    else { 
        TObjArray* tokensMC = Ndata_str.Tokenize(" "); 
        TString Nmc_str(dynamic_cast<TObjString *>(tokensMC->At(0))->GetString());   
        delete tokensMC;
        Nmc_str.ReplaceAll("$","");
        return atof(Nmc_str); 
    }
}


