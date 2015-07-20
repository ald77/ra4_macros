// plot_trigger: Plots for the trigger section of the RA4 note

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

using namespace std;
using std::cout;
using std::endl;

int main(){ 

  TString folders[] ={"/net/cms2/cms2r0/manuelf/root/small/hlt/15-04-01/el15/",
		      "/net/cms2/cms2r0/manuelf/root/small/hlt/15-04-01/mu15/"};
  TString folder_offline="/cms5r0/ald77/archive/2015_05_10/";
  TString leptag[] = {"electron", "muon"};
  vector<TString> s_t1t[2], s_t1tc[2], s_tt[2], s_qcd[2], s_wjets[2];
  vector<TString> s_t1t_offline;
  s_t1t_offline.push_back(folder_offline+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc_offline;
  s_t1tc_offline.push_back(folder_offline+"*T1tttt*1200_*PU20*");
  vector<TString> s_tt_offline;
  s_tt_offline.push_back(folder_offline+"*_TTJet*12.root");
  s_tt_offline.push_back(folder_offline+"*_TTJet*11.root");

  vector<sfeats> Samples; 
  unsigned Nsam(0);
  for(int lep(0); lep<2; lep++){
    s_t1t[lep].push_back(folders[lep]+"*T1tttt*1500_*PU20*root");
    s_t1tc[lep].push_back(folders[lep]+"*T1tttt*1200_*PU20*");
    s_tt[lep].push_back(folders[lep]+"*TT_Tune*");
    s_wjets[lep].push_back(folders[lep]+"*WTo*");
    s_qcd[lep].push_back(folders[lep]+"*QCD_Pt*");

    Samples.push_back(sfeats(s_t1t[lep], "T1tttt(1500,100)", ra4::c_t1tttt));
    Samples.push_back(sfeats(s_t1tc[lep], "T1tttt(1200,800)", ra4::c_t1tttt,2));
    Samples.push_back(sfeats(s_tt[lep], "t#bar{t}", ra4::c_tt_1l));
    //Samples.push_back(sfeats(s_wjets[lep], "W+jets", ra4::c_singlet));
    Samples.push_back(sfeats(s_qcd[lep], "QCD", 1));
    if(lep==0) Nsam = Samples.size();
  }
  Samples.push_back(sfeats(s_t1t_offline, "T1tttt(1500,100)", ra4::c_t1tttt));
  Samples.push_back(sfeats(s_t1tc_offline, "T1tttt(1200,800)", ra4::c_t1tttt,2));
  Samples.push_back(sfeats(s_tt_offline, "t#bar{t}", ra4::c_tt_1l,1));

  vector<int> all_sam[2], tt_t1[2], tt_t1_offline;
  for(unsigned lep(0); lep<2; lep++){
    for(unsigned iall(0); iall<Nsam; iall++) all_sam[lep].push_back(iall+lep*Nsam);
    tt_t1[lep].push_back(0+lep*Nsam);
    tt_t1[lep].push_back(1+lep*Nsam);
    tt_t1[lep].push_back(2+lep*Nsam);
  }
  tt_t1_offline.push_back(2*Nsam+0);
  tt_t1_offline.push_back(2*Nsam+1);
  tt_t1_offline.push_back(2*Nsam+2);

  /////////////////////////// Trigger variables ///////////////////////////
  vector<hfeats> std_varis;
  std_varis.push_back(hfeats("Max$(mus_pt)",60,0,300, tt_t1[1], "Leading #mu HLT p_{T} [GeV]",
			"onht>350&&Sum$(genmus_pt>0)>=1",20,"muon"));
  std_varis.push_back(hfeats("Max$(els_pt)",60,0,300, tt_t1[0], "Leading e HLT p_{T} [GeV]",
			"onht>350&&Sum$(genels_pt>0)>=1",20,"electron"));
  for(unsigned lep(0); lep<2; lep++){
    std_varis.push_back(hfeats("onht",75,0,2500, all_sam[lep], "HLT H_{T} [GeV]",
			  "1",350,leptag[lep]));
    std_varis.push_back(hfeats("onmet",65,0,650, all_sam[lep], "HLT MET [GeV]",
			  "1",-1,leptag[lep]));
  }

  std_varis.push_back(hfeats("Max$(mc_pt*(abs(mc_id)==13))",60,0,300, tt_t1_offline, "Leading gen #mu p_{T} (GeV)",
			"ht>350&&Sum$(abs(mc_id)==13)>=1",20,"muon"));
  std_varis.push_back(hfeats("Max$(mc_pt*(abs(mc_id)==11))",60,0,300, tt_t1_offline, "Leading gen e p_{T} (GeV)",
			"ht>350&&Sum$(abs(mc_id)==11)>=1",20,"electron"));

  TString filetype(".png"), namestyle("RA4");
  bool do_mu(true);
  //plot_distributions(Samples, std_varis, "10", filetype, namestyle);


  /////////////////////////// Turn on curves ///////////////////////////
  styles style(namestyle); style.yTitleOffset = 1.;
  style.setDefaultStyle();
  TCanvas can;
  TChain *chain[2];
  TH1D* histo[2][20];
  TString title, hname, totCut, pname;
  for(unsigned lep(0); lep<2; lep++) {
    chain[lep] = new TChain("tree");
    chain[lep]->Add(s_t1tc[lep][0]);
  }
  vector<hfeats> vars;
  // vars.push_back(hfeats("Max$(genmus_pt)",20,25,35, tt_t1[1], "True muon max p_{T} [GeV]", 
  // 			"Sum$(genmus_pt>0)>=1",30,"Max$(mus_pt)"));
  vars.push_back(hfeats("genmet",40,0,400, tt_t1[1], "True MET [GeV]", "1",70,"onmet"));
  vars.push_back(hfeats("genmet",40,0,400, tt_t1[1], "True MET [GeV]", "1",100,"onmet"));
  vars.push_back(hfeats("genmet",40,0,400, tt_t1[1], "True MET [GeV]", "1",170,"onmet"));
  vars.push_back(hfeats("genht",85,200,1050, tt_t1[1], "True H_{T} [GeV]", "1",350,"onht"));
  vars.push_back(hfeats("genht",85,200,1050, tt_t1[1], "True H_{T} [GeV]", "1",400,"onht"));
  vars.push_back(hfeats("genht",85,200,1050, tt_t1[1], "True H_{T} [GeV]", "1",600,"onht"));
  for(unsigned var(0); var<vars.size(); var++){
    title = cuts2title(vars[var].cuts); 
    for(unsigned his(0); his < 2; his++){
      hname = "histo"; hname += var; hname += his; 
      totCut = vars[var].cuts; 
      if(his==1) {
	totCut += ("&&"+vars[var].tagname+">");
	totCut += vars[var].cut;
      }
      histo[his][var] = new TH1D(hname, title, vars[var].nbins, vars[var].minx, vars[var].maxx);
      //cout<<var<<": hname "<<hname<<", cuts "<<totCut<<endl;
      if(do_mu) chain[1]->Project(hname, vars[var].varname, totCut);
      else chain[0]->Project(hname, vars[var].varname, totCut);
    }
  }

  unsigned Ncurves(3);
  double legY(style.PadBottomMargin+0.02), legSingle = 0.06;
  double legW = 0.13, legH = legSingle*Ncurves;
  double legX1 = 0.58;
  TLegend leg[2]; int nLegs(2);
  for(int ileg(0); ileg<nLegs; ileg++){
    leg[ileg].SetX1NDC(legX1); leg[ileg].SetX2NDC(legX1+legW); 
    leg[ileg].SetY1NDC(legY); leg[ileg].SetY2NDC(legY+legH); 
    leg[ileg].SetTextSize(style.LegendSize); leg[ileg].SetFillColor(0); 
    leg[ileg].SetFillStyle(0); leg[ileg].SetBorderSize(0);
    leg[ileg].SetTextFont(style.nFont); 
  }
  int colors[] = {1,4,2};
  TString labtag[] = {"HLT MET > ", "HLT H_{T} > "}, label;
  TGraphAsymmErrors heff[20];
  for(unsigned type(0); type<2; type++){
    for(unsigned ivar(0); ivar<Ncurves; ivar++){
      unsigned var = ivar+Ncurves*type;
      heff[var] = TGraphAsymmErrors(histo[1][var], histo[0][var]);
      heff[var].SetMarkerStyle(20); heff[var].SetMarkerSize(0.9);
      heff[var].SetMarkerColor(colors[ivar]); heff[var].SetLineColor(colors[ivar]);
      if(ivar==0) {
	heff[var].GetXaxis()->SetTitle(vars[var].title);
	heff[var].GetYaxis()->SetTitle("Efficiency");
	if(do_mu) heff[var].SetTitle("15 GeV #mu trigger");
	else heff[var].SetTitle("15 GeV e trigger");
	heff[var].Draw("ap");
      } else heff[var].Draw("p same");
      label = labtag[type]; label += vars[var].cut; label += " GeV";
      leg[type].AddEntry(&heff[var],label,"pl");
    } // Loop over curves in a plot
    leg[type].Draw();
    pname = "plots/1d/turnon_"+vars[Ncurves*type].varname+(do_mu?"_mu":"_el")+filetype;
    can.SaveAs(pname);
  }

}

