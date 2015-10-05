// plot_kappa: Plots kappa for the different fitting methods.
//             Uncertainties found fluctuating yields with Gamma distributions

#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>

#include <unistd.h>
#include <getopt.h>

#include "TMath.h"
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TLatex.h"
#include "TArrow.h"
#include "TGraphAsymmErrors.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace  {
  bool do_rmt(true);
  bool do_rmj(!(do_rmt)); // if do_rmt is true, do_rmj is false and vice versa
  TString ntuple_date("2015_09_14");
  TString lumi("10");
  int method(1);
  int nrep = 100000;    // Fluctuations of Gamma distribution
  bool do_2l(false);
  bool do_1ltt(false); // Kappa just for 1l ttbar
  bool do_2ltt(false); // Kappa just for 2l ttbar
  bool do_ttbar(true); // Include ttbar in kappa
  bool do_other(false); // Include other in kappa
  TString plot_type=".pdf";
  bool do_sigma_avError(true); 
}

using namespace std;

void plotKappa(vector<vector<double> > &vx, vector<vector<double> > &vy, vector<vector<double> > &vexl, 
	       vector<vector<double> > &vexh, vector<vector<double> > &veyl, vector<vector<double> > &veyh,
	       unsigned idata, TH1D &histo, vector<TString> &nbcuts);
