// small_tree: base class to handle reduce tree ntuples
//File generated with generate_small_tree.exe

#include "small_tree.hpp"

#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>

#include "TROOT.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"

using namespace std;

bool small_tree::VectorLoader::loaded_ = false;

small_tree::VectorLoader small_tree::vl_ = small_tree::VectorLoader();

small_tree::VectorLoader::VectorLoader(){
  if(!loaded_){
    gROOT->ProcessLine("#include <vector>");
    loaded_ = true;
  }
}

const double small_tree::bad_val_ = -999.;

small_tree::small_tree():
  chain_("junk", "junk"),
  tree_("tree", "tree"),
  entry_(0),
  read_only_(false),
  dphi_wlep_(0),
  b_dphi_wlep_(tree_.Branch("dphi_wlep", &dphi_wlep_)),
  c_dphi_wlep_(false),
  dphi_wlep_reliso_(0),
  b_dphi_wlep_reliso_(tree_.Branch("dphi_wlep_reliso", &dphi_wlep_reliso_)),
  c_dphi_wlep_reliso_(false),
  ht_(0),
  b_ht_(tree_.Branch("ht", &ht_)),
  c_ht_(false),
  lep_pt_(0),
  b_lep_pt_(tree_.Branch("lep_pt", &lep_pt_)),
  c_lep_pt_(false),
  lep_pt_reliso_(0),
  b_lep_pt_reliso_(tree_.Branch("lep_pt_reliso", &lep_pt_reliso_)),
  c_lep_pt_reliso_(false),
  met_(0),
  b_met_(tree_.Branch("met", &met_)),
  c_met_(false),
  met_phi_(0),
  b_met_phi_(tree_.Branch("met_phi", &met_phi_)),
  c_met_phi_(false),
  mht_(0),
  b_mht_(tree_.Branch("mht", &mht_)),
  c_mht_(false),
  min_mt_bmet_(0),
  b_min_mt_bmet_(tree_.Branch("min_mt_bmet", &min_mt_bmet_)),
  c_min_mt_bmet_(false),
  min_mt_bmet_with_w_mass_(0),
  b_min_mt_bmet_with_w_mass_(tree_.Branch("min_mt_bmet_with_w_mass", &min_mt_bmet_with_w_mass_)),
  c_min_mt_bmet_with_w_mass_(false),
  mindphin_metjet_(0),
  b_mindphin_metjet_(tree_.Branch("mindphin_metjet", &mindphin_metjet_)),
  c_mindphin_metjet_(false),
  mj_(0),
  b_mj_(tree_.Branch("mj", &mj_)),
  c_mj_(false),
  mt_(0),
  b_mt_(tree_.Branch("mt", &mt_)),
  c_mt_(false),
  mt_reliso_(0),
  b_mt_reliso_(tree_.Branch("mt_reliso", &mt_reliso_)),
  c_mt_reliso_(false),
  ntrupv_mean_(0),
  b_ntrupv_mean_(tree_.Branch("ntrupv_mean", &ntrupv_mean_)),
  c_ntrupv_mean_(false),
  st_(0),
  b_st_(tree_.Branch("st", &st_)),
  c_st_(false),
  st_reliso_(0),
  b_st_reliso_(tree_.Branch("st_reliso", &st_reliso_)),
  c_st_reliso_(false),
  weight_(0),
  b_weight_(tree_.Branch("weight", &weight_)),
  c_weight_(false),
  event_(0),
  b_event_(tree_.Branch("event", &event_)),
  c_event_(false),
  lumiblock_(0),
  b_lumiblock_(tree_.Branch("lumiblock", &lumiblock_)),
  c_lumiblock_(false),
  nbl_(0),
  b_nbl_(tree_.Branch("nbl", &nbl_)),
  c_nbl_(false),
  nbm_(0),
  b_nbm_(tree_.Branch("nbm", &nbm_)),
  c_nbm_(false),
  nbt_(0),
  b_nbt_(tree_.Branch("nbt", &nbt_)),
  c_nbt_(false),
  nels_(0),
  b_nels_(tree_.Branch("nels", &nels_)),
  c_nels_(false),
  nels_reliso_(0),
  b_nels_reliso_(tree_.Branch("nels_reliso", &nels_reliso_)),
  c_nels_reliso_(false),
  nfjets_(0),
  b_nfjets_(tree_.Branch("nfjets", &nfjets_)),
  c_nfjets_(false),
  njets_(0),
  b_njets_(tree_.Branch("njets", &njets_)),
  c_njets_(false),
  nleps_(0),
  b_nleps_(tree_.Branch("nleps", &nleps_)),
  c_nleps_(false),
  nleps_reliso_(0),
  b_nleps_reliso_(tree_.Branch("nleps_reliso", &nleps_reliso_)),
  c_nleps_reliso_(false),
  nmus_(0),
  b_nmus_(tree_.Branch("nmus", &nmus_)),
  c_nmus_(false),
  nmus_reliso_(0),
  b_nmus_reliso_(tree_.Branch("nmus_reliso", &nmus_reliso_)),
  c_nmus_reliso_(false),
  npv_(0),
  b_npv_(tree_.Branch("npv", &npv_)),
  c_npv_(false),
  ntks_(0),
  b_ntks_(tree_.Branch("ntks", &ntks_)),
  c_ntks_(false),
  ntks_chg_(0),
  b_ntks_chg_(tree_.Branch("ntks_chg", &ntks_chg_)),
  c_ntks_chg_(false),
  ntks_chg_reliso_(0),
  b_ntks_chg_reliso_(tree_.Branch("ntks_chg_reliso", &ntks_chg_reliso_)),
  c_ntks_chg_reliso_(false),
  ntrupv_(0),
  b_ntrupv_(tree_.Branch("ntrupv", &ntrupv_)),
  c_ntrupv_(false),
  nvels_(0),
  b_nvels_(tree_.Branch("nvels", &nvels_)),
  c_nvels_(false),
  nvels_reliso_(0),
  b_nvels_reliso_(tree_.Branch("nvels_reliso", &nvels_reliso_)),
  c_nvels_reliso_(false),
  nvmus_(0),
  b_nvmus_(tree_.Branch("nvmus", &nvmus_)),
  c_nvmus_(false),
  nvmus_reliso_(0),
  b_nvmus_reliso_(tree_.Branch("nvmus_reliso", &nvmus_reliso_)),
  c_nvmus_reliso_(false),
  run_(0),
  b_run_(tree_.Branch("run", &run_)),
  c_run_(false),
  els_ispf_(0),
  p_els_ispf_(&els_ispf_),
  b_els_ispf_(tree_.Branch("els_ispf", &p_els_ispf_)),
  c_els_ispf_(false),
  els_sigid_(0),
  p_els_sigid_(&els_sigid_),
  b_els_sigid_(tree_.Branch("els_sigid", &p_els_sigid_)),
  c_els_sigid_(false),
  els_tru_tm_(0),
  p_els_tru_tm_(&els_tru_tm_),
  b_els_tru_tm_(tree_.Branch("els_tru_tm", &p_els_tru_tm_)),
  c_els_tru_tm_(false),
  mus_sigid_(0),
  p_mus_sigid_(&mus_sigid_),
  b_mus_sigid_(tree_.Branch("mus_sigid", &p_mus_sigid_)),
  c_mus_sigid_(false),
  mus_tru_tm_(0),
  p_mus_tru_tm_(&mus_tru_tm_),
  b_mus_tru_tm_(tree_.Branch("mus_tru_tm", &p_mus_tru_tm_)),
  c_mus_tru_tm_(false),
  tks_from_tau_(0),
  p_tks_from_tau_(&tks_from_tau_),
  b_tks_from_tau_(tree_.Branch("tks_from_tau", &p_tks_from_tau_)),
  c_tks_from_tau_(false),
  tks_from_tauhad_(0),
  p_tks_from_tauhad_(&tks_from_tauhad_),
  b_tks_from_tauhad_(tree_.Branch("tks_from_tauhad", &p_tks_from_tauhad_)),
  c_tks_from_tauhad_(false),
  tks_from_taulep_(0),
  p_tks_from_taulep_(&tks_from_taulep_),
  b_tks_from_taulep_(tree_.Branch("tks_from_taulep", &p_tks_from_taulep_)),
  c_tks_from_taulep_(false),
  tks_from_w_(0),
  p_tks_from_w_(&tks_from_w_),
  b_tks_from_w_(tree_.Branch("tks_from_w", &p_tks_from_w_)),
  c_tks_from_w_(false),
  tks_is_primary_(0),
  p_tks_is_primary_(&tks_is_primary_),
  b_tks_is_primary_(tree_.Branch("tks_is_primary", &p_tks_is_primary_)),
  c_tks_is_primary_(false),
  tks_is_primary_reliso_(0),
  p_tks_is_primary_reliso_(&tks_is_primary_reliso_),
  b_tks_is_primary_reliso_(tree_.Branch("tks_is_primary_reliso", &p_tks_is_primary_reliso_)),
  c_tks_is_primary_reliso_(false),
  tks_is_sig_lep_(0),
  p_tks_is_sig_lep_(&tks_is_sig_lep_),
  b_tks_is_sig_lep_(tree_.Branch("tks_is_sig_lep", &p_tks_is_sig_lep_)),
  c_tks_is_sig_lep_(false),
  els_d0_(0),
  p_els_d0_(&els_d0_),
  b_els_d0_(tree_.Branch("els_d0", &p_els_d0_)),
  c_els_d0_(false),
  els_dz_(0),
  p_els_dz_(&els_dz_),
  b_els_dz_(tree_.Branch("els_dz", &p_els_dz_)),
  c_els_dz_(false),
  els_eta_(0),
  p_els_eta_(&els_eta_),
  b_els_eta_(tree_.Branch("els_eta", &p_els_eta_)),
  c_els_eta_(false),
  els_miniso_10_(0),
  p_els_miniso_10_(&els_miniso_10_),
  b_els_miniso_10_(tree_.Branch("els_miniso_10", &p_els_miniso_10_)),
  c_els_miniso_10_(false),
  els_miniso_10_ch_(0),
  p_els_miniso_10_ch_(&els_miniso_10_ch_),
  b_els_miniso_10_ch_(tree_.Branch("els_miniso_10_ch", &p_els_miniso_10_ch_)),
  c_els_miniso_10_ch_(false),
  els_miniso_10_pfpu_(0),
  p_els_miniso_10_pfpu_(&els_miniso_10_pfpu_),
  b_els_miniso_10_pfpu_(tree_.Branch("els_miniso_10_pfpu", &p_els_miniso_10_pfpu_)),
  c_els_miniso_10_pfpu_(false),
  els_miniso_tr10_(0),
  p_els_miniso_tr10_(&els_miniso_tr10_),
  b_els_miniso_tr10_(tree_.Branch("els_miniso_tr10", &p_els_miniso_tr10_)),
  c_els_miniso_tr10_(false),
  els_miniso_tr10_ch_(0),
  p_els_miniso_tr10_ch_(&els_miniso_tr10_ch_),
  b_els_miniso_tr10_ch_(tree_.Branch("els_miniso_tr10_ch", &p_els_miniso_tr10_ch_)),
  c_els_miniso_tr10_ch_(false),
  els_miniso_tr10_pfpu_(0),
  p_els_miniso_tr10_pfpu_(&els_miniso_tr10_pfpu_),
  b_els_miniso_tr10_pfpu_(tree_.Branch("els_miniso_tr10_pfpu", &p_els_miniso_tr10_pfpu_)),
  c_els_miniso_tr10_pfpu_(false),
  els_mt_(0),
  p_els_mt_(&els_mt_),
  b_els_mt_(tree_.Branch("els_mt", &p_els_mt_)),
  c_els_mt_(false),
  els_phi_(0),
  p_els_phi_(&els_phi_),
  b_els_phi_(tree_.Branch("els_phi", &p_els_phi_)),
  c_els_phi_(false),
  els_pt_(0),
  p_els_pt_(&els_pt_),
  b_els_pt_(tree_.Branch("els_pt", &p_els_pt_)),
  c_els_pt_(false),
  els_reliso_(0),
  p_els_reliso_(&els_reliso_),
  b_els_reliso_(tree_.Branch("els_reliso", &p_els_reliso_)),
  c_els_reliso_(false),
  els_reliso_r01_(0),
  p_els_reliso_r01_(&els_reliso_r01_),
  b_els_reliso_r01_(tree_.Branch("els_reliso_r01", &p_els_reliso_r01_)),
  c_els_reliso_r01_(false),
  els_reliso_r015_(0),
  p_els_reliso_r015_(&els_reliso_r015_),
  b_els_reliso_r015_(tree_.Branch("els_reliso_r015", &p_els_reliso_r015_)),
  c_els_reliso_r015_(false),
  els_reliso_r02_(0),
  p_els_reliso_r02_(&els_reliso_r02_),
  b_els_reliso_r02_(tree_.Branch("els_reliso_r02", &p_els_reliso_r02_)),
  c_els_reliso_r02_(false),
  els_reliso_r03_(0),
  p_els_reliso_r03_(&els_reliso_r03_),
  b_els_reliso_r03_(tree_.Branch("els_reliso_r03", &p_els_reliso_r03_)),
  c_els_reliso_r03_(false),
  els_reliso_r04_(0),
  p_els_reliso_r04_(&els_reliso_r04_),
  b_els_reliso_r04_(tree_.Branch("els_reliso_r04", &p_els_reliso_r04_)),
  c_els_reliso_r04_(false),
  els_tru_dr_(0),
  p_els_tru_dr_(&els_tru_dr_),
  b_els_tru_dr_(tree_.Branch("els_tru_dr", &p_els_tru_dr_)),
  c_els_tru_dr_(false),
  fjets_eta_(0),
  p_fjets_eta_(&fjets_eta_),
  b_fjets_eta_(tree_.Branch("fjets_eta", &p_fjets_eta_)),
  c_fjets_eta_(false),
  fjets_m_(0),
  p_fjets_m_(&fjets_m_),
  b_fjets_m_(tree_.Branch("fjets_m", &p_fjets_m_)),
  c_fjets_m_(false),
  fjets_nconst_(0),
  p_fjets_nconst_(&fjets_nconst_),
  b_fjets_nconst_(tree_.Branch("fjets_nconst", &p_fjets_nconst_)),
  c_fjets_nconst_(false),
  fjets_phi_(0),
  p_fjets_phi_(&fjets_phi_),
  b_fjets_phi_(tree_.Branch("fjets_phi", &p_fjets_phi_)),
  c_fjets_phi_(false),
  fjets_pt_(0),
  p_fjets_pt_(&fjets_pt_),
  b_fjets_pt_(tree_.Branch("fjets_pt", &p_fjets_pt_)),
  c_fjets_pt_(false),
  jets_csv_(0),
  p_jets_csv_(&jets_csv_),
  b_jets_csv_(tree_.Branch("jets_csv", &p_jets_csv_)),
  c_jets_csv_(false),
  jets_eta_(0),
  p_jets_eta_(&jets_eta_),
  b_jets_eta_(tree_.Branch("jets_eta", &p_jets_eta_)),
  c_jets_eta_(false),
  jets_id_(0),
  p_jets_id_(&jets_id_),
  b_jets_id_(tree_.Branch("jets_id", &p_jets_id_)),
  c_jets_id_(false),
  jets_islep_(0),
  p_jets_islep_(&jets_islep_),
  b_jets_islep_(tree_.Branch("jets_islep", &p_jets_islep_)),
  c_jets_islep_(false),
  jets_phi_(0),
  p_jets_phi_(&jets_phi_),
  b_jets_phi_(tree_.Branch("jets_phi", &p_jets_phi_)),
  c_jets_phi_(false),
  jets_pt_(0),
  p_jets_pt_(&jets_pt_),
  b_jets_pt_(tree_.Branch("jets_pt", &p_jets_pt_)),
  c_jets_pt_(false),
  mc_eta_(0),
  p_mc_eta_(&mc_eta_),
  b_mc_eta_(tree_.Branch("mc_eta", &p_mc_eta_)),
  c_mc_eta_(false),
  mc_phi_(0),
  p_mc_phi_(&mc_phi_),
  b_mc_phi_(tree_.Branch("mc_phi", &p_mc_phi_)),
  c_mc_phi_(false),
  mc_pt_(0),
  p_mc_pt_(&mc_pt_),
  b_mc_pt_(tree_.Branch("mc_pt", &p_mc_pt_)),
  c_mc_pt_(false),
  mus_d0_(0),
  p_mus_d0_(&mus_d0_),
  b_mus_d0_(tree_.Branch("mus_d0", &p_mus_d0_)),
  c_mus_d0_(false),
  mus_dz_(0),
  p_mus_dz_(&mus_dz_),
  b_mus_dz_(tree_.Branch("mus_dz", &p_mus_dz_)),
  c_mus_dz_(false),
  mus_eta_(0),
  p_mus_eta_(&mus_eta_),
  b_mus_eta_(tree_.Branch("mus_eta", &p_mus_eta_)),
  c_mus_eta_(false),
  mus_miniso_10_(0),
  p_mus_miniso_10_(&mus_miniso_10_),
  b_mus_miniso_10_(tree_.Branch("mus_miniso_10", &p_mus_miniso_10_)),
  c_mus_miniso_10_(false),
  mus_miniso_10_ch_(0),
  p_mus_miniso_10_ch_(&mus_miniso_10_ch_),
  b_mus_miniso_10_ch_(tree_.Branch("mus_miniso_10_ch", &p_mus_miniso_10_ch_)),
  c_mus_miniso_10_ch_(false),
  mus_miniso_10_pfpu_(0),
  p_mus_miniso_10_pfpu_(&mus_miniso_10_pfpu_),
  b_mus_miniso_10_pfpu_(tree_.Branch("mus_miniso_10_pfpu", &p_mus_miniso_10_pfpu_)),
  c_mus_miniso_10_pfpu_(false),
  mus_miniso_tr10_(0),
  p_mus_miniso_tr10_(&mus_miniso_tr10_),
  b_mus_miniso_tr10_(tree_.Branch("mus_miniso_tr10", &p_mus_miniso_tr10_)),
  c_mus_miniso_tr10_(false),
  mus_miniso_tr10_ch_(0),
  p_mus_miniso_tr10_ch_(&mus_miniso_tr10_ch_),
  b_mus_miniso_tr10_ch_(tree_.Branch("mus_miniso_tr10_ch", &p_mus_miniso_tr10_ch_)),
  c_mus_miniso_tr10_ch_(false),
  mus_miniso_tr10_pfpu_(0),
  p_mus_miniso_tr10_pfpu_(&mus_miniso_tr10_pfpu_),
  b_mus_miniso_tr10_pfpu_(tree_.Branch("mus_miniso_tr10_pfpu", &p_mus_miniso_tr10_pfpu_)),
  c_mus_miniso_tr10_pfpu_(false),
  mus_mt_(0),
  p_mus_mt_(&mus_mt_),
  b_mus_mt_(tree_.Branch("mus_mt", &p_mus_mt_)),
  c_mus_mt_(false),
  mus_phi_(0),
  p_mus_phi_(&mus_phi_),
  b_mus_phi_(tree_.Branch("mus_phi", &p_mus_phi_)),
  c_mus_phi_(false),
  mus_pt_(0),
  p_mus_pt_(&mus_pt_),
  b_mus_pt_(tree_.Branch("mus_pt", &p_mus_pt_)),
  c_mus_pt_(false),
  mus_reliso_(0),
  p_mus_reliso_(&mus_reliso_),
  b_mus_reliso_(tree_.Branch("mus_reliso", &p_mus_reliso_)),
  c_mus_reliso_(false),
  mus_reliso_r01_(0),
  p_mus_reliso_r01_(&mus_reliso_r01_),
  b_mus_reliso_r01_(tree_.Branch("mus_reliso_r01", &p_mus_reliso_r01_)),
  c_mus_reliso_r01_(false),
  mus_reliso_r015_(0),
  p_mus_reliso_r015_(&mus_reliso_r015_),
  b_mus_reliso_r015_(tree_.Branch("mus_reliso_r015", &p_mus_reliso_r015_)),
  c_mus_reliso_r015_(false),
  mus_reliso_r02_(0),
  p_mus_reliso_r02_(&mus_reliso_r02_),
  b_mus_reliso_r02_(tree_.Branch("mus_reliso_r02", &p_mus_reliso_r02_)),
  c_mus_reliso_r02_(false),
  mus_reliso_r03_(0),
  p_mus_reliso_r03_(&mus_reliso_r03_),
  b_mus_reliso_r03_(tree_.Branch("mus_reliso_r03", &p_mus_reliso_r03_)),
  c_mus_reliso_r03_(false),
  mus_reliso_r04_(0),
  p_mus_reliso_r04_(&mus_reliso_r04_),
  b_mus_reliso_r04_(tree_.Branch("mus_reliso_r04", &p_mus_reliso_r04_)),
  c_mus_reliso_r04_(false),
  mus_tru_dr_(0),
  p_mus_tru_dr_(&mus_tru_dr_),
  b_mus_tru_dr_(tree_.Branch("mus_tru_dr", &p_mus_tru_dr_)),
  c_mus_tru_dr_(false),
  tks_eta_(0),
  p_tks_eta_(&tks_eta_),
  b_tks_eta_(tree_.Branch("tks_eta", &p_tks_eta_)),
  c_tks_eta_(false),
  tks_mini_ch_(0),
  p_tks_mini_ch_(&tks_mini_ch_),
  b_tks_mini_ch_(tree_.Branch("tks_mini_ch", &p_tks_mini_ch_)),
  c_tks_mini_ch_(false),
  tks_mini_ne_(0),
  p_tks_mini_ne_(&tks_mini_ne_),
  b_tks_mini_ne_(tree_.Branch("tks_mini_ne", &p_tks_mini_ne_)),
  c_tks_mini_ne_(false),
  tks_mt_(0),
  p_tks_mt_(&tks_mt_),
  b_tks_mt_(tree_.Branch("tks_mt", &p_tks_mt_)),
  c_tks_mt_(false),
  tks_phi_(0),
  p_tks_phi_(&tks_phi_),
  b_tks_phi_(tree_.Branch("tks_phi", &p_tks_phi_)),
  c_tks_phi_(false),
  tks_pt_(0),
  p_tks_pt_(&tks_pt_),
  b_tks_pt_(tree_.Branch("tks_pt", &p_tks_pt_)),
  c_tks_pt_(false),
  tks_r02_ch_(0),
  p_tks_r02_ch_(&tks_r02_ch_),
  b_tks_r02_ch_(tree_.Branch("tks_r02_ch", &p_tks_r02_ch_)),
  c_tks_r02_ch_(false),
  tks_r02_ne_(0),
  p_tks_r02_ne_(&tks_r02_ne_),
  b_tks_r02_ne_(tree_.Branch("tks_r02_ne", &p_tks_r02_ne_)),
  c_tks_r02_ne_(false),
  tks_r03_ch_(0),
  p_tks_r03_ch_(&tks_r03_ch_),
  b_tks_r03_ch_(tree_.Branch("tks_r03_ch", &p_tks_r03_ch_)),
  c_tks_r03_ch_(false),
  tks_r03_ne_(0),
  p_tks_r03_ne_(&tks_r03_ne_),
  b_tks_r03_ne_(tree_.Branch("tks_r03_ne", &p_tks_r03_ne_)),
  c_tks_r03_ne_(false),
  tks_tru_dp_(0),
  p_tks_tru_dp_(&tks_tru_dp_),
  b_tks_tru_dp_(tree_.Branch("tks_tru_dp", &p_tks_tru_dp_)),
  c_tks_tru_dp_(false),
  tks_tru_dr_(0),
  p_tks_tru_dr_(&tks_tru_dr_),
  b_tks_tru_dr_(tree_.Branch("tks_tru_dr", &p_tks_tru_dr_)),
  c_tks_tru_dr_(false),
  els_charge_(0),
  p_els_charge_(&els_charge_),
  b_els_charge_(tree_.Branch("els_charge", &p_els_charge_)),
  c_els_charge_(false),
  els_tru_id_(0),
  p_els_tru_id_(&els_tru_id_),
  b_els_tru_id_(tree_.Branch("els_tru_id", &p_els_tru_id_)),
  c_els_tru_id_(false),
  els_tru_momid_(0),
  p_els_tru_momid_(&els_tru_momid_),
  b_els_tru_momid_(tree_.Branch("els_tru_momid", &p_els_tru_momid_)),
  c_els_tru_momid_(false),
  mc_id_(0),
  p_mc_id_(&mc_id_),
  b_mc_id_(tree_.Branch("mc_id", &p_mc_id_)),
  c_mc_id_(false),
  mc_status_(0),
  p_mc_status_(&mc_status_),
  b_mc_status_(tree_.Branch("mc_status", &p_mc_status_)),
  c_mc_status_(false),
  mus_charge_(0),
  p_mus_charge_(&mus_charge_),
  b_mus_charge_(tree_.Branch("mus_charge", &p_mus_charge_)),
  c_mus_charge_(false),
  mus_tru_id_(0),
  p_mus_tru_id_(&mus_tru_id_),
  b_mus_tru_id_(tree_.Branch("mus_tru_id", &p_mus_tru_id_)),
  c_mus_tru_id_(false),
  mus_tru_momid_(0),
  p_mus_tru_momid_(&mus_tru_momid_),
  b_mus_tru_momid_(tree_.Branch("mus_tru_momid", &p_mus_tru_momid_)),
  c_mus_tru_momid_(false),
  tks_from_pv_(0),
  p_tks_from_pv_(&tks_from_pv_),
  b_tks_from_pv_(tree_.Branch("tks_from_pv", &p_tks_from_pv_)),
  c_tks_from_pv_(false),
  tks_id_(0),
  p_tks_id_(&tks_id_),
  b_tks_id_(tree_.Branch("tks_id", &p_tks_id_)),
  c_tks_id_(false),
  tks_num_prongs_(0),
  p_tks_num_prongs_(&tks_num_prongs_),
  b_tks_num_prongs_(tree_.Branch("tks_num_prongs", &p_tks_num_prongs_)),
  c_tks_num_prongs_(false),
  tks_tru_id_(0),
  p_tks_tru_id_(&tks_tru_id_),
  b_tks_tru_id_(tree_.Branch("tks_tru_id", &p_tks_tru_id_)),
  c_tks_tru_id_(false),
  mc_mom_(0),
  p_mc_mom_(&mc_mom_),
  b_mc_mom_(tree_.Branch("mc_mom", &p_mc_mom_)),
  c_mc_mom_(false),
  mc_type_(0),
  b_mc_type_(tree_.Branch("mc_type", &mc_type_)),
  c_mc_type_(false){
}

