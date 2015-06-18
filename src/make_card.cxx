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
#include "TError.h"
#include "TSystem.h"

#include "gamma_params.hpp"
#include "small_tree_quick.hpp"
#include "timer.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"

namespace{
  //Options that can all be set with command line args
  int method = 0;
  std::string ntuple_date = "2015_06_05";
  double lumi = 10.;
  double mc_multiplier = 1.;
  bool no_mc_kappa = false;
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

  bool verbose = false;

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
  gErrorIgnoreLevel=6000;
  GetOptions(argc, argv);

  string folder="archive/"+ntuple_date+"/skim/";

  small_tree_quick ttbar(folder+"*TTJets*");
  small_tree_quick pos(folder+"*TTWJets*");
  pos.Add(folder+"*TTZJets*");
  pos.Add(folder+"*QCD_Pt*");
  pos.Add(folder+"*WJetsToLNu_HT*");
  pos.Add(folder+"*ZJetsToLNu_HT*");
  pos.Add(folder+"*DYJets*");
  pos.Add(folder+"*H_HToBB*");
  small_tree_quick neg(folder+"*_T*s-channel*");
  neg.Add(folder+"_T*tW-channel*");
  neg.Add(folder+"_T*t-channel*");
  small_tree_quick sig(folder+"*T1tttt*"+mgluino+"*"+mlsp+"*");

  vector<GammaParams> ttbar_gp, pos_gp, neg_gp, sig_gp;
  GetCounts(ttbar, ttbar_gp);
  GetCounts(pos, pos_gp);
  GetCounts(neg, neg_gp);
  GetCounts(sig, sig_gp);

  //Force possibly negative component to be positive and ignore uncertainty, then lump together with "pos"
  vector<GammaParams> other_gp(pos_gp.size());
  for(size_t bin = 0; bin < other_gp.size(); ++bin){
    other_gp.at(bin).SetYieldAndUncertainty(pos_gp.at(bin).Yield()+max(0.,neg_gp.at(bin).Yield()), pos_gp.at(bin).Uncertainty());
  }

  vector<string> bkg_names;
  vector<vector<GammaParams> > bkg_gps;
  bkg_gps.push_back(ttbar_gp); bkg_names.push_back("ttbar");
  bkg_gps.push_back(other_gp); bkg_names.push_back("other");

  vector<GammaParams> mc_gp;
  GetMCTotals(mc_gp, bkg_gps);

  vector<double> data_counts;
  MockUpData(data_counts, sig_gp, bkg_gps);

  WriteFile(bkg_gps, bkg_names, sig_gp, mc_gp, data_counts);

