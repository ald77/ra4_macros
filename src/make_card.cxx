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
  std::string ntuple_date = "2015_06_05";
  double lumi = 10.;
  double mc_multiplier = 1.;
  bool no_mc_kappa = false;
  bool no_dilep = false;
  bool no_systematics = false;

  std::string mgluino = "1500";
  std::string mlsp = "100";

  double inject_signal = 0.;

  bool do_tk_veto = false;

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

  //Track whether option was set in command line for logging purposes
  bool set_method = false;
  bool set_ntuple_date = false;
  bool set_lumi = false;
  bool set_mc_multiplier = false;

  bool set_masses = false;
  bool set_inject_signal = false;

  bool set_mt = false;
  bool set_mj = false;
  bool set_njets = false;
  bool set_met = false;
  bool set_ht = false;
  bool set_nb = false;
}

using namespace std;

int main(int argc, char *argv[]){
  GetOptions(argc, argv);
  string folder="/cms5r0/ald77/archive/"+ntuple_date+"/skim/";
  //string folder="/afs/cern.ch/user/m/manuelf/work/ucsb/"+ntuple_date+"/skim/";
  cout << folder << endl;

  small_tree_quick ttbar(folder+"*TTJets*");
  small_tree_quick other(folder+"*QCD_Pt*");
  other.Add(folder+"*TTWJets*");
  other.Add(folder+"*TTZJets*");
  other.Add(folder+"*_T*-channel*");
  other.Add(folder+"*WJetsToLNu_HT*");
  other.Add(folder+"*ZJetsToLNu_HT*");
  other.Add(folder+"*DYJets*");
  other.Add(folder+"*H_HToBB*");
  small_tree_quick full(folder+"*TTJets*");
  full.Add(folder+"*QCD_Pt*");
  full.Add(folder+"*TTWJets*");
  full.Add(folder+"*TTZJets*");
  full.Add(folder+"*_T*-channel*");
  full.Add(folder+"*WJetsToLNu_HT*");
  full.Add(folder+"*ZJetsToLNu_HT*");
  full.Add(folder+"*DYJets*");
  full.Add(folder+"*H_HToBB*");
  small_tree_quick sig(folder+"*T1tttt*"+mgluino+"*"+mlsp+"*");

  //Single lepton region
  vector<double> ttbar_raw, ttbar_wght;
  vector<double> other_raw, other_wght;
  vector<double> sig_raw, sig_wght;
  vector<double> dilep_raw, dilep_wght;
  GetCounts(ttbar, ttbar_raw, ttbar_wght);
  GetCounts(other, other_raw, other_wght);
  GetCounts(sig, sig_raw, sig_wght);
  GetCounts(full, dilep_raw, dilep_wght, true);

  vector<double> dilep_count = dilep_raw;
  for(size_t i = 0; i < dilep_count.size(); ++i){
    dilep_count.at(i) *= dilep_wght.at(i);
  }

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
            data_counts, dilep_count);
}

