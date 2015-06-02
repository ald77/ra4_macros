#include "make_card.hpp"

#include <cstdlib>

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include <unistd.h>
#include <getopt.h>

#include "TMath.h"

#include "small_tree_quick.hpp"
#include "timer.hpp"
#include "utilities.hpp"

namespace{
  //Options that can all be set with command line args
  int method = 0;
  std::string ntuple_date = "2015_05_25";
  double lumi = 10.;
  bool no_mc_kappa = false;
  bool include_signal = false;

  std::string mgluino = "1500";
  std::string mlsp = "100";

  double mt_min = 0.;
  double mt_div = 140.;
  double mj_min = 0.;
  double mj_div = 400.;
  int njets_min = 7;
  int njets_div = 9;
  double met_min = 200.;
  double met_div = 400.;
  double ht_min = 500.;
  int nb_min = 2;
  int nb_div = 3;
}

using namespace std;

int main(int argc, char *argv[]){
  GetOptions(argc, argv);
  string folder="/cms5r0/ald77/archive/"+ntuple_date+"/skim/";

  small_tree_quick ttbar(folder+"*TTJets*");
  small_tree_quick other(folder+"*QCD_Pt*");
  other.Add(folder+"*TTWJets*");
  other.Add(folder+"*TTZJets*");
  other.Add(folder+"*_T*-channel*");
  other.Add(folder+"*WJetsToLNu_HT*");
  other.Add(folder+"*ZJetsToLNu_HT*");
  other.Add(folder+"*DYJets*");
  other.Add(folder+"*H_HToBB*");
  small_tree_quick sig(folder+"*T1tttt*"+mgluino+"*"+mlsp+"*");

  vector<double> ttbar_raw, ttbar_wght;
  vector<double> other_raw, other_wght;
  vector<double> sig_raw, sig_wght;

  GetCounts(lumi, ttbar, ttbar_raw, ttbar_wght);
  GetCounts(lumi, other, other_raw, other_wght);
  GetCounts(lumi, sig, sig_raw, sig_wght);

  vector<double> mc_raw, mc_wght;
  GetMCTotals(mc_raw, mc_wght,
              ttbar_raw, ttbar_wght,
              other_raw, other_wght);

  vector<double> data_counts;
  MockUpData(data_counts,
             ttbar_raw, ttbar_wght,
             other_raw, other_wght,
             sig_raw, sig_wght);

  WriteFile(ttbar_raw, ttbar_wght,
            other_raw, other_wght,
            sig_raw, sig_wght,
            mc_raw, mc_wght,
            data_counts);
}

