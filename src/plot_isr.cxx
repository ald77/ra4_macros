#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace {
  TString luminosity="2.1";
  TString plot_type=".pdf";
  TString plot_style="CMSPaper";
}

using namespace std;

int main(){

  TString skim = "zisr";                          
  TString folder1l("/cms2r0/babymaker/babies/2015_11_20/data/singlelep/combined/skim_"+skim+"/");
  TString foldermc("/cms2r0/babymaker/babies/2015_11_28/mc/skim_"+skim+"/");
  TString folderdy("/cms2r0/babymaker/babies/2015_11_28/mc/skim_"+skim+"/");

  vector<TString> s_slep;
  s_slep.push_back(folder1l+"*root");

  vector<TString> s_t1t;
  s_t1t.push_back(foldermc+"*T1tttt*1500_*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(foldermc+"*T1tttt*1200_*");
  vector<TString> s_tt;
  s_tt.push_back(foldermc+"*_TTJets*Lept*");
  s_tt.push_back(foldermc+"*_TTJets_HT*");
  vector<TString> s_ttv;
  s_ttv.push_back(foldermc+"*_TTWJets*");
  s_ttv.push_back(foldermc+"*_TTZTo*");
  s_ttv.push_back(foldermc+"*_TTG*");
  s_ttv.push_back(foldermc+"*_TTTT*");
  vector<TString> s_vv;
  s_vv.push_back(foldermc+"*_WZTo*");
  s_vv.push_back(foldermc+"*_WWTo*");
  s_vv.push_back(foldermc+"*_ZZTo*");
  vector<TString> s_dy;
  s_dy.push_back(folderdy+"*DYJetsToLL_M-50_*");
  vector<TString> s_other;                                                                                      
  s_other.push_back(foldermc+"*_ZJet*");
  s_other.push_back(foldermc+"*_WJetsToLNu*");
  s_other.push_back(foldermc+"*_ST_*");
  s_other.push_back(foldermc+"*ggZH_HToBB*");
  s_other.push_back(foldermc+"*ttHJetTobb*");
  // s_other.push_back(foldermc+"*QCD*");


  // Reading ntuples                                                                                                                  

  vector<sfeats> Samples;
  if (skim=="zisr"){
    Samples.push_back(sfeats(s_slep, "Data", kBlack,1,"pass&&(trig[18]||trig[21]||trig[23]||trig[24])")); Samples.back().isData = true;
    Samples.push_back(sfeats(s_dy, "Z+jets", dps::c_qcd,1,"stitch"));
    Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
    Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1&&stitch"));
    Samples.push_back(sfeats(s_vv, "VV", dps::c_singlet));
    Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
    Samples.push_back(sfeats(s_other, "Other", dps::c_other, 1));
  } else {
    Samples.push_back(sfeats(s_slep, "Data", kBlack,1,"pass&&(trig[18]||trig[21]||trig[23]||trig[24])")); Samples.back().isData = true;
    Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 true leptons", dps::c_tt_2l,1,"ntruleps>=2&&stitch"));
    Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 true lepton", dps::c_tt_1l, 1,"ntruleps<=1&&stitch"));
    Samples.push_back(sfeats(s_dy, "Z+jets", dps::c_qcd,1,"stitch"));
    Samples.push_back(sfeats(s_vv, "Single top", dps::c_singlet));
    Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));
    Samples.push_back(sfeats(s_other, "Other", dps::c_other, 1));
  }

  vector<int> ra4_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
  } // Loop over samples                                                                                                              



  vector<hfeats> vars;
  float ibinning[] = {0., 50., 100., 150., 200., 300., 400.,600., 800.};
  int nbins = sizeof(ibinning)/sizeof(float)-1;

  if (skim=="zisr"){
    //---------- Z+jets ISR --------------------
    vars.push_back(hfeats("elelv_pt*(elelv_m>0)+mumuv_pt*(mumuv_m>0)",nbins,ibinning, ra4_sam, "p_{T}(ll) [GeV]",
                  "nvleps==2&&nleps>=1&&Max$(leps_pt)>30&&((elelv_m>80&&elelv_m<100)||(mumuv_m>80&&mumuv_m<100))"));
    vars.back().whichPlots = "1";
  } else {
    //---------- TTBAR ISR --------------------
    vars.push_back(hfeats("jetsys_nob_pt",nbins,ibinning, ra4_sam, "p_{T}(ISR jets) [GeV]",
                          "nleps==2&&Max$(leps_pt)>30&&njets>2&&nbm==2"));
    vars.back().whichPlots = "12"; vars.back().normalize = true;
  }                                                                                                                                                                                            
  plot_distributions(Samples, vars, luminosity, plot_type, plot_style, "1d_dec11",true);

}


