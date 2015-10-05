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
  TString ntuple_date("2015_09_14");
  TString lumi("10");
  int nrep = 100000;    // Fluctuations of Gamma distribution
  bool do_2l(false);  // Calculate dilepton closure stat uncertainty
  bool do_1ltt(false); // Kappa just for 1l ttbar
  bool do_2ltt(false); // Kappa just for 2l ttbar
  bool do_ttbar(true); // Include ttbar in kappa
  bool do_mj200(false);
  TString plot_type=".pdf";
  bool do_sigma_avError(true); 

  TString mjthresh("400"); 
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
  while((c=getopt(argc, argv, "m:n:to12j"))!=-1){
    switch(c){
    case 'm':
      mjthresh=optarg;
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
    case 'j':
      do_mj200 = true;
      break;
    default:
      break;
    }
  }

  styles style("RA4long"); //style.LabelSize = 0.05;
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
  } // Loop over samples

  // Reading ntuples
  vector<TChain *> chain;
  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  float mSigma, pSigma;
  vector<float> powersk;
  vector<TString> cuts;
  if(!do_2l){
    powersk.push_back(-1);   cuts.push_back("mt<=140"); 
    powersk.push_back(1);  cuts.push_back("mt>140"); 
  } else {
    powersk.push_back(-1); cuts.push_back("mj<="+mjthresh);  // R3
    powersk.push_back(1);  cuts.push_back("mj>"+mjthresh);   // R4
  }

  TString baseline("ht>500&&met>200&&(nmus+nels)==1");
  if(do_mj200) baseline += "&&mj>200";
  if(do_2l) baseline = "(nmus+nels)==2&&ht>500&&met>200&&njets>=6&&nbm>=1";
  vector<TString> mjcuts, njcuts, nbcuts, mjnames, njnames;
  mjcuts.push_back("mj<="+mjthresh);
  mjcuts.push_back("mj>"+mjthresh);
  if(!do_2l){
    njcuts.push_back("njets==4");
    njcuts.push_back("njets==5");
    njcuts.push_back("njets==6");
    njcuts.push_back("njets==7");
    njcuts.push_back("njets==8");
    njcuts.push_back("njets==9");
    njcuts.push_back("njets==10");
    njcuts.push_back("njets>=11");
    //njcuts.push_back("njets==11");
    //    njcuts.push_back("njets>=12");
  } else {
    njcuts.push_back("njets<=7");
    njcuts.push_back("njets>=8");
  }
  if(do_2l) nbcuts.push_back("nbm<=2");
  else {
    nbcuts.push_back("nbm==1");
    nbcuts.push_back("nbm>=2");
    //nbcuts.push_back("nbm==2");
    //nbcuts.push_back("nbm>=3");   
  }
  for(unsigned imj(0); imj<mjcuts.size(); imj++){
    mjnames.push_back(mjcuts[imj]);
    mjnames[imj].ReplaceAll("mj","M_{J}");
    mjnames[imj].ReplaceAll("<="," #leq ");
    mjnames[imj].ReplaceAll("=="," = ");
    mjnames[imj].ReplaceAll(">"," > ");
  }

  for(unsigned inj(0); inj<njcuts.size(); inj++){
    njnames.push_back(njcuts[inj]);
    njnames[inj].ReplaceAll("njets","n_{j}");
    njnames[inj].ReplaceAll(">="," #geq ");
    njnames[inj].ReplaceAll("=="," = ");
  }

  float minh(0), maxh(mjcuts.size()*njcuts.size()), wtot(maxh-minh);
  float wmj(wtot/static_cast<float>(mjcuts.size()));
  float wnj(wmj/static_cast<float>(njcuts.size()));
  float wnb(wnj/static_cast<float>(nbcuts.size()+4));
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
    for(unsigned inb(0); inb<nbcuts.size(); inb++){
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
  for(unsigned imj(0); imj<mjcuts.size(); imj++){
    for(unsigned inj(0); inj<njcuts.size(); inj++){
      for(unsigned inb(0); inb<nbcuts.size(); inb++){
	vector<vector<float> > entries;
	vector<vector<float> > weights;
	for(unsigned obs(0); obs < powersk.size(); obs++) {
	  entries.push_back(vector<float>());
	  weights.push_back(vector<float>());
	  float yield_singlet(0);
	  for(unsigned sam(0); sam < ra4_sam.size(); sam++) {
	    totcut = (lumi+"*weight*("+baseline+"&&"+nbcuts[inb]+"&&"+mjcuts[imj]+"&&"+cuts[obs]+
		      "&&"+Samples[ra4_sam[sam]].cut);
	    totcut += "&&"+njcuts[inj]+")";
	    cout << totcut<<endl;
	    double yield(0.), sigma(0.), avWeight(1.);
	    int Nentries(0);
	    Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
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
	  float xpoint = inj*wnj+imj*wmj+(inb+2)*wnb;
	  vx[idata][inb].push_back(xpoint);
	  vy[idata][inb].push_back(kappa);
	  vexl[idata][inb].push_back(0);
	  vexh[idata][inb].push_back(0);
	  veyl[idata][inb].push_back(mSigma);
	  veyh[idata][inb].push_back(pSigma);
	} // Loop over MC and data
	time(&endtime); time_gen += difftime(endtime, begtime);
	time(&begtime);	
      } // Loop over nb cuts
    } // Loop over met cuts
  } // Loop over nj cuts

  vector<unsigned> ind(nbcuts.size(),0);
  if(do_2l) {
    for(unsigned imj(0); imj<mjcuts.size(); imj++){
      for(unsigned inj(0); inj<njcuts.size(); inj++){
	unsigned inb(0);
	float MC(vy[0][inb][ind[inb]]), Data(vy[1][inb][ind[inb]]);
	float epMC(veyh[0][inb][ind[inb]]), emMC(veyl[0][inb][ind[inb]]);
	float epData(veyh[1][inb][ind[inb]]), emData(veyl[1][inb][ind[inb]]);
	float epTotal(sqrt(pow(epMC/MC,2)+pow(epData/Data,2))), emTotal(sqrt(pow(emMC/MC,2)+pow(emData/Data,2)));

	mjcuts[imj].ReplaceAll("mj>200&&","");
	TString cutname = njcuts[inj]+", "+mjcuts[imj];

	cout<<endl<<cutname<<": Data +"<<RoundNumber(epData*100,1,Data)<<"% -"<<RoundNumber(emData*100,1,Data)
	    <<"%\t  MC +"<<RoundNumber(epMC*100,1,MC)<<"% -"<<RoundNumber(emMC*100,1,MC)
	    <<"% \t  Total +"<<RoundNumber(epTotal*100,1)<<"% -"<<RoundNumber(emTotal*100,1)<<endl;
	ind[inb]++;
      }
    }  // Loop over nj cuts
    return 0;
  }

  TH1D histo("histo",cuts2title(baseline),njcuts.size()*mjcuts.size(), minh, maxh);
  for(unsigned imj(0); imj<mjcuts.size(); imj++)
    for(unsigned inj(0); inj<njcuts.size(); inj++)
      histo.GetXaxis()->SetBinLabel(1+inj+imj*njcuts.size(), njnames[inj]);
  for(unsigned idata(0); idata<1; idata++)
    plotKappa(vx[idata], vy[idata], vexl[idata], vexh[idata], veyl[idata], veyh[idata], idata, histo, nbcuts);


  time(&endtime); time_setup += difftime(endtime, begtime);
  time(&begtime);	
  cout<<endl<<"Total time: set up "<<time_setup<<" s, finding yields "<<time_ntu
      <<" s, toys "<<time_gen<<" s"<<endl<<endl; 
}

void plotKappa(vector<vector<double> > &vx, vector<vector<double> > &vy, vector<vector<double> > &vexl, 
	       vector<vector<double> > &vexh, vector<vector<double> > &veyl, vector<vector<double> > &veyh,
	       unsigned idata, TH1D &histo, vector<TString> &nbcuts){

  bool do_data((idata%2)==1), do_pred(true);
  styles style("RA4long"); //style.LabelSize = 0.05;
  style.setDefaultStyle();
  float max_axis(3.2), max_kappa(0.);
  unsigned nbsize(vx.size());
  float minh(histo.GetBinLowEdge(1)), maxh(histo.GetBinLowEdge(histo.GetNbinsX()+1));
  float wtot(maxh-minh);
  for(unsigned inb(0); inb<nbsize; inb++){
    for(unsigned ik(0); ik<vy[inb].size(); ik++){
      if((vy[inb][ik]+veyh[inb][ik]) > max_kappa) max_kappa = vy[inb][ik]+veyh[inb][ik];
    }
  }
  max_axis = max_kappa*1.1;
  TCanvas can;
  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(3);
  histo.Draw();
  TString ytitle("#kappa^{MC}"); 
  if(do_pred) ytitle = "R_{m_{T}}"; 
  histo.SetYTitle(ytitle);
  histo.SetMaximum(max_axis);
  style.moveYAxisLabel(&histo, 1000, false);
  if(!do_pred) line.DrawLine(minh, 1, maxh, 1);
  line.SetLineColor(1); line.SetLineWidth(2); 
  line.DrawLine(minh+wtot/2., 0, minh+wtot/2, max_axis);

  int acolor(kGreen+3);
  float alength((maxh-minh)/8.), yarrow(max_axis/15.), llength(max_axis/1.5);
  TArrow arrow; arrow.SetLineColor(acolor); arrow.SetLineWidth(1); arrow.SetArrowSize(0.02);
  TLatex label; label.SetNDC(kFALSE);label.SetTextAlign(11); label.SetTextColor(acolor);
  float binw(histo.GetBinWidth(1));
  line.SetLineColor(acolor); line.SetLineWidth(1); line.SetLineStyle(2);
  float xarrow(minh+binw*3);
  line.DrawLine(xarrow, 0, xarrow, llength);
  arrow.DrawArrow(xarrow, yarrow, xarrow+alength, yarrow);
  label.DrawLatex(xarrow+0.1, yarrow+max_axis/80., "Baseline");

  xarrow = minh+wtot/2+binw*3;
  line.DrawLine(xarrow, 0, xarrow, llength);
  arrow.DrawArrow(xarrow, yarrow, xarrow+alength, yarrow);
  label.DrawLatex(xarrow+0.1, yarrow+max_axis/80., "Baseline");

  double legX(style.PadLeftMargin+0.0), legY(0.88), legSingle = 0.052;
  //if(do_pred) legX = 0.62;
  double legW = 0.29, legH = legSingle*nbsize;
  if(nbsize>3) legH = legSingle*((nbsize+1)/2);
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(style.LegendSize); leg.SetFillColor(0); 
  leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(style.nFont); 
  if(nbsize>3) leg.SetNColumns(2);
  TGraphAsymmErrors graph[20];
  int colors[] = {2,4,kMagenta+2, kGreen+3}, styles[] = {20, 21, 22, 23};
  for(unsigned inb(0); inb<nbsize; inb++){
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
  label.SetNDC(kTRUE); label.SetTextAlign(22); label.SetTextColor(1);
  TString cutname;
  label.DrawLatex(0.37,0.03,"M_{J} #leq "+mjthresh);
  label.DrawLatex(0.73,0.03,"M_{J} > "+mjthresh);

  TString pname = "plots/ratio_mt"; 
  if(do_1ltt) pname += "_1ltt";
  else {
    if(do_2ltt) pname += "_2ltt";
    else {
      if(do_ttbar) pname += "_tt";
    }
  }
  if(do_mj200) pname += +"_mj200";
  if(do_data) pname += "_data";
  else  pname += "_mc";
  pname += plot_type;
  if(do_pred) pname.ReplaceAll("kappa","npred");
  can.SaveAs(pname);
}

