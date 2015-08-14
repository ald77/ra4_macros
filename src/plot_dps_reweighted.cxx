// plot_dps: Macro that plots variables both for comissioning DPS

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TColor.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace {
  TString minjets("5");
  TString mjthresh("400");
  TString metcut("met_nohf>150");
  TString minht("400");
  TString minbm("1");
  TString luminosity="0.0419"; // in ifb
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
}

using namespace std;
using std::cout;
using std::endl;

int main(){ 
  TString folder_ns="/net/cms2/cms2r0/ald77/archive/2015_07_22/";
  TString folder_nsdata="/net/cms2/cms2r0/ald77/archive/2015_07_26/";
  TString folder_1l = "/net/cms2/cms2r0/ald77/archive/2015_07_22/skim_1lht400/";
  TString folder_1ldata = "/net/cms2/cms2r0/ald77/archive/2015_07_26/skim_1lht400/";
  folder_1l = "/net/cms2/cms2r0/ald77/archive/2015_08_13/skim_1lht400/";
  folder_1ldata = "/net/cms2/cms2r0/ald77/archive/2015_08_13/skim_1lht400/";
  TString folder_2l="/net/cms2/cms2r0/ald77/archive/2015_07_22/skim_2l/";
  TString folder_2ldata="/net/cms2/cms2r0/ald77/archive/2015_07_26/skim_2l/";
  folder_2l = "/net/cms2/cms2r0/ald77/archive/2015_08_13/skim_2l/";
  folder_2ldata = "/net/cms2/cms2r0/ald77/archive/2015_08_13/skim_2l/";

  folder_1l = "/net/cms29/cms29r0/heller/susy_cfa/out/PU/";
  folder_2l = "/net/cms29/cms29r0/heller/susy_cfa/out/PU/";
  folder_ns = folder_1l;

  vector<TString> s_trig_htmht;
  s_trig_htmht.push_back(folder_1ldata+"*HTMHT*");
  vector<TString> s_trig_dl;
  s_trig_dl.push_back(folder_2ldata+"/*DoubleMu*");
  s_trig_dl.push_back(folder_2ldata+"/*DoubleE*");
  vector<TString> s_trig_dmu;
  s_trig_dmu.push_back(folder_2ldata+"/*DoubleMu*");
  vector<TString> s_trig_del;
  s_trig_del.push_back(folder_2ldata+"/*DoubleE*");


  vector<TString> s_t1t;
  //s_t1t.push_back(folder_1l+"*T1tttt*1500_*PU20*");
  s_t1t.push_back("/net/cms2/cms2r0/ald77/archive/2015_07_22/skim_1lht400/*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  //s_t1tc.push_back(folder_1l+"*T1tttt*1200_*PU20*");
  s_t1tc.push_back("/net/cms2/cms2r0/ald77/archive/2015_07_22/skim_1lht400/*T1tttt*1200_*PU20*");
  vector<TString> s_t1t_ns;
  s_t1t_ns.push_back(folder_ns+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc_ns;
  s_t1tc_ns.push_back(folder_ns+"*T1tttt*1200_*PU20*");
  vector<TString> s_tt;
  s_tt.push_back(folder_1l+"*_TTJet*50ns*");
  vector<TString> s_tt_ns;
  s_tt_ns.push_back(folder_2l+"*_TTJet*50ns*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder_1l+"*_WJets*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder_1l+"*ST_*");
  vector<TString> s_DY;
  s_DY.push_back(folder_2l+"*DY*");
  vector<TString> s_qcd;
  s_qcd.push_back(folder_1l+"*QCD_Pt*");
  vector<TString> s_ttv; //negligible
  s_ttv.push_back(folder_2l+"*TTW*"); 
  s_ttv.push_back(folder_2l+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder_1l+"*_ZJet*");
  s_other.push_back(folder_1l+"*DY*");
  s_other.push_back(folder_1l+"*WH_HToBB*");
  //s_other.push_back(folder_1l+"*_TTJet*25ns*");
  s_other.push_back(folder_2l+"*TTW*"); 
  s_other.push_back(folder_2l+"*TTZ*");
  vector<TString> s_qcdDY;
  s_qcdDY.push_back(folder_2l+"*QCD_Pt*");
  vector<TString> s_otherDY;
  s_otherDY.push_back(folder_2l+"*_ZJet*");
  s_otherDY.push_back(folder_2l+"*WH_HToBB*");
  s_otherDY.push_back(folder_2l+"*ST_*");
  s_otherDY.push_back(folder_2l+"*_WJets*");

  // Reading ntuples
  vector<sfeats> Samples; 
  TString datatitle = "Data";
  Samples.push_back(sfeats(s_trig_htmht, datatitle,kBlack,1,"trig[0]&&json_golden")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2"));
  Samples.push_back(sfeats(s_wjets, "W+jets", dps::c_wjets,1));
  Samples.push_back(sfeats(s_singlet, "Single top", dps::c_singlet));
  //Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_qcd, "QCD", dps::c_qcd));
  // Some of Other has leptons, but very little, and this is the easiest to put had tt with QCD
  //Samples.push_back(sfeats(s_other, "Other", dps::c_other, 1,"ntruleps==0")); 
  Samples.push_back(sfeats(s_other, "Other", dps::c_other, 1)); 

  vector<int> ra4_sam, ra4_sam_ns;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
    ra4_sam_ns.push_back(nsam+sam);
    vector<TString> sam_files = Samples[sam].file;
    for(unsigned ifile(0); ifile < sam_files.size(); ifile++)
      if(Samples[sam].isData) sam_files[ifile].ReplaceAll(folder_1ldata, folder_nsdata);
      else sam_files[ifile].ReplaceAll(folder_1l, folder_ns);
    Samples.push_back(sfeats(sam_files, Samples[sam].label, Samples[sam].color, Samples[sam].style,
           Samples[sam].cut));
    Samples.back().isData = Samples[sam].isData;
  } // Loop over samples

  int imj(Samples.size());
  Samples.push_back(sfeats(s_trig_htmht, "Data 2l, N_{jets} #geq 4",kBlack,1,
			   "trig[0]&&njets>=4&&(nvmus+nvels)==2&&json_golden")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_htmht, "Data 1l, N_{jets} #geq 5, m_{T}#leq140, n_{b}#geq1",dps::c_tt_1l,1,
			   "trig[0]&&njets>=5&&(nvmus+nvels)==1&&mt<=140&&nbm>=1&&json_golden")); //Samples.back().isData = true;
  vector<int> mj_sam;
  mj_sam.push_back(imj);
  mj_sam.push_back(imj+1);

  int idl(Samples.size());
  Samples.push_back(sfeats(s_trig_dl, "Data",kBlack,1,
			   "(trig[10]||trig[9])&&json_golden")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_DY, "Z+jets", dps::c_qcd));//12
  Samples.push_back(sfeats(s_tt_ns, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2"));
  Samples.push_back(sfeats(s_tt_ns, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps==1"));
  Samples.push_back(sfeats(s_qcdDY, "QCD", dps::c_singlet));
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
  Samples.push_back(sfeats(s_otherDY, "Other", dps::c_other)); 
  vector<int> dl_sam;
  for(unsigned sam(idl); sam < Samples.size(); sam++) {
    dl_sam.push_back(sam);
  }
  vector<int> dmu_sam(dl_sam), del_sam(dl_sam);
  Samples.push_back(sfeats(s_trig_dmu, "Data",kBlack,1,
			   "(trig[9])&&json_golden")); Samples.back().isData = true;
  Samples.push_back(sfeats(s_trig_del, "Data",kBlack,1,
			   "(trig[10])&&json_golden")); Samples.back().isData = true;
  dmu_sam[0] = Samples.size()-2;
  del_sam[0] = Samples.size()-1;

  // Defining samples with signal
  TString factor_c("100"), factor_nc("300");
  int isig(Samples.size());
  Samples.push_back(sfeats(s_t1t, "Sig NC #times "+factor_nc, kRed+2)); Samples.back().factor = factor_nc;
  Samples.push_back(sfeats(s_t1tc, "Sig C #times "+factor_c, kRed+2,2)); Samples.back().factor = factor_c;
  vector<int> sig_sam;
  sig_sam.push_back(ra4_sam[0]);
  sig_sam.push_back(isig);
  sig_sam.push_back(isig+1);
  for(unsigned ind(1); ind<ra4_sam.size(); ind++)
    sig_sam.push_back(ra4_sam[ind]);

  // Defining samples with signal, no skim, for log
  factor_c = "20"; factor_nc = "20";
  isig = Samples.size();
  Samples.push_back(sfeats(s_t1t_ns, "Sig NC #times "+factor_nc, kRed+2)); Samples.back().factor = factor_nc;
  Samples.push_back(sfeats(s_t1tc_ns, "Sig C #times "+factor_c, kRed+2,2)); Samples.back().factor = factor_c;
  vector<int> sig_sam_ns;
  sig_sam_ns.push_back(ra4_sam_ns[0]);
  sig_sam_ns.push_back(isig);
  sig_sam_ns.push_back(isig+1);
  for(unsigned ind(1); ind<ra4_sam.size(); ind++)
    sig_sam_ns.push_back(ra4_sam_ns[ind]);

  // Defining samples with signal, for log
  isig = Samples.size();
  Samples.push_back(sfeats(s_t1t, "Sig NC #times "+factor_nc, kRed+2)); Samples.back().factor = factor_nc;
  Samples.push_back(sfeats(s_t1tc, "Sig C #times "+factor_c, kRed+2,2)); Samples.back().factor = factor_c;
  vector<int> sig_sam_log;
  sig_sam_log.push_back(ra4_sam[0]);
  sig_sam_log.push_back(isig);
  sig_sam_log.push_back(isig+1);
  for(unsigned ind(1); ind<ra4_sam.size(); ind++)
    sig_sam_log.push_back(ra4_sam[ind]);

  vector<hfeats> vars;
   TString mll("(mumuv_m*(mumuv_m>0)+elelv_m*(elelv_m>0))>80&&(mumuv_m*(mumuv_m>0)+elelv_m*(elelv_m>0))<100");

  vars.push_back(hfeats("npv",10,0,40, ra4_sam, "NPV",
  			"onht>350&&pass&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+
  			"&&"+metcut+"&&njets>=4&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true; vars.back().PU_reweight = true;

  vars.push_back(hfeats("npv",10,0,40, dl_sam, "NPV",
   			"onht>300&&pass&&ht>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2"; vars.back().normalize = true; vars.back().PU_reweight = true;

  vars.push_back(hfeats("npv",10,0,40, ra4_sam, "NPV",
  			"pass&&onht>350&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+
  			"&&"+metcut+"&&njets>=4&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  vars.push_back(hfeats("npv",10,0,40, dl_sam, "NPV",
   			"pass&&onht>300&&ht>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2"; vars.back().normalize = true;


  /////////////////////////// MJ plots for the DPS////////////////////////////
  vars.push_back(hfeats("fjets_m[0]",24,0,480, ra4_sam, "m(J_{1}) [GeV]",
  			"pass&&onht>350&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+
  			"&&"+metcut+"&&njets>=4&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

  vars.push_back(hfeats("mj",30,0,600, ra4_sam, "m_{J} [GeV]",
  			"pass&&onht>350&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+
  			"&&"+metcut+"&&njets>=4&&nbm>=1"));
  vars.back().whichPlots = "2";  vars.back().normalize = true;

 
  vars.push_back(hfeats("fjets_m[0]",12,0,480, dl_sam, "m(J_{1}) [GeV]",
   			"pass&&onht>300&&ht>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("mj",15,0,600, dl_sam, "m_{J} [GeV]",
   			"pass&&onht>300&&ht>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  vars.back().whichPlots = "2"; vars.back().normalize = true;


vars.push_back(hfeats("fjets_m[0]",24,0,480, ra4_sam, "m(J_{1}) [GeV]",
  			"onht>350&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+
  			"&&"+metcut+"&&njets>=4&&nbm>=1&&pass"));
 vars.back().whichPlots = "2";  vars.back().normalize = true; vars.back().PU_reweight = true;

  vars.push_back(hfeats("mj",30,0,600, ra4_sam, "m_{J} [GeV]",
  			"onht>350&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+
  			"&&"+metcut+"&&njets>=4&&nbm>=1&&pass"));
  vars.back().whichPlots = "2";  vars.back().normalize = true; vars.back().PU_reweight = true;

  
  vars.push_back(hfeats("fjets_m[0]",12,0,480, dl_sam, "m(J_{1}) [GeV]",
   			"onht>300&&ht>350&&(nvmus>=2||nvels>=2)&&njets>=4&&pass&&"+mll));
  vars.back().whichPlots = "2"; vars.back().normalize = true; vars.back().PU_reweight = true;

  vars.push_back(hfeats("mj",15,0,600, dl_sam, "m_{J} [GeV]",
   			"onht>300&&ht>350&&(nvmus>=2||nvels>=2)&&njets>=4&&pass&&"+mll));
  vars.back().whichPlots = "2"; vars.back().normalize = true; vars.back().PU_reweight = true;

  /////////////////////////// N-1 plots for the DPS////////////////////////////

  vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "pass&&onht>350&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("met_nohf",16,0,800, ra4_sam, "MET^{no HF} [GeV]",
          "pass&&onht>350&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true;

  vars.push_back(hfeats("njets",14,-0.5,13.5, ra4_sam, "Number of jets",
          "pass&&onht>350&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+"&&"+metcut+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true;

  vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Number of b-tags (CSVM)",
          "pass&&onht>350&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets));
  vars.back().whichPlots = "2"; vars.back().normalize = true;


vars.push_back(hfeats("mt",18,0,360, ra4_sam, "m_{T} [GeV]",
          "onht>350&&pass&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true; vars.back().PU_reweight = true;

  vars.push_back(hfeats("met_nohf",16,0,800, ra4_sam, "MET^{no HF} [GeV]",
          "onht>350&&pass&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+"&&njets>="+minjets+"&&nbm>="+minbm));
  vars.back().whichPlots = "1"; vars.back().normalize = true; vars.back().PU_reweight = true;

  vars.push_back(hfeats("njets",14,-0.5,13.5, ra4_sam, "Number of jets",
          "onht>350&&pass&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+"&&"+metcut+"&&nbm>="+minbm));
  vars.back().whichPlots = "2"; vars.back().normalize = true; vars.back().PU_reweight = true;

  vars.push_back(hfeats("nbm",7,-0.5,6.5, ra4_sam, "Number of b-tags (CSVM)",
          "onht>350&&pass&&onmet>100&&(nvmus+nvels)==1&&ht>"+minht+"&&"+metcut+"&&njets>="+minjets));
  vars.back().whichPlots = "2"; vars.back().normalize = true; vars.back().PU_reweight = true;


  // // ////////////////////////////// MJ plots for the DPS ////////////////////////////
  // vars.push_back(hfeats("mj",15,0,600, ra4_sam, "M_{J} [GeV]",
  //  			"pass&&onht>350&&onmet>100&&ht>400&&met>150&nbm>=1&&njets>=5&&(nvmus+nvels)==1&&mt<=140"));
  // vars.back().whichPlots = "2"; vars.back().normalize = true;

  // vars.push_back(hfeats("mj",15,0,600, ra4_sam, "M_{J} [GeV]",
  //  			"pass&&onht>350&&onmet>100&&ht>400&&met>150&&njets>=4&&(nvmus+nvels)==2"));
  // vars.back().whichPlots = "2"; vars.back().normalize = true;

  // vars.push_back(hfeats("mj",15,0,600, mj_sam, "M_{J} [GeV]",
  //  			"pass&ht>400&&met>150"));
  // vars.back().whichPlots = "2"; vars.back().normalize = true; //vars.back().maxRatio = 2.2;

  // TString mll("(mumuv_m*(mumuv_m>0)+elelv_m*(elelv_m>0))>80&&(mumuv_m*(mumuv_m>0)+elelv_m*(elelv_m>0))<100");
  // vars.push_back(hfeats("mj",15,0,600, dl_sam, "M_{J} [GeV]",
  //  			"ht>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  // vars.back().whichPlots = "2"; // vars.back().normalize = true;


  //////////////////////////// DY plots for Ryan's talk ////////////////////////////

  // // Invariant masses
  // vars.push_back(hfeats("mumuv_m",32,20,180, dmu_sam, "m_{#mu#mu} [GeV]",
  //  			"ht>350&&nvmus>=2&&njets<=3"));
  // vars.back().whichPlots = "2"; //vars.back().normalize = true;
  // vars.push_back(hfeats("elelv_m",32,20,180, del_sam, "m_{ee} [GeV]",
  //  			"ht>350&&nvels>=2&&njets<=3"));
  // vars.back().whichPlots = "2"; //vars.back().normalize = true;
  // vars.push_back(hfeats("mumuv_m*(mumuv_m>0)+elelv_m*(elelv_m>0)",32,20,180, dl_sam, "m_{ll} [GeV]",
  //  			"ht>350&&(nvmus>=2||nvels>=2)&&njets<=3"));
  // vars.back().whichPlots = "2"; //vars.back().normalize = true;


  // vars.push_back(hfeats("mumuv_m",32,20,180, dmu_sam, "m_{#mu#mu} [GeV]",
  //  			"ht>350&&nvmus>=2&&njets>=4"));
  // vars.back().whichPlots = "2"; //vars.back().normalize = true;
  // vars.push_back(hfeats("elelv_m",32,20,180, del_sam, "m_{ee} [GeV]",
  //  			"ht>350&&nvels>=2&&njets>=4"));
  // vars.back().whichPlots = "2"; //vars.back().normalize = true;
  // vars.push_back(hfeats("mumuv_m*(mumuv_m>0)+elelv_m*(elelv_m>0)",32,20,180, dl_sam, "m_{ll} [GeV]",
  //  			"ht>350&&(nvmus>=2||nvels>=2)&&njets>=4"));
  // vars.back().whichPlots = "2"; //vars.back().normalize = true;


  // // Kinematic distributions
  // vars.push_back(hfeats("ht",20,0,1000, dl_sam, "H_{T}^{HLT} [GeV]",
  //  			"(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  // vars.back().whichPlots = "2"; // vars.back().normalize = true;
  // vars.push_back(hfeats("njets",9,-0.5,8.5, dl_sam, "N_{jets} [GeV]",
  //  			"ht>350&&(nvmus>=2||nvels>=2)&&"+mll));
  // vars.back().whichPlots = "2"; // vars.back().normalize = true;
  // vars.push_back(hfeats("fjets_m[0]",20,0,500, dl_sam, "Most massive fat jet mass [GeV]",
  //  			"ht>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  // vars.back().whichPlots = "2"; // vars.back().normalize = true;
  // vars.push_back(hfeats("nfjets",10,-0.5,9.5, dl_sam, "N_{fat jets}",
  //  			"ht>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  // vars.back().whichPlots = "2"; // vars.back().normalize = true;


  // // MJ distributions
  // vars.push_back(hfeats("mj",20,0,600, dl_sam, "M_{J} [GeV]",
  //  			"ht>350&&(nvmus>=2||nvels>=2)&&njets<=3&&"+mll));
  // vars.back().whichPlots = "2"; // vars.back().normalize = true;
  // vars.push_back(hfeats("mj",20,0,600, dl_sam, "M_{J} [GeV]",
  //  			"ht>350&&(nvmus>=2||nvels>=2)&&njets>=4&&"+mll));
  // vars.back().whichPlots = "2"; // vars.back().normalize = true;


  // ////////////////////////////// ttbar plots for Ryan's talk ////////////////////////////
  // vars.push_back(hfeats("mj",15,0,600, ra4_sam, "M_{J} [GeV]",
  //  			"onht>350&&onmet>100&&ht>400&&met>150&nbm>=1&&njets>=5&&(nvmus+nvels)==1&&mt<=140"));
  // vars.back().whichPlots = "2"; vars.back().normalize = true;

  // vars.push_back(hfeats("mj",15,0,600, ra4_sam, "M_{J} [GeV]",
  //  			"onht>350&&onmet>100&&ht>400&&met>150&&njets>=4&&(nvmus+nvels)==2"));
  // vars.back().whichPlots = "2"; vars.back().normalize = true;

  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "1d/PU",true);


}