void GetOptions(int argc, char *argv[]){
  bool set_gluino = false, set_lsp = false, set_mj_div = false;
  while(true){
    static struct option long_options[] = {
      {"method", required_argument, 0, 'm'},
      {"ntuple_date", required_argument, 0, 'd'},
      {"lumi", required_argument, 0, 'l'},
      {"mc_multiplier", required_argument, 0, 0},
      {"no_mc_kappa", no_argument, 0, 0},
      {"no_dilep", no_argument, 0, 0},
      {"no_systematics", no_argument, 0, 0},
      {"inject_signal", required_argument, 0, 's'},
      {"mgluino", required_argument, 0, 0},
      {"mlsp", required_argument, 0, 0},
      {"tk_veto", no_argument, 0, 't'},
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
    opt = getopt_long(argc, argv, "m:d:l:s:t", long_options, &option_index);
    if( opt == -1) break;

    string optname;
    switch(opt){
    case 'm':
      method = atoi(optarg);
      set_method = true;
      break;
    case 'd':
      ntuple_date = optarg;
      set_ntuple_date = true;
      break;
    case 'l':
      lumi = atof(optarg);
      set_lumi = true;
      break;
    case 's':
      inject_signal = atof(optarg);
      set_inject_signal = true;
      break;
    case 't':
      do_tk_veto = true;
      break;
    case 0:
      optname = long_options[option_index].name;
      if(optname == "mc_multiplier"){
	mc_multiplier = atof(optarg);
	set_mc_multiplier = true;
      }else if(optname == "no_mc_kappa"){
        no_mc_kappa = true;
      }else if(optname == "no_dilep"){
        no_dilep = true;
      }else if(optname == "no_systematics"){
        no_systematics = true;
      }else if(optname == "mgluino"){
        mgluino = optarg;
        set_gluino = true;
        set_masses = true;
      }else if(optname == "mlsp"){
        mlsp = optarg;
        set_lsp = true;
        set_masses = true;
      }else if(optname == "mt_min"){
        mt_min = atof(optarg);
        set_mt = true;
      }else if(optname == "mt_div"){
        mt_div = atof(optarg);
        set_mt = true;
      }else if(optname == "mj_min"){
        mj_min = atof(optarg);
        set_mj = true;
      }else if(optname == "mj_div"){
        mj_div = atof(optarg);
        set_mj = true;
        set_mj_div = true;
      }else if(optname == "njets_min"){
        njets_min = atoi(optarg);
        set_njets = true;
      }else if(optname == "njets_div"){
        njets_div = atoi(optarg);
        set_njets = true;
      }else if(optname == "met_min"){
        met_min = atof(optarg);
        set_met = true;
      }else if(optname == "met_div"){
        met_div = atof(optarg);
        set_met = true;
      }else if(optname == "nb_min"){
        nb_min = atoi(optarg);
        set_nb = true;
      }else if(optname == "nb_div"){
        nb_div = atoi(optarg);
        set_nb = true;
      }else if(optname == "ht_min"){
        ht_min = atof(optarg);
        set_ht = true;
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

void GetCounts(small_tree_quick &tree,
               vector<double> &raw,
               vector<double> &wght,
               bool dilepton_mode){
  size_t nbins = 0;
  if(dilepton_mode){
    nbins = 2;
  }else{
    switch(method){
    case 0: nbins = 8; break;
    case 1: nbins = 16; break;
    case 2: nbins = 12; break;
    case 3: nbins = 18; break;
    default: break;
    }
  }
  vector<double> counts(nbins, 0.);
  vector<double> squares(nbins, 0.);
  raw = counts;
  wght = counts;

  double sumw = 0., sumw2 = 0.;

  int num_entries = tree.GetEntries();
  Timer timer(num_entries, 1.);
  timer.Start();
  for(int entry = 0; entry < num_entries; ++entry){
    tree.GetEntry(entry);
    timer.Iterate();

    double weight = lumi*tree.weight();

    bool fail = false;
    if(dilepton_mode){
      fail = (tree.nbm()<1
              || tree.njets()<(njets_min-1)
              || (tree.nmus()+tree.nels())!=2
              || tree.met()<=met_min
              || tree.ht()<=ht_min);
    }else{
      fail = (tree.nbm()<nb_min
              || tree.njets()<njets_min
              || (tree.nmus()+tree.nels())!=1
              || tree.met()<=met_min
              || (do_tk_veto && tree.ntks_chg_mini()>0)
              || tree.ht()<=ht_min);
    }

    if(fail) continue;

    size_t bin;
    if(dilepton_mode){
      if(tree.njets()>=njets_div){
        bin = 1;
      }else{
        bin = 0;
      }
    }else{
      bin = LookUpBin(tree);
    }

    counts.at(bin) += weight;
    squares.at(bin) += weight*weight;

    sumw += weight;
    sumw2 += weight*weight;
  }
  sumw2 /= mc_multiplier;

  for(size_t bin = 0; bin < counts.size(); ++bin){
    squares.at(bin) /= mc_multiplier;
    CountsToGammas(counts.at(bin), squares.at(bin),
                   sumw, sumw2,
                   raw.at(bin), wght.at(bin));
  }
}

void CountsToGammas(double sumw, double sumw2,
                    double sumw_backup, double sumw2_backup,
                    double &nraw, double &weight){
  if(sumw2 == 0.){
    //Never filled
    nraw = 0.;
    weight = sumw2_backup/sumw_backup;
  }else if(sumw <= 0.){
    //Negative estimate
    nraw = 0;
    weight = sqrt(sumw2+sumw*sumw);
  }else{
    //Normal case
    nraw = sumw*sumw/sumw2;
    weight = sumw2/sumw;
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
      + other_raw.at(i)*other_wght.at(i)
      + inject_signal*sig_raw.at(i)*sig_wght.at(i);
  }
}

void GetDileptonBinMapping(size_t &nr4, vector<size_t> &r4_map){
  switch(method){
  case 0:
  case 1:
  case 2:
    nr4 = 4;
    r4_map.push_back(0);
    r4_map.push_back(1);
    r4_map.push_back(0);
    r4_map.push_back(1);
    break;
  case 3:
    nr4 = 6;
    r4_map.push_back(0);
    r4_map.push_back(0);
    r4_map.push_back(1);
    r4_map.push_back(1);
    r4_map.push_back(0);
    r4_map.push_back(1);
    break;
  default:
    break;
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

double sqr(double x){
  return x*x;
}

void WriteFile(const vector<double> &ttbar_raw, const vector<double> &ttbar_wght,
               const vector<double> &other_raw, const vector<double> &other_wght,
               const vector<double> &sig_raw, const vector<double> &sig_wght,
               const vector<double> &mc_raw, const vector<double> &mc_wght,
               const vector<double> &data_counts, const vector<double> &dilep_count){
  size_t nr1, nr2, nr3, nr4;
  vector<size_t> r1_map, r2_map, r3_map, r4_map;
  GetBinMapping(nr1, r1_map, nr2, r2_map, nr3, r3_map, nr4, r4_map);

  ostringstream file_name;
  file_name << "txt/data_card";
  if(set_ntuple_date) file_name << '_' << ntuple_date;
  if(set_method) file_name << "_method_" << method;
  if(set_mc_multiplier) file_name << "_mc_multplier_" << NoDecimal(mc_multiplier);
  if(no_mc_kappa) file_name << "_no_mc_kappa";
  if(no_dilep) file_name << "_no_dilep";
  if(no_systematics) file_name << "_no_systematics";
  if(set_masses) file_name << "_T1tttt_" << mgluino << '_' << mlsp;
  if(set_lumi) file_name << "_lumi_" << lumi;
  if(set_inject_signal) file_name << "_inject_" << NoDecimal(inject_signal);
  if(do_tk_veto) file_name << "_tk_veto";
  if(set_ht) file_name << "_ht_" << ht_min;
  if(set_mt) file_name << "_mt_" << mt_min << '_' << mt_div;
  if(set_mj) file_name << "_mj_" << mj_min << '_' << mj_div;
  if(set_njets) file_name << "_njets_" << njets_min << '_' << njets_div;
  if(set_met) file_name << "_met_" << met_min << '_' << met_div;
  if(set_nb) file_name << "_nb_" << nb_min << '_' << nb_div;
  file_name << ".txt" << flush;

  ofstream file(file_name.str().c_str());
  file << "imax " << nr4 << "   number of channels\n";
  file << "jmax 2   number of backgrounds\n";
  file << "kmax *   number of nuisance parameters\n";
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

  if(!no_dilep){
    PrintDilepton(file, dilep_count);
  }
  if(!no_systematics){
    PrintSystematics(file);
  }

  file << "#kappa                    ";
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    size_t ir1 = r1_map.at(ir4);
    size_t ir2 = r2_map.at(ir4)+nr1;
    size_t ir3 = r3_map.at(ir4)+nr1+nr2;
    size_t iir4 = r4_map.at(ir4)+nr1+nr2+nr3;

    double kappa = mc_raw.at(ir1)*mc_wght.at(ir1);
    kappa /= mc_raw.at(ir2)*mc_wght.at(ir2);
    kappa /= mc_raw.at(ir3)*mc_wght.at(ir3);
    kappa *= mc_raw.at(iir4)*mc_wght.at(iir4);

    file
      << ' ' << setw(12) << '-'
      << ' ' << setw(12) << kappa
      << ' ' << setw(12) << kappa;
  }

  file << endl;
  file.close();

  cout << endl << "Created data card at " << file_name.str() << endl << endl;
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
        double val = 1.+1./sqrt(raw_counts.at(iri+offset)+1.);
        file << "            - " << setw(12) << val << ' ' << setw(12) << val;
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

string NoDecimal(double x){
  ostringstream oss;
  oss.precision(2);
  oss << fixed << x;
  string s = oss.str();
  size_t l = s.find('.');
  while(l<s.size()){
    s.at(l)='_';
    l = s.find('.');
  }
  return s;
}

void PrintDilepton(ofstream &file, const vector<double> &dilep_count){
  size_t nr4;
  vector<size_t> bin_map;
  GetDileptonBinMapping(nr4, bin_map);
  file << "dilepton  lnN             ";
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    double val = 1.+1./sqrt(dilep_count.at(bin_map.at(ir4))+1);
    file << "            - "
         << setw(12) << val
         << ' ' << setw(12) << val;
  }
  file << endl;
}

void PrintSystematics(ofstream &file){
  switch(method){
  case 0:
  case 1:
    file << "n_isr     lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.02;
    file << ' ' << setw(12) << 1.02;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.00;
    file << ' ' << setw(12) << 1.00;
    file << endl;
    file << "isr_pt    lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.10;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.70;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.00;
    file << ' ' << setw(12) << '-';
    file << endl;
    file << "top_pt    lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.08;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.00;
    file << ' ' << setw(12) << '-';
    file << endl;
    file << "high_mt   lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.07;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.16;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.08;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.00;
    file << ' ' << setw(12) << '-';
    file << endl;
    break;
  case 2:
    file << "n_isr     lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.02;
    file << ' ' << setw(12) << 1.02;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.04;
    file << ' ' << setw(12) << 1.04;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.04;
    file << ' ' << setw(12) << 1.04;
    file << endl;
    file << "isr_pt    lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.05;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.21;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.76;
    file << ' ' << setw(12) << '-';
    file << endl;
    file << "top_pt    lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.02;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.10;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.23;
    file << ' ' << setw(12) << '-';
    file << endl;
    file << "high_mt   lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.09;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.17;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.20;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 2.00;
    file << ' ' << setw(12) << '-';
    file << endl;
    break;
  case 3:
    file << "n_isr     lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.02;
    file << ' ' << setw(12) << 1.02;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.02;
    file << ' ' << setw(12) << 1.02;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.04;
    file << ' ' << setw(12) << 1.04;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.04;
    file << ' ' << setw(12) << 1.04;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.04;
    file << ' ' << setw(12) << 1.04;
    file << endl;
    file << "isr_pt    lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.05;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.05;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.21;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.76;
    file << ' ' << setw(12) << '-';
    file << endl;
    file << "top_pt    lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.01;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.02;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.02;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.10;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.23;
    file << ' ' << setw(12) << '-';
    file << endl;
    file << "high_mt   lnN             ";
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.09;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.09;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.17;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.17;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 1.20;
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << '-';
    file << ' ' << setw(12) << 2.00;
    file << ' ' << setw(12) << '-';
    file << endl;
    break;
  default:
    break;
  }
}
