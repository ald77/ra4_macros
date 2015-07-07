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
#include "TGraphAsymmErrors.h"
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

void EffErr(float pass, float total, float weight, float &eff, float &ehigh, float &elow);
TString YieldsCut(TString title, TString event_cuts, TString track_cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig);

int main(){

  // Reading ntuples
  TString folder="/Users/heller/code/skim_tight/";
  
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
  //Samples.push_back(sfeats(s_other, "Other", 1001));
  //Samples.push_back(sfeats(s_ttv, "$t\\bar{t}V$", 1002));
  //Samples.push_back(sfeats(s_single, "Single $t$", 1005));
  // Samples.push_back(sfeats(s_wjets, "W+jets", 1004));
  //Samples.push_back(sfeats(s_tt, "$t\\bar{t}$ (1$\\ell$)", 1000,1,
  //	   "((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)<=1"));
  Samples.push_back(sfeats(s_tt, "$t\\bar{t}$ ($2\\ell$) efficiency", 1006,1,
			   "((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)>=2"));
  int nsig(2);
  Samples.push_back(sfeats(s_t1t, "T1tttt NC", 2));
  // Samples.push_back(sfeats(s_t1t, "T1tttt NC, $1\\ell$", 2, 1,
  //			   "((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)<=1"));
  Samples.push_back(sfeats(s_t1tc, "T1tttt C", 2,2));

  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  

 
  TString cuts("(nmus+nels)==1&&ht>500&&met>200&&nbm>=1&&njets>=7&&mt>140&&mj>300");
 
 

  //only use opposite sign tracks from primary lepton
  TString os("((tks_id*lep_charge)>0)"); //tks_id and tks_charge are opposite sign for leptons (way to go, Benjamin Franklin)
  TString os_had("((tks_id*lep_charge)<0)"); //tks_id and tks_charge are same sign for pions

  //exclude primary lepton
  TString notp("&&(!tks_is_primary)");

  //classify flavor
  TString elec("&&tks_id*tks_id==121");
  TString muon("&&tks_id*tks_id==169");
  TString had("&&!(tks_id*tks_id==121||tks_id*tks_id==169)");

  vector<TString> trackdefs;
  trackdefs.push_back(os+notp+elec);
  trackdefs.push_back(os+notp+muon);
  trackdefs.push_back(os_had+notp+had);
  trackdefs.push_back(os_had+notp+"&&tks_pt>15"+had);
  //abs iso for e (2 options), abs iso for mu (2 options), then charge iso for hadrons (2 options)
  //TString iso[6] = {"&&(tks_pt*(tks_mini_ne+tks_mini_ch))<10","&&(tks_pt*(tks_mini_ne+tks_mini_ch))<20","&&(tks_pt*(tks_mini_ne+tks_mini_ch))<30","&&(tks_pt*(tks_mini_ne+tks_mini_ch))<50","&&(tks_pt*(tks_mini_ch))<2.5","&&tks_mini_ch<0.05"};
  // absolute iso = pt*(rel iso) 

  
  TString mtc[3] = {"&&tks_mt<80","&&tks_mt<90","&&tks_mt<100"};
  // TString dphi[3] = {"&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<0.5","&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<0.75","&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1.0"};
   vector<TString> isotypes;
  vector<TString> isonames;

  isonames.push_back("abs chg+neu mini isolation");  isotypes.push_back("(tks_pt)*min((tks_mini_ne+tks_mini_ch),(tks_r02_ne+tks_r02_ch))");
  isonames.push_back("abs chg+neu R=0.5 mini isolation");  isotypes.push_back("(tks_pt)*min((tks_mini_ne+tks_mini_ch),(tks_r05_ne+tks_r05_ch))");
  isonames.push_back("abs chg+neu untruncated mini isolation"); isotypes.push_back("(tks_pt)*(tks_mini_ne+tks_mini_ch)");

  isonames.push_back("abs chg mini isolation");  isotypes.push_back("(tks_pt)*min((tks_mini_ch),(tks_r02_ch))");
  isonames.push_back("abs chg R=0.5 mini isolation");  isotypes.push_back("(tks_pt)*min((tks_mini_ch),(tks_r05_ch))");
  isonames.push_back("abs chg untruncated mini isolation"); isotypes.push_back("(tks_pt)*(tks_mini_ch)");

  isonames.push_back("rel chg+neu mini isolation");  isotypes.push_back("min((tks_mini_ne+tks_mini_ch),(tks_r02_ne+tks_r02_ch))");
  isonames.push_back("rel chg+neu R=0.5 mini isolation");  isotypes.push_back("min((tks_mini_ne+tks_mini_ch),(tks_r05_ne+tks_r05_ch))");
  isonames.push_back("rel chg+neu untruncated mini isolation"); isotypes.push_back("(tks_mini_ne+tks_mini_ch)");

  isonames.push_back("rel chg mini isolation");  isotypes.push_back("min((tks_mini_ch),(tks_r02_ch))");
  isonames.push_back("rel chg R=0.5 mini isolation");  isotypes.push_back("min((tks_mini_ch),(tks_r05_ch))");
  isonames.push_back("rel chg untruncated mini isolation"); isotypes.push_back("(tks_mini_ch)");
  
  vector<int> eliso,muiso,hadiso;
  vector<TString> elcut,mucut,hadcut;
  vector< vector<int> > alliso;
  vector< vector<TString> > allcut;
  eliso.push_back(0); elcut.push_back("1.0");
  eliso.push_back(0); elcut.push_back("2.5");
  eliso.push_back(6); elcut.push_back("0.1");

  muiso.push_back(0); mucut.push_back("2.5");
  muiso.push_back(0); mucut.push_back("5.0");
  muiso.push_back(6); mucut.push_back("0.15");
  muiso.push_back(6); mucut.push_back("0.2");

  hadiso.push_back(3); hadcut.push_back("1.0");
  hadiso.push_back(3); hadcut.push_back("2.5");
  hadiso.push_back(3); hadcut.push_back("5.0");
  hadiso.push_back(9); hadcut.push_back("0.05");
  hadiso.push_back(9); hadcut.push_back("0.1");
  
  alliso.push_back(eliso);  alliso.push_back(muiso);  alliso.push_back(hadiso);  alliso.push_back(hadiso);
  allcut.push_back(elcut);  allcut.push_back(mucut);  allcut.push_back(hadcut);  allcut.push_back(hadcut);
  
  vector<int> finaliso;
  vector<TString> finalcut;
  finaliso.push_back(6); finalcut.push_back("0.1");
  finaliso.push_back(6); finalcut.push_back("0.2");
  finaliso.push_back(9); finalcut.push_back("0.05");

  vector<TString> tracknames;
  tracknames.push_back("els");
  tracknames.push_back("mus");
  tracknames.push_back("had");
  tracknames.push_back("had2");

 vector<TString> tracknames2;
  tracknames2.push_back("el");
  tracknames2.push_back("mu");
  tracknames2.push_back("had");  
  tracknames2.push_back("had, p$_{T} > 15$ "); 


  

  TString fastveto_prompt = "&&Sum$("+os+elec+notp+"&&tks_from_w&&(tks_pt)*min((tks_mini_ne+tks_mini_ch),(tks_r02_ne+tks_r02_ch))<2.5"+mtc[1]+")>0";
  TString fastveto_nonprompt = "&&Sum$("+os+elec+notp+"&&!tks_from_w&&(tks_pt)*min((tks_mini_ne+tks_mini_ch),(tks_r02_ne+tks_r02_ch))<2.5"+mtc[1]+")>0";
  
  //////////////////////////////////// Lepton veto study //////////////////////////////////
  TString sumname = "txt/summary_veto_lumi_"+luminosity+".tex";
 ifstream sumheader("txt/header.tex");
  ifstream sumfooter("txt/footer.tex");
  ofstream sumfile(sumname);
  sumfile<<sumheader.rdbuf();
  sumfile << "\n\\begin{tabular}{ l | ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++) sumfile << "r";
  sumfile<<" | r ";
  for(int sam(0); sam < nsig; sam++) sumfile<<"| r | r ";
  sumfile<<"}\\hline\\hline\n";
  sumfile << " \\multicolumn{1}{c|}{${\\cal L} = 10$ fb$^{-1}$} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    sumfile << " & "<<Samples[sam].label;
  sumfile<< " & $t\\bar{t}$ ($2\\ell$) counts ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    sumfile << " & "<<Samples[sam].label<<" effic &"<<Samples[sam].label<<" counts ";//<< " & $Z_{\\rm bi}$ ";
  sumfile << "\\\\ \\hline \n ";


  sumfile << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, M_J>300, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 7, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";



sumfile << YieldsCut("$MJ>300$", cuts, "",
		    chain, Samples, nsig);
  sumfile << "\\hline \n ";

TString eveto ="Sum$("+trackdefs.at(0)+"&&"+isotypes.at(finaliso.at(0))+"<"+finalcut.at(0)+mtc[2]+")>0";
  
 sumfile << YieldsCut("has e, " + isonames.at(finaliso.at(0)) + " $<$ " + finalcut.at(0) + " ", cuts,"&&"+eveto, 
  		    chain, Samples, nsig);

TString muveto ="Sum$("+trackdefs.at(1)+"&&"+isotypes.at(finaliso.at(1))+"<"+finalcut.at(1)+mtc[2]+")>0";
  
 sumfile << YieldsCut("has mu, " + isonames.at(finaliso.at(1)) + " $<$ " + finalcut.at(1) + " ", cuts,"&&"+muveto, 
  		    chain, Samples, nsig);

TString hadveto ="Sum$("+trackdefs.at(3)+"&&"+isotypes.at(finaliso.at(2))+"<"+finalcut.at(2)+mtc[2]+")>0";
  
 sumfile << YieldsCut("has hadron p$_{T}$ $>$ 15, " + isonames.at(finaliso.at(2)) + " $<$ " + finalcut.at(2) + " ", cuts,"&&"+hadveto, 
  		    chain, Samples, nsig);

  sumfile << "\\hline \n ";

  
 sumfile << YieldsCut("removed by e, mu or had track veto ", cuts,"&&("+eveto+"||"+muveto+"||"+hadveto+")", 
  		    chain, Samples, nsig);

  
 
  sumfile << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    sumfile << " & "<<Samples[sam].label;
  sumfile<< " & $t\\bar{t}$ ($2\\ell$) counts ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    sumfile << " & "<<Samples[sam].label<<" effic &"<<Samples[sam].label<<" counts ";//<< " & $Z_{\\rm bi}$ ";
  sumfile << "\\\\ \n ";

  sumfile<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
sumfile << "\\\\ \n ";
 sumfile<< "\\hfill \\break \n";
sumfile<< "\\hfill \\break \n";

 sumfile << "\n\\begin{tabular}{ l | ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++) sumfile << "r";
  sumfile<<" | r ";
  for(int sam(0); sam < nsig; sam++) sumfile<<"| r | r ";
  sumfile<<"}\\hline\\hline\n";
  sumfile << " \\multicolumn{1}{c|}{${\\cal L} = 10$ fb$^{-1}$} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    sumfile << " & "<<Samples[sam].label;
  sumfile<< " & $t\\bar{t}$ ($2\\ell$) counts ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    sumfile << " & "<<Samples[sam].label<<" effic &"<<Samples[sam].label<<" counts ";//<< " & $Z_{\\rm bi}$ ";
  sumfile << "\\\\ \\hline \n ";
   sumfile << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, M_J>300, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 7, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";


 sumfile << YieldsCut("$MJ>300, MET>400$", cuts+"&&met>400", "",
		    chain, Samples, nsig);
  sumfile << "\\hline \n ";

  //TString eveto ="Sum$("+trackdefs.at(0)+isotypes.at(finaliso.at(0))+"<"+finalcut.at(0)+mtc[2]+")>0";
  
 sumfile << YieldsCut("has e, " + isonames.at(finaliso.at(0)) + " $<$ " + finalcut.at(0) + " ", cuts+"&&met>400","&&"+eveto, 
  		    chain, Samples, nsig);

 //TString muveto ="Sum$("+trackdefs.at(1)+isotypes.at(finaliso.at(1))+"<"+finalcut.at(1)+mtc[2]+")>0";
  
 sumfile << YieldsCut("has mu, " + isonames.at(finaliso.at(1)) + " $<$ " + finalcut.at(1) + " ", cuts+"&&met>400","&&"+muveto, 
  		    chain, Samples, nsig);

 //TString hadveto ="Sum$("+trackdefs.at(3)+isotypes.at(finaliso.at(3))+"<"+finalcut.at(3)+mtc[2]+")>0";
  
 sumfile << YieldsCut("has hadron p$_{T}$ $>$ 15, " + isonames.at(finaliso.at(2)) + " $<$ " + finalcut.at(2) + " ", cuts+"&&met>400","&&"+hadveto, 
  		    chain, Samples, nsig);

  sumfile << "\\hline \n ";

  
 sumfile << YieldsCut("removed by e, mu or had track veto ", cuts+"&&met>400","&&("+eveto+"||"+muveto+"||"+hadveto+")", 
  		    chain, Samples, nsig);

  sumfile << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    sumfile << " & "<<Samples[sam].label;
  sumfile<< " & $t\\bar{t}$ ($2\\ell$) counts ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    sumfile << " & "<<Samples[sam].label<<" effic &"<<Samples[sam].label<<" counts ";//<< " & $Z_{\\rm bi}$ ";
  sumfile << "\\\\ \n ";

  sumfile<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
sumfile << "\\\\ \n ";

sumfile<< "\\hfill \\break \n";
sumfile<< "\\hfill \\break \n";

  
 
   

  sumfile<<sumfooter.rdbuf();
  sumfile.close();
  cout<<"Written "<<sumname<<endl;

  for(int itrack=0;itrack<4;itrack++){
    TString name = "txt/"+tracknames.at(itrack)+"_veto_lumi_"+luminosity+".tex";
 ifstream header("txt/header.tex");
  ifstream footer("txt/footer.tex");
  ofstream file(name);
  file<<header.rdbuf();
  file << "\n\\begin{tabular}{ l | ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++) file << "r";
  file<<" | r ";
  for(int sam(0); sam < nsig; sam++) file<<"| r | r ";
  file<<"}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{${\\cal L} = 10$ fb$^{-1}$} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & $t\\bar{t}$ ($2\\ell$) counts ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<<" effic &"<<Samples[sam].label<<" counts ";//<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";


  file << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, M_J>300, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 7, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";



file << YieldsCut("$MJ>300$", cuts, "",
		    chain, Samples, nsig);
  file << "\\hline \n ";
  for(unsigned int iiso=0;iiso<alliso.at(itrack).size(); iiso++){
    TString veto_prompt ="&&Sum$("+trackdefs.at(itrack)+"&&tks_from_w&&"+isotypes.at(alliso.at(itrack).at(iiso))+"<"+allcut.at(itrack).at(iiso)+mtc[2]+")>0";
    TString veto_nonprompt ="&&Sum$("+trackdefs.at(itrack)+"&&!tks_from_w&&"+isotypes.at(alliso.at(itrack).at(iiso))+"<"+allcut.at(itrack).at(iiso)+mtc[2]+")>0";

 file << YieldsCut("have prompt "+tracknames2.at(itrack)+", " + isonames.at(alliso.at(itrack).at(iiso)) + " $<$ " + allcut.at(itrack).at(iiso) + " ", cuts,veto_prompt, 
  		    chain, Samples, nsig);
 file << YieldsCut("have fake "+tracknames2.at(itrack)+" " + isonames.at(alliso.at(itrack).at(iiso)) + " $<$ " + allcut.at(itrack).at(iiso) + " ", cuts,veto_nonprompt, 
  		    chain, Samples, nsig);
  file << "\\hline \n ";
  }
 
  file << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & $t\\bar{t}$ ($2\\ell$) counts ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<<" effic &"<<Samples[sam].label<<" counts ";//<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
file << "\\\\ \n ";
 file<< "\\hfill \\break \n";
file<< "\\hfill \\break \n";

 file << "\n\\begin{tabular}{ l | ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++) file << "r";
  file<<" | r ";
  for(int sam(0); sam < nsig; sam++) file<<"| r | r ";
  file<<"}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{${\\cal L} = 10$ fb$^{-1}$} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & $t\\bar{t}$ ($2\\ell$) counts ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<<" effic &"<<Samples[sam].label<<" counts ";//<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";
   file << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, M_J>300, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 7, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";


 file << YieldsCut("$MJ>300, MET>400$", cuts+"&&met>400", "",
		    chain, Samples, nsig);
  file << "\\hline \n ";

  for(unsigned int iiso=0;iiso<alliso.at(itrack).size(); iiso++){
    TString veto_prompt ="&&Sum$("+trackdefs.at(itrack)+"&&tks_from_w&&"+isotypes.at(alliso.at(itrack).at(iiso))+"<"+allcut.at(itrack).at(iiso)+mtc[2]+")>0";
    TString veto_nonprompt ="&&Sum$("+trackdefs.at(itrack)+"&&!tks_from_w&&"+isotypes.at(alliso.at(itrack).at(iiso))+"<"+allcut.at(itrack).at(iiso)+mtc[2]+")>0";


 file << YieldsCut("have prompt "+tracknames2.at(itrack)+", " + isonames.at(alliso.at(itrack).at(iiso)) + " $<$ " + allcut.at(itrack).at(iiso) + " ", cuts+"&&met>400",veto_prompt, 
  		    chain, Samples, nsig);
 file << YieldsCut("have fake "+tracknames2.at(itrack)+" " + isonames.at(alliso.at(itrack).at(iiso)) + " $<$ " + allcut.at(itrack).at(iiso) + " ", cuts+"&&met>400",veto_nonprompt, 
  		    chain, Samples, nsig);
  file << "\\hline \n ";
  }

  file << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & $t\\bar{t}$ ($2\\ell$) counts ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<<" effic &"<<Samples[sam].label<<" counts ";//<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
file << "\\\\ \n ";

file<< "\\hfill \\break \n";
file<< "\\hfill \\break \n";

  
 
   

  file<<footer.rdbuf();
  file.close();
  cout<<"Written "<<name<<endl;
  }
}