small_tree::small_tree(const string &filename):
  chain_("tree","tree"),
  tree_("junk","junk"),
  entry_(0),
  read_only_(true),
  dphi_wlep_(0),
  b_dphi_wlep_(NULL),
  c_dphi_wlep_(false),
  dphi_wlep_reliso_(0),
  b_dphi_wlep_reliso_(NULL),
  c_dphi_wlep_reliso_(false),
  ht_(0),
  b_ht_(NULL),
  c_ht_(false),
  lep_pt_(0),
  b_lep_pt_(NULL),
  c_lep_pt_(false),
  lep_pt_reliso_(0),
  b_lep_pt_reliso_(NULL),
  c_lep_pt_reliso_(false),
  met_(0),
  b_met_(NULL),
  c_met_(false),
  met_phi_(0),
  b_met_phi_(NULL),
  c_met_phi_(false),
  mht_(0),
  b_mht_(NULL),
  c_mht_(false),
  min_mt_bmet_(0),
  b_min_mt_bmet_(NULL),
  c_min_mt_bmet_(false),
  min_mt_bmet_with_w_mass_(0),
  b_min_mt_bmet_with_w_mass_(NULL),
  c_min_mt_bmet_with_w_mass_(false),
  mindphin_metjet_(0),
  b_mindphin_metjet_(NULL),
  c_mindphin_metjet_(false),
  mj_(0),
  b_mj_(NULL),
  c_mj_(false),
  mt_(0),
  b_mt_(NULL),
  c_mt_(false),
  mt_reliso_(0),
  b_mt_reliso_(NULL),
  c_mt_reliso_(false),
  ntrupv_mean_(0),
  b_ntrupv_mean_(NULL),
  c_ntrupv_mean_(false),
  st_(0),
  b_st_(NULL),
  c_st_(false),
  st_reliso_(0),
  b_st_reliso_(NULL),
  c_st_reliso_(false),
  weight_(0),
  b_weight_(NULL),
  c_weight_(false),
  event_(0),
  b_event_(NULL),
  c_event_(false),
  lumiblock_(0),
  b_lumiblock_(NULL),
  c_lumiblock_(false),
  nbl_(0),
  b_nbl_(NULL),
  c_nbl_(false),
  nbm_(0),
  b_nbm_(NULL),
  c_nbm_(false),
  nbt_(0),
  b_nbt_(NULL),
  c_nbt_(false),
  nels_(0),
  b_nels_(NULL),
  c_nels_(false),
  nels_reliso_(0),
  b_nels_reliso_(NULL),
  c_nels_reliso_(false),
  nfjets_(0),
  b_nfjets_(NULL),
  c_nfjets_(false),
  njets_(0),
  b_njets_(NULL),
  c_njets_(false),
  nleps_(0),
  b_nleps_(NULL),
  c_nleps_(false),
  nleps_reliso_(0),
  b_nleps_reliso_(NULL),
  c_nleps_reliso_(false),
  nmus_(0),
  b_nmus_(NULL),
  c_nmus_(false),
  nmus_reliso_(0),
  b_nmus_reliso_(NULL),
  c_nmus_reliso_(false),
  npv_(0),
  b_npv_(NULL),
  c_npv_(false),
  ntks_(0),
  b_ntks_(NULL),
  c_ntks_(false),
  ntks_chg_(0),
  b_ntks_chg_(NULL),
  c_ntks_chg_(false),
  ntks_chg_reliso_(0),
  b_ntks_chg_reliso_(NULL),
  c_ntks_chg_reliso_(false),
  ntrupv_(0),
  b_ntrupv_(NULL),
  c_ntrupv_(false),
  nvels_(0),
  b_nvels_(NULL),
  c_nvels_(false),
  nvels_reliso_(0),
  b_nvels_reliso_(NULL),
  c_nvels_reliso_(false),
  nvmus_(0),
  b_nvmus_(NULL),
  c_nvmus_(false),
  nvmus_reliso_(0),
  b_nvmus_reliso_(NULL),
  c_nvmus_reliso_(false),
  run_(0),
  b_run_(NULL),
  c_run_(false),
  els_ispf_(0),
  p_els_ispf_(&els_ispf_),
  b_els_ispf_(NULL),
  c_els_ispf_(false),
  els_sigid_(0),
  p_els_sigid_(&els_sigid_),
  b_els_sigid_(NULL),
  c_els_sigid_(false),
  els_tru_tm_(0),
  p_els_tru_tm_(&els_tru_tm_),
  b_els_tru_tm_(NULL),
  c_els_tru_tm_(false),
  mus_sigid_(0),
  p_mus_sigid_(&mus_sigid_),
  b_mus_sigid_(NULL),
  c_mus_sigid_(false),
  mus_tru_tm_(0),
  p_mus_tru_tm_(&mus_tru_tm_),
  b_mus_tru_tm_(NULL),
  c_mus_tru_tm_(false),
  tks_from_tau_(0),
  p_tks_from_tau_(&tks_from_tau_),
  b_tks_from_tau_(NULL),
  c_tks_from_tau_(false),
  tks_from_tauhad_(0),
  p_tks_from_tauhad_(&tks_from_tauhad_),
  b_tks_from_tauhad_(NULL),
  c_tks_from_tauhad_(false),
  tks_from_taulep_(0),
  p_tks_from_taulep_(&tks_from_taulep_),
  b_tks_from_taulep_(NULL),
  c_tks_from_taulep_(false),
  tks_from_w_(0),
  p_tks_from_w_(&tks_from_w_),
  b_tks_from_w_(NULL),
  c_tks_from_w_(false),
  tks_is_primary_(0),
  p_tks_is_primary_(&tks_is_primary_),
  b_tks_is_primary_(NULL),
  c_tks_is_primary_(false),
  tks_is_primary_reliso_(0),
  p_tks_is_primary_reliso_(&tks_is_primary_reliso_),
  b_tks_is_primary_reliso_(NULL),
  c_tks_is_primary_reliso_(false),
  tks_is_sig_lep_(0),
  p_tks_is_sig_lep_(&tks_is_sig_lep_),
  b_tks_is_sig_lep_(NULL),
  c_tks_is_sig_lep_(false),
  els_d0_(0),
  p_els_d0_(&els_d0_),
  b_els_d0_(NULL),
  c_els_d0_(false),
  els_dz_(0),
  p_els_dz_(&els_dz_),
  b_els_dz_(NULL),
  c_els_dz_(false),
  els_eta_(0),
  p_els_eta_(&els_eta_),
  b_els_eta_(NULL),
  c_els_eta_(false),
  els_miniso_10_(0),
  p_els_miniso_10_(&els_miniso_10_),
  b_els_miniso_10_(NULL),
  c_els_miniso_10_(false),
  els_miniso_10_ch_(0),
  p_els_miniso_10_ch_(&els_miniso_10_ch_),
  b_els_miniso_10_ch_(NULL),
  c_els_miniso_10_ch_(false),
  els_miniso_10_pfpu_(0),
  p_els_miniso_10_pfpu_(&els_miniso_10_pfpu_),
  b_els_miniso_10_pfpu_(NULL),
  c_els_miniso_10_pfpu_(false),
  els_miniso_tr10_(0),
  p_els_miniso_tr10_(&els_miniso_tr10_),
  b_els_miniso_tr10_(NULL),
  c_els_miniso_tr10_(false),
  els_miniso_tr10_ch_(0),
  p_els_miniso_tr10_ch_(&els_miniso_tr10_ch_),
  b_els_miniso_tr10_ch_(NULL),
  c_els_miniso_tr10_ch_(false),
  els_miniso_tr10_pfpu_(0),
  p_els_miniso_tr10_pfpu_(&els_miniso_tr10_pfpu_),
  b_els_miniso_tr10_pfpu_(NULL),
  c_els_miniso_tr10_pfpu_(false),
  els_mt_(0),
  p_els_mt_(&els_mt_),
  b_els_mt_(NULL),
  c_els_mt_(false),
  els_phi_(0),
  p_els_phi_(&els_phi_),
  b_els_phi_(NULL),
  c_els_phi_(false),
  els_pt_(0),
  p_els_pt_(&els_pt_),
  b_els_pt_(NULL),
  c_els_pt_(false),
  els_reliso_(0),
  p_els_reliso_(&els_reliso_),
  b_els_reliso_(NULL),
  c_els_reliso_(false),
  els_reliso_r01_(0),
  p_els_reliso_r01_(&els_reliso_r01_),
  b_els_reliso_r01_(NULL),
  c_els_reliso_r01_(false),
  els_reliso_r015_(0),
  p_els_reliso_r015_(&els_reliso_r015_),
  b_els_reliso_r015_(NULL),
  c_els_reliso_r015_(false),
  els_reliso_r02_(0),
  p_els_reliso_r02_(&els_reliso_r02_),
  b_els_reliso_r02_(NULL),
  c_els_reliso_r02_(false),
  els_reliso_r03_(0),
  p_els_reliso_r03_(&els_reliso_r03_),
  b_els_reliso_r03_(NULL),
  c_els_reliso_r03_(false),
  els_reliso_r04_(0),
  p_els_reliso_r04_(&els_reliso_r04_),
  b_els_reliso_r04_(NULL),
  c_els_reliso_r04_(false),
  els_tru_dr_(0),
  p_els_tru_dr_(&els_tru_dr_),
  b_els_tru_dr_(NULL),
  c_els_tru_dr_(false),
  fjets_eta_(0),
  p_fjets_eta_(&fjets_eta_),
  b_fjets_eta_(NULL),
  c_fjets_eta_(false),
  fjets_m_(0),
  p_fjets_m_(&fjets_m_),
  b_fjets_m_(NULL),
  c_fjets_m_(false),
  fjets_nconst_(0),
  p_fjets_nconst_(&fjets_nconst_),
  b_fjets_nconst_(NULL),
  c_fjets_nconst_(false),
  fjets_phi_(0),
  p_fjets_phi_(&fjets_phi_),
  b_fjets_phi_(NULL),
  c_fjets_phi_(false),
  fjets_pt_(0),
  p_fjets_pt_(&fjets_pt_),
  b_fjets_pt_(NULL),
  c_fjets_pt_(false),
  jets_csv_(0),
  p_jets_csv_(&jets_csv_),
  b_jets_csv_(NULL),
  c_jets_csv_(false),
  jets_eta_(0),
  p_jets_eta_(&jets_eta_),
  b_jets_eta_(NULL),
  c_jets_eta_(false),
  jets_id_(0),
  p_jets_id_(&jets_id_),
  b_jets_id_(NULL),
  c_jets_id_(false),
  jets_islep_(0),
  p_jets_islep_(&jets_islep_),
  b_jets_islep_(NULL),
  c_jets_islep_(false),
  jets_phi_(0),
  p_jets_phi_(&jets_phi_),
  b_jets_phi_(NULL),
  c_jets_phi_(false),
  jets_pt_(0),
  p_jets_pt_(&jets_pt_),
  b_jets_pt_(NULL),
  c_jets_pt_(false),
  mc_eta_(0),
  p_mc_eta_(&mc_eta_),
  b_mc_eta_(NULL),
  c_mc_eta_(false),
  mc_phi_(0),
  p_mc_phi_(&mc_phi_),
  b_mc_phi_(NULL),
  c_mc_phi_(false),
  mc_pt_(0),
  p_mc_pt_(&mc_pt_),
  b_mc_pt_(NULL),
  c_mc_pt_(false),
  mus_d0_(0),
  p_mus_d0_(&mus_d0_),
  b_mus_d0_(NULL),
  c_mus_d0_(false),
  mus_dz_(0),
  p_mus_dz_(&mus_dz_),
  b_mus_dz_(NULL),
  c_mus_dz_(false),
  mus_eta_(0),
  p_mus_eta_(&mus_eta_),
  b_mus_eta_(NULL),
  c_mus_eta_(false),
  mus_miniso_10_(0),
  p_mus_miniso_10_(&mus_miniso_10_),
  b_mus_miniso_10_(NULL),
  c_mus_miniso_10_(false),
  mus_miniso_10_ch_(0),
  p_mus_miniso_10_ch_(&mus_miniso_10_ch_),
  b_mus_miniso_10_ch_(NULL),
  c_mus_miniso_10_ch_(false),
  mus_miniso_10_pfpu_(0),
  p_mus_miniso_10_pfpu_(&mus_miniso_10_pfpu_),
  b_mus_miniso_10_pfpu_(NULL),
  c_mus_miniso_10_pfpu_(false),
  mus_miniso_tr10_(0),
  p_mus_miniso_tr10_(&mus_miniso_tr10_),
  b_mus_miniso_tr10_(NULL),
  c_mus_miniso_tr10_(false),
  mus_miniso_tr10_ch_(0),
  p_mus_miniso_tr10_ch_(&mus_miniso_tr10_ch_),
  b_mus_miniso_tr10_ch_(NULL),
  c_mus_miniso_tr10_ch_(false),
  mus_miniso_tr10_pfpu_(0),
  p_mus_miniso_tr10_pfpu_(&mus_miniso_tr10_pfpu_),
  b_mus_miniso_tr10_pfpu_(NULL),
  c_mus_miniso_tr10_pfpu_(false),
  mus_mt_(0),
  p_mus_mt_(&mus_mt_),
  b_mus_mt_(NULL),
  c_mus_mt_(false),
  mus_phi_(0),
  p_mus_phi_(&mus_phi_),
  b_mus_phi_(NULL),
  c_mus_phi_(false),
  mus_pt_(0),
  p_mus_pt_(&mus_pt_),
  b_mus_pt_(NULL),
  c_mus_pt_(false),
  mus_reliso_(0),
  p_mus_reliso_(&mus_reliso_),
  b_mus_reliso_(NULL),
  c_mus_reliso_(false),
  mus_reliso_r01_(0),
  p_mus_reliso_r01_(&mus_reliso_r01_),
  b_mus_reliso_r01_(NULL),
  c_mus_reliso_r01_(false),
  mus_reliso_r015_(0),
  p_mus_reliso_r015_(&mus_reliso_r015_),
  b_mus_reliso_r015_(NULL),
  c_mus_reliso_r015_(false),
  mus_reliso_r02_(0),
  p_mus_reliso_r02_(&mus_reliso_r02_),
  b_mus_reliso_r02_(NULL),
  c_mus_reliso_r02_(false),
  mus_reliso_r03_(0),
  p_mus_reliso_r03_(&mus_reliso_r03_),
  b_mus_reliso_r03_(NULL),
  c_mus_reliso_r03_(false),
  mus_reliso_r04_(0),
  p_mus_reliso_r04_(&mus_reliso_r04_),
  b_mus_reliso_r04_(NULL),
  c_mus_reliso_r04_(false),
  mus_tru_dr_(0),
  p_mus_tru_dr_(&mus_tru_dr_),
  b_mus_tru_dr_(NULL),
  c_mus_tru_dr_(false),
  tks_eta_(0),
  p_tks_eta_(&tks_eta_),
  b_tks_eta_(NULL),
  c_tks_eta_(false),
  tks_mini_ch_(0),
  p_tks_mini_ch_(&tks_mini_ch_),
  b_tks_mini_ch_(NULL),
  c_tks_mini_ch_(false),
  tks_mini_ne_(0),
  p_tks_mini_ne_(&tks_mini_ne_),
  b_tks_mini_ne_(NULL),
  c_tks_mini_ne_(false),
  tks_mt_(0),
  p_tks_mt_(&tks_mt_),
  b_tks_mt_(NULL),
  c_tks_mt_(false),
  tks_phi_(0),
  p_tks_phi_(&tks_phi_),
  b_tks_phi_(NULL),
  c_tks_phi_(false),
  tks_pt_(0),
  p_tks_pt_(&tks_pt_),
  b_tks_pt_(NULL),
  c_tks_pt_(false),
  tks_r02_ch_(0),
  p_tks_r02_ch_(&tks_r02_ch_),
  b_tks_r02_ch_(NULL),
  c_tks_r02_ch_(false),
  tks_r02_ne_(0),
  p_tks_r02_ne_(&tks_r02_ne_),
  b_tks_r02_ne_(NULL),
  c_tks_r02_ne_(false),
  tks_r03_ch_(0),
  p_tks_r03_ch_(&tks_r03_ch_),
  b_tks_r03_ch_(NULL),
  c_tks_r03_ch_(false),
  tks_r03_ne_(0),
  p_tks_r03_ne_(&tks_r03_ne_),
  b_tks_r03_ne_(NULL),
  c_tks_r03_ne_(false),
  tks_tru_dp_(0),
  p_tks_tru_dp_(&tks_tru_dp_),
  b_tks_tru_dp_(NULL),
  c_tks_tru_dp_(false),
  tks_tru_dr_(0),
  p_tks_tru_dr_(&tks_tru_dr_),
  b_tks_tru_dr_(NULL),
  c_tks_tru_dr_(false),
  els_charge_(0),
  p_els_charge_(&els_charge_),
  b_els_charge_(NULL),
  c_els_charge_(false),
  els_tru_id_(0),
  p_els_tru_id_(&els_tru_id_),
  b_els_tru_id_(NULL),
  c_els_tru_id_(false),
  els_tru_momid_(0),
  p_els_tru_momid_(&els_tru_momid_),
  b_els_tru_momid_(NULL),
  c_els_tru_momid_(false),
  mc_id_(0),
  p_mc_id_(&mc_id_),
  b_mc_id_(NULL),
  c_mc_id_(false),
  mc_status_(0),
  p_mc_status_(&mc_status_),
  b_mc_status_(NULL),
  c_mc_status_(false),
  mus_charge_(0),
  p_mus_charge_(&mus_charge_),
  b_mus_charge_(NULL),
  c_mus_charge_(false),
  mus_tru_id_(0),
  p_mus_tru_id_(&mus_tru_id_),
  b_mus_tru_id_(NULL),
  c_mus_tru_id_(false),
  mus_tru_momid_(0),
  p_mus_tru_momid_(&mus_tru_momid_),
  b_mus_tru_momid_(NULL),
  c_mus_tru_momid_(false),
  tks_from_pv_(0),
  p_tks_from_pv_(&tks_from_pv_),
  b_tks_from_pv_(NULL),
  c_tks_from_pv_(false),
  tks_id_(0),
  p_tks_id_(&tks_id_),
  b_tks_id_(NULL),
  c_tks_id_(false),
  tks_num_prongs_(0),
  p_tks_num_prongs_(&tks_num_prongs_),
  b_tks_num_prongs_(NULL),
  c_tks_num_prongs_(false),
  tks_tru_id_(0),
  p_tks_tru_id_(&tks_tru_id_),
  b_tks_tru_id_(NULL),
  c_tks_tru_id_(false),
  mc_mom_(0),
  p_mc_mom_(&mc_mom_),
  b_mc_mom_(NULL),
  c_mc_mom_(false),
  mc_type_(0),
  b_mc_type_(NULL),
  c_mc_type_(false){
  chain_.Add(filename.c_str());
  chain_.SetBranchAddress("dphi_wlep", &dphi_wlep_, &b_dphi_wlep_);
  chain_.SetBranchAddress("dphi_wlep_reliso", &dphi_wlep_reliso_, &b_dphi_wlep_reliso_);
  chain_.SetBranchAddress("ht", &ht_, &b_ht_);
  chain_.SetBranchAddress("lep_pt", &lep_pt_, &b_lep_pt_);
  chain_.SetBranchAddress("lep_pt_reliso", &lep_pt_reliso_, &b_lep_pt_reliso_);
  chain_.SetBranchAddress("met", &met_, &b_met_);
  chain_.SetBranchAddress("met_phi", &met_phi_, &b_met_phi_);
  chain_.SetBranchAddress("mht", &mht_, &b_mht_);
  chain_.SetBranchAddress("min_mt_bmet", &min_mt_bmet_, &b_min_mt_bmet_);
  chain_.SetBranchAddress("min_mt_bmet_with_w_mass", &min_mt_bmet_with_w_mass_, &b_min_mt_bmet_with_w_mass_);
  chain_.SetBranchAddress("mindphin_metjet", &mindphin_metjet_, &b_mindphin_metjet_);
  chain_.SetBranchAddress("mj", &mj_, &b_mj_);
  chain_.SetBranchAddress("mt", &mt_, &b_mt_);
  chain_.SetBranchAddress("mt_reliso", &mt_reliso_, &b_mt_reliso_);
  chain_.SetBranchAddress("ntrupv_mean", &ntrupv_mean_, &b_ntrupv_mean_);
  chain_.SetBranchAddress("st", &st_, &b_st_);
  chain_.SetBranchAddress("st_reliso", &st_reliso_, &b_st_reliso_);
  chain_.SetBranchAddress("weight", &weight_, &b_weight_);
  chain_.SetBranchAddress("event", &event_, &b_event_);
  chain_.SetBranchAddress("lumiblock", &lumiblock_, &b_lumiblock_);
  chain_.SetBranchAddress("nbl", &nbl_, &b_nbl_);
  chain_.SetBranchAddress("nbm", &nbm_, &b_nbm_);
  chain_.SetBranchAddress("nbt", &nbt_, &b_nbt_);
  chain_.SetBranchAddress("nels", &nels_, &b_nels_);
  chain_.SetBranchAddress("nels_reliso", &nels_reliso_, &b_nels_reliso_);
  chain_.SetBranchAddress("nfjets", &nfjets_, &b_nfjets_);
  chain_.SetBranchAddress("njets", &njets_, &b_njets_);
  chain_.SetBranchAddress("nleps", &nleps_, &b_nleps_);
  chain_.SetBranchAddress("nleps_reliso", &nleps_reliso_, &b_nleps_reliso_);
  chain_.SetBranchAddress("nmus", &nmus_, &b_nmus_);
  chain_.SetBranchAddress("nmus_reliso", &nmus_reliso_, &b_nmus_reliso_);
  chain_.SetBranchAddress("npv", &npv_, &b_npv_);
  chain_.SetBranchAddress("ntks", &ntks_, &b_ntks_);
  chain_.SetBranchAddress("ntks_chg", &ntks_chg_, &b_ntks_chg_);
  chain_.SetBranchAddress("ntks_chg_reliso", &ntks_chg_reliso_, &b_ntks_chg_reliso_);
  chain_.SetBranchAddress("ntrupv", &ntrupv_, &b_ntrupv_);
  chain_.SetBranchAddress("nvels", &nvels_, &b_nvels_);
  chain_.SetBranchAddress("nvels_reliso", &nvels_reliso_, &b_nvels_reliso_);
  chain_.SetBranchAddress("nvmus", &nvmus_, &b_nvmus_);
  chain_.SetBranchAddress("nvmus_reliso", &nvmus_reliso_, &b_nvmus_reliso_);
  chain_.SetBranchAddress("run", &run_, &b_run_);
  chain_.SetBranchAddress("els_ispf", &p_els_ispf_, &b_els_ispf_);
  chain_.SetBranchAddress("els_sigid", &p_els_sigid_, &b_els_sigid_);
  chain_.SetBranchAddress("els_tru_tm", &p_els_tru_tm_, &b_els_tru_tm_);
  chain_.SetBranchAddress("mus_sigid", &p_mus_sigid_, &b_mus_sigid_);
  chain_.SetBranchAddress("mus_tru_tm", &p_mus_tru_tm_, &b_mus_tru_tm_);
  chain_.SetBranchAddress("tks_from_tau", &p_tks_from_tau_, &b_tks_from_tau_);
  chain_.SetBranchAddress("tks_from_tauhad", &p_tks_from_tauhad_, &b_tks_from_tauhad_);
  chain_.SetBranchAddress("tks_from_taulep", &p_tks_from_taulep_, &b_tks_from_taulep_);
  chain_.SetBranchAddress("tks_from_w", &p_tks_from_w_, &b_tks_from_w_);
  chain_.SetBranchAddress("tks_is_primary", &p_tks_is_primary_, &b_tks_is_primary_);
  chain_.SetBranchAddress("tks_is_primary_reliso", &p_tks_is_primary_reliso_, &b_tks_is_primary_reliso_);
  chain_.SetBranchAddress("tks_is_sig_lep", &p_tks_is_sig_lep_, &b_tks_is_sig_lep_);
  chain_.SetBranchAddress("els_d0", &p_els_d0_, &b_els_d0_);
  chain_.SetBranchAddress("els_dz", &p_els_dz_, &b_els_dz_);
  chain_.SetBranchAddress("els_eta", &p_els_eta_, &b_els_eta_);
  chain_.SetBranchAddress("els_miniso_10", &p_els_miniso_10_, &b_els_miniso_10_);
  chain_.SetBranchAddress("els_miniso_10_ch", &p_els_miniso_10_ch_, &b_els_miniso_10_ch_);
  chain_.SetBranchAddress("els_miniso_10_pfpu", &p_els_miniso_10_pfpu_, &b_els_miniso_10_pfpu_);
  chain_.SetBranchAddress("els_miniso_tr10", &p_els_miniso_tr10_, &b_els_miniso_tr10_);
  chain_.SetBranchAddress("els_miniso_tr10_ch", &p_els_miniso_tr10_ch_, &b_els_miniso_tr10_ch_);
  chain_.SetBranchAddress("els_miniso_tr10_pfpu", &p_els_miniso_tr10_pfpu_, &b_els_miniso_tr10_pfpu_);
  chain_.SetBranchAddress("els_mt", &p_els_mt_, &b_els_mt_);
  chain_.SetBranchAddress("els_phi", &p_els_phi_, &b_els_phi_);
  chain_.SetBranchAddress("els_pt", &p_els_pt_, &b_els_pt_);
  chain_.SetBranchAddress("els_reliso", &p_els_reliso_, &b_els_reliso_);
  chain_.SetBranchAddress("els_reliso_r01", &p_els_reliso_r01_, &b_els_reliso_r01_);
  chain_.SetBranchAddress("els_reliso_r015", &p_els_reliso_r015_, &b_els_reliso_r015_);
  chain_.SetBranchAddress("els_reliso_r02", &p_els_reliso_r02_, &b_els_reliso_r02_);
  chain_.SetBranchAddress("els_reliso_r03", &p_els_reliso_r03_, &b_els_reliso_r03_);
  chain_.SetBranchAddress("els_reliso_r04", &p_els_reliso_r04_, &b_els_reliso_r04_);
  chain_.SetBranchAddress("els_tru_dr", &p_els_tru_dr_, &b_els_tru_dr_);
  chain_.SetBranchAddress("fjets_eta", &p_fjets_eta_, &b_fjets_eta_);
  chain_.SetBranchAddress("fjets_m", &p_fjets_m_, &b_fjets_m_);
  chain_.SetBranchAddress("fjets_nconst", &p_fjets_nconst_, &b_fjets_nconst_);
  chain_.SetBranchAddress("fjets_phi", &p_fjets_phi_, &b_fjets_phi_);
  chain_.SetBranchAddress("fjets_pt", &p_fjets_pt_, &b_fjets_pt_);
  chain_.SetBranchAddress("jets_csv", &p_jets_csv_, &b_jets_csv_);
  chain_.SetBranchAddress("jets_eta", &p_jets_eta_, &b_jets_eta_);
  chain_.SetBranchAddress("jets_id", &p_jets_id_, &b_jets_id_);
  chain_.SetBranchAddress("jets_islep", &p_jets_islep_, &b_jets_islep_);
  chain_.SetBranchAddress("jets_phi", &p_jets_phi_, &b_jets_phi_);
  chain_.SetBranchAddress("jets_pt", &p_jets_pt_, &b_jets_pt_);
  chain_.SetBranchAddress("mc_eta", &p_mc_eta_, &b_mc_eta_);
  chain_.SetBranchAddress("mc_phi", &p_mc_phi_, &b_mc_phi_);
  chain_.SetBranchAddress("mc_pt", &p_mc_pt_, &b_mc_pt_);
  chain_.SetBranchAddress("mus_d0", &p_mus_d0_, &b_mus_d0_);
  chain_.SetBranchAddress("mus_dz", &p_mus_dz_, &b_mus_dz_);
  chain_.SetBranchAddress("mus_eta", &p_mus_eta_, &b_mus_eta_);
  chain_.SetBranchAddress("mus_miniso_10", &p_mus_miniso_10_, &b_mus_miniso_10_);
  chain_.SetBranchAddress("mus_miniso_10_ch", &p_mus_miniso_10_ch_, &b_mus_miniso_10_ch_);
  chain_.SetBranchAddress("mus_miniso_10_pfpu", &p_mus_miniso_10_pfpu_, &b_mus_miniso_10_pfpu_);
  chain_.SetBranchAddress("mus_miniso_tr10", &p_mus_miniso_tr10_, &b_mus_miniso_tr10_);
  chain_.SetBranchAddress("mus_miniso_tr10_ch", &p_mus_miniso_tr10_ch_, &b_mus_miniso_tr10_ch_);
  chain_.SetBranchAddress("mus_miniso_tr10_pfpu", &p_mus_miniso_tr10_pfpu_, &b_mus_miniso_tr10_pfpu_);
  chain_.SetBranchAddress("mus_mt", &p_mus_mt_, &b_mus_mt_);
  chain_.SetBranchAddress("mus_phi", &p_mus_phi_, &b_mus_phi_);
  chain_.SetBranchAddress("mus_pt", &p_mus_pt_, &b_mus_pt_);
  chain_.SetBranchAddress("mus_reliso", &p_mus_reliso_, &b_mus_reliso_);
  chain_.SetBranchAddress("mus_reliso_r01", &p_mus_reliso_r01_, &b_mus_reliso_r01_);
  chain_.SetBranchAddress("mus_reliso_r015", &p_mus_reliso_r015_, &b_mus_reliso_r015_);
  chain_.SetBranchAddress("mus_reliso_r02", &p_mus_reliso_r02_, &b_mus_reliso_r02_);
  chain_.SetBranchAddress("mus_reliso_r03", &p_mus_reliso_r03_, &b_mus_reliso_r03_);
  chain_.SetBranchAddress("mus_reliso_r04", &p_mus_reliso_r04_, &b_mus_reliso_r04_);
  chain_.SetBranchAddress("mus_tru_dr", &p_mus_tru_dr_, &b_mus_tru_dr_);
  chain_.SetBranchAddress("tks_eta", &p_tks_eta_, &b_tks_eta_);
  chain_.SetBranchAddress("tks_mini_ch", &p_tks_mini_ch_, &b_tks_mini_ch_);
  chain_.SetBranchAddress("tks_mini_ne", &p_tks_mini_ne_, &b_tks_mini_ne_);
  chain_.SetBranchAddress("tks_mt", &p_tks_mt_, &b_tks_mt_);
  chain_.SetBranchAddress("tks_phi", &p_tks_phi_, &b_tks_phi_);
  chain_.SetBranchAddress("tks_pt", &p_tks_pt_, &b_tks_pt_);
  chain_.SetBranchAddress("tks_r02_ch", &p_tks_r02_ch_, &b_tks_r02_ch_);
  chain_.SetBranchAddress("tks_r02_ne", &p_tks_r02_ne_, &b_tks_r02_ne_);
  chain_.SetBranchAddress("tks_r03_ch", &p_tks_r03_ch_, &b_tks_r03_ch_);
  chain_.SetBranchAddress("tks_r03_ne", &p_tks_r03_ne_, &b_tks_r03_ne_);
  chain_.SetBranchAddress("tks_tru_dp", &p_tks_tru_dp_, &b_tks_tru_dp_);
  chain_.SetBranchAddress("tks_tru_dr", &p_tks_tru_dr_, &b_tks_tru_dr_);
  chain_.SetBranchAddress("els_charge", &p_els_charge_, &b_els_charge_);
  chain_.SetBranchAddress("els_tru_id", &p_els_tru_id_, &b_els_tru_id_);
  chain_.SetBranchAddress("els_tru_momid", &p_els_tru_momid_, &b_els_tru_momid_);
  chain_.SetBranchAddress("mc_id", &p_mc_id_, &b_mc_id_);
  chain_.SetBranchAddress("mc_status", &p_mc_status_, &b_mc_status_);
  chain_.SetBranchAddress("mus_charge", &p_mus_charge_, &b_mus_charge_);
  chain_.SetBranchAddress("mus_tru_id", &p_mus_tru_id_, &b_mus_tru_id_);
  chain_.SetBranchAddress("mus_tru_momid", &p_mus_tru_momid_, &b_mus_tru_momid_);
  chain_.SetBranchAddress("tks_from_pv", &p_tks_from_pv_, &b_tks_from_pv_);
  chain_.SetBranchAddress("tks_id", &p_tks_id_, &b_tks_id_);
  chain_.SetBranchAddress("tks_num_prongs", &p_tks_num_prongs_, &b_tks_num_prongs_);
  chain_.SetBranchAddress("tks_tru_id", &p_tks_tru_id_, &b_tks_tru_id_);
  chain_.SetBranchAddress("mc_mom", &p_mc_mom_, &b_mc_mom_);
  chain_.SetBranchAddress("mc_type", &mc_type_, &b_mc_type_);
}

