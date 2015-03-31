#include "make_card.hpp"

#include <numeric>
#include <vector>
#include <fstream>
#include <iomanip>

#include "TMath.h"

#include "small_tree_quick.hpp"
#include "timer.hpp"

using namespace std;

int main(){
  size_t nbins = 1;
  double lumi = 4.;

  small_tree_quick bkg("archive/current/*QCD_HT*");
  bkg.Add("archive/current/*TTJets*");
  bkg.Add("archive/current/*TTWJets*");
  bkg.Add("archive/current/*TTZJets*");
  bkg.Add("archive/current/*_T*-channel*");
  bkg.Add("archive/current/*WJetsToLNu_HT*");
  bkg.Add("archive/current/*ZJetsToNuNu_HT*");
  bkg.Add("archive/current/*DYJets*");
  bkg.Add("archive/current/*H_HToBB*");
  small_tree_quick sig("archive/current/*T1tttt*1500*100*");

  vector<double> bkg_low_cnt(0), bkg_low_sqr(0), bkg_hgh_cnt(0), bkg_hgh_sqr(0);
  vector<double> sig_low_cnt(0), sig_low_sqr(0), sig_hgh_cnt(0), sig_hgh_sqr(0);

  GetCounts(nbins, lumi, bkg, bkg_low_cnt, bkg_low_sqr, bkg_hgh_cnt, bkg_hgh_sqr);
  GetCounts(nbins, lumi, sig, sig_low_cnt, sig_low_sqr, sig_hgh_cnt, sig_hgh_sqr);

  double kappa_uncert = KappaUncert(bkg_low_cnt, bkg_low_sqr);

  ofstream file("data_card.txt");
  file << "imax " << nbins << "  number of channels\n";
  file << "jmax 1  number of backgrounds\n";
  file << "kmax " << 1+2*nbins << "  number of nuisance parameters\n";
  file << "------------\n";
  file << "bin        ";
  for(size_t bin = 0; bin < nbins; ++bin) file << ' ' << setw(12) << (bin+1);
  file << '\n';
  file << "observation";
  for(size_t bin = 0; bin < nbins; ++bin) file << ' ' << setw(12) << TMath::Nint(bkg_hgh_cnt.at(bin));
  file << '\n';
  file << "------------\n";
  file << "bin                    ";
  for(size_t bin = 0; bin < nbins; ++bin) file << ' ' << setw(12) << (bin+1) << ' ' << setw(12) << (bin+1);
  file << '\n';
  file << "process                ";
  for(size_t bin = 0; bin < nbins; ++bin) file << ' ' << setw(12) << "sig" << ' ' << setw(12) << "bkg";
  file << '\n';
  file << "process                ";
  for(size_t bin = 0; bin < nbins; ++bin) file << "            0            1";
  file << '\n';
  file << "rate                   ";
  for(size_t bin = 0; bin < nbins; ++bin) file << ' ' << setw(12) << sig_hgh_cnt.at(bin) << ' ' << setw(12) << bkg_hgh_cnt.at(bin);
  file << '\n';
  file << "------------\n";
  file << "rewght lnN             ";
  for(size_t bin = 0; bin < nbins; ++bin) file << "            - " << setw(12) << 1.+kappa_uncert;
  file << '\n';
  for(size_t rbin = 0; rbin < nbins; ++rbin){
    double sig_wght = sig_hgh_sqr.at(rbin)/sig_hgh_cnt.at(rbin);
    int sig_raw = max(1,TMath::Nint(sig_hgh_cnt.at(rbin)/sig_wght));
    sig_wght = sig_hgh_cnt.at(rbin)/sig_raw;
    file << "sstat" << rbin << " gmN " << setw(12) << sig_raw;
    for(size_t cbin = 0; cbin < nbins; ++cbin){
      if(cbin == rbin){
	file << ' ' << setw(12) << sig_wght << "            -";
      }else{
	file << "            -            -";
      }
    }
    file << '\n';
    double bkg_wght = bkg_hgh_sqr.at(rbin)/bkg_hgh_cnt.at(rbin);
    int bkg_raw = max(1,TMath::Nint(bkg_hgh_cnt.at(rbin)/bkg_wght));
    bkg_wght = bkg_hgh_cnt.at(rbin)/bkg_raw;
    file << "bstat" << rbin << " gmN " << setw(12) << bkg_raw;
    for(size_t cbin = 0; cbin < nbins; ++cbin){
      if(cbin == rbin){
	file << "            - " << setw(12) << bkg_wght;
      }else{
	file << "            -            -";
      }
    }
    file << '\n';
  }
  file << flush;
  file.close();
}

void GetCounts(size_t nbins,
	       double lumi,
	       small_tree_quick &tree,
	       vector<double> &low_cnt,
	       vector<double> &low_sqr,
	       vector<double> &hgh_cnt,
	       vector<double> &hgh_sqr){
  low_cnt = vector<double>(nbins, 0.);
  low_sqr = vector<double>(nbins, 0.);
  hgh_cnt = vector<double>(nbins, 0.);
  hgh_sqr = vector<double>(nbins, 0.);

  int num_entries = tree.GetEntries();

  Timer timer(num_entries, 1.);
  timer.Start();
  for(int entry = 0; entry < num_entries; ++entry){
    tree.GetEntry(entry);
    timer.Iterate();

    if(tree.nbm()<2 || tree.njets()<6 || (tree.nmus()+tree.nels())!=1 || tree.met()<250. || tree.ht()<500. || tree.mj()<600.) continue;

    double weight = lumi*tree.weight();

    if(tree.mt()>150.){
      Fill(tree, hgh_cnt, hgh_sqr, weight);
    }else{
      Fill(tree, low_cnt, low_sqr, weight);
    }
  }
}

void Fill(small_tree_quick &/*tree*/,
	  vector<double> &cnt,
	  vector<double> &sqr,
	  double weight){
  size_t bin = 0;

  cnt.at(bin) += weight;
  sqr.at(bin) += weight*weight;
}

double KappaUncert(const vector<double> &cnt,
		   const vector<double> &sqr){
  double cnt_sum = accumulate(cnt.begin(), cnt.end(), 0.);
  double sqr_sum = accumulate(sqr.begin(), sqr.end(), 0.);
  return sqrt(cnt_sum+sqr_sum)/cnt_sum;
}
