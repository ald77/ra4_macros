#ifndef H_MAKE_CARD
#define H_MAKE_CARD

#include <string>
#include <vector>

#include "small_tree_quick.hpp"

void GetCounts(double lumi,
               small_tree_quick &tree,
               std::vector<double> &counts,
               std::vector<double> &squares);

size_t LookUpBin(small_tree_quick &tree);

void GetMCTotals(std::vector<double> &mc_counts, std::vector<double> &mc_squares,
                 const std::vector<double> &ttbar_counts, const std::vector<double> &ttbar_squares,
                 const std::vector<double> &other_counts, const std::vector<double> &other_squares);

void MockUpData(std::vector<double> &data,
                const std::vector<double> &ttbar,
                const std::vector<double> &other,
                const std::vector<double> &sig,
                bool use_sig);

void GetKappas(const std::vector<double> &mc_counts, const std::vector<double> &mc_squares,
               const std::vector<double> &data_counts,
               std::vector<double> &kappas, std::vector<double> &kappa_uncerts);

void GetKappa(std::vector<double> &kappas, std::vector<double> &kappa_uncerts,
              size_t ikappa, size_t ilowlow, size_t ilowhigh, size_t ihighlow,
              const std::vector<double> &mc_counts, const std::vector<double> &mc_squares,
              const std::vector<double> &data_counts);

double sqr(double x);

void WriteFile(const std::vector<double> &ttbar_counts, const std::vector<double> &ttbar_squares,
               const std::vector<double> &other_counts, const std::vector<double> &other_squares,
               const std::vector<double> &sig_counts, const std::vector<double> &sig_squares,
               const std::vector<double> &kappas, const std::vector<double> &kappa_uncerts,
               const std::vector<double> &data_counts);

std::string GetBinName(size_t i);

void GetGammaParameters(int &raw, double &weight,
                        double sumw, double sumw2);

#endif