void GetOptions(int argc, char *argv[]){
  bool set_gluino = false, set_lsp = false, set_mj_div = false;
  while(true){
    static struct option long_options[] = {
      {"method", required_argument, 0, 'm'},
      {"ntuple_date", required_argument, 0, 'd'},
      {"lumi", required_argument, 0, 'l'},
      {"no_mc_kappa", no_argument, 0, 'k'},
      {"include_signal", no_argument, 0, 's'},
      {"mgluino", required_argument, 0, 0},
      {"mlsp", required_argument, 0, 0},
      {"mt_min", required_argument, 0, 0},
      {"mt_div", required_argument, 0, 0},
      {"mj_min", required_argument, 0, 0},
      {"mj_div", required_argument, 0, 0},
      {"njets_min", required_argument, 0, 0},
      {"njets_div", required_argument, 0, 0},
      {"met_min", required_argument, 0, 0},
      {"met_div", required_argument, 0, 0},
      {"ht_min", required_argument, 0, 0},
      {"nb_min", required_argument, 0, 0},
      {0, 0, 0, 0}
    };

    char opt = -1;
    int option_index;
    opt = getopt_long(argc, argv, "m:d:l:ks", long_options, &option_index);
    if( opt == -1) break;

    string optname;
    switch(opt){
    case 'm':
      method = atoi(optarg);
      break;
    case 'd':
      ntuple_date = optarg;
      break;
    case 'l':
      lumi = atof(optarg);
      break;
    case 'k':
      no_mc_kappa = true;
      break;
    case 's':
      include_signal = true;
      break;
    case 0:
      optname = long_options[option_index].name;
      if(optarg == "mgluino"){
        mgluino = optarg;
        set_gluino = true;
      }else if(optname == "mlsp"){
        mlsp = optarg;
        set_lsp = true;
      }else if(optname == "mt_min"){
        mt_min = atof(optarg);
      }else if(optname == "mt_div"){
        mt_div = atof(optarg);
      }else if(optname == "mj_min"){
        mj_min = atof(optarg);
      }else if(optname == "mj_div"){
        mj_div = atof(optarg);
        set_mj_div = true;
      }else if(optname == "njets_min"){
        njets_min = atoi(optarg);
      }else if(optname == "njets_div"){
        njets_div = atoi(optarg);
      }else if(optname == "met_min"){
        met_min = atof(optarg);
      }else if(optname == "met_div"){
        met_div = atof(optarg);
      }else if(optname == "nb_min"){
        nb_min = atoi(optarg);
      }else if(optname == "nb_div"){
        nb_div = atoi(optarg);
      }else if(optname == "ht_min"){
        ht_min = atof(optarg);
      }else{
        printf("Bad option! Found option name %s\n", optname.c_str());
      }
      break;
    default:
      printf("Bad option! getopt_long returned character code 0%o\n", opt);
      break;
    }
  }

  //Fix masses if only one is set
  if(set_gluino && !set_lsp){
    if(mgluino == "1500"){
      mlsp = "100";
    }else if(mgluino == "1200"){
      mlsp = "800";
    }
  }else if(set_lsp && !set_gluino){
    if(mlsp == "100"){
      mgluino = "1500";
    }else if(mlsp == "800"){
      mgluino = "1200";
    }
  }

  //Adjust mj_div if using method 1 and not manually overridden
  if(method == 1 && !set_mj_div) mj_div = 600.;
}

void GetCounts(double lumi,
               small_tree_quick &tree,
               vector<double> &raw,
               vector<double> &wght){
  size_t nbins = 0;
  switch(method){
  case 0: nbins = 8; break;
  case 1: nbins = 16; break;
  case 2: nbins = 12; break;
  case 3: nbins = 18; break;
  default: break;
  }
  vector<double> counts = vector<double>(nbins, 0.);
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

    if(tree.nbm()<nb_min
       || tree.njets()<njets_min
       || (tree.nmus()+tree.nels())!=1
       || tree.met()<=met_min
       //       || tree.ntks_chg_mini()>0
       || tree.ht()<=ht_min) continue;

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
  unsigned code = 0;
  code += (tree.mt() > mt_div) << 4;
  code += (tree.mj() > mj_div) << 3;
  code += (tree.met() > met_div) << 2;
  code += (tree.njets() >= njets_div) << 1;
  code += (tree.nbm() >= nb_div) << 0;
  switch(method){
  case 0:
    switch(code){
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07: return 0;
    case 0x08:
    case 0x09: return 1;
    case 0x0A:
    case 0x0B: return 2;
    case 0x0C:
    case 0x0D: return 1;
    case 0x0E:
    case 0x0F: return 2;
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17: return 3;
    case 0x18:
    case 0x19: return 4;
    case 0x1A:
    case 0x1B: return 5;
    case 0x1C:
    case 0x1D: return 6;
    case 0x1E:
    case 0x1F: return 7;
    default: return -1;
    }
  case 1:
    switch(code){
    case 0x00:
    case 0x01: return 0;
    case 0x02:
    case 0x03: return 1;
    case 0x04:
    case 0x05: return 2;
    case 0x06:
    case 0x07: return 3;
    case 0x08:
    case 0x09: return 4;
    case 0x0A:
    case 0x0B: return 5;
    case 0x0C:
    case 0x0D: return 6;
    case 0x0E:
    case 0x0F: return 7;
    case 0x10:
    case 0x11: return 8;
    case 0x12:
    case 0x13: return 9;
    case 0x14:
    case 0x15: return 10;
    case 0x16:
    case 0x17: return 11;
    case 0x18:
    case 0x19: return 12;
    case 0x1A:
    case 0x1B: return 13;
    case 0x1C:
    case 0x1D: return 14;
    case 0x1E:
    case 0x1F: return 15;
    default: return -1;
    }
  case 2:
    switch(code){
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03: return 0;
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07: return 1;
    case 0x08:
    case 0x09: return 2;
    case 0x0A:
    case 0x0B: return 3;
    case 0x0C:
    case 0x0D: return 4;
    case 0x0E:
    case 0x0F: return 5;
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13: return 6;
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17: return 7;
    case 0x18:
    case 0x19: return 8;
    case 0x1A:
    case 0x1B: return 9;
    case 0x1C:
    case 0x1D: return 10;
    case 0x1E:
    case 0x1F: return 11;
    default: return -1;
    }
  case 3:
    switch(code){
    case 0x00: return 0;
    case 0x01: return 1;
    case 0x02: return 0;
    case 0x03: return 1;
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07: return 2;
    case 0x08: return 3;
    case 0x09: return 4;
    case 0x0A: return 5;
    case 0x0B: return 6;
    case 0x0C:
    case 0x0D: return 7;
    case 0x0E:
    case 0x0F: return 8;
    case 0x10: return 9;
    case 0x11: return 10;
    case 0x12: return 9;
    case 0x13: return 10;
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17: return 11;
    case 0x18: return 12;
    case 0x19: return 13;
    case 0x1A: return 14;
    case 0x1B: return 15;
    case 0x1C:
    case 0x1D: return 16;
    case 0x1E:
    case 0x1F: return 17;
    default: return -1;
    }
  default: return -1;
  }
  return -1;
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
                const vector<double> &sig_raw, const vector<double> &sig_wght){
  data = vector<double>(ttbar_raw.size(), 0);
  for(size_t i = 0; i < ttbar_raw.size(); ++i){
    data.at(i) = ttbar_raw.at(i)*ttbar_wght.at(i)
      + other_raw.at(i)*other_wght.at(i);
    if(include_signal) data.at(i) += sig_raw.at(i)*sig_wght.at(i);
  }
}

