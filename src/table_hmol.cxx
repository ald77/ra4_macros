//table_hmol.cxx: Creates tables that investigate high mT, single (true) lepton tt events

#include <iostream>
#include <fstream>
#include <vector>

#include "TChain.h"
#include "TString.h"

#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace {
  TString ntuple_date("2015_10_19");
  TString lumi("3");
}

using namespace std;


void maketable(const vector<double> &tot_entries, const vector<vector<vector<double> > > &entries);

int main(){

  TChain* tt = new TChain("tree");
  tt->Add("/afs/cern.ch/user/m/manuelf/work/babies/"+ntuple_date+"/mc/skim_1lht500met200/*TTJets*HT*");
  tt->Add("/afs/cern.ch/user/m/manuelf/work/babies/"+ntuple_date+"/mc/skim_1lht500met200/*TTJets*Lept*");

  //  tt->Add("/afs/cern.ch/user/m/manuelf/work/babies/2015_10_28/151028_044540/*TTJets*HT*");
  //  tt->Add("/afs/cern.ch/user/m/manuelf/work/babies/2015_10_28/151028_044540/*TTJets*Lept*");

  //TString cuts
  TString Baseline = "ht>500&&met>200&&njets>=6&&nbm>=1&&nleps==1&&mj>250";

  TString And = "&&";
  TString Or = "||";

  TString OneTruLep = "ntruleps==1";
  TString hmt = "mt>140";

  TString OneRecoEl = "nels==1";
  TString OneRecoMu = "nmus==1";

  TString ZeroGoodLeps = "(nleps_tm==0)";

  TString LostFakeEl = "("+OneRecoEl+And+ZeroGoodLeps+")";
  TString LostFakeMu = "("+OneRecoMu+And+ZeroGoodLeps+")";

  TString LostFake = "("+LostFakeEl+Or+LostFakeMu+")";
  TString NotLostFake = "!"+LostFake;

  TString LowGenMt = "(mt_tru<=140)";
  TString LowGenMtFromW = "(mt_tru_nuw<=140)";

  TString BadRecoMtEl = "("+OneRecoEl+And+LowGenMt+")";
  TString BadRecoMtMu = "("+OneRecoMu+And+LowGenMt+")";
  
  //CUTS
  vector<TString> nbcuts;
  nbcuts.push_back("nbm>=1");
  nbcuts.push_back("nbm>=2");
  nbcuts.push_back("nbm>=3");

  vector<vector<TString> > syscuts;
  syscuts.push_back(vector<TString>());
  syscuts.push_back(vector<TString>());
  syscuts[0].push_back(LostFakeEl);
  syscuts[0].push_back(NotLostFake+And+BadRecoMtEl);
  syscuts[0].push_back(NotLostFake+And+OneRecoEl+And+"!"+LowGenMt);
  syscuts[1].push_back(LostFakeMu);
  syscuts[1].push_back(NotLostFake+And+BadRecoMtMu);
  syscuts[1].push_back(NotLostFake+And+OneRecoMu+And+"!"+LowGenMt);

  // 3D vector. 1st argument: 0/1/2 = Nb=1/2/>=3. 2nd argument: 0/1/2 = Lost-Fake/RecoEffects/Other. 3rd argument: 0/1 = e/mu.
  vector<int> tot_entries; vector<double> tot_yield, tot_err;
  vector<vector<vector<int> > > Nentries;
  vector<vector<vector<double> > > yield, err;
  for(unsigned int iNb=0; iNb<nbcuts.size(); iNb++){
    Nentries.push_back(vector<vector<int> >()); yield.push_back(vector<vector<double> >()); err.push_back(vector<vector<double> >());      
    TString totcut_hmt = lumi+"*weight*("+Baseline+And+hmt+And+nbcuts[iNb]+")";

    double tot_baseline, tot_baseline_err;
    getYieldErr(*tt, totcut_hmt, tot_baseline, tot_baseline_err);

    TString totcut = lumi+"*weight*("+Baseline+And+OneTruLep+And+hmt+And+nbcuts[iNb]+")";
    
    int tot_tempentry(-1); double tot_tempyield, tot_temperr;
    tot_tempentry = getYieldErr(*tt, totcut, tot_tempyield, tot_temperr);

    tot_entries.push_back(tot_tempentry); tot_yield.push_back(tot_tempyield); tot_err.push_back(tot_temperr);

    cout<<"Totals for Nb="<<iNb+1<<" high mT, 1l events: Entries = "<<tot_entries[iNb]<<", Yield = "<<RoundNumber(tot_yield[iNb],2)<<" out of "<<RoundNumber(tot_baseline,2)<<" high mT tt events ("<<RoundNumber(tot_yield[iNb]/tot_baseline*100,2)<<"%)"<<endl;

    for(unsigned int iSys=0; iSys<(syscuts[0].size()); iSys++){
      Nentries[iNb].push_back(vector<int>()); yield[iNb].push_back(vector<double>()); err[iNb].push_back(vector<double>());
      for(unsigned int iLep=0; iLep<syscuts.size(); iLep++){

	totcut = lumi+"*weight*("+Baseline+And+OneTruLep+And+hmt+And+nbcuts[iNb]+And+syscuts[iLep][iSys]+")";

	//cout<<"totcut="<<totcut<<endl;
	
	int temp_entries(0);
	double temp_yield(0.), temp_err(0.);
	temp_entries = getYieldErr(*tt, totcut, temp_yield, temp_err);
	
	Nentries[iNb][iSys].push_back(temp_entries); yield[iNb][iSys].push_back(temp_yield); err[iNb][iSys].push_back(temp_err);
	
	cout<<"Nentries = "<<Nentries[iNb][iSys][iLep]<<", yield = "<<yield[iNb][iSys][iLep]<<", err = "<<err[iNb][iSys][iLep]<<endl;     
      }
    }
    cout<<"\n"<<endl;
  }

  maketable(tot_yield, yield);
  
}

