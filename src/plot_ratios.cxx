#include <iostream>
#include <vector>

#include "TString.h"
#include "TGraphErrors.h"

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TColor.h"
#include "TMath.h"
#include "TF1.h"
#include "TLatex.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;

void GetKappa(double &kappa,double &error, TString baseline,TString external,TString internal_cut,TString ratio_cut, sfeats Sample,TString luminosity);
void GetRatio(double &ratio,double &error, TString baseline,TString external,TString internal_cut,TString ratio_cut, sfeats Sample, TString luminosity);
void MakeGraph(TString baseline,vector<TString> external,TString internal_cut, TString ratio_cut, vector<sfeats> samples, vector<TString> binlabels, TString luminosity);

int main(){
  TString luminosity = "10";
  TString folder="/cms5r0/ald77/archive/2015_05_21/skim/";
  TString folder_noskim="/cms5r0/ald77/archive/2015_05_21/";
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_tt_noskim;
  s_tt_noskim.push_back(folder_noskim+"*_TTJet*12.root");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*WJetsToLNu_HT*");
  vector<TString> s_singlet;
  s_singlet.push_back(folder+"*_T*channel*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder+"*TTW*");
  s_ttv.push_back(folder+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD_HT*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");

  // Reading ntuples
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_t1t, "T1tttt(1500,100)", ra4::c_t1tttt));//0
  Samples.push_back(sfeats(s_t1tc, "T1tttt(1200,800)", ra4::c_t1tttt,2));//1
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", ra4::c_tt_1l, 1,"ntruleps<=1"));//2
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", ra4::c_tt_2l,1,"ntruleps>=2"));//3
  Samples.push_back(sfeats(s_wjets, "W+jets", ra4::c_wjets));//4
  Samples.push_back(sfeats(s_singlet, "Single t", ra4::c_singlet));//5
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));//6
  Samples.push_back(sfeats(s_other, "Other", ra4::c_other));//7
  Samples.push_back(sfeats(s_tt_noskim, "t#bar{t}", ra4::c_tt_1l,1));//8

  Samples.push_back(sfeats(s_tt, "t#bar{t} 5j", 1, 1,"njets==5"));//9
  Samples.push_back(sfeats(s_tt, "t#bar{t} 6j", 2, 1,"njets==6"));//10
  Samples.push_back(sfeats(s_tt, "t#bar{t} 7j", 4, 1,"njets==7"));//11
  Samples.push_back(sfeats(s_tt, "t#bar{t} 8j", 8, 1,"njets==8"));//12
  Samples.push_back(sfeats(s_tt, "t#bar{t} 9+j", 15, 1,"njets>=9"));//13

  Samples.push_back(sfeats(s_tt, "t#bar{t}", 15, 1));//14
  

  vector<int> ra4_sam;
  ra4_sam.push_back(0);
  ra4_sam.push_back(1);
  ra4_sam.push_back(2);
  ra4_sam.push_back(3);
  ra4_sam.push_back(4);
  ra4_sam.push_back(5);
  ra4_sam.push_back(6);
  ra4_sam.push_back(7);

  vector<int> ra4_tt_t1;
  ra4_tt_t1.push_back(0);
  ra4_tt_t1.push_back(1);
  ra4_tt_t1.push_back(2);

  //  const int nR=5;
  TString baseline = "ht>500&&met>200&&nbm>=2&&(nmus+nels)==1";

  vector<TString> externalvec;
  externalvec.push_back("njets>=6&&njets<=7&&met>200&&met<=400&&nbm==2");
  externalvec.push_back("njets>=6&&njets<=7&&met>200&&met<=400&&nbm>=3");
  externalvec.push_back("njets>=6&&njets<=7&&met>400&&nbm>=2");
  externalvec.push_back("njets>=8&&met>200&&met<=400&&nbm==2");
  externalvec.push_back("njets>=8&&met>200&&met<=400&&nbm>=3");
  externalvec.push_back("njets>=8&&met>400&&nbm>=2");

  TString internal_cut = "mj>400"; 
  
  TString ratio_cut = "mt>140";

  vector<sfeats> samples_MJ;
  samples_MJ.push_back(sfeats(s_tt, "t#bar{t}", 31, 1));
  
  vector<TString> binlabels; //push back twice for each njets bin
  binlabels.push_back("n_{B}=2, low MET");binlabels.push_back("n_{B}=3+, low MET");binlabels.push_back("n_{B}=2+, high MET");
  binlabels.push_back("n_{B}=2, low MET");binlabels.push_back("n_{B}=3+, low MET");binlabels.push_back("n_{B}=2+, high MET");

  MakeGraph(baseline,externalvec,internal_cut,ratio_cut,samples_MJ,binlabels,luminosity);  
}

