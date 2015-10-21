// plot_1d: Macro that plots variables both lumi weighted and normalized to the same area.

#include <iostream>
#include <vector>

#include "TString.h"

#include "utilities_macros.hpp"

namespace{
  TString ntuple_date("2015_10_09");
}

using namespace std;

int main(){ 

  // Reading ntuples
  TString folder_tt_skim, folder_tt_genht, folder_other;
  folder_tt_skim="~/archive/"+ntuple_date+"/skim_1l/";
  folder_tt_genht="~/archive/"+ntuple_date+"/skim_1l_genht/";

  vector<TString> s_tt;
  s_tt.push_back(folder_tt_skim+"*_TTJets*HT*");
  s_tt.push_back(folder_tt_genht+"*_TTJets*Lept*");

  //TString cuts
  TString Baseline = "ht>500&&met>200&&njets>=7&&nbm>=2&&(nels+nmus)==1";
  TString BaselineNb1 = "ht>500&&met>200&&njets>=7&&nbm==1&&(nels+nmus)==1";

  TString And = "&&";
  TString Or = "||";

  TString OneTruLep = "ntruleps==1";
  TString hmt = "mt>140";

  TString OneRecoEl = "nels==1";
  TString OneRecoMu = "nmus==1";

  TString Zerogoodels = "(nels_tru_prompt==0)";
  TString Zerogoodmus = "(nmus_tru_prompt==0)";

  TString LostFakeEl = "("+OneRecoEl+And+Zerogoodels+")";
  TString LostFakeMu = "("+OneRecoMu+And+Zerogoodmus+")";

  TString LostFake = "("+LostFakeEl+Or+LostFakeMu+")";
  TString NotLostFake = "!"+LostFake;

  TString GenMtEl = "(Max$(els_genmt)<=140)";
  TString GenMtMu = "(Max$(mus_genmt)<=140)";

  TString GenMtFromWEl = "(Max$(els_genmt_fromw)<=140)";
  TString GenMtFromWMu = "(Max$(mus_genmt_fromw)<=140)";

  TString BadRecoMtEl = "("+OneRecoEl+And+GenMtEl+")";
  TString BadRecoMtMu = "("+OneRecoMu+And+GenMtMu+")";

  TString BadRecoMt = "("+BadRecoMtEl+Or+BadRecoMtMu+")";
  TString NotBadRecoMt = "!"+BadRecoMt;

  vector<sfeats> Samples;
  Samples.push_back(sfeats(s_tt, "mt#leq140", 1,1,"mt<=140"));
  Samples.push_back(sfeats(s_tt, "Lost-Fake", 31,1,OneTruLep+And+hmt+And+LostFake));
  Samples.push_back(sfeats(s_tt, "Reco Effects", 46,1,OneTruLep+And+hmt+And+NotLostFake+And+"("+BadRecoMtEl+Or+BadRecoMtMu+")"));
  Samples.push_back(sfeats(s_tt, "Other", 41,1,OneTruLep+And+hmt+And+NotLostFake+And+NotBadRecoMt)); 

  vector<int> ra4_sam;
  ra4_sam.push_back(0);
  ra4_sam.push_back(1);
  ra4_sam.push_back(2);
  ra4_sam.push_back(3);

  vector<int> ra4_hmt;
  ra4_hmt.push_back(1);
  ra4_hmt.push_back(2);
  ra4_hmt.push_back(3);

  vector<int> ra4_lostfake;
  ra4_lostfake.push_back(1);

  vector<sfeats> mt_njets;
  mt_njets.push_back(sfeats(s_tt, "4#leqnjets#leq5", 1,1,"ht>500&&met>200&&nleps==1&&njets>=4&&njets<=5"));
  mt_njets.push_back(sfeats(s_tt, "6#leqnjets#leq7", 31,1,"ht>500&&met>200&&nleps==1&&njets>=6&&njets<=7"));
  mt_njets.push_back(sfeats(s_tt, "8#leqnjets#leq9", 46,1,"ht>500&&met>200&&nleps==1&&njets>=8&&njets<=9"));
  mt_njets.push_back(sfeats(s_tt, "njets#geq10", 41,1,"ht>500&&met>200&&nleps==1&&njets>=10"));

  vector<int> ra4_mt_njets;
  ra4_mt_njets.push_back(0);
  ra4_mt_njets.push_back(1);
  ra4_mt_njets.push_back(2);
  ra4_mt_njets.push_back(3);

  vector<sfeats> mt_comp;
  mt_comp.push_back(sfeats(s_tt, "mt_{reco}",1,1,"1","mt"));
  mt_comp.push_back(sfeats(s_tt, "mt_{gen}",31,1,"1","(Max$(els_genmt)+Max$(mus_genmt))"));
  mt_comp.push_back(sfeats(s_tt, "mt_{fromW}",46,1,"1","(Max$(els_genmt_fromw)+Max$(mus_genmt_fromw))"));

  vector<int> ra4_mt_comp;
  ra4_mt_comp.push_back(0);
  ra4_mt_comp.push_back(1);
  ra4_mt_comp.push_back(2);

  vector<int> ra4_2D_mt_comp;
  ra4_2D_mt_comp.push_back(0);

  vector<hfeats> vars, vars_hmt, vars_lostfake, vars_mt_njets, vars_mt_comp, vars_2D_mt_comp;
  //////////////////// N-1 plots ////////////////////////////////

  vars.push_back(hfeats("mj",50,0,3000, ra4_sam, "MJ",Baseline));
  vars.push_back(hfeats("mj",50,0,3000, ra4_sam, "MJ",BaselineNb1));
  vars.push_back(hfeats("ntrunus-ntrunus_fromw",15,0,15, ra4_sam, "Non-Prompt Neutrinos",Baseline));
  vars.push_back(hfeats("ntrunus-ntrunus_fromw",15,0,15, ra4_sam, "Non-Prompt Neutrinos",BaselineNb1));
  vars.push_back(hfeats("(Max$(els_genmt)+Max$(mus_genmt))",33,0,660, ra4_sam, "gen mt",Baseline));
  vars.push_back(hfeats("(Max$(els_genmt_fromw)+Max$(mus_genmt_fromw))",33,0,660, ra4_sam, "gen mt from w",Baseline));


  plot_distributions(Samples, vars, "3", ".pdf", "RA4");

  vars_hmt.push_back(hfeats("mt",60,140,1340, ra4_hmt, "m_{T}",Baseline));
  vars_hmt.push_back(hfeats("mt",60,140,1340, ra4_hmt, "m_{T}",BaselineNb1));

  plot_distributions(Samples, vars_hmt, "3", ".pdf", "RA4");

  vars_lostfake.push_back(hfeats("els_tru_phi",20,-3.2,3.2, ra4_lostfake, "els_tru_phi",Baseline+"&&ntruels==1"));
  vars_lostfake.push_back(hfeats("els_tru_phi",20,-3.2,3.2, ra4_lostfake, "els_tru_phi",BaselineNb1+"&&ntruels==1"));
  vars_lostfake.push_back(hfeats("mus_tru_phi",20,-3.2,3.2, ra4_lostfake, "mus_tru_phi",Baseline+"&&ntrumus==1"));
  vars_lostfake.push_back(hfeats("mus_tru_phi",20,-3.2,3.2, ra4_lostfake, "mus_tru_phi",BaselineNb1+"&&ntrumus==1"));

  vars_lostfake.push_back(hfeats("els_tru_eta",50,-5,5, ra4_lostfake, "els_tru_eta",Baseline+"&&ntruels==1"));
  vars_lostfake.push_back(hfeats("els_tru_eta",50,-5,5, ra4_lostfake, "els_tru_eta",BaselineNb1+"&&ntruels==1"));
  vars_lostfake.push_back(hfeats("mus_tru_eta",50,-5,5, ra4_lostfake, "mus_tru_eta",Baseline+"&&ntrumus==1"));
  vars_lostfake.push_back(hfeats("mus_tru_eta",50,-5,5, ra4_lostfake, "mus_tru_eta",BaselineNb1+"&&ntrumus==1"));

  plot_distributions(Samples, vars_lostfake, "3", ".pdf", "RA4");

  vars_mt_njets.push_back(hfeats("mt",33,0,660, ra4_mt_njets, "mt","nbm>=2"));
  vars_mt_njets.push_back(hfeats("mt",33,0,660, ra4_mt_njets, "mt","nbm==1"));
  
  plot_distributions(mt_njets, vars_mt_njets, "3", ".pdf", "RA4");
  
  vars_mt_comp.push_back(hfeats("mt",66,0,660, ra4_mt_comp, "mt comparison",Baseline));
  vars_mt_comp.push_back(hfeats("mt",66,0,660, ra4_mt_comp, "mt comparison",Baseline+And+OneTruLep));

  plot_distributions(mt_comp, vars_mt_comp, "3", ".pdf", "RA4");

  vars_2D_mt_comp.push_back(hfeats("mt","(Max$(els_genmt_fromw)+Max$(mus_genmt_fromw))",33,0,330,33,0,330,ra4_2D_mt_comp,"mt","mt_{gen}",Baseline,140,140,""));
  vars_2D_mt_comp.push_back(hfeats("mt","(Max$(els_genmt_fromw)+Max$(mus_genmt_fromw))",66,0,330,66,0,330,ra4_2D_mt_comp,"mt","mt_{gen}",Baseline+And+LostFake,140,140,""));
  vars_2D_mt_comp.push_back(hfeats("mt","(Max$(els_genmt_fromw)+Max$(mus_genmt_fromw))",33,0,330,33,0,330,ra4_2D_mt_comp,"mt","mt_{gen}",Baseline+And+NotLostFake+And+"("+BadRecoMtEl+Or+BadRecoMtMu+")",140,140,""));
  vars_2D_mt_comp.push_back(hfeats("mt","(Max$(els_genmt_fromw)+Max$(mus_genmt_fromw))",33,0,330,33,0,330,ra4_2D_mt_comp,"mt","mt_{gen}",Baseline+And+NotLostFake+And+NotBadRecoMt,140,140,""));
  vars_2D_mt_comp.push_back(hfeats("mt","(Max$(els_genmt)+Max$(mus_genmt))",33,0,330,33,0,330,ra4_2D_mt_comp,"mt","mt_{gen}",Baseline,140,140,""));
  vars_2D_mt_comp.push_back(hfeats("mt","(Max$(els_genmt)+Max$(mus_genmt))",66,0,330,66,0,330,ra4_2D_mt_comp,"mt","mt_{gen}",Baseline+And+LostFake,140,140,""));
  vars_2D_mt_comp.push_back(hfeats("mt","(Max$(els_genmt)+Max$(mus_genmt))",33,0,330,33,0,330,ra4_2D_mt_comp,"mt","mt_{gen}",Baseline+And+NotLostFake+And+"("+BadRecoMtEl+Or+BadRecoMtMu+")",140,140,""));
  vars_2D_mt_comp.push_back(hfeats("mt","(Max$(els_genmt)+Max$(mus_genmt))",33,0,330,33,0,330,ra4_2D_mt_comp,"mt","mt_{gen}",Baseline+And+NotLostFake+And+NotBadRecoMt,140,140,""));

  plot_2D_distributions(mt_comp,vars_2D_mt_comp,"3",".pdf","2Dtitle","1d/");

}