void small_tree::Fill(){
  if(read_only_){
    throw std::logic_error("Trying to write to read-only tree");
  }else{
    tree_.Fill();
  }

  //Resetting variables
  dphi_wlep_ = static_cast<float>(bad_val_);
  dphi_wlep_reliso_ = static_cast<float>(bad_val_);
  ht_ = static_cast<float>(bad_val_);
  lep_pt_ = static_cast<float>(bad_val_);
  lep_pt_reliso_ = static_cast<float>(bad_val_);
  met_ = static_cast<float>(bad_val_);
  met_phi_ = static_cast<float>(bad_val_);
  mht_ = static_cast<float>(bad_val_);
  min_mt_bmet_ = static_cast<float>(bad_val_);
  min_mt_bmet_with_w_mass_ = static_cast<float>(bad_val_);
  mindphin_metjet_ = static_cast<float>(bad_val_);
  mj_ = static_cast<float>(bad_val_);
  mt_ = static_cast<float>(bad_val_);
  mt_reliso_ = static_cast<float>(bad_val_);
  ntrupv_mean_ = static_cast<float>(bad_val_);
  st_ = static_cast<float>(bad_val_);
  st_reliso_ = static_cast<float>(bad_val_);
  weight_ = static_cast<float>(bad_val_);
  event_ = static_cast<int>(bad_val_);
  lumiblock_ = static_cast<int>(bad_val_);
  nbl_ = static_cast<int>(bad_val_);
  nbm_ = static_cast<int>(bad_val_);
  nbt_ = static_cast<int>(bad_val_);
  nels_ = static_cast<int>(bad_val_);
  nels_reliso_ = static_cast<int>(bad_val_);
  nfjets_ = static_cast<int>(bad_val_);
  njets_ = static_cast<int>(bad_val_);
  nleps_ = static_cast<int>(bad_val_);
  nleps_reliso_ = static_cast<int>(bad_val_);
  nmus_ = static_cast<int>(bad_val_);
  nmus_reliso_ = static_cast<int>(bad_val_);
  npv_ = static_cast<int>(bad_val_);
  ntks_ = static_cast<int>(bad_val_);
  ntks_chg_ = static_cast<int>(bad_val_);
  ntks_chg_reliso_ = static_cast<int>(bad_val_);
  ntrupv_ = static_cast<int>(bad_val_);
  nvels_ = static_cast<int>(bad_val_);
  nvels_reliso_ = static_cast<int>(bad_val_);
  nvmus_ = static_cast<int>(bad_val_);
  nvmus_reliso_ = static_cast<int>(bad_val_);
  run_ = static_cast<int>(bad_val_);
  els_ispf_.clear();
  els_sigid_.clear();
  els_tru_tm_.clear();
  mus_sigid_.clear();
  mus_tru_tm_.clear();
  tks_from_tau_.clear();
  tks_from_tauhad_.clear();
  tks_from_taulep_.clear();
  tks_from_w_.clear();
  tks_is_primary_.clear();
  tks_is_primary_reliso_.clear();
  tks_is_sig_lep_.clear();
  els_d0_.clear();
  els_dz_.clear();
  els_eta_.clear();
  els_miniso_10_.clear();
  els_miniso_10_ch_.clear();
  els_miniso_10_pfpu_.clear();
  els_miniso_tr10_.clear();
  els_miniso_tr10_ch_.clear();
  els_miniso_tr10_pfpu_.clear();
  els_mt_.clear();
  els_phi_.clear();
  els_pt_.clear();
  els_reliso_.clear();
  els_reliso_r01_.clear();
  els_reliso_r015_.clear();
  els_reliso_r02_.clear();
  els_reliso_r03_.clear();
  els_reliso_r04_.clear();
  els_tru_dr_.clear();
  fjets_eta_.clear();
  fjets_m_.clear();
  fjets_nconst_.clear();
  fjets_phi_.clear();
  fjets_pt_.clear();
  jets_csv_.clear();
  jets_eta_.clear();
  jets_id_.clear();
  jets_islep_.clear();
  jets_phi_.clear();
  jets_pt_.clear();
  mc_eta_.clear();
  mc_phi_.clear();
  mc_pt_.clear();
  mus_d0_.clear();
  mus_dz_.clear();
  mus_eta_.clear();
  mus_miniso_10_.clear();
  mus_miniso_10_ch_.clear();
  mus_miniso_10_pfpu_.clear();
  mus_miniso_tr10_.clear();
  mus_miniso_tr10_ch_.clear();
  mus_miniso_tr10_pfpu_.clear();
  mus_mt_.clear();
  mus_phi_.clear();
  mus_pt_.clear();
  mus_reliso_.clear();
  mus_reliso_r01_.clear();
  mus_reliso_r015_.clear();
  mus_reliso_r02_.clear();
  mus_reliso_r03_.clear();
  mus_reliso_r04_.clear();
  mus_tru_dr_.clear();
  tks_eta_.clear();
  tks_mini_ch_.clear();
  tks_mini_ne_.clear();
  tks_mt_.clear();
  tks_phi_.clear();
  tks_pt_.clear();
  tks_r02_ch_.clear();
  tks_r02_ne_.clear();
  tks_r03_ch_.clear();
  tks_r03_ne_.clear();
  tks_tru_dp_.clear();
  tks_tru_dr_.clear();
  els_charge_.clear();
  els_tru_id_.clear();
  els_tru_momid_.clear();
  mc_id_.clear();
  mc_status_.clear();
  mus_charge_.clear();
  mus_tru_id_.clear();
  mus_tru_momid_.clear();
  tks_from_pv_.clear();
  tks_id_.clear();
  tks_num_prongs_.clear();
  tks_tru_id_.clear();
  mc_mom_.clear();
  mc_type_ = static_cast<unsigned>(bad_val_);
}