int main(int argc, char *argv[]){ 
  float time_setup(0.), time_ntu(0.), time_gen(0.);
  time_t begtime, endtime;
  time(&begtime);

  int c(0);
  while((c=getopt(argc, argv, "r:m:n:to12"))!=-1){
    switch(c){
    case 'r':
      if(strcmp(optarg,"mt")==0){ do_rmt = true; do_rmj = false; } 
      else if(strcmp(optarg,"mj")==0){ do_rmt = false; do_rmj = true; }
      else { cout<<"-r flag needs to be either \"mt\" or \"mj\" for Rmt or Rmj, respectively"<<endl; exit(0); }
      break;
    case 'm':
      method=atoi(optarg);
      if(method!=1&&method!=3){ cout<<"-m flag can only be 1 or 3 (no other methods supported)"<<endl; exit(0); }
      break;
    case 'n':
      nrep=atoi(optarg);
      break;
    case 't':
      do_ttbar = true;
      break;
    case '1':
      do_1ltt = true;
      break;
    case '2':
      do_2ltt = true;
      break;
    case 'o':
      do_other = true;
      break;
    default:
      break;
    }
  }

  styles style("RA4"); //style.LabelSize = 0.05;
  style.setDefaultStyle();

  TString folder="/cms5r0/rohan/archive/"+ntuple_date+"/skim/";
  TString folder_1l="/cms5r0/rohan/archive/"+ntuple_date+"/skim_1l/";
  TString folder_2l="/cms5r0/rohan/archive/"+ntuple_date+"/skim_2l/";
  TString folder_genht="/cms5r0/rohan/archive/"+ntuple_date+"/skim_genht/";

  vector<TString> s_tt;
  if(do_1ltt) {
    s_tt.push_back(folder_genht+"*_TTJets*SingleLept*");
    s_tt.push_back(folder_1l+"*_TTJets*HT*");
  }
  else if(do_2ltt){
    s_tt.push_back(folder_genht+"*_TTJets*DiLept*");
    s_tt.push_back(folder_2l+"*_TTJets*HT*");
  }
  else{
    s_tt.push_back(folder_genht+"*_TTJets*Lept*");
    s_tt.push_back(folder+"*_TTJets*HT*");
  }
  // Reading ntuples
  vector<sfeats> Samples;
  Samples.push_back(sfeats(s_tt, "t#bar{t}", 46,1));

  // Adding non-skimmed samples
  vector<int> ra4_sam, ra4_sam_ns;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
    //    ra4_sam_ns.push_back(nsam+sam);
    //    vector<TString> sam_files = Samples[sam].file;
    //    for(unsigned ifile(0); ifile < sam_files.size(); ifile++)
    //      sam_files[ifile].ReplaceAll(folder, folder_ns);
    //    Samples.push_back(sfeats(sam_files, Samples[sam].label, Samples[sam].color, Samples[sam].style,
    //			     Samples[sam].cut));
  } // Loop over samples

  // Reading ntuples
  vector<TChain *> chain;
  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }
 
  TString mjthresh("400");
  if(method==1) mjthresh = "600";
  float mSigma, pSigma;
  vector<float> powersk;
  vector<TString> cuts;
  if(do_rmt){
    powersk.push_back(-1);   cuts.push_back("mt<=140"); // R1 & R2
    powersk.push_back(1);  cuts.push_back("mt>140");    // R3 & R4
  } else if(do_rmj) {
    powersk.push_back(-1); cuts.push_back("mj<="+mjthresh);  // R1 & R3
    powersk.push_back(1);  cuts.push_back("mj>"+mjthresh);   // R2 & R4
  }

  TString baseline("ht>500&&met>200&&njets>=7&&nbm>=2&&nleps==1");
  vector<TString> njcuts, metcuts, extcuts, njnames, metnames;
  njcuts.push_back("njets<=8");
  njcuts.push_back("njets>=9"); 
  if(method==1){
    metcuts.push_back("met<=400");
    metcuts.push_back("met>400");
  } else if(method==3) {
    metcuts.push_back("met<=400&&nbm==2");
    metcuts.push_back("met<=400&&nbm>=3");
    metcuts.push_back("met>400");
  }
  if(do_rmt){
    extcuts.push_back("mj>"+mjthresh);
    extcuts.push_back("mj<="+mjthresh);        
  }
  else if(do_rmj){
    extcuts.push_back("mt>140");
    extcuts.push_back("mt<=140");    
  }
  for(unsigned inj(0); inj<njcuts.size(); inj++){
    njnames.push_back(njcuts[inj]);
    njnames[inj].ReplaceAll("<="," #leq ");
    njnames[inj].ReplaceAll("=="," = ");
    njnames[inj].ReplaceAll(">"," > ");
  }

  for(unsigned imet(0); imet<metcuts.size(); imet++){
    metnames.push_back(metcuts[imet]);
    metnames[imet].ReplaceAll("met<=400&&nbm==2","met#leq400,n_{b}=2");
    metnames[imet].ReplaceAll("met<=400&&nbm>=3","met#leq400,n_{b}#geq3");
    metnames[imet].ReplaceAll("met>400","met>400");
    metnames[imet].ReplaceAll("njets","n_{j}");
    metnames[imet].ReplaceAll(">="," #geq ");
    metnames[imet].ReplaceAll("<="," #leq ");
    metnames[imet].ReplaceAll("=="," = ");
  }

  float minh(0), maxh(njcuts.size()*metcuts.size()), wtot(maxh-minh);
  float wmet(wtot/static_cast<float>(njcuts.size()));
  float wnj(wmet/static_cast<float>(metcuts.size()));
  float wnb(wnj/static_cast<float>(extcuts.size()+4));
  // These vectors have indices vx[4][nbsize][njsize*metsize]
  // The first index is: 0 -> k MC, 1 -> k data, 2 -> N4 MC, 3 -> N4 data
  vector<vector<vector<double> > > vx, vy, vexl, vexh, veyl, veyh;
  for(unsigned idata(0); idata<4; idata++){
    vx.push_back (vector<vector<double> >());
    vy.push_back (vector<vector<double> >());
    vexl.push_back(vector<vector<double> >());
    vexh.push_back(vector<vector<double> >());
    veyl.push_back(vector<vector<double> >());
    veyh.push_back(vector<vector<double> >());    
    for(unsigned iext(0); iext<extcuts.size(); iext++){
      vx[idata].push_back (vector<double>());
      vy[idata].push_back (vector<double>());
      vexl[idata].push_back(vector<double>());
      vexh[idata].push_back(vector<double>());
      veyl[idata].push_back(vector<double>());
      veyh[idata].push_back(vector<double>());
    }
  }

  time(&endtime); time_setup = difftime(endtime, begtime);
  time(&begtime);
  
  TString totcut("");
  for(unsigned inj(0); inj<njcuts.size(); inj++){
    for(unsigned imet(0); imet<metcuts.size(); imet++){
      for(unsigned iext(0); iext<extcuts.size(); iext++){
	vector<vector<float> > entries;
	vector<vector<float> > weights;
	for(unsigned obs(0); obs < powersk.size(); obs++) {
	  entries.push_back(vector<float>());
	  weights.push_back(vector<float>());
	  float yield_singlet(0);
	  for(unsigned sam(0); sam < ra4_sam.size(); sam++) {
	    totcut = (lumi+"*weight*("+baseline+"&&"+extcuts[iext]+"&&"+metcuts[imet]+"&&"+cuts[obs]+
		      "&&"+Samples[ra4_sam[sam]].cut);
	    if(method==1 || (do_rmj&&obs%2==1) || (do_rmt&&iext%2==0)) totcut += "&&"+njcuts[inj];
	    totcut += ")";
	    //cout << totcut<<endl;
	    double yield(0.), sigma(0.), avWeight(1.);
	    int Nentries(0);
	    Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
	    //cout<<Nentries<<endl;
	    // Zero-ing out the single t, not adding its uncertainty
	    if(Samples[ra4_sam[sam]].label=="Single t"){
	      if(yield>0) yield_singlet = yield;
	      continue;
	    }
	    if(Samples[ra4_sam[sam]].label=="Other") yield += yield_singlet;
	    if(yield<=0) entries[obs].push_back(0);
	    else {
	      if(do_sigma_avError) entries[obs].push_back(yield*yield/pow(sigma,2));	    
	      else entries[obs].push_back(Nentries);	    
	    }
	    if(Nentries==0){ // If no entries, find average weight in signal bin
	      totcut = (lumi+"*weight*("+baseline+"&&"+cuts[obs]+")");
	      Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
	      // If no entries, find average weight in baseline region
	      if(Nentries==0){
		totcut = (lumi+"*weight*("+baseline+")");
		Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
	      }
	    }
	    if(do_sigma_avError) avWeight = sigma*sigma/yield;
	    else avWeight = fabs(yield/static_cast<double>(Nentries));
	    weights[obs].push_back(avWeight);
	    //cout<<obs<<","<<sam<<": entries "<<entries[obs][sam]<<", weight "<<avWeight<<", yield "<<yield<<endl;
	  } // Loop over samples
	} // Loop over number of observables going into kappa
   
	time(&endtime); time_ntu += difftime(endtime, begtime);
	time(&begtime);
	for(unsigned idata(0); idata<1; idata++){
	  if(do_2l && idata>=2) continue;
	  double kappa(0);
	  kappa = calcKappa(entries, weights, powersk, mSigma, pSigma, (idata%2)==1, false);  
	  float xpoint = imet*wnj+inj*wmet+(iext+2)*wnb;
	  if(method==3 && iext==3) xpoint = imet*wnj+inj*wmet+(iext)*wnb;
	  vx[idata][iext].push_back(xpoint);
	  vy[idata][iext].push_back(kappa);
	  vexl[idata][iext].push_back(0);
	  vexh[idata][iext].push_back(0);
	  veyl[idata][iext].push_back(mSigma);
	  veyh[idata][iext].push_back(pSigma);
	} // Loop over MC and data
	time(&endtime); time_gen += difftime(endtime, begtime);
	time(&begtime);	
      } // Loop over nb cuts
    } // Loop over met cuts
  } // Loop over nj cuts


  vector<unsigned> ind(extcuts.size(),0);
  if(do_2l) {
    for(unsigned inj(0); inj<njcuts.size(); inj++){
      for(unsigned imet(0); imet<metcuts.size(); imet++){
	unsigned iext(0);
	if(method==3 && ((inj==0&&iext==3) || (inj==1&& (iext==1||iext==2)))) continue;
	float MC(vy[0][iext][ind[iext]]), Data(vy[1][iext][ind[iext]]);
	float epMC(veyh[0][iext][ind[iext]]), emMC(veyl[0][iext][ind[iext]]);
	float epData(veyh[1][iext][ind[iext]]), emData(veyl[1][iext][ind[iext]]);
	float epTotal(sqrt(pow(epMC/MC,2)+pow(epData/Data,2))), emTotal(sqrt(pow(emMC/MC,2)+pow(emData/Data,2)));

	njcuts[inj].ReplaceAll("met>200&&","");
	TString cutname = metcuts[imet]+", "+njcuts[inj];
	if(method==3) cutname += ", "+extcuts[iext];

	cout<<endl<<cutname<<": Data +"<<RoundNumber(epData*100,1,Data)<<"% -"<<RoundNumber(emData*100,1,Data)
	    <<"%\t  MC +"<<RoundNumber(epMC*100,1,MC)<<"% -"<<RoundNumber(emMC*100,1,MC)
	    <<"% \t  Total +"<<RoundNumber(epTotal*100,1)<<"% -"<<RoundNumber(emTotal*100,1)<<endl;
	ind[iext]++;
      }
    }  // Loop over nj cuts
    return 0;
  }

  TH1D histo("histo",cuts2title(baseline),metcuts.size()*njcuts.size(), minh, maxh);
  if(method==3) histo.SetLabelSize(0.047,"X");
  for(unsigned inj(0); inj<njcuts.size(); inj++)
  for(unsigned imet(0); imet<metcuts.size(); imet++)    
    histo.GetXaxis()->SetBinLabel(1+imet+inj*metcuts.size(), metnames[imet]);
  for(unsigned idata(0); idata<1; idata++)
    plotKappa(vx[idata], vy[idata], vexl[idata], vexh[idata], veyl[idata], veyh[idata], idata, histo, extcuts);


  time(&endtime); time_setup += difftime(endtime, begtime);
  time(&begtime);	
  cout<<endl<<"Total time: set up "<<time_setup<<" s, finding yields "<<time_ntu
      <<" s, toys "<<time_gen<<" s"<<endl<<endl; 
}

