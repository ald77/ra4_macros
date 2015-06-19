#ifndef H_MAKE_CARD
#define H_MAKE_CARD

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "gamma_params.hpp"
#include "small_tree_quick.hpp"

void GetCounts(small_tree_quick &tree, std::vector<GammaParams> &gp);

void CountsToGammas(double sumw, double sumw2,
                    double sumw_backup, double sumw2_backup,
                    double &nraw, double &weight);

size_t LookUpBin(small_tree_quick &tree);

void GetMCTotals(std::vector<GammaParams> &mc_gp,
                 const std::vector<std::vector<GammaParams> > &bkg_gps);

void MockUpData(std::vector<double> &data,
                const std::vector<GammaParams> &sig_gp,
                const std::vector<std::vector<GammaParams> > &bkg_gps);

double sqr(double x);

void WriteFile(const std::vector<std::vector<GammaParams> > &bkg_gps,
               const std::vector<std::string> &bkg_names,
               const std::vector<GammaParams> &sig_gp,
               const std::vector<GammaParams> &mc_gp,
               const std::vector<double> &data_counts);

void GetGammaParameters(int &raw, double &weight,
                        double sumw, double sumw2);

void GetBinMapping(size_t &nr1, std::vector<size_t> &r1_map,
                   size_t &nr2, std::vector<size_t> &r2_map,
                   size_t &nr3, std::vector<size_t> &r3_map,
                   size_t &nr4, std::vector<size_t> &r4_map);

void GetOptions(int argc, char *argv[]);

double GetPred(const std::vector<double> &data,
               const std::vector<GammaParams> &mc_gp,
               const std::vector<GammaParams> &proc_gp,
               size_t ir1,
               size_t ir2,
               size_t ir3,
               size_t ir4);

void PrintGamma(std::ofstream &file, const std::vector<size_t> map,
                const std::string &name, size_t iregion,
                size_t nr1, size_t nr2, size_t nr3, size_t nr4,
                const std::vector<GammaParams> &gp,
                const std::vector<double> &preds,
                size_t iproc, size_t nbkgs);

std::string NoDecimal(double x);

void PrintSystematics(std::ofstream &file, size_t bkgs);

void RepLogN(std::ofstream &file, double val, size_t nbkgs);

void RepAsymLogN(std::ofstream &file, double minus, double plus, size_t nbkgs);

std::string Expand(std::string in, size_t size);

template<typename T>
std::string ToString(const T &x){
  std::ostringstream oss;
  oss << x;
  return oss.str();
}

void GammaToLogN13(std::ofstream &file, const std::vector<size_t> &map,
                   size_t nr1, size_t nr2, size_t nr4,
                   const std::vector<double> &counts, size_t nbkgs);

void GammaToLogN2(std::ofstream &file, const std::vector<size_t> &map,
                  size_t nr1, size_t nr2, size_t nr4,
                  const std::vector<double> &counts, size_t nbkgs);

void GammaToLogN13(std::ofstream &file, const std::vector<size_t> &map,
                   size_t nr1, size_t nr2, size_t nr4,
                   const std::vector<std::vector<GammaParams> > &gps);

void GammaToLogN2(std::ofstream &file, const std::vector<size_t> &map,
                  size_t nr1, size_t nr2, size_t nr4,
                  const std::vector<std::vector<GammaParams> > &gps);

void PrintDebug(const std::vector<GammaParams> &gps, const std::string &name);

#endif