void small_tree::Write(){
  if(read_only_){
    throw std::logic_error("Trying to write to read-only tree.");
  }else{
    tree_.Write();
  }
}

string small_tree::Type() const{
  return "";
}

small_tree::~small_tree(){
}

int small_tree::Add(const std::string &filename){
  if(!read_only_){
    throw std::logic_error("Trying to add files to tree opened for writing.");
  }
  return chain_.Add(filename.c_str());
}

long small_tree::GetEntries() const{
  if(read_only_){
    return chain_.GetEntries();
  }else{
    return tree_.GetEntries();
  }
}

void small_tree::GetEntry(const long entry){
  if(!read_only_){
    throw std::logic_error("Trying to read from write-only tree.");
  }

  c_dphi_wlep_ = false;
  c_dphi_wlep_reliso_ = false;
  c_ht_ = false;
  c_lep_pt_ = false;
  c_lep_pt_reliso_ = false;
  c_met_ = false;
  c_met_phi_ = false;
  c_mht_ = false;
  c_min_mt_bmet_ = false;
  c_min_mt_bmet_with_w_mass_ = false;
  c_mindphin_metjet_ = false;
  c_mj_ = false;
  c_mt_ = false;
  c_mt_reliso_ = false;
  c_ntrupv_mean_ = false;
  c_st_ = false;
  c_st_reliso_ = false;
  c_weight_ = false;
  c_event_ = false;
  c_lumiblock_ = false;
  c_nbl_ = false;
  c_nbm_ = false;
  c_nbt_ = false;
  c_nels_ = false;
  c_nels_reliso_ = false;
  c_nfjets_ = false;
  c_njets_ = false;
  c_nleps_ = false;
  c_nleps_reliso_ = false;
  c_nmus_ = false;
  c_nmus_reliso_ = false;
  c_npv_ = false;
  c_ntks_ = false;
  c_ntks_chg_ = false;
  c_ntks_chg_reliso_ = false;
  c_ntrupv_ = false;
  c_nvels_ = false;
  c_nvels_reliso_ = false;
  c_nvmus_ = false;
  c_nvmus_reliso_ = false;
  c_run_ = false;
  c_els_ispf_ = false;
  c_els_sigid_ = false;
  c_els_tru_tm_ = false;
  c_mus_sigid_ = false;
  c_mus_tru_tm_ = false;
  c_tks_from_tau_ = false;
  c_tks_from_tauhad_ = false;
  c_tks_from_taulep_ = false;
  c_tks_from_w_ = false;
  c_tks_is_primary_ = false;
  c_tks_is_primary_reliso_ = false;
  c_tks_is_sig_lep_ = false;
  c_els_d0_ = false;
  c_els_dz_ = false;
  c_els_eta_ = false;
  c_els_miniso_10_ = false;
  c_els_miniso_10_ch_ = false;
  c_els_miniso_10_pfpu_ = false;
  c_els_miniso_tr10_ = false;
  c_els_miniso_tr10_ch_ = false;
  c_els_miniso_tr10_pfpu_ = false;
  c_els_mt_ = false;
  c_els_phi_ = false;
  c_els_pt_ = false;
  c_els_reliso_ = false;
  c_els_reliso_r01_ = false;
  c_els_reliso_r015_ = false;
  c_els_reliso_r02_ = false;
  c_els_reliso_r03_ = false;
  c_els_reliso_r04_ = false;
  c_els_tru_dr_ = false;
  c_fjets_eta_ = false;
  c_fjets_m_ = false;
  c_fjets_nconst_ = false;
  c_fjets_phi_ = false;
  c_fjets_pt_ = false;
  c_jets_csv_ = false;
  c_jets_eta_ = false;
  c_jets_id_ = false;
  c_jets_islep_ = false;
  c_jets_phi_ = false;
  c_jets_pt_ = false;
  c_mc_eta_ = false;
  c_mc_phi_ = false;
  c_mc_pt_ = false;
  c_mus_d0_ = false;
  c_mus_dz_ = false;
  c_mus_eta_ = false;
  c_mus_miniso_10_ = false;
  c_mus_miniso_10_ch_ = false;
  c_mus_miniso_10_pfpu_ = false;
  c_mus_miniso_tr10_ = false;
  c_mus_miniso_tr10_ch_ = false;
  c_mus_miniso_tr10_pfpu_ = false;
  c_mus_mt_ = false;
  c_mus_phi_ = false;
  c_mus_pt_ = false;
  c_mus_reliso_ = false;
  c_mus_reliso_r01_ = false;
  c_mus_reliso_r015_ = false;
  c_mus_reliso_r02_ = false;
  c_mus_reliso_r03_ = false;
  c_mus_reliso_r04_ = false;
  c_mus_tru_dr_ = false;
  c_tks_eta_ = false;
  c_tks_mini_ch_ = false;
  c_tks_mini_ne_ = false;
  c_tks_mt_ = false;
  c_tks_phi_ = false;
  c_tks_pt_ = false;
  c_tks_r02_ch_ = false;
  c_tks_r02_ne_ = false;
  c_tks_r03_ch_ = false;
  c_tks_r03_ne_ = false;
  c_tks_tru_dp_ = false;
  c_tks_tru_dr_ = false;
  c_els_charge_ = false;
  c_els_tru_id_ = false;
  c_els_tru_momid_ = false;
  c_mc_id_ = false;
  c_mc_status_ = false;
  c_mus_charge_ = false;
  c_mus_tru_id_ = false;
  c_mus_tru_momid_ = false;
  c_tks_from_pv_ = false;
  c_tks_id_ = false;
  c_tks_num_prongs_ = false;
  c_tks_tru_id_ = false;
  c_mc_mom_ = false;
  c_mc_type_ = false;
  entry_ = chain_.LoadTree(entry);
}

