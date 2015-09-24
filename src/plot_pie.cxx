// plot_1d: Macro that plots variables both lumi weighted and normalized to the same area.
#include "plot_pie.hpp"

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPie.h"
#include "TLegend.h"
#include "TString.h"
#include "TColor.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace {
  TString ntuple_date("2015_05_25");
  TString plot_type=".eps";
  TString plot_style="RA4";
}

using namespace std;

int main(){
  styles style(plot_style);
  style.setDefaultStyle();

  TString folder="/cms5r0/ald77/archive/"+ntuple_date+"/skim/";
  //TString folder="archive/current/";
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*WJetsToLNu_HT*");
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

  // Reading ntuples
  vector<TChain*> chains;
  vector<sfeats> samples;
  samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", 1006,1,"ntruleps>=2"));//0
  samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", 1000,1,"ntruleps<=1"));//1
  samples.push_back(sfeats(s_ttv, "ttV", 1002));//2
  samples.push_back(sfeats(s_single, "Single top", 1005));//3
  samples.push_back(sfeats(s_wjets, "W+jets", 1004));//4
  samples.push_back(sfeats(s_other, "Other", 1001));//5
  samples.push_back(sfeats(s_t1t, "T1tttt(1500,100)", 2));//6
  samples.push_back(sfeats(s_t1tc, "T1tttt(1200,800)", 2,2));//7

  samples.push_back(sfeats(s_tt, "0 leps", kGreen-4, 1, "ntruleps==0"));//8
  samples.push_back(sfeats(s_tt, "1 #tau_{had}", kRed-4, 1, "ntruleps==1 && ntrutaush==1"));//9
  samples.push_back(sfeats(s_tt, "1 e/#mu", kBlue-4, 1, "ntruleps==1 && (ntruels+ntrumus)==1"));//10
  samples.push_back(sfeats(s_tt, "2 #tau_{had}", kRed+2, 1, "ntruleps==2 && ntrutaush==2"));//11
  samples.push_back(sfeats(s_tt, "1 e/#mu, 1 #tau_{had}", kMagenta+2, 1, "ntruleps==2 && (ntruels+ntrumus)==1 && ntrutaush==1"));//12
  samples.push_back(sfeats(s_tt, "2 e/#mu", kBlue+2, 1, "ntruleps==2 && (ntruels+ntrumus)==2"));//13
  samples.push_back(sfeats(s_tt, "3+ leps", kGreen+2, 1, "ntruleps>=3"));//14

  samples.push_back(sfeats(s_tt, "0 leps", kGreen-4, 1, "ntruleps==0&&met>100&&met<=200"));//15
  samples.push_back(sfeats(s_tt, "1 #tau_{had}", kRed-4, 1, "ntruleps==1 && ntrutaush==1&&met>100&&met<=200"));//16
  samples.push_back(sfeats(s_tt, "1 e/#mu", kBlue-4, 1, "ntruleps==1 && (ntruels+ntrumus)==1&&met>100&&met<=200"));//17
  samples.push_back(sfeats(s_tt, "2 #tau_{had}", kRed+2, 1, "ntruleps==2 && ntrutaush==2&&met>100&&met<=200"));//18
  samples.push_back(sfeats(s_tt, "1 e/#mu, 1 #tau_{had}", kMagenta+2, 1, "ntruleps==2 && (ntruels+ntrumus)==1 && ntrutaush==1&&met>100&&met<=200"));//19
  samples.push_back(sfeats(s_tt, "2 e/#mu", kBlue+2, 1, "ntruleps==2 && (ntruels+ntrumus)==2&&met>100&&met<=200"));//20
  samples.push_back(sfeats(s_tt, "2 #tau_{lep}", kOrange+3, 1, "ntruleps==2 && ntrutaush==1"));//21
  samples.push_back(sfeats(s_tt, "1 e/#mu, 1 #tau_{lep}", kBlack, 1, "ntruleps==2 && (ntruels+ntrumus)==1 && ntrutausl==1"));//22
  samples.push_back(sfeats(s_tt, "1 #tau_{had}, 1 #tau_{lep}", kYellow, 1, "ntruleps==2 && ntrutaush==1 && ntrutausl==1"));//23
  samples.push_back(sfeats(s_tt, "3+ leps", kGreen+2, 1, "ntruleps>=3"));//24

  samples.push_back(sfeats(s_tt, "0 leps", kGreen-4, 1, "ntruleps==0&&met>200&&met<=400"));//25
  samples.push_back(sfeats(s_tt, "1 #tau_{had}", kRed-4, 1, "ntruleps==1 && ntrutaush==1&&met>200&&met<=400"));//26
  samples.push_back(sfeats(s_tt, "1 e/#mu", kBlue-4, 1, "ntruleps==1 && (ntruels+ntrumus)==1&&met>200&&met<=400"));//27
  samples.push_back(sfeats(s_tt, "2 #tau_{had}", kRed+2, 1, "ntruleps==2 && ntrutaush==2&&met>200&&met<=400"));//28
  samples.push_back(sfeats(s_tt, "1 e/#mu, 1 #tau_{had}", kMagenta+2, 1, "ntruleps==2 && (ntruels+ntrumus)==1 && ntrutaush==1&&met>200&&met<=400"));//29
  samples.push_back(sfeats(s_tt, "2 e/#mu", kBlue+2, 1, "ntruleps==2 && (ntruels+ntrumus)==2&&met>200&&met<=400"));//30
  samples.push_back(sfeats(s_tt, "2 #tau_{lep}", kOrange+3, 1, "ntruleps==2 && ntrutaush==1"));//31
  samples.push_back(sfeats(s_tt, "1 e/#mu, 1 #tau_{lep}", kBlack, 1, "ntruleps==2 && (ntruels+ntrumus)==1 && ntrutausl==1"));//32
  samples.push_back(sfeats(s_tt, "1 #tau_{had}, 1 #tau_{lep}", kYellow, 1, "ntruleps==2 && ntrutaush==1 && ntrutausl==1"));//33
  samples.push_back(sfeats(s_tt, "3+ leps", kGreen+2, 1, "ntruleps>=3&&met>200&&met<=400"));//34

  samples.push_back(sfeats(s_tt, "0 leps", kGreen-4, 1, "ntruleps==0&&met>400"));//35
  samples.push_back(sfeats(s_tt, "1 #tau_{had}", kRed-4, 1, "ntruleps==1 && ntrutaush==1&&met>400"));//36
  samples.push_back(sfeats(s_tt, "1 e/#mu", kBlue-4, 1, "ntruleps==1 && (ntruels+ntrumus)==1&&met>400"));//37
  samples.push_back(sfeats(s_tt, "2 #tau_{had}", kRed+2, 1, "ntruleps==2 && ntrutaush==2&&met>400"));//38
  samples.push_back(sfeats(s_tt, "1 e/#mu, 1 #tau_{had}", kMagenta+2, 1, "ntruleps==2 && (ntruels+ntrumus)==1 && ntrutaush==1&&met>400"));//39
  samples.push_back(sfeats(s_tt, "2 e/#mu", kBlue+2, 1, "ntruleps==2 && (ntruels+ntrumus)==2&&met>400"));//40
  samples.push_back(sfeats(s_tt, "2 #tau_{lep}", kOrange+3, 1, "ntruleps==2 && ntrutaush==1"));//41
  samples.push_back(sfeats(s_tt, "1 e/#mu, 1 #tau_{lep}", kBlack, 1, "ntruleps==2 && (ntruels+ntrumus)==1 && ntrutausl==1"));//42
  samples.push_back(sfeats(s_tt, "1 #tau_{had}, 1 #tau_{lep}", kYellow, 1, "ntruleps==2 && ntrutaush==1 && ntrutausl==1"));//43
  samples.push_back(sfeats(s_tt, "3+ leps", kGreen+2, 1, "ntruleps>=3&&met>400"));//44

  for(unsigned sam(0); sam < samples.size(); sam++){
    chains.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < samples[sam].file.size(); insam++)
      chains.at(sam)->Add(samples[sam].file[insam]);
  }

  vector<int> bkgs;
  bkgs.push_back(0);
  bkgs.push_back(1);
  bkgs.push_back(2);
  bkgs.push_back(3);
  bkgs.push_back(4);
  bkgs.push_back(5);

  vector<int> with1500 = bkgs;
  with1500.push_back(6);

  vector<int> with1200 = bkgs;
  with1200.push_back(7);

  vector<int> leps_lmet;
  //  leps_lmet.push_back(15);
  //leps_lmet.push_back(16);
  //  leps_lmet.push_back(17);
  leps_lmet.push_back(18);
  leps_lmet.push_back(19);
  leps_lmet.push_back(20);
  leps_lmet.push_back(21);
  leps_lmet.push_back(22);
  leps_lmet.push_back(23);
  leps_lmet.push_back(24);
  vector<int> leps_mmet;
  //  leps_mmet.push_back(25);
  //  leps_mmet.push_back(26);
  //  leps_mmet.push_back(27);
  leps_mmet.push_back(28);
  leps_mmet.push_back(29);
  leps_mmet.push_back(30);
  leps_mmet.push_back(31);
  leps_mmet.push_back(32);
  leps_mmet.push_back(33);
  leps_mmet.push_back(34);
  vector<int> leps_hmet;
  //  leps_hmet.push_back(35);
  //  leps_hmet.push_back(36);
  //  leps_hmet.push_back(37);
  leps_hmet.push_back(38);
  leps_hmet.push_back(39);
  leps_hmet.push_back(40);
  leps_hmet.push_back(41);
  leps_hmet.push_back(42);
  leps_hmet.push_back(43);
  leps_hmet.push_back(44);

  vector<pfeats> pies;
  ///////////////////////////////////////////////////////////////
  // pies.push_back(pfeats(bkgs, "ht>500&&met>200&&njets>=6&&nbm>=2&&mt>125&&mj>400", "comp"));
  //  pies.push_back(pfeats(bkgs, "ht>500&&met>200&&njets>=6&&nbm>=2&&mt>125&&mj<=400", "comp"));
  //  pies.push_back(pfeats(bkgs, "ht>500&&met>200&&njets>=6&&nbm>=2&&mt<=125&&mj>400", "comp"));
  //  pies.push_back(pfeats(bkgs, "ht>500&&met>200&&njets>=6&&nbm>=2&&mt<=125&&mj<=400", "comp"));

  pies.push_back(pfeats(leps_lmet, "ht>500&&met>100&&njets>=7&&nbm>=2&&(nels+nmus)==1", "leps_lmet"));
  pies.push_back(pfeats(leps_mmet, "ht>500&&met>100&&njets>=7&&nbm>=2&&(nels+nmus)==1", "leps_mmet"));
  pies.push_back(pfeats(leps_hmet, "ht>500&&met>100&&njets>=7&&nbm>=2&&(nels+nmus)==1", "leps_hmet"));
  ///////////////////////////////////////////////////////////////

  for(size_t pie = 0; pie < pies.size(); ++pie){
    DrawPie(pies.at(pie), samples, chains);
  }

  for(size_t ichain = 0; ichain < chains.size(); ++ichain){
    if(chains.at(ichain) != NULL){
      delete chains.at(ichain);
      chains.at(ichain) = NULL;
    }
  }
}

