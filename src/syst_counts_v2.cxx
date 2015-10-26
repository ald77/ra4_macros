#include "syst_counts_v2.hpp"

#include <cstdlib>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "TTreeFormula.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TMath.h"
#include "styles.hpp"
#include "small_tree_quick.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"
#include "timer.hpp"

namespace ra4 {
  std::string ntuple_date("2015_06_05");
  int minjets(6);
  double midjets(8);
  double mjthresh(400.);
  TString luminosity="10";
}

using namespace ra4;
using namespace std;

int main(int argc, char *argv[]){
  //bool include_signal = false; //Include signal when mocking up data from MC?
  //cout<<ntuple_date<<endl;
  string folder_other="/net/cms5/cms5r0/ald77/archive/"+ntuple_date+"/skim/";
  //string folder = "/cms7r0/heller/code/susy_cfa/out/";
  //string folder = "/net/cms2/cms2r0/manuelf/root/archive/15-06-01/skim/";
  string folder = "/net/cms29/cms29r0/heller/ra4_macros/rohanskim/";
  double lumi = 10.;
  string lumi_string = "10";
  bool compressed = false;
  if(argc>1){
    lumi = strtod(argv[1], NULL);
    lumi_string = argv[1];
    if(argc>2 && string("1200")==argv[2]) compressed = true;
  }
  if(lumi <= 0.0){
    lumi = 10.;
    lumi_string = "10";
  }

  
  small_tree_quick ttbar(folder+"*TTJets*");
  small_tree_quick other(folder_other+"*QCD_Pt*");
  other.Add(folder_other+"*TTWJets*");
  other.Add(folder_other+"*TTZJets*");
  other.Add(folder_other+"*_T*-channel*");
  other.Add(folder_other+"*WJetsToLNu_HT*");
  other.Add(folder_other+"*ZJetsToLNu_HT*");
  other.Add(folder_other+"*DYJets*");
  other.Add(folder_other+"*H_HToBB*");
  small_tree_quick *sig;
  if(compressed){
    sig = new small_tree_quick(folder_other+"*T1tttt*1200*800*");
  }else{
    sig = new small_tree_quick(folder_other+"*T1tttt*1500*100*");
  }


  //Method2
  vector< vector<TString> > bins_by_region2;
  //TString method = "method2";
  vector<TString> region1;
  region1.push_back("met<=400");
  region1.push_back("met>400");

  vector<TString> region2;
  region2.push_back("njets<=8&&met<=400");
  region2.push_back("njets<=8&&met>400");
  region2.push_back("njets>8&&met<=400");
  region2.push_back("njets>8&&met>400");

  vector<TString> region3;
  //region3.push_back("njets<=8&&met<=400"); //testing FindSuperbin
  //region3.push_back("njets>8&&met<=400");
  region3.push_back("met<=400");
  region3.push_back("met>400");

  vector<TString> region4;
  region4.push_back("njets<=8&&met<=400");
  region4.push_back("njets<=8&&met>400");
  region4.push_back("njets>8&&met<=400");
  region4.push_back("njets>8&&met>400");

  bins_by_region2.push_back(region1);
  bins_by_region2.push_back(region2);
  bins_by_region2.push_back(region3);
  bins_by_region2.push_back(region4);


   //Method 1
  vector< vector<TString> > bins_by_region1;
  vector<TString> method1_region;
  method1_region.push_back("njets<=8&&met<=400");
  method1_region.push_back("njets<=8&&met>400");
  method1_region.push_back("njets>8&&met<=400");
  method1_region.push_back("njets>8&&met>400");

  bins_by_region1.push_back(method1_region);
  bins_by_region1.push_back(method1_region);
  bins_by_region1.push_back(method1_region);
  bins_by_region1.push_back(method1_region);
  
  TString baseline = "njets>=7&&nbm>=1&&(nels+nmus)==1";
  TString dilepton = "njets>=6&&nbm>=1&&(nels+nmus)==2";
  //GetCounts will automatically implement object counting instead of jet counting
  // it does this by replacing "8&&" with "7&&" for dileptons. So never end another cut with 8!

  vector< vector<TString> > bins_by_region = bins_by_region1;
  TString method = "method2";
  //  TString sysname = "nISR";
  vector< TString> sysnames;
  sysnames.push_back("nominal");
  sysnames.push_back("nISR");
  sysnames.push_back("ISRpT");
  sysnames.push_back("ISRpTx150");
  sysnames.push_back("toppT");
  sysnames.push_back("1lep_frac");
  
  

  int nsys= static_cast<int>(sysnames.size());
  
  
  vector< vector< vector<double> > > ttbar_counts, ttbar_errors; //first dimension indicates region 1 through 4. second dimension gets counts for binning in that region (binning is allowed to be different in each region), third dimension is for all variations on reweighting
  vector< vector< vector<double> > > ttbar_counts_dilep, ttbar_errors_dilep; 
  vector< vector< vector<double> > > other_counts, other_errors;
  vector< vector< vector<double> > > other_counts_dilep, other_errors_dilep;
  vector< vector< vector<double> > > sig_counts, sig_errors;
  // cout<<"about to get counts"<<endl;
  vector<int> emptysamples;
  vector<hfeats> hists, emptyhists;
  
  vector< vector<TH1F*> > h, h_dilep,emptyh;
    //first dim is hists, second dimension is variation
 
  
  hists.push_back(hfeats("ISRpT",20,0,1000, emptysamples, "ISR pT","ht>500")); //must be first right now due to hack
  hists.push_back(hfeats("ntrumeisr",4,-0.5,3.5, emptysamples, "n ME ISR","ht>500"));
  hists.push_back(hfeats("mj",20,0,1000, emptysamples, "M_{J} [GeV]","mt>140"));
  hists.push_back(hfeats("mj",20,0,1000, emptysamples, "M_{J} [GeV]","mt<=140"));
  hists.push_back(hfeats("mj",10,0,1000, emptysamples, "M_{J} [GeV]","mt>140&&met>400"));
  hists.push_back(hfeats("mj",10,0,1000, emptysamples, "M_{J} [GeV]","mt<=140&&met>400"));
  
  hists.push_back(hfeats("met",30,0,600, emptysamples, "MET [GeV]","ht>500"));
  hists.push_back(hfeats("mt",20,0,280, emptysamples, "m_{T} [GeV]","ht>500"));
  hists.push_back(hfeats("mt",2,0,280, emptysamples, "m_{T} [GeV]","met>200"));
  hists.push_back(hfeats("mt",20,0,280, emptysamples, "m_{T} [GeV]","mj>400"));
  hists.push_back(hfeats("mt",2,0,280, emptysamples, "m_{T} [GeV]","mj>400&&met>200"));
  hists.push_back(hfeats("mt",2,0,280, emptysamples, "m_{T} [GeV]","ntruleps==1"));
  hists.push_back(hfeats("trutop1_pt",25,0,800, emptysamples, "top pT [GeV]","ht>500"));
  hists.push_back(hfeats("trutop1_pt+trutop2_pt",32,0,1600, emptysamples, "top pT + antitop pT [GeV]","ht>500"));

  
  int nhists = static_cast<int>(hists.size());
  //hists.push_back(hfeats("mt",20,0,280, emptysamples, "mT [GeV]","ht>500"));
  
  GetCounts(lumi,sysnames, method, baseline, ttbar, ttbar_counts, ttbar_errors, bins_by_region, hists, h);
  GetCounts(lumi,sysnames, method, dilepton, ttbar, ttbar_counts_dilep, ttbar_errors_dilep, bins_by_region, hists, h_dilep);

  vector< TString> othersys;
  for(int i=0;i<nsys; i++){
    othersys.push_back("nominal");
  }
  
  GetCounts(lumi,othersys, method, baseline, other, other_counts, other_errors, bins_by_region, emptyhists, emptyh);
  GetCounts(lumi,othersys, method, dilepton, other, other_counts_dilep, other_errors_dilep, bins_by_region, emptyhists, emptyh);
  
  cout<<"got counts"<<endl;
  //Replace vacant "low mT" regions with single-reco lepton counts
  ttbar_counts_dilep.at(0) = ttbar_counts.at(0);
  ttbar_counts_dilep.at(1) = ttbar_counts.at(1);
  ttbar_errors_dilep.at(0) = ttbar_errors.at(0);
  ttbar_errors_dilep.at(1) = ttbar_errors.at(1);
  /*for(int is=0;is<nsys;is++){
    ttbar_counts_dilep.at(is) = ttbar_counts.at(is);
    ttbar_errors_dilep.at(is) = ttbar_errors.at(is);

    }*/
  
  


 
  vector< vector<double> > delkappas(nsys,vector<double>(4,0.0));
  vector< vector<double> > delkappas_dilep(nsys,vector<double>(4,0.0));
  cout<<"initialized delkappas"<<endl;
  //  MakePlots(sysname,baseline,ttbar_counts, ttbar_errors, bins_by_region, method, delkappas);
  //  MakePlots(sysname,dilepton,ttbar_counts_dilep, ttbar_errors_dilep, bins_by_region, method+"_dilep",delkappas_dilep);

  vector<int> RMJ_nums; RMJ_nums.push_back(3); RMJ_nums.push_back(1); 
  vector<int> RMJ_dens; RMJ_dens.push_back(2); RMJ_dens.push_back(0); 
  vector<int> RmT_nums; RmT_nums.push_back(3); RmT_nums.push_back(2);
  vector<int> RmT_dens; RmT_dens.push_back(1); RmT_dens.push_back(0);
  cout<<"making plots"<<endl;
  for(int is=1;is<nsys; is++){
    cout<<"is = "<<is<<endl;
    MakePlots("R_{MJ}", RMJ_nums, RMJ_dens, ttbar_counts, ttbar_errors, bins_by_region, baseline, method, sysnames,is,delkappas[is]);
    MakePlots("R_{mT}", RmT_nums, RmT_dens, ttbar_counts, ttbar_errors, bins_by_region, baseline, method, sysnames,is,delkappas[is]);
    MakePlots("R_{MJ}", RMJ_nums, RMJ_dens, ttbar_counts_dilep, ttbar_errors_dilep, bins_by_region, dilepton, method, sysnames,is,delkappas_dilep[is]);
    MakePlots("R_{mT}", RmT_nums, RmT_dens, ttbar_counts_dilep, ttbar_errors_dilep, bins_by_region, dilepton, method, sysnames,is,delkappas_dilep[is]);
  }

  PrintTable(method,ttbar_counts,ttbar_errors,ttbar_counts_dilep, other_counts, other_counts_dilep, delkappas, bins_by_region);

  for(int ihist=0;ihist<nhists;ihist++){
    if(ihist==2 || ihist==3 || ihist==4 || ihist==5) continue;
    for(int isy=1;isy<nsys; isy++){
      vector<TH1F*> temp;
      temp.push_back(static_cast<TH1F*>(h[ihist][0]->Clone(Form("%i_%i",isy,ihist)))); //nominal
      temp.push_back(h[ihist][isy]); //varied
      OverlayHists(temp,hists[ihist],method,sysnames[isy],baseline);
    }
  }
  
  for(int isy=1;isy<nsys; isy++){
    vector<TH1F*> temp;
    temp.push_back(static_cast<TH1F*>(h[2][0]->Clone(Form("%i_%i",isy,2)))); //high mT nominal
    temp.push_back(h[2][isy]); //high mT varied
    temp.push_back(static_cast<TH1F*>(h[3][0]->Clone(Form("%i_%i",isy,3)))); //low mT nominal
    temp.push_back(h[3][isy]); //low mt varied
    OverlayHistsmT(temp,hists[2],method,sysnames[isy],baseline);
  }

   for(int isy=1;isy<nsys; isy++){
    vector<TH1F*> temp;
    temp.push_back(static_cast<TH1F*>(h[4][0]->Clone(Form("%i_%i",isy,4)))); //high mT nominal
    temp.push_back(h[4][isy]); //high mT varied
    temp.push_back(static_cast<TH1F*>(h[5][0]->Clone(Form("%i_%i",isy,5)))); //low mT nominal
    temp.push_back(h[5][isy]); //low mt varied
    OverlayHistsmT(temp,hists[4],method,sysnames[isy],baseline);
  }
  
  	    
}



