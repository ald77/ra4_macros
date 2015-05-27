#include "make_card.hpp"

#include <cstdlib>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "TMath.h"

#include "small_tree_quick.hpp"
#include "utilities.hpp"
#include "timer.hpp"

namespace ra4 {
  std::string ntuple_date("2015_05_25");
  int minjets(7);
  double midjets(8);
  double mjthresh(400.);
  TString luminosity="10";
}

using namespace ra4;
using namespace std;

int main(int argc, char *argv[]){
  bool include_signal = false; //Include signal when mocking up data from MC?
  string folder="/cms5r0/ald77/archive/"+ntuple_date+"/skim/";

  double lumi = 10.;
  string lumi_string = "10";
  bool compressed = false;
  if(argc>1){
    lumi = strtod(argv[1], NULL);
    lumi_string = argv[1];
    if(argc>2 && string("1200")==argv[2]) compressed = true;
  }
  if(lumi <= 0.0){
    lumi = 10.;
    lumi_string = "10";
  }

  small_tree_quick ttbar(folder+"*TTJets*");
  small_tree_quick other(folder+"*QCD_Pt*");
  other.Add(folder+"*TTWJets*");
  other.Add(folder+"*TTZJets*");
  other.Add(folder+"*_T*-channel*");
  other.Add(folder+"*WJetsToLNu_HT*");
  other.Add(folder+"*ZJetsToLNu_HT*");
  other.Add(folder+"*DYJets*");
  other.Add(folder+"*H_HToBB*");
  small_tree_quick *sig;
  if(compressed){
    sig = new small_tree_quick(folder+"*T1tttt*1200*800*");
  }else{
    sig = new small_tree_quick(folder+"*T1tttt*1500*100*");
  }

  vector<double> ttbar_raw, ttbar_wght;
  vector<double> other_raw, other_wght;
  vector<double> sig_raw, sig_wght;

  GetCounts(lumi, ttbar, ttbar_raw, ttbar_wght);
  GetCounts(lumi, other, other_raw, other_wght);
  GetCounts(lumi, *sig, sig_raw, sig_wght);

  vector<double> mc_raw, mc_wght;
  GetMCTotals(mc_raw, mc_wght,
              ttbar_raw, ttbar_wght,
              other_raw, other_wght);

  vector<double> data_counts;
  MockUpData(data_counts,
             ttbar_raw, ttbar_wght,
             other_raw, other_wght,
             sig_raw, sig_wght,
             include_signal);

  vector<double> kappas, kappa_uncerts;
  GetKappas(mc_raw, mc_wght,
            data_counts,
            kappas, kappa_uncerts);

  WriteFile(ttbar_raw, ttbar_wght,
            other_raw, other_wght,
            sig_raw, sig_wght,
            kappas, kappa_uncerts,
            data_counts, lumi_string, compressed);
}

void GetCounts(double lumi,
               small_tree_quick &tree,
               vector<double> &raw,
               vector<double> &wght){
  vector<double> counts = vector<double>(9, 0.);
  vector<double> squares = counts;
  raw = counts;
  wght = counts;

  double sumw = 0., sumw2 = 0.;

  int num_entries = tree.GetEntries();
  Timer timer(num_entries, 1.);
  timer.Start();
  for(int entry = 0; entry < num_entries; ++entry){
    tree.GetEntry(entry);
    timer.Iterate();

    if(tree.nbm()<2
       || tree.njets()<minjets
       || (tree.nmus()+tree.nels())!=1
       || tree.met()<=200.
       //       || tree.ntks_chg_mini()>0
       || tree.ht()<=500.) continue;

    size_t bin = LookUpBin(tree);

    double weight = lumi*tree.weight();

    counts.at(bin) += weight;
    squares.at(bin) += weight*weight;

    sumw += weight;
    sumw2 += weight*weight;
  }

  for(size_t bin = 0; bin < counts.size(); ++bin){
    if(squares.at(bin) == 0.){
      //Never filled
      raw.at(bin) = 0;
      wght.at(bin) = sumw2/sumw;
    }else if(counts.at(bin) <= 0.){
      //Negative estimate
      raw.at(bin) = 0;
      wght.at(bin) = sqrt(squares.at(bin)+sqr(counts.at(bin)));
    }else{
      //Normal case
      raw.at(bin) = sqr(counts.at(bin))/squares.at(bin);
      wght.at(bin) = squares.at(bin)/counts.at(bin);
    }
  }
}