void DrawPie(const pfeats &pfeat, const vector<sfeats> &samples, const vector<TChain*> &chains){
  vector<double> counts(pfeat.samples.size());
  vector<int> colors(pfeat.samples.size());
  vector<const char*> labels(pfeat.samples.size());
  vector<TH1D> dummy_histos(pfeat.samples.size(), TH1D("","",1,-1.,1.));
  TLegend l(0., 0., 1., 1.);
  for(size_t iisample = 0; iisample < pfeat.samples.size(); ++iisample){
    size_t isample = pfeat.samples.at(iisample);
    const sfeats &sample = samples.at(isample);
    TChain &chain = *chains.at(isample);
    TString cut = "3.0*weight*("+pfeat.cut+"&&"+sample.cut+")";
    double uncert;
    get_count_and_uncertainty(chain, cut.Data(), counts.at(iisample), uncert);
    colors.at(iisample) = sample.color;
    labels.at(iisample) = sample.label;
    dummy_histos.at(iisample).SetFillColor(sample.color);
    l.AddEntry(&dummy_histos.at(iisample), sample.label, "f");
  }

  TString name = FormatName(pfeat.tagname+"_"+pfeat.cut);

  TCanvas c("", "", 512, 512);
  c.SetFillColorAlpha(0, 0.);
  c.SetFillStyle(4000);
  l.Draw();
  c.Print("plots/pies/legen_"+name+plot_type);
  TPie pie("", "", pfeat.samples.size(), &counts.at(0), &colors.at(0), &labels.at(0));
  pie.SetCircle(0.5, 0.5, 0.3);
  pie.Draw();
  c.Print("plots/pies/label_"+name+plot_type);
  pie.SetLabelFormat("%val");
  pie.SetValueFormat("%0.1f");
  pie.Draw();
  c.Print("plots/pies/count_"+name+plot_type);
  pie.SetLabelFormat("");
  pie.SetCircle(0.5, 0.5, 0.49);
  pie.Draw();
  pie.SetLabelFormat("%perc");
  pie.SetValueFormat("%0.1f");
  pie.Draw();
  c.Print("plots/pies/perc_"+name+plot_type);
  c.Print("plots/pies/plain_"+name+plot_type);
}

TString FormatName(TString name){
  name.ReplaceAll("(","");
  name.ReplaceAll(")","");
  name.ReplaceAll("[","");
  name.ReplaceAll("]","");
  name.ReplaceAll("{","");
  name.ReplaceAll("}","");
  name.ReplaceAll("#","");
  name.ReplaceAll(",","_");
  name.ReplaceAll(" ","_");
  name.ReplaceAll("\\","_");
  name.ReplaceAll(">=","geq");
  name.ReplaceAll(">","gt");
  name.ReplaceAll("<=","leq");
  name.ReplaceAll("<","lt");
  name.ReplaceAll("==","eq");
  name.ReplaceAll("!=","neq");
  name.ReplaceAll("!","n");
  name.ReplaceAll("&&","_");
  name.ReplaceAll("||","_");
  name.ReplaceAll("&","band");
  name.ReplaceAll("|","bor");
  name.ReplaceAll("+","plus");
  name.ReplaceAll("-","minus");
  name.ReplaceAll("*","times");
  name.ReplaceAll("/","divby");
  name.ReplaceAll("$","");
  name.ReplaceAll("__","_");
  return name;
}
