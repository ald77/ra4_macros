// plot_kappa: Plots kappa for the different fitting methods.
//             Uncertainties found fluctuating yields with Gamma distributions

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
#include "TLatex.h"
#include "TGraphAsymmErrors.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace  {
  TString ntuple_date("2015_05_25");
  TString lumi("10");
  int method(2);
  int nrep = 10000;    // Fluctuations of Gamma distribution
  bool do_1ltt(false); // Kappa just for 1l ttbar
  bool do_2ltt(false); // Kappa just for 2l ttbar
  bool do_ttbar(true); // Include ttbar in kappa
  bool do_other(true); // Include other in kappa
  bool do_data(false);  // Plots kappa with data statistics
  bool do_pred(false);  // Plots bkg prediction
}

using namespace std;

// yields[Nobs][Nsam] has the entries for each sample for each observable going into kappa
// weights[Nobs][Nsam] has the average weight of each observable for each sample
// powers[Nobs] defines kappa = Product_obs{ Sum_sam{yields[sam][obs]*weights[sam][obs]}^powers[obs] }
double calcKappa(vector<vector<int> > &entries, vector<vector<float> > &weights, vector<float> &powers,
		 float &mSigma, float &pSigma);

int main(){ 
  styles style("RA4"); style.LabelSize = 0.05;
  style.setDefaultStyle();
  TString folder="/cms5r0/ald77/archive/"+ntuple_date+"/skim100/";
  TString folder_ns="/cms5r0/ald77/archive/"+ntuple_date+"/";


  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder+"*_T*channel*");
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD_HT*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");
  s_other.push_back(folder+"*TTW*");
  s_other.push_back(folder+"*TTZ*");
  s_other.push_back(folder+"*WJets*");

  // Reading ntuples
  vector<sfeats> Samples; 
  if(do_1ltt) Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", ra4::c_tt_1l, 1,"ntruleps==1"));
  else {
    if(do_2ltt) Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", ra4::c_tt_2l,1,"ntruleps>=2"));
    else {
      if(do_ttbar) Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", ra4::c_tt_2l,1));
      if(do_other){
	Samples.push_back(sfeats(s_singlet, "Single t", ra4::c_singlet));
	Samples.push_back(sfeats(s_other, "Other", ra4::c_other, 1)); 
      }
    }
  }
  // Adding non-skimmed samples
  vector<int> ra4_sam, ra4_sam_ns;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
    ra4_sam_ns.push_back(nsam+sam);
    vector<TString> sam_files = Samples[sam].file;
    for(unsigned ifile(0); ifile < sam_files.size(); ifile++)
      sam_files[ifile].ReplaceAll(folder, folder_ns);
    Samples.push_back(sfeats(sam_files, Samples[sam].label, Samples[sam].color, Samples[sam].style,
			     Samples[sam].cut));
  } // Loop over samples

  // Reading ntuples
  vector<TChain *> chain;
  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  
  time_t begtime, endtime;
  time(&begtime);

  TString mjthresh("400");
  if(method==1) mjthresh = "600";
  float mSigma, pSigma;
  vector<float> powers;
  vector<TString> cuts;
  if(do_pred){
    powers.push_back(-1);  cuts.push_back("mt<=140&&mj<="+mjthresh); // R1
    powers.push_back(1); cuts.push_back("mt<=140&&mj>"+mjthresh);  // R2
    powers.push_back(1); cuts.push_back("mt>140&&mj<="+mjthresh);  // R3
    //powers.push_back(1);  cuts.push_back("mt>140&&mj>"+mjthresh);   // R4
 } else {
    powers.push_back(1);  cuts.push_back("mt<=140&&mj<="+mjthresh); // R1
    powers.push_back(-1); cuts.push_back("mt<=140&&mj>"+mjthresh);  // R2
    powers.push_back(-1); cuts.push_back("mt>140&&mj<="+mjthresh);  // R3
    powers.push_back(1);  cuts.push_back("mt>140&&mj>"+mjthresh);   // R4
  }

  TString baseline("(nmus+nels)==1&&ht>500&&met>100&&njets>=7&&nbm>=1");
  vector<TString> metcuts, njcuts, nbcuts, metnames;
  // metcuts.push_back("met>100&&met<=200");
  // metcuts.push_back("met>200&&met<=300");
  // metcuts.push_back("met>300&&met<=400");
  metcuts.push_back("met>200&&met<=400");
  metcuts.push_back("met>400");
  njcuts.push_back("njets<=8");
  njcuts.push_back("njets>=9");
  nbcuts.push_back("nbm==1");
  if(method==1 || method==2) nbcuts.push_back("nbm>=2");
  else {
    nbcuts.push_back("nbm==2");
    nbcuts.push_back("nbm>=3");
  }
  for(unsigned imet(0); imet<metcuts.size(); imet++){
    metnames.push_back(metcuts[imet]);
    metnames[imet].ReplaceAll("met>","");
    metnames[imet].ReplaceAll("met<=","");
    metnames[imet].ReplaceAll("&&","-");
    if(!metnames[imet].Contains("-")) metnames[imet] += "+";
    metnames[imet] = "#splitline{MET}{"+metnames[imet]+"}";
  }

  float minh(0), maxh(10), wtot(maxh-minh), max_axis(3.2), max_kappa(0.);
  float wnj(wtot/static_cast<float>(njcuts.size()));
  float wmet(wnj/static_cast<float>(metcuts.size()));
  float wnb(wmet/static_cast<float>(nbcuts.size()+4));
  vector<vector<double> > vx, vy, vexl, vexh, veyl, veyh;
  for(unsigned inb(0); inb<nbcuts.size(); inb++){
    vx.push_back (vector<double>());
    vy.push_back (vector<double>());
    vexl.push_back(vector<double>());
    vexh.push_back(vector<double>());
    veyl.push_back(vector<double>());
    veyh.push_back(vector<double>());
  }

  TString totcut("");
  for(unsigned inj(0); inj<njcuts.size(); inj++){
    for(unsigned imet(0); imet<metcuts.size(); imet++){
      for(unsigned inb(0); inb<nbcuts.size(); inb++){
	vector<vector<int> > entries;
	vector<vector<float> > weights;
	for(unsigned obs(0); obs < powers.size(); obs++) {
	  entries.push_back(vector<int>());
	  weights.push_back(vector<float>());
	  // totcut = lumi+"*weight*("+baseline+"&&"+nbcuts[inb]+"&&"+metcuts[imet]+"&&"+cuts[obs];
	  // if(method==1 || obs%2==1) totcut += "&&"+njcuts[inj];
	  // totcut += ")";
	  // cout << totcut<<endl;
	  for(unsigned sam(0); sam < ra4_sam.size(); sam++) {
	    totcut = (lumi+"*weight*("+baseline+"&&"+nbcuts[inb]+"&&"+metcuts[imet]+"&&"+cuts[obs]+
		      "&&"+Samples[ra4_sam[sam]].cut);
	    if(method==1 || obs%2==1) totcut += "&&"+njcuts[inj];
	    totcut += ")";
	    //cout << totcut<<endl;
	    double yield(0.), sigma(0.), avWeight(1.);
	    int Nentries(0);
	    Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
	    if(do_data) Nentries = static_cast<int>(yield+0.5);
	    entries[obs].push_back(Nentries);
	    if(Nentries==0){ // If no entries, find averate weight in signal bin
	      totcut = (lumi+"*weight*("+baseline+"&&"+cuts[obs]+")");
	      Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
	      // If no entries, find averate weight in baseline region
	      if(Nentries==0){
		totcut = (lumi+"*weight*("+baseline+")");
		Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
	      }
	    }
	    avWeight = fabs(yield/static_cast<double>(Nentries));
	    if(do_data) avWeight = 1.;
	    weights[obs].push_back(avWeight);
	    //cout<<obs<<","<<sam<<": entries "<<entries[obs][sam]<<", weight "<<avWeight<<", yield "<<yield<<endl;
	  } // Loop over samples
	} // Loop over number of observables going into kappa
   
	double kappa = calcKappa(entries, weights, powers, mSigma, pSigma);  
	if(kappa > max_kappa) max_kappa = kappa;
	float xpoint = inj*wnj+imet*wmet+(inb+2)*wnb;
	if(kappa > max_axis && kappa-mSigma < max_axis && !do_pred) {
	  mSigma = max_axis-(kappa-mSigma);
	  kappa = max_axis;
	}
	vx[inb].push_back(xpoint);
	vy[inb].push_back(kappa);
	vexl[inb].push_back(0);
	vexh[inb].push_back(0);
	veyl[inb].push_back(mSigma);
	veyh[inb].push_back(pSigma);

      } // Loop over nb cuts
    } // Loop over met cuts
  } // Loop over nj cuts

  if(do_pred) max_axis = max_kappa*1.3;
  TCanvas can;
  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
  TH1D histo("histo",cuts2title(baseline),njcuts.size()*metcuts.size(), minh, maxh);
  histo.Draw();
  TString ytitle("#kappa for method "); ytitle += method;
  if(do_pred) ytitle.ReplaceAll("#kappa","Predicted N_{bkg}");
  histo.SetYTitle(ytitle);
  histo.SetMaximum(max_axis);
  style.moveYAxisLabel(&histo, max_axis, false);
  if(!do_pred) line.DrawLine(minh, 1, maxh, 1);
  line.SetLineColor(1); line.SetLineWidth(2); 
  line.DrawLine(minh+wtot/2., 0, minh+wtot/2, max_axis);
  for(unsigned inj(0); inj<njcuts.size(); inj++)
    for(unsigned imet(0); imet<metcuts.size(); imet++)
      histo.GetXaxis()->SetBinLabel(1+imet+inj*metcuts.size(), metnames[imet]);

  double legX(style.PadLeftMargin+0.03), legY(0.902), legSingle = 0.052;
  if(do_pred) legX = 0.8;
  double legW = 0.13, legH = legSingle*nbcuts.size();
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(style.LegendSize); leg.SetFillColor(0); 
  leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(style.nFont); 
  TGraphAsymmErrors graph[20];
  int colors[] = {2,4,kMagenta+2}, styles[] = {20, 21, 22};
  for(unsigned inb(0); inb<nbcuts.size(); inb++){
    graph[inb] = TGraphAsymmErrors(vx[inb].size(), &(vx[inb][0]), &(vy[inb][0]), 
				      &(vexl[inb][0]), &(vexh[inb][0]), &(veyl[inb][0]), &(veyh[inb][0]));
    graph[inb].SetMarkerStyle(styles[inb]); graph[inb].SetMarkerSize(1.4); 
    graph[inb].SetMarkerColor(colors[inb]); graph[inb].SetLineColor(colors[inb]);
    graph[inb].Draw("p same");
    nbcuts[inb].ReplaceAll("nbm","n_{b}");
    nbcuts[inb].ReplaceAll("=="," = ");
    nbcuts[inb].ReplaceAll(">="," #geq ");
    leg.AddEntry(&graph[inb], nbcuts[inb], "p");
  }
  leg.Draw();
  TLatex label; label.SetNDC(kTRUE);label.SetTextAlign(22);
  label.DrawLatex(0.37,0.03,"7 #leq n_{j} #leq 8");
  label.DrawLatex(0.73,0.03,"n_{j} #geq 9");

  TString pname = "plots/kappa_method"; pname += method;
  if(do_1ltt) pname += "_1ltt";
  else {
    if(do_2ltt) pname += "_2ltt";
    else {
      if(do_ttbar) pname += "_tt";
      if(do_other) pname += "_other";
    }
  }
  if(do_data) pname += "_data";
  else  pname += "_mc";
  pname += ".eps";
  if(do_pred) pname.ReplaceAll("kappa","npred");
  can.SaveAs(pname);

  time(&endtime);
  cout<<endl<<"Calculation took "<<difftime(endtime, begtime)<<" seconds"<<endl<<endl; 
}