size_t LookUpBin(small_tree_quick &tree){
  double mt_thresh = 140.;
  double mj_thresh = mjthresh;
  double njets_thresh = midjets;
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
	  return 5;
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

void GetMCTotals(vector<double> &mc_raw, vector<double> &mc_wght,
                 const vector<double> &ttbar_raw, const vector<double> &ttbar_wght,
                 const vector<double> &other_raw, const vector<double> &other_wght){
  mc_raw.resize(ttbar_raw.size());
  mc_wght.resize(ttbar_wght.size());
  for(size_t i = 0; i < mc_raw.size(); ++i){
    if(ttbar_raw.at(i)+other_raw.at(i)<= 0.){
      mc_raw.at(i) = 0.;
      mc_wght.at(i) = AddInQuadrature(ttbar_wght.at(i), other_wght.at(i));
    }else{
      double n = ttbar_raw.at(i)*ttbar_wght.at(i)
        +other_raw.at(i)*other_wght.at(i);
      double sig_sqr = ttbar_raw.at(i)*sqr(ttbar_wght.at(i))
        +other_raw.at(i)*sqr(other_wght.at(i));

      mc_raw.at(i) = sqr(n)/sig_sqr;
      mc_wght.at(i) = sig_sqr/n;
    }
  }
}

void MockUpData(vector<double> &data,
                const vector<double> &ttbar_raw, const vector<double> &ttbar_wght,
                const vector<double> &other_raw, const vector<double> &other_wght,
                const vector<double> &sig_raw, const vector<double> &sig_wght,
                bool use_sig){
  data = vector<double>(ttbar_raw.size(), 0);
  for(size_t i = 0; i < ttbar_raw.size(); ++i){
    data.at(i) = ttbar_raw.at(i)*ttbar_wght.at(i)
      + other_raw.at(i)*other_wght.at(i);
    if(use_sig) data.at(i) += sig_raw.at(i)*sig_wght.at(i);
  }
}

void GetKappas(const vector<double> &mc_raw, const vector<double> &mc_wght,
               const vector<double> &data_counts,
               vector<double> &kappas, vector<double> &kappa_uncerts){
  kappas = vector<double>(2, 1.);
  kappa_uncerts = vector<double>(2, 0.);

  GetKappa(kappas, kappa_uncerts, 0, 0, 1, 3,
           mc_raw, mc_wght, data_counts);
  GetKappa(kappas, kappa_uncerts, 1, 0, 2, 3,
           mc_raw, mc_wght, data_counts);
}

void GetKappa(vector<double> &kappas, vector<double> &kappa_uncerts,
              size_t ikappa, size_t ilowlow, size_t ilowhigh, size_t ihighlow,
              const vector<double> &mc_raw, const vector<double> &mc_wght,
              const vector<double> &data_counts){
  kappas.at(ikappa) = (mc_raw.at(ilowlow)*mc_wght.at(ilowlow))/data_counts.at(ilowlow);
  kappas.at(ikappa) *= data_counts.at(ilowhigh)/(mc_raw.at(ilowhigh)*mc_wght.at(ilowhigh));
  kappas.at(ikappa) *= data_counts.at(ihighlow)/(mc_raw.at(ihighlow)*mc_wght.at(ihighlow));

  kappa_uncerts.at(ikappa) = 1./data_counts.at(ilowlow);
  kappa_uncerts.at(ikappa) += 1./data_counts.at(ilowhigh);
  kappa_uncerts.at(ikappa) += 1./data_counts.at(ihighlow);
  kappa_uncerts.at(ikappa) += 1./mc_raw.at(ilowlow);
  kappa_uncerts.at(ikappa) += 1./mc_raw.at(ilowhigh);
  kappa_uncerts.at(ikappa) += 1./mc_raw.at(ihighlow);

  kappa_uncerts.at(ikappa) = sqrt(kappa_uncerts.at(ikappa));
}

double sqr(double x){
  return x*x;
}

void WriteFile(const vector<double> &ttbar_raw, const vector<double> &ttbar_wght,
               const vector<double> &other_raw, const vector<double> &other_wght,
               const vector<double> &sig_raw, const vector<double> &sig_wght,
               const vector<double> &kappas, const vector<double> &kappa_uncerts,
               const vector<double> &data_counts, const string &lumi_string, bool compressed){
  vector<size_t> bins(0), ikappas(0);
  //bins.push_back(4); ikappas.push_back(0);
  bins.push_back(5); ikappas.push_back(0);
  bins.push_back(6); ikappas.push_back(0);
  bins.push_back(7); ikappas.push_back(1);
  bins.push_back(8); ikappas.push_back(1);
  size_t nbins = bins.size();

  TString file_name = "txt/data_card_"; file_name += (compressed?"c_":"nc_");
  file_name += "mj";
  file_name += mjthresh; file_name += "_njets"; file_name += minjets; file_name += midjets;
  file_name += ("_lumi"+ lumi_string + "_" + ntuple_date + ".txt");

  ofstream file(file_name);
  file << "imax " << nbins << "   number of channels\n";
  file << "jmax 2   number of backgrounds\n";
  file << "kmax " << 2+3*nbins << "  number of nuisance parameters\n";
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
      << ' ' << setw(12) << sig_raw.at(bins.at(bin))*sig_wght.at(bins.at(bin))
      << ' ' << setw(12) << ttbar_raw.at(bins.at(bin))*ttbar_wght.at(bins.at(bin))*kappas.at(ikappas.at(bin))
      << ' ' << setw(12) << other_raw.at(bins.at(bin))*other_wght.at(bins.at(bin))*kappas.at(ikappas.at(bin));
  }
  file << '\n';
  file << "------------\n";
  for(size_t ikappa = 0; ikappa < kappas.size(); ++ikappa){
    file << "kappa" << ikappa << " lnN             ";
    for(size_t bin = 0; bin < nbins; ++bin){
      if(ikappas.at(bin)==ikappa){
        double kappa = 1.+kappa_uncerts.at(ikappas.at(bin));
        file << "            - " << setw(12) << kappa << ' ' << setw(12) << kappa;
      }else{
        file << "            -            -            -";
      }
    }
    file << '\n';
  }

  int gmn_raw;
  double gmn_wght;
  for(size_t rbin = 0; rbin < nbins; ++rbin){
    GetGammaParameters(gmn_raw, gmn_wght,
                       sig_raw.at(bins.at(rbin)), sig_wght.at(bins.at(rbin)));
    file << "sstat" << rbin << " gmN " << setw(12) << gmn_raw;
    for(size_t cbin = 0; cbin < nbins; ++cbin){
      if(cbin == rbin){
        file << ' ' << setw(12) << gmn_wght << "            -            -";
      }else{
        file << "            -            -            -";
      }
    }
    file << '\n';

    GetGammaParameters(gmn_raw, gmn_wght,
                       ttbar_raw.at(bins.at(rbin)), ttbar_wght.at(bins.at(rbin)));
    file << "tstat" << rbin << " gmN " << setw(12) << gmn_raw;
    for(size_t cbin = 0; cbin < nbins; ++cbin){
      if(cbin == rbin){
        file << "            - " << setw(12) << gmn_wght << "            -";
      }else{
        file << "            -            -            -";
      }
    }
    file << '\n';

    GetGammaParameters(gmn_raw, gmn_wght,
                       other_raw.at(bins.at(rbin)), other_wght.at(bins.at(rbin)));
    file << "ostat" << rbin << " gmN " << setw(12) << gmn_raw;
    for(size_t cbin = 0; cbin < nbins; ++cbin){
      if(cbin == rbin){
        file << "            -            - " << setw(12) << gmn_wght;
      }else{
        file << "            -            -            -";
      }
    }
    file << '\n';
  }

  file << flush;
  file.close();

  WriteDebug(bins, ikappas,
             ttbar_raw, ttbar_wght,
             other_raw, other_wght,
             sig_raw, sig_wght,
             kappas, kappa_uncerts,
             data_counts);

  cout<<endl<<"Created data card at "<<file_name<<endl<<endl;

}