void MakeGraph(TString baseline,vector<TString> external,TString internal_cut,TString ratio_cut, vector<sfeats> samples, vector<TString> binlabels,TString luminosity){
  
  TString not_internal_cut = invertcut(internal_cut);

  int nsamples = samples.size();
  vector<TGraphErrors*> graphs;
  TGraphErrors* kappas;
  float max = 0.01;
  int nR = static_cast<int>(external.size());
  vector<double> x;
  vector<double> x_err;

  for(int isam = 0; isam<nsamples; isam++){
    vector<double> R;
    vector<double> R_err;
  
    for(int iR=0;iR<nR;iR++){ //For one internal_cut
      double rholder;
      double r_errholder;
    
      GetRatio(rholder,r_errholder,baseline,external[iR],internal_cut,ratio_cut,samples.at(isam),luminosity);
      R.push_back(rholder);
      R_err.push_back(r_errholder);
      
      x.push_back(iR);//=iR; //just a placeholder for a label
      x_err.push_back(0);//x_err[iR]=0;
    }    

    TGraphErrors *g1 = new TGraphErrors(nR,&x[0],&R[0],&x_err[0],&R_err[0]);  // &foo[0] convert vector "foo" to an array because TGraphErrors only accepts arrays
    graphs.push_back(g1);

    R.clear();
    R_err.clear();

    for(int iR=0;iR<nR;iR++){ //For the inverse internal_cut
      double rholder;
      double r_errholder;
      
      GetRatio(rholder,r_errholder,baseline,external[iR],not_internal_cut,ratio_cut,samples.at(isam),luminosity);
      R.push_back(rholder);
      R_err.push_back(r_errholder);

      x.push_back(iR);//=iR; //just a placeholder for a label
      x_err.push_back(0);//x_err[iR]=0;
    }
    
    TGraphErrors *g2 = new TGraphErrors(nR,&x[0],&R[0],&x_err[0],&R_err[0]); // &foo[0] converts vector "foo" to an array because TGraphErrors only accepts arrays
    graphs.push_back(g2);
    
    for(int imax = 0;imax<nR;imax++){
      if(R[imax]>max) max=R[imax];
    }
  }
  vector<double> kappa;
  vector<double> kappa_err;
  for(int iR=0;iR<nR;iR++){
    double kholder;
    double k_errholder;

    GetKappa(kholder,k_errholder,baseline,external[iR],internal_cut,ratio_cut,samples.at(0),luminosity);
    kappa.push_back(kholder);
    kappa_err.push_back(k_errholder);    
  }

  kappas = new TGraphErrors(nR,&x[0],&kappa[0],&x_err[0],&kappa_err[0]);
  
  styles style("RA4"); style.setDefaultStyle();
  TCanvas can;
  //TPad *pad = static_cast<TPad *>(can.cd());
  TString plot_tag("_lumi"+luminosity+".eps");

  //make hist to define pad with labels on x-axis
  TH1F *h = new TH1F("for_axis_label",cuts2title(baseline),nR,-0.5,nR-0.5);
  for (int ih=1;ih<=nR;ih++){ h->GetXaxis()->SetBinLabel(ih,cuts2title(external[ih-1]));
    if(external[0].Contains("nbm")) h->GetXaxis()->SetBinLabel(ih,binlabels[ih-1]);
    //    if(external[0].Contains("const")) h->GetXaxis()->SetBinLabel(ih,binlabels3[ih-1]);
  }
  //h->SetMaximum(1.5*max);
  h->SetMaximum(1.5*max);
  h->SetMinimum(0.0);
  if(ratio_cut.Contains("mt")){
    h->GetYaxis()->SetTitle("R_{mT}");
  }
  else {if(ratio_cut.Contains("400")) h->GetYaxis()->SetTitle("R_{MJ 400}");
    if(ratio_cut.Contains("500")) h->GetYaxis()->SetTitle("R_{MJ 500}");
    if(ratio_cut.Contains("600")) h->GetYaxis()->SetTitle("R_{MJ 600}");
  }
  h->GetXaxis()->SetLabelSize(0.03);
  h->Draw();

  double legX = 0.65, legY = 0.89, legSingle = 0.14;
  double legW = 0.22, legH = legSingle;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.057); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);
  
  for(int ig = 0;ig<nsamples;ig++){
    graphs.at(0)->SetMarkerStyle(20);
    graphs.at(0)->SetMarkerColor(31);
    graphs.at(0)->SetLineColor(31);
    graphs.at(0)->Draw("PZ");
    leg.AddEntry(graphs.at(0), samples.at(ig).label+" "+cuts2title(internal_cut),"p");

    graphs.at(1)->SetMarkerStyle(20);
    graphs.at(1)->SetMarkerColor(46);
    graphs.at(1)->SetLineColor(46);
    graphs.at(1)->Draw("PZ");
    leg.AddEntry(graphs.at(1), samples.at(ig).label+" "+cuts2title(not_internal_cut),"p");
  }

  leg.Draw("p");
  
  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
  if(external[0].Contains("nbm")){ line.DrawLine(2.5, 0, 2.5, 1.5*max);
    TLatex *text67 = new TLatex(0.35,0.03,"n_{jets}= 6-7");
    text67->SetNDC();
    text67->SetTextSize(0.03);
    text67->SetLineWidth(2);
    text67->Draw();
    TLatex *text8 = new TLatex(0.7,0.03,"n_{jets}#geq 8");
    text8->SetNDC();
    text8->SetTextSize(0.03);
    text8->SetLineWidth(2);
    text8->Draw();
    
  }
  
  TString pname1 = "plots/ratios/ratios_"+format_tag(ratio_cut+samples.at(0).cut+external[0]+baseline)+".eps";
  can.SaveAs(pname1);
  
  graphs.at(0)->Delete();
  graphs.at(1)->Delete();
  
  //now plot Kappa
  TCanvas can2;
  
  //make hist to define pad with labels on x-axis
  h->SetMaximum(2.0);
  h->GetYaxis()->SetTitle("\\kappa");
  h->Draw();

  kappas->SetMarkerStyle(20);
  kappas->SetMarkerColor(kBlack);
  kappas->SetLineColor(kBlack);
  kappas->Draw("PZ");
  line.DrawLine(h->GetBinLowEdge(1), 1, h->GetBinLowEdge(h->GetNbinsX()+1), 1);

  if(external[0].Contains("nbm")){ line.DrawLine(2.5, 0, 2.5, 1.5*max);
    TLatex *text67 = new TLatex(0.35,0.03,"n_{jets}= 6-7");
    text67->SetNDC();
    text67->SetTextSize(0.03);
    text67->SetLineWidth(2);
    text67->Draw();
    TLatex *text8 = new TLatex(0.7,0.03,"n_{jets}= 8");
    text8->SetNDC();
    text8->SetTextSize(0.03);
    text8->SetLineWidth(2);
    text8->Draw();
  }
  
  TString pname2 = "plots/ratios/kappa_"+format_tag(ratio_cut+samples.at(0).cut+external[0]+baseline)+".eps";
  can2.SaveAs(pname2);
  h->Delete(); 
}

