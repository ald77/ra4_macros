// plot_1d: Macro that plots variables both lumi weighted and normalized to the same area.
#include "small_tree_full.hpp"
#include <iostream>
#include <vector>

#include "TString.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TString.h"
#include "TProfile2D.h"
#include "TLine.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"
#include "TStyle.h"

#include "styles.hpp"
#include "utilities_macros.hpp"
#include "utilities.hpp"
#include "TRandom3.h"

namespace{
  std::string ntuple_date = "2015_10_12";
  double lumi = 3;
  const int nExp = 10;
}

using namespace std;

int main(){

  TH1::SetDefaultSumw2(true);
  TH1::StatOverflows(kTRUE);
  
 // string folder="~/archive/"+ntuple_date+"/skim_tight/";
 // string folder_genht="~/archive/"+ntuple_date+"/skim_tight_genht/";

  string folder="/Users/jaehyeok/scratch/JetMismeasurement/babies/skim_tighter/";
  string folder_genht="/Users/jaehyeok/scratch/JetMismeasurement/babies/skim_tighter_genht/";

  small_tree_quick tt(folder+"*HT*");  
  tt.Add(folder_genht+"*Lept*");
  const int nEntries = tt.GetEntries();

  cout<<"Entries = "<<nEntries<<endl;

  TH1D h_ht_mean("ht_mean","ht_mean",220,928,950);
  //TH1D h_ht_mean("ht_mean","ht_mean",220,1500,2500);
  TH1D h_met_mean("met_mean","met_mean",100,265,275);
  TH1D h_njets_mean("njets_mean","njets_mean",100,7,10);
  TH1D h_nbm_mean("nbm_mean","nbm_mean",100,2,5);
  TH1D h_mj_mean("mj_mean","mj_mean",50,370,375);
  TH1D h_mt_mean("mt_mean","mt_mean",100,65,75);

  TH1D h_meth1A("meth1A","meth1_lNj_lMet",300,0.5,2.5);
  TH1D h_meth1B("meth1B","meth1_lNj_hMet",300,0.5,2.5);
  TH1D h_meth1C("meth1C","meth1_hNj_lMet",300,0.5,2.5);
  TH1D h_meth1D("meth1D","meth1_hNj_hMet",300,0.5,2.5);

  TH1D h_Rmt1A_lmj("Rmt1A_lmj","Rmt1A_lmj",200,0,0.2);
  TH1D h_Rmt1A_hmj("Rmt1A_hmj","Rmt1A_hmj",200,0,0.2);
  TH1D h_Rmt1B_lmj("Rmt1B_lmj","Rmt1B_lmj",200,0,0.2);
  TH1D h_Rmt1B_hmj("Rmt1B_hmj","Rmt1B_hmj",200,0,0.2);
  TH1D h_Rmt1C_lmj("Rmt1C_lmj","Rmt1C_lmj",200,0,0.2);
  TH1D h_Rmt1C_hmj("Rmt1C_hmj","Rmt1C_hmj",200,0,0.2);
  TH1D h_Rmt1D_lmj("Rmt1D_lmj","Rmt1D_lmj",200,0,0.2);
  TH1D h_Rmt1D_hmj("Rmt1D_hmj","Rmt1D_hmj",200,0,0.2);

  TH1D h_Rmj1A_lmt("Rmj1A_lmt","Rmj1A_lmt",200,0,1);
  TH1D h_Rmj1A_hmt("Rmj1A_hmt","Rmj1A_hmt",200,0,1);
  TH1D h_Rmj1B_lmt("Rmj1B_lmt","Rmj1B_lmt",200,0,1);
  TH1D h_Rmj1B_hmt("Rmj1B_hmt","Rmj1B_hmt",200,0,1);
  TH1D h_Rmj1C_lmt("Rmj1C_lmt","Rmj1C_lmt",200,0,1);
  TH1D h_Rmj1C_hmt("Rmj1C_hmt","Rmj1C_hmt",200,0,1);
  TH1D h_Rmj1D_lmt("Rmj1D_lmt","Rmj1D_lmt",200,0,1);
  TH1D h_Rmj1D_hmt("Rmj1D_hmt","Rmj1D_hmt",200,0,1);

  TH1D h_Rmt2A_lmj("Rmt2A_lmj","Rmt2A_lmj",200,0,0.15);
  TH1D h_Rmt2A_hmj("Rmt2A_hmj","Rmt2A_hmj",200,0,0.15);
  TH1D h_Rmt2B_lmj("Rmt2B_lmj","Rmt2B_lmj",200,0,0.15);
  TH1D h_Rmt2B_hmj("Rmt2B_hmj","Rmt2B_hmj",200,0,0.15);
  TH1D h_Rmt2C_lmj("Rmt2C_lmj","Rmt2C_lmj",200,0,0.15);
  TH1D h_Rmt2C_hmj("Rmt2C_hmj","Rmt2C_hmj",200,0,0.15);
  TH1D h_Rmt2D_lmj("Rmt2D_lmj","Rmt2D_lmj",200,0,0.15);
  TH1D h_Rmt2D_hmj("Rmt2D_hmj","Rmt2D_hmj",200,0,0.15);
  TH1D h_Rmt2E_lmj("Rmt2E_lmj","Rmt2E_lmj",200,0,0.15);
  TH1D h_Rmt2E_hmj("Rmt2E_hmj","Rmt2E_hmj",200,0,0.15);
  TH1D h_Rmt2F_lmj("Rmt2F_lmj","Rmt2F_lmj",200,0,0.15);
  TH1D h_Rmt2F_hmj("Rmt2F_hmj","Rmt2F_hmj",200,0,0.15);

  TH1D h_Rmj2A_lmt("Rmj2A_lmt","Rmj2A_lmt",200,0,1.5);
  TH1D h_Rmj2A_hmt("Rmj2A_hmt","Rmj2A_hmt",200,0,1.5);
  TH1D h_Rmj2B_lmt("Rmj2B_lmt","Rmj2B_lmt",200,0,1.5);
  TH1D h_Rmj2B_hmt("Rmj2B_hmt","Rmj2B_hmt",200,0,1.5);
  TH1D h_Rmj2C_lmt("Rmj2C_lmt","Rmj2C_lmt",200,0,1.5);
  TH1D h_Rmj2C_hmt("Rmj2C_hmt","Rmj2C_hmt",200,0,1.5);
  TH1D h_Rmj2D_lmt("Rmj2D_lmt","Rmj2D_lmt",200,0,1.5);
  TH1D h_Rmj2D_hmt("Rmj2D_hmt","Rmj2D_hmt",200,0,1.5);
  TH1D h_Rmj2E_lmt("Rmj2E_lmt","Rmj2E_lmt",200,0,1.5);
  TH1D h_Rmj2E_hmt("Rmj2E_hmt","Rmj2E_hmt",200,0,1.5);
  TH1D h_Rmj2F_lmt("Rmj2F_lmt","Rmj2F_lmt",200,0,1.5);
  TH1D h_Rmj2F_hmt("Rmj2F_hmt","Rmj2F_hmt",200,0,1.5);

  TH1D h_meth2A("meth2A","meth2_lNj_lMet_lNb",300,0.5,2.5);
  TH1D h_meth2B("meth2B","meth2_lNj_lMet_hNb",300,0.5,2.5);
  TH1D h_meth2C("meth2C","meth2_lNj_hMet",300,0.5,2.5);
  TH1D h_meth2D("meth2D","meth2_hNj_lMet_lNb",300,0.5,2.5);
  TH1D h_meth2E("meth2E","meth2_hNj_lMet_hNb",300,0.5,2.5);
  TH1D h_meth2F("meth2F","meth2_hNj_hMet",300,0.5,2.5);

  for(int iExp=0; iExp<nExp; iExp++){

    if(iExp%(nExp/10)==0) cout<<"Experiment #"<<iExp<<endl;
    
    TH1D h_ht("ht","ht",9500,500,10000);
    TH1D h_met("met","met",5000,200,5200);
    TH1D h_njets("njets","njets",10,0,10);
    TH1D h_nbm("nbm","nbm",10,0,10);
    TH1D h_mj("mj","mj",5000,0,5000);
    TH1D h_mt("mt","mt",2500,0,2500);

    double R1_1A=0, R2_1A=0, R3_1A=0, R4_1A=0;
    double R1_1B=0, R2_1B=0, R3_1B=0, R4_1B=0;
    double R1_1C=0, R2_1C=0, R3_1C=0, R4_1C=0;
    double R1_1D=0, R2_1D=0, R3_1D=0, R4_1D=0;

    double R1_2A=0, R2_2A=0, R3_2A=0, R4_2A=0;
    double R1_2B=0, R2_2B=0, R3_2B=0, R4_2B=0;
    double R1_2C=0, R2_2C=0, R3_2C=0, R4_2C=0;
    double R2_2D=0, R4_2D=0; 
    double R2_2E=0, R4_2E=0;
    double R2_2F=0, R4_2F=0;
 
    TRandom3 rand;
    rand.SetSeed(0);
    for(int iEnt=0; iEnt<nEntries; iEnt++){
    //for(int iEnt=0; iEnt<20; iEnt++){
      tt.GetEntry(iEnt);

      double weight = lumi*tt.weight();

      if(tt.nleps()==1){
	int randInt = rand.Integer(100); // Returns an int from 0-99
      
	//if(randInt == 93 || randInt == 36){	
	if(randInt<100000){	
	  int ipos = rand.Integer(tt.ht_var().size());

      //cout << "Entry : " << iEnt << " ipos : " << ipos << endl; 
      //cout << "normal == HT : "  << tt.ht() << " MET : "  << tt.met() << " nbm : "  << tt.nbm() << " njets : "  << tt.njets() << endl;
      //cout << "varied == HT : "  << tt.ht_var().at(ipos) << " MET : "  << tt.met_var().at(ipos) << " nbm : "  << tt.nbm_var().at(ipos) << " njets : "  << tt.njets_var().at(ipos) << endl;
	  
      // Event variable calculations 
	  //if(tt.ht_var().at(ipos)>500 && tt.nbm_var().at(ipos)>=2 && tt.met_var().at(ipos)>200 && tt.njets_var().at(ipos)>=7){  // FIXME
	  if(tt.njets_var().at(ipos)>=7){  // FIXME
	    h_ht.Fill(tt.ht_var().at(ipos), weight);
	    h_met.Fill(tt.met_var().at(ipos), weight);
	    h_mj.Fill(tt.mj_var().at(ipos), weight);
	    h_mt.Fill(tt.mt_var().at(ipos), weight);
	    h_njets.Fill(tt.njets_var().at(ipos), weight);
	    h_nbm.Fill(tt.nbm_var().at(ipos), weight); 

	  }

	  //Kappa calculations
	  if(tt.ht_var().at(ipos)>500 && tt.nbm_var().at(ipos)>=2 && tt.met_var().at(ipos)>200 && tt.njets_var().at(ipos)>=7){
	    
	    // Method 1
	    if(tt.met_var().at(ipos)>200 && tt.met_var().at(ipos)<=400 && tt.njets_var().at(ipos)>=7 && tt.njets_var().at(ipos)<=8){
	      if(tt.mj_var().at(ipos)<=600 && tt.mt_var().at(ipos)<=140) R1_1A += weight;
	      if(tt.mj_var().at(ipos)>600 && tt.mt_var().at(ipos)<=140) R2_1A += weight;
	      if(tt.mj_var().at(ipos)<=600 && tt.mt_var().at(ipos)>140) R3_1A += weight;
	      if(tt.mj_var().at(ipos)>600 && tt.mt_var().at(ipos)>140) R4_1A += weight;	      
	    }
	    else if(tt.met_var().at(ipos)>400 && tt.njets_var().at(ipos)>=7 && tt.njets_var().at(ipos)<=8){
	      if(tt.mj_var().at(ipos)<=600 && tt.mt_var().at(ipos)<=140) R1_1B += weight;
	      if(tt.mj_var().at(ipos)>600 && tt.mt_var().at(ipos)<=140) R2_1B += weight;
	      if(tt.mj_var().at(ipos)<=600 && tt.mt_var().at(ipos)>140) R3_1B += weight;
	      if(tt.mj_var().at(ipos)>600 && tt.mt_var().at(ipos)>140) R4_1B += weight;	      
	    }
	    else if(tt.met_var().at(ipos)>200 && tt.met_var().at(ipos)<=400 && tt.njets_var().at(ipos)>=9){
	      if(tt.mj_var().at(ipos)<=600 && tt.mt_var().at(ipos)<=140) R1_1C += weight;
	      if(tt.mj_var().at(ipos)>600 && tt.mt_var().at(ipos)<=140) R2_1C += weight;
	      if(tt.mj_var().at(ipos)<=600 && tt.mt_var().at(ipos)>140) R3_1C += weight;
	      if(tt.mj_var().at(ipos)>600 && tt.mt_var().at(ipos)>140) R4_1C += weight;	      
	    }
	    else if(tt.met_var().at(ipos)>400 && tt.njets_var().at(ipos)>=9){
	      if(tt.mj_var().at(ipos)<=600 && tt.mt_var().at(ipos)<=140) R1_1D += weight;
	      if(tt.mj_var().at(ipos)>600 && tt.mt_var().at(ipos)<=140) R2_1D += weight;
	      if(tt.mj_var().at(ipos)<=600 && tt.mt_var().at(ipos)>140) R3_1D += weight;
	      if(tt.mj_var().at(ipos)>600 && tt.mt_var().at(ipos)>140) R4_1D += weight;	      
	    }
	    // Method 2
	    if(tt.met_var().at(ipos)>200 && tt.met_var().at(ipos)<=400 && tt.njets_var().at(ipos)>=7 && tt.njets_var().at(ipos)<=8 && tt.nbm_var().at(ipos)==2){
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)<=140) R1_2A += weight;
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)<=140) R2_2A += weight;
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)>140) R3_2A += weight;
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)>140) R4_2A += weight;	      
	    }
	    else if(tt.met_var().at(ipos)>200 && tt.met_var().at(ipos)<=400 && tt.njets_var().at(ipos)>=7 && tt.njets_var().at(ipos)<=8 && tt.nbm_var().at(ipos)>=3){
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)<=140) R1_2B += weight;
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)<=140) R2_2B += weight;
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)>140) R3_2B += weight;
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)>140) R4_2B += weight;	      
	    }
	    else if(tt.met_var().at(ipos)>400 && tt.njets_var().at(ipos)>=7 && tt.njets_var().at(ipos)<=8){
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)<=140) R1_2C += weight;
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)<=140) R2_2C += weight;
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)>140) R3_2C += weight;
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)>140) R4_2C += weight;	      
	    }
	    else if(tt.met_var().at(ipos)>200 && tt.met_var().at(ipos)<=400 && tt.njets_var().at(ipos)>=9 && tt.nbm_var().at(ipos)==2){
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)<=140) R1_2A += weight; // Method 2 Binning
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)<=140) R2_2D += weight;
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)>140) R3_2A += weight;
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)>140) R4_2D += weight;	      
	    }
	    else if(tt.met_var().at(ipos)>200 && tt.met_var().at(ipos)<=400 && tt.njets_var().at(ipos)>=9 && tt.nbm_var().at(ipos)>=3){
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)<=140) R1_2B += weight;
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)<=140) R2_2E += weight;
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)>140) R3_2B += weight;
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)>140) R4_2E += weight;	      
	    }
	    else if(tt.met_var().at(ipos)>400 && tt.njets_var().at(ipos)>=9){
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)<=140) R1_2C += weight;
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)<=140) R2_2F += weight;
	      if(tt.mj_var().at(ipos)<=400 && tt.mt_var().at(ipos)>140) R3_2C += weight;
	      if(tt.mj_var().at(ipos)>400 && tt.mt_var().at(ipos)>140) R4_2F += weight;	      
	    }
	    
	  } //Kappa calculations
	} //randInt
	else{
	  if(tt.ht()>500 && tt.nbm()>=2 && tt.met()>200 && tt.njets()>=7){
	    h_ht.Fill(tt.ht(),weight);       
	    h_met.Fill(tt.met(),weight);
	    h_mj.Fill(tt.mj(),weight);
	    h_mt.Fill(tt.mt(),weight);
	    h_njets.Fill(tt.njets(),weight);
	    h_nbm.Fill(tt.nbm(), weight);
	  }

	  //Kappa calculations
	  if(tt.ht()>500 && tt.nbm()>=2 && tt.met()>200 && tt.njets()>=7){
	    
	    // Method 1
	    if(tt.met()>200 && tt.met()<=400 && tt.njets()>=7 && tt.njets()<=8){
	      if(tt.mj()<=600 && tt.mt()<=140) R1_1A += weight;
	      if(tt.mj()>600 && tt.mt()<=140) R2_1A += weight;
	      if(tt.mj()<=600 && tt.mt()>140) R3_1A += weight;
	      if(tt.mj()>600 && tt.mt()>140) R4_1A += weight;	      
	    }
	    else if(tt.met()>400 && tt.njets()>=7 && tt.njets()<=8){
	      if(tt.mj()<=600 && tt.mt()<=140) R1_1B += weight;
	      if(tt.mj()>600 && tt.mt()<=140) R2_1B += weight;
	      if(tt.mj()<=600 && tt.mt()>140) R3_1B += weight;
	      if(tt.mj()>600 && tt.mt()>140) R4_1B += weight;	      
	    }
	    else if(tt.met()>200 && tt.met()<=400 && tt.njets()>=9){
	      if(tt.mj()<=600 && tt.mt()<=140) R1_1C += weight;
	      if(tt.mj()>600 && tt.mt()<=140) R2_1C += weight;
	      if(tt.mj()<=600 && tt.mt()>140) R3_1C += weight;
	      if(tt.mj()>600 && tt.mt()>140) R4_1C += weight;	      
	    }
	    else if(tt.met()>400 && tt.njets()>=9){
	      if(tt.mj()<=600 && tt.mt()<=140) R1_1D += weight;
	      if(tt.mj()>600 && tt.mt()<=140) R2_1D += weight;
	      if(tt.mj()<=600 && tt.mt()>140) R3_1D += weight;
	      if(tt.mj()>600 && tt.mt()>140) R4_1D += weight;	      
	    }
	    // Method 2
	    if(tt.met()>200 && tt.met()<=400 && tt.njets()>=7 && tt.njets()<=8 && tt.nbm()==2){
	      if(tt.mj()<=400 && tt.mt()<=140) R1_2A += weight;
	      if(tt.mj()>400 && tt.mt()<=140) R2_2A += weight;
	      if(tt.mj()<=400 && tt.mt()>140) R3_2A += weight;
	      if(tt.mj()>400 && tt.mt()>140) R4_2A += weight;	      
	    }
	    else if(tt.met()>200 && tt.met()<=400 && tt.njets()>=7 && tt.njets()<=8 && tt.nbm()>=3){
	      if(tt.mj()<=400 && tt.mt()<=140) R1_2B += weight;
	      if(tt.mj()>400 && tt.mt()<=140) R2_2B += weight;
	      if(tt.mj()<=400 && tt.mt()>140) R3_2B += weight;
	      if(tt.mj()>400 && tt.mt()>140) R4_2B += weight;	      
	    }
	    else if(tt.met()>400 && tt.njets()>=7 && tt.njets()<=8){
	      if(tt.mj()<=400 && tt.mt()<=140) R1_2C += weight;
	      if(tt.mj()>400 && tt.mt()<=140) R2_2C += weight;
	      if(tt.mj()<=400 && tt.mt()>140) R3_2C += weight;
	      if(tt.mj()>400 && tt.mt()>140) R4_2C += weight;	      
	    }
	    else if(tt.met()>200 && tt.met()<=400 && tt.njets()>=9 && tt.nbm()==2){
	      if(tt.mj()<=400 && tt.mt()<=140) R1_2A += weight; // Method 2 Binning
	      if(tt.mj()>400 && tt.mt()<=140) R2_2D += weight;
	      if(tt.mj()<=400 && tt.mt()>140) R3_2A += weight;
	      if(tt.mj()>400 && tt.mt()>140) R4_2D += weight;	      
	    }
	    else if(tt.met()>200 && tt.met()<=400 && tt.njets()>=9 && tt.nbm()>=3){
	      if(tt.mj()<=400 && tt.mt()<=140) R1_2B += weight;
	      if(tt.mj()>400 && tt.mt()<=140) R2_2E += weight;
	      if(tt.mj()<=400 && tt.mt()>140) R3_2B += weight;
	      if(tt.mj()>400 && tt.mt()>140) R4_2E += weight;	      
	    }
	    else if(tt.met()>400 && tt.njets()>=9){
	      if(tt.mj()<=400 && tt.mt()<=140) R1_2C += weight;
	      if(tt.mj()>400 && tt.mt()<=140) R2_2F += weight;
	      if(tt.mj()<=400 && tt.mt()>140) R3_2C += weight;
	      if(tt.mj()>400 && tt.mt()>140) R4_2F += weight;	      
	    }
	  }
	} //Nominal cuts check
      } // 1 lepton
    } //entries loop    
    cout << iExp << " HT mean: "  << h_ht.GetMean() << endl; 
    h_ht_mean.Fill(h_ht.GetMean());
    h_met_mean.Fill(h_met.GetMean());
    h_njets_mean.Fill(h_njets.GetMean());
    h_nbm_mean.Fill(h_nbm.GetMean());
    h_mj_mean.Fill(h_mj.GetMean());
    h_mt_mean.Fill(h_mt.GetMean());
    
    // Method 1
    h_meth1A.Fill((R1_1A*R4_1A)/(R2_1A*R3_1A));
    h_meth1B.Fill((R1_1B*R4_1B)/(R2_1B*R3_1B));
    h_meth1C.Fill((R1_1C*R4_1C)/(R2_1C*R3_1C));
    h_meth1D.Fill((R1_1D*R4_1D)/(R2_1D*R3_1D));

    h_Rmt1A_lmj.Fill(R3_1A/R1_1A);
    h_Rmt1A_hmj.Fill(R4_1A/R2_1A);
    h_Rmt1B_lmj.Fill(R3_1B/R1_1B);
    h_Rmt1B_hmj.Fill(R4_1B/R2_1B);
    h_Rmt1C_lmj.Fill(R3_1C/R1_1C);
    h_Rmt1C_hmj.Fill(R4_1C/R2_1C);
    h_Rmt1D_lmj.Fill(R3_1D/R1_1D);
    h_Rmt1D_hmj.Fill(R4_1D/R2_1D);

    h_Rmj1A_lmt.Fill(R2_1A/R1_1A);
    h_Rmj1A_hmt.Fill(R4_1A/R3_1A);
    h_Rmj1B_lmt.Fill(R2_1B/R1_1B);
    h_Rmj1B_hmt.Fill(R4_1B/R3_1B);
    h_Rmj1C_lmt.Fill(R2_1C/R1_1C);
    h_Rmj1C_hmt.Fill(R4_1C/R3_1C);
    h_Rmj1D_lmt.Fill(R2_1D/R1_1D);
    h_Rmj1D_hmt.Fill(R4_1D/R3_1D);
    
    // Method 2
    h_meth2A.Fill((R1_2A*R4_2A)/(R2_2A*R3_2A));
    h_meth2B.Fill((R1_2B*R4_2B)/(R2_2B*R3_2B));
    h_meth2C.Fill((R1_2C*R4_2C)/(R2_2C*R3_2C));
    h_meth2D.Fill((R1_2A*R4_2D)/(R2_2D*R3_2A));
    h_meth2E.Fill((R1_2B*R4_2E)/(R2_2E*R3_2B));
    h_meth2F.Fill((R1_2C*R4_2F)/(R2_2F*R3_2C));

    h_Rmt2A_lmj.Fill(R3_2A/R1_2A);
    h_Rmt2A_hmj.Fill(R4_2A/R2_2A);
    h_Rmt2B_lmj.Fill(R3_2B/R1_2B);
    h_Rmt2B_hmj.Fill(R4_2B/R2_2B);
    h_Rmt2C_lmj.Fill(R3_2C/R1_2C);
    h_Rmt2C_hmj.Fill(R4_2C/R2_2C);
    h_Rmt2D_lmj.Fill(R3_2A/R1_2A);
    h_Rmt2D_hmj.Fill(R4_2D/R2_2D);
    h_Rmt2E_lmj.Fill(R3_2B/R1_2B);
    h_Rmt2E_hmj.Fill(R4_2E/R2_2E);
    h_Rmt2F_lmj.Fill(R3_2C/R1_2C);
    h_Rmt2F_hmj.Fill(R4_2F/R2_2F);

    h_Rmj2A_lmt.Fill(R2_2A/R1_2A);
    h_Rmj2A_hmt.Fill(R4_2A/R3_2A);
    h_Rmj2B_lmt.Fill(R2_2B/R1_2B);
    h_Rmj2B_hmt.Fill(R4_2B/R3_2B);
    h_Rmj2C_lmt.Fill(R2_2C/R1_2C);
    h_Rmj2C_hmt.Fill(R4_2C/R3_2C);
    h_Rmj2D_lmt.Fill(R2_2D/R1_2A);
    h_Rmj2D_hmt.Fill(R4_2D/R3_2A);
    h_Rmj2E_lmt.Fill(R2_2E/R1_2B);
    h_Rmj2E_hmt.Fill(R4_2E/R3_2B);
    h_Rmj2F_lmt.Fill(R2_2F/R1_2C);
    h_Rmj2F_hmt.Fill(R4_2F/R3_2C);

    h_ht.Delete();
    h_met.Delete();
    h_njets.Delete();
    h_nbm.Delete();
    h_mj.Delete();
    h_mt.Delete();
  } // experiments

  styles style("RA4"); style.setDefaultStyle();
  //  gStyle->SetOptStat("emr");
  gStyle->SetOptTitle(0);

  TLine line; line.SetLineColor(28); line.SetLineWidth(3); line.SetLineStyle(2); 
  TCanvas canHT;
  canHT.SetLogy(1);
  h_ht_mean.SetXTitle("Mean H_{T}[GeV]");
  h_ht_mean.SetYTitle("Entries/0.1 GeV");
  h_ht_mean.Draw("hist");
  line.DrawLine(941.9,0,941.9,h_ht_mean.GetMaximum());

  TString sHT = "#mu="; sHT+=RoundNumber(h_ht_mean.GetMean(),3); sHT+=" #sigma="; sHT+=RoundNumber(h_ht_mean.GetRMS(),3);
  TLegend* legHT = new TLegend(0.75, 0.8, 1, 1.0);
  legHT->AddEntry("ht_mean",sHT,"lpf");
  legHT->Draw();

  canHT.SaveAs("plots/1d/badjets/ht_mean.pdf");
  canHT.SaveAs("plots/1d/badjets/rootfiles/ht_mean.root");

  TCanvas canMET;
  canMET.SetLogy(1);
  h_met_mean.SetXTitle("MET [GeV]");
  h_met_mean.SetYTitle("Entries/0.1 GeV");
  h_met_mean.Draw("hist");
  line.DrawLine(272.6,0,272.6,h_met_mean.GetMaximum());

  TString sMET = "#mu="; sMET+=RoundNumber(h_met_mean.GetMean(),3); sMET+=" #sigma="; sMET+=RoundNumber(h_met_mean.GetRMS(),3);
  TLegend* legMET = new TLegend(0.75, 0.8, 1, 1.0);
  legMET->AddEntry("met_mean",sMET,"lpf");
  legMET->Draw();

  canMET.SaveAs("plots/1d/badjets/met_mean.pdf");
  canMET.SaveAs("plots/1d/badjets/rootfiles/met_mean.root");

  TCanvas canNjets;
  canNjets.SetLogy(1);
  h_njets_mean.SetXTitle("N_{jets}");
  h_njets_mean.SetYTitle("Entries/0.1");
  h_njets_mean.Draw("hist");
  line.DrawLine(7.632,0,7.632,h_njets_mean.GetMaximum());

  TString snjets = "#mu="; snjets+=RoundNumber(h_njets_mean.GetMean(),3); snjets+=" #sigma="; snjets+=RoundNumber(h_njets_mean.GetRMS(),3);
  TLegend* legnjets = new TLegend(0.75, 0.8, 1, 1.0);
  legnjets->AddEntry("njets_mean",snjets,"lpf");
  legnjets->Draw();

  canNjets.SaveAs("plots/1d/badjets/njets_mean.pdf");
  canNjets.SaveAs("plots/1d/badjets/rootfiles/njets_mean.root");

  TCanvas canNbm;
  canNbm.SetLogy(1);
  h_nbm_mean.SetXTitle("N_{b}");
  h_nbm_mean.SetYTitle("Entries/0.1");
  h_nbm_mean.Draw("hist");
  line.DrawLine(2.212,0,2.212,h_nbm_mean.GetMaximum());

  TString sNBM = "#mu="; sNBM+=RoundNumber(h_nbm_mean.GetMean(),3); sNBM+=" #sigma="; sNBM+=RoundNumber(h_nbm_mean.GetRMS(),3);
  TLegend* legNBM = new TLegend(0.75, 0.8, 1, 1.0);
  legNBM->AddEntry("nbm_mean",sNBM,"lpf");
  legNBM->Draw();

  canNbm.SaveAs("plots/1d/badjets/nbm_mean.pdf");
  canNbm.SaveAs("plots/1d/badjets/rootfiles/nbm_mean.root");

  TCanvas canMJ;
  canMJ.SetLogy(1);
  h_mj_mean.SetXTitle("M_{J} [GeV]");
  h_mj_mean.SetYTitle("Entries/0.1 GeV");
  h_mj_mean.Draw("hist");
  line.DrawLine(371.1,0,371.1,h_mj_mean.GetMaximum());

  TString sMJ = "#mu="; sMJ+=RoundNumber(h_mj_mean.GetMean(),3); sMJ+=" #sigma="; sMJ+=RoundNumber(h_mj_mean.GetRMS(),3);
  TLegend* legMJ = new TLegend(0.75, 0.8, 1, 1.0);
  legMJ->AddEntry("mj_mean",sMJ,"lpf");
  legMJ->Draw();

  canMJ.SaveAs("plots/1d/badjets/mj_mean.pdf");
  canMJ.SaveAs("plots/1d/badjets/rootfiles/mj_mean.root");

  TCanvas canMT;
  canMT.SetLogy(1);
  h_mt_mean.SetXTitle("m_{T} [GeV]");
  h_mt_mean.SetYTitle("Entries/0.1 GeV");
  h_mt_mean.Draw("hist");
  line.DrawLine(68.26,0,68.26,h_mt_mean.GetMaximum());

  TString sMT = "#mu="; sMT+=RoundNumber(h_mt_mean.GetMean(),3); sMT+=" #sigma="; sMT+=RoundNumber(h_mt_mean.GetRMS(),3);
  TLegend* legMT = new TLegend(0.75, 0.8, 1, 1.0);
  legMT->AddEntry("mt_mean",sMT,"lpf");
  legMT->Draw();

  canMT.SaveAs("plots/1d/badjets/mt_mean.pdf");
  canMT.SaveAs("plots/1d/badjets/rootfiles/mt_mean.root");

  TCanvas can1A;
  can1A.SetLogy(1);
  h_meth1A.SetXTitle("#Kappa");
  h_meth1A.SetYTitle("Entries/0.01");
  h_meth1A.Draw("hist");
  line.DrawLine(1.23,0,1.23,h_meth1A.GetMaximum());

  TString s1A = "#mu="; s1A+=RoundNumber(h_meth1A.GetMean(),3); s1A+=" #sigma="; s1A+=RoundNumber(h_meth1A.GetRMS(),3);
  TLegend* leg1A = new TLegend(0.75, 0.8, 1, 1.0);
  leg1A->AddEntry("meth1A",s1A,"lpf");
  leg1A->Draw();

  can1A.SaveAs("plots/1d/badjets/kappa_meth1A.pdf");
  can1A.SaveAs("plots/1d/badjets/rootfiles/kappa_meth1A.root");

  TCanvas can1B;
  can1B.SetLogy(1);
  h_meth1B.SetXTitle("#Kappa");
  h_meth1B.SetYTitle("Entries/0.01");
  h_meth1B.Draw("hist");
  line.DrawLine(1.47,0,1.47,h_meth1B.GetMaximum());

  TString s1B = "#mu="; s1B+=RoundNumber(h_meth1B.GetMean(),3); s1B+=" #sigma="; s1B+=RoundNumber(h_meth1B.GetRMS(),3);
  TLegend* leg1B = new TLegend(0.75, 0.8, 1, 1.0);
  leg1B->AddEntry("meth1B",s1B,"lpf");
  leg1B->Draw();

  can1B.SaveAs("plots/1d/badjets/kappa_meth1B.pdf");
  can1B.SaveAs("plots/1d/badjets/rootfiles/kappa_meth1B.root");

  TCanvas can1C;
  can1C.SetLogy(1);
  h_meth1C.SetXTitle("#Kappa");
  h_meth1C.SetYTitle("Entries/0.01");
  h_meth1C.Draw("hist");
  line.DrawLine(1.30,0,1.30,h_meth1C.GetMaximum());

  TString s1C = "#mu="; s1C+=RoundNumber(h_meth1C.GetMean(),3); s1C+=" #sigma="; s1C+=RoundNumber(h_meth1C.GetRMS(),3);
  TLegend* leg1C = new TLegend(0.75, 0.8, 1, 1.0);
  leg1C->AddEntry("meth1C",s1C,"lpf");
  leg1C->Draw();

  can1C.SaveAs("plots/1d/badjets/kappa_meth1C.pdf");
  can1C.SaveAs("plots/1d/badjets/rootfiles/kappa_meth1C.root");

  TCanvas can1D;
  can1D.SetLogy(1);
  h_meth1D.SetXTitle("#Kappa");
  h_meth1D.SetYTitle("Entries/0.01");
  h_meth1D.Draw("hist");
  line.DrawLine(1.35,0,1.35,h_meth1D.GetMaximum());

  TString s1D = "#mu="; s1D+=RoundNumber(h_meth1D.GetMean(),3); s1D+=" #sigma="; s1D+=RoundNumber(h_meth1D.GetRMS(),3);
  TLegend* leg1D = new TLegend(0.75, 0.8, 1, 1.0);
  leg1D->AddEntry("meth1D",s1D,"lpf");
  leg1D->Draw();

  can1D.SaveAs("plots/1d/badjets/kappa_meth1D.pdf");
  can1D.SaveAs("plots/1d/badjets/rootfiles/kappa_meth1D.root");

  TCanvas canRmt1A;
  canRmt1A.SetLogy(1);
  h_Rmt1A_lmj.SetLineColor(46);
  h_Rmt1A_lmj.SetXTitle("R_{m_{T}}");
  h_Rmt1A_lmj.SetYTitle("Entries/0.001");  
  h_Rmt1A_lmj.Draw("hist");
  h_Rmt1A_hmj.SetLineColor(31);
  h_Rmt1A_hmj.Draw("hist sames");
  line.DrawLine(0.076,0,0.076,h_Rmt1A_lmj.GetMaximum());
  line.DrawLine(0.094,0,0.094,h_Rmt1A_hmj.GetMaximum());

  TString slegRmt1A_lmj = "#mu="; slegRmt1A_lmj+=RoundNumber(h_Rmt1A_lmj.GetMean(),3); slegRmt1A_lmj+=" #sigma="; slegRmt1A_lmj+=RoundNumber(h_Rmt1A_lmj.GetRMS(),3);
  TString slegRmt1A_hmj = "#mu="; slegRmt1A_hmj+=RoundNumber(h_Rmt1A_hmj.GetMean(),3); slegRmt1A_hmj+=" #sigma="; slegRmt1A_hmj+=RoundNumber(h_Rmt1A_hmj.GetRMS(),3);
  TLegend* legRmt1A = new TLegend(0.75, 0.8, 1, 1.0);
  legRmt1A->AddEntry("Rmt1A_lmj",slegRmt1A_lmj,"lpf");
  legRmt1A->AddEntry("Rmt1A_hmj",slegRmt1A_hmj,"lpf");
  legRmt1A->Draw();

  canRmt1A.SaveAs("plots/1d/badjets/Rmt1A.pdf");
  canRmt1A.SaveAs("plots/1d/badjets/rootfiles/Rmt1A.root");

  TCanvas canRmt1B;
  canRmt1B.SetLogy(1);
  h_Rmt1B_lmj.SetLineColor(46);
  h_Rmt1B_lmj.SetXTitle("R_{m_{T}}");
  h_Rmt1B_lmj.SetYTitle("Entries/0.001");
  h_Rmt1B_hmj.SetLineColor(31);
  h_Rmt1B_hmj.Draw("hist");
  h_Rmt1B_lmj.Draw("hist sames");
  line.DrawLine(0.086,0,0.086,h_Rmt1B_lmj.GetMaximum());
  line.DrawLine(0.127,0,0.127,h_Rmt1B_hmj.GetMaximum());

  TString slegRmt1B_lmj = "#mu="; slegRmt1B_lmj+=RoundNumber(h_Rmt1B_lmj.GetMean(),3); slegRmt1B_lmj+=" #sigma="; slegRmt1B_lmj+=RoundNumber(h_Rmt1B_lmj.GetRMS(),3);
  TString slegRmt1B_hmj = "#mu="; slegRmt1B_hmj+=RoundNumber(h_Rmt1B_hmj.GetMean(),3); slegRmt1B_hmj+=" #sigma="; slegRmt1B_hmj+=RoundNumber(h_Rmt1B_hmj.GetRMS(),3);
  TLegend* legRmt1B = new TLegend(0.75, 0.8, 1, 1.0);
  legRmt1B->AddEntry("Rmt1B_lmj",slegRmt1B_lmj,"lpf");
  legRmt1B->AddEntry("Rmt1B_hmj",slegRmt1B_hmj,"lpf");
  legRmt1B->Draw();

  canRmt1B.SaveAs("plots/1d/badjets/Rmt1B.pdf");
  canRmt1B.SaveAs("plots/1d/badjets/rootfiles/Rmt1B.root");

  TCanvas canRmt1C;
  canRmt1C.SetLogy(1);
  h_Rmt1C_lmj.SetLineColor(46);
  h_Rmt1C_lmj.SetXTitle("R_{m_{T}}");
  h_Rmt1C_lmj.SetYTitle("Entries/0.001");
  h_Rmt1C_lmj.Draw("hist");
  h_Rmt1C_hmj.SetLineColor(31);
  h_Rmt1C_hmj.Draw("hist sames");
  line.DrawLine(0.066,0,0.066,h_Rmt1C_lmj.GetMaximum());
  line.DrawLine(0.086,0,0.086,h_Rmt1C_hmj.GetMaximum());


  TString slegRmt1C_lmj = "#mu="; slegRmt1C_lmj+=RoundNumber(h_Rmt1C_lmj.GetMean(),3); slegRmt1C_lmj+=" #sigma="; slegRmt1C_lmj+=RoundNumber(h_Rmt1C_lmj.GetRMS(),3);
  TString slegRmt1C_hmj = "#mu="; slegRmt1C_hmj+=RoundNumber(h_Rmt1C_hmj.GetMean(),3); slegRmt1C_hmj+=" #sigma="; slegRmt1C_hmj+=RoundNumber(h_Rmt1C_hmj.GetRMS(),3);
  TLegend* legRmt1C = new TLegend(0.75, 0.8, 1, 1.0);
  legRmt1C->AddEntry("Rmt1C_lmj",slegRmt1C_lmj,"lpf");
  legRmt1C->AddEntry("Rmt1C_hmj",slegRmt1C_hmj,"lpf");
  legRmt1C->Draw();

  canRmt1C.SaveAs("plots/1d/badjets/Rmt1C.pdf");
  canRmt1C.SaveAs("plots/1d/badjets/rootfiles/Rmt1C.root");

  TCanvas canRmt1D;
  canRmt1D.SetLogy(1);
  h_Rmt1D_lmj.SetLineColor(46);
  h_Rmt1D_lmj.SetXTitle("R_{m_{T}}");
  h_Rmt1D_lmj.SetYTitle("Entries/0.001");
  h_Rmt1D_lmj.Draw("hist");
  h_Rmt1D_hmj.SetLineColor(31);
  h_Rmt1D_hmj.Draw("hist sames");
  line.DrawLine(0.069,0,0.069,h_Rmt1D_lmj.GetMaximum());
  line.DrawLine(0.933,0,0.933,h_Rmt1D_hmj.GetMaximum());


  TString slegRmt1D_lmj = "#mu="; slegRmt1D_lmj+=RoundNumber(h_Rmt1D_lmj.GetMean(),3); slegRmt1D_lmj+=" #sigma="; slegRmt1D_lmj+=RoundNumber(h_Rmt1D_lmj.GetRMS(),3);
  TString slegRmt1D_hmj = "#mu="; slegRmt1D_hmj+=RoundNumber(h_Rmt1D_hmj.GetMean(),3); slegRmt1D_hmj+=" #sigma="; slegRmt1D_hmj+=RoundNumber(h_Rmt1D_hmj.GetRMS(),3);
  TLegend* legRmt1D = new TLegend(0.75, 0.8, 1, 1.0);
  legRmt1D->AddEntry("Rmt1D_lmj",slegRmt1D_lmj,"lpf");
  legRmt1D->AddEntry("Rmt1D_hmj",slegRmt1D_hmj,"lpf");
  legRmt1D->Draw();

  canRmt1D.SaveAs("plots/1d/badjets/Rmt1D.pdf");
  canRmt1D.SaveAs("plots/1d/badjets/rootfiles/Rmt1D.root");

  TCanvas canRmj1A;
  canRmj1A.SetLogy(1);
  h_Rmj1A_lmt.SetLineColor(46);
  h_Rmj1A_lmt.SetXTitle("R_{m_{T}}");
  h_Rmj1A_lmt.SetYTitle("Entries/0.001");  
  h_Rmj1A_lmt.Draw("hist");
  h_Rmj1A_hmt.SetLineColor(31);
  h_Rmj1A_hmt.Draw("hist sames");
  line.DrawLine(0.041,0,0.041,h_Rmj1A_lmt.GetMaximum());
  line.DrawLine(0.051,0,0.051,h_Rmj1A_hmt.GetMaximum());

  TString slegRmj1A_lmt = "#mu="; slegRmj1A_lmt+=RoundNumber(h_Rmj1A_lmt.GetMean(),3); slegRmj1A_lmt+=" #sigma="; slegRmj1A_lmt+=RoundNumber(h_Rmj1A_lmt.GetRMS(),3);
  TString slegRmj1A_hmt = "#mu="; slegRmj1A_hmt+=RoundNumber(h_Rmj1A_hmt.GetMean(),3); slegRmj1A_hmt+=" #sigma="; slegRmj1A_hmt+=RoundNumber(h_Rmj1A_hmt.GetRMS(),3);
  TLegend* legRmj1A = new TLegend(0.75, 0.8, 1, 1.0);
  legRmj1A->AddEntry("Rmj1A_lmt",slegRmj1A_lmt,"lpf");
  legRmj1A->AddEntry("Rmj1A_hmt",slegRmj1A_hmt,"lpf");
  legRmj1A->Draw();

  canRmj1A.SaveAs("plots/1d/badjets/Rmj1A.pdf");
  canRmj1A.SaveAs("plots/1d/badjets/rootfiles/Rmj1A.root");

  TCanvas canRmj1B;
  canRmj1B.SetLogy(1);
  h_Rmj1B_lmt.SetLineColor(46);
  h_Rmj1B_lmt.SetXTitle("R_{m_{T}}");
  h_Rmj1B_lmt.SetYTitle("Entries/0.001");
  h_Rmj1B_lmt.Draw("hist");
  h_Rmj1B_hmt.SetLineColor(31);
  h_Rmj1B_hmt.Draw("hist sames");
  line.DrawLine(0.141,0,0.141,h_Rmj1B_lmt.GetMaximum());
  line.DrawLine(0.207,0,0.207,h_Rmj1B_hmt.GetMaximum());

  TString slegRmj1B_lmt = "#mu="; slegRmj1B_lmt+=RoundNumber(h_Rmj1B_lmt.GetMean(),3); slegRmj1B_lmt+=" #sigma="; slegRmj1B_lmt+=RoundNumber(h_Rmj1B_lmt.GetRMS(),3);
  TString slegRmj1B_hmt = "#mu="; slegRmj1B_hmt+=RoundNumber(h_Rmj1B_hmt.GetMean(),3); slegRmj1B_hmt+=" #sigma="; slegRmj1B_hmt+=RoundNumber(h_Rmj1B_hmt.GetRMS(),3);
  TLegend* legRmj1B = new TLegend(0.75, 0.8, 1, 1.0);
  legRmj1B->AddEntry("Rmj1B_lmt",slegRmj1B_lmt,"lpf");
  legRmj1B->AddEntry("Rmj1B_hmt",slegRmj1B_hmt,"lpf");
  legRmj1B->Draw();

  canRmj1B.SaveAs("plots/1d/badjets/Rmj1B.pdf");
  canRmj1B.SaveAs("plots/1d/badjets/rootfiles/Rmj1B.root");

  TCanvas canRmj1C;
  canRmj1C.SetLogy(1);
  h_Rmj1C_lmt.SetLineColor(46);
  h_Rmj1C_lmt.SetXTitle("R_{m_{T}}");
  h_Rmj1C_lmt.SetYTitle("Entries/0.001");
  h_Rmj1C_lmt.Draw("hist");
  h_Rmj1C_hmt.SetLineColor(31);
  h_Rmj1C_hmt.Draw("hist sames");
  line.DrawLine(0.210,0,0.210,h_Rmj1C_lmt.GetMaximum());
  line.DrawLine(0.274,0,0.274,h_Rmj1C_hmt.GetMaximum());

  TString slegRmj1C_lmt = "#mu="; slegRmj1C_lmt+=RoundNumber(h_Rmj1C_lmt.GetMean(),3); slegRmj1C_lmt+=" #sigma="; slegRmj1C_lmt+=RoundNumber(h_Rmj1C_lmt.GetRMS(),3);
  TString slegRmj1C_hmt = "#mu="; slegRmj1C_hmt+=RoundNumber(h_Rmj1C_hmt.GetMean(),3); slegRmj1C_hmt+=" #sigma="; slegRmj1C_hmt+=RoundNumber(h_Rmj1C_hmt.GetRMS(),3);
  TLegend* legRmj1C = new TLegend(0.75, 0.8, 1, 1.0);
  legRmj1C->AddEntry("Rmj1C_lmt",slegRmj1C_lmt,"lpf");
  legRmj1C->AddEntry("Rmj1C_hmt",slegRmj1C_hmt,"lpf");
  legRmj1C->Draw();

  canRmj1C.SaveAs("plots/1d/badjets/Rmj1C.pdf");
  canRmj1C.SaveAs("plots/1d/badjets/rootfiles/Rmj1C.root");

  TCanvas canRmj1D;
  canRmj1D.SetLogy(1);
  h_Rmj1D_lmt.SetLineColor(46);
  h_Rmj1D_lmt.SetXTitle("R_{m_{T}}");
  h_Rmj1D_lmt.SetYTitle("Entries/0.001");
  h_Rmj1D_lmt.Draw("hist");
  h_Rmj1D_hmt.SetLineColor(31);
  h_Rmj1D_hmt.Draw("hist sames");
  line.DrawLine(0.602,0,0.602,h_Rmj1D_lmt.GetMaximum());
  line.DrawLine(0.816,0,0.816,h_Rmj1D_hmt.GetMaximum());

  TString slegRmj1D_lmt = "#mu="; slegRmj1D_lmt+=RoundNumber(h_Rmj1D_lmt.GetMean(),3); slegRmj1D_lmt+=" #sigma="; slegRmj1D_lmt+=RoundNumber(h_Rmj1D_lmt.GetRMS(),3);
  TString slegRmj1D_hmt = "#mu="; slegRmj1D_hmt+=RoundNumber(h_Rmj1D_hmt.GetMean(),3); slegRmj1D_hmt+=" #sigma="; slegRmj1D_hmt+=RoundNumber(h_Rmj1D_hmt.GetRMS(),3);
  TLegend* legRmj1D = new TLegend(0.75, 0.8, 1, 1.0);
  legRmj1D->AddEntry("Rmj1D_lmt",slegRmj1D_lmt,"lpf");
  legRmj1D->AddEntry("Rmj1D_hmt",slegRmj1D_hmt,"lpf");
  legRmj1D->Draw();

  canRmj1D.SaveAs("plots/1d/badjets/Rmj1D.pdf");
  canRmj1D.SaveAs("plots/1d/badjets/rootfiles/Rmj1D.root");

  TCanvas can2A;
  can2A.SetLogy(1);
  h_meth2A.SetXTitle("#Kappa");
  h_meth2A.SetYTitle("Entries/0.01");
  h_meth2A.Draw("hist");
  line.DrawLine(1.09,0,1.09,h_meth2A.GetMaximum());

  TString s2A = "#mu="; s2A+=RoundNumber(h_meth2A.GetMean(),3); s2A+=" #sigma="; s2A+=RoundNumber(h_meth2A.GetRMS(),3);
  TLegend* leg2A = new TLegend(0.75, 0.8, 1, 1.0);
  leg2A->AddEntry("meth2A",s2A,"lpf");
  leg2A->Draw();

  can2A.SaveAs("plots/1d/badjets/kappa_meth2A.pdf");
  can2A.SaveAs("plots/1d/badjets/rootfiles/kappa_meth2A.root");

  TCanvas can2B;
  can2B.SetLogy(1);
  h_meth2B.SetXTitle("#Kappa");
  h_meth2B.SetYTitle("Entries/0.01");
  h_meth2B.Draw("hist");
  line.DrawLine(1.13,0,1.13,h_meth2B.GetMaximum());

  TString s2B = "#mu="; s2B+=RoundNumber(h_meth2B.GetMean(),3); s2B+=" #sigma="; s2B+=RoundNumber(h_meth2B.GetRMS(),3);
  TLegend* leg2B = new TLegend(0.75, 0.8, 1, 1.0);
  leg2B->AddEntry("meth2B",s2B,"lpf");
  leg2B->Draw();

  can2B.SaveAs("plots/1d/badjets/kappa_meth2B.pdf");
  can2B.SaveAs("plots/1d/badjets/rootfiles/kappa_meth2B.root");

  TCanvas can2C;
  can2C.SetLogy(1);
  h_meth2C.SetXTitle("#Kappa");
  h_meth2C.SetYTitle("Entries/0.01");
  h_meth2C.Draw("hist");
  line.DrawLine(1.05,0,1.05,h_meth2C.GetMaximum());

  TString s2C = "#mu="; s2C+=RoundNumber(h_meth2C.GetMean(),3); s2C+=" #sigma="; s2C+=RoundNumber(h_meth2C.GetRMS(),3);
  TLegend* leg2C = new TLegend(0.75, 0.8, 1, 1.0);
  leg2C->AddEntry("meth2C",s2C,"lpf");
  leg2C->Draw();

  can2C.SaveAs("plots/1d/badjets/kappa_meth2C.pdf");
  can2C.SaveAs("plots/1d/badjets/rootfiles/kappa_meth2C.root");

  TCanvas can2D;
  can2D.SetLogy(1);
  h_meth2D.SetXTitle("#Kappa");
  h_meth2D.SetYTitle("Entries/0.01");
  h_meth2D.Draw("hist");
  line.DrawLine(0.92,0,0.92,h_meth2D.GetMaximum());

  TString s2D = "#mu="; s2D+=RoundNumber(h_meth2D.GetMean(),3); s2D+=" #sigma="; s2D+=RoundNumber(h_meth2D.GetRMS(),3);
  TLegend* leg2D = new TLegend(0.75, 0.8, 1, 1.0);
  leg2D->AddEntry("meth2D",s2D,"lpf");
  leg2D->Draw();

  can2D.SaveAs("plots/1d/badjets/kappa_meth2D.pdf");
  can2D.SaveAs("plots/1d/badjets/rootfiles/kappa_meth2D.root");

  TCanvas can2E;
  can2E.SetLogy(1);
  h_meth2E.SetXTitle("#Kappa");
  h_meth2E.SetYTitle("Entries/0.01");
  h_meth2E.Draw("hist");
  line.DrawLine(0.99,0,0.99,h_meth2E.GetMaximum());

  TString s2E = "#mu="; s2E+=RoundNumber(h_meth2E.GetMean(),3); s2E+=" #sigma="; s2E+=RoundNumber(h_meth2E.GetRMS(),3);
  TLegend* leg2E = new TLegend(0.75, 0.8, 1, 1.0);
  leg2E->AddEntry("meth2E",s2E,"lpf");
  leg2E->Draw();

  can2E.SaveAs("plots/1d/badjets/kappa_meth2E.pdf");
  can2E.SaveAs("plots/1d/badjets/rootfiles/kappa_meth2E.root");

  TCanvas can2F;
  can2F.SetLogy(1);
  h_meth2F.SetXTitle("#Kappa");
  h_meth2F.SetYTitle("Entries/0.01");
  h_meth2F.Draw("hist");
  line.DrawLine(0.91,0,0.91,h_meth2F.GetMaximum());

  TString s2F = "#mu="; s2F+=RoundNumber(h_meth2F.GetMean(),3); s2F+=" #sigma="; s2F+=RoundNumber(h_meth2F.GetRMS(),3);
  TLegend* leg2F = new TLegend(0.75, 0.8, 1, 1.0);
  leg2F->AddEntry("meth2F",s2F,"lpf");
  leg2F->Draw();

  can2F.SaveAs("plots/1d/badjets/kappa_meth2F.pdf");
  can2F.SaveAs("plots/1d/badjets/rootfiles/kappa_meth2F.root");

  TCanvas canRmt2A;
  canRmt2A.SetLogy(1);
  h_Rmt2A_lmj.SetLineColor(46);
  h_Rmt2A_lmj.SetXTitle("R_{m_{T}}");
  h_Rmt2A_lmj.SetYTitle("Entries/0.001");  
  h_Rmt2A_lmj.Draw("hist");
  h_Rmt2A_hmj.SetLineColor(31);
  h_Rmt2A_hmj.Draw("hist sames");
  line.DrawLine(0.074,0,0.074,h_Rmt2A_lmj.GetMaximum());
  line.DrawLine(0.081,0,0.081,h_Rmt2A_hmj.GetMaximum());

  TString slegRmt2A_lmj = "#mu="; slegRmt2A_lmj+=RoundNumber(h_Rmt2A_lmj.GetMean(),3); slegRmt2A_lmj+=" #sigma="; slegRmt2A_lmj+=RoundNumber(h_Rmt2A_lmj.GetRMS(),3);
  TString slegRmt2A_hmj = "#mu="; slegRmt2A_hmj+=RoundNumber(h_Rmt2A_hmj.GetMean(),3); slegRmt2A_hmj+=" #sigma="; slegRmt2A_hmj+=RoundNumber(h_Rmt2A_hmj.GetRMS(),3);
  TLegend* legRmt2A = new TLegend(0.75, 0.8, 1, 1.0);
  legRmt2A->AddEntry("Rmt2A_lmj",slegRmt2A_lmj,"lpf");
  legRmt2A->AddEntry("Rmt2A_hmj",slegRmt2A_hmj,"lpf");
  legRmt2A->Draw();

  canRmt2A.SaveAs("plots/1d/badjets/Rmt2A.pdf");
  canRmt2A.SaveAs("plots/1d/badjets/rootfiles/Rmt2A.root");

  TCanvas canRmt2B;
  canRmt2B.SetLogy(1);
  h_Rmt2B_lmj.SetLineColor(46);
  h_Rmt2B_lmj.SetXTitle("R_{m_{T}}");
  h_Rmt2B_lmj.SetYTitle("Entries/0.001");
  h_Rmt2B_lmj.Draw("hist");
  h_Rmt2B_hmj.SetLineColor(31);
  h_Rmt2B_hmj.Draw("hist sames");
  line.DrawLine(0.076,0,0.076,h_Rmt2B_lmj.GetMaximum());
  line.DrawLine(0.086,0,0.860,h_Rmt2B_hmj.GetMaximum());

  TString slegRmt2B_lmj = "#mu="; slegRmt2B_lmj+=RoundNumber(h_Rmt2B_lmj.GetMean(),3); slegRmt2B_lmj+=" #sigma="; slegRmt2B_lmj+=RoundNumber(h_Rmt2B_lmj.GetRMS(),3);
  TString slegRmt2B_hmj = "#mu="; slegRmt2B_hmj+=RoundNumber(h_Rmt2B_hmj.GetMean(),3); slegRmt2B_hmj+=" #sigma="; slegRmt2B_hmj+=RoundNumber(h_Rmt2B_hmj.GetRMS(),3);
  TLegend* legRmt2B = new TLegend(0.75, 0.8, 1, 1.0);
  legRmt2B->AddEntry("Rmt2B_lmj",slegRmt2B_lmj,"lpf");
  legRmt2B->AddEntry("Rmt2B_hmj",slegRmt2B_hmj,"lpf");
  legRmt2B->Draw();

  canRmt2B.SaveAs("plots/1d/badjets/Rmt2B.pdf");
  canRmt2B.SaveAs("plots/1d/badjets/rootfiles/Rmt2B.root");

  TCanvas canRmt2C;
  canRmt2C.SetLogy(1);
  h_Rmt2C_lmj.SetLineColor(46);
  h_Rmt2C_lmj.SetXTitle("R_{m_{T}}");
  h_Rmt2C_lmj.SetYTitle("Entries/0.001");
  h_Rmt2C_lmj.Draw("hist");
  h_Rmt2C_hmj.SetLineColor(31);
  h_Rmt2C_hmj.Draw("hist sames");
  line.DrawLine(0.088,0,0.088,h_Rmt2C_lmj.GetMaximum());
  line.DrawLine(0.093,0,0.093,h_Rmt2C_hmj.GetMaximum());

  TString slegRmt2C_lmj = "#mu="; slegRmt2C_lmj+=RoundNumber(h_Rmt2C_lmj.GetMean(),3); slegRmt2C_lmj+=" #sigma="; slegRmt2C_lmj+=RoundNumber(h_Rmt2C_lmj.GetRMS(),3);
  TString slegRmt2C_hmj = "#mu="; slegRmt2C_hmj+=RoundNumber(h_Rmt2C_hmj.GetMean(),3); slegRmt2C_hmj+=" #sigma="; slegRmt2C_hmj+=RoundNumber(h_Rmt2C_hmj.GetRMS(),3);
  TLegend* legRmt2C = new TLegend(0.75, 0.8, 1, 1.0);
  legRmt2C->AddEntry("Rmt2C_lmj",slegRmt2C_lmj,"lpf");
  legRmt2C->AddEntry("Rmt2C_hmj",slegRmt2C_hmj,"lpf");
  legRmt2C->Draw();

  canRmt2C.SaveAs("plots/1d/badjets/Rmt2C.pdf");
  canRmt2C.SaveAs("plots/1d/badjets/rootfiles/Rmt2C.root");

  TCanvas canRmt2D;
  canRmt2D.SetLogy(1);
  h_Rmt2D_lmj.SetLineColor(46);
  h_Rmt2D_lmj.SetXTitle("R_{m_{T}}");
  h_Rmt2D_lmj.SetYTitle("Entries/0.001");
  h_Rmt2D_lmj.Draw("hist");
  h_Rmt2D_hmj.SetLineColor(31);
  h_Rmt2D_hmj.Draw("hist sames");
  line.DrawLine(0.074,0,0.074,h_Rmt2D_lmj.GetMaximum());
  line.DrawLine(0.068,0,0.068,h_Rmt2D_hmj.GetMaximum());

  TString slegRmt2D_lmj = "#mu="; slegRmt2D_lmj+=RoundNumber(h_Rmt2D_lmj.GetMean(),3); slegRmt2D_lmj+=" #sigma="; slegRmt2D_lmj+=RoundNumber(h_Rmt2D_lmj.GetRMS(),3);
  TString slegRmt2D_hmj = "#mu="; slegRmt2D_hmj+=RoundNumber(h_Rmt2D_hmj.GetMean(),3); slegRmt2D_hmj+=" #sigma="; slegRmt2D_hmj+=RoundNumber(h_Rmt2D_hmj.GetRMS(),3);
  TLegend* legRmt2D = new TLegend(0.75, 0.8, 1, 1.0);
  legRmt2D->AddEntry("Rmt2D_lmj",slegRmt2D_lmj,"lpf");
  legRmt2D->AddEntry("Rmt2D_hmj",slegRmt2D_hmj,"lpf");
  legRmt2D->Draw();

  canRmt2D.SaveAs("plots/1d/badjets/Rmt2D.pdf");
  canRmt2D.SaveAs("plots/1d/badjets/rootfiles/Rmt2D.root");

  TCanvas canRmt2E;
  canRmt2E.SetLogy(1);
  h_Rmt2E_lmj.SetLineColor(46);
  h_Rmt2E_lmj.SetXTitle("R_{m_{T}}");
  h_Rmt2E_lmj.SetYTitle("Entries/0.001");
  h_Rmt2E_lmj.Draw("hist");
  h_Rmt2E_hmj.SetLineColor(31);
  h_Rmt2E_hmj.Draw("hist sames");
  line.DrawLine(0.076,0,0.076,h_Rmt2D_lmj.GetMaximum());
  line.DrawLine(0.076,0,0.076,h_Rmt2D_hmj.GetMaximum());

  TString slegRmt2E_lmj = "#mu="; slegRmt2E_lmj+=RoundNumber(h_Rmt2E_lmj.GetMean(),3); slegRmt2E_lmj+=" #sigma="; slegRmt2E_lmj+=RoundNumber(h_Rmt2E_lmj.GetRMS(),3);
  TString slegRmt2E_hmj = "#mu="; slegRmt2E_hmj+=RoundNumber(h_Rmt2E_hmj.GetMean(),3); slegRmt2E_hmj+=" #sigma="; slegRmt2E_hmj+=RoundNumber(h_Rmt2E_hmj.GetRMS(),3);
  TLegend* legRmt2E = new TLegend(0.75, 0.8, 1, 1.0);
  legRmt2E->AddEntry("Rmt2E_lmj",slegRmt2E_lmj,"lpf");
  legRmt2E->AddEntry("Rmt2E_hmj",slegRmt2E_hmj,"lpf");
  legRmt2E->Draw();

  canRmt2E.SaveAs("plots/1d/badjets/Rmt2E.pdf");
  canRmt2E.SaveAs("plots/1d/badjets/rootfiles/Rmt2E.root");

  TCanvas canRmt2F;
  canRmt2F.SetLogy(1);
  h_Rmt2F_lmj.SetLineColor(46);
  h_Rmt2F_lmj.SetXTitle("R_{m_{T}}");
  h_Rmt2F_lmj.SetYTitle("Entries/0.001");
  h_Rmt2F_hmj.SetLineColor(31);
  h_Rmt2F_hmj.Draw("hist");
  h_Rmt2F_lmj.Draw("hist sames");
  line.DrawLine(0.088,0,0.088,h_Rmt2F_lmj.GetMaximum());
  line.DrawLine(0.081,0,0.081,h_Rmt2F_hmj.GetMaximum());

  TString slegRmt2F_lmj = "#mu="; slegRmt2F_lmj+=RoundNumber(h_Rmt2F_lmj.GetMean(),3); slegRmt2F_lmj+=" #sigma="; slegRmt2F_lmj+=RoundNumber(h_Rmt2F_lmj.GetRMS(),3);
  TString slegRmt2F_hmj = "#mu="; slegRmt2F_hmj+=RoundNumber(h_Rmt2F_hmj.GetMean(),3); slegRmt2F_hmj+=" #sigma="; slegRmt2F_hmj+=RoundNumber(h_Rmt2F_hmj.GetRMS(),3);
  TLegend* legRmt2F = new TLegend(0.75, 0.8, 1, 1.0);
  legRmt2F->AddEntry("Rmt2F_lmj",slegRmt2F_lmj,"lpf");
  legRmt2F->AddEntry("Rmt2F_hmj",slegRmt2F_hmj,"lpf");
  legRmt2F->Draw();

  canRmt2F.SaveAs("plots/1d/badjets/Rmt2F.pdf");
  canRmt2F.SaveAs("plots/1d/badjets/rootfiles/Rmt2F.root");

  TCanvas canRmj2A;
  canRmj2A.SetLogy(1);
  h_Rmj2A_lmt.SetLineColor(46);
  h_Rmj2A_lmt.SetXTitle("R_{m_{T}}");
  h_Rmj2A_lmt.SetYTitle("Entries/0.001");  
  h_Rmj2A_lmt.Draw("hist");
  h_Rmj2A_hmt.SetLineColor(31);
  h_Rmj2A_hmt.Draw("hist sames");
  line.DrawLine(0.322,0,0.322,h_Rmj2A_lmt.GetMaximum());
  line.DrawLine(0.352,0,0.352,h_Rmj2A_hmt.GetMaximum());

  TString slegRmj2A_lmt = "#mu="; slegRmj2A_lmt+=RoundNumber(h_Rmj2A_lmt.GetMean(),3); slegRmj2A_lmt+=" #sigma="; slegRmj2A_lmt+=RoundNumber(h_Rmj2A_lmt.GetRMS(),3);
  TString slegRmj2A_hmt = "#mu="; slegRmj2A_hmt+=RoundNumber(h_Rmj2A_hmt.GetMean(),3); slegRmj2A_hmt+=" #sigma="; slegRmj2A_hmt+=RoundNumber(h_Rmj2A_hmt.GetRMS(),3);
  TLegend* legRmj2A = new TLegend(0.75, 0.8, 1, 1.0);
  legRmj2A->AddEntry("Rmj2A_lmt",slegRmj2A_lmt,"lpf");
  legRmj2A->AddEntry("Rmj2A_hmt",slegRmj2A_hmt,"lpf");
  legRmj2A->Draw();

  canRmj2A.SaveAs("plots/1d/badjets/Rmj2A.pdf");
  canRmj2A.SaveAs("plots/1d/badjets/rootfiles/Rmj2A.root");

  TCanvas canRmj2B;
  canRmj2B.SetLogy(1);
  h_Rmj2B_lmt.SetLineColor(46);
  h_Rmj2B_lmt.SetXTitle("R_{m_{T}}");
  h_Rmj2B_lmt.SetYTitle("Entries/0.001");
  h_Rmj2B_lmt.Draw("hist");
  h_Rmj2B_hmt.SetLineColor(31);
  h_Rmj2B_hmt.Draw("hist sames");
  line.DrawLine(0.363,0,0.363,h_Rmj2B_lmt.GetMaximum());
  line.DrawLine(0.410,0,0.410,h_Rmj2B_hmt.GetMaximum());

  TString slegRmj2B_lmt = "#mu="; slegRmj2B_lmt+=RoundNumber(h_Rmj2B_lmt.GetMean(),3); slegRmj2B_lmt+=" #sigma="; slegRmj2B_lmt+=RoundNumber(h_Rmj2B_lmt.GetRMS(),3);
  TString slegRmj2B_hmt = "#mu="; slegRmj2B_hmt+=RoundNumber(h_Rmj2B_hmt.GetMean(),3); slegRmj2B_hmt+=" #sigma="; slegRmj2B_hmt+=RoundNumber(h_Rmj2B_hmt.GetRMS(),3);
  TLegend* legRmj2B = new TLegend(0.75, 0.8, 1, 1.0);
  legRmj2B->AddEntry("Rmj2B_lmt",slegRmj2B_lmt,"lpf");
  legRmj2B->AddEntry("Rmj2B_hmt",slegRmj2B_hmt,"lpf");
  legRmj2B->Draw();

  canRmj2B.SaveAs("plots/1d/badjets/Rmj2B.pdf");
  canRmj2B.SaveAs("plots/1d/badjets/rootfiles/Rmj2B.root");

  TCanvas canRmj2C;
  canRmj2C.SetLogy(1);
  h_Rmj2C_lmt.SetLineColor(46);
  h_Rmj2C_lmt.SetXTitle("R_{m_{T}}");
  h_Rmj2C_lmt.SetYTitle("Entries/0.001");
  h_Rmj2C_lmt.Draw("hist");
  h_Rmj2C_hmt.SetLineColor(31);
  h_Rmj2C_hmt.Draw("hist sames");
  line.DrawLine(1.003,0,1.003,h_Rmj2C_lmt.GetMaximum());
  line.DrawLine(1.053,0,1.053,h_Rmj2C_hmt.GetMaximum());

  TString slegRmj2C_lmt = "#mu="; slegRmj2C_lmt+=RoundNumber(h_Rmj2C_lmt.GetMean(),3); slegRmj2C_lmt+=" #sigma="; slegRmj2C_lmt+=RoundNumber(h_Rmj2C_lmt.GetRMS(),3);
  TString slegRmj2C_hmt = "#mu="; slegRmj2C_hmt+=RoundNumber(h_Rmj2C_hmt.GetMean(),3); slegRmj2C_hmt+=" #sigma="; slegRmj2C_hmt+=RoundNumber(h_Rmj2C_hmt.GetRMS(),3);
  TLegend* legRmj2C = new TLegend(0.75, 0.8, 1, 1.0);
  legRmj2C->AddEntry("Rmj2C_lmt",slegRmj2C_lmt,"lpf");
  legRmj2C->AddEntry("Rmj2C_hmt",slegRmj2C_hmt,"lpf");
  legRmj2C->Draw();

  canRmj2C.SaveAs("plots/1d/badjets/Rmj2C.pdf");
  canRmj2C.SaveAs("plots/1d/badjets/rootfiles/Rmj2C.root");

  TCanvas canRmj2D;
  canRmj2D.SetLogy(1);
  h_Rmj2D_lmt.SetLineColor(46);
  h_Rmj2D_lmt.SetXTitle("R_{m_{T}}");
  h_Rmj2D_lmt.SetYTitle("Entries/0.001");
  h_Rmj2D_lmt.Draw("hist");
  h_Rmj2D_hmt.SetLineColor(31);
  h_Rmj2D_hmt.Draw("hist sames");
  line.DrawLine(0.122,0,0.122,h_Rmj2D_lmt.GetMaximum());
  line.DrawLine(0.112,0,0.112,h_Rmj2D_hmt.GetMaximum());

  TString slegRmj2D_lmt = "#mu="; slegRmj2D_lmt+=RoundNumber(h_Rmj2D_lmt.GetMean(),3); slegRmj2D_lmt+=" #sigma="; slegRmj2D_lmt+=RoundNumber(h_Rmj2D_lmt.GetRMS(),3);
  TString slegRmj2D_hmt = "#mu="; slegRmj2D_hmt+=RoundNumber(h_Rmj2D_hmt.GetMean(),3); slegRmj2D_hmt+=" #sigma="; slegRmj2D_hmt+=RoundNumber(h_Rmj2D_hmt.GetRMS(),3);
  TLegend* legRmj2D = new TLegend(0.75, 0.8, 1, 1.0);
  legRmj2D->AddEntry("Rmj2D_lmt",slegRmj2D_lmt,"lpf");
  legRmj2D->AddEntry("Rmj2D_hmt",slegRmj2D_hmt,"lpf");
  legRmj2D->Draw();

  canRmj2D.SaveAs("plots/1d/badjets/Rmj2D.pdf");
  canRmj2D.SaveAs("plots/1d/badjets/rootfiles/Rmj2D.root");

  TCanvas canRmj2E;
  canRmj2E.SetLogy(1);
  h_Rmj2E_lmt.SetLineColor(46);
  h_Rmj2E_lmt.SetXTitle("R_{m_{T}}");
  h_Rmj2E_lmt.SetYTitle("Entries/0.001");
  h_Rmj2E_lmt.Draw("hist");
  h_Rmj2E_hmt.SetLineColor(31);
  h_Rmj2E_hmt.Draw("hist sames");
  line.DrawLine(0.187,0,0.187,h_Rmj2E_lmt.GetMaximum());
  line.DrawLine(0.186,0,0.186,h_Rmj2E_hmt.GetMaximum());

  TString slegRmj2E_lmt = "#mu="; slegRmj2E_lmt+=RoundNumber(h_Rmj2E_lmt.GetMean(),3); slegRmj2E_lmt+=" #sigma="; slegRmj2E_lmt+=RoundNumber(h_Rmj2E_lmt.GetRMS(),3);
  TString slegRmj2E_hmt = "#mu="; slegRmj2E_hmt+=RoundNumber(h_Rmj2E_hmt.GetMean(),3); slegRmj2E_hmt+=" #sigma="; slegRmj2E_hmt+=RoundNumber(h_Rmj2E_hmt.GetRMS(),3);
  TLegend* legRmj2E = new TLegend(0.75, 0.8, 1, 1.0);
  legRmj2E->AddEntry("Rmj2E_lmt",slegRmj2E_lmt,"lpf");
  legRmj2E->AddEntry("Rmj2E_hmt",slegRmj2E_hmt,"lpf");
  legRmj2E->Draw();

  canRmj2E.SaveAs("plots/1d/badjets/Rmj2E.pdf");
  canRmj2E.SaveAs("plots/1d/badjets/rootfiles/Rmj2E.root");

  TCanvas canRmj2F;
  canRmj2F.SetLogy(1);
  h_Rmj2F_lmt.SetLineColor(46);
  h_Rmj2F_lmt.SetXTitle("R_{m_{T}}");
  h_Rmj2F_lmt.SetYTitle("Entries/0.001");
  h_Rmj2F_lmt.Draw("hist");
  h_Rmj2F_hmt.SetLineColor(31);
  h_Rmj2F_hmt.Draw("hist sames");
  line.DrawLine(0.371,0,0.371,h_Rmj2F_lmt.GetMaximum());
  line.DrawLine(0.339,0,0.339,h_Rmj2F_hmt.GetMaximum());

  TString slegRmj2F_lmt = "#mu="; slegRmj2F_lmt+=RoundNumber(h_Rmj2F_lmt.GetMean(),3); slegRmj2F_lmt+=" #sigma="; slegRmj2F_lmt+=RoundNumber(h_Rmj2F_lmt.GetRMS(),3);
  TString slegRmj2F_hmt = "#mu="; slegRmj2F_hmt+=RoundNumber(h_Rmj2F_hmt.GetMean(),3); slegRmj2F_hmt+=" #sigma="; slegRmj2F_hmt+=RoundNumber(h_Rmj2F_hmt.GetRMS(),3);
  TLegend* legRmj2F = new TLegend(0.75, 0.8, 1, 1.0);
  legRmj2F->AddEntry("Rmj2F_lmt",slegRmj2F_lmt,"lpf");
  legRmj2F->AddEntry("Rmj2F_hmt",slegRmj2F_hmt,"lpf");
  legRmj2F->Draw();

  canRmj2F.SaveAs("plots/1d/badjets/Rmj2F.pdf");
  canRmj2F.SaveAs("plots/1d/badjets/rootfiles/Rmj2F.root");
}
