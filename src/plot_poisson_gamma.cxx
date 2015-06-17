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
double calcPoisson(vector<vector<int> > &entries, vector<vector<float> > &weights, vector<float> &powers, float &uncert);
double calcKappa(vector<vector<int> > &entries, vector<vector<float> > &weights, vector<float> &powers,
		 float &mSigma, float &pSigma);

int main(){ 

  vector<int> yield;
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
    vector<vector<int> > entries;
    vector<vector<float> > weights;
    vector<float> powers;

    for(unsigned ind(0); ind<ipowers.size(); ind++){
      entries.push_back(vector<int>());
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
      k_gam = calcKappa(entries, weights, powers, emk_gam, epk_gam);

      int digits(2-log10(k_poi));
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

double calcPoisson(vector<vector<int> > &entries, vector<vector<float> > &weights, vector<float> &powers, float &uncert){
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


double calcKappa(vector<vector<int> > &entries, vector<vector<float> > &weights, vector<float> &powers,
		 float &mSigma, float &pSigma){
  styles style("RA4"); style.setDefaultStyle();
  int nrep = 100000, nbadk(0);
  TCanvas can;
  vector<float> fKappas;
  double mean(0.), bignum(1e10);
  // Doing kappa variations
  for(int rep(0), irep(0); rep < nrep; rep++) {
    fKappas.push_back(1.);
    bool Denom_is0(false);
    for(unsigned obs(0); obs < powers.size(); obs++) {
      float observed(0.);
      for(unsigned sam(0); sam < entries[obs].size(); sam++) {
	// Using a flat prior, the expected average of the Poisson with N observed is Gamma(N+1,1)
	//observed += gsl_ran_gamma(2*entries[obs][sam]+1,1/2.)*weights[obs][sam];
	observed += gsl_ran_gamma(entries[obs][sam]+1,1)*weights[obs][sam];
      } // Loop over samples
      if(observed <= 0 && powers[obs] < 0) Denom_is0 = true;
      else fKappas[irep] *= pow(observed, powers[obs]);
    } // Loop over number of observables going into kappa
    if(Denom_is0 && fKappas[irep]==0) {
      fKappas.pop_back();
      nbadk++;
    }else {
      if(Denom_is0) fKappas[irep] = bignum;
      else mean += fKappas[irep];
      irep++;
    }
  } // Loop over fluctuations of kappa (repetitions)
  int ntot(nrep-nbadk);
  mean /= static_cast<double>(ntot);

  sort(fKappas.begin(), fKappas.end());
  double gSigma = intGaus(0,1,0,1);
  int iMedian((nrep-nbadk+1)/2-1);
  int imSigma(iMedian-static_cast<int>(gSigma*ntot)), ipSigma(iMedian+static_cast<int>(gSigma*ntot));
  float median(fKappas[iMedian]);
  mSigma = median-fKappas[imSigma]; pSigma = fKappas[ipSigma]-median;

  // Finding standard value
  float stdval(1.);
  bool infStd(false);
  for(unsigned obs(0); obs < powers.size(); obs++) {
    float stdyield(0.);
    for(unsigned sam(0); sam < entries[obs].size(); sam++) 
      stdyield += entries[obs][sam]*weights[obs][sam];
    if(stdyield <= 0 && powers[obs] < 0) infStd = true;
    else stdval *= pow(stdyield, powers[obs]);
  } // Loop over number of observables going into kappa
  if(infStd) stdval = median;
  else {
    int istd(0);
    for(int rep(0); rep < ntot; rep++) 
      if(fKappas[rep]>stdval) {istd = rep; break;}
    imSigma = istd-static_cast<int>(gSigma*ntot);
    ipSigma = istd+static_cast<int>(gSigma*ntot);
    if(imSigma<0){ // Adjusting the length of the interval in case imSigma has less than 1sigma
      ipSigma += (-imSigma);
      imSigma = 0;
    }
    if(ipSigma>=ntot){
      imSigma -= (ipSigma-ntot+1);
      ipSigma = ntot-1;
    }
    mSigma = stdval-fKappas[imSigma]; pSigma = fKappas[ipSigma]-stdval;
  }

  int nbins(50);
  double minH(stdval-3*mSigma), maxH(stdval+3*pSigma);
  if(minH < fKappas[0]) minH = fKappas[0];
  if(maxH > fKappas[ntot-1]) maxH = fKappas[ntot-1];
  TH1D histo("h","",nbins, minH, maxH);
  for(int rep(0); rep < ntot; rep++) 
      histo.Fill(fKappas[rep]);   
  //histo.SetBinContent(1, histo.GetBinContent(1)+nbadk);
  //histo.SetBinContent(nbins, histo.GetBinContent(nbins)+histo.GetBinContent(nbins+1));
  histo.Scale(1/histo.Integral());
  histo.SetMaximum(histo.GetMaximum()*1.2);
  histo.SetLineWidth(3);
  histo.Draw();
  histo.SetXTitle("Expected value");
  histo.SetYTitle("Probability");
  //can.SaveAs("test.eps");

  //double mode(histo.GetBinLowEdge(histo.GetMaximumBin()));
  // cout<<endl<<"Std kappa = "<<stdval<<"+"<<pSigma<<"-"<<mSigma<<".   Mean = "<<mean
  //     <<". Mode = "<<mode<<". Median = "<<median<<endl;

  return stdval;
}