double calcKappa(vector<vector<int> > &entries, vector<vector<float> > &weights, vector<float> &powers,
		 float &mSigma, float &pSigma){
  int nbadk(0);
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
    //cout<<obs<<": stdyield "<<stdyield<<endl;
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
    if(ipSigma>=ntot){ // Adjusting the length of the interval in case ipSigma has less than 1sigma
      imSigma -= (ipSigma-ntot+1);
      ipSigma = ntot-1;
    }
    mSigma = stdval-fKappas[imSigma]; pSigma = fKappas[ipSigma]-stdval;
  }

  int nbins(100);
  double minH(stdval-3*mSigma), maxH(stdval+3*pSigma);
  if(minH < fKappas[0]) minH = fKappas[0];
  if(maxH > fKappas[ntot-1]) maxH = fKappas[ntot-1];
  TH1D histo("h","",nbins, minH, maxH);
  for(int rep(0); rep < ntot; rep++) 
    histo.Fill(fKappas[rep]);   
  //histo.SetBinContent(1, histo.GetBinContent(1)+nbadk);
  //histo.SetBinContent(nbins, histo.GetBinContent(nbins)+histo.GetBinContent(nbins+1));
  histo.SetMaximum(histo.GetMaximum()*1.2);
  histo.Draw();
  //can.SaveAs("test.eps");

  double mode(histo.GetBinLowEdge(histo.GetMaximumBin()));
  cout<<"Std kappa = "<<stdval<<"+"<<pSigma<<"-"<<mSigma<<".   Mean = "<<mean
      <<". Mode = "<<mode<<". Median = "<<median<<endl;

  return stdval;
}

