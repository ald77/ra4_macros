#include "make_card.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "TMath.h"

#include "small_tree_quick.hpp"
#include "timer.hpp"

using namespace std;

int main(){
  bool include_signal = false; //Include signal when mocking up data from MC?
  string folder = "archive/current/skim/";
  double lumi = 10.;

  small_tree_quick ttbar(folder+"*TTJets*");
  small_tree_quick other(folder+"*QCD_Pt*");
  other.Add(folder+"*TTWJets*");
  other.Add(folder+"*TTZJets*");
  other.Add(folder+"*_T*-channel*");
  other.Add(folder+"*WJetsToLNu_HT*");
  other.Add(folder+"*ZJetsToLNu_HT*");
  other.Add(folder+"*DYJets*");
  other.Add(folder+"*H_HToBB*");
  small_tree_quick sig(folder+"*T1tttt*1500*100*");

  vector<double> ttbar_counts, ttbar_squares;
  vector<double> other_counts, other_squares;
  vector<double> sig_counts, sig_squares;

  GetCounts(lumi, ttbar, ttbar_counts, ttbar_squares);
  GetCounts(lumi, other, other_counts, other_squares);
  GetCounts(lumi, sig, sig_counts, sig_squares);

  vector<double> mc_counts, mc_squares;
  GetMCTotals(mc_counts, mc_squares,
	      ttbar_counts, ttbar_squares,
	      other_counts, other_squares);

  vector<double> data_counts;
  MockUpData(data_counts, ttbar_counts, other_counts, sig_counts, include_signal);

  vector<double> kappas, kappa_uncerts;
  GetKappas(mc_counts, mc_squares,
	    data_counts,
	    kappas, kappa_uncerts);

  WriteFile(ttbar_counts, ttbar_squares,
	    other_counts, other_squares,
	    sig_counts, sig_squares,
	    kappas, kappa_uncerts,
	    data_counts);
}

void GetCounts(double lumi,
	       small_tree_quick &tree,
	       vector<double> &counts,
	       vector<double> &squares){
  counts = vector<double>(9, 0.);
  squares = counts;

  int num_entries = tree.GetEntries();
  Timer timer(num_entries, 1.);
  timer.Start();
  for(int entry = 0; entry < num_entries; ++entry){
    tree.GetEntry(entry);
    timer.Iterate();

    if(tree.nbm()<2
       || tree.njets()<6
       || (tree.nmus()+tree.nels())!=1
       || tree.met()<=200.
       || tree.ht()<=500.) continue;

    size_t bin = LookUpBin(tree);

    double weight = lumi*tree.weight();
    counts.at(bin) += weight;
    squares.at(bin) += weight*weight;
  }
}

size_t LookUpBin(small_tree_quick &tree){
  double mt_thresh = 140.;
  double mj_thresh = 500.;
  double njets_thresh = 7.5;
  double met_thresh = 400.;
  double nbm_thresh = 2.5;
  if(tree.mt()<=mt_thresh){
    if(tree.mj()<=mj_thresh){
      return 0;
    }else{
      if(tree.njets()<=njets_thresh){
	return 1;
      }else{
	return 2;
      }
    }
  }else{
    if(tree.mj()<=mj_thresh){
      return 3;
    }else{
      if(tree.njets()<=njets_thresh){
	if(tree.met()<=met_thresh){
	  if(tree.nbm()<=nbm_thresh){
	    return 4;
	  }else{
	    return 5;
	  }
	}else{
	  return 6;
	}
      }else{
	if(tree.met()<=met_thresh){
	  return 7;
	}else{
	  return 8;
	}
      }
    }
  }
}

void GetMCTotals(vector<double> &mc_counts, vector<double> &mc_squares,
		 const vector<double> &ttbar_counts, const vector<double> &ttbar_squares,
		 const vector<double> &other_counts, const vector<double> &other_squares){
  mc_counts = ttbar_counts;
  mc_squares = ttbar_squares;
  for(size_t i = 0; i < mc_counts.size(); ++i){
    mc_counts.at(i) += other_counts.at(i);
    mc_squares.at(i) += other_squares.at(i);
  }
}

