// plot_mj_bdt_aux: trains various BDTs which combine MJ, njets, and HT. Plots ROCs with them
//     If run without parameters, it plots the ROCs (it requires the root files with the BDTs)
//     If run with a parameter, it does the BDT training
//     If run with two parameters, it doesn the BDT training and adds the 2nd parameter to the folder name

#include <stdexcept>
#include <iostream>
#include <vector>
#include <utility> 

#include "TFile.h"
#include "TCanvas.h"
#include "TMarker.h"
#include "TString.h"
#include "TChain.h"
#include "TLegend.h"
#include "TCut.h"
#include "TSystem.h"
#include "TDirectory.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"

#include "plot_mj_bdt_aux.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"
#include "styles.hpp"

using namespace std;

// If run without parameters, it plots the ROCs (it requires the root files with the BDTs)
// If run with a parameter, it does the BDT training
// If run with two parameters, it doesn the BDT training and adds the 2nd parameter to the folder name
int main(int argc, char *argv[]){
  TString tag("");
  if(argc > 2) tag = argv[2];

  TString bfolder("");
  string hostname = execute("echo $HOSTNAME");
  if(Contains(hostname, "cms") || Contains(hostname, "compute-"))  
    bfolder = "/net/cms2"; // In laptops, you can't create a /net folder
  
  TString ntufolder=bfolder+"/cms2r0/babymaker/babies/2015_01_30/small_tree/skim_ht500met200/";
  TString bdtfolder=bfolder+"/cms2r0/babymaker/babies/2015_01_30/small_tree/bdt"+tag+"/";

  if(argc > 1) trainBDT(ntufolder, bdtfolder);
  else plotROC(bdtfolder);
}


