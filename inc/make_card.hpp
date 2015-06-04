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
                const std::vector<double> &sig_raw, const std::vector<double> &sig_wght);

double sqr(double x);

void WriteFile(const std::vector<double> &ttbar_raw, const std::vector<double> &ttbar_wght,
               const std::vector<double> &other_raw, const std::vector<double> &other_wght,
               const std::vector<double> &sig_raw, const std::vector<double> &sig_wght,
               const std::vector<double> &mc_raw, const std::vector<double> &mc_wght,
               const std::vector<double> &data_counts);

void GetGammaParameters(int &raw, double &weight,
                        double sumw, double sumw2);

void GetBinMapping(size_t &nr1, std::vector<size_t> &r1_map,
                   size_t &nr2, std::vector<size_t> &r2_map,
                   size_t &nr3, std::vector<size_t> &r3_map,
                   size_t &nr4, std::vector<size_t> &r4_map);

void GetOptions(int argc, char *argv[]);

double GetPred(const std::vector<double> &data,
               const std::vector<double> &mc_raw,
               const std::vector<double> &mc_wght,
               const std::vector<double> &proc_raw,
               const std::vector<double> &proc_wght,
               size_t ir1,
               size_t ir2,
               size_t ir3,
               size_t ir4);

void PrintLogN(std::ofstream &file, const std::vector<size_t> map,
               const std::string &name, size_t ibin,
               size_t nr1, size_t nr2, size_t nr3, size_t nr4,
               const std::vector<double> &raw_counts);

void PrintGamma(std::ofstream &file, const std::vector<size_t> map,
                const std::string &name, size_t ibin,
                size_t nr1, size_t nr2, size_t nr3, size_t nr4,
                const std::vector<double> &raw_counts,
                const std::vector<double> &weights,
                const std::vector<double> &preds,
                size_t iproc);

std::string NoDecimal(double x);

#endif
