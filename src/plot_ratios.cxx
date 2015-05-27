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

void GetRatio(double &ratio,double &error, TString baseline,TString external,TString ratio_cut, sfeats Sample, TString luminosity);
void MakeGraph(TString baseline,vector<TString> external,TString ratio_cut, vector<sfeats> samples, TString luminosity);

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
  s_wjets.push_back(folder+"*WJets*");
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
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", ra4::c_tt_1l, 1,
			   "ntruleps<=1"));//2
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", ra4::c_tt_2l,1,
			   "ntruleps>=2"));//3
  Samples.push_back(sfeats(s_wjets, "W+jets", ra4::c_wjets));//4
  Samples.push_back(sfeats(s_singlet, "Single t", ra4::c_singlet));//5
  Samples.push_back(sfeats(s_ttv, "ttV", ra4::c_ttv));//6
  Samples.push_back(sfeats(s_other, "Other", ra4::c_other));//7
  Samples.push_back(sfeats(s_tt_noskim, "t#bar{t}", ra4::c_tt_1l,1));//8

  Samples.push_back(sfeats(s_tt, "t#bar{t} 5j", 1, 1,
			   "njets==5"));//9
  Samples.push_back(sfeats(s_tt, "t#bar{t} 6j", 2, 1,
			   "njets==6"));//10
  Samples.push_back(sfeats(s_tt, "t#bar{t} 7j", 4, 1,
			   "njets==7"));//11
  Samples.push_back(sfeats(s_tt, "t#bar{t} 8j", 8, 1,
			   "njets==8"));//12
  Samples.push_back(sfeats(s_tt, "t#bar{t} 9+j", 15, 1,
			   "njets>=9"));//13

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
  TString baseline_no_lep = "ht>500&&met>200&&nbm>=2";
  TString baseline1b = "ht>500&&met>200&&nbm>=1&&(nmus+nels)==1";
  //TString external[nR] = {"njets==4","njets==5","njets==6","njets==7","njets>=8"};
  vector<TString> externalvec;
  externalvec.push_back("njets==4");externalvec.push_back("njets==5");externalvec.push_back("njets==6");externalvec.push_back("njets==7");externalvec.push_back("njets>=8");
  vector<TString> externalvec_nobjs;
  externalvec_nobjs.push_back("Sum$((fjets_pt>50)*fjets_nconst)==4");externalvec_nobjs.push_back("Sum$((fjets_pt>50)*fjets_nconst)==5");externalvec_nobjs.push_back("Sum$((fjets_pt>50)*fjets_nconst)==6");externalvec_nobjs.push_back("Sum$((fjets_pt>50)*fjets_nconst)==7");externalvec_nobjs.push_back("Sum$((fjets_pt>50)*fjets_nconst)>=8");


  vector<TString> externalvec2;
  externalvec2.push_back("njets>=6&&njets<=7&&nbm==1");externalvec2.push_back("njets>=6&&njets<=7&&nbm==2");externalvec2.push_back("njets>=6&&njets<=7&&nbm>=3");externalvec2.push_back("njets>=8&&nbm==1");externalvec2.push_back("njets>=8&&nbm==2");externalvec2.push_back("njets>=8&&nbm>=3");
  /*vector<TString> cosmetic2;
    cosmetic2.push_back("n_{B}=1, n_{j}=6-7");cosmetic2.push_back("n_{B}=2, n_{j}=6-7");cosmetic2.push_back("n_{B}=3+, n_{j}=6-7");
    cosmetic2.push_back("n_{B}=1, n_{j}=8+");cosmetic2.push_back("n_{B}=2, n_{j}=8+");cosmetic2.push_back("n_{B}=3+, n_{j}=8+");*/
  TString ratio_cut = "mt>140";

  // MakeGraph(baseline,externalvec,ratio_cut,sfeats(s_tt, "t#bar{t}", 15, 1),luminosity);

  vector<sfeats> samples_MJ;
  samples_MJ.push_back(sfeats(s_tt, "t#bar{t} M_{J}>400", 31, 1,"mj>400"));
  samples_MJ.push_back(sfeats(s_tt, "t#bar{t} M_{J}<400", 46, 1,"mj<=400"));
  MakeGraph(baseline,externalvec,ratio_cut,samples_MJ,luminosity);

  vector<sfeats> samples_MJ2;
  samples_MJ2.push_back(sfeats(s_tt, "t#bar{t} M_{J}>500", 31, 1,"mj>500"));
  samples_MJ2.push_back(sfeats(s_tt, "t#bar{t} M_{J}<500", 46, 1,"mj<=500"));
  MakeGraph(baseline,externalvec,ratio_cut,samples_MJ2,luminosity);

  vector<sfeats> samples_MJ3;
  samples_MJ3.push_back(sfeats(s_tt, "t#bar{t} M_{J}>600", 31, 1,"mj>600"));
  samples_MJ3.push_back(sfeats(s_tt, "t#bar{t} M_{J}<600", 46, 1,"mj<=600"));
  MakeGraph(baseline,externalvec,ratio_cut,samples_MJ3,luminosity);

  MakeGraph(baseline1b,externalvec2,ratio_cut,samples_MJ,luminosity);
  MakeGraph(baseline1b,externalvec2,ratio_cut,samples_MJ2,luminosity);

    
  MakeGraph(baseline1b+"&&ntruleps<=1",externalvec2,ratio_cut,samples_MJ,luminosity);
  MakeGraph(baseline1b+"&&ntruleps>=2",externalvec2,ratio_cut,samples_MJ,luminosity);
  
  MakeGraph(baseline1b+"&&ntruleps<=1",externalvec2,ratio_cut,samples_MJ2,luminosity);
  MakeGraph(baseline1b+"&&ntruleps>=2",externalvec2,ratio_cut,samples_MJ2,luminosity);
  
  ratio_cut = "mj>400";
  vector<sfeats> samples_mT;
  samples_mT.push_back(sfeats(s_tt, "t#bar{t} m_{T}>140", 2, 1,"mt>140&&(nmus+nels)==1"));
  samples_mT.push_back(sfeats(s_tt, "t#bar{t} m_{T}<140", 4, 1,"mt<=140&&(nmus+nels)==1"));
  samples_mT.push_back(sfeats(s_tt, "t#bar{t} reco dilepton", 8, 1,"(nmus+nels)==2"));
  MakeGraph(baseline_no_lep,externalvec,ratio_cut,samples_mT,luminosity);
  MakeGraph(baseline_no_lep,externalvec_nobjs,ratio_cut,samples_mT,luminosity);

  ratio_cut = "mj>500";
  MakeGraph(baseline_no_lep,externalvec,ratio_cut,samples_mT,luminosity);
  MakeGraph(baseline_no_lep,externalvec_nobjs,ratio_cut,samples_mT,luminosity);
  ratio_cut = "mj>600";
  MakeGraph(baseline_no_lep,externalvec,ratio_cut,samples_mT,luminosity);
  MakeGraph(baseline_no_lep,externalvec_nobjs,ratio_cut,samples_mT,luminosity);


  
  // MakeGraph(baseline+"&&mj>400",externalvec,ratio_cut,sfeats(s_tt, "t#bar{t}", 15, 1),luminosity);
  // MakeGraph(baseline+"&&mj<=400",externalvec,ratio_cut,sfeats(s_tt, "t#bar{t}", 15, 1),luminosity);

  
}