void  WriteDebug(const vector<size_t> &bins, const vector<size_t> &ikappas,
                 const vector<double> &ttbar_raw, const vector<double> &ttbar_wght,
                 const vector<double> &other_raw, const vector<double> &other_wght,
                 const vector<double> &sig_raw, const vector<double> &sig_wght,
                 const vector<double> &kappas, const vector<double> &kappa_uncerts,
                 const vector<double> &data_counts){
  for(size_t i = 0; i < bins.size(); ++i){
    size_t ikappa = ikappas.at(i);
    size_t ibin = bins.at(i);
    cout << "Bin " << i << ": " << GetBinName(i) << endl;
    cout << "  Data: "
         << setw(12) << TMath::Nint(data_counts.at(ibin)) << endl;
    cout << " Kappa: "
         << setw(12) << kappas.at(ikappa) << " +- "
         << setw(12) << kappa_uncerts.at(ikappa) << endl;
    cout << "Signal: "
         << setw(12) << sig_raw.at(ibin)*sig_wght.at(ibin) << " +- "
         << setw(12) << sqrt(sig_raw.at(ibin)+1)*sig_wght.at(ibin) << endl;
    cout << " ttbar: "
         << setw(12) << ttbar_raw.at(ibin)*ttbar_wght.at(ibin) << " +- "
         << setw(12) << sqrt(ttbar_raw.at(ibin)+1)*ttbar_wght.at(ibin) << endl;
    cout << " Other: "
         << setw(12) << other_raw.at(ibin)*other_wght.at(ibin) << " +- "
         << setw(12) << sqrt(other_raw.at(ibin)+1)*other_wght.at(ibin) << endl;
    cout << endl;
  }
}

string GetBinName(size_t i){
  switch(i){
  case 0: return "low njets, low met, low nb";
  case 1: return "low njets, low met, high nb";
  case 2: return "low njets, high met";
  case 3: return "high njets, low met";
  case 4: return "high njets, high met";
  default: return "BAD BIN!!!";
  }
}

void GetGammaParameters(int &raw_out, double &weight_out,
                        double raw_in, double weight_in){
  raw_out = TMath::Nint(raw_in);
  weight_out = weight_in;
  if(raw_out>0) weight_out *= raw_in/raw_out;
}
