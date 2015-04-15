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

using namespace std;
using std::cout;
using std::endl;


class sfeats {
public:
  sfeats(vector<TString> ifile, TString ilabel, int icolor, int istyle=1, TString icut="1"){
    file = ifile; label = ilabel; cut = icut;
    color = icolor; style = istyle;
    isSig = ifile[0].Contains("T1tttt");// && ifile.Contains("1200");
    factor = "1";
    //if(ifile[0].Contains("TTW")) factor = "0.36";
  }
  vector<TString> file;
  TString label, cut, factor;
  int color, style;
  bool isSig;
};

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
  Samples.push_back(sfeats(s_other, "Other", 1001));
  Samples.push_back(sfeats(s_ttv, "$t\\bar{t}V$", 1002));
  Samples.push_back(sfeats(s_single, "Single $t$", 1005));
  Samples.push_back(sfeats(s_wjets, "W + jets", 1004));
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

  
  TString metcut("400"), mjcut("400");
  TString name = "txt/ra4_yields_met"+metcut+"_mj"+mjcut+".tex";
  TString cuts_1bmt("(nmus+nels)==1&&ht>500&&mt>150&&nbm==1&&njets>=6");
  TString cuts_2bmt("(nmus+nels)==1&&ht>500&&mt>150&&nbm>=2&&njets>=6");
  TString cuts_1b("(nmus+nels)==1&&ht>500&&met>"+metcut+"&&nbm==1&&njets>=6");
  TString cuts_2b("(nmus+nels)==1&&ht>500&&met>"+metcut+"&&nbm==2&&njets>=6");
  TString cuts_3b("(nmus+nels)==1&&ht>500&&met>"+metcut+"&&nbm>=3&&njets>=6");
  TString cuts_2l("(nmus+nels)==2&&ht>500&&met>"+metcut+"&&nbm==1&&njets>=4");
  TString cuts_2lp("(nmus+nels)==2&&ht>500&&met>"+metcut+"&&nbm==2&&njets>=4");
  TString cuts_2l_3b("(nmus+nels)==2&&ht>500&&met>"+metcut+"&&nbm>=3&&njets>=4");
  ofstream file(name);
  
  file << "\n\\begin{tabular}{ l | rrrrrr | r | rr | rr}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{Cuts} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & {\\bf SM bkg.} ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & {\\bf S/B} ";
  file << "\\\\ \\hline \n ";

  //////////////////////////////////// MET study //////////////////////////////////
  file << " \\multicolumn{"<< Samples.size()+nsig*2<<"}{c}{"<< "$H_T>500, \\mathrm{MET}>200, m_T>150, n_{\\rm jets}\\geq 6, "
       <<"n_b=1, n_{\\rm lep}=1$"<<"} \\\\ \\hline\n";
  file << YieldsCut("MET$  < "+metcut+",m_J< "+mjcut+"$      ", "mj<"+mjcut+"&&met<"+metcut+"&&"+cuts_1bmt, chain, Samples, nsig);
  file << YieldsCut("MET$  < "+metcut+",m_J\\geq "+mjcut+"$  ", "mj>="+mjcut+"&&met<"+metcut+"&&"+cuts_1bmt, chain, Samples, nsig);
  file << YieldsCut("MET$\\geq "+metcut+",m_J< "+mjcut+"$    ", "mj<"+mjcut+"&&met>="+metcut+"&&"+cuts_1bmt, chain, Samples, nsig);
  file << YieldsCut("MET$\\geq "+metcut+",m_J\\geq "+mjcut+"$", "mj>="+mjcut+"&&met>="+metcut+"&&"+cuts_1bmt, chain, Samples, nsig);

  file << " \\hline\\multicolumn{"<< Samples.size()+nsig*2<<"}{c}{"<< "$H_T>500, \\mathrm{MET}>200, m_T>150, n_{\\rm jets}\\geq 6, "
       <<"n_b\\geq 2, n_{\\rm lep}=1$"<<"} \\\\ \\hline\n";
  file << YieldsCut("MET$  < "+metcut+",m_J< "+mjcut+"$      ", "mj<"+mjcut+"&&met<"+metcut+"&&"+cuts_2bmt, chain, Samples, nsig);
  file << YieldsCut("MET$  < "+metcut+",m_J\\geq "+mjcut+"$  ", "mj>="+mjcut+"&&met<"+metcut+"&&"+cuts_2bmt, chain, Samples, nsig);
  file << YieldsCut("MET$\\geq "+metcut+",m_J< "+mjcut+"$    ", "mj<"+mjcut+"&&met>="+metcut+"&&"+cuts_2bmt, chain, Samples, nsig);
  file << YieldsCut("MET$\\geq "+metcut+",m_J\\geq "+mjcut+"$", "mj>="+mjcut+"&&met>="+metcut+"&&"+cuts_2bmt, chain, Samples, nsig);
  //////////////////////////////////// MET study //////////////////////////////////


  // //////////////////////////////////// STANDARD //////////////////////////////////
  // file << " \\multicolumn{"<< Samples.size()+nsig*2<<"}{c}{"<< "$H_T>500, \\mathrm{MET}>"+metcut
  //      <<", n_{\\rm jets}\\geq 6, "
  //      <<"n_b=1, n_{\\rm lep}=1$"<<"} \\\\ \\hline\n";
  // file << YieldsCut("$m_T  < 150,m_J< 600$      ", "mj<600&&mt<150&&"+cuts_1b, chain, Samples, nsig);
  // file << YieldsCut("$m_T  < 150,m_J\\geq 600$  ", "mj>=600&&mt<150&&"+cuts_1b, chain, Samples, nsig);
  // file << YieldsCut("$m_T\\geq 150,m_J< 600$    ", "mj<600&&mt>=150&&"+cuts_1b, chain, Samples, nsig);
  // file << YieldsCut("$m_T\\geq 150,m_J\\geq 600$", "mj>=600&&mt>=150&&"+cuts_1b, chain, Samples, nsig);

  // file << " \\hline\\multicolumn{"<< Samples.size()+nsig*2<<"}{c}{"<< "$H_T>500, \\mathrm{MET}>"+metcut
  //      <<", n_{\\rm jets}\\geq 6, "
  //      <<"n_b= 2, n_{\\rm lep}=1$"<<"} \\\\ \\hline\n";
  // file << YieldsCut("$m_T  < 150,m_J< 600$      ", "mj<600&&mt<150&&"+cuts_2b, chain, Samples, nsig);
  // file << YieldsCut("$m_T  < 150,m_J\\geq 600$  ", "mj>=600&&mt<150&&"+cuts_2b, chain, Samples, nsig);
  // file << YieldsCut("$m_T\\geq 150,m_J< 600$    ", "mj<600&&mt>=150&&"+cuts_2b, chain, Samples, nsig);
  // file << YieldsCut("$m_T\\geq 150,m_J\\geq 600$", "mj>=600&&mt>=150&&"+cuts_2b, chain, Samples, nsig);

  // file << " \\hline\\multicolumn{"<< Samples.size()+nsig*2<<"}{c}{"<< "$H_T>500, \\mathrm{MET}>"+metcut
  //      <<", n_{\\rm jets}\\geq 4, "
  //      <<"n_b=1, n_{\\rm lep}=2$"<<"} \\\\ \\hline\n";
  // file << YieldsCut("$m_J< 600$", "mj<600&&"+cuts_2l, chain, Samples, nsig);
  // file << YieldsCut("$m_J\\geq 600$", "mj>=600&&"+cuts_2l, chain, Samples, nsig);

  // file << " \\hline\\multicolumn{"<< Samples.size()+nsig*2<<"}{c}{"<< "$H_T>500, \\mathrm{MET}>"+metcut
  //      <<", n_{\\rm jets}\\geq 4, "
  //      <<"n_b= 2, n_{\\rm lep}=2$"<<"} \\\\ \\hline\n";
  // file << YieldsCut("$m_J< 600$", "mj<600&&"+cuts_2lp, chain, Samples, nsig);
  // file << YieldsCut("$m_J\\geq 600$", "mj>=600&&"+cuts_2lp, chain, Samples, nsig);
  // //////////////////////////////////// STANDARD //////////////////////////////////


  //////////////////////////////////// 3b //////////////////////////////////
  // metcut = "200";
  // cuts_3b.ReplaceAll("met>250","met>200");
  // cuts_2l_3b.ReplaceAll("met>250","met>200");
  // file << " \\multicolumn{"<< Samples.size()+nsig*2<<"}{c}{"<< "$H_T>500, \\mathrm{MET}>"+metcut
  //      <<", n_{\\rm jets}\\geq 6, "
  //      <<"n_b\\geq 3, n_{\\rm lep}=1$"<<"} \\\\ \\hline\n";
  // file << YieldsCut("$m_T  < 150,m_J< 600$      ", "mj<600&&mt<150&&"+cuts_3b, chain, Samples, nsig);
  // file << YieldsCut("$m_T  < 150,m_J\\geq 600$  ", "mj>=600&&mt<150&&"+cuts_3b, chain, Samples, nsig);
  // file << YieldsCut("$m_T\\geq 150,m_J< 600$    ", "mj<600&&mt>=150&&"+cuts_3b, chain, Samples, nsig);
  // file << YieldsCut("$m_T\\geq 150,m_J\\geq 600$", "mj>=600&&mt>=150&&"+cuts_3b, chain, Samples, nsig);

  // file << " \\hline\\multicolumn{"<< Samples.size()+nsig*2<<"}{c}{"<< "$H_T>500, \\mathrm{MET}>"+metcut
  //      <<", n_{\\rm jets}\\geq 4, "
  //      <<"n_b\\geq 3, n_{\\rm lep}=2$"<<"} \\\\ \\hline\n";
  // file << YieldsCut("$m_J< 600$", "mj<600&&"+cuts_2l_3b, chain, Samples, nsig);
  // file << YieldsCut("$m_J\\geq 600$", "mj>=600&&"+cuts_2l_3b, chain, Samples, nsig);

  // metcut = "250";
  // cuts_3b.ReplaceAll("met>200","met>250");
  // cuts_2l_3b.ReplaceAll("met>200","met>250");
  // file << " \\multicolumn{"<< Samples.size()+nsig*2<<"}{c}{"<< "$H_T>500, \\mathrm{MET}>"+metcut
  //      <<", n_{\\rm jets}\\geq 6, "
  //      <<"n_b\\geq 3, n_{\\rm lep}=1$"<<"} \\\\ \\hline\n";
  // file << YieldsCut("$m_T  < 150,m_J< 600$      ", "mj<600&&mt<150&&"+cuts_3b, chain, Samples, nsig);
  // file << YieldsCut("$m_T  < 150,m_J\\geq 600$  ", "mj>=600&&mt<150&&"+cuts_3b, chain, Samples, nsig);
  // file << YieldsCut("$m_T\\geq 150,m_J< 600$    ", "mj<600&&mt>=150&&"+cuts_3b, chain, Samples, nsig);
  // file << YieldsCut("$m_T\\geq 150,m_J\\geq 600$", "mj>=600&&mt>=150&&"+cuts_3b, chain, Samples, nsig);

  // file << " \\hline\\multicolumn{"<< Samples.size()+nsig*2<<"}{c}{"<< "$H_T>500, \\mathrm{MET}>"+metcut
  //      <<", n_{\\rm jets}\\geq 4, "
  //      <<"n_b\\geq 3, n_{\\rm lep}=2$"<<"} \\\\ \\hline\n";
  // file << YieldsCut("$m_J< 600$", "mj<600&&"+cuts_2l_3b, chain, Samples, nsig);
  // file << YieldsCut("$m_J\\geq 600$", "mj>=600&&"+cuts_2l_3b, chain, Samples, nsig);
  //////////////////////////////////// 3b //////////////////////////////////





  file << " \\hline\\multicolumn{1}{c|}{Cuts} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & {\\bf SM bkg.} ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & {\\bf S/B} ";
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file.close();
  cout<<"Written "<<name<<endl;
}

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig){
  TString totCut, luminosity="4", Hname = "histo", out;
  vector<float> yield;
  float bkg(0);
  int nsam(chain.size());
  TH1D histo(Hname, "",100, 0, 10000);
  for(int sam(0); sam < nsam; sam++){
    totCut = luminosity+"*weight*("+cuts+"&&"+Samples[sam].cut+")";
    chain[sam]->Project(Hname, "met", totCut);
    yield.push_back(histo.Integral(0,101));
    if(sam<nsam-nsig) bkg += yield[sam];
    //cout<<sam<<": yield "<<Samples[sam].label<<" "<<yield[sam]<<" \t "<<totCut<<endl;
  }
  
  out = title;
  for(int sam(0); sam < nsam-nsig; sam++) out += (" \t & " + RoundNumber(yield[sam],1));
  out += (" \t & {\\bf "+RoundNumber(bkg,1))+"}";
  for(int sam(nsam-nsig); sam < nsam; sam++) 
    out += (" \t& " + RoundNumber(yield[sam],1) + " \t& {\\bf " +
	    RoundNumber(yield[sam],2,bkg)) + "}";
  //RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, 0.3),2));
  out += " \\\\ \n";
  //cout<<out;
  return out;
}