void PrintTable(TString method, vector< vector< vector<double> > > ttbar_counts, vector< vector< vector<double> > > ttbar_errors, vector< vector< vector<double> > > ttbar_counts_dilep, vector< vector< vector<double> > > other_counts,vector< vector< vector<double> > > other_counts_dilep, vector< vector<double> > delkappas, vector< vector<TString> > bins_by_region){
  ofstream file("txt/newtable_1b_"+method+".csv");
  for(unsigned int ibin=0;ibin<bins_by_region.at(3).size(); ibin++){
    int bin2 = FindSuperBin(bins_by_region.at(3).at(ibin),bins_by_region.at(2));
    int bin1 = FindSuperBin(bins_by_region.at(3).at(ibin),bins_by_region.at(1));
    int bin0 = FindSuperBin(bins_by_region.at(3).at(ibin),bins_by_region.at(0));
   
    double statabcd = sqrt(1.0/(other_counts.at(0).at(bin0).at(0)+ttbar_counts.at(0).at(bin0).at(0)) + 1.0/(other_counts.at(1).at(bin1).at(0)+ttbar_counts.at(1).at(bin1).at(0)) + 1.0/(other_counts.at(2).at(bin2).at(0)+ttbar_counts.at(2).at(bin2).at(0)));
    double mcstat = sqrt(sqr(ttbar_errors.at(3).at(ibin).at(0)/ttbar_counts.at(3).at(ibin).at(0))+sqr(ttbar_errors.at(0).at(bin0).at(0)/ttbar_counts.at(0).at(bin0).at(0)) + sqr(ttbar_errors.at(1).at(bin1).at(0)/ttbar_counts.at(1).at(bin1).at(0)) + sqr(ttbar_errors.at(2).at(bin2).at(0)/ttbar_counts.at(2).at(bin2).at(0)));
    double closure = sqrt(1.0/(other_counts_dilep.at(3).at(ibin).at(0)+ ttbar_counts_dilep.at(3).at(ibin).at(0)));
    
    file << bins_by_region.at(3).at(ibin) <<","<< (other_counts.at(3).at(ibin).at(0)+ttbar_counts.at(3).at(ibin).at(0))<<","<<sqr(1.0/closure)<<","<<closure<<","<<statabcd<<","<<mcstat;
    for(unsigned int is=1; is< delkappas.size();is++){
      file <<"," <<delkappas.at(is).at(ibin);
    }
    file<<"\n";
  }


}