void MockUpData(vector<double> &data,
		const vector<double> &ttbar,
		const vector<double> &other,
		const vector<double> &sig,
		bool use_sig){
  data = vector<double>(ttbar.size(), 0);
  for(size_t i = 0; i < ttbar.size(); ++i){
    data.at(i) = ttbar.at(i) + other.at(i);
    if(use_sig) data.at(i) += sig.at(i);
  }
}

void GetKappas(const vector<double> &mc_counts, const vector<double> &mc_squares,
	       const vector<double> &data_counts,
	       vector<double> &kappas, vector<double> &kappa_uncerts){
  kappas = vector<double>(2, 1.);
  kappa_uncerts = vector<double>(2, 0.);

  GetKappa(kappas, kappa_uncerts, 0, 0, 1, 3,
	   mc_counts, mc_squares, data_counts);
  GetKappa(kappas, kappa_uncerts, 1, 0, 2, 3,
	   mc_counts, mc_squares, data_counts);
}

void GetKappa(vector<double> &kappas, vector<double> &kappa_uncerts,
	      size_t ikappa, size_t ilowlow, size_t ilowhigh, size_t ihighlow,
	      const vector<double> &mc_counts, const vector<double> &mc_squares,
	      const vector<double> &data_counts){
  kappas.at(ikappa) = mc_counts.at(ilowlow)/data_counts.at(ilowlow);
  kappas.at(ikappa) *= data_counts.at(ilowhigh)/mc_counts.at(ilowhigh);
  kappas.at(ikappa) *= data_counts.at(ihighlow)/mc_counts.at(ihighlow);

  kappa_uncerts.at(ikappa) = 1./data_counts.at(ilowlow);
  kappa_uncerts.at(ikappa) += 1./data_counts.at(ilowhigh);
  kappa_uncerts.at(ikappa) += 1./data_counts.at(ihighlow);
  kappa_uncerts.at(ikappa) += mc_squares.at(ilowlow)/sqr(mc_counts.at(ilowlow));
  kappa_uncerts.at(ikappa) += mc_squares.at(ilowhigh)/sqr(mc_counts.at(ilowhigh));
  kappa_uncerts.at(ikappa) += mc_squares.at(ihighlow)/sqr(mc_counts.at(ihighlow));

  kappa_uncerts.at(ikappa) = sqrt(kappa_uncerts.at(ikappa));
}

double sqr(double x){
  return x*x;
}

