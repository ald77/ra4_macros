// plot_trigger: Plots for the trigger section of the RA4 note

#include <iostream>
#include <vector>
#include <ctime>
//#include <random>
#include <boost/random.hpp>
#include <boost/random/gamma_distribution.hpp>

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

// yields[Nobs][Nsam] has the entries for each sample for each observable going into kappa
// weights[Nobs][Nsam] has the average weight of each observable for each sample
// powers[Nobs] defines kappa = Sum_obs{ Sum_sam{yields[sam][obs]*weights[sam][obs]}^powers[obs] }
double calcKappa(vector<vector<int> > &yields, vector<vector<float> > &weights, vector<float> &powers,
		 float &mSigma, float &pSigma);
double gsl_ran_gamma (const double a, const double b);
double intGaus(double mean, double sigma, double minX, double maxX);

int main(){ 
  vector<vector<int> > yields;
  vector<vector<float> > weights;
  vector<float> powers;

  yields.push_back(vector<int>());
  yields[0].push_back(10);
  yields.push_back(vector<int>());
  yields[1].push_back(10);

  weights.push_back(vector<float>());
  weights[0].push_back(1.);
  weights.push_back(vector<float>());
  weights[1].push_back(1.);

  powers.push_back(1.);
  powers.push_back(-1.);

  time_t begtime, endtime;
  time(&begtime);


  float mSigma, pSigma;
  calcKappa(yields, weights, powers, mSigma, pSigma);
  time(&endtime);
  cout<<endl<<"Calculation took "<<difftime(endtime, begtime)<<" seconds"<<endl<<endl; 
}

double calcKappa(vector<vector<int> > &yields, vector<vector<float> > &weights, vector<float> &powers,
		 float &mSigma, float &pSigma){
  int nrep = 1000000, nbadk(0);
  TCanvas can;
  vector<float> fKappas;
  double mean(0.), bignum(1e10);
  for(int rep(0); rep < nrep; rep++) {
    fKappas.push_back(1.);
    bool infKappa(false);
    for(unsigned obs(0); obs < powers.size(); obs++) {
      float observed(0.);
      for(unsigned sam(0); sam < yields[obs].size(); sam++) {
	// Using a flat prior, the expected average of the Poisson with N observed is Gamma(N+1,1)
	observed += gsl_ran_gamma(yields[obs][sam]+1,1)*weights[obs][sam];
      } // Loop over samples
      if(observed <= 0 && powers[obs] < 0) infKappa = true;
      else fKappas[rep] *= pow(observed, powers[obs]);
    } // Loop over number of observables going into kappa
    if(infKappa && fKappas[rep]==0) nbadk++;
    else {
      if(infKappa) fKappas[rep] = bignum;
      else mean += fKappas[rep];
    }
  } // Loop over fluctuations (repetitions)
  int ntot(nrep-nbadk);
  mean /= static_cast<double>(ntot);

  sort(fKappas.begin(), fKappas.end());
  double gSigma = intGaus(0,1,0,1);
  int iMedian((nrep-nbadk+1)/2-1);
  int imSigma(iMedian-static_cast<int>(gSigma*ntot)), ipSigma(iMedian+static_cast<int>(gSigma*ntot));
  float median(fKappas[iMedian]);
  mSigma = median-fKappas[imSigma]; pSigma = fKappas[ipSigma]-median;

  int nbins(100);
  double minH(median-3*mSigma), maxH(median+3*pSigma);
  if(minH < fKappas[0]) minH = fKappas[0];
  if(maxH > fKappas[ntot-1]) maxH = fKappas[ntot-1];
  TH1D histo("h","",nbins, minH, maxH);
  for(int rep(0); rep < ntot; rep++) 
      histo.Fill(fKappas[rep]);   
  //histo.SetBinContent(1, histo.GetBinContent(1)+nbadk);
  //histo.SetBinContent(nbins, histo.GetBinContent(nbins)+histo.GetBinContent(nbins+1));
  histo.SetMaximum(histo.GetMaximum()*1.2);
  histo.Draw();
  can.SaveAs("test.eps");

  double mode(histo.GetBinLowEdge(histo.GetMaximumBin()));
  cout<<endl<<"Median kappa = "<<median<<" + "<<pSigma<<" - "<<mSigma<<". Mean = "<<mean<<". Mode = "<<mode<<endl;

  return median;
}

double intGaus(double mean, double sigma, double minX, double maxX){
  return (TMath::Erf((maxX-mean)/sigma/sqrt(2.))-TMath::Erf((minX-mean)/sigma/sqrt(2.)))/2.;
}


// Code from http://www.hongliangjie.com/2012/12/19/how-to-generate-gamma-random-variables/
// Parameter b could be theta...
double gsl_ran_gamma(const double a, const double b){
  TRandom3 rand(0); 

  if (a < 1){
    double u = rand.Uniform(1);
    return gsl_ran_gamma(1.0 + a, b) * pow (u, 1.0 / a);
  }

  double x, v, u;
  double d = a - 1.0 / 3.0;
  double c = (1.0 / 3.0) / sqrt (d);
  
  while (1) {
    do {
      x = rand.Gaus(0, 1.0);
      v = 1.0 + c * x;
    }
    while (v <= 0);
      
    v = v * v * v;
    u = rand.Uniform(1);

    if (u < 1 - 0.0331 * x * x * x * x) 
      break;

    if (log (u) < 0.5 * x * x + d * (1 - v + log (v)))
      break;
  }
    
  return b * d * v;
}