double GetWeight(small_tree_quick &tree, TString sysname, int ivar){
  if(ivar==0) return 1.;
  if(sysname.Contains("not_ttbar") )return 1.;
  if(sysname.Contains("nominal") ) return 1.;
  if(sysname.Contains("Nominal") ) return 1.;
  double weight(1.0);
  
  if(sysname.Contains("1lep_frac")){
    if(tree.ntruleps()==1 && tree.mt()>140){
      weight = 2.0;
    }
  }
  
  if(sysname.Contains("toppT")){
    //8 TeV Form
    float t1pT,t2pT;
    //if(tree.trutop1_pt()>600) t1pT=600;
    //if(tree.trutop1_pt()<200) t1pT=200;
    t1pT=tree.trutop1_pt();

    //if(tree.trutop2_pt()>600) t2pT=600;
    //  if(tree.trutop2_pt()<200) t2pT=200;
    t2pT=tree.trutop2_pt();
    float weight_top1pT = TMath::Exp(0.156-0.00137*t1pT);
    float weight_top2pT = TMath::Exp(0.156-0.00137*t2pT);
    weight = TMath::Sqrt(weight_top1pT*weight_top2pT);
  }

  if(sysname.Contains("nISR")){
    if(tree.ntrumeisr()>3) {cout<<"ERROR: n ISR = "<<tree.ntrumeisr()<<endl; weight =  1.0;}
    else if(tree.ntrumeisr()==3) weight =  1.5;
    else if(tree.ntrumeisr()==2) weight =  1.25;
    else if(tree.ntrumeisr()==1) weight =  1.0;
    else weight =  0.75; 
  }

  /*  if(sysname.Contains("ISRpT")){
    float ISRpT=0;
    float ISRpx = tree.trutop1_pt()*TMath::Cos(tree.trutop1_phi()) + tree.trutop2_pt()*TMath::Cos(tree.trutop2_phi());
    float ISRpy = tree.trutop1_pt()*TMath::Sin(tree.trutop1_phi()) + tree.trutop2_pt()*TMath::Sin(tree.trutop2_phi());
    ISRpT = TMath::Sqrt(ISRpx*ISRpx+ISRpy*ISRpy);
    
    if( ISRpT>500) weight =  0.6;
    else if( ISRpT>300 ) weight =  0.8;
    else if( ISRpT>240 ) weight =  0.9;
    else if( ISRpT>120) weight =  0.95;
    else weight =  1.0;
    }*/


  if(sysname.Contains("ISRpT")){
    float ISRpT=0;
    float ISRpx = tree.trutop1_pt()*TMath::Cos(tree.trutop1_phi()) + tree.trutop2_pt()*TMath::Cos(tree.trutop2_phi());
    float ISRpy = tree.trutop1_pt()*TMath::Sin(tree.trutop1_phi()) + tree.trutop2_pt()*TMath::Sin(tree.trutop2_phi());
    ISRpT = TMath::Sqrt(ISRpx*ISRpx+ISRpy*ISRpy);

    if( ISRpT>800) weight =  0.2;
    else if( ISRpT>650) weight =  0.4;
    else if( ISRpT>500) weight =  0.6;
    else if( ISRpT>300 ) weight =  0.8;
    else if( ISRpT>200 ) weight =  0.9;
    else if( ISRpT>120) weight =  0.95;
    else weight =  1.0;
  }

  
  if(sysname.Contains("ISRpTx150")){
    float ISRpT=0;
    float ISRpx = tree.trutop1_pt()*TMath::Cos(tree.trutop1_phi()) + tree.trutop2_pt()*TMath::Cos(tree.trutop2_phi());
    float ISRpy = tree.trutop1_pt()*TMath::Sin(tree.trutop1_phi()) + tree.trutop2_pt()*TMath::Sin(tree.trutop2_phi());
    ISRpT = TMath::Sqrt(ISRpx*ISRpx+ISRpy*ISRpy);

    if( ISRpT>800) weight =  0.;
    else if( ISRpT>650) weight =  0.1;
    else if( ISRpT>500) weight =  0.4;
    else if( ISRpT>300 ) weight =  0.7;
    else if( ISRpT>200 ) weight =  0.85;
    else if( ISRpT>120) weight =  0.925;
    else weight =  1.0;
  }
  

  

  return weight;
}