void plotROC(TString bdtfolder){

  ////////////////////////// SAMPLES for the axes /////////////////////////
  TString lsp = "{#lower[-0.1]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}}";
  TString t1t_label = "#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#kern[0.18]{#bar{t}}#kern[0.18]"+lsp;
  vector<TString> v_t1t_tt_htmj_30;
  v_t1t_tt_htmj_30.push_back(bdtfolder+"bdt_T1tttt1500_TTJet_htmj_30.root");
  vector<sample_class> t1t_tt_htmj_30; 
  t1t_tt_htmj_30.push_back(sample_class(t1t_label+" (1500,100)", v_t1t_tt_htmj_30));
  t1t_tt_htmj_30.push_back(sample_class("t#bar{t}", v_t1t_tt_htmj_30));

  vector<TString> v_t1t_tt_htnjets30;
  v_t1t_tt_htnjets30.push_back(bdtfolder+"bdt_T1tttt1500_TTJet_htnjets30.root");
  vector<sample_class> t1t_tt_htnjets30; 
  t1t_tt_htnjets30.push_back(sample_class(t1t_label+" (1500,100)", v_t1t_tt_htnjets30));
  t1t_tt_htnjets30.push_back(sample_class("t#bar{t}", v_t1t_tt_htnjets30));

  vector<TString> v_t1t_tt_njets30mj_30;
  v_t1t_tt_njets30mj_30.push_back(bdtfolder+"bdt_T1tttt1500_TTJet_njets30mj_30.root");
  vector<sample_class> t1t_tt_njets30mj_30; 
  t1t_tt_njets30mj_30.push_back(sample_class(t1t_label+" (1500,100)", v_t1t_tt_njets30mj_30));
  t1t_tt_njets30mj_30.push_back(sample_class("t#bar{t}", v_t1t_tt_njets30mj_30));

  // T1tttt(1200,800)
  vector<TString> v_t1tc_tt_htmj_30;
  v_t1tc_tt_htmj_30.push_back(bdtfolder+"bdt_T1tttt1200_TTJet_htmj_30.root");
  vector<sample_class> t1tc_tt_htmj_30; 
  t1tc_tt_htmj_30.push_back(sample_class(t1t_label+" (1200,800)", v_t1tc_tt_htmj_30));
  t1tc_tt_htmj_30.push_back(sample_class("t#bar{t}", v_t1tc_tt_htmj_30));

  vector<TString> v_t1tc_tt_htnjets30;
  v_t1tc_tt_htnjets30.push_back(bdtfolder+"bdt_T1tttt1200_TTJet_htnjets30.root");
  vector<sample_class> t1tc_tt_htnjets30; 
  t1tc_tt_htnjets30.push_back(sample_class(t1t_label+" (1200,800)", v_t1tc_tt_htnjets30));
  t1tc_tt_htnjets30.push_back(sample_class("t#bar{t}", v_t1tc_tt_htnjets30));

  vector<TString> v_t1tc_tt_njets30mj_30;
  v_t1tc_tt_njets30mj_30.push_back(bdtfolder+"bdt_T1tttt1200_TTJet_njets30mj_30.root");
  vector<sample_class> t1tc_tt_njets30mj_30; 
  t1tc_tt_njets30mj_30.push_back(sample_class(t1t_label+" (1200,800)", v_t1tc_tt_njets30mj_30));
  t1tc_tt_njets30mj_30.push_back(sample_class("t#bar{t}", v_t1tc_tt_njets30mj_30));

  ///////////////////// Markers for each ROC /////////////////////
  int ht_col(kRed+1), mj_col(kAzure+2), col2(kGreen+1), col3(1), col4(kOrange+7);
  int mj_style(8); float mj_size(2.);
  vector<marker_class> mj_points, ht_points, nj_points;
  mj_points.push_back(marker_class(200,  mj_size, mj_col, mj_style));
  mj_points.push_back(marker_class(400,  4, mj_col, 29));
  mj_points.push_back(marker_class(600,  mj_size, mj_col, mj_style));
  mj_points.push_back(marker_class(800,  mj_size, mj_col, mj_style));

  ht_points.push_back(marker_class(1000,  4, ht_col, 29));
  ht_points.push_back(marker_class(1500,  mj_size, ht_col, mj_style));
  ht_points.push_back(marker_class(2000,  mj_size, ht_col, mj_style));

  nj_points.push_back(marker_class(6,  mj_size, col4, mj_style));
  nj_points.push_back(marker_class(9,  4, col4, 29));
  nj_points.push_back(marker_class(11,  mj_size, col4, mj_style));


  ///////////////////// ROCs to plot /////////////////////
  TString mj_s("M#lower[-.1]{_{J}}"), nj_s("N#lower[-.1]{_{jets}}"), ht_s("H#lower[-.1]{_{T}}");
  vector<var_class> mj_t1t_tt;
  mj_t1t_tt.push_back(var_class(t1t_tt_njets30mj_30, "BDT",0.32, -0.5,"BDT["+mj_s+", "+nj_s+"]",col3,7));
  mj_t1t_tt.push_back(var_class(t1t_tt_htnjets30, "BDT",0.3, -0.5,"BDT["+ht_s+", "+nj_s+"]",col2,2));
  mj_t1t_tt.push_back(var_class(t1t_tt_htmj_30, "BDT",0.23, -0.5,"BDT["+ht_s+", "+mj_s+"]",kMagenta+1,7));
  mj_t1t_tt.push_back(var_class(t1t_tt_htmj_30, "ht",4000,0,""+ht_s+"",ht_col,1,ht_points));
  mj_t1t_tt.push_back(var_class(t1t_tt_htmj_30, "njets30",15,0,""+nj_s+"",col4,1,nj_points));
  mj_t1t_tt.push_back(var_class(t1t_tt_htmj_30, "mj_30",2000,0,""+mj_s+"",mj_col,1,mj_points));

  vector<var_class> mj_t1tc_tt;
  mj_t1tc_tt.push_back(var_class(t1tc_tt_njets30mj_30, "BDT",0.24, -0.5,"BDT["+mj_s+", "+nj_s+"]",col3,7));
  mj_t1tc_tt.push_back(var_class(t1tc_tt_htnjets30, "BDT",0.24, -0.5,"BDT["+ht_s+", "+nj_s+"]",col2,2));
  mj_t1tc_tt.push_back(var_class(t1tc_tt_htmj_30, "BDT",0.23, -0.5,"BDT["+ht_s+", "+mj_s+"]",kMagenta+1,7));
  mj_t1tc_tt.push_back(var_class(t1tc_tt_htmj_30, "ht",4000,0,""+ht_s+"",ht_col,1,ht_points));
  mj_t1tc_tt.push_back(var_class(t1tc_tt_htmj_30, "njets30",15,0,""+nj_s+"",col4,1,nj_points));
  mj_t1tc_tt.push_back(var_class(t1tc_tt_htmj_30, "mj_30",2000,0,""+mj_s+"",mj_col,1,mj_points));

  vector<TString> vs_sam;
  vector<vector<var_class> > v_mj;
  v_mj.push_back(mj_t1t_tt);   vs_sam.push_back("t1t_tt");
  v_mj.push_back(mj_t1tc_tt);  vs_sam.push_back("t1tc_tt");

  vector<TString> v_cuts;
  v_cuts.push_back("ht>500&&met>200");

  for(unsigned icut(0); icut<v_cuts.size(); icut++){
    for(unsigned isam(0); isam<vs_sam.size(); isam++){
      DrawROC(v_mj[isam], v_cuts[icut], "mj_bdt_"+vs_sam[isam]);
    } // Loop over samples
  } // Loop over cuts

}