void GetRatio(double &ratio,double &error, TString baseline,TString external,TString internal_cut,TString ratio_cut, sfeats Sample,TString luminosity){
  
  TChain * chain = new TChain("tree");
  for(unsigned insam(0); insam < Sample.file.size(); insam++) 
    chain->Add(Sample.file[insam]);
  
  TH1D h_numerator("num", "",100, 0, 10000);
  h_numerator.Sumw2();
  TH1D h_denominator("den", "",100, 0, 10000);
  h_denominator.Sumw2();
  TH1D h_weight("weight", "",2000, 0, 2);
  h_weight.Sumw2();

  TString numCut = luminosity+"*weight*("+baseline+"&&"+external+"&&"+internal_cut+"&&"+ratio_cut+"&&"+Sample.cut+")";
  TString denCut = luminosity+"*weight*("+baseline+"&&"+external+"&&"+internal_cut+"&&!("+ratio_cut+")&&"+Sample.cut+")";
  
  chain->Project("num", "met", numCut);
  chain->Project("den", "met", denCut);
  chain->Project("weight", "weight*"+luminosity);
  
  double weight = h_weight.GetMean(); // Really shitty way of getting the weight. Only works if all the weights are the same.

  double numerator, denominator,numerator_err, denominator_err;
  numerator = h_numerator.IntegralAndError(0,101,numerator_err);
  denominator = h_denominator.IntegralAndError(0,101,denominator_err);

  if(numerator==0 && denominator==0){
    ratio = 0;
    error = 0;
  }
  else if(numerator==0){ // Error on zero is ~1 event
    numerator = 0; 
    numerator_err = weight; 
  }
  else if(denominator==0){ //Pretend as if you saw 1 event.
    denominator = weight*1.;
    denominator_err = weight;
  }
  ratio = numerator/denominator;
  error = sqrt(pow(numerator_err/denominator,2)+pow(numerator/pow(denominator,2)*denominator_err,2));
}