void plotKappa(vector<vector<double> > &vx, vector<vector<double> > &vy, vector<vector<double> > &vexl, 
	       vector<vector<double> > &vexh, vector<vector<double> > &veyl, vector<vector<double> > &veyh,
	       unsigned idata, TH1D &histo, vector<TString> &extcuts){

  bool do_data((idata%2)==1), do_pred(true);
  styles style("RA4"); //style.LabelSize = 0.05;
  style.setDefaultStyle();
  float max_axis(3.2), max_kappa(0.);
  unsigned nbsize(vx.size());
  float minh(histo.GetBinLowEdge(1)), maxh(histo.GetBinLowEdge(histo.GetNbinsX()+1));
  float wtot(maxh-minh);
  for(unsigned iext(0); iext<nbsize; iext++){
    for(unsigned ik(0); ik<vy[iext].size(); ik++){
      if((vy[iext][ik]+veyh[iext][ik]) > max_kappa) max_kappa = vy[iext][ik]+veyh[iext][ik];
    }
  }
  max_axis = max_kappa*1.1;
  TCanvas can;
  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(3);
  histo.Draw();
  TString ytitle("#kappa^{MC}"); 
  if(do_pred&&do_rmt) ytitle = "R_{mT}"; 
  else if(do_pred&&do_rmj) ytitle = "R_{MJ}"; 
  histo.SetYTitle(ytitle);
  if(false)  histo.SetMaximum(max_axis);
  if(do_rmt) histo.SetMaximum(0.25);
  if(do_rmj) histo.SetMaximum(1.5);
  style.moveYAxisLabel(&histo, 1000, false);
  if(!do_pred) line.DrawLine(minh, 1, maxh, 1);
  line.SetLineColor(1); line.SetLineWidth(2); 
  if(do_rmt)  line.DrawLine(minh+wtot/2., 0, minh+wtot/2, 0.25);
  if(do_rmj)  line.DrawLine(minh+wtot/2., 0, minh+wtot/2, 1.5);

  double legX(style.PadLeftMargin+0.0), legY(0.88), legSingle = 0.052;
  double legW = 0.29, legH = legSingle*nbsize;
  if(nbsize>3) legH = legSingle*((nbsize+1)/2);
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(style.LegendSize); leg.SetFillColor(0); 
  leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(style.nFont); 
  if(nbsize>3) leg.SetNColumns(2);
  TGraphAsymmErrors graph[20];
  int colors[] = {31,46,kMagenta+2, kGreen+3}, styles[] = {20, 21, 22, 23};
  for(unsigned iext(0); iext<nbsize; iext++){
    graph[iext] = TGraphAsymmErrors(vx[iext].size(), &(vx[iext][0]), &(vy[iext][0]), 
				   &(vexl[iext][0]), &(vexh[iext][0]), &(veyl[iext][0]), &(veyh[iext][0]));
    graph[iext].SetMarkerStyle(styles[iext]); graph[iext].SetMarkerSize(1.4); 
    graph[iext].SetMarkerColor(colors[iext]); graph[iext].SetLineColor(colors[iext]);
    graph[iext].Draw("p same");   
    extcuts[iext].ReplaceAll("mt","m_{T}");
    extcuts[iext].ReplaceAll("mj","MJ");
    extcuts[iext].ReplaceAll("=="," = ");
    extcuts[iext].ReplaceAll("<="," #leq ");
    extcuts[iext].ReplaceAll(">"," > ");
    leg.AddEntry(&graph[iext], extcuts[iext], "p");
 }
  leg.Draw();
  TLatex label;
  label.SetNDC(kTRUE); label.SetTextAlign(22); label.SetTextColor(1);
  TString cutname;
  label.DrawLatex(0.37,0.03,"7 #leq n_{jets} #leq 8");
  label.DrawLatex(0.73,0.03,"n_{jets} #geq 9");

  can.SetGridy(1);

  TString pname;
  if(do_rmt) pname = "plots/rmt"; 
  if(do_rmj) pname = "plots/rmj"; 
  pname += "_"+ntuple_date; pname += "_method"+TString::Itoa(method,10);
  if(do_1ltt) pname += "_1ltt";
  else {
    if(do_2ltt) pname += "_2ltt";
    else {
      if(do_ttbar) pname += "_tt";
      if(do_other) {
	pname += "_other";
      }
    }
  }
  if(do_data) pname += "_data";
  else  pname += "_mc";
  pname += plot_type;
  if(do_pred) pname.ReplaceAll("kappa","npred");
  can.SaveAs(pname);

}
