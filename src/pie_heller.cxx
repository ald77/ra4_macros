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

using namespace std;

int main(){
  styles style("Standard");
  style.setDefaultStyle();

  /*  TColor ucsb_blue(1000, 1/255.,57/255.,166/255.);
  TColor ucsb_gold(1001, 255/255.,200/255.,47/255);
  TColor penn_red(1002, 149/255.,0/255.,26/255.);
  TColor uf_orange(1003, 255/255.,74/255.,0/255.);
  TColor uo_green(1004, 0/255.,79/255.,39/255.);
  TColor tcu_purple(1005, 52/255.,42/255.,123/255.);
  TColor tar_heel_blue(1006, 86/255.,160/255.,211/255.);
  TColor sig_teal(1007, 96/255.,159/255.,128/255.);
  TColor sig_gold(1008, 215/255.,162/255.,50/255.);
  TColor seal_brown(1010, 89/255.,38/255.,11/255.);
  */
  TString folder="archive/current/skim/";
  //TString folder="archive/current/";
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*WJets*");
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
  samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", 1006,1,"((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)>=2"));//0
  samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", 1000,1,"((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)<=1"));//1
  samples.push_back(sfeats(s_ttv, "ttV", 1002));//2
  samples.push_back(sfeats(s_single, "Single top", 1005));//3
  samples.push_back(sfeats(s_wjets, "W+jets", 1004));//4
  samples.push_back(sfeats(s_other, "Other", 1001));//5
  samples.push_back(sfeats(s_t1t, "T1tttt(1500,100)", 2));//6
  samples.push_back(sfeats(s_t1tc, "T1tttt(1200,800)", 2,2));//7

  samples.push_back(sfeats(s_tt, "0 leps", kGreen-4, 1, "((mc_type&0x0F00)/0x100)==0 && ((mc_type&0x000F)-((mc_type&0x00F0)/0x10))==0"));//8
  samples.push_back(sfeats(s_tt, "1 #tau_{had}", kRed-4, 1, "((mc_type&0x0F00)/0x100)==0 && ((mc_type&0x000F)-((mc_type&0x00F0)/0x10))==1"));//9
  samples.push_back(sfeats(s_tt, "1 e/#mu", kBlue-4, 1, "((mc_type&0x0F00)/0x100)==1 && ((mc_type&0x000F)-((mc_type&0x00F0)/0x10))==0"));//10
  samples.push_back(sfeats(s_tt, "2 #tau_{had}", kRed+2, 1, "((mc_type&0x0F00)/0x100)==0 && ((mc_type&0x000F)-((mc_type&0x00F0)/0x10))==2"));//11
  samples.push_back(sfeats(s_tt, "1 e/#mu, 1 #tau_{had}", kMagenta+2, 1, "((mc_type&0x0F00)/0x100)==1 && ((mc_type&0x000F)-((mc_type&0x00F0)/0x10))==1"));//12
  samples.push_back(sfeats(s_tt, "2 e/#mu", kBlue+2, 1, "((mc_type&0x0F00)/0x100)==2 && ((mc_type&0x000F)-((mc_type&0x00F0)/0x10))==0"));//13
  samples.push_back(sfeats(s_tt, "3+ leps", kGreen+2, 1, "(((mc_type&0x0F00)/0x100)+((mc_type&0x000F)-((mc_type&0x00F0)/0x10)))>2"));//14


  //  tree.mc_type()&0x00F0)>>4 //leptonic taus

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

  vector<int> leps;
  leps.push_back(8);
  leps.push_back(9);
  leps.push_back(10);
  leps.push_back(11);
  leps.push_back(12);
  leps.push_back(13);
  leps.push_back(14);

  vector<int> dileps;
  dileps.push_back(11);
  dileps.push_back(12);
  dileps.push_back(13);
  dileps.push_back(14);
  vector<pfeats> pies;
  ///////////////////////////////////////////////////////////////
  /*pies.push_back(pfeats(bkgs, "ht>500&&met>200&&njets>=6&&nbm>=2&&mt>125&&mj>400", "comp"));
  pies.push_back(pfeats(bkgs, "ht>500&&met>200&&njets>=6&&nbm>=2&&mt>125&&mj<=400", "comp"));
  pies.push_back(pfeats(bkgs, "ht>500&&met>200&&njets>=6&&nbm>=2&&mt<=125&&mj>400", "comp"));
  pies.push_back(pfeats(bkgs, "ht>500&&met>200&&njets>=6&&nbm>=2&&mt<=125&&mj<=400", "comp"));*/

  pies.push_back(pfeats(dileps, "ht>500&&met>200&&njets>=6&&nbm>=1&&mt>140", "dileps"));
  pies.push_back(pfeats(dileps, "ht>500&&mj>300&&met>200&&njets>=6&&nbm>=1&&mt>140", "dileps"));
  // pies.push_back(pfeats(dileps, "ht>500&&met>200&&njets>=6&&nbm>=2&&mt<=125&&mj>400", "leps"));
  // pies.push_back(pfeats(leps, "ht>500&&met>200&&njets>=6&&nbm>=2&&mt<=125&&mj<=400", "leps"));
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
  c.Print("plots/pies/legen_"+name+".pdf");
  TPie pie("", "", pfeat.samples.size(), &counts.at(0), &colors.at(0), &labels.at(0));
  pie.SetCircle(0.5, 0.5, 0.3);
  pie.Draw();
  c.Print("plots/pies/label_"+name+".pdf");
  pie.SetLabelFormat("%val");
  pie.SetValueFormat("%0.1f");
  pie.Draw();
  c.Print("plots/pies/count_"+name+".pdf");
  pie.SetLabelFormat("");
  pie.SetCircle(0.5, 0.5, 0.49);
  pie.Draw();
  c.Print("plots/pies/plain_"+name+".pdf");
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