void trainBDT(TString ntufolder, TString bdtfolder){

  TString nTrain("20000"), nTest("0");
  gSystem->mkdir(bdtfolder, kTRUE);

  vector<bdtvar_class> v_htmj;
  v_htmj.push_back(bdtvar_class("ht","H_{T}",'F'));
  v_htmj.push_back(bdtvar_class("mj_30","M_{J}",'F'));

  vector<bdtvar_class> v_htnjets;
  v_htnjets.push_back(bdtvar_class("ht","H_{T}",'F'));
  v_htnjets.push_back(bdtvar_class("njets30","N_{jets}",'I'));

  vector<bdtvar_class> v_njetsmj;
  v_njetsmj.push_back(bdtvar_class("njets30","N_{jets}",'I'));
  v_njetsmj.push_back(bdtvar_class("mj_30","M_{J}",'F'));

  vector<bdtvar_class> v_njetsmjhtmet;
  v_njetsmjhtmet.push_back(bdtvar_class("mj_30","M_{J}",'F'));
  v_njetsmjhtmet.push_back(bdtvar_class("ht","H_{T}",'F'));
  v_njetsmjhtmet.push_back(bdtvar_class("njets30","N_{jets}",'I'));
  v_njetsmjhtmet.push_back(bdtvar_class("met","MET",'F'));

  vector<TString> v_signal;
  v_signal.push_back("*T1tttt*1500*");
  v_signal.push_back("*T1tttt*1200*");
		      
  vector<TString> v_bkg;
  v_bkg.push_back("*TTJet*");

  vector<bdt_class> bdts;
  for(unsigned isig(0); isig < v_signal.size(); isig++){
    for(unsigned ibkg(0); ibkg < v_bkg.size(); ibkg++){
      bdts.push_back(bdt_class(v_htmj, v_signal[isig], v_bkg[ibkg]));
      bdts.push_back(bdt_class(v_htnjets, v_signal[isig], v_bkg[ibkg]));
      bdts.push_back(bdt_class(v_njetsmj, v_signal[isig], v_bkg[ibkg]));
      //bdts.push_back(bdt_class(v_njetsmjhtmet, v_signal[isig], v_bkg[ibkg]));
    }
  }

  for(unsigned ibdt(0); ibdt < bdts.size(); ibdt++){
    // Loading root file and trees
    TFile tmvaFile(bdtfolder+bdts[ibdt].name+".root", "RECREATE");
    tmvaFile.cd();
    TChain signal("tree"), ttbar("tree");
    signal.Add(ntufolder+bdts[ibdt].signal);
    ttbar.Add(ntufolder+bdts[ibdt].bkg);

    // Creating TMVA factory
    TMVA::Factory *factory = new TMVA::Factory(bdts[ibdt].name, &tmvaFile,"!V:Silent:Color");
    for(unsigned ind(0); ind < bdts[ibdt].vars.size(); ind++) 
      factory->AddVariable(bdts[ibdt].vars[ind].variable, bdts[ibdt].vars[ind].name, 
			   bdts[ibdt].vars[ind].unit, bdts[ibdt].vars[ind].type);
    factory->AddSpectator("met := met", "MET", "GeV", 'F');
    factory->AddSpectator("ht := ht", "H_{T}", "GeV", 'F');
    factory->AddSpectator("mj_30 := mj_30", "M_{J}", "GeV", 'F');
    factory->AddSpectator("nvmus10 := nvmus10", "Number of veto muons", "", 'I');
    factory->AddSpectator("nvels10 := nvels10", "Number of veto electrons", "", 'I');
    factory->AddSpectator("nmus := nmus", "Number of muons", "", 'I');
    factory->AddSpectator("nels := nels", "Number of electrons", "", 'I');
    factory->AddSpectator("njets30 := njets30", "Number of 30 GeV jets", "", 'I');
    factory->AddSignalTree    (&signal, 1.);
    factory->AddBackgroundTree(&ttbar, 1.);
    factory->SetSignalWeightExpression    ("weight");
    factory->SetBackgroundWeightExpression("weight");

    TCut mycuts(""), mycutb("");
    factory->PrepareTrainingAndTestTree( mycuts, mycutb, "nTrain_Signal="+nTrain+":nTrain_Background="+nTrain+
					 ":nTest_Signal="+nTest+":nTest_Background="+nTest+
					 ":SplitMode=Random:NormMode=NumEvents:!V" );
    factory->BookMethod( TMVA::Types::kBDT, "BDT",
			 "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
    
    factory->TrainAllMethods();    // Train MVAs using the set of training events
    factory->TestAllMethods();     // Evaluate all MVAs using the set of test events
    factory->EvaluateAllMethods(); // Evaluate and compare performance of all configured MVAs

    delete factory;
    tmvaFile.Write();
    tmvaFile.Close();
    cout<<"\t\t\t\t\t\t\t\t\t\t\tWritten "<<bdtfolder+bdts[ibdt].name+".root"<<endl;

  } // Loop over BDTs

}


void DrawROC(vector<var_class> vars, TString cuts, TString tag){
  styles style("RA4"); style.setDefaultStyle();
  TCanvas can;
  const int nbins(1000);
  vector<vector<TH1D> > histos;
  TString hname, totcut;
  TChain *chain[2];

  for(unsigned var(0); var<vars.size(); var++){
    vector<sample_class> samples = vars[var].samples;
    for(unsigned sam(0); sam < samples.size(); sam++){
      // Loading chains
      for(unsigned isam(0); isam < samples[sam].files.size(); isam++){
	chain[sam] = new TChain("TestTree");
	int nfiles = chain[sam]->Add(samples[sam].files[isam]);
	if(nfiles==0) cout<<samples[sam].files[isam]<<" not found"<<endl;
      }
      histos.push_back(vector<TH1D>());

      // Projecting variables
      float minh(vars[var].minx), maxh(vars[var].maxx);
      if(minh > maxh){
	minh = maxh;
	maxh = vars[var].minx;
      }
      hname = "histo"; hname += sam; hname += var;
      totcut = "weight*("+cuts+"&&"+samples[sam].cut+"&&classID==";
      totcut += sam; totcut += ")";
      histos[sam].push_back(TH1D(hname,"",nbins,minh,maxh));
      chain[sam]->Project(hname, vars[var].varname, totcut);
      chain[sam]->Delete();
    } // Loop over variables
  } // Loop over samples

  TH1D base_histo("base","",1,0.01,1.0);
  base_histo.SetMinimum(0.0);
  base_histo.SetMaximum(1.0);
  base_histo.SetDirectory(0);
  base_histo.Draw();
  style.setTitles(&base_histo, vars[0].samples[0].label+" efficiency", vars[0].samples[1].label+" efficiency", 
		  "#scale[0.8]{#font[62]{CMS}} #scale[0.6]{#font[52]{Supplementary (Simulation)}}", "#scale[0.8]{13 TeV}");

  // Legend
  double legX = style.PadLeftMargin+0.03, legY = 1-style.PadTopMargin-0.02, legSingle = 0.06;
  double legW = 0.2, legH = legSingle*vars.size();
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.054); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  // Making individual graphs
  TGraph graphs[100]; // Had to make it an array because a vector<TGraph> kept crashing
  for(unsigned var(0); var<vars.size(); var++){
    graphs[var] = MakeROC(histos[0][var], histos[1][var], vars[var].minx < vars[var].maxx, vars[var].cuts);
    graphs[var].SetLineColor(vars[var].color);
    graphs[var].SetLineStyle(vars[var].style);
    if(vars[var].style==1) graphs[var].SetLineWidth(5);
    else  graphs[var].SetLineWidth(8);
    leg.AddEntry(&(graphs[var]), vars[var].title, "l");
    graphs[var].Draw("lsame");
  } // Loop over variables
  leg.Draw();

  cuts.ReplaceAll(".",""); 
  cuts.ReplaceAll("(",""); cuts.ReplaceAll("$","");  cuts.ReplaceAll(")",""); 
  cuts.ReplaceAll("[",""); cuts.ReplaceAll("]",""); 
  cuts.ReplaceAll("/","_"); cuts.ReplaceAll("*",""); cuts.ReplaceAll("&&","_");
  cuts.ReplaceAll(">=","ge"); cuts.ReplaceAll("<=","se"); 
  cuts.ReplaceAll(">","g"); cuts.ReplaceAll("<","s"); cuts.ReplaceAll("=","");
  cuts.ReplaceAll("+",""); 
  TString pname("plots/roc_"+tag+"_"+cuts+".pdf");  
  //can.Print(pname);
  can.SetLogx(1);
  can.SetLogy(1);
  pname.ReplaceAll("roc_","log_roc_");
  base_histo.SetMinimum(1e-4);
  can.Print(pname);

}