void GetCounts(double lumi, vector<TString> sysnames, TString method, TString baseline,
               small_tree_quick &tree,
               vector< vector< vector<double> > > &finalcounts,
               vector< vector< vector<double> > > &errors, vector< vector<TString> > bins_by_region, vector<hfeats> hists, vector< vector<TH1F*> > &h){
  
  int nvar=sysnames.size();
  //replace jet thresholds with 7 instead of 8 for dileptons
  if(baseline.Contains("(nels+nmus)==2")){
      for(unsigned int ir=0;ir<bins_by_region.size(); ir++){
	for(unsigned int ib=0; ib<bins_by_region[ir].size(); ib++){
	  bins_by_region[ir][ib].ReplaceAll("8&&","7&&");
	}
      }
    }


  //initialize hists
  if(hists.size()>0){
    h.resize(0);
  
    for(unsigned int ihist=0;ihist<hists.size();ihist++){
      vector<TH1F*> temp;
      for(int is=0;is<nvar;is++){
	TH1F * h1 = new TH1F(hists[ihist].varname+"_"+sysnames[is]+baseline+"_"+Form("%i",ihist),baseline+"&&"+hists[ihist].cuts,hists[ihist].nbins,hists[ihist].minx,hists[ihist].maxx);
	h1->Sumw2();
	temp.push_back(h1);
      }
      h.push_back(temp);
      
    }
  }
    vector< vector< vector<double> > > counts; //= vector< vector< vector<double> > >(4, vector< vector<double> >());
   
    for(int ireg=0;ireg<4;ireg++){
      counts.push_back(vector<vector<double> >(bins_by_region.at(ireg).size(),vector<double>(nvar,0.)));
    }  
    vector< vector< vector<double> > > squares = counts;
    finalcounts = counts;
    errors = counts;

    double sumw = 0., sumw2 = 0.; //for entire tree, to assign weights for unfilled bins
  
    int num_entries = tree.GetEntries();
    //    num_entries=1000;
    Timer timer(num_entries, 1.);
    timer.Start();
    //cout<<"starting loop"<<endl;
    for(int entry = 0; entry < num_entries; ++entry){
      tree.GetEntry(entry);
      timer.Iterate();
      
      if(//tree.nbm()<2
	 tree.njets()<minjets
	 //|| (tree.nmus()+tree.nels())!=1
	 || tree.met()<=200.
	 //       || tree.ntks_chg_mini()>0
	 || tree.ht()<=500.) continue; //Faster to check this way than using baseline string

      if(!tree.PassString(baseline)) continue;

      double weight = lumi*tree.weight();
      double varweight(0);
      float ISRpT(0); float ISRpx(0); float ISRpy(0);
      if(hists.size()>0){
	ISRpx = tree.trutop1_pt()*TMath::Cos(tree.trutop1_phi()) + tree.trutop2_pt()*TMath::Cos(tree.trutop2_phi());
	ISRpy = tree.trutop1_pt()*TMath::Sin(tree.trutop1_phi()) + tree.trutop2_pt()*TMath::Sin(tree.trutop2_phi());
	ISRpT = TMath::Sqrt(ISRpx*ISRpx+ISRpy*ISRpy);
	if(ISRpT > hists[0].maxx) ISRpT = hists[0].maxx - 0.01;
	if(ISRpT < hists[0].minx) ISRpT = hists[0].minx + 0.01;
      }
      
      // cout<<"passed baseline"<<endl;
      int region = LookUpRegion(tree,method);
      if(region>-1){
	int bin = LookUpBin(tree,region,bins_by_region);
	if(bin>-1){
	  //fill first element with nominal counts
	
	  for(int ivar=0; ivar<nvar; ivar++){
	 
	    varweight = weight*GetWeight(tree,sysnames.at(ivar), ivar);
	    counts.at(region).at(bin).at(ivar) += varweight;
	    squares.at(region).at(bin).at(ivar) += varweight*varweight;

	    //fill hists

	    //first, hack to fill ISR pT which cannot be done in string yet

	    if(hists.size()>0){
	    h[0][ivar]->Fill(ISRpT,varweight);
	    }
	    for(unsigned int ih=1;ih < hists.size();ih++){
	      if(tree.PassString(hists[ih].cuts)){
		float val = tree.GetBranchValue(hists[ih].varname);
		//overflow handling.. yes i know this ruins the mean
		if(val > hists[ih].maxx) val = hists[ih].maxx - 0.01;
		if(val < hists[ih].minx) val = hists[ih].minx + 0.01;
		h[ih][ivar]->Fill(val,varweight);
		//	h[ih][1]->Fill(val,varweight);
	      }
	    }
	  }
	}
      }
      sumw += weight;
      sumw2 += weight*weight;
      
    
      
    }
   
    for(unsigned int ireg = 0; ireg< counts.size(); ireg++){
      for(unsigned int ibin = 0; ibin < counts.at(ireg).size(); ++ibin){
	for( int ifill = 0; ifill< nvar; ifill++){
	  //Hack to "add one single lepton event 
	  if(sysnames[ifill].Contains("1lep_frac") && ireg==3&&ibin==3){ counts.at(ireg).at(ibin).at(ifill) += sumw2/sumw; squares.at(ireg).at(ibin).at(ifill) += sqr(sumw2/sumw); }
	  
	  if(squares.at(ireg).at(ibin).at(ifill) == 0.){
	    //Never filled
	    finalcounts.at(ireg).at(ibin).at(ifill) = 0;
	    errors.at(ireg).at(ibin).at(ifill) = sumw2/sumw; //assign average weight as error?
	  }else if(counts.at(ireg).at(ibin).at(ifill) <= 0.){
	    //Negative estimate
	    finalcounts.at(ireg).at(ibin).at(ifill) = 0;
	    errors.at(ireg).at(ibin).at(ifill) = sqrt(squares.at(ireg).at(ibin).at(ifill)+sqr(counts.at(ireg).at(ibin).at(ifill)));
	    //sqrt(squares.at(ireg).at(ibin).at(ifill)+sqr(counts.at(ireg).at(ibin).at(ifill))); //assign average weight as error?
	  }else{
	    //Normal case
	    finalcounts.at(ireg).at(ibin).at(ifill) = counts.at(ireg).at(ibin).at(ifill);//sqr(counts.at(ireg).at(ibin).at(ifill))/squares.at(ireg).at(ibin).at(ifill);
	    errors.at(ireg).at(ibin).at(ifill) = sqrt(squares.at(ireg).at(ibin).at(ifill));
	    // weight: squares.at(ireg).at(ibin).at(ifill)/counts.at(ireg).at(ibin).at(ifill);
	  }
	}
      }
    }
    
}
    