float const & small_tree::dphi_wlep() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_dphi_wlep_ && b_dphi_wlep_){
    b_dphi_wlep_->GetEntry(entry_);
    c_dphi_wlep_ = true;
  }
  return dphi_wlep_;
}

float const & small_tree::dphi_wlep_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_dphi_wlep_reliso_ && b_dphi_wlep_reliso_){
    b_dphi_wlep_reliso_->GetEntry(entry_);
    c_dphi_wlep_reliso_ = true;
  }
  return dphi_wlep_reliso_;
}

float const & small_tree::ht() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_ht_ && b_ht_){
    b_ht_->GetEntry(entry_);
    c_ht_ = true;
  }
  return ht_;
}

float const & small_tree::lep_pt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_lep_pt_ && b_lep_pt_){
    b_lep_pt_->GetEntry(entry_);
    c_lep_pt_ = true;
  }
  return lep_pt_;
}

float const & small_tree::lep_pt_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_lep_pt_reliso_ && b_lep_pt_reliso_){
    b_lep_pt_reliso_->GetEntry(entry_);
    c_lep_pt_reliso_ = true;
  }
  return lep_pt_reliso_;
}

float const & small_tree::met() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_met_ && b_met_){
    b_met_->GetEntry(entry_);
    c_met_ = true;
  }
  return met_;
}

float const & small_tree::met_phi() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_met_phi_ && b_met_phi_){
    b_met_phi_->GetEntry(entry_);
    c_met_phi_ = true;
  }
  return met_phi_;
}

float const & small_tree::mht() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mht_ && b_mht_){
    b_mht_->GetEntry(entry_);
    c_mht_ = true;
  }
  return mht_;
}

float const & small_tree::min_mt_bmet() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_min_mt_bmet_ && b_min_mt_bmet_){
    b_min_mt_bmet_->GetEntry(entry_);
    c_min_mt_bmet_ = true;
  }
  return min_mt_bmet_;
}

float const & small_tree::min_mt_bmet_with_w_mass() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_min_mt_bmet_with_w_mass_ && b_min_mt_bmet_with_w_mass_){
    b_min_mt_bmet_with_w_mass_->GetEntry(entry_);
    c_min_mt_bmet_with_w_mass_ = true;
  }
  return min_mt_bmet_with_w_mass_;
}

float const & small_tree::mindphin_metjet() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mindphin_metjet_ && b_mindphin_metjet_){
    b_mindphin_metjet_->GetEntry(entry_);
    c_mindphin_metjet_ = true;
  }
  return mindphin_metjet_;
}

float const & small_tree::mj() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mj_ && b_mj_){
    b_mj_->GetEntry(entry_);
    c_mj_ = true;
  }
  return mj_;
}

float const & small_tree::mt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mt_ && b_mt_){
    b_mt_->GetEntry(entry_);
    c_mt_ = true;
  }
  return mt_;
}

float const & small_tree::mt_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mt_reliso_ && b_mt_reliso_){
    b_mt_reliso_->GetEntry(entry_);
    c_mt_reliso_ = true;
  }
  return mt_reliso_;
}

float const & small_tree::ntrupv_mean() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_ntrupv_mean_ && b_ntrupv_mean_){
    b_ntrupv_mean_->GetEntry(entry_);
    c_ntrupv_mean_ = true;
  }
  return ntrupv_mean_;
}

float const & small_tree::st() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_st_ && b_st_){
    b_st_->GetEntry(entry_);
    c_st_ = true;
  }
  return st_;
}

float const & small_tree::st_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_st_reliso_ && b_st_reliso_){
    b_st_reliso_->GetEntry(entry_);
    c_st_reliso_ = true;
  }
  return st_reliso_;
}

float const & small_tree::weight() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_weight_ && b_weight_){
    b_weight_->GetEntry(entry_);
    c_weight_ = true;
  }
  return weight_;
}

int const & small_tree::event() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_event_ && b_event_){
    b_event_->GetEntry(entry_);
    c_event_ = true;
  }
  return event_;
}

int const & small_tree::lumiblock() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_lumiblock_ && b_lumiblock_){
    b_lumiblock_->GetEntry(entry_);
    c_lumiblock_ = true;
  }
  return lumiblock_;
}

int const & small_tree::nbl() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nbl_ && b_nbl_){
    b_nbl_->GetEntry(entry_);
    c_nbl_ = true;
  }
  return nbl_;
}

int const & small_tree::nbm() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nbm_ && b_nbm_){
    b_nbm_->GetEntry(entry_);
    c_nbm_ = true;
  }
  return nbm_;
}

int const & small_tree::nbt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nbt_ && b_nbt_){
    b_nbt_->GetEntry(entry_);
    c_nbt_ = true;
  }
  return nbt_;
}

int const & small_tree::nels() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nels_ && b_nels_){
    b_nels_->GetEntry(entry_);
    c_nels_ = true;
  }
  return nels_;
}

int const & small_tree::nels_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nels_reliso_ && b_nels_reliso_){
    b_nels_reliso_->GetEntry(entry_);
    c_nels_reliso_ = true;
  }
  return nels_reliso_;
}

int const & small_tree::nfjets() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nfjets_ && b_nfjets_){
    b_nfjets_->GetEntry(entry_);
    c_nfjets_ = true;
  }
  return nfjets_;
}

int const & small_tree::njets() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_njets_ && b_njets_){
    b_njets_->GetEntry(entry_);
    c_njets_ = true;
  }
  return njets_;
}

int const & small_tree::nleps() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nleps_ && b_nleps_){
    b_nleps_->GetEntry(entry_);
    c_nleps_ = true;
  }
  return nleps_;
}

int const & small_tree::nleps_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nleps_reliso_ && b_nleps_reliso_){
    b_nleps_reliso_->GetEntry(entry_);
    c_nleps_reliso_ = true;
  }
  return nleps_reliso_;
}

int const & small_tree::nmus() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nmus_ && b_nmus_){
    b_nmus_->GetEntry(entry_);
    c_nmus_ = true;
  }
  return nmus_;
}

int const & small_tree::nmus_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nmus_reliso_ && b_nmus_reliso_){
    b_nmus_reliso_->GetEntry(entry_);
    c_nmus_reliso_ = true;
  }
  return nmus_reliso_;
}

int const & small_tree::npv() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_npv_ && b_npv_){
    b_npv_->GetEntry(entry_);
    c_npv_ = true;
  }
  return npv_;
}

int const & small_tree::ntks() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_ntks_ && b_ntks_){
    b_ntks_->GetEntry(entry_);
    c_ntks_ = true;
  }
  return ntks_;
}

int const & small_tree::ntks_chg() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_ntks_chg_ && b_ntks_chg_){
    b_ntks_chg_->GetEntry(entry_);
    c_ntks_chg_ = true;
  }
  return ntks_chg_;
}

int const & small_tree::ntks_chg_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_ntks_chg_reliso_ && b_ntks_chg_reliso_){
    b_ntks_chg_reliso_->GetEntry(entry_);
    c_ntks_chg_reliso_ = true;
  }
  return ntks_chg_reliso_;
}

int const & small_tree::ntrupv() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_ntrupv_ && b_ntrupv_){
    b_ntrupv_->GetEntry(entry_);
    c_ntrupv_ = true;
  }
  return ntrupv_;
}

int const & small_tree::nvels() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nvels_ && b_nvels_){
    b_nvels_->GetEntry(entry_);
    c_nvels_ = true;
  }
  return nvels_;
}

int const & small_tree::nvels_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nvels_reliso_ && b_nvels_reliso_){
    b_nvels_reliso_->GetEntry(entry_);
    c_nvels_reliso_ = true;
  }
  return nvels_reliso_;
}

int const & small_tree::nvmus() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nvmus_ && b_nvmus_){
    b_nvmus_->GetEntry(entry_);
    c_nvmus_ = true;
  }
  return nvmus_;
}

int const & small_tree::nvmus_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_nvmus_reliso_ && b_nvmus_reliso_){
    b_nvmus_reliso_->GetEntry(entry_);
    c_nvmus_reliso_ = true;
  }
  return nvmus_reliso_;
}

int const & small_tree::run() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_run_ && b_run_){
    b_run_->GetEntry(entry_);
    c_run_ = true;
  }
  return run_;
}

std::vector<bool> const & small_tree::els_ispf() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_ispf_ && b_els_ispf_){
    b_els_ispf_->GetEntry(entry_);
    c_els_ispf_ = true;
  }
  return els_ispf_;
}

std::vector<bool> const & small_tree::els_sigid() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_sigid_ && b_els_sigid_){
    b_els_sigid_->GetEntry(entry_);
    c_els_sigid_ = true;
  }
  return els_sigid_;
}

std::vector<bool> const & small_tree::els_tru_tm() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_tru_tm_ && b_els_tru_tm_){
    b_els_tru_tm_->GetEntry(entry_);
    c_els_tru_tm_ = true;
  }
  return els_tru_tm_;
}

std::vector<bool> const & small_tree::mus_sigid() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_sigid_ && b_mus_sigid_){
    b_mus_sigid_->GetEntry(entry_);
    c_mus_sigid_ = true;
  }
  return mus_sigid_;
}

std::vector<bool> const & small_tree::mus_tru_tm() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_tru_tm_ && b_mus_tru_tm_){
    b_mus_tru_tm_->GetEntry(entry_);
    c_mus_tru_tm_ = true;
  }
  return mus_tru_tm_;
}

std::vector<bool> const & small_tree::tks_from_tau() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_from_tau_ && b_tks_from_tau_){
    b_tks_from_tau_->GetEntry(entry_);
    c_tks_from_tau_ = true;
  }
  return tks_from_tau_;
}

std::vector<bool> const & small_tree::tks_from_tauhad() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_from_tauhad_ && b_tks_from_tauhad_){
    b_tks_from_tauhad_->GetEntry(entry_);
    c_tks_from_tauhad_ = true;
  }
  return tks_from_tauhad_;
}

std::vector<bool> const & small_tree::tks_from_taulep() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_from_taulep_ && b_tks_from_taulep_){
    b_tks_from_taulep_->GetEntry(entry_);
    c_tks_from_taulep_ = true;
  }
  return tks_from_taulep_;
}

std::vector<bool> const & small_tree::tks_from_w() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_from_w_ && b_tks_from_w_){
    b_tks_from_w_->GetEntry(entry_);
    c_tks_from_w_ = true;
  }
  return tks_from_w_;
}

std::vector<bool> const & small_tree::tks_is_primary() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_is_primary_ && b_tks_is_primary_){
    b_tks_is_primary_->GetEntry(entry_);
    c_tks_is_primary_ = true;
  }
  return tks_is_primary_;
}

std::vector<bool> const & small_tree::tks_is_primary_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_is_primary_reliso_ && b_tks_is_primary_reliso_){
    b_tks_is_primary_reliso_->GetEntry(entry_);
    c_tks_is_primary_reliso_ = true;
  }
  return tks_is_primary_reliso_;
}

std::vector<bool> const & small_tree::tks_is_sig_lep() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_is_sig_lep_ && b_tks_is_sig_lep_){
    b_tks_is_sig_lep_->GetEntry(entry_);
    c_tks_is_sig_lep_ = true;
  }
  return tks_is_sig_lep_;
}

std::vector<float> const & small_tree::els_d0() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_d0_ && b_els_d0_){
    b_els_d0_->GetEntry(entry_);
    c_els_d0_ = true;
  }
  return els_d0_;
}

std::vector<float> const & small_tree::els_dz() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_dz_ && b_els_dz_){
    b_els_dz_->GetEntry(entry_);
    c_els_dz_ = true;
  }
  return els_dz_;
}

std::vector<float> const & small_tree::els_eta() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_eta_ && b_els_eta_){
    b_els_eta_->GetEntry(entry_);
    c_els_eta_ = true;
  }
  return els_eta_;
}

std::vector<float> const & small_tree::els_miniso_10() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_miniso_10_ && b_els_miniso_10_){
    b_els_miniso_10_->GetEntry(entry_);
    c_els_miniso_10_ = true;
  }
  return els_miniso_10_;
}

std::vector<float> const & small_tree::els_miniso_10_ch() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_miniso_10_ch_ && b_els_miniso_10_ch_){
    b_els_miniso_10_ch_->GetEntry(entry_);
    c_els_miniso_10_ch_ = true;
  }
  return els_miniso_10_ch_;
}

std::vector<float> const & small_tree::els_miniso_10_pfpu() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_miniso_10_pfpu_ && b_els_miniso_10_pfpu_){
    b_els_miniso_10_pfpu_->GetEntry(entry_);
    c_els_miniso_10_pfpu_ = true;
  }
  return els_miniso_10_pfpu_;
}

std::vector<float> const & small_tree::els_miniso_tr10() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_miniso_tr10_ && b_els_miniso_tr10_){
    b_els_miniso_tr10_->GetEntry(entry_);
    c_els_miniso_tr10_ = true;
  }
  return els_miniso_tr10_;
}

std::vector<float> const & small_tree::els_miniso_tr10_ch() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_miniso_tr10_ch_ && b_els_miniso_tr10_ch_){
    b_els_miniso_tr10_ch_->GetEntry(entry_);
    c_els_miniso_tr10_ch_ = true;
  }
  return els_miniso_tr10_ch_;
}

std::vector<float> const & small_tree::els_miniso_tr10_pfpu() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_miniso_tr10_pfpu_ && b_els_miniso_tr10_pfpu_){
    b_els_miniso_tr10_pfpu_->GetEntry(entry_);
    c_els_miniso_tr10_pfpu_ = true;
  }
  return els_miniso_tr10_pfpu_;
}

std::vector<float> const & small_tree::els_mt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_mt_ && b_els_mt_){
    b_els_mt_->GetEntry(entry_);
    c_els_mt_ = true;
  }
  return els_mt_;
}