void GetBinMapping(size_t &nr1, vector<size_t> &r1_map,
                   size_t &nr2, vector<size_t> &r2_map,
                   size_t &nr3, vector<size_t> &r3_map,
                   size_t &nr4, vector<size_t> &r4_map){
  nr1 = 0; nr2 = 0; nr3 = 0; nr4 = 0;
  r1_map.clear(); r2_map.clear(); r3_map.clear(); r4_map.clear();
  switch(method){
  case 0:
    nr1 = 1; nr2 = 2; nr3 = 1; nr4 = 4;
    r1_map.push_back(0); r2_map.push_back(0); r3_map.push_back(0); r4_map.push_back(0);
    r1_map.push_back(0); r2_map.push_back(1); r3_map.push_back(0); r4_map.push_back(1);
    r1_map.push_back(0); r2_map.push_back(0); r3_map.push_back(0); r4_map.push_back(2);
    r1_map.push_back(0); r2_map.push_back(1); r3_map.push_back(0); r4_map.push_back(3);
    break;
  case 1:
    nr1 = 4; nr2 = 4; nr3 = 4; nr4 = 4;
    r1_map.push_back(0); r2_map.push_back(0); r3_map.push_back(0); r4_map.push_back(0);
    r1_map.push_back(1); r2_map.push_back(1); r3_map.push_back(1); r4_map.push_back(1);
    r1_map.push_back(2); r2_map.push_back(2); r3_map.push_back(2); r4_map.push_back(2);
    r1_map.push_back(3); r2_map.push_back(3); r3_map.push_back(3); r4_map.push_back(3);
    break;
  case 2:
    nr1 = 2; nr2 = 4; nr3 = 2; nr4 = 4;
    r1_map.push_back(0); r2_map.push_back(0); r3_map.push_back(0); r4_map.push_back(0);
    r1_map.push_back(0); r2_map.push_back(1); r3_map.push_back(0); r4_map.push_back(1);
    r1_map.push_back(1); r2_map.push_back(2); r3_map.push_back(1); r4_map.push_back(2);
    r1_map.push_back(1); r2_map.push_back(3); r3_map.push_back(1); r4_map.push_back(3);
    break;
  case 3:
    nr1 = 3; nr2 = 6; nr3 = 3; nr4 = 6;
    r1_map.push_back(0); r2_map.push_back(0); r3_map.push_back(0); r4_map.push_back(0);
    r1_map.push_back(1); r2_map.push_back(1); r3_map.push_back(1); r4_map.push_back(1);
    r1_map.push_back(0); r2_map.push_back(2); r3_map.push_back(0); r4_map.push_back(2);
    r1_map.push_back(1); r2_map.push_back(3); r3_map.push_back(1); r4_map.push_back(3);
    r1_map.push_back(2); r2_map.push_back(4); r3_map.push_back(2); r4_map.push_back(4);
    r1_map.push_back(2); r2_map.push_back(5); r3_map.push_back(2); r4_map.push_back(5);
    break;
  default:
    break;
  }
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
               const vector<double> &mc_raw, const vector<double> &mc_wght,
               const vector<double> &data_counts){
  size_t nr1, nr2, nr3, nr4;
  vector<size_t> r1_map, r2_map, r3_map, r4_map;
  GetBinMapping(nr1, r1_map, nr2, r2_map, nr3, r3_map, nr4, r4_map);

  ostringstream file_name;
  file_name << "txt/data_card_" << ntuple_date
            << "_method_" << method
            << (no_mc_kappa?"_no":"_with") << "_mc_kappa"
            << "_T1tttt_" << mgluino << '_' << mlsp
            << "_lumi_" << lumi
            << "_ht_" << ht_min
            << "_mt_" << mt_min << '_' << mt_div
            << "_mj_" << mj_min << '_' << mj_div
            << "_njets_" << njets_min << '_' << njets_div
            << "_met_" << met_min << '_' << met_div
            << ".txt" << flush;

  ofstream file(file_name.str().c_str());
  file << "imax " << nr4 << "   number of channels\n";
  file << "jmax 2   number of backgrounds\n";
  file << "kmax " << ((no_mc_kappa?1:2)*(nr1+nr2+nr3)+3*nr4) << "  number of nuisance parameters\n";
  file << "------------\n";
  file << "bin        ";
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    file << ' ' << setw(12) << (ir4+1);
  }
  file << '\n';
  file << "observation";
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    file << ' ' << setw(12) << data_counts.at(ir4+nr1+nr2+nr3);
  }
  file << '\n';
  file << "------------\n";
  file << "bin                       ";
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    for(size_t i = 0; i < 3; ++i){
      file << ' ' << setw(12) << (ir4+1);
    }
  }
  file << '\n';
  file << "process                   ";
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    file << ' ' << setw(12) << "sig" << ' ' << setw(12) << "ttbar" << ' ' << setw(12) << "other";
  }
  file << '\n';
  file << "process                   ";
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    file << "            0            1            2";
  }
  file << '\n';
  file << "rate                      ";
  vector<double> sig_pred(0), ttbar_pred(0), other_pred(0);
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    size_t ir1 = r1_map.at(ir4);
    size_t ir2 = r2_map.at(ir4)+nr1;
    size_t ir3 = r3_map.at(ir4)+nr1+nr2;
    size_t iir4 = r4_map.at(ir4)+nr1+nr2+nr3;
    sig_pred.push_back(sig_raw.at(iir4)*sig_wght.at(iir4));
    ttbar_pred.push_back(GetPred(data_counts, mc_raw, mc_wght, ttbar_raw, ttbar_wght, ir1, ir2, ir3, iir4));
    other_pred.push_back(GetPred(data_counts, mc_raw, mc_wght, other_raw, other_wght, ir1, ir2, ir3, iir4));
    file
      << ' ' << setw(12) << sig_pred.back()
      << ' ' << setw(12) << ttbar_pred.back()
      << ' ' << setw(12) << other_pred.back();
  }
  file << '\n';
  file << "------------\n";

  //Print control region uncertainty from data
  PrintLogN(file, r1_map, "data_r1", 1, nr1, nr2, nr3, nr4, data_counts);
  PrintLogN(file, r2_map, "data_r2", 2, nr1, nr2, nr3, nr4, data_counts);
  PrintLogN(file, r3_map, "data_r3", 3, nr1, nr2, nr3, nr4, data_counts);

  if(no_mc_kappa){
    //NEED TO FIX THIS!!!
    //Faking this for now and just using uncertainty on MC bin count for statistical shape uncertainty.
    //Does not take into account uncertainty on MC R4 total or anti-correlation between bin fractions.
    PrintGamma(file, r4_map, "sgnl_r4", 4, nr1, nr2, nr3, nr4, sig_raw, sig_wght, sig_pred, 0);
    PrintGamma(file, r4_map, "ttbr_r4", 4, nr1, nr2, nr3, nr4, ttbar_raw, ttbar_wght, ttbar_pred, 1);
    PrintGamma(file, r4_map, "othr_r4", 4, nr1, nr2, nr3, nr4, other_raw, other_wght, other_pred, 2);
  }else{
    //Print MC kappa uncertainties
    PrintLogN(file, r1_map, "mc___r1", 1, nr1, nr2, nr3, nr4, mc_raw);
    PrintLogN(file, r2_map, "mc___r2", 2, nr1, nr2, nr3, nr4, mc_raw);
    PrintLogN(file, r3_map, "mc___r3", 3, nr1, nr2, nr3, nr4, mc_raw);
    PrintGamma(file, r4_map, "sgnl_r4", 4, nr1, nr2, nr3, nr4, sig_raw, sig_wght, sig_pred, 0);
    PrintGamma(file, r4_map, "ttbr_r4", 4, nr1, nr2, nr3, nr4, ttbar_raw, ttbar_wght, ttbar_pred, 1);
    PrintGamma(file, r4_map, "othr_r4", 4, nr1, nr2, nr3, nr4, other_raw, other_wght, other_pred, 2);
  }

  file << flush;
  file.close();

  cout << endl << "Created data card at " << file_name.str() << endl << endl;
}