void OverlayHists(vector<TH1F*> h, hfeats hist, TString method, TString sysname, TString baseline){  
        TString pname = "plots/systematics/1d/"+format_tag(hist.varname+baseline+hist.cuts+"_"+method+"_"+sysname)+".pdf";
	if(sysname=="toppT") sysname = "top p_{T}";

      styles style("RA4"); style.setDefaultStyle();
      TCanvas can;
      float max(0.0);
      for(unsigned int ih=0;ih < h.size();ih++){
	if(h[ih]->GetMaximum() > max) max = h[ih]->GetMaximum();
      }
      double legX = 0.6, legY = 0.89, legSingle = 0.17;
      double legW = 0.22, legH = legSingle;
      TLegend leg(legX, legY-legH, legX+legW, legY);
      leg.SetTextSize(0.057); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
      leg.SetTextFont(132);
      
      for(unsigned int ih=0;ih < h.size();ih++){
	h[ih]->GetXaxis()->SetLabelSize(0.05);
	h[ih]->GetYaxis()->SetLabelSize(0.05);
	h[ih]->GetXaxis()->SetTitle(cuts2title(hist.title));
	h[ih]->GetXaxis()->SetTitleOffset(1.15);
	h[ih]->GetXaxis()->SetTitleSize(0.06);
	h[ih]->GetXaxis()->SetTitle(cuts2title(hist.title));
	h[ih]->GetYaxis()->SetTitle("Entries for 3 fb^{-1}");
	//h[ih]->GetYaxis()->SetTitleOffset(2);
	h[ih]->GetYaxis()->SetTitleSize(0.06);
	h[ih]->SetLineWidth(3);
	h[ih]->SetMaximum(1.35*max);
	h[ih]->SetTitle(cuts2title(baseline));
	if(ih==1) h[ih]->SetLineColor(kRed);
	else h[ih]->SetLineColor(kBlack);

	if(ih==0) leg.AddEntry(h[ih],"Nominal t#bar{t}","l");
	if(ih==1) leg.AddEntry(h[ih],sysname+" Variation","l");
	
	
	if(ih==0) h[ih]->Draw("hist");
	else h[ih]->Draw("hist same");

      }
      leg.Draw("same");
      //     TString pname = "plots/systematics/1d/"+format_tag(hist.varname+baseline+hist.cuts+"_"+method+"_"+sysname)+".pdf";
      can.SaveAs(pname);
    
    
  }