std::vector<float> const & small_tree::els_phi() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_phi_ && b_els_phi_){
    b_els_phi_->GetEntry(entry_);
    c_els_phi_ = true;
  }
  return els_phi_;
}

std::vector<float> const & small_tree::els_pt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_pt_ && b_els_pt_){
    b_els_pt_->GetEntry(entry_);
    c_els_pt_ = true;
  }
  return els_pt_;
}

std::vector<float> const & small_tree::els_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_reliso_ && b_els_reliso_){
    b_els_reliso_->GetEntry(entry_);
    c_els_reliso_ = true;
  }
  return els_reliso_;
}

std::vector<float> const & small_tree::els_reliso_r01() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_reliso_r01_ && b_els_reliso_r01_){
    b_els_reliso_r01_->GetEntry(entry_);
    c_els_reliso_r01_ = true;
  }
  return els_reliso_r01_;
}

std::vector<float> const & small_tree::els_reliso_r015() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_reliso_r015_ && b_els_reliso_r015_){
    b_els_reliso_r015_->GetEntry(entry_);
    c_els_reliso_r015_ = true;
  }
  return els_reliso_r015_;
}

std::vector<float> const & small_tree::els_reliso_r02() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_reliso_r02_ && b_els_reliso_r02_){
    b_els_reliso_r02_->GetEntry(entry_);
    c_els_reliso_r02_ = true;
  }
  return els_reliso_r02_;
}

std::vector<float> const & small_tree::els_reliso_r03() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_reliso_r03_ && b_els_reliso_r03_){
    b_els_reliso_r03_->GetEntry(entry_);
    c_els_reliso_r03_ = true;
  }
  return els_reliso_r03_;
}

std::vector<float> const & small_tree::els_reliso_r04() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_reliso_r04_ && b_els_reliso_r04_){
    b_els_reliso_r04_->GetEntry(entry_);
    c_els_reliso_r04_ = true;
  }
  return els_reliso_r04_;
}

std::vector<float> const & small_tree::els_tru_dr() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_tru_dr_ && b_els_tru_dr_){
    b_els_tru_dr_->GetEntry(entry_);
    c_els_tru_dr_ = true;
  }
  return els_tru_dr_;
}

std::vector<float> const & small_tree::fjets_eta() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_fjets_eta_ && b_fjets_eta_){
    b_fjets_eta_->GetEntry(entry_);
    c_fjets_eta_ = true;
  }
  return fjets_eta_;
}

std::vector<float> const & small_tree::fjets_m() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_fjets_m_ && b_fjets_m_){
    b_fjets_m_->GetEntry(entry_);
    c_fjets_m_ = true;
  }
  return fjets_m_;
}

std::vector<float> const & small_tree::fjets_nconst() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_fjets_nconst_ && b_fjets_nconst_){
    b_fjets_nconst_->GetEntry(entry_);
    c_fjets_nconst_ = true;
  }
  return fjets_nconst_;
}

std::vector<float> const & small_tree::fjets_phi() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_fjets_phi_ && b_fjets_phi_){
    b_fjets_phi_->GetEntry(entry_);
    c_fjets_phi_ = true;
  }
  return fjets_phi_;
}

std::vector<float> const & small_tree::fjets_pt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_fjets_pt_ && b_fjets_pt_){
    b_fjets_pt_->GetEntry(entry_);
    c_fjets_pt_ = true;
  }
  return fjets_pt_;
}

std::vector<float> const & small_tree::jets_csv() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_jets_csv_ && b_jets_csv_){
    b_jets_csv_->GetEntry(entry_);
    c_jets_csv_ = true;
  }
  return jets_csv_;
}

std::vector<float> const & small_tree::jets_eta() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_jets_eta_ && b_jets_eta_){
    b_jets_eta_->GetEntry(entry_);
    c_jets_eta_ = true;
  }
  return jets_eta_;
}

std::vector<float> const & small_tree::jets_id() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_jets_id_ && b_jets_id_){
    b_jets_id_->GetEntry(entry_);
    c_jets_id_ = true;
  }
  return jets_id_;
}

std::vector<float> const & small_tree::jets_islep() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_jets_islep_ && b_jets_islep_){
    b_jets_islep_->GetEntry(entry_);
    c_jets_islep_ = true;
  }
  return jets_islep_;
}

std::vector<float> const & small_tree::jets_phi() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_jets_phi_ && b_jets_phi_){
    b_jets_phi_->GetEntry(entry_);
    c_jets_phi_ = true;
  }
  return jets_phi_;
}

std::vector<float> const & small_tree::jets_pt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_jets_pt_ && b_jets_pt_){
    b_jets_pt_->GetEntry(entry_);
    c_jets_pt_ = true;
  }
  return jets_pt_;
}

std::vector<float> const & small_tree::mc_eta() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mc_eta_ && b_mc_eta_){
    b_mc_eta_->GetEntry(entry_);
    c_mc_eta_ = true;
  }
  return mc_eta_;
}

std::vector<float> const & small_tree::mc_phi() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mc_phi_ && b_mc_phi_){
    b_mc_phi_->GetEntry(entry_);
    c_mc_phi_ = true;
  }
  return mc_phi_;
}

std::vector<float> const & small_tree::mc_pt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mc_pt_ && b_mc_pt_){
    b_mc_pt_->GetEntry(entry_);
    c_mc_pt_ = true;
  }
  return mc_pt_;
}

std::vector<float> const & small_tree::mus_d0() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_d0_ && b_mus_d0_){
    b_mus_d0_->GetEntry(entry_);
    c_mus_d0_ = true;
  }
  return mus_d0_;
}

std::vector<float> const & small_tree::mus_dz() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_dz_ && b_mus_dz_){
    b_mus_dz_->GetEntry(entry_);
    c_mus_dz_ = true;
  }
  return mus_dz_;
}

std::vector<float> const & small_tree::mus_eta() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_eta_ && b_mus_eta_){
    b_mus_eta_->GetEntry(entry_);
    c_mus_eta_ = true;
  }
  return mus_eta_;
}

std::vector<float> const & small_tree::mus_miniso_10() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_miniso_10_ && b_mus_miniso_10_){
    b_mus_miniso_10_->GetEntry(entry_);
    c_mus_miniso_10_ = true;
  }
  return mus_miniso_10_;
}

std::vector<float> const & small_tree::mus_miniso_10_ch() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_miniso_10_ch_ && b_mus_miniso_10_ch_){
    b_mus_miniso_10_ch_->GetEntry(entry_);
    c_mus_miniso_10_ch_ = true;
  }
  return mus_miniso_10_ch_;
}

std::vector<float> const & small_tree::mus_miniso_10_pfpu() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_miniso_10_pfpu_ && b_mus_miniso_10_pfpu_){
    b_mus_miniso_10_pfpu_->GetEntry(entry_);
    c_mus_miniso_10_pfpu_ = true;
  }
  return mus_miniso_10_pfpu_;
}

std::vector<float> const & small_tree::mus_miniso_tr10() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_miniso_tr10_ && b_mus_miniso_tr10_){
    b_mus_miniso_tr10_->GetEntry(entry_);
    c_mus_miniso_tr10_ = true;
  }
  return mus_miniso_tr10_;
}

std::vector<float> const & small_tree::mus_miniso_tr10_ch() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_miniso_tr10_ch_ && b_mus_miniso_tr10_ch_){
    b_mus_miniso_tr10_ch_->GetEntry(entry_);
    c_mus_miniso_tr10_ch_ = true;
  }
  return mus_miniso_tr10_ch_;
}

std::vector<float> const & small_tree::mus_miniso_tr10_pfpu() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_miniso_tr10_pfpu_ && b_mus_miniso_tr10_pfpu_){
    b_mus_miniso_tr10_pfpu_->GetEntry(entry_);
    c_mus_miniso_tr10_pfpu_ = true;
  }
  return mus_miniso_tr10_pfpu_;
}

std::vector<float> const & small_tree::mus_mt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_mt_ && b_mus_mt_){
    b_mus_mt_->GetEntry(entry_);
    c_mus_mt_ = true;
  }
  return mus_mt_;
}

std::vector<float> const & small_tree::mus_phi() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_phi_ && b_mus_phi_){
    b_mus_phi_->GetEntry(entry_);
    c_mus_phi_ = true;
  }
  return mus_phi_;
}

std::vector<float> const & small_tree::mus_pt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_pt_ && b_mus_pt_){
    b_mus_pt_->GetEntry(entry_);
    c_mus_pt_ = true;
  }
  return mus_pt_;
}

std::vector<float> const & small_tree::mus_reliso() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_reliso_ && b_mus_reliso_){
    b_mus_reliso_->GetEntry(entry_);
    c_mus_reliso_ = true;
  }
  return mus_reliso_;
}

std::vector<float> const & small_tree::mus_reliso_r01() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_reliso_r01_ && b_mus_reliso_r01_){
    b_mus_reliso_r01_->GetEntry(entry_);
    c_mus_reliso_r01_ = true;
  }
  return mus_reliso_r01_;
}

std::vector<float> const & small_tree::mus_reliso_r015() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_reliso_r015_ && b_mus_reliso_r015_){
    b_mus_reliso_r015_->GetEntry(entry_);
    c_mus_reliso_r015_ = true;
  }
  return mus_reliso_r015_;
}

std::vector<float> const & small_tree::mus_reliso_r02() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_reliso_r02_ && b_mus_reliso_r02_){
    b_mus_reliso_r02_->GetEntry(entry_);
    c_mus_reliso_r02_ = true;
  }
  return mus_reliso_r02_;
}

std::vector<float> const & small_tree::mus_reliso_r03() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_reliso_r03_ && b_mus_reliso_r03_){
    b_mus_reliso_r03_->GetEntry(entry_);
    c_mus_reliso_r03_ = true;
  }
  return mus_reliso_r03_;
}

std::vector<float> const & small_tree::mus_reliso_r04() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_reliso_r04_ && b_mus_reliso_r04_){
    b_mus_reliso_r04_->GetEntry(entry_);
    c_mus_reliso_r04_ = true;
  }
  return mus_reliso_r04_;
}

std::vector<float> const & small_tree::mus_tru_dr() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_tru_dr_ && b_mus_tru_dr_){
    b_mus_tru_dr_->GetEntry(entry_);
    c_mus_tru_dr_ = true;
  }
  return mus_tru_dr_;
}

std::vector<float> const & small_tree::tks_eta() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_eta_ && b_tks_eta_){
    b_tks_eta_->GetEntry(entry_);
    c_tks_eta_ = true;
  }
  return tks_eta_;
}

std::vector<float> const & small_tree::tks_mini_ch() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_mini_ch_ && b_tks_mini_ch_){
    b_tks_mini_ch_->GetEntry(entry_);
    c_tks_mini_ch_ = true;
  }
  return tks_mini_ch_;
}

std::vector<float> const & small_tree::tks_mini_ne() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_mini_ne_ && b_tks_mini_ne_){
    b_tks_mini_ne_->GetEntry(entry_);
    c_tks_mini_ne_ = true;
  }
  return tks_mini_ne_;
}

std::vector<float> const & small_tree::tks_mt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_mt_ && b_tks_mt_){
    b_tks_mt_->GetEntry(entry_);
    c_tks_mt_ = true;
  }
  return tks_mt_;
}

std::vector<float> const & small_tree::tks_phi() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_phi_ && b_tks_phi_){
    b_tks_phi_->GetEntry(entry_);
    c_tks_phi_ = true;
  }
  return tks_phi_;
}

std::vector<float> const & small_tree::tks_pt() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_pt_ && b_tks_pt_){
    b_tks_pt_->GetEntry(entry_);
    c_tks_pt_ = true;
  }
  return tks_pt_;
}

std::vector<float> const & small_tree::tks_r02_ch() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_r02_ch_ && b_tks_r02_ch_){
    b_tks_r02_ch_->GetEntry(entry_);
    c_tks_r02_ch_ = true;
  }
  return tks_r02_ch_;
}

std::vector<float> const & small_tree::tks_r02_ne() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_r02_ne_ && b_tks_r02_ne_){
    b_tks_r02_ne_->GetEntry(entry_);
    c_tks_r02_ne_ = true;
  }
  return tks_r02_ne_;
}

std::vector<float> const & small_tree::tks_r03_ch() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_r03_ch_ && b_tks_r03_ch_){
    b_tks_r03_ch_->GetEntry(entry_);
    c_tks_r03_ch_ = true;
  }
  return tks_r03_ch_;
}

std::vector<float> const & small_tree::tks_r03_ne() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_r03_ne_ && b_tks_r03_ne_){
    b_tks_r03_ne_->GetEntry(entry_);
    c_tks_r03_ne_ = true;
  }
  return tks_r03_ne_;
}

std::vector<float> const & small_tree::tks_tru_dp() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_tru_dp_ && b_tks_tru_dp_){
    b_tks_tru_dp_->GetEntry(entry_);
    c_tks_tru_dp_ = true;
  }
  return tks_tru_dp_;
}

std::vector<float> const & small_tree::tks_tru_dr() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_tru_dr_ && b_tks_tru_dr_){
    b_tks_tru_dr_->GetEntry(entry_);
    c_tks_tru_dr_ = true;
  }
  return tks_tru_dr_;
}

std::vector<int> const & small_tree::els_charge() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_charge_ && b_els_charge_){
    b_els_charge_->GetEntry(entry_);
    c_els_charge_ = true;
  }
  return els_charge_;
}

std::vector<int> const & small_tree::els_tru_id() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_tru_id_ && b_els_tru_id_){
    b_els_tru_id_->GetEntry(entry_);
    c_els_tru_id_ = true;
  }
  return els_tru_id_;
}

std::vector<int> const & small_tree::els_tru_momid() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_els_tru_momid_ && b_els_tru_momid_){
    b_els_tru_momid_->GetEntry(entry_);
    c_els_tru_momid_ = true;
  }
  return els_tru_momid_;
}

std::vector<int> const & small_tree::mc_id() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mc_id_ && b_mc_id_){
    b_mc_id_->GetEntry(entry_);
    c_mc_id_ = true;
  }
  return mc_id_;
}

std::vector<int> const & small_tree::mc_status() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mc_status_ && b_mc_status_){
    b_mc_status_->GetEntry(entry_);
    c_mc_status_ = true;
  }
  return mc_status_;
}

std::vector<int> const & small_tree::mus_charge() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_charge_ && b_mus_charge_){
    b_mus_charge_->GetEntry(entry_);
    c_mus_charge_ = true;
  }
  return mus_charge_;
}

std::vector<int> const & small_tree::mus_tru_id() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_tru_id_ && b_mus_tru_id_){
    b_mus_tru_id_->GetEntry(entry_);
    c_mus_tru_id_ = true;
  }
  return mus_tru_id_;
}

std::vector<int> const & small_tree::mus_tru_momid() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mus_tru_momid_ && b_mus_tru_momid_){
    b_mus_tru_momid_->GetEntry(entry_);
    c_mus_tru_momid_ = true;
  }
  return mus_tru_momid_;
}

std::vector<int> const & small_tree::tks_from_pv() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_from_pv_ && b_tks_from_pv_){
    b_tks_from_pv_->GetEntry(entry_);
    c_tks_from_pv_ = true;
  }
  return tks_from_pv_;
}

std::vector<int> const & small_tree::tks_id() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_id_ && b_tks_id_){
    b_tks_id_->GetEntry(entry_);
    c_tks_id_ = true;
  }
  return tks_id_;
}

std::vector<int> const & small_tree::tks_num_prongs() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_num_prongs_ && b_tks_num_prongs_){
    b_tks_num_prongs_->GetEntry(entry_);
    c_tks_num_prongs_ = true;
  }
  return tks_num_prongs_;
}

std::vector<int> const & small_tree::tks_tru_id() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_tks_tru_id_ && b_tks_tru_id_){
    b_tks_tru_id_->GetEntry(entry_);
    c_tks_tru_id_ = true;
  }
  return tks_tru_id_;
}

std::vector<size_t> const & small_tree::mc_mom() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mc_mom_ && b_mc_mom_){
    b_mc_mom_->GetEntry(entry_);
    c_mc_mom_ = true;
  }
  return mc_mom_;
}

unsigned const & small_tree::mc_type() const{
  if(!read_only_){
    throw std::logic_error("Trying to write to const tree.");
  }
  if(!c_mc_type_ && b_mc_type_){
    b_mc_type_->GetEntry(entry_);
    c_mc_type_ = true;
  }
  return mc_type_;
}

float & small_tree::dphi_wlep(){
  if(read_only_ && !c_dphi_wlep_ && b_dphi_wlep_){
    b_dphi_wlep_->GetEntry(entry_);
    c_dphi_wlep_ = true;
  }
  return dphi_wlep_;
}

float & small_tree::dphi_wlep_reliso(){
  if(read_only_ && !c_dphi_wlep_reliso_ && b_dphi_wlep_reliso_){
    b_dphi_wlep_reliso_->GetEntry(entry_);
    c_dphi_wlep_reliso_ = true;
  }
  return dphi_wlep_reliso_;
}