  if(verbose){
    PrintDebug(sig_gp, "signal");
    for(size_t ibkg = 0; ibkg < bkg_gps.size(); ++ibkg){
      PrintDebug(bkg_gps.at(ibkg), bkg_names.at(ibkg));
    }
  }
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
      {"verbose", no_argument, 0, 'v'},
      {0, 0, 0, 0}
    };

    char opt = -1;
    int option_index;
    opt = getopt_long(argc, argv, "m:d:l:s:tv", long_options, &option_index);
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
    case 'v':
      verbose = true;
      break;
    case 0:
      optname = long_options[option_index].name;
      if(optname == "mc_multiplier"){
	mc_multiplier = atof(optarg);
	set_mc_multiplier = true;
      }else if(optname == "no_mc_kappa"){
        no_mc_kappa = true;
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

void GetCounts(small_tree_quick &tree, vector<GammaParams> &gp){
  size_t nbins = 0;
  switch(method){
  case 0: nbins = 8; break;
  case 1: nbins = 16; break;
  case 2: nbins = 12; break;
  case 3: nbins = 18; break;
  default: break;
  }

  vector<double> counts(nbins, 0.);
  vector<double> squares(nbins, 0.);

  double sumw = 0., sumw2 = 0.;
  int num_entries = tree.GetEntries();
  Timer timer(num_entries, 1.);
  timer.Start();
  for(int entry = 0; entry < num_entries; ++entry){
    tree.GetEntry(entry);
    timer.Iterate();

    double weight = lumi*tree.weight();

    if(tree.nbm()<nb_min
       || tree.njets()<njets_min
       || (tree.nmus()+tree.nels())!=1
       || tree.met()<=met_min
       || (do_tk_veto && tree.ntks_chg_mini()>0)
       || tree.ht()<=ht_min){
      continue;
    }
    size_t bin = LookUpBin(tree);

    counts.at(bin) += weight;
    squares.at(bin) += weight*weight;

    sumw += weight;
    sumw2 += weight*weight;
  }
  sumw2 /= mc_multiplier;

  gp.resize(counts.size());
  for(size_t bin = 0; bin < counts.size(); ++bin){
    double raw, weight;
    squares.at(bin) /= mc_multiplier;
    CountsToGammas(counts.at(bin), squares.at(bin),
                   sumw, sumw2,
                   raw, weight);
    gp.at(bin).SetNEffectiveAndWeight(raw, weight);
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

void GetMCTotals(vector<GammaParams> &mc_gp,
		 const vector<vector<GammaParams> > &bkg_gps){
  if(bkg_gps.size() == 0){
    mc_gp.clear();
    return;
  }

  mc_gp = vector<GammaParams>(bkg_gps.at(0).size());
  for(size_t sample = 0; sample < bkg_gps.size(); ++sample){
    for(size_t bin = 0; bin < bkg_gps.at(sample).size(); ++bin){
      mc_gp.at(bin) += bkg_gps.at(sample).at(bin);
    }
  }
}

void MockUpData(vector<double> &data,
		const vector<GammaParams> &sig_gp,
		const vector<vector<GammaParams> > &bkg_gps){
  data.resize(sig_gp.size());
  for(size_t bin = 0; bin < data.size(); ++bin){
    data.at(bin) = inject_signal*sig_gp.at(bin).Yield();
    for(size_t sample = 0; sample < bkg_gps.size(); ++sample){
      data.at(bin) += bkg_gps.at(sample).at(bin).Yield();
    }
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

void WriteFile(const vector<vector<GammaParams> > &bkg_gps,
	       const vector<string> &bkg_names,
	       const vector<GammaParams> &sig_gp,
	       const vector<GammaParams> &mc_gp,
	       const vector<double> &data_counts){
  size_t nr1, nr2, nr3, nr4;
  vector<size_t> r1_map, r2_map, r3_map, r4_map;
  GetBinMapping(nr1, r1_map, nr2, r2_map, nr3, r3_map, nr4, r4_map);

  ostringstream file_name;
  file_name << "txt/data_card";
  if(set_ntuple_date) file_name << '_' << ntuple_date;
  if(set_method) file_name << "_method_" << method;
  if(set_mc_multiplier) file_name << "_mc_multplier_" << NoDecimal(mc_multiplier);
  if(no_mc_kappa) file_name << "_no_mc_kappa";
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
  file << "jmax " << bkg_gps.size() << "   number of backgrounds\n";
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
  file << "bin                          ";
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    for(size_t i = 0; i < 3; ++i){
      file << ' ' << setw(12) << (ir4+1);
    }
  }
  file << '\n';
  file << "process                      ";
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    file << ' ' << setw(12) << "sig";
    for(size_t isam = 0; isam < bkg_names.size(); ++isam){
      file  << ' ' << setw(12) << bkg_names.at(isam);
    }
  }
  file << '\n';
  file << "process                      ";
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    file << "            0";
    for(size_t isam = 0; isam < bkg_names.size(); ++isam){
      file << ' ' << setw(12) << (isam+1);
    }
  }
  file << '\n';
  file << "rate                         ";
  vector<double> sig_pred(0);
  vector<vector<double> > bkg_preds(bkg_gps.size());
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    size_t ir1 = r1_map.at(ir4);
    size_t ir2 = r2_map.at(ir4)+nr1;
    size_t ir3 = r3_map.at(ir4)+nr1+nr2;
    size_t iir4 = r4_map.at(ir4)+nr1+nr2+nr3;
    sig_pred.push_back(sig_gp.at(iir4).Yield());
    file << ' ' << setw(12) << sig_pred.back();
    for(size_t isam = 0; isam < bkg_gps.size(); ++isam){
      bkg_preds.at(isam).push_back(GetPred(data_counts, mc_gp, bkg_gps.at(isam), ir1, ir2, ir3, iir4));
      file << ' ' << setw(12) << bkg_preds.at(isam).back();
    }
  }
  file << '\n';
  file << "------------\n";

  //Print control region uncertainty from data
  GammaToLogN13(file, r1_map, nr1, nr2, nr4, data_counts, bkg_gps.size());
  GammaToLogN2(file, r2_map, nr1, nr2, nr4, data_counts, bkg_gps.size());

  vector<double> mc_counts(mc_gp.size());
  for(size_t i = 0; i < mc_gp.size(); ++i){
    mc_counts.at(i) = mc_gp.at(i).NEffective();
  }
  if(!no_mc_kappa){
    GammaToLogN13(file, r1_map, nr1, nr2, nr4, bkg_gps);
    GammaToLogN2(file, r2_map, nr1, nr2, nr4, bkg_gps);
  }

  PrintGamma(file, r4_map, "sgnl_r4", 4, nr1, nr2, nr3, nr4, sig_gp, sig_pred, 0, bkg_gps.size());
  for(size_t isam = 0; isam < bkg_preds.size(); ++isam){
    PrintGamma(file, r4_map, bkg_names.at(isam)+"_r4",4, nr1, nr2, nr3, nr4, bkg_gps.at(isam), bkg_preds.at(isam), isam+1, bkg_gps.size());
  }

  if(!no_systematics){
    PrintSystematics(file, bkg_gps.size());
  }

  file << "#kappa                       ";
  for(size_t ir4 = 0; ir4 < nr4; ++ir4){
    size_t ir1 = r1_map.at(ir4);
    size_t ir2 = r2_map.at(ir4)+nr1;
    size_t ir3 = r3_map.at(ir4)+nr1+nr2;
    size_t iir4 = r4_map.at(ir4)+nr1+nr2+nr3;

    double kappa = mc_gp.at(ir1).Yield();
    kappa /= mc_gp.at(ir2).Yield();
    kappa /= mc_gp.at(ir3).Yield();
    kappa *= mc_gp.at(iir4).Yield();

    file << ' ' << setw(12) << '-';
    for(size_t isam = 0; isam < bkg_gps.size(); ++isam){
      file << ' ' << setw(12) << kappa;
    }
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
               const vector<GammaParams> &mc_gp,
               const vector<GammaParams> &proc_gp,
               size_t ir1,
               size_t ir2,
               size_t ir3,
               size_t ir4){
  //Full data prediction for bin
  double pred = data.at(ir2)*data.at(ir3)/data.at(ir1);

  //Fraction belonging to this process
  pred *= (proc_gp.at(ir4).Yield())/mc_gp.at(ir4).Yield();

  //MC kappa correction
  if(!no_mc_kappa){
    pred *= mc_gp.at(ir1).Yield();
    pred /= mc_gp.at(ir2).Yield();
    pred /= mc_gp.at(ir3).Yield();
    pred *= mc_gp.at(ir4).Yield();
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
      pred *= proc_gp.at(ir4).Yield();
      pred /= proc_gp.at(id1).Yield()+proc_gp.at(id2).Yield();
    }
  }

  return max(0.,pred);
}

void PrintGamma(ofstream &file, const vector<size_t> map,
                const string &name, size_t iregion,
                size_t nr1, size_t nr2, size_t nr3, size_t nr4,
		const vector<GammaParams> &gps,
                const vector<double> &preds,
                size_t iproc, size_t nbkgs){
  size_t nri = -1; size_t offset = 0;
  switch(iregion){
  case 1: nri = nr1; offset = 0; break;
  case 2: nri = nr2; offset = nr1; break;
  case 3: nri = nr3; offset = nr1+nr2; break;
  case 4: nri = nr4; offset = nr1+nr2+nr3; break;
  default: break;
  }
  for(size_t iri = 0; iri < nri; ++iri){
    int gmn_raw;
    double gmn_wght;
    GetGammaParameters(gmn_raw, gmn_wght, gps.at(iri+offset).NEffective(), gps.at(iri+offset).Weight(), preds.at(iri));
    file << Expand(name+'_'+ToString(iri+1),12) << " gmN " << setw(12) << gmn_raw;
    for(size_t ir4 = 0; ir4 < nr4; ++ir4){
      if(map.at(ir4) == iri){
        for(size_t i = 0; i < (nbkgs+1); ++i){
          if(i == iproc){
            file << ' ' << setw(12) << gmn_wght;
          }else{
            file << ' ' << setw(12) << '-';
          }
        }
      }else{
	for(size_t isam = 0; isam < (nbkgs+1); ++isam){
	  file << ' ' << setw(12) << '-';
	}
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

void PrintSystematics(ofstream &file, size_t nbkgs){
  double lf = 1./sqrt(lumi/10.);
  double mf = 1./sqrt(mc_multiplier);
  switch(method){
  case 0:
  case 1:
    file << "n_isr        lnN             ";
    RepLogN(file, 4, nbkgs);
    RepLogN(file, 4, nbkgs);
    RepLogN(file, 9, nbkgs);
    RepLogN(file, 14, nbkgs);
    file << endl;
    file << "isr_pt       lnN             ";
    RepLogN(file, 4, nbkgs);
    RepLogN(file, 5, nbkgs);
    RepLogN(file, 59, nbkgs);
    RepLogN(file, 19, nbkgs);
    file << endl;
    file << "top_pt       lnN             ";
    RepLogN(file, 1, nbkgs);
    RepLogN(file, 1, nbkgs);
    RepLogN(file, 8, nbkgs);
    RepLogN(file, 0, nbkgs);
    file << endl;
    file << "high_mt      lnN             ";
    RepLogN(file, 7, nbkgs);
    RepLogN(file, 16, nbkgs);
    RepLogN(file, 2, nbkgs);
    RepLogN(file, 23, nbkgs);
    file << endl;
    file << "dilep_data   lnN             ";
    RepAsymLogN(file, 58.4*lf, 30.8*lf, nbkgs);
    RepAsymLogN(file, 94.0*lf, 72.2*lf, nbkgs);
    RepAsymLogN(file, 58.4*lf, 30.8*lf, nbkgs);
    RepAsymLogN(file, 94.0*lf, 72.2*lf, nbkgs);
    file << endl;
    file << "dilep_mc     lnN             ";
    RepAsymLogN(file, 16.2*mf, 16.7*mf, nbkgs);
    RepAsymLogN(file, 30.3*mf, 31.8*mf, nbkgs);
    RepAsymLogN(file, 16.2*mf, 16.7*mf, nbkgs);
    RepAsymLogN(file, 30.3*mf, 31.8*mf, nbkgs);
    file << endl;
    break;
  case 2:
    file << "n_isr        lnN             ";
    RepLogN(file, 2, nbkgs);
    RepLogN(file, 6, nbkgs);
    RepLogN(file, 4, nbkgs);
    RepLogN(file, 19, nbkgs);
    file << endl;
    file << "isr_pt       lnN             ";
    RepLogN(file, 4, nbkgs);
    RepLogN(file, 1, nbkgs);
    RepLogN(file, 25, nbkgs);
    RepLogN(file, 42, nbkgs);
    file << endl;
    file << "top_pt       lnN             ";
    RepLogN(file, 1, nbkgs);
    RepLogN(file, 2, nbkgs);
    RepLogN(file, 10, nbkgs);
    RepLogN(file, 23, nbkgs);
    file << endl;
    file << "high_mt      lnN             ";
    RepLogN(file, 9, nbkgs);
    RepLogN(file, 17, nbkgs);
    RepLogN(file, 18, nbkgs);
    RepLogN(file, 20, nbkgs);
    file << endl;
    file << "dilep_data   lnN             ";
    RepAsymLogN(file, 15.5*lf, 16.3*lf, nbkgs);
    RepAsymLogN(file, 37.6*lf, 28.0*lf, nbkgs);
    RepAsymLogN(file, 15.5*lf, 16.3*lf, nbkgs);
    RepAsymLogN(file, 37.6*lf, 28.0*lf, nbkgs);
    file << endl;
    file << "dilep_mc     lnN             ";
    RepAsymLogN(file, 8.2*mf, 8.5*mf, nbkgs);
    RepAsymLogN(file, 15.2*mf, 15.6*mf, nbkgs);
    RepAsymLogN(file, 8.2*mf, 8.5*mf, nbkgs);
    RepAsymLogN(file, 15.2*mf, 15.6*mf, nbkgs);
    file << endl;
    break;
  case 3:
    file << "n_isr        lnN             ";
    RepLogN(file, 2, nbkgs);
    RepLogN(file, 2, nbkgs);
    RepLogN(file, 6, nbkgs);
    RepLogN(file, 6, nbkgs);
    RepLogN(file, 4, nbkgs);
    RepLogN(file, 19, nbkgs);
    file << endl;
    file << "isr_pt       lnN             ";
    RepLogN(file, 4, nbkgs);
    RepLogN(file, 4, nbkgs);
    RepLogN(file, 1, nbkgs);
    RepLogN(file, 1, nbkgs);
    RepLogN(file, 25, nbkgs);
    RepLogN(file, 42, nbkgs);
    file << endl;
    file << "top_pt       lnN             ";
    RepLogN(file, 1, nbkgs);
    RepLogN(file, 1, nbkgs);
    RepLogN(file, 2, nbkgs);
    RepLogN(file, 2, nbkgs);
    RepLogN(file, 10, nbkgs);
    RepLogN(file, 23, nbkgs);
    file << endl;
    file << "high_mt      lnN             ";
    RepLogN(file, 9, nbkgs);
    RepLogN(file, 9, nbkgs);
    RepLogN(file, 17, nbkgs);
    RepLogN(file, 17, nbkgs);
    RepLogN(file, 18, nbkgs);
    RepLogN(file, 20, nbkgs);
    file << endl;
    file << "dilep_data   lnN             ";
    RepAsymLogN(file, 15.5*lf, 16.3*lf, nbkgs);
    RepAsymLogN(file, 15.5*lf, 16.3*lf, nbkgs);
    RepAsymLogN(file, 37.6*lf, 28.0*lf, nbkgs);
    RepAsymLogN(file, 37.6*lf, 28.0*lf, nbkgs);
    RepAsymLogN(file, 15.5*lf, 16.3*lf, nbkgs);
    RepAsymLogN(file, 37.6*lf, 28.0*lf, nbkgs);
    file << endl;
    file << "dilep_mc     lnN             ";
    RepAsymLogN(file, 8.2*mf, 8.5*mf, nbkgs);
    RepAsymLogN(file, 8.2*mf, 8.5*mf, nbkgs);
    RepAsymLogN(file, 15.2*mf, 15.6*mf, nbkgs);
    RepAsymLogN(file, 15.2*mf, 15.6*mf, nbkgs);
    RepAsymLogN(file, 8.2*mf, 8.5*mf, nbkgs);
    RepAsymLogN(file, 15.2*mf, 15.6*mf, nbkgs);
    file << endl;
    break;
  default:
    break;
  }
}

void RepLogN(ofstream &file, double val, size_t nbkgs){
  file << ' ' << setw(12) << '-';
  for(size_t ibkg = 0; ibkg < nbkgs; ++ibkg){
    file << ' ' << setw(12) << (1.+val/100.);
  }
}

void RepAsymLogN(ofstream &file, double minus, double plus, size_t nbkgs){
  file << ' ' << setw(12) << '-';
  for(size_t ibkg = 0; ibkg < nbkgs; ++ibkg){
    file << ' ' << setw(12) << (ToString(1./(1.+minus/100.))+'/'+ToString(1.+plus/100.));
  }
}

string Expand(string in, size_t size){
  while(in.size() < size){
    in += ' ';
  }
  return in;
}

void GammaToLogN13(ofstream &file, const vector<size_t> &map,
		 size_t nr1, size_t nr2, size_t nr4,
		 const vector<double> &counts, size_t nbkgs){
  for(size_t icr = 0; icr < nr1; ++icr){
    file << Expand(string("data_31_")+ToString(icr+1),12) << " lnN             ";
    vector<vector<float> > entries(2,vector<float>(1)), weights(2,vector<float>(1,1.));
    vector<float> powers(2);
    entries.at(0).at(0) = counts.at(icr);
    powers.at(0) = -1.;
    entries.at(1).at(0) = counts.at(icr+nr1+nr2);
    powers.at(1) = 1.;

    float minus, plus;

    double center = calcKappa(entries, weights, powers, minus, plus, false, verbose);
    minus = 1./(1.+minus/center);
    plus = 1.+plus/center;
    for(size_t ir4 = 0; ir4 < nr4; ++ir4){
      if(map.at(ir4) == icr){
	file << "            -";
	for(size_t ibkg = 0; ibkg < nbkgs; ++ibkg){
	  file << ' ' << minus << '/' << plus;
	}
      }else{
	for(size_t ibkg = 0; ibkg < (nbkgs+1); ++ibkg){
	  file << ' ' << setw(12) << '-';
	}
      }
    }
    file << '\n';
  }
}

void GammaToLogN2(ofstream &file, const vector<size_t> &map,
		  size_t nr1, size_t nr2, size_t nr4,
		 const vector<double> &counts, size_t nbkgs){
  for(size_t icr = 0; icr < nr2; ++icr){
    file << Expand(string("data_r2_")+ToString(icr+1),12) << " lnN             ";
    vector<vector<float> > entries(1,vector<float>(1)), weights(1,vector<float>(1,1.));
    entries.at(0).at(0) = counts.at(icr+nr1);
    vector<float> powers(1,1.);

    float minus, plus;

    double center = calcKappa(entries, weights, powers, minus, plus, false, verbose);
    minus = 1./(1.+minus/center);
    plus = 1.+plus/center;
    for(size_t ir4 = 0; ir4 < nr4; ++ir4){
      if(map.at(ir4) == icr){
	file << "            -";
	for(size_t ibkg = 0; ibkg < nbkgs; ++ibkg){
	  file << ' ' << minus << '/' << plus;
	}
      }else{
	for(size_t ibkg = 0; ibkg < (nbkgs+1); ++ibkg){
	  file << ' ' << setw(12) << '-';
	}
      }
    }
    file << '\n';
  }
}

void GammaToLogN13(ofstream &file, const vector<size_t> &map,
		   size_t nr1, size_t nr2, size_t nr4,
		   const vector<vector<GammaParams> > &gps){
  for(size_t icr = 0; icr < nr1; ++icr){
    file << Expand(string("mc_13_")+ToString(icr+1),12) << " lnN             ";
    vector<vector<float> > entries(2,vector<float>(gps.size())), weights(2,vector<float>(gps.size()));
    for(size_t isample = 0; isample < gps.size(); ++isample){
      entries.at(0).at(isample) = gps.at(isample).at(icr).NEffective();
      entries.at(1).at(isample) = gps.at(isample).at(icr+nr1+nr2).NEffective();
      weights.at(0).at(isample) = gps.at(isample).at(icr).Weight();
      weights.at(1).at(isample) = gps.at(isample).at(icr+nr1+nr2).Weight();
    }
    vector<float> powers(2);
    powers.at(0) = 1.;
    powers.at(1) = -1.;

    float minus, plus;

    double center = calcKappa(entries, weights, powers, minus, plus, false, verbose);
    minus = 1./(1.+minus/center);
    plus = 1.+plus/center;
    for(size_t ir4 = 0; ir4 < nr4; ++ir4){
      if(map.at(ir4) == icr){
	file << "            -";
	for(size_t ibkg = 0; ibkg < gps.size(); ++ibkg){
	  file << ' ' << minus << '/' << plus;
	}
      }else{
	for(size_t ibkg = 0; ibkg < (gps.size()+1); ++ibkg){
	  file << ' ' << setw(12) << '-';
	}
      }
    }
    file << '\n';
  }
}

void GammaToLogN2(ofstream &file, const vector<size_t> &map,
		  size_t nr1, size_t nr2, size_t nr4,
		  const vector<vector<GammaParams> > &gps){
  for(size_t icr = 0; icr < nr2; ++icr){
    file << Expand(string("mc_r2_")+ToString(icr+1),12) << " lnN             ";
    vector<vector<float> > entries(1,vector<float>(gps.size())), weights(1,vector<float>(gps.size()));
    for(size_t isample = 0; isample < gps.size(); ++isample){
      entries.at(0).at(isample) = gps.at(isample).at(icr+nr1).NEffective();
      weights.at(0).at(isample) = gps.at(isample).at(icr+nr1).Weight();
    }
    vector<float> powers(1,-1.);

    float minus, plus;

    double center = calcKappa(entries, weights, powers, minus, plus, false, verbose);
    minus = 1./(1.+minus/center);
    plus = 1.+plus/center;
    for(size_t ir4 = 0; ir4 < nr4; ++ir4){
      if(map.at(ir4) == icr){
	file << "            -";
	for(size_t ibkg = 0; ibkg < gps.size(); ++ibkg){
	  file << ' ' << minus << '/' << plus;
	}
      }else{
	for(size_t ibkg = 0; ibkg < (gps.size()+1); ++ibkg){
	  file << ' ' << setw(12) << '-';
	}
      }
    }
    file << '\n';
  }
}

void PrintDebug(const vector<GammaParams> &gps, const string &name){
  cout
    << ' ' << setw(16) << "Process"
    << ' ' << setw(16) << "Bin"
    << ' ' << setw(16) << "Yield"
    << ' ' << setw(16) << "Uncertainty"
    << ' ' << setw(16) << "N. Effective"
    << ' ' << setw(16) << "Weight"
    << endl;
  for(size_t ibin = 0; ibin < gps.size(); ++ibin){
    printf(" %16s", name.c_str());
    printf(" %16d", static_cast<int>(ibin+1));
    printf(" %16.2f", gps.at(ibin).Yield());
    printf(" %16.2f", gps.at(ibin).Uncertainty());
    printf(" %16.2f", gps.at(ibin).NEffective());
    printf(" %16.6f", gps.at(ibin).Weight());
    printf("\n");
    fflush(0);
  }
}