void OverlayHistsmT(vector<TH1F*> h, hfeats hist, TString method, TString sysname, TString baseline){  
  TString pname = "plots/systematics/1d/mt_"+format_tag(hist.varname+baseline+hist.cuts+"_"+method+"_"+sysname)+".pdf";
  if(sysname=="toppT") sysname = "top p_{T}";
     styles style("RA4"); style.setDefaultStyle();
      TCanvas can;
      float max(0);
      
      for(unsigned int ih=0;ih < h.size();ih++){
	float integral = h[ih]->Integral();
	if(integral>0.0001) h[ih]->Scale(100./integral);
	if(h[ih]->GetMaximum() > max) max = h[ih]->GetMaximum();
	//	cout<<"Hist Name "<<h[ih]->GetName()<<endl;
	//	cout<<"integral for this hist: "<<h[ih]->Integral()<<" float integral = "<<integral<<" Maximum and max: "<< h[ih]->GetMaximum()<<" "<<max<<endl;
      }

      double legX = 0.45, legY = 0.89, legSingle = 0.25;
      double legW = 0.22, legH = legSingle;
      TLegend leg(legX, legY-legH, legX+legW, legY);
      leg.SetTextSize(0.057); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
      leg.SetTextFont(132);

      //  TString titles = {"High mT ttbar Nominal"};
      for(unsigned int ih=0;ih < h.size();ih++){

	h[ih]->GetXaxis()->SetLabelSize(0.05);
	h[ih]->GetYaxis()->SetLabelSize(0.05);
	h[ih]->GetXaxis()->SetTitle(cuts2title(hist.title));
	h[ih]->GetXaxis()->SetTitleOffset(1.15);
	h[ih]->GetXaxis()->SetTitleSize(0.06);
	h[ih]->GetYaxis()->SetTitle(cuts2title(hist.title));
	//h[ih]->GetYaxis()->SetTitleOffset(2);
	h[ih]->GetYaxis()->SetTitleSize(0.06);
	h[ih]->SetLineWidth(3);
	h[ih]->SetMaximum(1.5*max);
	TString title = static_cast<TString>(h[ih]->GetTitle());
	if(title.Contains("mt>140") || ih==0 || ih==1) h[ih]->SetLineColor(kRed);
	else h[ih]->SetLineColor(9);

       	title.ReplaceAll("&&mt>140",""); title.ReplaceAll("&&mt<=140","");
	h[ih]->SetTitle(cuts2title(title));
	if(ih==1 || ih==3) h[ih]->SetLineStyle(2);

	h[ih]->GetYaxis()->SetTitle("Entries (%)");
	TString lege;
	if(ih<2) lege="High m_{T} t#bar{t}, ";
	else lege= "Low m_{T} t#bar{t}, ";

	if(ih==1 || ih ==3) {lege+= sysname;
	  lege+= " variation";}
	else lege+= "nominal";
	  
	
	leg.AddEntry(h[ih], lege,"l");
	if(ih==0) h[ih]->Draw("hist");
	else h[ih]->Draw("hist same");

      }
      leg.Draw("same");
      // TString pname = "plots/systematics/1d/mt_"+format_tag(hist.varname+baseline+hist.cuts+"_"+method+"_"+sysname)+".pdf";
      can.SaveAs(pname);
    
    
  }

  
int FindSuperBin(TString cut, vector<TString> other_region){
  //Find bin in region 1-3 to make relevant kappa for region 4 bin defined by "cut"
  //The correct control region bin has a SUBSET OF THE CUTS in the signal bin (superset of events)
  //In some cases, there may be no bin with a strict subset of cuts (for example, if the control regions use met<400 to kill contamination).
  //In this case, use the bin nearest to being a subset (fewest number of extra cuts)
 

  vector<int> score(other_region.size(),0);
  int min_score=9999; int min_bin=-1;
  
  for(unsigned int ibin=0;ibin<score.size();ibin++){
    TObjArray * this_bin_tokens = other_region.at(ibin).Tokenize("&&");
    TIter this_bin_iter(this_bin_tokens);
    TObjString* os=0;
    while ((os=static_cast<TObjString *>(this_bin_iter()))) {
      if(!cut.Contains(os->GetString())) score.at(ibin)++;   
    }
    this_bin_tokens->Delete();
    if(score.at(ibin)<min_score){min_score=score.at(ibin); min_bin=ibin;}
  }
  cout<<"Signal cut: "<<cut<<", Closest match: "<<other_region.at(min_bin)<<", Score: "<<min_score<<endl;
  return min_bin;
}



int LookUpRegion(small_tree_quick &tree, TString method){
  double mt_thresh = 140.;
  double mj_thresh = mjthresh;
  if(method.Contains("method1")) mj_thresh+=200;
  //double njets_thresh = midjets;
  //doublemet_thresh = 400.;
  //double nbm_thresh = 2.5;
  int reg = -1;
  if(tree.mt()<=mt_thresh && (tree.nmus()+tree.nels())==1){
    if(tree.mj()<=mj_thresh) reg=0;
    else reg = 1;
  }
  else{
    if(tree.mj()<=mj_thresh) reg=2;
    else reg=3;

  }
  return reg;

}

int LookUpBin(small_tree_quick &tree, int region, vector< vector<TString> > bins_by_region){
  int bin = -1;
  for(unsigned int ibin=0;ibin<bins_by_region.at(region).size();ibin++){
    
    if(tree.PassString(bins_by_region.at(region).at(ibin))) {
      if(bin!= -1) {cout<<"non-orthogonal bins"<<endl;}
      bin = ibin;
    }
    
  }
  return bin;

}



