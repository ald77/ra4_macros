#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include "TMath.h"
#include "TChain.h"
#include "TRandom3.h"
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
  int method(1);
  int nrep = 10000;    // Fluctuations of Gamma distribution
  bool do_1ltt(false); // Kappa just for 1l ttbar
  bool do_2ltt(false); // Kappa just for 2l ttbar
  bool do_ttbar(true); // Include ttbar in kappa
  bool do_other(false); // Include other in kappa
}

using namespace std;

// yields[Nobs][Nsam] has the entries for each sample for each observable going into kappa
// weights[Nobs][Nsam] has the average weight of each observable for each sample
// powers[Nobs] defines kappa = Product_obs{ Sum_sam{yields[sam][obs]*weights[sam][obs]}^powers[obs] }
double calcKappaRelUnc(vector<vector<vector<int> > > &entries, vector<vector<vector<float> > > &weights, vector<float> &powers,
     float &mSigma, float &pSigma, TString label);

double getSysWeight();

int main(){ 
  styles style("RA4"); style.LabelSize = 0.05;
  style.setDefaultStyle();
  // TString folder="/cms5r0/ald77/archive/"+ntuple_date+"/skim100/";
  // TString folder_ns="/cms5r0/ald77/archive/"+ntuple_date+"/";
  // TString folder="/net/cms2/cms2r0/manuelf/root/archive/15-06-01/skim/";
  TString folder="/cms5r0/aovcharova/archive/2015_06_05/skim/";


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
  //   ra4_sam_ns.push_back(nsam+sam);
  //   vector<TString> sam_files = Samples[sam].file;
  //   for(unsigned ifile(0); ifile < sam_files.size(); ifile++)
  //     sam_files[ifile].ReplaceAll(folder, folder_ns);
  //   Samples.push_back(sfeats(sam_files, Samples[sam].label, Samples[sam].color, Samples[sam].style,
  //          Samples[sam].cut));
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
  powers.push_back(1);  cuts.push_back("mt<=140&&mj<="+mjthresh); // R1
  powers.push_back(-1); cuts.push_back("mt<=140&&mj>"+mjthresh);  // R2
  powers.push_back(-1); cuts.push_back("mt>140&&mj<="+mjthresh);  // R3
  powers.push_back(1);  cuts.push_back("mt>140&&mj>"+mjthresh);   // R4
  
  TString baseline("(nmus+nels)==1&&ht>500&&met>100&&njets>=7&&nbm>=1");
  vector<TString> metcuts, njcuts, nbcuts, metnames;
  metcuts.push_back("met>200&&met<=400");
  metcuts.push_back("met>400");
  njcuts.push_back("njets<=8");
  njcuts.push_back("njets>=9");
  // nbcuts.push_back("nbm==1");
  if(method==1 || method==2) nbcuts.push_back("nbm>=1");
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

  vector<sysfeats> syst;
  syst.push_back(sysfeats("nisr","ME ISR multiplicity"));
  syst.back().push_back("n_isr_me==0", 0.75);
  syst.back().push_back("n_isr_me==1", 1.);
  syst.back().push_back("n_isr_me==2", 1.25);
  syst.back().push_back("n_isr_me==3", 1.5);

  syst.push_back(sysfeats("pt_isr","ISR pT"));
  syst.back().push_back("tru_tt_pt<=120", 1.);
  syst.back().push_back("120<tru_tt_pt&&tru_tt_pt<=200", 0.95);
  syst.back().push_back("200<tru_tt_pt&&tru_tt_pt<=300", 0.9);
  syst.back().push_back("300<tru_tt_pt&&tru_tt_pt<=500", 0.8);
  syst.back().push_back("500<tru_tt_pt&&tru_tt_pt<=650", 0.6);
  syst.back().push_back("650<tru_tt_pt&&tru_tt_pt<=800", 0.4);
  syst.back().push_back("800<tru_tt_pt&&tru_tt_pt", 0.2);

  // syst.push_back(sysfeats("top_pt","top $p_T$"));
  // syst.back().push_back("(trutop1_pt+trutop2_pt)<=300", exp(0.156-0.000685*200.));
  // syst.back().push_back("300<(trutop1_pt+trutop2_pt)&&(trutop1_pt+trutop2_pt)<=500", exp(0.156-0.000685*400.));
  // syst.back().push_back("500<(trutop1_pt+trutop2_pt)&&(trutop1_pt+trutop2_pt)<=700", exp(0.156-0.000685*600.));
  // syst.back().push_back("700<(trutop1_pt+trutop2_pt)&&(trutop1_pt+trutop2_pt)<=900", exp(0.156-0.000685*800.));
  // syst.back().push_back("900<(trutop1_pt+trutop2_pt)&&(trutop1_pt+trutop2_pt)<=1100", exp(0.156-0.000685*1000.));
  // syst.back().push_back("1100<(trutop1_pt+trutop2_pt)&&(trutop1_pt+trutop2_pt)<=1300", exp(0.156-0.000685*1200.));
  // syst.back().push_back("1300<(trutop1_pt+trutop2_pt)&&(trutop1_pt+trutop2_pt)<=1500", exp(0.156-0.000685*1400.));
  // syst.back().push_back("1500<(trutop1_pt+trutop2_pt)", exp(0.156-0.000685*1500.));


  syst.push_back(sysfeats("dilep","2 $\\times$ dilep."));
  syst.back().push_back("((ntruleps==1&&mt<=140.)||ntruleps==2)", 1.);
  syst.back().push_back("ntruleps==1&&mt>140", 2.);

  float minh(0), maxh(10), wtot(maxh-minh), max_axis(2.), max_krelunc(0.);
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
  for(unsigned isys(0); isys<syst.size(); isys++){
    for(unsigned inj(0); inj<njcuts.size(); inj++){
      for(unsigned imet(0); imet<metcuts.size(); imet++){
        for(unsigned inb(0); inb<nbcuts.size(); inb++){
          vector<vector<vector<int> > > entries;
          vector<vector<vector<float> > > weights;
          for(unsigned obs(0); obs < powers.size(); obs++) {
            entries.push_back(vector<vector<int> >());
            weights.push_back(vector<vector<float> >());
            // totcut = lumi+"*weight*("+baseline+"&&"+nbcuts[inb]+"&&"+metcuts[imet]+"&&"+cuts[obs];
            // if(method==1 || obs%2==1) totcut += "&&"+njcuts[inj];
            // totcut += ")";
            // cout << totcut<<endl;
            for(unsigned sam(0); sam < ra4_sam.size(); sam++) {
              for(unsigned isbin(0); isbin < syst[isys].size(); isbin++) {
                entries[obs].push_back(vector<int>());
                weights[obs].push_back(vector<float>());
                totcut = (lumi+"*weight*("+baseline+"&&"+nbcuts[inb]+"&&"+metcuts[imet]+"&&"+cuts[obs]+
                          "&&"+Samples[ra4_sam[sam]].cut+"&&"+syst[isys].bincut(isbin));
                if(method==1 || obs%2==1) totcut += "&&"+njcuts[inj];
                totcut += ")";
                //cout << totcut<<endl;
                
                double yield(0.), sigma(0.), avWeight(1.);
                int Nentries(0);
                Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
                entries[obs][sam].push_back(Nentries);
                if(Nentries==0){ // If no entries, find average weight in signal bin
                  totcut = (lumi+"*weight*("+baseline+"&&"+cuts[obs]+")");
                  Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
                  // If no entries, find averate weight in baseline region
                  if(Nentries==0){
                    totcut = (lumi+"*weight*("+baseline+")");
                    Nentries = getYieldErr(*chain[ra4_sam[sam]], totcut, yield, sigma);
                  }
                }
                avWeight = fabs(yield/static_cast<double>(Nentries));
                if (isbin == 0) weights[obs][sam].push_back(avWeight);
                weights[obs][sam].push_back(syst[isys].weight(isbin));
              }
              //cout<<obs<<","<<sam<<": entries "<<entries[obs][sam]<<", weight "<<avWeight<<", yield "<<yield<<endl;
            } // Loop over samples
          } // Loop over number of observables going into kappa
          
          cout<<"----- Calc kappa for: "<<njcuts[inj]<<", "<<metcuts[imet]<<", "<<nbcuts[inb]<<endl;
          double krelunc = calcKappaRelUnc(entries, weights, powers, mSigma, pSigma, Form("nj%i_imet%i_isys%i",inj,imet,isys));  
          if(krelunc > max_krelunc) max_krelunc = krelunc;
          float xpoint = inj*wnj+imet*wmet+(inb+2)*wnb;
          if(krelunc > max_axis && krelunc-mSigma < max_axis) {
            mSigma = max_axis-(krelunc-mSigma);
            krelunc = max_axis;
          }
          vx[inb].push_back(xpoint);
          vy[inb].push_back(krelunc);
          vexl[inb].push_back(0);
          vexh[inb].push_back(0);
          veyl[inb].push_back(mSigma);
          veyh[inb].push_back(pSigma);

        } // Loop over nb cuts
      } // Loop over met cuts
    } // Loop over nj cuts
  } // Loop over systematics

  int digits(1);
  vector<unsigned> ind(nbcuts.size(),0);
  TString pname = "txt/kappa_method", cutname; pname += method;
  if(do_1ltt) pname += "_1ltt";
  else {
    if(do_2ltt) pname += "_2ltt";
    else {
      if(do_ttbar) pname += "_tt";
      if(do_other) pname += "_other";
    }
  }
  pname += ".tex";
  // ifstream header("txt/header.tex");
  // ifstream footer("txt/footer.tex");
  ofstream file(pname);
  // file<<header.rdbuf();
  /////////////////////// TABLE  ///////////////////////////
  file << "\\renewcommand{\\arraystretch}{1.4}"<<endl;
  file << "\\begin{tabular}[tbp!]{ l | cccc}\\hline\\hline\n";
  file << "& $n_{jets} \\leq 8$ & $n_{jets} \\leq 8$ & $n_{jets} > 8$ & $n_{jets} > 8$ \\\\"<<endl;
  file << "& MET $\\leq 400$ & MET $> 400$ & MET $\\leq 400$ & MET $> 400$ \\\\"<<endl;
  file << "\\hline\n";

  // file << " \\multicolumn{1}{c|}{${\\cal L} = "<<lumi<<"$ fb$^{-1}$} ";
  // file << " & $N_{\\rm R2}\\frac{N_{\\rm R3}}{N_{\\rm R1}}$ & Data stat. [\\%] \n";
  // file << "& $\\kappa^{\\rm MC}$ & MC stat. [\\%] & \\multicolumn{1}{c}{$\\hat{N}_{\\rm R4}$} \\\\ \\hline\n";
  for(unsigned isys(0); isys<syst.size(); isys++){
    file << syst[isys].title;
    for(unsigned inj(0); inj<njcuts.size(); inj++){
      for(unsigned imet(0); imet<metcuts.size(); imet++){
        for(unsigned inb(0); inb<nbcuts.size(); inb++){
          float Kappa(vy[inb][ind[inb]]);
          float epKappa(veyh[inb][ind[inb]]), emKappa(veyl[inb][ind[inb]]);
          file << " \t& $" <<RoundNumber(Kappa,digits+1)<<"^{+"<<RoundNumber(epKappa,digits+1)
          <<"}_{-"<<RoundNumber(emKappa,digits+1)<<"}$";
          ind[inb]++;
        } // Loop over nb cuts
      } // Loop over met cuts
    } // Loop over nj cuts
    file << "\\\\"<<endl;
  } // Loop over systematics

  file<< "\\hline\\hline\n\\end{tabular}"<<endl<<endl;
  // file<<footer.rdbuf();
  file.close();
  cout<<endl<<"Written "<<pname<<endl;

  // TCanvas can;
  // TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
  // TH1D histo("histo",cuts2title(baseline),njcuts.size()*metcuts.size(), minh, maxh);
  // histo.Draw();
  // TString ytitle("#kappa rel. unc. for method "); ytitle += method;
  // histo.SetYTitle(ytitle);
  // histo.SetMaximum(max_axis);
  // style.moveYAxisLabel(&histo, max_axis, false);
  // line.SetLineColor(1); line.SetLineWidth(2); 
  // line.DrawLine(minh+wtot/2., 0, minh+wtot/2, max_axis);
  // for(unsigned inj(0); inj<njcuts.size(); inj++)
  //   for(unsigned imet(0); imet<metcuts.size(); imet++)
  //     histo.GetXaxis()->SetBinLabel(1+imet+inj*metcuts.size(), metnames[imet]);

  // double legX(style.PadLeftMargin+0.03), legY(0.902), legSingle = 0.052;
  // double legW = 0.13, legH = legSingle*nbcuts.size();
  // TLegend leg(legX, legY-legH, legX+legW, legY);
  // leg.SetTextSize(style.LegendSize); leg.SetFillColor(0); 
  // leg.SetFillStyle(0); leg.SetBorderSize(0);
  // leg.SetTextFont(style.nFont); 
  // TGraphAsymmErrors graph[20];
  // int colors[] = {2,4,kMagenta+2}, styles[] = {20, 21, 22};
  // for(unsigned isys(0); isys<syst.size(); isys++){
  //   graph[isys] = TGraphAsymmErrors(vx[isys].size(), &(vx[isys][0]), &(vy[isys][0]), 
  //             &(vexl[isys][0]), &(vexh[isys][0]), &(veyl[isys][0]), &(veyh[isys][0]));
  //   graph[isys].SetMarkerStyle(styles[isys]); graph[isys].SetMarkerSize(1.4); 
  //   graph[isys].SetMarkerColor(colors[isys]); graph[isys].SetLineColor(colors[isys]);
  //   graph[isys].Draw("p same");
  //   leg.AddEntry(&graph[isys], syst[isys].title, "p");
  // }
  // leg.Draw();
  // TLatex label; label.SetNDC(kTRUE);label.SetTextAlign(22);
  // label.DrawLatex(0.37,0.03,"7 #leq n_{j} #leq 8");
  // label.DrawLatex(0.73,0.03,"n_{j} #geq 9");

  // TString pname = "plots/relunc_method"; pname += method;
  // if(do_1ltt) pname += "_1ltt";
  // else {
  //   if(do_2ltt) pname += "_2ltt";
  //   else {
  //     if(do_ttbar) pname += "_tt";
  //     if(do_other) pname += "_other";
  //   }
  // }
  // pname += "_mc";
  // pname += ".pdf";
  // can.SaveAs(pname);

  time(&endtime);
  cout<<endl<<"Calculation took "<<difftime(endtime, begtime)<<" seconds"<<endl<<endl; 
}