float & small_tree::ht(){
  if(read_only_ && !c_ht_ && b_ht_){
    b_ht_->GetEntry(entry_);
    c_ht_ = true;
  }
  return ht_;
}

float & small_tree::lep_pt(){
  if(read_only_ && !c_lep_pt_ && b_lep_pt_){
    b_lep_pt_->GetEntry(entry_);
    c_lep_pt_ = true;
  }
  return lep_pt_;
}

float & small_tree::lep_pt_reliso(){
  if(read_only_ && !c_lep_pt_reliso_ && b_lep_pt_reliso_){
    b_lep_pt_reliso_->GetEntry(entry_);
    c_lep_pt_reliso_ = true;
  }
  return lep_pt_reliso_;
}

float & small_tree::met(){
  if(read_only_ && !c_met_ && b_met_){
    b_met_->GetEntry(entry_);
    c_met_ = true;
  }
  return met_;
}

float & small_tree::met_phi(){
  if(read_only_ && !c_met_phi_ && b_met_phi_){
    b_met_phi_->GetEntry(entry_);
    c_met_phi_ = true;
  }
  return met_phi_;
}

float & small_tree::mht(){
  if(read_only_ && !c_mht_ && b_mht_){
    b_mht_->GetEntry(entry_);
    c_mht_ = true;
  }
  return mht_;
}

float & small_tree::min_mt_bmet(){
  if(read_only_ && !c_min_mt_bmet_ && b_min_mt_bmet_){
    b_min_mt_bmet_->GetEntry(entry_);
    c_min_mt_bmet_ = true;
  }
  return min_mt_bmet_;
}

float & small_tree::min_mt_bmet_with_w_mass(){
  if(read_only_ && !c_min_mt_bmet_with_w_mass_ && b_min_mt_bmet_with_w_mass_){
    b_min_mt_bmet_with_w_mass_->GetEntry(entry_);
    c_min_mt_bmet_with_w_mass_ = true;
  }
  return min_mt_bmet_with_w_mass_;
}

float & small_tree::mindphin_metjet(){
  if(read_only_ && !c_mindphin_metjet_ && b_mindphin_metjet_){
    b_mindphin_metjet_->GetEntry(entry_);
    c_mindphin_metjet_ = true;
  }
  return mindphin_metjet_;
}

float & small_tree::mj(){
  if(read_only_ && !c_mj_ && b_mj_){
    b_mj_->GetEntry(entry_);
    c_mj_ = true;
  }
  return mj_;
}

float & small_tree::mt(){
  if(read_only_ && !c_mt_ && b_mt_){
    b_mt_->GetEntry(entry_);
    c_mt_ = true;
  }
  return mt_;
}

float & small_tree::mt_reliso(){
  if(read_only_ && !c_mt_reliso_ && b_mt_reliso_){
    b_mt_reliso_->GetEntry(entry_);
    c_mt_reliso_ = true;
  }
  return mt_reliso_;
}

float & small_tree::ntrupv_mean(){
  if(read_only_ && !c_ntrupv_mean_ && b_ntrupv_mean_){
    b_ntrupv_mean_->GetEntry(entry_);
    c_ntrupv_mean_ = true;
  }
  return ntrupv_mean_;
}

float & small_tree::st(){
  if(read_only_ && !c_st_ && b_st_){
    b_st_->GetEntry(entry_);
    c_st_ = true;
  }
  return st_;
}

float & small_tree::st_reliso(){
  if(read_only_ && !c_st_reliso_ && b_st_reliso_){
    b_st_reliso_->GetEntry(entry_);
    c_st_reliso_ = true;
  }
  return st_reliso_;
}

float & small_tree::weight(){
  if(read_only_ && !c_weight_ && b_weight_){
    b_weight_->GetEntry(entry_);
    c_weight_ = true;
  }
  return weight_;
}

int & small_tree::event(){
  if(read_only_ && !c_event_ && b_event_){
    b_event_->GetEntry(entry_);
    c_event_ = true;
  }
  return event_;
}

int & small_tree::lumiblock(){
  if(read_only_ && !c_lumiblock_ && b_lumiblock_){
    b_lumiblock_->GetEntry(entry_);
    c_lumiblock_ = true;
  }
  return lumiblock_;
}

int & small_tree::nbl(){
  if(read_only_ && !c_nbl_ && b_nbl_){
    b_nbl_->GetEntry(entry_);
    c_nbl_ = true;
  }
  return nbl_;
}

int & small_tree::nbm(){
  if(read_only_ && !c_nbm_ && b_nbm_){
    b_nbm_->GetEntry(entry_);
    c_nbm_ = true;
  }
  return nbm_;
}

int & small_tree::nbt(){
  if(read_only_ && !c_nbt_ && b_nbt_){
    b_nbt_->GetEntry(entry_);
    c_nbt_ = true;
  }
  return nbt_;
}

int & small_tree::nels(){
  if(read_only_ && !c_nels_ && b_nels_){
    b_nels_->GetEntry(entry_);
    c_nels_ = true;
  }
  return nels_;
}

int & small_tree::nels_reliso(){
  if(read_only_ && !c_nels_reliso_ && b_nels_reliso_){
    b_nels_reliso_->GetEntry(entry_);
    c_nels_reliso_ = true;
  }
  return nels_reliso_;
}

int & small_tree::nfjets(){
  if(read_only_ && !c_nfjets_ && b_nfjets_){
    b_nfjets_->GetEntry(entry_);
    c_nfjets_ = true;
  }
  return nfjets_;
}

int & small_tree::njets(){
  if(read_only_ && !c_njets_ && b_njets_){
    b_njets_->GetEntry(entry_);
    c_njets_ = true;
  }
  return njets_;
}

int & small_tree::nleps(){
  if(read_only_ && !c_nleps_ && b_nleps_){
    b_nleps_->GetEntry(entry_);
    c_nleps_ = true;
  }
  return nleps_;
}

int & small_tree::nleps_reliso(){
  if(read_only_ && !c_nleps_reliso_ && b_nleps_reliso_){
    b_nleps_reliso_->GetEntry(entry_);
    c_nleps_reliso_ = true;
  }
  return nleps_reliso_;
}

int & small_tree::nmus(){
  if(read_only_ && !c_nmus_ && b_nmus_){
    b_nmus_->GetEntry(entry_);
    c_nmus_ = true;
  }
  return nmus_;
}

int & small_tree::nmus_reliso(){
  if(read_only_ && !c_nmus_reliso_ && b_nmus_reliso_){
    b_nmus_reliso_->GetEntry(entry_);
    c_nmus_reliso_ = true;
  }
  return nmus_reliso_;
}

int & small_tree::npv(){
  if(read_only_ && !c_npv_ && b_npv_){
    b_npv_->GetEntry(entry_);
    c_npv_ = true;
  }
  return npv_;
}

int & small_tree::ntks(){
  if(read_only_ && !c_ntks_ && b_ntks_){
    b_ntks_->GetEntry(entry_);
    c_ntks_ = true;
  }
  return ntks_;
}

int & small_tree::ntks_chg(){
  if(read_only_ && !c_ntks_chg_ && b_ntks_chg_){
    b_ntks_chg_->GetEntry(entry_);
    c_ntks_chg_ = true;
  }
  return ntks_chg_;
}

int & small_tree::ntks_chg_reliso(){
  if(read_only_ && !c_ntks_chg_reliso_ && b_ntks_chg_reliso_){
    b_ntks_chg_reliso_->GetEntry(entry_);
    c_ntks_chg_reliso_ = true;
  }
  return ntks_chg_reliso_;
}

int & small_tree::ntrupv(){
  if(read_only_ && !c_ntrupv_ && b_ntrupv_){
    b_ntrupv_->GetEntry(entry_);
    c_ntrupv_ = true;
  }
  return ntrupv_;
}

int & small_tree::nvels(){
  if(read_only_ && !c_nvels_ && b_nvels_){
    b_nvels_->GetEntry(entry_);
    c_nvels_ = true;
  }
  return nvels_;
}

int & small_tree::nvels_reliso(){
  if(read_only_ && !c_nvels_reliso_ && b_nvels_reliso_){
    b_nvels_reliso_->GetEntry(entry_);
    c_nvels_reliso_ = true;
  }
  return nvels_reliso_;
}

int & small_tree::nvmus(){
  if(read_only_ && !c_nvmus_ && b_nvmus_){
    b_nvmus_->GetEntry(entry_);
    c_nvmus_ = true;
  }
  return nvmus_;
}

int & small_tree::nvmus_reliso(){
  if(read_only_ && !c_nvmus_reliso_ && b_nvmus_reliso_){
    b_nvmus_reliso_->GetEntry(entry_);
    c_nvmus_reliso_ = true;
  }
  return nvmus_reliso_;
}

int & small_tree::run(){
  if(read_only_ && !c_run_ && b_run_){
    b_run_->GetEntry(entry_);
    c_run_ = true;
  }
  return run_;
}

std::vector<bool> & small_tree::els_ispf(){
  if(read_only_ && !c_els_ispf_ && b_els_ispf_){
    b_els_ispf_->GetEntry(entry_);
    c_els_ispf_ = true;
  }
  return els_ispf_;
}

std::vector<bool> & small_tree::els_sigid(){
  if(read_only_ && !c_els_sigid_ && b_els_sigid_){
    b_els_sigid_->GetEntry(entry_);
    c_els_sigid_ = true;
  }
  return els_sigid_;
}

std::vector<bool> & small_tree::els_tru_tm(){
  if(read_only_ && !c_els_tru_tm_ && b_els_tru_tm_){
    b_els_tru_tm_->GetEntry(entry_);
    c_els_tru_tm_ = true;
  }
  return els_tru_tm_;
}

std::vector<bool> & small_tree::mus_sigid(){
  if(read_only_ && !c_mus_sigid_ && b_mus_sigid_){
    b_mus_sigid_->GetEntry(entry_);
    c_mus_sigid_ = true;
  }
  return mus_sigid_;
}

std::vector<bool> & small_tree::mus_tru_tm(){
  if(read_only_ && !c_mus_tru_tm_ && b_mus_tru_tm_){
    b_mus_tru_tm_->GetEntry(entry_);
    c_mus_tru_tm_ = true;
  }
  return mus_tru_tm_;
}

std::vector<bool> & small_tree::tks_from_tau(){
  if(read_only_ && !c_tks_from_tau_ && b_tks_from_tau_){
    b_tks_from_tau_->GetEntry(entry_);
    c_tks_from_tau_ = true;
  }
  return tks_from_tau_;
}

std::vector<bool> & small_tree::tks_from_tauhad(){
  if(read_only_ && !c_tks_from_tauhad_ && b_tks_from_tauhad_){
    b_tks_from_tauhad_->GetEntry(entry_);
    c_tks_from_tauhad_ = true;
  }
  return tks_from_tauhad_;
}

std::vector<bool> & small_tree::tks_from_taulep(){
  if(read_only_ && !c_tks_from_taulep_ && b_tks_from_taulep_){
    b_tks_from_taulep_->GetEntry(entry_);
    c_tks_from_taulep_ = true;
  }
  return tks_from_taulep_;
}

std::vector<bool> & small_tree::tks_from_w(){
  if(read_only_ && !c_tks_from_w_ && b_tks_from_w_){
    b_tks_from_w_->GetEntry(entry_);
    c_tks_from_w_ = true;
  }
  return tks_from_w_;
}

std::vector<bool> & small_tree::tks_is_primary(){
  if(read_only_ && !c_tks_is_primary_ && b_tks_is_primary_){
    b_tks_is_primary_->GetEntry(entry_);
    c_tks_is_primary_ = true;
  }
  return tks_is_primary_;
}

std::vector<bool> & small_tree::tks_is_primary_reliso(){
  if(read_only_ && !c_tks_is_primary_reliso_ && b_tks_is_primary_reliso_){
    b_tks_is_primary_reliso_->GetEntry(entry_);
    c_tks_is_primary_reliso_ = true;
  }
  return tks_is_primary_reliso_;
}

std::vector<bool> & small_tree::tks_is_sig_lep(){
  if(read_only_ && !c_tks_is_sig_lep_ && b_tks_is_sig_lep_){
    b_tks_is_sig_lep_->GetEntry(entry_);
    c_tks_is_sig_lep_ = true;
  }
  return tks_is_sig_lep_;
}

std::vector<float> & small_tree::els_d0(){
  if(read_only_ && !c_els_d0_ && b_els_d0_){
    b_els_d0_->GetEntry(entry_);
    c_els_d0_ = true;
  }
  return els_d0_;
}

std::vector<float> & small_tree::els_dz(){
  if(read_only_ && !c_els_dz_ && b_els_dz_){
    b_els_dz_->GetEntry(entry_);
    c_els_dz_ = true;
  }
  return els_dz_;
}

std::vector<float> & small_tree::els_eta(){
  if(read_only_ && !c_els_eta_ && b_els_eta_){
    b_els_eta_->GetEntry(entry_);
    c_els_eta_ = true;
  }
  return els_eta_;
}

std::vector<float> & small_tree::els_miniso_10(){
  if(read_only_ && !c_els_miniso_10_ && b_els_miniso_10_){
    b_els_miniso_10_->GetEntry(entry_);
    c_els_miniso_10_ = true;
  }
  return els_miniso_10_;
}

std::vector<float> & small_tree::els_miniso_10_ch(){
  if(read_only_ && !c_els_miniso_10_ch_ && b_els_miniso_10_ch_){
    b_els_miniso_10_ch_->GetEntry(entry_);
    c_els_miniso_10_ch_ = true;
  }
  return els_miniso_10_ch_;
}

std::vector<float> & small_tree::els_miniso_10_pfpu(){
  if(read_only_ && !c_els_miniso_10_pfpu_ && b_els_miniso_10_pfpu_){
    b_els_miniso_10_pfpu_->GetEntry(entry_);
    c_els_miniso_10_pfpu_ = true;
  }
  return els_miniso_10_pfpu_;
}

std::vector<float> & small_tree::els_miniso_tr10(){
  if(read_only_ && !c_els_miniso_tr10_ && b_els_miniso_tr10_){
    b_els_miniso_tr10_->GetEntry(entry_);
    c_els_miniso_tr10_ = true;
  }
  return els_miniso_tr10_;
}

std::vector<float> & small_tree::els_miniso_tr10_ch(){
  if(read_only_ && !c_els_miniso_tr10_ch_ && b_els_miniso_tr10_ch_){
    b_els_miniso_tr10_ch_->GetEntry(entry_);
    c_els_miniso_tr10_ch_ = true;
  }
  return els_miniso_tr10_ch_;
}

std::vector<float> & small_tree::els_miniso_tr10_pfpu(){
  if(read_only_ && !c_els_miniso_tr10_pfpu_ && b_els_miniso_tr10_pfpu_){
    b_els_miniso_tr10_pfpu_->GetEntry(entry_);
    c_els_miniso_tr10_pfpu_ = true;
  }
  return els_miniso_tr10_pfpu_;
}

std::vector<float> & small_tree::els_mt(){
  if(read_only_ && !c_els_mt_ && b_els_mt_){
    b_els_mt_->GetEntry(entry_);
    c_els_mt_ = true;
  }
  return els_mt_;
}

std::vector<float> & small_tree::els_phi(){
  if(read_only_ && !c_els_phi_ && b_els_phi_){
    b_els_phi_->GetEntry(entry_);
    c_els_phi_ = true;
  }
  return els_phi_;
}

std::vector<float> & small_tree::els_pt(){
  if(read_only_ && !c_els_pt_ && b_els_pt_){
    b_els_pt_->GetEntry(entry_);
    c_els_pt_ = true;
  }
  return els_pt_;
}

std::vector<float> & small_tree::els_reliso(){
  if(read_only_ && !c_els_reliso_ && b_els_reliso_){
    b_els_reliso_->GetEntry(entry_);
    c_els_reliso_ = true;
  }
  return els_reliso_;
}

std::vector<float> & small_tree::els_reliso_r01(){
  if(read_only_ && !c_els_reliso_r01_ && b_els_reliso_r01_){
    b_els_reliso_r01_->GetEntry(entry_);
    c_els_reliso_r01_ = true;
  }
  return els_reliso_r01_;
}

std::vector<float> & small_tree::els_reliso_r015(){
  if(read_only_ && !c_els_reliso_r015_ && b_els_reliso_r015_){
    b_els_reliso_r015_->GetEntry(entry_);
    c_els_reliso_r015_ = true;
  }
  return els_reliso_r015_;
}

std::vector<float> & small_tree::els_reliso_r02(){
  if(read_only_ && !c_els_reliso_r02_ && b_els_reliso_r02_){
    b_els_reliso_r02_->GetEntry(entry_);
    c_els_reliso_r02_ = true;
  }
  return els_reliso_r02_;
}

std::vector<float> & small_tree::els_reliso_r03(){
  if(read_only_ && !c_els_reliso_r03_ && b_els_reliso_r03_){
    b_els_reliso_r03_->GetEntry(entry_);
    c_els_reliso_r03_ = true;
  }
  return els_reliso_r03_;
}

