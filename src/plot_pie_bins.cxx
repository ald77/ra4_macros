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
  TString ntuple_date("2016_06_14");
  TString lumi = "5";
  TString plot_type=".pdf";
  TString plot_style="RA4";
}

using namespace std;

int main(){
  styles style(plot_style);
  style.setDefaultStyle();

  TString folder="/cms2r0/babymaker/babies/"+ntuple_date+"/mc/skim_baseline/";

  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJets*Lept*");
  s_tt.push_back(folder+"*_TTJets*HT*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*_WJetsToLNu*");
  vector<TString> s_single;
  s_single.push_back(folder+"*_ST_*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder+"*TTW*");
  s_ttv.push_back(folder+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD_HT*");
  s_other.push_back(folder+"*DYJetsToLL*.root");
  s_other.push_back(folder+"*_ZJet*.root");
  s_other.push_back(folder+"*ggZH_HToBB*.root");
  s_other.push_back(folder+"*ttHJetTobb*.root");
  s_other.push_back(folder+"*_TTGJets*.root");
  s_other.push_back(folder+"*_TTTT*.root");
  s_other.push_back(folder+"*_WH_HToBB*.root");
  s_other.push_back(folder+"*_ZH_HToBB*.root");
  s_other.push_back(folder+"*_WWTo*.root");
  s_other.push_back(folder+"*_WZTo*.root");
  s_other.push_back(folder+"*_ZZ_*.root");

  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");

  // Reading
  vector<TChain*> chains;
  vector<sfeats> samples;
  samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1&&stitch")); 
  samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
  samples.push_back(sfeats(s_wjets, "W+jets", dps::c_wjets,1));
  samples.push_back(sfeats(s_single, "Single t", 42,1));
  samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv,1));
  samples.push_back(sfeats(s_other, "Other", 2001, 1));


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

  vector<pfeats> pies;
  TString baseline = "ht>500&&met>200&&nleps==1&&nveto==0&&njets>=6&&nbm>=1&&mj14>250&&pass&&stitch";

  /*  ///////////////////////////////////////////////////////////////
  //LOW MET
  //Low nj
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm>=1&&mj14<=400)||(mj14>400&&njets<=8&&nbm==1))","lowmet_lownj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm>=1&&mj14<=400)||(mj14>400&&njets<=8&&nbm==2))","lowmet_lownj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm>=1&&mj14<=400)||(mj14>400&&njets<=8&&nbm>=3))","lowmet_lownj_highnb"));
  //High nj
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm>=1&&mj14<=400)||(mj14>400&&njets>=9&&nbm==1))","lowmet_highnj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm>=1&&mj14<=400)||(mj14>400&&njets>=9&&nbm==2))","lowmet_highnj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm>=1&&mj14<=400)||(mj14>400&&njets>=9&&nbm>=3))","lowmet_highnj_highnb"));
  //MED MET
  //Low nj
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets<=8&&nbm==1))","medmet_lownj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets<=8&&nbm==2))","medmet_lownj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets<=8&&nbm>=3))","medmet_lownj_highnb"));
  //High nj
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets>=9&&nbm==1))","medmet_highnj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets>=9&&nbm==2))","medmet_highnj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets>=9&&nbm>=3))","medmet_highnj_highnb"));
  //HIGH MET
  //Low nj
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets<=8&&nbm==1))","highmet_lownj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets<=8&&nbm==2))","highmet_lownj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets<=8&&nbm>=3))","highmet_lownj_highnb"));
  //High nj
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets>=9&&nbm==1))","highmet_highnj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets>=9&&nbm==2))","highmet_highnj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm>=1&&mj14<=400)||(mj14>400&&njets>=9&&nbm>=3))","highmet_highnj_highnb"));
  /////////////////////////////////////////////////////////////// */


  // NO NB=1 INTEGRATION
  ///////////////////////////////////////////////////////////////
  //LOW MET
  //Low nj
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm==1&&mj14<=400)||(mj14>400&&njets<=8&&nbm==1))","no_nb1int_lowmet_lownj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm>=2&&mj14<=400)||(mj14>400&&njets<=8&&nbm==2))","no_nb1int_lowmet_lownj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm>=2&&mj14<=400)||(mj14>400&&njets<=8&&nbm>=3))","no_nb1int_lowmet_lownj_highnb"));
  //High nj
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm==1&&mj14<=400)||(mj14>400&&njets>=9&&nbm==1))","no_nb1int_lowmet_highnj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm>=2&&mj14<=400)||(mj14>400&&njets>=9&&nbm==2))","no_nb1int_lowmet_highnj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met<=350&&((nbm>=2&&mj14<=400)||(mj14>400&&njets>=9&&nbm>=3))","no_nb1int_lowmet_highnj_highnb"));
  //MED MET
  //Low nj
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm==1&&mj14<=400)||(mj14>400&&njets<=8&&nbm==1))","no_nb1int_medmet_lownj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm>=2&&mj14<=400)||(mj14>400&&njets<=8&&nbm==2))","no_nb1int_medmet_lownj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm>=2&&mj14<=400)||(mj14>400&&njets<=8&&nbm>=3))","no_nb1int_medmet_lownj_highnb"));
  //High nj
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm==1&&mj14<=400)||(mj14>400&&njets>=9&&nbm==1))","no_nb1int_medmet_highnj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm>=2&&mj14<=400)||(mj14>400&&njets>=9&&nbm==2))","no_nb1int_medmet_highnj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>350&&met<=500&&((nbm>=2&&mj14<=400)||(mj14>400&&njets>=9&&nbm>=3))","no_nb1int_medmet_highnj_highnb"));
  //HIGH MET
  //Low nj
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm==1&&mj14<=400)||(mj14>400&&njets<=8&&nbm==1))","no_nb1int_highmet_lownj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm>=2&&mj14<=400)||(mj14>400&&njets<=8&&nbm==2))","no_nb1int_highmet_lownj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm>=2&&mj14<=400)||(mj14>400&&njets<=8&&nbm>=3))","no_nb1int_highmet_lownj_highnb"));
  //High nj
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm==1&&mj14<=400)||(mj14>400&&njets>=9&&nbm==1))","no_nb1int_highmet_highnj_lownb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm>=2&&mj14<=400)||(mj14>400&&njets>=9&&nbm==2))","no_nb1int_highmet_highnj_mednb"));
  pies.push_back(pfeats(bkgs,baseline+"&&met>500&&((nbm>=2&&mj14<=400)||(mj14>400&&njets>=9&&nbm>=3))","no_nb1int_highmet_highnj_highnb"));
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
    TString cut = lumi+"*weight*("+pfeat.cut+"&&"+sample.cut+")";
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
  c.Print("plots/pie_bins/legen_"+name+plot_type);
  TPie pie("", "", pfeat.samples.size(), &counts.at(0), &colors.at(0), &labels.at(0));
  pie.SetCircle(0.5, 0.5, 0.35);
  pie.Draw();
  c.Print("plots/pie_bins/label_"+name+plot_type);
  pie.SetLabelFormat("%val");
  pie.SetValueFormat("%0.1f");
  pie.Draw();
  c.Print("plots/pie_bins/count_"+name+plot_type);
  pie.SetLabelFormat("");
  pie.SetCircle(0.5, 0.5, 0.35);
  pie.Draw();
  pie.SetLabelFormat("%perc");
  pie.SetValueFormat("%0.1f");
  pie.Draw();
  c.Print("plots/pie_bins/perc_"+name+plot_type);
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
