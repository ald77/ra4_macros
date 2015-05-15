#ifndef H_MAKE_CARD
#define H_MAKE_CARD

#include <string>
#include <vector>

#include "small_tree_quick.hpp"

void GetCounts(double lumi,
               small_tree_quick &tree,
               std::vector<double> &raw,
               std::vector<double> &wght);

size_t LookUpBin(small_tree_quick &tree);

void GetMCTotals(std::vector<double> &mc_raw, std::vector<double> &mc_wght,
                 const std::vector<double> &ttbar_raw, const std::vector<double> &ttbar_wght,
                 const std::vector<double> &other_raw, const std::vector<double> &other_wght);

void MockUpData(std::vector<double> &data,
                const std::vector<double> &ttbar_raw, const std::vector<double> &ttbar_wght,
                const std::vector<double> &other_raw, const std::vector<double> & other_wght,
                const std::vector<double> &sig_raw, const std::vector<double> &sig_wght,
                bool use_sig);

void GetKappas(const std::vector<double> &mc_raw, const std::vector<double> &mc_wght,
               const std::vector<double> &data_counts,
               std::vector<double> &kappas, std::vector<double> &kappa_uncerts);

void GetKappa(std::vector<double> &kappas, std::vector<double> &kappa_uncerts,
              size_t ikappa, size_t ilowlow, size_t ilowhigh, size_t ihighlow,
              const std::vector<double> &mc_raw, const std::vector<double> &mc_wght,
              const std::vector<double> &data_counts);

double sqr(double x);

void WriteFile(const std::vector<double> &ttbar_raw, const std::vector<double> &ttbar_wght,
               const std::vector<double> &other_raw, const std::vector<double> &other_wght,
               const std::vector<double> &sig_raw, const std::vector<double> &sig_wght,
               const std::vector<double> &kappas, const std::vector<double> &kappa_uncerts,
               const std::vector<double> &data_counts, const std::string &lumi_string, bool compressed);

std::string GetBinName(size_t i);

void GetGammaParameters(int &raw, double &weight,
                        double sumw, double sumw2);

void  WriteDebug(const std::vector<size_t> &bins, const std::vector<size_t> &ikappas,
                 const std::vector<double> &ttbar_raw, const std::vector<double> &ttbar_wght,
                 const std::vector<double> &other_raw, const std::vector<double> &other_wght,
                 const std::vector<double> &sig_raw, const std::vector<double> &sig_wght,
                 const std::vector<double> &kappas, const std::vector<double> &kappa_uncerts,
                 const std::vector<double> &data_counts);

#endif