void MakePlots(TString axisname, vector<int> numerator_regions, vector<int> denominator_regions, vector< vector< vector<double> > > counts,
	       vector< vector< vector<double> > > errors, vector< vector<TString> > bins_by_region, TString baseline, TString method, vector<TString> sysnames,int sysnum, vector<double> &delkappas ){

  //Make list of pairs (Region, Bin) for seamless iteration over all points at once, without worrying about region boundaries
  TString sysname = sysnames.at(sysnum);
  vector<int> sysindices;
  sysindices.push_back(0);
  sysindices.push_back(sysnum);
  vector< vector<int> > nums_reg_bin;
  vector< vector<int> > dens_reg_bin;
  cout<<"Make Plots: sysnum, syname = "<<sysnum<<" "<<sysname<<endl;
  //Make kappa for simplest case: when same number in both numerator regions
  bool make_kappa=false;
  if(bins_by_region.at(numerator_regions.at(0)).size() ==bins_by_region.at(numerator_regions.at(1)).size()) make_kappa=true;
    
  for(unsigned int inum=0;inum<numerator_regions.size(); inum++){ //iterate over numerator regions
    for(unsigned int ibin=0; ibin< bins_by_region.at(numerator_regions.at(inum)).size(); ibin++){ //iterate over bins within numerator region
      vector<int> num;
      num.push_back(numerator_regions.at(inum)); //numerator region
      num.push_back(ibin); //numerator bin
      nums_reg_bin.push_back(num); 

      vector<int> den;
      den.push_back(denominator_regions.at(inum)); //denominator region (Always corresponds 1-1 with numerator region)
      int denbin = FindSuperBin(bins_by_region.at(numerator_regions.at(inum)).at(ibin),bins_by_region.at(denominator_regions.at(inum))); // find denominator bin (does NOT always correspond 1-1 with numerator bin)
      if(denbin<0) cout<<"failed to find denominator bin"<<endl;
      den.push_back(denbin);
      dens_reg_bin.push_back(den);
    }
  }
  
  int npoints = static_cast<int>(nums_reg_bin.size());
  vector<TGraphErrors*> g_Rs;
  vector<TGraphErrors*> g_kappas;
  int npoints_kappa = static_cast<int>(bins_by_region.at(numerator_regions.at(0)).size());
  float maxkappa=0.;
  float max=0.;
  for(unsigned int is=0;is<sysindices.size();is++){
    int isys = sysindices.at(is);
    //initialize all values to go into TGraph
    vector<double> R(npoints,0.);
    vector<double> eR(npoints,0.);
    vector<double> x(npoints,0.);
    vector<double> ex(npoints,0.);

    vector<double> kappa(npoints_kappa,0.);
    vector<double> ekappa(npoints_kappa,0.);
    vector<double> xkappa(npoints_kappa,0.);
    vector<double> exkappa(npoints_kappa,0.);

    
    cout<<"inside MakePlots loop: isys = "<<isys<<endl;
    for(int ipoint = 0; ipoint < npoints; ipoint++){
      
      double numerator(0), denominator(0), enumer(0), edenom(0);
      
      numerator = counts.at(nums_reg_bin[ipoint][0]).at(nums_reg_bin[ipoint][1]).at(isys);
      enumer = errors.at(nums_reg_bin[ipoint][0]).at(nums_reg_bin[ipoint][1]).at(isys);
      denominator = counts.at(dens_reg_bin[ipoint][0]).at(dens_reg_bin[ipoint][1]).at(isys);
      edenom = errors.at(dens_reg_bin[ipoint][0]).at(dens_reg_bin[ipoint][1]).at(isys);

      //if denominator is 0, assign weight of 1 mc event
      if(denominator<=0) denominator = edenom; //in the case that denominator==0, its error is assigned by GetCountsthe weight of 1 MC event.

      R[ipoint] = numerator/denominator;
      if(R[ipoint]>max) max = R[ipoint];

      //Now that R has been captured, need to also replace 0's in numerator for relative error calculation
      if(numerator<=0) numerator = enumer;
      eR[ipoint] = R[ipoint] * sqrt(sqr(enumer/numerator)+sqr(edenom/denominator));
      
      if(make_kappa && ipoint >= npoints_kappa){
	cout<<"making kappa"<<endl;
	double knum = R[ipoint - npoints_kappa]; //Ok if 0
	double kden = numerator/denominator; //Since the current Ratio is denominator for our kappa, we can use this to be safe from the case R=0, since we have already fixed a 0 in this numerator
	double eknum = eR[ipoint - npoints_kappa];
	double ekden = eR[ipoint];

	kappa[ipoint - npoints_kappa] = knum/kden;
	if(kappa[ipoint - npoints_kappa]>maxkappa) maxkappa = kappa[ipoint - npoints_kappa];
	if(knum==0) ekappa[ipoint - npoints_kappa]= 0.0;
	else ekappa[ipoint - npoints_kappa] = kappa[ipoint - npoints_kappa]*sqrt(sqr(eknum/knum)+sqr(ekden/kden));

	xkappa[ipoint - npoints_kappa]= ipoint - npoints_kappa + 0.1*is;
	exkappa[ipoint - npoints_kappa]=0;
    
      }

      //arbitary x-axis:
      x[ipoint]=ipoint+0.1*is;
      ex[ipoint]=0;
    
    }

    TGraphErrors *g1 = new TGraphErrors(npoints,&x[0],&R[0],&ex[0],&eR[0]); //awful syntax to initialize with vectors instead of arrays
    g_Rs.push_back(g1);
    if(make_kappa){
    TGraphErrors *gkappa = new TGraphErrors(npoints_kappa,&xkappa[0],&kappa[0],&exkappa[0],&ekappa[0]); //awful syntax to initialize with vectors instead of arrays
    g_kappas.push_back(gkappa);
    }
  }


  if(make_kappa){
    delkappas.resize(npoints_kappa);
    if(g_kappas.size() ==2){
     for(int ibin=0;ibin<npoints_kappa;ibin++){
       cout<<"kappa loop ibin = "<<ibin<<endl;
       double x1(0),y1(0),x2(0),y2(0);
       g_kappas.at(0)->GetPoint(ibin,x1,y1);
       g_kappas.at(1)->GetPoint(ibin,x2,y2);
       delkappas.at(ibin) = fabs(y1-y2)/y1;
       cout<<Form("delkappa %i = %.3f", ibin+1, delkappas.at(ibin))<<endl;
     }
   }
  }

  
   styles style("RA4"); style.setDefaultStyle();
   TCanvas can;
   TH1F *h;
   h = new TH1F("for_axis_label",cuts2title(baseline),npoints,-0.5,npoints-0.5);  
  
   for (int ih=1;ih<=npoints;ih++){
     int ipoint = ih-1;
     h->GetXaxis()->SetBinLabel(ih,cuts2title(bins_by_region.at(nums_reg_bin[ipoint][0]).at(nums_reg_bin[ipoint][1])));   //adjust indices by 1 for goofy bin numbering
   }
   
   h->SetMaximum(1.5*max);
   h->SetMinimum(0.0);
   h->GetXaxis()->SetLabelSize(0.04);
   h->GetYaxis()->SetTitle(axisname);
   h->Draw();

   double legX = 0.65, legY = 0.89, legSingle = 0.14;
   double legW = 0.22, legH = legSingle;
   TLegend leg(legX, legY-legH, legX+legW, legY);
   leg.SetTextSize(0.057); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
   leg.SetTextFont(132);

   //int color[2] = {8,9};
   //  int ptrstyle[2] = {20,22};
   // TString labels[2] = {"Nominal",sysname+" Variation"};
   vector<TString> labels;
   labels.push_back("Nominal");
   for(unsigned int i=1;i<sysnames.size();i++){
     if(i!=4) labels.push_back(sysnames[i]+" Variation");
     if(i==4) labels.push_back("top p_{T} Variation");
   }
   for(unsigned int ivar=0; ivar<sysindices.size(); ivar++){
     int isys = sysindices.at(ivar);
     // if(ivar>1) cout<<"need more colors"<<endl;
     /*  g_Rs.at(isys)->SetMarkerStyle(ptrstyle[isys]);
     g_Rs.at(isys)->SetMarkerColor(color[isys]);
     g_Rs.at(isys)->SetLineColor(color[isys]);*/
     if(isys>0){
       g_Rs.at(ivar)->SetMarkerStyle(22);
       g_Rs.at(ivar)->SetMarkerColor(9);
       g_Rs.at(ivar)->SetLineColor(9);

     }
     else{
       g_Rs.at(ivar)->SetMarkerStyle(20);
       g_Rs.at(ivar)->SetMarkerColor(8);
       g_Rs.at(ivar)->SetLineColor(8);
     }
     g_Rs.at(ivar)->SetMarkerSize(2);
     g_Rs.at(ivar)->Draw("PZ");
     leg.AddEntry(g_Rs.at(ivar), labels[isys],"p");
   }
 

   TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
   line.DrawLine(3.5, 0, 3.5, 1.5*max);
   leg.Draw("p");

   TString external_cut="";
   if(axisname.Contains("{MJ}")) external_cut="m_{T}";
   if(axisname.Contains("{mT}")) external_cut="M_{J}";
     
   TLatex *text1 = new TLatex(0.35,0.03,"high "+external_cut);
   text1->SetNDC();
   text1->SetTextSize(0.04);
   text1->SetLineWidth(2);
   text1->Draw();
   TLatex *text2 = new TLatex(0.7,0.03,"low "+external_cut);
   text2->SetNDC();
   text2->SetTextSize(0.04);
   text2->SetLineWidth(2);
   text2->Draw();

   TString pname = "plots/systematics/"+format_tag(axisname+"_"+method+"_"+sysname+baseline)+".pdf";
   can.SaveAs(pname);
  
   for(unsigned int ivar=0; ivar<g_Rs.size(); ivar++){
     g_Rs.at(ivar)->Delete();
   }
   h->Delete();


   if(make_kappa){
   TCanvas can3;
   TH1F *h3 = new TH1F("for_axis_label2",cuts2title(baseline),npoints_kappa,-0.5,npoints_kappa-0.5);
   for (unsigned int ih=1;ih<=bins_by_region.at(3).size();ih++){
     h3->GetXaxis()->SetBinLabel(ih,cuts2title(bins_by_region.at(3).at(ih-1)));
   }

    h3->SetMaximum(1.5*maxkappa);
    h3->SetMinimum(0.0);
    h3->GetXaxis()->SetLabelSize(0.05);
    h3->GetYaxis()->SetTitle("Kappa");
    h3->Draw();
    
    TLegend leg3(legX, legY-legH, legX+legW, legY);
    leg3.SetTextSize(0.057); leg3.SetFillColor(0); leg3.SetFillStyle(0); leg3.SetBorderSize(0);
    leg3.SetTextFont(132);
  
    for(unsigned int ivar=0; ivar<sysindices.size(); ivar++){
      int isys = sysindices.at(ivar);
      if(isys>1) cout<<"need more colors"<<endl;
      /* g_kappas.at(isys)->SetMarkerStyle(ptrstyle[isys]);
      g_kappas.at(isys)->SetMarkerColor(color[isys]);
      g_kappas.at(isys)->SetLineColor(color[isys]);*/

      
      if(isys>0){
	g_kappas.at(ivar)->SetMarkerStyle(22);
	g_kappas.at(ivar)->SetMarkerColor(9);
	g_kappas.at(ivar)->SetLineColor(9);

      }
      else{
	g_kappas.at(ivar)->SetMarkerStyle(20);
	g_kappas.at(ivar)->SetMarkerColor(8);
	g_kappas.at(ivar)->SetLineColor(8);
      }
       
      g_kappas.at(ivar)->SetMarkerSize(2);
      g_kappas.at(ivar)->Draw("PZ");
      leg3.AddEntry(g_kappas.at(ivar), labels[isys],"p");
    }
    leg3.Draw("p");

    TString pname3 = "plots/systematics/kappas_"+format_tag(method+"_"+sysname+baseline)+".pdf";
    can3.SaveAs(pname3);
 
    for(unsigned int ivar=0; ivar<g_kappas.size(); ivar++){
      g_kappas.at(ivar)->Delete();
    }
    h3->Delete();

   }
}



double sqr(double x){
  return x*x;
}