void WriteFile(const vector<double> &ttbar_counts, const vector<double> &ttbar_squares,
	       const vector<double> &other_counts, const vector<double> &other_squares,
	       const vector<double> &sig_counts, const vector<double> &sig_squares,
	       const vector<double> &kappas, const vector<double> &kappa_uncerts,
	       const vector<double> &data_counts){
  vector<size_t> bins(0), ikappas(0);
  bins.push_back(4); ikappas.push_back(0);
  bins.push_back(5); ikappas.push_back(0);
  bins.push_back(6); ikappas.push_back(0);
  bins.push_back(7); ikappas.push_back(1);
  bins.push_back(8); ikappas.push_back(1);
  size_t nbins = bins.size();
  
  ofstream file("data_card.txt");
  file << "imax " << nbins << "   number of channels\n";
  file << "jmax 2   number of backgrounds\n";
  file << "kmax " << 2+3*nbins << "  number of nusiance parameters\n";
  file << "------------\n";
  file << "bin        ";
  for(size_t bin = 0; bin < nbins; ++bin){
    file << ' ' << setw(12) << (bin+1);
  }
  file << '\n';
  file << "observation";
  for(size_t bin = 0; bin < nbins; ++bin){
    file << ' ' << setw(12) << TMath::Nint(data_counts.at(bins.at(bin)));
  }
  file << '\n';
  file << "------------\n";
  file << "bin                    ";
  for(size_t bin = 0; bin < nbins; ++bin){
    for(size_t i = 0; i < 3; ++i){
      file << ' ' << setw(12) << (bin+1);
    }
  }
  file << '\n';
  file << "process                ";
  for(size_t bin = 0; bin < nbins; ++bin){
    file << ' ' << setw(12) << "sig" << ' ' << setw(12) << "ttbar" << ' ' << setw(12) << "other";
  }
  file << '\n';
  file << "process                ";
  for(size_t bin = 0; bin < nbins; ++bin){
    file << "            0            1            2";
  }
  file << '\n';
  file << "rate                   ";
  for(size_t bin = 0; bin < nbins; ++bin){
    file
      << ' ' << setw(12) << sig_counts.at(bins.at(bin))
      << ' ' << setw(12) << ttbar_counts.at(bins.at(bin))*kappas.at(ikappas.at(bin))
      << ' ' << setw(12) << other_counts.at(bins.at(bin))*kappas.at(ikappas.at(bin));
    cout << "Bin " << bin << ':' << endl;
    cout << "     Data: " << setw(12) << data_counts.at(bins.at(bin)) << endl;
    cout << "    kappa: " << setw(12) << kappas.at(ikappas.at(bin)) << " +- " << setw(12) <<  kappas.at(ikappas.at(bin))*kappa_uncerts.at(ikappas.at(bin)) << endl;
    cout << "Signal MC: " << setw(12) << sig_counts.at(bins.at(bin)) << " +- " << setw(12) << sqrt(sig_squares.at(bins.at(bin))) << endl;
    cout << " ttbar MC: " << setw(12) << ttbar_counts.at(bins.at(bin)) << " +- " << setw(12) << sqrt(ttbar_squares.at(bins.at(bin))) << endl;
    cout << " other MC: " << setw(12) << other_counts.at(bins.at(bin)) << " +- " << setw(12) << sqrt(other_squares.at(bins.at(bin))) << endl;
    cout << endl;
  }
  file << '\n';
  file << "------------\n";
  for(size_t ikappa = 0; ikappa < kappas.size(); ++ikappa){
    file << "rwght" << ikappa << " lnN             ";
    for(size_t bin = 0; bin < nbins; ++bin){
      if(ikappas.at(bin)==ikappa){
	double kappa = 1.+kappa_uncerts.at(ikappas.at(bin));
	file << ' ' << setw(12) << kappa << ' ' << setw(12) << kappa << ' ' << setw(12) << kappa;
      }else{
	file << "            -            -            -";
      }
    }
    file << '\n';
  }
  for(size_t rbin = 0; rbin < nbins; ++rbin){
    double sig_wght = sig_squares.at(bins.at(rbin))/sig_counts.at(bins.at(rbin));
    int sig_raw = max(1,TMath::Nint(sig_counts.at(bins.at(rbin))/sig_wght));
    sig_wght = sig_counts.at(bins.at(rbin))/sig_raw;
    file << "sstat" << rbin << " gmN " << setw(12) << sig_raw;
    for(size_t cbin = 0; cbin < nbins; ++cbin){
      if(cbin == rbin){
	file << ' ' << setw(12) << sig_wght << "            -            -";
      }else{
	file << "            -            -            -";
      }
    }
    file << '\n';
    double ttbar_wght = ttbar_squares.at(bins.at(rbin))/ttbar_counts.at(bins.at(rbin));
    int ttbar_raw = max(1,TMath::Nint(ttbar_counts.at(bins.at(rbin))/ttbar_wght));
    ttbar_wght = ttbar_counts.at(bins.at(rbin))/ttbar_raw;
    file << "tstat" << rbin << " gmN " << setw(12) << ttbar_raw;
    for(size_t cbin = 0; cbin < nbins; ++cbin){
      if(cbin == rbin){
	file << "            - " << setw(12) << ttbar_wght << "            -";
      }else{
	file << "            -            -            -";
      }
    }
    file << '\n';
    double other_wght = other_squares.at(bins.at(rbin))/other_counts.at(bins.at(rbin));
    int other_raw = max(1,TMath::Nint(other_counts.at(bins.at(rbin))/other_wght));
    other_wght = other_counts.at(bins.at(rbin))/other_raw;
    file << "ostat" << rbin << " gmN " << setw(12) << other_raw;
    for(size_t cbin = 0; cbin < nbins; ++cbin){
      if(cbin == rbin){
	file << "            -            - " << setw(12) << other_wght;
      }else{
	file << "            -            -            -";
      }
    }
    file << '\n';
  }

  file << flush;
  file.close();
}
