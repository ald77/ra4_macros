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
}

using namespace ra4;

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

  int veto_type(0);

  TString name = "txt/ra4_lepton_veto_cutflow_loose_iso_lumi"+luminosity+".tex";
  TString cuts[3] = {"(nmus+nels)==1&&ht>500&&met>200&&nbm>=1&&njets>=6&&mt>140","(nmus+nels)==1&&ht>500&&mj>300&&met>200&&nbm>=1&&njets>=6&&mt>140","(nmus+nels)==1&&ht>500&&met>400&&nbm>=1&&njets>=6&&mt>140"};
  TString cutnames[3] = {"baseline","MJ>300","MET>400"};
  TString vetocut("(nvels+nvmus)==1"), tagcut("LV");
  if(veto_type==1) {
    name.ReplaceAll("lepton","track");
    vetocut = "ntks==1";
    tagcut = "TV";
  }else if(veto_type==2){
    name.ReplaceAll("lepton","track_chg");
    vetocut = "ntks_chg==0";
    tagcut = "TV\\_chg";
  }

  
  TString os("((tks_id*lep_charge)>0)");
  TString ss("((tks_id*lep_charge)<0)");
  TString notp("&&(!tks_is_primary)");
  TString elec("&&tks_id*tks_id==121");
  TString muon("&&tks_id*tks_id==169");
  TString had("&&!(tks_id*tks_id==121||tks_id*tks_id==169)");
  TString taut("2>1");
  
  //TString chgiso[2] = {"&&tks_mini_ch<0.1","&&tks_mini_ch<0.2"}
  TString iso[4]={"&&(tks_mini_ne+tks_mini_ch)<0.1","&&(tks_mini_ne+tks_mini_ch)<2.0","&&tks_mini_ch<0.1","&&tks_mini_ch<2.0"};
  TString mtc[3] = {"&&tks_mt<80","&&tks_mt<90","&&tks_mt<100"};
  TString dphi[3] = {"&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<0.5","&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<0.75","&&abs(abs(abs(tks_phi-met_phi)-3.14159)-3.14159)<1.0"};

  ifstream header("txt/header.tex");
  ifstream footer("txt/footer.tex");
  ofstream file(name);
  file<<header.rdbuf();

  //cutflow

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
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";

  for(int ibaseline=0;ibaseline<3;ibaseline++){
    file << YieldsCut(cutnames[ibaseline], cuts[ibaseline],chain, Samples, nsig);
    file << YieldsCut("notp, elec", cuts[ibaseline]+"&&Sum$("+taut+notp+elec+")>0",chain, Samples, nsig);
    file << YieldsCut("+ os", cuts[ibaseline]+"&&Sum$("+os+notp+elec+")>0",chain, Samples, nsig);
    file << YieldsCut("+ only one", cuts[ibaseline]+"&&Sum$("+os+notp+elec+")==1",chain, Samples, nsig);
    file << YieldsCut("+ chg iso 0.2", cuts[ibaseline]+"&&Sum$("+os+notp+elec+iso[3]+")==1",chain, Samples, nsig);
    file << YieldsCut("+ tot iso 0.2", cuts[ibaseline]+"&&Sum$("+os+notp+elec+iso[1]+")==1",chain, Samples, nsig);
    file << YieldsCut("+ mt<90", cuts[ibaseline]+"&&Sum$("+os+notp+elec+iso[3]+mtc[1]+")==1",chain, Samples, nsig);
    file << YieldsCut("+ dphi<1.0", cuts[ibaseline]+"&&Sum$("+os+notp+elec+iso[3]+dphi[2]+")==1",chain, Samples, nsig);
  }

  file << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \n ";
  
  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file<<" \\end{sidewaystable}"<<endl;
  file<<"\\newpage\n";
  file<<"\\begin{sidewaystable}[hp]\\centering"<<endl;
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
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";

  for(int ibaseline=0;ibaseline<2;ibaseline++){
    file << YieldsCut(cutnames[ibaseline], cuts[ibaseline],chain, Samples, nsig);
    file << YieldsCut("notp, muon", cuts[ibaseline]+"&&Sum$("+taut+notp+muon+")>0",chain, Samples, nsig);
    file << YieldsCut("+ os", cuts[ibaseline]+"&&Sum$("+os+notp+muon+")>0",chain, Samples, nsig);
    file << YieldsCut("+ only one", cuts[ibaseline]+"&&Sum$("+os+notp+muon+")==1",chain, Samples, nsig);
    file << YieldsCut("+ chg iso 0.2", cuts[ibaseline]+"&&Sum$("+os+notp+muon+iso[3]+")==1",chain, Samples, nsig);
    file << YieldsCut("+ tot iso 0.2", cuts[ibaseline]+"&&Sum$("+os+notp+muon+iso[1]+")==1",chain, Samples, nsig);
    file << YieldsCut("+ mt<90", cuts[ibaseline]+"&&Sum$("+os+notp+muon+iso[3]+mtc[1]+")==1",chain, Samples, nsig);
    file << YieldsCut("+ dphi<1.0", cuts[ibaseline]+"&&Sum$("+os+notp+muon+iso[3]+dphi[2]+")==1",chain, Samples, nsig);
  }

  file << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \n ";
  
  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file<<" \\end{sidewaystable}"<<endl;
  file<<"\\newpage\n";
  file<<"\\begin{sidewaystable}[hp]\\centering"<<endl;
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
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";

  for(int ibaseline=0;ibaseline<2;ibaseline++){
  file << YieldsCut(cutnames[ibaseline], cuts[ibaseline],chain, Samples, nsig);
    file << YieldsCut("notp, had", cuts[ibaseline]+"&&Sum$("+taut+notp+had+")>0",chain, Samples, nsig);
    file << YieldsCut("+ os", cuts[ibaseline]+"&&Sum$("+os+notp+had+")>0",chain, Samples, nsig);
    file << YieldsCut("+ only one", cuts[ibaseline]+"&&Sum$("+os+notp+had+")==1",chain, Samples, nsig);
    file << YieldsCut("+ chg iso 0.2", cuts[ibaseline]+"&&Sum$("+os+notp+had+iso[3]+")==1",chain, Samples, nsig);
    file << YieldsCut("+ tot iso 0.2", cuts[ibaseline]+"&&Sum$("+os+notp+had+iso[1]+")==1",chain, Samples, nsig);
    file << YieldsCut("+ mt<90", cuts[ibaseline]+"&&Sum$("+os+notp+had+iso[3]+mtc[1]+")==1",chain, Samples, nsig);
    file << YieldsCut("+ dphi<1.0", cuts[ibaseline]+"&&Sum$("+os+notp+had+iso[3]+dphi[2]+")==1",chain, Samples, nsig);
  }

  file << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \n ";
  
  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
 

  
  //////////////////////////////////// Lepton veto study //////////////////////////////////
  /*file << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 6, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";
  */
  for(int ibaseline=0;ibaseline<0;ibaseline++){
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
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";
  
  //////////////////////////////////// Lepton veto study //////////////////////////////////
  file << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 6, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";


 
  file << YieldsCut(cutnames[ibaseline], cuts[ibaseline],chain, Samples, nsig);
  for(int iiso=0;iiso<4;iiso++){
    for(int icut=0;icut<3;icut++){
      file << YieldsCut(Form("elec mt: iiso icut %i %i",iiso,icut), cuts[ibaseline]+"&&Sum$("+os+notp+elec+iso[iiso]+mtc[icut]+")==1",chain, Samples, nsig);
    }
  }
  file << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \n ";
  
  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file<<" \\end{sidewaystable}"<<endl;
  file<<"\\newpage\n";
  file<<"\\begin{sidewaystable}[hp]\\centering"<<endl;
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
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";

  //////////////////////////////////// Lepton veto study //////////////////////////////////
  file << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 6, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";
    
  file << YieldsCut(cutnames[ibaseline], cuts[ibaseline],chain, Samples, nsig);
  for(int iiso=0;iiso<4;iiso++){
    for(int icut=0;icut<3;icut++){
      file << YieldsCut(Form("muon mt: iiso icut %i %i",iiso,icut), cuts[ibaseline]+"&&Sum$("+os+notp+muon+iso[iiso]+mtc[icut]+")==1",chain, Samples, nsig);
    }
  }
  file << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file<<" \\end{sidewaystable}"<<endl;
  file<<"\\newpage\n";
  file<<"\\begin{sidewaystable}[hp]\\centering"<<endl;
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
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";

  //////////////////////////////////// Lepton veto study //////////////////////////////////
  file << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 6, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";
    
  file << YieldsCut(cutnames[ibaseline], cuts[ibaseline],chain, Samples, nsig);
  for(int iiso=0;iiso<4;iiso++){
    for(int icut=0;icut<3;icut++){
      file << YieldsCut(Form("had mt: iiso icut %i %i",iiso,icut), cuts[ibaseline]+"&&Sum$("+os+notp+had+iso[iiso]+mtc[icut]+")==1",chain, Samples, nsig);
    }
  }
  file << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file<<" \\end{sidewaystable}"<<endl;
  file<<"\\newpage\n";
  file<<"\\begin{sidewaystable}[hp]\\centering"<<endl;
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
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";

  //////////////////////////////////// Lepton veto study //////////////////////////////////
  file << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 6, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";
    
  file << YieldsCut(cutnames[ibaseline], cuts[ibaseline],chain, Samples, nsig);
  for(int iiso=0;iiso<4;iiso++){
    for(int icut=0;icut<3;icut++){
      file << YieldsCut(Form("elec dphi: iiso icut %i %i",iiso,icut), cuts[ibaseline]+"&&Sum$("+os+notp+elec+iso[iiso]+dphi[icut]+")==1",chain, Samples, nsig);
    }
  }
  file << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file<<" \\end{sidewaystable}"<<endl;
  file<<"\\newpage\n";
  file<<"\\begin{sidewaystable}[hp]\\centering"<<endl;  file << "\n\\begin{tabular}{ l | ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++) file << "r";
  file<<" | r ";
  for(int sam(0); sam < nsig; sam++) file<<"| rr ";
  file<<"}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{${\\cal L} = 10$ fb$^{-1}$} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";

  //////////////////////////////////// Lepton veto study //////////////////////////////////
  file << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 6, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";
    
  file << YieldsCut(cutnames[ibaseline], cuts[ibaseline],chain, Samples, nsig);
  for(int iiso=0;iiso<4;iiso++){
    for(int icut=0;icut<3;icut++){
      file << YieldsCut(Form("muon dphi: iiso icut %i %i",iiso,icut), cuts[ibaseline]+"&&Sum$("+os+notp+muon+iso[iiso]+dphi[icut]+")==1",chain, Samples, nsig); 
    }
  }
  file << " \\hline\\multicolumn{1}{c|}{} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \n ";

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  file<<" \\end{sidewaystable}"<<endl;
  file<<"\\newpage\n";
  file<<"\\begin{sidewaystable}[hp]\\centering"<<endl;  file << "\n\\begin{tabular}{ l | ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++) file << "r";
  file<<" | r ";
  for(int sam(0); sam < nsig; sam++) file<<"| rr ";
  file<<"}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{${\\cal L} = 10$ fb$^{-1}$} ";
  for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
    file << " & "<<Samples[sam].label;
  file<< " & SM bkg. ";
  for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
    file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
  file << "\\\\ \\hline \n ";

  //////////////////////////////////// Lepton veto study //////////////////////////////////
  file << " \\multicolumn{"<< Samples.size()+1+nsig<<"}{c}{"
       << "$H_T>500, \\mathrm{MET}>200, m_T>140, n_{\\rm jets}\\geq 6, n_b\\geq 1, n_{\\rm lep}=1$"
       <<"} \\\\ \\hline\n";
    
  file << YieldsCut(cutnames[ibaseline], cuts[ibaseline],chain, Samples, nsig);
  for(int iiso=0;iiso<4;iiso++){
    for(int icut=0;icut<3;icut++){
      file << YieldsCut(Form("had dphi: iiso icut %i %i",iiso,icut), cuts[ibaseline]+"&&Sum$("+os+notp+had+iso[iiso]+dphi[icut]+")==1",chain, Samples, nsig);
    }
  }
    
      
    

// file << YieldsCut(tagcut, cuts[ibaseline]+vetocut,chain, Samples, nsig);
  
/*file << YieldsCut("$M_J > 300$", cuts[ibaseline]+"&&mj>300", chain, Samples, nsig);
  file << YieldsCut("$M_J > 300$, "+tagcut, cuts[ibaseline]+"&&mj>300&&"+vetocut,chain, Samples, nsig);

  file << YieldsCut("$MET > 400$", cuts[ibaseline]+"&&met>300", chain, Samples, nsig);
  file << YieldsCut("$MET > 400$, "+tagcut, cuts[ibaseline]+"&&met>300&&"+vetocut,chain, Samples, nsig);*/


/*file << YieldsCut("$M_J > 400$, MET$ > 400$", cuts[ibaseline]+"&&mj>400&&met>400", 
  chain, Samples, nsig);
  file << YieldsCut("$M_J > 400$, MET$ > 400$, "+tagcut, cuts[ibaseline]+"&&mj>400&&met>400&&"+vetocut, 
  chain, Samples, nsig);
  file << YieldsCut("$M_J > 400$, MET$ > 400$, $n_{\\rm jets} \\geq 8$", cuts[ibaseline]+"&&mj>400&&met>400&&njets>=8", 
  chain, Samples, nsig);
  file << YieldsCut("$M_J > 400$, MET$ > 400$, $n_{\\rm jets} \\geq 8$, "+tagcut, cuts[ibaseline]+"&&mj>400&&met>400&&njets>=8&&"+vetocut, 
  chain, Samples, nsig);
  file << YieldsCut("$M_J > 600$, MET$ > 400$", cuts[ibaseline]+"&&mj>600&&met>400", 
  chain, Samples, nsig);
  file << YieldsCut("$M_J > 600$, MET$ > 400$, "+tagcut, cuts[ibaseline]+"&&mj>600&&met>400&&"+vetocut, 
  chain, Samples, nsig);*/




file << " \\hline\\multicolumn{1}{c|}{} ";
for(unsigned sam(0); sam < Samples.size()-nsig; sam++)
  file << " & "<<Samples[sam].label;
file<< " & SM bkg. ";
for(unsigned sam(Samples.size()-nsig); sam < Samples.size(); sam++)
  file << " & "<<Samples[sam].label<< " & $Z_{\\rm bi}$ ";
file << "\\\\ \n ";

file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;

  }
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
    out += (" \t& $" + RoundNumber(yield[sam],1)+" \\pm "+RoundNumber(error[sam],1) + "$ \t& " +
	    //RoundNumber(yield[sam],2,bkg));
	    RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2));
    cout<<", S = "<<RoundNumber(yield[sam],1)+" +- "+RoundNumber(error[sam],1)<<" with Zbi = ";
    cout<<RoundNumber(RooStats::NumberCountingUtils::BinomialExpZ(yield[sam], bkg, fracerr),2);
  }
  out += " \\\\ \n";
  cout<<endl;
  return out;
}