string GetBinName(size_t bin){
  switch(method){
  case 0:
  case 1:
  case 2:
    switch(bin){
    case 0: return "low met, low njets";
    case 1: return "low met, high njets";
    case 2: return "high met, low njets";
    case 3: return "high met, high njets";
    default: break;
    }
  case 3:
    switch(bin){
    case 0: return "low met, low njets, low nb";
    case 1: return "low met, low njets, high nb";
    case 2: return "low met, high njets, low nb";
    case 3: return "low met, high njets, high nb";
    case 4: return "high met, low njets";
    case 5: return "high met, high njets";
    default: break;
    }
  default: break;
  }
  TString msg = "Bad method/bin: ";
  msg += method;
  msg += '/';
  msg == bin;
  return msg.Data();
}

void GetGammaParameters(int &raw_out, double &weight_out,
                        double raw_in, double weight_in, double pred_in){
  raw_out = TMath::Nint(raw_in);
  if(raw_out>0){
    weight_out = pred_in/raw_out;
  }else{
    weight_out = weight_in;
  }
}

double GetPred(const vector<double> &data,
               const vector<double> &mc_raw,
               const vector<double> &mc_wght,
               const vector<double> &proc_raw,
               const vector<double> &proc_wght,
               size_t ir1,
               size_t ir2,
               size_t ir3,
               size_t ir4){
  //Full data prediction for bin
  double pred = data.at(ir2)*data.at(ir3)/data.at(ir1);

  //Fraction belonging to this process
  pred *= (proc_raw.at(ir4)*proc_wght.at(ir4))/(mc_raw.at(ir4)*mc_wght.at(ir4));

  //MC kappa correction
  if(!no_mc_kappa){
    pred *= mc_raw.at(ir1)*mc_wght.at(ir1);
    pred /= mc_raw.at(ir2)*mc_wght.at(ir2);
    pred /= mc_raw.at(ir3)*mc_wght.at(ir3);
    pred *= mc_raw.at(ir4)*mc_wght.at(ir4);
  }else{
    if(method==0){
      //Deal with MET spectrum from MC due to incomplete binning in R2
      size_t id1 = 0, id2 = 0;
      switch(ir4){
      case 4:
      case 6: id1 = 4; id2 = 6; break;
      case 5:
      case 7: id1 = 5; id2 = 7; break;
      default: break;
      }
      pred *= proc_raw.at(ir4)*proc_wght.at(ir4);
      pred /= proc_raw.at(id1)*proc_wght.at(id1)+proc_raw.at(id2)*proc_wght.at(id2);
    }
  }

  return pred;
}

