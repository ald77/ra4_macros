#include "binning.hpp"

#include <iostream>

#include "RooStats/NumberCountingUtils.h"
#include "TMath.h"

#include "small_tree_quick.hpp"
#include "timer.hpp"
#include "utilities.hpp"

using namespace std;

int main(){
  small_tree_quick bkg("archive/current/*TTJets*");
  bkg.Add("archive/current/*T_*-channel*");
  bkg.Add("archive/current/*WJetsToLNu*");
  bkg.Add("archive/current/*ZJetsToLNu*");
  bkg.Add("archive/current/*TTWJets*");
  bkg.Add("archive/current/*TTZJets*");
  small_tree_quick sig("archive/current/*T1tttt*1500*100*");

  double low_mt_counts = 0., high_mt_counts = 0.;
  double low_mt_squares = 0., high_mt_squares = 0.;
  double low_mt_counts_sig = 0., high_mt_counts_sig = 0.;
  double low_mt_squares_sig = 0., high_mt_squares_sig = 0.;

  GetCounts(bkg, low_mt_counts, low_mt_squares, high_mt_counts, high_mt_squares);
  GetCounts(sig, low_mt_counts_sig, low_mt_squares_sig, high_mt_counts_sig, high_mt_squares_sig);

  PrintLine(high_mt_counts, high_mt_squares, low_mt_counts, low_mt_squares, high_mt_counts_sig, high_mt_squares_sig);
}

void Fill(double &counts, double &squares, const small_tree_quick &tree){
  double lumi = 4.;
  double weight = tree.weight();
  counts += lumi*weight;
  squares += lumi*lumi*weight*weight;
}

double PredUncert(double h, double sh, double l, double sl){
  return sqrt(h*h*sl/(l*l)+h*h/l+sh);
}

void GetCounts(small_tree_quick &tree, double &low, double &low_sq, double &high, double &high_sq){
  low=0.;
  low_sq=0.;
  high=0.;
  high_sq=0.;

  int num_entries = tree.GetEntries();
  
  Timer timer(num_entries, 1.);
  timer.Start();
  for(int entry = 0; entry < num_entries; ++entry){
    tree.GetEntry(entry);
    timer.Iterate();

    if(tree.nleps()!=1 || tree.nbl()<2 || tree.njets()<6 || tree.met()<250 || tree.ht()<750. || !IsoTkChargeVeto(tree)) continue;

    //if(tree.mj()<600.) continue;

    if(tree.mt()<150.){
      Fill(low, low_sq, tree);
    }else{
      Fill(high, high_sq, tree);
    }
  }  
}

void PrintLine(double h, double sh, double l, double sl, double sig, double ssig){
  double uncert = PredUncert(h, sh, l, sl);
  cout
    << " & $" << l << "\\pm" << sqrt(sl)
    << "$ & $" << h << "\\pm" << sqrt(sh)
    << "$ & $" << h << "\\pm" << uncert
    << "$ & $" << sig << "\\pm" << sqrt(ssig)
    << "$ & $" << RooStats::NumberCountingUtils::BinomialExpZ(sig, h, uncert/h) << "$\\\\" << endl;
}

bool IsoTkChargeVeto(const small_tree_quick &tree){
  int charge;
  float mt;
  bool is_el;
  size_t the_lep;
  CountLepsNew(tree, mt, charge, is_el, the_lep);
  unsigned isotks = 0;
  for(size_t itk = 0; itk < tree.tks_pt().size() && isotks < 2; ++itk){
    switch(abs(tree.tks_id().at(itk))){
    case 11:
    case 13:
      if(tree.tks_pt().at(itk)>5.
         && tree.tks_r03_ch().at(itk)<0.2
	 && Sign(TMath::Nint(tree.tks_id().at(itk)))*charge>0){
	++isotks;
      }
      break;
    default:
      if(tree.tks_pt().at(itk)>10.
         && tree.tks_r03_ch().at(itk)<0.1
	 && Sign(TMath::Nint(tree.tks_id().at(itk)))*charge<0){
	++isotks;
      }
      break;
    }
  }
  return isotks < 1;
}

int CountLepsNew(const small_tree_quick &tree,
                 float &mt,
		 int &charge,
		 bool &is_el,
		 size_t &the_lep){
  unsigned leps = 0;
  the_lep = -1;
  is_el = false;
  bool found_one = false;
  double best_pt = 0.;
  for(size_t i = 0; i < tree.mus_pt().size(); ++i){
    if(tree.mus_pt().at(i)>20.
       && tree.mus_sigid().at(i)
       && min(tree.mus_reliso_r02().at(i),tree.mus_miniso_tr10().at(i))<0.1){
      ++leps;
      if(tree.mus_pt().at(i)>best_pt){
	charge = Sign(TMath::Nint(3.*tree.mus_charge().at(i)));
	best_pt = tree.mus_pt().at(i);
	the_lep = i;
	is_el = false;
        found_one = true;
      }
    }
  }

  for(size_t i = 0; i < tree.els_pt().size(); ++i){
    if(tree.els_pt().at(i)>20.
       && tree.els_ispf().at(i)
       && tree.els_sigid().at(i)
       && min(tree.els_reliso_r02().at(i),tree.els_miniso_tr10().at(i)) < 0.1){
      ++leps;
      if(tree.els_pt().at(i)>best_pt){
	charge = Sign(TMath::Nint(3.*tree.els_charge().at(i)));
	best_pt = tree.els_pt().at(i);
        the_lep = i;
        is_el = true;
        found_one = true;
      }
    }
  }

  if(!found_one){
    mt = 0;
  }else{
    if(is_el){
      mt = GetMT(tree.els_pt().at(the_lep), tree.els_phi().at(the_lep),
                 tree.met(), tree.met_phi());
    }else{
      mt = GetMT(tree.mus_pt().at(the_lep), tree.mus_phi().at(the_lep),
                 tree.met(), tree.met_phi());
    }
  }

  return leps;
}
