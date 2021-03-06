// plot_kappa: Plots kappa for the different fitting methods.
//             Uncertainties found fluctuating yields with Gamma distributions

#include <ctime>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <unistd.h>
#include <vector>

#include "TCanvas.h"
#include "TChain.h"
#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TLine.h"
#include "TString.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace  {
  TString ntuple_date("2015_10_19");
  TString lumi("2.1");
  int method(3);
  int nrep = 100000;    // Fluctuations of Gamma distribution
  bool do_2l(false);  // Calculate dilepton closure stat uncertainty
  bool do_1ltt(false); // Kappa just for 1l ttbar
  bool do_2ltt(false); // Kappa just for 2l ttbar
  bool do_ttbar(true); // Include ttbar in kappa
  bool do_other(false); // Include other in kappa
  bool t_other(false); // Merges single t and other
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
  while((c=getopt(argc, argv, "m:n:toj12"))!=-1){
    switch(c){
    case 'm':
      method=atoi(optarg);
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

  TString folder="/afs/cern.ch/user/m/manuelf/work/babies/"+ntuple_date+"/mc/skim_1lht500met200/";
  
  vector<TString> s_tt, s_bkg;
  if(do_ttbar){
    s_tt.push_back(folder+"*_TTJets*Lept*");
    s_tt.push_back(folder+"*_TTJets*HT*");
  }
  if(do_other){
    s_bkg.push_back(folder+"*_WJetsToLNu*");
    s_bkg.push_back(folder+"*_TTWJets*");
    s_bkg.push_back(folder+"*_TTZTo*");
    s_bkg.push_back(folder+"*_ST_*");
    s_bkg.push_back(folder+"*DYJetsToLL*");
    s_bkg.push_back(folder+"*_QCD_HT*");
    s_bkg.push_back(folder+"*ttHJetTobb*");
    s_bkg.push_back(folder+"*_WWTo*");
    s_bkg.push_back(folder+"*ggZH_HToBB*");
  }
  // Reading ntuples
  vector<sfeats> Samples; 
  if(do_1ltt) Samples.push_back(sfeats(s_tt, "t#bar{t}",46,1,"ntruleps==1"));
  else if(do_2ltt) Samples.push_back(sfeats(s_tt, "t#bar{t}",46,1,"ntruleps==2"));
  else Samples.push_back(sfeats(s_tt, "t#bar{t}",46,1));
  if(do_other) Samples.push_back(sfeats(s_bkg, "Other", 46, 1));

  vector<int> ra4_sam;
  unsigned nsam(Samples.size());
  for(unsigned sam(0); sam < nsam; sam++){
    ra4_sam.push_back(sam);
  }
  
  // Reading ntuples
  vector<TChain *> chain;
  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
    //    cout<<"Label: "<<Samples[sam].label<<" Chains: "<<chain[sam]->GetNtrees()<<" Entries: "<<chain[sam]->GetEntries()<<endl;
  }

  TString mjthresh("400");
  if(method==1) mjthresh = "600";
  float mSigma, pSigma;
  vector<float> powersk, powersn;
  vector<TString> cuts;
  if(!do_2l){
    powersk.push_back(1);  cuts.push_back("mt<=140&&mj<="+mjthresh); // R1
    powersk.push_back(-1); cuts.push_back("mt<=140&&mj>"+mjthresh);  // R2
    powersk.push_back(-1); cuts.push_back("mt>140&&mj<="+mjthresh);  // R3
    powersk.push_back(1);  cuts.push_back("mt>140&&mj>"+mjthresh);   // R4
  } else {
    powersk.push_back(-1); cuts.push_back("mj<="+mjthresh);  // R3
    powersk.push_back(1);  cuts.push_back("mj>"+mjthresh);   // R4
  }

  powersn.push_back(-1);  
  powersn.push_back(1); 
  powersn.push_back(1); 

  TString baseline("ht>500&&met>200&&nbm>=1&&nleps==1&&njets>=6");
  if(do_2l) baseline = "nleps==2&&ht>500&&met>200&&njets>=5&&nbm>=1";
  vector<TString> metcuts, njcuts, nbcuts, metnames;
  metcuts.push_back("met>200&&met<=400");
  metcuts.push_back("met>400");
  if(!do_2l){
    njcuts.push_back("njets<=8");
    njcuts.push_back("njets>=9");    
  } else {
    njcuts.push_back("njets<=7");
    njcuts.push_back("njets>=8");
  }
  if(!do_2l){
    nbcuts.push_back("nbm==1");
    if(method==1) 
      nbcuts.push_back("nbm>=2");
    else {
      nbcuts.push_back("nbm==2");
      nbcuts.push_back("nbm>=3");
      nbcuts.push_back("nbm>=2");    
    }
  }
  
  for(unsigned imet(0); imet<metcuts.size(); imet++){
    metnames.push_back(metcuts[imet]);
    metnames[imet].ReplaceAll("met>","");
    metnames[imet].ReplaceAll("met<=","");
    metnames[imet].ReplaceAll("&&","-");
    if(!metnames[imet].Contains("-")) metnames[imet] += "+";
    metnames[imet] = "#splitline{MET}{"+metnames[imet]+"}";
  }

  float minh(0), maxh(10), wtot(maxh-minh);
  float wnj(wtot/static_cast<float>(njcuts.size()));
  float wmet(wnj/static_cast<float>(metcuts.size()));
  float wnb(wmet/static_cast<float>(nbcuts.size()+4));
  if(method==3) wnb = wmet/static_cast<float>(nbcuts.size()+4-1);
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
  for(unsigned inj(0); inj<njcuts.size(); inj++){
    for(unsigned imet(0); imet<metcuts.size(); imet++){
      for(unsigned inb(0); inb<nbcuts.size(); inb++){
	if(method==3 && ((imet==0&&inb==3) || (imet==1&& (inb==1||inb==2)))) if(!do_2l) continue;
	vector<vector<float> > entries;
	vector<vector<float> > weights;
	for(unsigned obs(0); obs < powersk.size(); obs++) {
	  entries.push_back(vector<float>());
	  weights.push_back(vector<float>());
	  // cout << totcut<<endl;
	  float yield_singlet(0);
	  for(unsigned sam(0); sam < ra4_sam.size(); sam++) {
	    totcut = (lumi+"*weight*("+baseline+"&&"+metcuts[imet]+"&&"+cuts[obs]+
		      "&&"+Samples[ra4_sam[sam]].cut);
	    //cout<<"Cut: "<<totcut<<endl;
	    if(method==1 || obs%2==1) totcut += "&&"+njcuts[inj]+"&&"+nbcuts[inb];
	    totcut += ")";
	    //cout << totcut<<endl;
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
	    //	    cout<<obs<<","<<sam<<": entries "<<entries[obs][sam]<<", weight "<<avWeight<<", yield "<<yield<<endl;
	  } // Loop over samples
	} // Loop over number of observables going into kappa
   
	time(&endtime); time_ntu += difftime(endtime, begtime);
	time(&begtime);
	for(unsigned idata(0); idata<4; idata++){
	  if(do_2l && idata>=2) continue;
	  double kappa(0);
	  if(idata<2) kappa = calcKappa(entries, weights, powersk, mSigma, pSigma, (idata%2)==1, true);  
	  else kappa = calcKappa(entries, weights, powersn, mSigma, pSigma, (idata%2)==1, true);  
	  float xpoint = inj*wnj+imet*wmet+(inb+2)*wnb;
	  if(method==3 && inb==3) xpoint = inj*wnj+imet*wmet+(inb)*wnb;
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
    for(unsigned inj(0); inj<njcuts.size(); inj++){
      for(unsigned imet(0); imet<metcuts.size(); imet++){
	unsigned inb(0);
	if(method==3 && ((imet==0&&inb==3) || (imet==1&& (inb==1||inb==2)))) continue;
	float MC(vy[0][inb][ind[inb]]), Data(vy[1][inb][ind[inb]]);
	float epMC(veyh[0][inb][ind[inb]]), emMC(veyl[0][inb][ind[inb]]);
	float epData(veyh[1][inb][ind[inb]]), emData(veyl[1][inb][ind[inb]]);
	float epTotal(sqrt(pow(epMC/MC,2)+pow(epData/Data,2))), emTotal(sqrt(pow(emMC/MC,2)+pow(emData/Data,2)));

	metcuts[imet].ReplaceAll("met>200&&","");
	TString cutname = njcuts[inj]+", "+metcuts[imet];
	if(method==3) cutname += ", "+nbcuts[inb];
	// cutname.ReplaceAll("njets","n_{\\rm j}");
	// cutname.ReplaceAll("<=","\\leq "); cutname.ReplaceAll(">=","\\geq "); 
	// cutname.ReplaceAll("met","{\\rm MET}"); cutname.ReplaceAll("nbm","n_b");
	// cutname.ReplaceAll("==","="); 

	cout<<endl<<cutname<<": Data +"<<RoundNumber(epData*100,1,Data)<<"% -"<<RoundNumber(emData*100,1,Data)
	    <<"%\t  MC +"<<RoundNumber(epMC*100,1,MC)<<"% -"<<RoundNumber(emMC*100,1,MC)
	    <<"% \t  Total +"<<RoundNumber(epTotal*100,1)<<"% -"<<RoundNumber(emTotal*100,1)<<endl;
	ind[inb]++;
      }
    }  // Loop over nj cuts
    return 0;
  }

  int digits(1), digper(0);
  TString pname = "txt/kappa_method", cutname; pname += method;
  if(do_1ltt) pname += "_1ltt";
  else {
    if(do_2ltt) pname += "_2ltt";
    else {
      if(do_ttbar) pname += "_tt";
      if(do_other) {
	pname += "_other";
	if(t_other) pname += "t";
      }
    }
  }
  pname += ".tex";
  ifstream header("txt/header.tex");
  ifstream footer("txt/footer.tex");
  ofstream file(pname);
  file<<header.rdbuf();
  /////////////////////// TABLE  ///////////////////////////
  file << "{\\renewcommand{\\arraystretch}{1.4}}"<<endl;
  file << "\n\\begin{tabular}[tbp!]{ l | rc | rc | r}\\hline\\hline\n";
  file << " \\multicolumn{1}{c|}{${\\cal L} = "<<lumi<<"$ fb$^{-1}$} ";
  file << " & $N_{\\rm R2}\\frac{N_{\\rm R3}}{N_{\\rm R1}}$ & Data stat. [\\%] \n";
  file << "& $\\kappa^{\\rm MC}$ & MC stat. [\\%] & \\multicolumn{1}{c}{$\\hat{N}_{\\rm R4}$} \\\\ \\hline\n";
  for(unsigned inj(0); inj<njcuts.size(); inj++){
    for(unsigned imet(0); imet<metcuts.size(); imet++){
      for(unsigned inb(1); inb<nbcuts.size(); inb++){
	if(method==3 && ((imet==0&&inb==3) || (imet==1&& (inb==1||inb==2)))) continue;
	metcuts[imet].ReplaceAll("met>200&&","");
	cutname = "$"+njcuts[inj]+", "+metcuts[imet];
	if(method==3) cutname += ", "+nbcuts[inb];
	cutname += "$";
	cutname.ReplaceAll("njets","n_{\\rm j}");
	cutname.ReplaceAll("<=","\\leq "); cutname.ReplaceAll(">=","\\geq "); 
	cutname.ReplaceAll("met","{\\rm MET}"); cutname.ReplaceAll("nbm","n_b");
	cutname.ReplaceAll("==","="); 
	float Kappa(vy[0][inb][ind[inb]]), N4(vy[3][inb][ind[inb]]);
	float epKappa(veyh[0][inb][ind[inb]]), emKappa(veyl[0][inb][ind[inb]]);
	float epN4(veyh[3][inb][ind[inb]]), emN4(veyl[3][inb][ind[inb]]);
	file << cutname << " \t& $" <<RoundNumber(N4,digits)<<"^{+"<<RoundNumber(epN4,digits)
	     <<"}_{-"<<RoundNumber(emN4,digits)<<"}$ & ${}^{+"<<RoundNumber(epN4*100,digper,N4)
	     <<"}_{-"<<RoundNumber(emN4*100,digper,N4)<<"}$ & $"
	     <<RoundNumber(Kappa,digits+1)<<"^{+"<<RoundNumber(epKappa,digits+1)
	     <<"}_{-"<<RoundNumber(emKappa,digits+1)<<"}$ & ${}^{+"<<RoundNumber(epKappa*100,digper,Kappa)
	     <<"}_{-"<<RoundNumber(emKappa*100,digper,Kappa)<<"}$  & $"
	     <<RoundNumber(N4*Kappa,digits)<<"^{+"<<RoundNumber(epN4*Kappa,digits)
	     <<"+"<<RoundNumber(N4*epKappa,digits)<<"}_{-"<<RoundNumber(emN4*Kappa,digits)
	     <<"-"<<RoundNumber(N4*emKappa,digits)<<"}$ \\\\"<<endl;
	ind[inb]++;
      } // Loop over nb cuts
    } // Loop over met cuts
    if(inj==0) file<<"\\hline"<<endl;
  } // Loop over nj cuts


  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  /////////////////////// TABLE  ///////////////////////////
  file<<footer.rdbuf();
  file.close();
  cout<<endl<<"Written "<<pname<<endl;


  TH1D histo("histo",cuts2title(baseline),njcuts.size()*metcuts.size(), minh, maxh);
  for(unsigned inj(0); inj<njcuts.size(); inj++)
    for(unsigned imet(0); imet<metcuts.size(); imet++)
      histo.GetXaxis()->SetBinLabel(1+imet+inj*metcuts.size(), metnames[imet]);
  for(unsigned idata(0); idata<4; idata++)
    plotKappa(vx[idata], vy[idata], vexl[idata], vexh[idata], veyl[idata], veyh[idata], idata, histo, nbcuts);


  time(&endtime); time_setup += difftime(endtime, begtime);
  time(&begtime);	
  cout<<endl<<"Total time: set up "<<time_setup<<" s, finding yields "<<time_ntu
      <<" s, toys "<<time_gen<<" s"<<endl<<endl; 
}

void plotKappa(vector<vector<double> > &vx, vector<vector<double> > &vy, vector<vector<double> > &vexl, 
	       vector<vector<double> > &vexh, vector<vector<double> > &veyl, vector<vector<double> > &veyh,
	       unsigned idata, TH1D &histo, vector<TString> &nbcuts){

  bool do_data((idata%2)==1), do_pred(idata>=2);
  TString stylename = "RA4";
  styles style(stylename);
  style.setDefaultStyle();
  float max_axis(3.2), max_kappa(0.);
  unsigned nbsize(vx.size());
  float minh(histo.GetBinLowEdge(1)), maxh(histo.GetBinLowEdge(histo.GetNbinsX()+1));
  float wtot(maxh-minh);
  for(unsigned inb(0); inb<nbsize; inb++){
    for(unsigned ik(0); ik<vy.size(); ik++){
      if(vy[inb][ik] > max_kappa) max_kappa = vy[inb][ik];
      if(vy[inb][ik] > max_axis && vy[inb][ik]-veyl[inb][ik] < max_axis && !do_pred) {
	veyl[inb][ik] = max_axis-(vy[inb][ik]-veyl[inb][ik]);
	vy[inb][ik] = max_axis;
      }
    }
  }
  if(do_pred) max_axis = max_kappa*1.3;
  TCanvas can;
  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
  histo.Draw();
  TString ytitle("#kappa^{MC}"); 
  if(do_pred) ytitle = "N_{2}#timesN_{3}/N_{1}"; 
  if(do_data) ytitle += " (data uncert.)";
  else ytitle += " (MC uncert.)";
  histo.SetYTitle(ytitle);
  histo.SetMaximum(max_axis);
  style.moveYAxisLabel(&histo, max_axis, false);
  if(!do_pred) line.DrawLine(minh, 1, maxh, 1);
  line.SetLineColor(1); line.SetLineWidth(2); 
  line.DrawLine(minh+wtot/2., 0, minh+wtot/2, max_axis);

  double legX(style.PadLeftMargin+0.03), legY(0.902), legSingle = 0.052;
  if(do_pred) legX = 0.62;
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
  TLatex label; label.SetNDC(kTRUE);label.SetTextAlign(22);
  label.DrawLatex(0.37,0.03,"7 #leq n_{j} #leq 8");
  label.DrawLatex(0.73,0.03,"n_{j} #geq 9");

  TString pname = "plots/kappa_method"; pname += method;
  if(do_1ltt) pname += "_1ltt";
  else {
    if(do_2ltt) pname += "_2ltt";
    else {
      if(do_ttbar) pname += "_tt";
      if(do_other) {
	pname += "_other";
	if(t_other) pname += "t";
      }
    }
  }
  if(do_data) pname += "_data";
  else  pname += "_mc";
  pname += plot_type;
  if(do_pred) pname.ReplaceAll("kappa","npred");
  can.SaveAs(pname);

}