std::vector<float> & small_tree::els_reliso_r04(){
  if(read_only_ && !c_els_reliso_r04_ && b_els_reliso_r04_){
    b_els_reliso_r04_->GetEntry(entry_);
    c_els_reliso_r04_ = true;
  }
  return els_reliso_r04_;
}

std::vector<float> & small_tree::els_tru_dr(){
  if(read_only_ && !c_els_tru_dr_ && b_els_tru_dr_){
    b_els_tru_dr_->GetEntry(entry_);
    c_els_tru_dr_ = true;
  }
  return els_tru_dr_;
}

std::vector<float> & small_tree::fjets_eta(){
  if(read_only_ && !c_fjets_eta_ && b_fjets_eta_){
    b_fjets_eta_->GetEntry(entry_);
    c_fjets_eta_ = true;
  }
  return fjets_eta_;
}

std::vector<float> & small_tree::fjets_m(){
  if(read_only_ && !c_fjets_m_ && b_fjets_m_){
    b_fjets_m_->GetEntry(entry_);
    c_fjets_m_ = true;
  }
  return fjets_m_;
}

std::vector<float> & small_tree::fjets_nconst(){
  if(read_only_ && !c_fjets_nconst_ && b_fjets_nconst_){
    b_fjets_nconst_->GetEntry(entry_);
    c_fjets_nconst_ = true;
  }
  return fjets_nconst_;
}

std::vector<float> & small_tree::fjets_phi(){
  if(read_only_ && !c_fjets_phi_ && b_fjets_phi_){
    b_fjets_phi_->GetEntry(entry_);
    c_fjets_phi_ = true;
  }
  return fjets_phi_;
}

std::vector<float> & small_tree::fjets_pt(){
  if(read_only_ && !c_fjets_pt_ && b_fjets_pt_){
    b_fjets_pt_->GetEntry(entry_);
    c_fjets_pt_ = true;
  }
  return fjets_pt_;
}

std::vector<float> & small_tree::jets_csv(){
  if(read_only_ && !c_jets_csv_ && b_jets_csv_){
    b_jets_csv_->GetEntry(entry_);
    c_jets_csv_ = true;
  }
  return jets_csv_;
}

std::vector<float> & small_tree::jets_eta(){
  if(read_only_ && !c_jets_eta_ && b_jets_eta_){
    b_jets_eta_->GetEntry(entry_);
    c_jets_eta_ = true;
  }
  return jets_eta_;
}

std::vector<float> & small_tree::jets_id(){
  if(read_only_ && !c_jets_id_ && b_jets_id_){
    b_jets_id_->GetEntry(entry_);
    c_jets_id_ = true;
  }
  return jets_id_;
}

std::vector<float> & small_tree::jets_islep(){
  if(read_only_ && !c_jets_islep_ && b_jets_islep_){
    b_jets_islep_->GetEntry(entry_);
    c_jets_islep_ = true;
  }
  return jets_islep_;
}

std::vector<float> & small_tree::jets_phi(){
  if(read_only_ && !c_jets_phi_ && b_jets_phi_){
    b_jets_phi_->GetEntry(entry_);
    c_jets_phi_ = true;
  }
  return jets_phi_;
}

std::vector<float> & small_tree::jets_pt(){
  if(read_only_ && !c_jets_pt_ && b_jets_pt_){
    b_jets_pt_->GetEntry(entry_);
    c_jets_pt_ = true;
  }
  return jets_pt_;
}

std::vector<float> & small_tree::mc_eta(){
  if(read_only_ && !c_mc_eta_ && b_mc_eta_){
    b_mc_eta_->GetEntry(entry_);
    c_mc_eta_ = true;
  }
  return mc_eta_;
}

std::vector<float> & small_tree::mc_phi(){
  if(read_only_ && !c_mc_phi_ && b_mc_phi_){
    b_mc_phi_->GetEntry(entry_);
    c_mc_phi_ = true;
  }
  return mc_phi_;
}

std::vector<float> & small_tree::mc_pt(){
  if(read_only_ && !c_mc_pt_ && b_mc_pt_){
    b_mc_pt_->GetEntry(entry_);
    c_mc_pt_ = true;
  }
  return mc_pt_;
}

std::vector<float> & small_tree::mus_d0(){
  if(read_only_ && !c_mus_d0_ && b_mus_d0_){
    b_mus_d0_->GetEntry(entry_);
    c_mus_d0_ = true;
  }
  return mus_d0_;
}

std::vector<float> & small_tree::mus_dz(){
  if(read_only_ && !c_mus_dz_ && b_mus_dz_){
    b_mus_dz_->GetEntry(entry_);
    c_mus_dz_ = true;
  }
  return mus_dz_;
}

std::vector<float> & small_tree::mus_eta(){
  if(read_only_ && !c_mus_eta_ && b_mus_eta_){
    b_mus_eta_->GetEntry(entry_);
    c_mus_eta_ = true;
  }
  return mus_eta_;
}

std::vector<float> & small_tree::mus_miniso_10(){
  if(read_only_ && !c_mus_miniso_10_ && b_mus_miniso_10_){
    b_mus_miniso_10_->GetEntry(entry_);
    c_mus_miniso_10_ = true;
  }
  return mus_miniso_10_;
}

std::vector<float> & small_tree::mus_miniso_10_ch(){
  if(read_only_ && !c_mus_miniso_10_ch_ && b_mus_miniso_10_ch_){
    b_mus_miniso_10_ch_->GetEntry(entry_);
    c_mus_miniso_10_ch_ = true;
  }
  return mus_miniso_10_ch_;
}

std::vector<float> & small_tree::mus_miniso_10_pfpu(){
  if(read_only_ && !c_mus_miniso_10_pfpu_ && b_mus_miniso_10_pfpu_){
    b_mus_miniso_10_pfpu_->GetEntry(entry_);
    c_mus_miniso_10_pfpu_ = true;
  }
  return mus_miniso_10_pfpu_;
}

std::vector<float> & small_tree::mus_miniso_tr10(){
  if(read_only_ && !c_mus_miniso_tr10_ && b_mus_miniso_tr10_){
    b_mus_miniso_tr10_->GetEntry(entry_);
    c_mus_miniso_tr10_ = true;
  }
  return mus_miniso_tr10_;
}

std::vector<float> & small_tree::mus_miniso_tr10_ch(){
  if(read_only_ && !c_mus_miniso_tr10_ch_ && b_mus_miniso_tr10_ch_){
    b_mus_miniso_tr10_ch_->GetEntry(entry_);
    c_mus_miniso_tr10_ch_ = true;
  }
  return mus_miniso_tr10_ch_;
}

std::vector<float> & small_tree::mus_miniso_tr10_pfpu(){
  if(read_only_ && !c_mus_miniso_tr10_pfpu_ && b_mus_miniso_tr10_pfpu_){
    b_mus_miniso_tr10_pfpu_->GetEntry(entry_);
    c_mus_miniso_tr10_pfpu_ = true;
  }
  return mus_miniso_tr10_pfpu_;
}

std::vector<float> & small_tree::mus_mt(){
  if(read_only_ && !c_mus_mt_ && b_mus_mt_){
    b_mus_mt_->GetEntry(entry_);
    c_mus_mt_ = true;
  }
  return mus_mt_;
}

std::vector<float> & small_tree::mus_phi(){
  if(read_only_ && !c_mus_phi_ && b_mus_phi_){
    b_mus_phi_->GetEntry(entry_);
    c_mus_phi_ = true;
  }
  return mus_phi_;
}

std::vector<float> & small_tree::mus_pt(){
  if(read_only_ && !c_mus_pt_ && b_mus_pt_){
    b_mus_pt_->GetEntry(entry_);
    c_mus_pt_ = true;
  }
  return mus_pt_;
}

std::vector<float> & small_tree::mus_reliso(){
  if(read_only_ && !c_mus_reliso_ && b_mus_reliso_){
    b_mus_reliso_->GetEntry(entry_);
    c_mus_reliso_ = true;
  }
  return mus_reliso_;
}

std::vector<float> & small_tree::mus_reliso_r01(){
  if(read_only_ && !c_mus_reliso_r01_ && b_mus_reliso_r01_){
    b_mus_reliso_r01_->GetEntry(entry_);
    c_mus_reliso_r01_ = true;
  }
  return mus_reliso_r01_;
}

std::vector<float> & small_tree::mus_reliso_r015(){
  if(read_only_ && !c_mus_reliso_r015_ && b_mus_reliso_r015_){
    b_mus_reliso_r015_->GetEntry(entry_);
    c_mus_reliso_r015_ = true;
  }
  return mus_reliso_r015_;
}

std::vector<float> & small_tree::mus_reliso_r02(){
  if(read_only_ && !c_mus_reliso_r02_ && b_mus_reliso_r02_){
    b_mus_reliso_r02_->GetEntry(entry_);
    c_mus_reliso_r02_ = true;
  }
  return mus_reliso_r02_;
}

std::vector<float> & small_tree::mus_reliso_r03(){
  if(read_only_ && !c_mus_reliso_r03_ && b_mus_reliso_r03_){
    b_mus_reliso_r03_->GetEntry(entry_);
    c_mus_reliso_r03_ = true;
  }
  return mus_reliso_r03_;
}

std::vector<float> & small_tree::mus_reliso_r04(){
  if(read_only_ && !c_mus_reliso_r04_ && b_mus_reliso_r04_){
    b_mus_reliso_r04_->GetEntry(entry_);
    c_mus_reliso_r04_ = true;
  }
  return mus_reliso_r04_;
}

std::vector<float> & small_tree::mus_tru_dr(){
  if(read_only_ && !c_mus_tru_dr_ && b_mus_tru_dr_){
    b_mus_tru_dr_->GetEntry(entry_);
    c_mus_tru_dr_ = true;
  }
  return mus_tru_dr_;
}

std::vector<float> & small_tree::tks_eta(){
  if(read_only_ && !c_tks_eta_ && b_tks_eta_){
    b_tks_eta_->GetEntry(entry_);
    c_tks_eta_ = true;
  }
  return tks_eta_;
}

std::vector<float> & small_tree::tks_mini_ch(){
  if(read_only_ && !c_tks_mini_ch_ && b_tks_mini_ch_){
    b_tks_mini_ch_->GetEntry(entry_);
    c_tks_mini_ch_ = true;
  }
  return tks_mini_ch_;
}

std::vector<float> & small_tree::tks_mini_ne(){
  if(read_only_ && !c_tks_mini_ne_ && b_tks_mini_ne_){
    b_tks_mini_ne_->GetEntry(entry_);
    c_tks_mini_ne_ = true;
  }
  return tks_mini_ne_;
}

std::vector<float> & small_tree::tks_mt(){
  if(read_only_ && !c_tks_mt_ && b_tks_mt_){
    b_tks_mt_->GetEntry(entry_);
    c_tks_mt_ = true;
  }
  return tks_mt_;
}

std::vector<float> & small_tree::tks_phi(){
  if(read_only_ && !c_tks_phi_ && b_tks_phi_){
    b_tks_phi_->GetEntry(entry_);
    c_tks_phi_ = true;
  }
  return tks_phi_;
}

std::vector<float> & small_tree::tks_pt(){
  if(read_only_ && !c_tks_pt_ && b_tks_pt_){
    b_tks_pt_->GetEntry(entry_);
    c_tks_pt_ = true;
  }
  return tks_pt_;
}

std::vector<float> & small_tree::tks_r02_ch(){
  if(read_only_ && !c_tks_r02_ch_ && b_tks_r02_ch_){
    b_tks_r02_ch_->GetEntry(entry_);
    c_tks_r02_ch_ = true;
  }
  return tks_r02_ch_;
}

std::vector<float> & small_tree::tks_r02_ne(){
  if(read_only_ && !c_tks_r02_ne_ && b_tks_r02_ne_){
    b_tks_r02_ne_->GetEntry(entry_);
    c_tks_r02_ne_ = true;
  }
  return tks_r02_ne_;
}

std::vector<float> & small_tree::tks_r03_ch(){
  if(read_only_ && !c_tks_r03_ch_ && b_tks_r03_ch_){
    b_tks_r03_ch_->GetEntry(entry_);
    c_tks_r03_ch_ = true;
  }
  return tks_r03_ch_;
}

std::vector<float> & small_tree::tks_r03_ne(){
  if(read_only_ && !c_tks_r03_ne_ && b_tks_r03_ne_){
    b_tks_r03_ne_->GetEntry(entry_);
    c_tks_r03_ne_ = true;
  }
  return tks_r03_ne_;
}

std::vector<float> & small_tree::tks_tru_dp(){
  if(read_only_ && !c_tks_tru_dp_ && b_tks_tru_dp_){
    b_tks_tru_dp_->GetEntry(entry_);
    c_tks_tru_dp_ = true;
  }
  return tks_tru_dp_;
}

std::vector<float> & small_tree::tks_tru_dr(){
  if(read_only_ && !c_tks_tru_dr_ && b_tks_tru_dr_){
    b_tks_tru_dr_->GetEntry(entry_);
    c_tks_tru_dr_ = true;
  }
  return tks_tru_dr_;
}

std::vector<int> & small_tree::els_charge(){
  if(read_only_ && !c_els_charge_ && b_els_charge_){
    b_els_charge_->GetEntry(entry_);
    c_els_charge_ = true;
  }
  return els_charge_;
}

std::vector<int> & small_tree::els_tru_id(){
  if(read_only_ && !c_els_tru_id_ && b_els_tru_id_){
    b_els_tru_id_->GetEntry(entry_);
    c_els_tru_id_ = true;
  }
  return els_tru_id_;
}

std::vector<int> & small_tree::els_tru_momid(){
  if(read_only_ && !c_els_tru_momid_ && b_els_tru_momid_){
    b_els_tru_momid_->GetEntry(entry_);
    c_els_tru_momid_ = true;
  }
  return els_tru_momid_;
}

std::vector<int> & small_tree::mc_id(){
  if(read_only_ && !c_mc_id_ && b_mc_id_){
    b_mc_id_->GetEntry(entry_);
    c_mc_id_ = true;
  }
  return mc_id_;
}

std::vector<int> & small_tree::mc_status(){
  if(read_only_ && !c_mc_status_ && b_mc_status_){
    b_mc_status_->GetEntry(entry_);
    c_mc_status_ = true;
  }
  return mc_status_;
}

std::vector<int> & small_tree::mus_charge(){
  if(read_only_ && !c_mus_charge_ && b_mus_charge_){
    b_mus_charge_->GetEntry(entry_);
    c_mus_charge_ = true;
  }
  return mus_charge_;
}

std::vector<int> & small_tree::mus_tru_id(){
  if(read_only_ && !c_mus_tru_id_ && b_mus_tru_id_){
    b_mus_tru_id_->GetEntry(entry_);
    c_mus_tru_id_ = true;
  }
  return mus_tru_id_;
}

std::vector<int> & small_tree::mus_tru_momid(){
  if(read_only_ && !c_mus_tru_momid_ && b_mus_tru_momid_){
    b_mus_tru_momid_->GetEntry(entry_);
    c_mus_tru_momid_ = true;
  }
  return mus_tru_momid_;
}

std::vector<int> & small_tree::tks_from_pv(){
  if(read_only_ && !c_tks_from_pv_ && b_tks_from_pv_){
    b_tks_from_pv_->GetEntry(entry_);
    c_tks_from_pv_ = true;
  }
  return tks_from_pv_;
}

std::vector<int> & small_tree::tks_id(){
  if(read_only_ && !c_tks_id_ && b_tks_id_){
    b_tks_id_->GetEntry(entry_);
    c_tks_id_ = true;
  }
  return tks_id_;
}

std::vector<int> & small_tree::tks_num_prongs(){
  if(read_only_ && !c_tks_num_prongs_ && b_tks_num_prongs_){
    b_tks_num_prongs_->GetEntry(entry_);
    c_tks_num_prongs_ = true;
  }
  return tks_num_prongs_;
}

std::vector<int> & small_tree::tks_tru_id(){
  if(read_only_ && !c_tks_tru_id_ && b_tks_tru_id_){
    b_tks_tru_id_->GetEntry(entry_);
    c_tks_tru_id_ = true;
  }
  return tks_tru_id_;
}

std::vector<size_t> & small_tree::mc_mom(){
  if(read_only_ && !c_mc_mom_ && b_mc_mom_){
    b_mc_mom_->GetEntry(entry_);
    c_mc_mom_ = true;
  }
  return mc_mom_;
}

unsigned & small_tree::mc_type(){
  if(read_only_ && !c_mc_type_ && b_mc_type_){
    b_mc_type_->GetEntry(entry_);
    c_mc_type_ = true;
  }
  return mc_type_;
}

#include "small_tree_quick.hpp"
small_tree* NewTree(const std::type_info &type){

  if(type == typeid(small_tree)) return new small_tree;
  else if(type == typeid(small_tree_quick)) return static_cast<small_tree*>(new small_tree_quick);
  else return new small_tree;
}