void PrintLogN(ofstream &file, const vector<size_t> map,
               const string &name, size_t ibin,
               size_t nr1, size_t nr2, size_t nr3, size_t nr4,
               const vector<double> &raw_counts){
  size_t nri = -1; size_t offset = 0;
  switch(ibin){
  case 1: nri = nr1; offset = 0; break;
  case 2: nri = nr2; offset = nr1; break;
  case 3: nri = nr3; offset = nr1+nr2; break;
  case 4: nri = nr4; offset = nr1+nr2+nr3; break;
  default: break;
  }
  for(size_t iri = 0; iri < nri; ++iri){
    file << name << '_' << (iri+1) << " lnN             ";
    for(size_t ir4 = 0; ir4 < nr4; ++ir4){
      if(map.at(ir4) == iri){
        double val = 1.+1./sqrt(raw_counts.at(iri+offset));
        file << "            - " << setw(12) << val << ' ' << setw(12) << val;
      }else{
        file << "            -            -            -";
      }
    }
    file << '\n';
  }
}

void PrintLogN(ofstream &file, const vector<size_t> map,
               const string &name, size_t ibin,
               size_t nr1, size_t nr2, size_t nr3, size_t nr4,
               const vector<double> &raw_counts, size_t iproc){
  size_t nri = -1; size_t offset = 0;
  switch(ibin){
  case 1: nri = nr1; offset = 0; break;
  case 2: nri = nr2; offset = nr1; break;
  case 3: nri = nr3; offset = nr1+nr2; break;
  case 4: nri = nr4; offset = nr1+nr2+nr3; break;
  default: break;
  }
  for(size_t iri = 0; iri < nri; ++iri){
    file << name << '_' << (iri+1) << " lnN             ";
    for(size_t ir4 = 0; ir4 < nr4; ++ir4){
      if(map.at(ir4) == iri){
        double val = 1.+1./sqrt(raw_counts.at(iri+offset));
        for(size_t i = 0; i < 3; ++i){
          if(i == iproc){
            file << ' ' << setw(12) << val;
          }else{
            file << ' ' << setw(12) << '-';
          }
        }
      }else{
        file << "            -            -            -";
      }
    }
    file << '\n';
  }
}