void MakeGraph(TString baseline,vector<TString> external,TString ratio_cut, vector<sfeats> samples, TString luminosity){
  // const int nR = 5;
  vector<TString> cosmetic2;
  cosmetic2.push_back("n_{B}=1");cosmetic2.push_back("n_{B}=2");cosmetic2.push_back("n_{B}=3+");
  cosmetic2.push_back("n_{B}=1");cosmetic2.push_back("n_{B}=2");cosmetic2.push_back("n_{B}=3+");
  vector<TString> cosmetic3;
  cosmetic3.push_back("n_{obj}=4");cosmetic3.push_back("n_{obj}=5");cosmetic3.push_back("n_{obj}=6");cosmetic3.push_back("n_{obj}=7");cosmetic3.push_back("n_{obj}=8+");
  int nsamples = samples.size();
  vector<TGraphErrors*> graphs;
  TGraphErrors* kappas;
  float max = 0.01;
  int nR = static_cast<int>(external.size());
  vector<double> x;
  vector<double> x_err;
  for(int isam = 0; isam<nsamples; isam++){
    /*double R[nR];
      double R_err[nR];
      double x[nR];
      double x_err[nR];*/
    vector<double> R;
    vector<double> R_err;
  
    for(int iR=0;iR<nR;iR++){
      double rholder;
      double r_errholder;
    
      GetRatio(rholder,r_errholder,baseline,external[iR],ratio_cut,samples.at(isam),luminosity);
      R.push_back(rholder);
      R_err.push_back(r_errholder);

      x.push_back(iR);//=iR; //just a placeholder for a label
      x_err.push_back(0);//x_err[iR]=0;
    }

    //This is some garbage to convert the vectors to arrays because TGraphErrors only accepts arrays
    TGraphErrors *g = new TGraphErrors(nR,&x[0],&R[0],&x_err[0],&R_err[0]);
    graphs.push_back(g);
    
    for(int imax = 0;imax<nR;imax++){
      if(R[imax]>max) max=R[imax];
    }
  }


  vector<double> kappa;
  vector<double> kappa_err;
  for(int iR=0;iR<nR;iR++){
    Double_t R1,R2,y;
    Double_t R1_err, R2_err;
    graphs.at(0)->GetPoint(iR,y,R1);
    R1_err = graphs.at(0)->GetErrorY(iR);
    graphs.at(1)->GetPoint(iR,y,R2);
    R2_err = graphs.at(1)->GetErrorY(iR);
    
    kappa.push_back(R1/R2);
    kappa_err.push_back((R1/R2)*sqrt(pow(R1_err/R1,2)+pow(R2_err/R2,2)));
          
  }
  kappas = new TGraphErrors(nR,&x[0],&kappa[0],&x_err[0],&kappa_err[0]);
  
  styles style("RA4"); style.setDefaultStyle();
  TCanvas can;
  //TPad *pad = static_cast<TPad *>(can.cd());
  TString plot_tag("_lumi"+luminosity+".eps");

  
  //make hist to define pad with labels on x-axis
  TH1F *h = new TH1F("for_axis_label",cuts2title(baseline),nR,-0.5,nR-0.5);
  for (int ih=1;ih<=nR;ih++){ h->GetXaxis()->SetBinLabel(ih,cuts2title(external[ih-1]));
    if(external[0].Contains("nbm")) h->GetXaxis()->SetBinLabel(ih,cosmetic2[ih-1]);
    if(external[0].Contains("const")) h->GetXaxis()->SetBinLabel(ih,cosmetic3[ih-1]);
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
  h->Draw();

  double legX = 0.65, legY = 0.89, legSingle = 0.14;
  double legW = 0.22, legH = legSingle;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.057); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);
  
  for(int ig = 0;ig<nsamples;ig++){
    graphs.at(ig)->SetMarkerStyle(20);
    graphs.at(ig)->SetMarkerColor(samples.at(ig).color);
    graphs.at(ig)->SetLineColor(samples.at(ig).color);
    graphs.at(ig)->Draw("PZ");
    leg.AddEntry(graphs.at(ig), samples.at(ig).label,"p");
  }
  /*TF1 linfit("linfit","[0]+[1]*x",0, nR);
    linfit.SetParameter(0, 200);  linfit.SetParameter(1, 0);

    g->Fit(&linfit,"M N","", 0, nR);
    TString leghisto = "[<R_{mT}> = "+RoundNumber(linfit.GetParameter(0),3)+" + "+
    RoundNumber(linfit.GetParameter(1),3)+"n_{jets}]";
  */



  leg.Draw("p");

  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
  if(external[0].Contains("nbm")){ line.DrawLine(2.5, 0, 2.5, 1.5*max);
    TLatex *text67 = new TLatex(0.35,0.05,"n_{jets}= 6-7");
    text67->SetNDC();
    text67->SetTextSize(0.045);
    text67->SetLineWidth(2);
    text67->Draw();
    TLatex *text8 = new TLatex(0.7,0.05,"n_{jets}= 8");
    text8->SetNDC();
    text8->SetTextSize(0.045);
    text8->SetLineWidth(2);
    text8->Draw();

  }

  
  TString pname = "plots/ratios/ratios_"+format_tag(ratio_cut+samples.at(0).cut+external[0]+baseline)+".eps";
  can.SaveAs(pname);
  
  for(int ig = 0;ig<nsamples;ig++) graphs.at(ig)->Delete();

  //now plot Kappa
  TCanvas can2;
  //TPad *pad = static_cast<TPad *>(can.cd());

  
  //make hist to define pad with labels on x-axis

  h->SetMaximum(2.0);
  h->Draw();

  kappas->SetMarkerStyle(20);
  kappas->SetMarkerColor(kBlack);
  kappas->SetLineColor(kBlack);
  kappas->Draw("PZ");
  line.DrawLine(h->GetBinLowEdge(1), 1, h->GetBinLowEdge(h->GetNbinsX()+1), 1);
  /*  double legX = 0.65, legY = 0.89, legSingle = 0.14;
      double legW = 0.22, legH = legSingle;
      TLegend leg(legX, legY-legH, legX+legW, legY);
      leg.SetTextSize(0.057); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
      leg.SetTextFont(132);
  
      for(int ig = 0;ig<nsamples;ig++){
      graphs.at(ig)->SetMarkerStyle(20);
      graphs.at(ig)->SetMarkerColor(samples.at(ig).color);
      graphs.at(ig)->SetLineColor(samples.at(ig).color);
      graphs.at(ig)->Draw("PZ");
      leg.AddEntry(graphs.at(ig), samples.at(ig).label,"p");
      }*/
  /*TF1 linfit("linfit","[0]+[1]*x",0, nR);
    linfit.SetParameter(0, 200);  linfit.SetParameter(1, 0);

    g->Fit(&linfit,"M N","", 0, nR);
    TString leghisto = "[<R_{mT}> = "+RoundNumber(linfit.GetParameter(0),3)+" + "+
    RoundNumber(linfit.GetParameter(1),3)+"n_{jets}]";
  */



  // leg.Draw("p");


  if(external[0].Contains("nbm")){ line.DrawLine(2.5, 0, 2.5, 1.5*max);
    TLatex *text67 = new TLatex(0.35,0.05,"n_{jets}= 6-7");
    text67->SetNDC();
    text67->SetTextSize(0.045);
    text67->SetLineWidth(2);
    text67->Draw();
    TLatex *text8 = new TLatex(0.7,0.05,"n_{jets}= 8");
    text8->SetNDC();
    text8->SetTextSize(0.045);
    text8->SetLineWidth(2);
    text8->Draw();

  }

  
  TString pname2 = "plots/ratios/kappa_"+format_tag(ratio_cut+samples.at(0).cut+external[0]+baseline)+".eps";
  can2.SaveAs(pname2);
  h->Delete();
 
}

void GetRatio(double &ratio,double &error, TString baseline,TString external,TString ratio_cut, sfeats Sample,TString luminosity){

  TChain * chain = new TChain("tree");
  for(unsigned insam(0); insam < Sample.file.size(); insam++) chain->Add(Sample.file[insam]);

  TH1D h_numerator("num", "",100, 0, 10000);
  h_numerator.Sumw2();
  TH1D h_denominator("den", "",100, 0, 10000);
  h_denominator.Sumw2();

  TString numCut = luminosity+"*weight*("+baseline+"&&"+external+"&&"+ratio_cut+"&&"+Sample.cut+")";
  TString denCut = luminosity+"*weight*("+baseline+"&&"+external+"&&!("+ratio_cut+")&&"+Sample.cut+")";
  
  chain->Project("num", "met", numCut);
  chain->Project("den", "met", denCut);

  double numerator, denominator,numerator_err, denominator_err;
  numerator = h_numerator.IntegralAndError(0,101,numerator_err);
  denominator = h_denominator.IntegralAndError(0,101,denominator_err);

  ratio = numerator/denominator;
  error = ratio*sqrt(pow(numerator_err/numerator,2)+pow(denominator_err/denominator,2));
  
  
}




