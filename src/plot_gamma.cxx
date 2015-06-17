// plot_trigger: Plots for the trigger section of the RA4 note

#include <iostream>
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

int main(){ 
  vector<vector<float> > entries;
  vector<vector<float> > weights;
  vector<float> powers;

  entries.push_back(vector<float>());
  entries[0].push_back(170);
  entries.push_back(vector<float>());
  entries[1].push_back(69);
  entries.push_back(vector<float>());
  entries[2].push_back(12);

  entries.push_back(vector<float>());
  entries[0].push_back(29);
  entries.push_back(vector<float>());
  entries[1].push_back(34);
  entries.push_back(vector<float>());
  entries[2].push_back(3);
  entries.push_back(vector<float>());
  entries[3].push_back(33);

  weights.push_back(vector<float>());
  weights[0].push_back(1.);
  weights.push_back(vector<float>());
  weights[1].push_back(1.);
  weights.push_back(vector<float>());
  weights[2].push_back(1.);
  weights.push_back(vector<float>());
  weights[3].push_back(1.);

  powers.push_back(-1.);
  powers.push_back(1.);
   powers.push_back(1.);
  // powers.push_back(-1.);

  time_t begtime, endtime;
  time(&begtime);

  // TCanvas can;
  // vector<double> vx, vy, vexl, vexh, veyl, veyh;
  // vx.push_back(1);
  // vy.push_back(2);
  // vexl.push_back(0.2);
  // vexh.push_back(0.3);
  // veyl.push_back(0.1);
  // veyh.push_back(0.5);
  // TGraphAsymmErrors graph(vx.size(), &(vx[0]), &(vy[0]), &(vexl[0]), &(vexh[0]), &(veyl[0]), &(veyh[0]));
  // graph.SetMarkerStyle(20); graph.SetMarkerSize(0.9);
  // graph.Draw("ap");
  // can.SaveAs("graph.eps");

  float mSigma, pSigma;
  calcKappa(entries, weights, powers, mSigma, pSigma, false, true, true);
  time(&endtime);
  cout<<endl<<"Calculation took "<<difftime(endtime, begtime)<<" seconds"<<endl<<endl; 
}