void EffErr(float pass, float total, float weight, Double_t &eff, Double_t &ehigh, Double_t &elow)
{
  
  TH1F * h_pass;
  h_pass = new TH1F("hist_pass","",1,0,2);
  TH1F * h_tot;
  h_tot = new TH1F("hist_tot","",1,0,2);
  h_pass->SetBinContent(h_pass->FindBin(1.0),pass/weight);
  h_tot->SetBinContent(h_tot->FindBin(1.0),total/weight);

  TGraphAsymmErrors g1;
  g1.Divide(h_pass,h_tot);
  Int_t bin = 0;
  Double_t x;
  g1.GetPoint(bin,x,eff);
  ehigh=g1.GetErrorYhigh(bin);
  elow=g1.GetErrorYlow(bin);

  h_pass->Delete();
  h_tot->Delete();
  
}


TString YieldsCut(TString title, TString event_cuts, TString track_cuts, vector<TChain*> chain, vector<sfeats> Samples, int nsig){
  TString totCut, Hname = "histo", out, Hname2="histo2";
  vector<double> yield, error,reg_yield, reg_error, weight;
  double bkg(0), bkg_err(0), err,err2;
  int nsam(chain.size());
  TH1D histo(Hname, "",100, 0, 10000);
  histo.Sumw2();
  TH1D histo2(Hname2, "",100, 0, 10000);
  histo2.Sumw2();
 
  for(int sam(0); sam < nsam; sam++){
    
    totCut = luminosity+"*weight*("+event_cuts+"&&"+Samples[sam].cut+")";
    chain[sam]->Project(Hname2, "met", totCut);
    reg_yield.push_back(histo2.IntegralAndError(0,101,err2));
    reg_error.push_back(err2);
    weight.push_back(histo2.Integral()/histo2.GetEntries());
   
    
    totCut = luminosity+"*weight*("+event_cuts+track_cuts+"&&"+Samples[sam].cut+")";
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
  
  for(int sam(0); sam < nsam-nsig; sam++){
    // if(sam%2 == 1 )
    Double_t ehigh, elow,eff;
    EffErr(yield[sam],reg_yield[sam],weight[sam],eff,ehigh,elow);
    if(yield[sam]!=reg_yield[sam]){
      out += (" \t & $" + RoundNumber(eff,2)+"^{+"+RoundNumber(ehigh,2) + "}_{-"+RoundNumber(elow,2)+"}$") ;
    }
    else out+= "\t &";
    //  out += (" \t & " + RoundNumber(yield[sam]/reg_yield[sam],2)+" \\pm "+RoundNumber((yield[sam]/reg_yield[sam])*sqrt(pow(reg_error[sam]/reg_yield[sam],2)+pow(error[sam]/yield[sam],2)),2));
    // else out+=" \t &";
  }

  out += (" \t &  $"+RoundNumber(bkg,1))+" \\pm "+RoundNumber(bkg_err,1)+"$";
  
  for(int sam(nsam-nsig); sam < nsam; sam++) {
    // float fracerr(sqrt(pow(bkg_err/bkg,2)+0.3*0.3+0.24*0.24));
    Double_t ehigh, elow,eff;
    EffErr(yield[sam],reg_yield[sam],weight[sam],eff,ehigh,elow);
    if(yield[sam]!=reg_yield[sam]){
      out += (" \t& $"+ RoundNumber(eff,2)+"^{+"+RoundNumber(ehigh,2) + "}_{-"+RoundNumber(elow,2) +"}$ & \t $"+ RoundNumber(yield[sam],1)+" \\pm "+RoundNumber(error[sam],1)+"$ ");// + "$ \t& " +
    }
    else out+= ("\t& & \t $"+ RoundNumber(yield[sam],1)+" \\pm "+RoundNumber(error[sam],1)+"$ ");
    //RoundNumber(yield[sam],2,bkg));
	    // RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2));
	    cout<<", S = "<<RoundNumber(yield[sam],1)+" +- "+RoundNumber(error[sam],1);//<<" with Zbi = ";
	    //cout<<RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2);
  }
  out += " \\\\ \n";
  cout<<endl;
  return out;
}