void maketable(const vector<double> &tot_entries, const vector<vector<vector<double> > > &entries){

  for(int iNb=0; iNb<3; iNb++){
    
    TString tname("txt/table_hmol_Nb");
    tname += (iNb+1);
    tname += "_"+ntuple_date+".tex";

    ifstream header("txt/header.tex");
    ifstream footer("txt/footer.tex");
    ofstream file(tname);
    file<<header.rdbuf();

    //
    file<< "\\begin{tabular}[tbp!]{l|c|c|c} \\hline \\hline"<<endl;

    file << "Baseline (${\\cal L} = "<<lumi<<"$ fb$^{-1}$) & Reco Lepton & MC Predicted Events & \\% (for both flavors) \\\\"<<endl;
    file << "\\hline \\hline"<<endl;
    file << "Lost-Fake Lepton & e & "<<RoundNumber(entries[iNb][0][0],1)<<" & \\multirow{2}{*}{"<<RoundNumber((entries[iNb][0][0]+entries[iNb][0][1])/tot_entries[iNb]*100.,0)<<"\\%} \\\\"<<endl;
    file << "& $\\mu$ & "<<RoundNumber(entries[iNb][0][1],1)<<" & \\\\"<<endl;
    file << "\\hline"<<endl;
    file << "Jet Mismeasurement & e & "<<RoundNumber(entries[iNb][1][0],1)<<" & \\multirow{2}{*}{"<<RoundNumber((entries[iNb][1][0]+entries[iNb][1][1])/tot_entries[iNb]*100.,0)<<"\\%} \\\\"<<endl;
    file << "& $\\mu$ & "<<RoundNumber(entries[iNb][1][1],1)<<" & \\\\"<<endl;
    file << "\\hline"<<endl;
    file << "Non-Prompt Neutrinos & e & "<<RoundNumber(entries[iNb][2][0],1)<<" & \\multirow{2}{*}{"<<RoundNumber((entries[iNb][2][0]+entries[iNb][2][1])/tot_entries[iNb]*100.,0)<<"\\%} \\\\"<<endl;
  file << "& $\\mu$ & "<<RoundNumber(entries[iNb][2][1],1)<<" & \\\\"<<endl;
  file << "\\hline \\hline"<<endl;      

    file << "\\end{tabular}"<<endl;

    file<<footer.rdbuf();
    file.close();
    cout<<"Written "<<tname<<endl;
    
  }  
}