void GetKappa(double &kappa,double &error, TString baseline,TString external,TString internal_cut,TString ratio_cut, sfeats Sample,TString luminosity){

  if( !(external.Contains(">" )&& internal_cut.Contains(">")) ) 
      cout<<"WARNING: GetKappa needs \"external\" and \"internal_cut\" to use a \">\" sign"<<endl;
  
  TChain * chain = new TChain("tree");
  for(unsigned insam(0); insam < Sample.file.size(); insam++) 
    chain->Add(Sample.file[insam]);
  
  // Note: These yields numberings do not correspond to the regions
  TH1D h_yields1("yields1", "",100, 0, 10000);
  h_yields1.Sumw2();
  TH1D h_yields2("yields2", "",100, 0, 10000);
  h_yields2.Sumw2();
  TH1D h_yields3("yields3", "",100, 0, 10000);
  h_yields3.Sumw2();
  TH1D h_yields4("yields4", "",100, 0, 10000);
  h_yields4.Sumw2();
  TH1D h_weight("weight", "",2000, 0, 2);
  h_weight.Sumw2();

  TString yields1Cut = luminosity+"*weight*("+baseline+"&&"+external+"&&!("+internal_cut+")&&!("+ratio_cut+")&&"+Sample.cut+")";
  TString yields2Cut = luminosity+"*weight*("+baseline+"&&"+external+"&&!("+internal_cut+")&&"+ratio_cut+"&&"+Sample.cut+")";
  TString yields3Cut = luminosity+"*weight*("+baseline+"&&"+external+"&&"+internal_cut+"&&!("+ratio_cut+")&&"+Sample.cut+")";
  TString yields4Cut = luminosity+"*weight*("+baseline+"&&"+external+"&&"+internal_cut+"&&"+ratio_cut+"&&"+Sample.cut+")";
  
  chain->Project("yields1", "met", yields1Cut);
  chain->Project("yields2", "met", yields2Cut);
  chain->Project("yields3", "met", yields3Cut);
  chain->Project("yields4", "met", yields4Cut);
  chain->Project("weight", "weight*"+luminosity);

  double weight = h_weight.GetMean(); // This method could be improved. Only works if all the weights are the same.

  double n1, n2, n3, n4, n1_err, n2_err, n3_err, n4_err;
  n1 = h_yields1.IntegralAndError(0,101,n1_err);
  n2 = h_yields2.IntegralAndError(0,101,n2_err);
  n3 = h_yields3.IntegralAndError(0,101,n3_err);
  n4 = h_yields4.IntegralAndError(0,101,n4_err);

  int nzeroes = 0;
  if(n1==0) nzeroes++;
  if(n2==0) nzeroes++;
  if(n3==0) nzeroes++;
  if(n4==0) nzeroes++;

  if(nzeroes>=2){ // If more than 1 zero, don't calculate since one R-factor is indeterminate
    kappa = 0;
    error = 0;
  }
  else if(n1==0){ // Error on zero is ~1 event
    n1 = 0;
    n1_err = weight;
  }
  else if(n4==0){ // Error on zero is ~1 event
    n4 = 0;
    n4_err = weight;
  }  
  else if(n2==0){ // Pretend as if you saw 1 event
    n2 = weight*1;
    n2_err = weight;
  }
  else if(n3==0){ // Pretend as if you saw 1 event
    n3 = weight*1;
    n3_err = weight;
  }
  kappa = (n4*n1)/(n3*n2);
  error = sqrt(pow((n4/(n2*n3))*n1_err,2)+pow((n1/(n2*n3))*n4_err,2)+pow(((n1*n4)/(pow(n2,2)*n3))*n2_err,2)+pow(((n1*n4)/(n2*pow(n3,2)))*n3_err,2));
}