void PrintGamma(ofstream &file, const vector<size_t> map,
                const string &name, size_t ibin,
                size_t nr1, size_t nr2, size_t nr3, size_t nr4,
                const vector<double> &raw_counts,
                const vector<double> &weights,
                const vector<double> &preds,
                size_t iproc){
  size_t nri = -1; size_t offset = 0;
  switch(ibin){
  case 1: nri = nr1; offset = 0; break;
  case 2: nri = nr2; offset = nr1; break;
  case 3: nri = nr3; offset = nr1+nr2; break;
  case 4: nri = nr4; offset = nr1+nr2+nr3; break;
  default: break;
  }
  for(size_t iri = 0; iri < nri; ++iri){
    int gmn_raw;
    double gmn_wght;
    GetGammaParameters(gmn_raw, gmn_wght, raw_counts.at(iri+offset), weights.at(iri+offset), preds.at(iri));
    file << name << '_' << (iri+1) << " gmN " << setw(12) << gmn_raw;
    for(size_t ir4 = 0; ir4 < nr4; ++ir4){
      if(map.at(ir4) == iri){
        for(size_t i = 0; i < 3; ++i){
          if(i == iproc){
            file << ' ' << setw(12) << gmn_wght;
          }else{
            file << ' ' << setw(12) << '-';
          }
        }
      }else{
        file << "            -            -            -";
      }
    }
    file << '\n';
  }
}