TGraph MakeROC(TH1D &good, TH1D &bad, const bool less_is_better, vector<marker_class> cuts){
  const int nbins = good.GetNbinsX();
  if(bad.GetNbinsX() != nbins) throw logic_error("Mismatched number of bins");

  TMarker marker;  

  TGraph graph(0);
  const double good_tot = good.Integral(0, nbins+1);
  const double bad_tot = bad.Integral(0, nbins+1);
  int inibin(0), endbin(nbins+1), dbin(1); unsigned icut(0);
  if(less_is_better){
    inibin = nbins+1;
    endbin = 0;
    dbin = -1;
  }
  for(int bin = inibin; bin*dbin<=endbin*dbin; bin+=dbin){
    const double good_pass = good.Integral(min(endbin,bin), max(endbin,bin));
    const double bad_pass = bad.Integral(min(endbin,bin), max(endbin,bin));
    const double x = good_pass/good_tot;
    const double y = bad_pass/bad_tot;
    graph.SetPoint(graph.GetN(), x, y);
 
    // Plotting the stars
    if(icut<cuts.size()){
      float edge(good.GetXaxis()->GetBinUpEdge(bin));
      if((edge>=cuts[icut].cut&&!less_is_better) || (edge<=cuts[icut].cut&&less_is_better)){
	marker.SetMarkerStyle(cuts[icut].style);marker.SetMarkerColor(cuts[icut].color);
	marker.SetMarkerSize(cuts[icut].size); 
	if(x>0.01 && y>0.0001) marker.DrawMarker(x,y);
	icut++;
      }
    }
  }
  TString name(good.GetName());
  name += "graph";
  graph.SetName(name);
  graph.SetTitle(name);

  return graph;
}

var_class::var_class(vector<sample_class> isamples, TString ivarname, float iminx, float imaxx, TString ititle, 
		     int icolor, int istyle, vector<marker_class> icuts){
  varname = ivarname; minx = iminx; maxx = imaxx; title = ititle;
  cuts = icuts; 
  color = icolor; style = istyle;
  samples = isamples;
}

sample_class::sample_class(TString ilabel, vector<TString> ifiles, TString icut){
  files = ifiles; label = ilabel; cut = icut;
}

marker_class::marker_class(float icut, float isize, int icolor, int istyle){
  cut=icut; size=isize; color=icolor; style=istyle;
}



bdt_class::bdt_class(vector<bdtvar_class> ivars, TString isignal, TString ibkg):
  vars(ivars),
  signal(isignal),
  bkg(ibkg){
  name = "bdt_"+signal+"_"+bkg+"_";
  for(unsigned ind(0); ind < vars.size(); ind++) name += vars[ind].variable;
  name.ReplaceAll("*","");
}

bdtvar_class::bdtvar_class(TString ivariable, TString iname, char itype, TString iunit):
  variable(ivariable),
  name(iname),
  type(itype),
  unit(iunit){
}

