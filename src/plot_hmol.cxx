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
  

  vector<hfeats> vars;
  //////////////////// N-1 plots ////////////////////////////////

  vars.push_back(hfeats("mj",50,0,3000, ra4_sam, "MJ",Baseline));

  plot_distributions(Samples, vars, "3", ".pdf", "RA4");

}
