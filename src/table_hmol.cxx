//table_hmol.cxx: Creates tables that investigate high mT, single (true) lepton tt events

#include <iostream>
#include <fstream>
#include <vector>

#include "TChain.h"
#include "TString.h"

#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace {
  TString ntuple_date("2015_10_09");
  TString lumi("3");
}

using namespace std;


void maketable(const vector<double> &tot_entries, const vector<vector<vector<double> > > &entries);

int main(){

  TChain* tt = new TChain("tree");
  tt->Add("/afs/cern.ch/work/r/rbhandar/public/archive/"+ntuple_date+"/skim_1l/*HT*");
  tt->Add("/afs/cern.ch/work/r/rbhandar/public/archive/"+ntuple_date+"/skim_1l_genht/*SingleLept*");

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

  TString GenMtEl = "(els_genmt[0]<=140)";
  TString GenMtMu = "(mus_genmt[0]<=140)";

  TString GenMtFromWEl = "(els_genmt_fromw[0]<=140)";
  TString GenMtFromWMu = "(mus_genmt_fromw[0]<=140)";

  TString BadRecoMtEl = "("+OneRecoEl+And+GenMtEl+")";
  TString BadRecoMtMu = "("+OneRecoMu+And+GenMtMu+")";
  
  // 3D vector. 1st argument: 0/1 = Nb=1/>=2. 2nd argument: 0/1/2 = Lost-Fake/RecoEffects/Other. 3rd argument: 0/1 = e/mu.
  vector<int> tot_entries; vector<double> tot_yield, tot_err;
  vector<vector<vector<int> > > Nentries;
  vector<vector<vector<double> > > yield, err;
  for(int iNb=0; iNb<2; iNb++){
    Nentries.push_back(vector<vector<int> >()); yield.push_back(vector<vector<double> >()); err.push_back(vector<vector<double> >());      
    
    TString totcut = lumi+"*weight*("+BaselineNb1+And+OneTruLep+And+hmt+")";
    if(iNb==1) totcut = lumi+"*weight*("+Baseline+And+OneTruLep+And+hmt+")";

    int tot_tempentry(-1); double tot_tempyield, tot_temperr;
    tot_tempentry = getYieldErr(*tt, totcut, tot_tempyield, tot_temperr);

    tot_entries.push_back(tot_tempentry); tot_yield.push_back(tot_tempyield); tot_err.push_back(tot_temperr);
    
    if(iNb==0) cout<<"Totals for Nb=1 high mT, 1l events: Entries = "<<tot_entries[0]<<", Yield = "<<tot_yield[0]<<endl;
    if(iNb==1) cout<<"Totals for Nb>=2 high mT, 1l events: Entries = "<<tot_entries[1]<<", Yield = "<<tot_yield[1]<<endl;
    
    for(int iSys=0; iSys<3; iSys++){
      Nentries[iNb].push_back(vector<int>()); yield[iNb].push_back(vector<double>()); err[iNb].push_back(vector<double>());
      for(int iLep=0; iLep<2; iLep++){

	if(iNb==0) totcut = lumi+"*weight*("+BaselineNb1+And+OneTruLep+And+hmt+And;
	if(iNb==1) totcut = lumi+"*weight*("+Baseline+And+OneTruLep+And+hmt+And;          
	
	if(iLep==0){
	  if(iSys==0) totcut += LostFakeEl;
	  if(iSys==1) totcut += NotLostFake+And+BadRecoMtEl;
	  if(iSys==2) totcut += NotLostFake+And+OneRecoEl+And+"!"+GenMtEl;
	  //if(iSys==2) totcut += NotLostFake+And+OneRecoEl+And+"!"+GenMtEl+And+GenMtFromWEl;
	}
	if(iLep==1){
	  if(iSys==0) totcut += LostFakeMu;
	  if(iSys==1) totcut += NotLostFake+And+BadRecoMtMu;
	  if(iSys==2) totcut += NotLostFake+And+OneRecoMu+And+"!"+GenMtMu;
	  //if(iSys==2) totcut += NotLostFake+And+OneRecoMu+And+"!"+GenMtMu+And+GenMtFromWMu;
	}
	totcut += ")";
	
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

  for(int iNb=0; iNb<2; iNb++){
    
    TString tname("txt/table_hmol");
    if(iNb==0) tname += "_Nb1_";
    if(iNb==1) tname += "_Nb2_";
    tname += ntuple_date+".tex";

    ifstream header("txt/header.tex");
    ifstream footer("txt/footer.tex");
    ofstream file(tname);
    file<<header.rdbuf();

    //
    file<< "\\begin{tabular}[tbp!]{l|c|c|c} \\hline \\hline"<<endl;

    if(iNb==0) file << "Baseline + $N_{b}=1$ & Reco Lepton & $N_{Data}$ (out of "<<RoundNumber(tot_entries[0],3)<<") & \\% (for both flavors) \\\\"<<endl;
    if(iNb==1) file << "Baseline & Reco Lepton & $N_{Data}$ (out of "<<RoundNumber(tot_entries[1],3)<<") & \\% (for both flavors) \\\\"<<endl;
    file << "\\hline \\hline"<<endl;
    file << "Lost-Fake & e & "<<RoundNumber(entries[iNb][0][0],3)<<" & \\multirow{2}{*}{"<<RoundNumber((entries[iNb][0][0]+entries[iNb][0][1])/tot_entries[iNb]*100.,1)<<"\\%} \\\\"<<endl;
    file << "& $\\mu$ & "<<RoundNumber(entries[iNb][0][1],3)<<" & \\\\"<<endl;
    file << "\\hline"<<endl;
    file << "Reco Effects & e & "<<RoundNumber(entries[iNb][1][0],3)<<" & \\multirow{2}{*}{"<<RoundNumber((entries[iNb][1][0]+entries[iNb][1][1])/tot_entries[iNb]*100.,1)<<"\\%} \\\\"<<endl;
    file << "& $\\mu$ & "<<RoundNumber(entries[iNb][1][1],3)<<" & \\\\"<<endl;
    file << "\\hline"<<endl;
    file << "Other & e & "<<RoundNumber(entries[iNb][2][0],3)<<" & \\multirow{2}{*}{"<<RoundNumber((entries[iNb][2][0]+entries[iNb][2][1])/tot_entries[iNb]*100.,1)<<"\\%} \\\\"<<endl;
  file << "& $\\mu$ & "<<RoundNumber(entries[iNb][2][1],3)<<" & \\\\"<<endl;
  file << "\\hline \\hline"<<endl;      

    file << "\\end{tabular}"<<endl;

    file<<footer.rdbuf();
    file.close();
    cout<<"Written "<<tname<<endl;
    
  }  
}
