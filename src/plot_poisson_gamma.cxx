// plot_trigger: Plots for the trigger section of the RA4 note

#include <cstdlib>

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>

#include "TMath.h"
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TRandom3.h"
#include "TGraphAsymmErrors.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;

// entries[Nobs][Nsam] has the entries for each sample for each observable going into kappa
// weights[Nobs][Nsam] has the average weight of each observable for each sample
// powers[Nobs] defines kappa = Product_obs{ Sum_sam{entries[sam][obs]*weights[sam][obs]}^powers[obs] }
double calcPoisson(vector<vector<float> > &entries, vector<vector<float> > &weights, vector<float> &powers, float &uncert);

int main(){ 

  vector<float> yield;
  yield.push_back(1);
  yield.push_back(5);
  yield.push_back(10);
  yield.push_back(100);
  yield.push_back(10000);

  vector<TString> tags;
  tags.push_back("$N0$");
  tags.push_back("$\\frac{N0}{N1}$");
  tags.push_back("$N2\\times\\frac{N0}{N1}$");
  tags.push_back("$\\frac{N2}{N3}\\times\\frac{N0}{N1}$");

  vector<float> ipowers;
  ipowers.push_back(1.);
  ipowers.push_back(-1.);
  ipowers.push_back(1.);
  ipowers.push_back(-1.);

  cout<<"\\begin{table}"<<endl;
  time_t begtime, endtime;
  time(&begtime);
  for(unsigned yind(0); yind<yield.size(); yind++){

    cout<<endl<<"\\begin{tabular}{l | rc | rc | c}\\hline\\hline"<<endl;
    cout<<" & $N_{\\rm Poisson}$ & $\\Delta N_{\\rm P}$ [\\%] "
	<<"& $N_{\\Gamma}$ & $\\Delta N_{\\Gamma}$ [\\%] & "
	<<"$\\Delta N_{\\rm P}/\\Delta N_\\Gamma$ [\\%]\\\\ \\hline"<<endl;
    vector<vector<float> > entries;
    vector<vector<float> > weights;
    vector<float> powers;

    for(unsigned ind(0); ind<ipowers.size(); ind++){
      entries.push_back(vector<float>());
      entries[ind].push_back(yield[yind]);
      weights.push_back(vector<float>());
      weights[ind].push_back(1.);

      TString label(tags[ind]);
      for(unsigned entry(0); entry<entries.size(); entry++){
	TString number(""), reptag("N");
	number += entries[entry][0]; reptag += entry;
	label.ReplaceAll(reptag, number);
      }
      powers.push_back(ipowers[ind]);
      float k_poi, ek_poi, k_gam, epk_gam, emk_gam;
      k_poi = calcPoisson(entries, weights, powers, ek_poi);
      k_gam = calcKappa(entries, weights, powers, emk_gam, epk_gam, false, true);

      int digits(static_cast<int>(2-log10(k_poi)));
      if(digits<0) digits=0;
      cout<<setw(28)<<label<<"   &   $"<<setw(6)<<RoundNumber(k_poi,digits)<<"\\pm"
	  <<RoundNumber(ek_poi,digits)<<"$ & "<<RoundNumber(ek_poi*100,1,k_poi)
	  <<" & $"<<setw(6)<<RoundNumber(k_gam,digits)<<"^{+"<<RoundNumber(epk_gam,digits)
	  <<"}_{-"<<RoundNumber(emk_gam,digits)
	  <<"}$ & ${}^{+"<<RoundNumber(epk_gam*100,1,k_gam)
	  <<"}_{-"<<RoundNumber(emk_gam*100,1,k_gam)<<"}$ & "
	  <<"${}^{+"<<RoundNumber(ek_poi*100,1,epk_gam)
	  <<"}_{-"<<RoundNumber(ek_poi*100,1,emk_gam)<<"}$ \\\\"<<endl;
    }
    cout<<"\\hline\\hline\\end{tabular}\\vspace{0.1in}"<<endl<<endl;
  }
  cout<<"\\end{table}"<<endl<<endl;
  time(&endtime);
  cout<<endl<<"Calculation took "<<difftime(endtime, begtime)<<" seconds"<<endl<<endl; 
}

double calcPoisson(vector<vector<float> > &entries, vector<vector<float> > &weights, vector<float> &powers, float &uncert){
  double kappa(1.);
  for(unsigned obs(0); obs < powers.size(); obs++) {
    float observed(0.);
    for(unsigned sam(0); sam < entries[obs].size(); sam++) 
      observed += entries[obs][sam]*weights[obs][sam];
    if(observed==0 && powers[obs] < 0) return -1.;
    kappa *= pow(observed, powers[obs]);
  }
  uncert = 0;
  for(unsigned obs(0); obs < powers.size(); obs++) {
    float observed(0.);
    for(unsigned sam(0); sam < entries[obs].size(); sam++) 
      observed += entries[obs][sam]*weights[obs][sam];
    if(observed==0 && powers[obs] < 0) return -1.;
    if(observed==0) observed=1;
    uncert += pow(kappa*sqrt(observed+1)/observed, 2);
  }
  uncert = sqrt(uncert);
  return kappa;
}