double calcKappaRelUnc(vector<vector<vector<int> > > &entries, vector<vector<vector<float> > > &weights, vector<float> &powers,
                    float &mSigma, float &pSigma, TString label){
  TRandom3 rand(1234);
  int nbadk(0);
  TCanvas can;
  vector<float> fKappaRelShift;
  double mean(0.), bignum(1e10);
  // Doing kappa variations
  for(int rep(0), irep(0); rep < nrep; rep++) {
    fKappaRelShift.push_back(1.);
    bool Denom_is0(false);
    for(unsigned obs(0); obs < powers.size(); obs++) {
      float observed(0.), observed_sys(0.);
      for(unsigned sam(0); sam < entries[obs].size(); sam++) {
        for(unsigned sysbin(0); sysbin < entries[obs][sam].size(); sysbin++) {
        // Using a flat prior, the expected average of the Poisson with N observed is Gamma(N+1,1)
          // double grnd = gsl_ran_gamma(entries[obs][sam][sysbin]+1,1,rand);
          double grnd = gsl_ran_gamma(entries[obs][sam][sysbin]+1,1,rand);
          observed += grnd*weights[obs][sam][0]; //store the nominal weight at 0
          observed_sys += grnd*weights[obs][sam][0]*weights[obs][sam][sysbin+1];
          // if (irep==0)
          //   cout<<"obs: "<<obs<<"sam: "<<sam<<"sbin: "<<sysbin<<
          //          "entr: "<<entries[obs][sam][sysbin]<<
          //          " rnd "<<grnd<<" w"<<weights[obs][sam][0]<<
          //          " wsys"<<weights[obs][sam][sysbin+1]<<endl;
        } // Loop over systematic bin
      } // Loop over samples
      // cout<<obs<<": observed "<<observed<<" sys "<<observed_sys<<endl;
      if( (observed <= 0 && powers[obs]*(-1) < 0) || (observed_sys <= 0 && powers[obs] < 0) ) Denom_is0 = true;
      else {
        // cout<<obs<<": pow observed "<<pow(observed, powers[obs]*(-1))<<" powsys "<<pow(observed_sys, powers[obs])<<endl;
        // cout<<"before "<<fKappaRelShift[irep]<<endl;
        // cout<<"mult fact: "<<(pow(observed, powers[obs]*(-1))*pow(observed_sys, powers[obs]))<<endl;
        fKappaRelShift[irep] *= (pow(observed, powers[obs]*(-1))*pow(observed_sys, powers[obs]));
        // cout<<"after "<<fKappaRelShift[irep]<<endl;
      }
      // cout<<"fKappaRelShift "<<fKappaRelShift[irep]<<endl;
    } // Loop over number of observables going into kappa
    fKappaRelShift[irep] -=1.;
    if(Denom_is0 && fKappaRelShift[irep]==0) {
      fKappaRelShift.pop_back();
      nbadk++;
    } else {
      if(Denom_is0) fKappaRelShift[irep] = bignum;
      else mean += fKappaRelShift[irep];
      irep++;
    }
    // cout<<"fKappaRelShift "<<fKappaRelShift[irep]<<endl;
  } // Loop over fluctuations of kappa (repetitions)
  int ntot(nrep-nbadk);
  mean /= static_cast<double>(ntot);

  sort(fKappaRelShift.begin(), fKappaRelShift.end());
  double gSigma = intGaus(0,1,0,1);
  int iMedian((nrep-nbadk+1)/2-1);
  int imSigma(iMedian-static_cast<int>(gSigma*ntot)), ipSigma(iMedian+static_cast<int>(gSigma*ntot));
  float median(fKappaRelShift[iMedian]);
  mSigma = median-fKappaRelShift[imSigma]; pSigma = fKappaRelShift[ipSigma]-median;

  // Finding standard value
  float stdRelShift(1.);
  bool infStd(false);
  for(unsigned obs(0); obs < powers.size(); obs++) {
    float observed(0.), observed_sys(0.);
    for(unsigned sam(0); sam < entries[obs].size(); sam++) {
      for(unsigned sysbin(0); sysbin < entries[obs][sam].size(); sysbin++) {
        observed += entries[obs][sam][sysbin]*weights[obs][sam][0];
        observed_sys += entries[obs][sam][sysbin]*weights[obs][sam][0]*weights[obs][sam][sysbin+1];
      }
    }
    if( (observed <= 0 && powers[obs]*(-1) < 0) || (observed_sys <= 0 && powers[obs] < 0) ) infStd = true;    
    else stdRelShift *= pow(observed, powers[obs]*(-1))*pow(observed_sys, powers[obs]);
  } // Loop over number of observables going into kappa
  stdRelShift -=1.;
  //if I want to pick up the mean instead:
  stdRelShift = median;
  cout<<"Standard rel. shift = "<<stdRelShift<<" median = "<<median<<endl;
  if(infStd) stdRelShift = median;
  else {
    int istd(0);
    for(int rep(0); rep < ntot; rep++) {
      if(fKappaRelShift[rep]>stdRelShift) {istd = rep; break;}
    }
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
    mSigma = stdRelShift-fKappaRelShift[imSigma]; 
    pSigma = fKappaRelShift[ipSigma]-stdRelShift;
  }

  int nbins(100);
  double minH(stdRelShift-3*mSigma), maxH(stdRelShift+3*pSigma);
  if(minH < fKappaRelShift[0]) minH = fKappaRelShift[0];
  if(maxH > fKappaRelShift[ntot-1]) maxH = fKappaRelShift[ntot-1];
  TH1D histo("h","",nbins, minH, maxH);
  for(int rep(0); rep < ntot; rep++) 
    histo.Fill(fKappaRelShift[rep]);   
  histo.SetBinContent(1, histo.GetBinContent(1)+nbadk);
  histo.SetBinContent(nbins, histo.GetBinContent(nbins)+histo.GetBinContent(nbins+1));
  histo.SetMaximum(histo.GetMaximum()*1.2);
  histo.Draw();
  can.SaveAs("plots/test_"+label+".pdf");

  double mode(histo.GetBinLowEdge(histo.GetMaximumBin()));
  cout<<"Std kappa = "<<stdRelShift<<"+"<<pSigma<<"-"<<mSigma<<".   Mean = "<<mean
      <<". Mode = "<<mode<<". Median = "<<median<<endl;

  return stdRelShift;
}


