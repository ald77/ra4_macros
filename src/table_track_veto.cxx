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

namespace localnames {
    TString luminosity="10";
}

using namespace localnames;

using namespace std;
using std::cout;
using std::endl;

TString YieldsCut(TString title, TString cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig);

int main(){

  // Reading ntuples
  TString folder="archive/2015_05_02/skim/";
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
  int nsig(3);
  Samples.push_back(sfeats(s_t1t, "T1tttt NC", 2));
  Samples.push_back(sfeats(s_t1t, "T1tttt NC, $1\\ell$", 2, 1,
			   "((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)<=1"));
  Samples.push_back(sfeats(s_t1tc, "T1tttt C", 2,2));

  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  

  TString name = "txt/all_track_veto_lumi_"+luminosity+".tex";
  TString cuts("(nmus+nels)==1&&ht>500&&met>200&&nbm>=1&&njets>=6&&mt>140&&mj>300");
 
  ifstream header("txt/header.tex");
  ifstream footer("txt/footer.tex");
  ofstream file(name);
  file<<header.rdbuf();
  file << "\n\\begin{tabular}{ l | ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++) file << "r";
  file<<" | r ";
  for(int sam(0); sam < nsig; sam++) file<<"| rr ";
  file<<"}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{${\\cal L} = 10$ fb$^{-1}$} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<<" &";//<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";

  //only use opposite sign tracks from primary lepton
  TString os("((tks_id*lep_charge)>0)"); //tks_id and tks_charge are opposite sign for leptons (way to go, Benjamin Franklin)
  TString os_had("((tks_id*lep_charge)<0)"); //tks_id and tks_charge are same sign for pions

  //exclude primary lepton
  TString notp("&&(!tks_is_primary)");

  //classify flavor
  TString elec("&&tks_id*tks_id==121");
  TString muon("&&tks_id*tks_id==169");
  TString had("&&!(tks_id*tks_id==121||tks_id*tks_id==169)");

  //abs iso for e (2 options), abs iso for mu (2 options), then charge iso for hadrons (2 options)
  TString iso[6] = {"&&(tks_pt*(tks_mini_ne+tks_mini_ch))<10","&&(tks_pt*(tks_mini_ne+tks_mini_ch))<20","&&(tks_pt*(tks_mini_ne+tks_mini_ch))<30","&&(tks_pt*(tks_mini_ne+tks_mini_ch))<50","&&(tks_pt*(tks_mini_ch))<2.5","&&tks_mini_ch<0.05"};
  // absolute iso = pt*(rel iso) 

  
  TString mtc[3] = {"&&tks_mt<80","&&tks_mt<90","&&tks_mt<100"};
  // TString dphi[3] = {"&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<0.5","&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<0.75","&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1.0"};
  

  //////////////////////////////////// Lepton veto study //////////////////////////////////
  file << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, $M_J>300, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 6, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";


  //Adjust "cuts" to change region definition
  //study region baseline counts
  file << YieldsCut("$M_J>300$", cuts, 
		    chain, Samples, nsig);


  //OR of all 3 vetoes
  file << YieldsCut("removed by had TV, mu TV or e TV, $", cuts+"&&(Sum$("+os_had+had+notp+iso[4]+mtc[1]+")>0||"+"Sum$("+os+muon+notp+iso[2]+mtc[1]+")>0||"+"Sum$("+os+elec+notp+iso[0]+mtc[1]+")>0)", 
  		    chain, Samples, nsig);


  /* // HADRONS
    file << YieldsCut("removed by had TV, $mT_{trk}<90 + abs chg MI<2.5 GeV, $", cuts+"&&Sum$("+os_had+had+notp+iso[4]+mtc[1]+")>0", 
		    chain, Samples, nsig);
  file << YieldsCut("removed by had TV, $mT_{trk}<90 + rel chg MI<0.05, $", cuts+"&&Sum$("+os_had+had+notp+iso[5]+mtc[1]+")>0", 
		    chain, Samples, nsig);
  file << YieldsCut("removed by had TV, $mT_{trk}<100 + abs chg MI<2.5 GeV, $", cuts+"&&Sum$("+os_had+had+notp+iso[4]+mtc[2]+")>0", 
		    chain, Samples, nsig);
  file << YieldsCut("removed by had TV, $mT_{trk}<100 + rel chg MI<0.05, $", cuts+"&&Sum$("+os_had+had+notp+iso[5]+mtc[2]+")>0", 
  chain, Samples, nsig);
  
  */

 /* // MUONS
    file << YieldsCut("removed by mu TV, $mT_{trk}<90 + abs chg+neu MI<30 GeV, $", cuts+"&&Sum$("+os+muon+notp+iso[2]+mtc[1]+")>0", 
		    chain, Samples, nsig);
  file << YieldsCut("removed by mu TV, $mT_{trk}<90 + abs chg+neu MI<50 GeV, $", cuts+"&&Sum$("+os+muon+notp+iso[3]+mtc[1]+")>0", 
		    chain, Samples, nsig);
  file << YieldsCut("removed by mu TV, $mT_{trk}<100 + abs chg+neu MI<30 GeV, $", cuts+"&&Sum$("+os+muon+notp+iso[2]+mtc[2]+")>0", 
		    chain, Samples, nsig);
  file << YieldsCut("removed by mu TV, $mT_{trk}<100 + abs chg+neu MI<50 GeV, $", cuts+"&&Sum$("+os+muon+notp+iso[3]+mtc[2]+")>0", 
  chain, Samples, nsig);
  
  */


   /* // ELECTRONS
    file << YieldsCut("removed by e TV, $mT_{trk}<90 + abs chg+neu MI<10 GeV, $", cuts+"&&Sum$("+os+elec+notp+iso[0]+mtc[1]+")>0", 
		    chain, Samples, nsig);
  file << YieldsCut("removed by e TV, $mT_{trk}<90 + abs chg+neu MI<20 GeV, $", cuts+"&&Sum$("+os+elec+notp+iso[1]+mtc[1]+")>0", 
		    chain, Samples, nsig);
  file << YieldsCut("removed by e TV, $mT_{trk}<100 + abs chg+neu MI<10 GeV, $", cuts+"&&Sum$("+os+elec+notp+iso[0]+mtc[2]+")>0", 
		    chain, Samples, nsig);
  file << YieldsCut("removed by e TV, $mT_{trk}<100 + abs chg+neu MI<20 GeV, $", cuts+"&&Sum$("+os+elec+notp+iso[1]+mtc[2]+")>0", 
  chain, Samples, nsig);
  
  */
  


  file << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< "&";//<< " & $Z_{\\rm bi}$ ";
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
    // float fracerr(sqrt(pow(bkg_err/bkg,2)+0.3*0.3+0.24*0.24));
    out += (" \t& $" + RoundNumber(yield[sam],1)+" \\pm "+RoundNumber(error[sam],1))+"&";// + "$ \t& " +
	    //RoundNumber(yield[sam],2,bkg));
	    // RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2));
	    cout<<", S = "<<RoundNumber(yield[sam],1)+" +- "+RoundNumber(error[sam],1);//<<" with Zbi = ";
	    //cout<<RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2);
  }
  out += " \\\\ \n";
  cout<<endl;
  return out;
}
