#ifndef __ENUMHIST_H__
#define __ENUMHIST_H__
#include <string>
using namespace std;

enum histList {
    hist_EvtInfo_NPu,
    hist_EvtInfo_Rho,
    hist_EvtInfo_Rho_wopu,
    hist_EvtInfo_NVtx,
    hist_EvtInfo_NVtx_wopu,
    hist_EvtInfo_genweight,
    hist_DiPhoInfo_mass,
    hist_DiPhoInfo_pt,
    hist_DiPhoInfo_eta,
    hist_DiPhoInfo_phi,
    hist_DiPhoInfo_energy,
    hist_DiPhoInfo_leadPt,
    hist_DiPhoInfo_leadEta,
    hist_DiPhoInfo_leadPhi,
    hist_DiPhoInfo_leadE,
    hist_DiPhoInfo_leadhoe,
    hist_DiPhoInfo_leadIDMVA,
    hist_DiPhoInfo_subleadPt,
    hist_DiPhoInfo_subleadEta,
    hist_DiPhoInfo_subleadPhi,
    hist_DiPhoInfo_subleadE,
    hist_DiPhoInfo_subleadhoe,
    hist_DiPhoInfo_subleadIDMVA,
    hist_ElecInfo_Size,
    hist_MuonInfo_Size,
    hist_num_leptons,
    hist_num_electrons,
    hist_num_muons,
    hist_ElecInfo_electron_charge,
    hist_ElecInfo_electron_pt,
    hist_ElecInfo_electron_eta,
    hist_ElecInfo_electron_phi,
    hist_ElecInfo_electron_energy,
    hist_ElecInfo_electron_diphoton_deltaR,
    hist_MuonInfo_muon_charge,
    hist_MuonInfo_muon_pt,
    hist_MuonInfo_muon_eta,
    hist_MuonInfo_muon_phi,
    hist_MuonInfo_muon_energy,
    hist_MuonInfo_muon_diphoton_deltaR,
    hist_jets_size,
    hist_num_jets,
    hist_JetInfo_jet_pt,
    hist_JetInfo_jet_eta,
    hist_JetInfo_jet_phi,
    hist_JetInfo_jet_energy,
    hist_JetInfo_jet_diphoton_deltaR,
    hist_MetInfo_Pt,
    hist_MetInfo_Phi,
    hist_MetInfo_Px,
    hist_MetInfo_Py,
    hist_MetInfo_SumET,
    hist_MetInfo_Pz_solution_1,
    hist_MetInfo_Pz_solution_2,
    hist_MetInfo_coeff_D,
    hist_MetInfo_coeff_A,
    hist_MetInfo_coeff_B2A,
    hist_MetInfo_coeff_D2A,
    hist_lepton_charge,
    hist_lepton_pt,
    hist_lepton_eta,
    hist_lepton_phi,
    hist_lepton_energy,
    hist_lepton_diphoton_deltaR,
    hist_jet1_pt,
    hist_jet1_eta,
    hist_jet1_phi,
    hist_jet1_energy,
    hist_jet1_btag_score,
    hist_jet1_CvsL_score,
    hist_jet1_CvsB_score,
    hist_jet1_diphoton_deltaR,
    hist_jet1_lepton_deltaR,
    hist_jet2_pt,
    hist_jet2_eta,
    hist_jet2_phi,
    hist_jet2_energy,
    hist_jet2_btag_score,
    hist_jet2_CvsL_score,
    hist_jet2_CvsB_score,
    hist_jet2_diphoton_deltaR,
    hist_jet2_lepton_deltaR,
    hist_wjet1_pt,
    hist_wjet1_eta,
    hist_wjet1_phi,
    hist_wjet1_energy,
    hist_wjet1_btag_score,
    hist_wjet1_CvsL_score,
    hist_wjet1_CvsB_score,
    hist_wjet1_diphoton_deltaR,
    hist_wjet1_lepton_deltaR,
    hist_wjet2_pt,
    hist_wjet2_eta,
    hist_wjet2_phi,
    hist_wjet2_energy,
    hist_wjet2_btag_score,
    hist_wjet2_CvsL_score,
    hist_wjet2_CvsB_score,
    hist_wjet2_diphoton_deltaR,
    hist_wjet2_lepton_deltaR,
    hist_jetq_pt,
    hist_jetq_eta,
    hist_jetq_phi,
    hist_jetq_energy,
    hist_jetq_btag_score,
    hist_jetq_CvsL_score,
    hist_jetq_CvsB_score,
    hist_jetq_diphoton_deltaR,
    hist_jetq_lepton_deltaR,
    hist_leading_bjet_pt,
    hist_leading_bjet_eta,
    hist_leading_bjet_phi,
    hist_leading_bjet_energy,
    hist_leading_bjet_btag_score,
    hist_leading_bjet_CvsL_score,
    hist_leading_bjet_CvsB_score,
    hist_deltaR_top_top,
    hist_deltaR_qH,
    hist_deltaR_photon_photon,
    hist_deltaR_bW,
    hist_deltaR_HW,
    hist_deltaR_tH,
    hist_deltaR_lep_met,
    hist_deltaR_jet1_jet2,
    hist_deltaR_wjet1_wjet2,
    hist_top_tqh_pt,
    hist_top_tqh_eta,
    hist_top_tqh_mass,
    hist_hadronic_w_candidate_pt,
    hist_hadronic_w_candidate_eta,
    hist_hadronic_w_candidate_mass,
    hist_hadronic_top_tbw_pt,
    hist_hadronic_top_tbw_eta,
    hist_hadronic_top_tbw_mass,
    hist_leptonic_w_candidate_solution1_pt,
    hist_leptonic_w_candidate_solution1_eta,
    hist_leptonic_w_candidate_solution1_mass,
    hist_leptonic_top_tbw_solution1_pt,
    hist_leptonic_top_tbw_solution1_eta,
    hist_leptonic_top_tbw_solution1_mass,
    hist_leptonic_w_candidate_solution2_pt,
    hist_leptonic_w_candidate_solution2_eta,
    hist_leptonic_w_candidate_solution2_mass,
    hist_leptonic_top_tbw_solution2_pt,
    hist_leptonic_top_tbw_solution2_eta,
    hist_leptonic_top_tbw_solution2_mass,
    totalHistNum
};
std::string histNames[totalHistNum]{
    "hist_EvtInfo_NPu",
    "hist_EvtInfo_Rho",
    "hist_EvtInfo_Rho_wopu",
    "hist_EvtInfo_NVtx",
    "hist_EvtInfo_NVtx_wopu",
    "hist_EvtInfo_genweight",
    "hist_DiPhoInfo_mass",
    "hist_DiPhoInfo_pt",
    "hist_DiPhoInfo_eta",
    "hist_DiPhoInfo_phi",
    "hist_DiPhoInfo_energy",
    "hist_DiPhoInfo_leadPt",
    "hist_DiPhoInfo_leadEta",
    "hist_DiPhoInfo_leadPhi",
    "hist_DiPhoInfo_leadE",
    "hist_DiPhoInfo_leadhoe",
    "hist_DiPhoInfo_leadIDMVA",
    "hist_DiPhoInfo_subleadPt",
    "hist_DiPhoInfo_subleadEta",
    "hist_DiPhoInfo_subleadPhi",
    "hist_DiPhoInfo_subleadE",
    "hist_DiPhoInfo_subleadhoe",
    "hist_DiPhoInfo_subleadIDMVA",
    "hist_ElecInfo_Size",
    "hist_MuonInfo_Size",
    "hist_num_leptons",
    "hist_num_electrons",
    "hist_num_muons",
    "hist_ElecInfo_electron_charge",
    "hist_ElecInfo_electron_pt",
    "hist_ElecInfo_electron_eta",
    "hist_ElecInfo_electron_phi",
    "hist_ElecInfo_electron_energy",
    "hist_ElecInfo_electron_diphoton_deltaR",
    "hist_MuonInfo_muon_charge",
    "hist_MuonInfo_muon_pt",
    "hist_MuonInfo_muon_eta",
    "hist_MuonInfo_muon_phi",
    "hist_MuonInfo_muon_energy",
    "hist_MuonInfo_muon_diphoton_deltaR",
    "hist_jets_size",
    "hist_num_jets",
    "hist_JetInfo_jet_pt",
    "hist_JetInfo_jet_eta",
    "hist_JetInfo_jet_phi",
    "hist_JetInfo_jet_energy",
    "hist_JetInfo_jet_diphoton_deltaR",
    "hist_MetInfo_Pt",
    "hist_MetInfo_Phi",
    "hist_MetInfo_Px",
    "hist_MetInfo_Py",
    "hist_MetInfo_SumET",
    "hist_MetInfo_Pz_solution_1",
    "hist_MetInfo_Pz_solution_2",
    "hist_MetInfo_coeff_D",
    "hist_MetInfo_coeff_A",
    "hist_MetInfo_coeff_B2A",
    "hist_MetInfo_coeff_D2A",
    "hist_lepton_charge",
    "hist_lepton_pt",
    "hist_lepton_eta",
    "hist_lepton_phi",
    "hist_lepton_energy",
    "hist_lepton_diphoton_deltaR",
    "hist_jet1_pt",
    "hist_jet1_eta",
    "hist_jet1_phi",
    "hist_jet1_energy",
    "hist_jet1_btag_score",
    "hist_jet1_CvsL_score",
    "hist_jet1_CvsB_score",
    "hist_jet1_diphoton_deltaR",
    "hist_jet1_lepton_deltaR",
    "hist_jet2_pt",
    "hist_jet2_eta",
    "hist_jet2_phi",
    "hist_jet2_energy",
    "hist_jet2_btag_score",
    "hist_jet2_CvsL_score",
    "hist_jet2_CvsB_score",
    "hist_jet2_diphoton_deltaR",
    "hist_jet2_lepton_deltaR",
    "hist_wjet1_pt",
    "hist_wjet1_eta",
    "hist_wjet1_phi",
    "hist_wjet1_energy",
    "hist_wjet1_btag_score",
    "hist_wjet1_CvsL_score",
    "hist_wjet1_CvsB_score",
    "hist_wjet1_diphoton_deltaR",
    "hist_wjet1_lepton_deltaR",
    "hist_wjet2_pt",
    "hist_wjet2_eta",
    "hist_wjet2_phi",
    "hist_wjet2_energy",
    "hist_wjet2_btag_score",
    "hist_wjet2_CvsL_score",
    "hist_wjet2_CvsB_score",
    "hist_wjet2_diphoton_deltaR",
    "hist_wjet2_lepton_deltaR",
    "hist_jetq_pt",
    "hist_jetq_eta",
    "hist_jetq_phi",
    "hist_jetq_energy",
    "hist_jetq_btag_score",
    "hist_jetq_CvsL_score",
    "hist_jetq_CvsB_score",
    "hist_jetq_diphoton_deltaR",
    "hist_jetq_lepton_deltaR",
    "hist_leading_bjet_pt",
    "hist_leading_bjet_eta",
    "hist_leading_bjet_phi",
    "hist_leading_bjet_energy",
    "hist_leading_bjet_btag_score",
    "hist_leading_bjet_CvsL_score",
    "hist_leading_bjet_CvsB_score",
    "hist_deltaR_top_top",
    "hist_deltaR_qH",
    "hist_deltaR_photon_photon",
    "hist_deltaR_bW",
    "hist_deltaR_HW",
    "hist_deltaR_tH",
    "hist_deltaR_lep_met",
    "hist_deltaR_jet1_jet2",
    "hist_deltaR_wjet1_wjet2",
    "hist_top_tqh_pt",
    "hist_top_tqh_eta",
    "hist_top_tqh_mass",
    "hist_hadronic_w_candidate_pt",
    "hist_hadronic_w_candidate_eta",
    "hist_hadronic_w_candidate_mass",
    "hist_hadronic_top_tbw_pt",
    "hist_hadronic_top_tbw_eta",
    "hist_hadronic_top_tbw_mass",
    "hist_leptonic_w_candidate_solution1_pt",
    "hist_leptonic_w_candidate_solution1_eta",
    "hist_leptonic_w_candidate_solution1_mass",
    "hist_leptonic_top_tbw_solution1_pt",
    "hist_leptonic_top_tbw_solution1_eta",
    "hist_leptonic_top_tbw_solution1_mass",
    "hist_leptonic_w_candidate_solution2_pt",
    "hist_leptonic_w_candidate_solution2_eta",
    "hist_leptonic_w_candidate_solution2_mass",
    "hist_leptonic_top_tbw_solution2_pt",
    "hist_leptonic_top_tbw_solution2_eta",
    "hist_leptonic_top_tbw_solution2_mass"
};
int histNbins[totalHistNum]{
    100,//hist_EvtInfo_NPu
    40,//hist_EvtInfo_Rho
    40,//hist_EvtInfo_Rho_wopu
    50,//hist_EvtInfo_NVtx
    50,//hist_EvtInfo_NVtx_wopu
    100,//hist_EvtInfo_genweight
    40,//hist_DiPhoInfo_mass
    40,//hist_DiPhoInfo_pt
    40,//hist_DiPhoInfo_eta
    40,//hist_DiPhoInfo_phi
    40,//hist_DiPhoInfo_energy
    40,//hist_DiPhoInfo_leadPt
    40,//hist_DiPhoInfo_leadEta
    40,//hist_DiPhoInfo_leadPhi
    40,//hist_DiPhoInfo_leadE
    50,//hist_DiPhoInfo_leadhoe
    50,//hist_DiPhoInfo_leadIDMVA
    40,//hist_DiPhoInfo_subleadPt
    40,//hist_DiPhoInfo_subleadEta
    40,//hist_DiPhoInfo_subleadPhi
    40,//hist_DiPhoInfo_subleadE
    50,//hist_DiPhoInfo_subleadhoe
    50,//hist_DiPhoInfo_subleadIDMVA
    10,//hist_ElecInfo_Size
    10,//hist_MuonInfo_Size
    10,//hist_num_leptons
    10,//hist_num_electrons
    10,//hist_num_muons
    2,//hist_ElecInfo_electron_charge
    40,//hist_ElecInfo_electron_pt
    40,//hist_ElecInfo_electron_eta
    40,//hist_ElecInfo_electron_phi
    40,//hist_ElecInfo_electron_energy
    40,//hist_ElecInfo_electron_diphoton_deltaR
    2,//hist_MuonInfo_muon_charge
    40,//hist_MuonInfo_muon_pt
    40,//hist_MuonInfo_muon_eta
    40,//hist_MuonInfo_muon_phi
    40,//hist_MuonInfo_muon_energy
    40,//hist_MuonInfo_muon_diphoton_deltaR
    10,//hist_jets_size
    15,//hist_num_jets
    40,//hist_JetInfo_jet_pt
    40,//hist_JetInfo_jet_eta
    40,//hist_JetInfo_jet_phi
    40,//hist_JetInfo_jet_energy
    40,//hist_JetInfo_jet_diphoton_deltaR
    40,//hist_MetInfo_Pt
    40,//hist_MetInfo_Phi
    40,//hist_MetInfo_Px
    40,//hist_MetInfo_Py
    40,//hist_MetInfo_SumET
    40,//hist_MetInfo_Pz_solution_1
    40,//hist_MetInfo_Pz_solution_2
    40,//hist_MetInfo_coeff_D
    40,//hist_MetInfo_coeff_A
    40,//hist_MetInfo_coeff_B2A
    40,//hist_MetInfo_coeff_D2A
    2,//hist_lepton_charge
    40,//hist_lepton_pt
    40,//hist_lepton_eta
    40,//hist_lepton_phi
    40,//hist_lepton_energy
    40,//hist_lepton_diphoton_deltaR
    40,//hist_jet1_pt
    40,//hist_jet1_eta
    40,//hist_jet1_phi
    40,//hist_jet1_energy
    10,//hist_jet1_btag_score
    10,//hist_jet1_CvsL_score
    10,//hist_jet1_CvsB_score
    40,//hist_jet1_diphoton_deltaR
    40,//hist_jet1_lepton_deltaR
    40,//hist_jet2_pt
    40,//hist_jet2_eta
    40,//hist_jet2_phi
    40,//hist_jet2_energy
    10,//hist_jet2_btag_score
    10,//hist_jet2_CvsL_score
    10,//hist_jet2_CvsB_score
    40,//hist_jet2_diphoton_deltaR
    40,//hist_jet2_lepton_deltaR
    40,//hist_wjet1_pt
    40,//hist_wjet1_eta
    40,//hist_wjet1_phi
    40,//hist_wjet1_energy
    10,//hist_wjet1_btag_score
    10,//hist_wjet1_CvsL_score
    10,//hist_wjet1_CvsB_score
    40,//hist_wjet1_diphoton_deltaR
    40,//hist_wjet1_lepton_deltaR
    40,//hist_wjet2_pt
    40,//hist_wjet2_eta
    40,//hist_wjet2_phi
    40,//hist_wjet2_energy
    10,//hist_wjet2_btag_score
    10,//hist_wjet2_CvsL_score
    10,//hist_wjet2_CvsB_score
    40,//hist_wjet2_diphoton_deltaR
    40,//hist_wjet2_lepton_deltaR
    40,//hist_jetq_pt
    40,//hist_jetq_eta
    40,//hist_jetq_phi
    40,//hist_jetq_energy
    10,//hist_jetq_btag_score
    10,//hist_jetq_CvsL_score
    10,//hist_jetq_CvsB_score
    40,//hist_jetq_diphoton_deltaR
    40,//hist_jetq_lepton_deltaR
    40,//hist_leading_bjet_pt
    40,//hist_leading_bjet_eta
    40,//hist_leading_bjet_phi
    40,//hist_leading_bjet_energy
    10,//hist_leading_bjet_btag_score
    10,//hist_leading_bjet_CvsL_score
    10,//hist_leading_bjet_CvsB_score
    40,//hist_deltaR_top_top
    40,//hist_deltaR_qH
    40,//hist_deltaR_photon_photon
    40,//hist_deltaR_bW
    40,//hist_deltaR_HW
    40,//hist_deltaR_tH
    40,//hist_deltaR_lep_met
    40,//hist_deltaR_jet1_jet2
    40,//hist_deltaR_wjet1_wjet2
    40,//hist_top_tqh_pt
    40,//hist_top_tqh_eta
    40,//hist_top_tqh_mass
    40,//hist_hadronic_w_candidate_pt
    40,//hist_hadronic_w_candidate_eta
    50,//hist_hadronic_w_candidate_mass
    40,//hist_hadronic_top_tbw_pt
    40,//hist_hadronic_top_tbw_eta
    40,//hist_hadronic_top_tbw_mass
    40,//hist_leptonic_w_candidate_solution1_pt
    40,//hist_leptonic_w_candidate_solution1_eta
    50,//hist_leptonic_w_candidate_solution1_mass
    40,//hist_leptonic_top_tbw_solution1_pt
    40,//hist_leptonic_top_tbw_solution1_eta
    40,//hist_leptonic_top_tbw_solution1_mass
    40,//hist_leptonic_w_candidate_solution2_pt
    40,//hist_leptonic_w_candidate_solution2_eta
    50,//hist_leptonic_w_candidate_solution2_mass
    40,//hist_leptonic_top_tbw_solution2_pt
    40,//hist_leptonic_top_tbw_solution2_eta
    40//hist_leptonic_top_tbw_solution2_mass
};
double histBinLow[totalHistNum]{
    0,//hist_EvtInfo_NPu
    0,//hist_EvtInfo_Rho
    0,//hist_EvtInfo_Rho_wopu
    0,//hist_EvtInfo_NVtx
    0,//hist_EvtInfo_NVtx_wopu
    0,//hist_EvtInfo_genweight
    100,//hist_DiPhoInfo_mass
    0,//hist_DiPhoInfo_pt
    -2.5,//hist_DiPhoInfo_eta
    -3.0,//hist_DiPhoInfo_phi
    0,//hist_DiPhoInfo_energy
    0,//hist_DiPhoInfo_leadPt
    -2.5,//hist_DiPhoInfo_leadEta
    -3.0,//hist_DiPhoInfo_leadPhi
    0,//hist_DiPhoInfo_leadE
    0,//hist_DiPhoInfo_leadhoe
    -1.,//hist_DiPhoInfo_leadIDMVA
    0,//hist_DiPhoInfo_subleadPt
    -2.5,//hist_DiPhoInfo_subleadEta
    -3.0,//hist_DiPhoInfo_subleadPhi
    0,//hist_DiPhoInfo_subleadE
    0,//hist_DiPhoInfo_subleadhoe
    -1.,//hist_DiPhoInfo_subleadIDMVA
    0,//hist_ElecInfo_Size
    0,//hist_MuonInfo_Size
    0,//hist_num_leptons
    0,//hist_num_electrons
    0,//hist_num_muons
    -2,//hist_ElecInfo_electron_charge
    0,//hist_ElecInfo_electron_pt
    -2.5,//hist_ElecInfo_electron_eta
    -3.0,//hist_ElecInfo_electron_phi
    0,//hist_ElecInfo_electron_energy
    0,//hist_ElecInfo_electron_diphoton_deltaR
    -2,//hist_MuonInfo_muon_charge
    0,//hist_MuonInfo_muon_pt
    -2.5,//hist_MuonInfo_muon_eta
    -3.0,//hist_MuonInfo_muon_phi
    0,//hist_MuonInfo_muon_energy
    0,//hist_MuonInfo_muon_diphoton_deltaR
    0,//hist_jets_size
    0,//hist_num_jets
    0,//hist_JetInfo_jet_pt
    -2.5,//hist_JetInfo_jet_eta
    -3.0,//hist_JetInfo_jet_phi
    0,//hist_JetInfo_jet_energy
    0,//hist_JetInfo_jet_diphoton_deltaR
    0,//hist_MetInfo_Pt
    -3.0,//hist_MetInfo_Phi
    -200,//hist_MetInfo_Px
    -200,//hist_MetInfo_Py
    0,//hist_MetInfo_SumET
    -200,//hist_MetInfo_Pz_solution_1
    -200,//hist_MetInfo_Pz_solution_2
    -1000,//hist_MetInfo_coeff_D
    -1000.,//hist_MetInfo_coeff_A
    -600,//hist_MetInfo_coeff_B2A
    -600,//hist_MetInfo_coeff_D2A
    -2,//hist_lepton_charge
    0,//hist_lepton_pt
    -2.5,//hist_lepton_eta
    -3.0,//hist_lepton_phi
    0,//hist_lepton_energy
    0,//hist_lepton_diphoton_deltaR
    0,//hist_jet1_pt
    -2.5,//hist_jet1_eta
    -3.0,//hist_jet1_phi
    0,//hist_jet1_energy
    0,//hist_jet1_btag_score
    0,//hist_jet1_CvsL_score
    0,//hist_jet1_CvsB_score
    0,//hist_jet1_diphoton_deltaR
    0,//hist_jet1_lepton_deltaR
    0,//hist_jet2_pt
    -2.5,//hist_jet2_eta
    -3.0,//hist_jet2_phi
    0,//hist_jet2_energy
    0,//hist_jet2_btag_score
    0,//hist_jet2_CvsL_score
    0,//hist_jet2_CvsB_score
    0,//hist_jet2_diphoton_deltaR
    0,//hist_jet2_lepton_deltaR
    0,//hist_wjet1_pt
    -2.5,//hist_wjet1_eta
    -3.0,//hist_wjet1_phi
    0,//hist_wjet1_energy
    0,//hist_wjet1_btag_score
    0,//hist_wjet1_CvsL_score
    0,//hist_wjet1_CvsB_score
    0,//hist_wjet1_diphoton_deltaR
    0,//hist_wjet1_lepton_deltaR
    0,//hist_wjet2_pt
    -2.5,//hist_wjet2_eta
    -3.0,//hist_wjet2_phi
    0,//hist_wjet2_energy
    0,//hist_wjet2_btag_score
    0,//hist_wjet2_CvsL_score
    0,//hist_wjet2_CvsB_score
    0,//hist_wjet2_diphoton_deltaR
    0,//hist_wjet2_lepton_deltaR
    0,//hist_jetq_pt
    -2.5,//hist_jetq_eta
    -3.0,//hist_jetq_phi
    0,//hist_jetq_energy
    0,//hist_jetq_btag_score
    0,//hist_jetq_CvsL_score
    0,//hist_jetq_CvsB_score
    0,//hist_jetq_diphoton_deltaR
    0,//hist_jetq_lepton_deltaR
    0,//hist_leading_bjet_pt
    -2.5,//hist_leading_bjet_eta
    -3.0,//hist_leading_bjet_phi
    0,//hist_leading_bjet_energy
    0,//hist_leading_bjet_btag_score
    0,//hist_leading_bjet_CvsL_score
    0,//hist_leading_bjet_CvsB_score
    0,//hist_deltaR_top_top
    0,//hist_deltaR_qH
    0,//hist_deltaR_photon_photon
    0,//hist_deltaR_bW
    0,//hist_deltaR_HW
    0,//hist_deltaR_tH
    0,//hist_deltaR_lep_met
    0,//hist_deltaR_jet1_jet2
    0,//hist_deltaR_wjet1_wjet2
    0,//hist_top_tqh_pt
    -2.5,//hist_top_tqh_eta
    0,//hist_top_tqh_mass
    0,//hist_hadronic_w_candidate_pt
    -2.5,//hist_hadronic_w_candidate_eta
    0,//hist_hadronic_w_candidate_mass
    0,//hist_hadronic_top_tbw_pt
    -2.5,//hist_hadronic_top_tbw_eta
    0,//hist_hadronic_top_tbw_mass
    0,//hist_leptonic_w_candidate_solution1_pt
    -2.5,//hist_leptonic_w_candidate_solution1_eta
    0,//hist_leptonic_w_candidate_solution1_mass
    0,//hist_leptonic_top_tbw_solution1_pt
    -2.5,//hist_leptonic_top_tbw_solution1_eta
    0,//hist_leptonic_top_tbw_solution1_mass
    0,//hist_leptonic_w_candidate_solution2_pt
    -2.5,//hist_leptonic_w_candidate_solution2_eta
    0,//hist_leptonic_w_candidate_solution2_mass
    0,//hist_leptonic_top_tbw_solution2_pt
    -2.5,//hist_leptonic_top_tbw_solution2_eta
    0//hist_leptonic_top_tbw_solution2_mass
};
double histBinHigh[totalHistNum]{
    100,//hist_EvtInfo_NPu
    80,//hist_EvtInfo_Rho
    80,//hist_EvtInfo_Rho_wopu
    100,//hist_EvtInfo_NVtx
    100,//hist_EvtInfo_NVtx_wopu
    100,//hist_EvtInfo_genweight
    180,//hist_DiPhoInfo_mass
    200,//hist_DiPhoInfo_pt
    2.5,//hist_DiPhoInfo_eta
    3.0,//hist_DiPhoInfo_phi
    500,//hist_DiPhoInfo_energy
    200,//hist_DiPhoInfo_leadPt
    2.5,//hist_DiPhoInfo_leadEta
    3.0,//hist_DiPhoInfo_leadPhi
    200,//hist_DiPhoInfo_leadE
    0.10,//hist_DiPhoInfo_leadhoe
    1.,//hist_DiPhoInfo_leadIDMVA
    200,//hist_DiPhoInfo_subleadPt
    2.5,//hist_DiPhoInfo_subleadEta
    3.0,//hist_DiPhoInfo_subleadPhi
    200,//hist_DiPhoInfo_subleadE
    0.10,//hist_DiPhoInfo_subleadhoe
    1.,//hist_DiPhoInfo_subleadIDMVA
    10,//hist_ElecInfo_Size
    10,//hist_MuonInfo_Size
    10,//hist_num_leptons
    10,//hist_num_electrons
    10,//hist_num_muons
    2,//hist_ElecInfo_electron_charge
    200,//hist_ElecInfo_electron_pt
    2.5,//hist_ElecInfo_electron_eta
    3.0,//hist_ElecInfo_electron_phi
    200,//hist_ElecInfo_electron_energy
    6,//hist_ElecInfo_electron_diphoton_deltaR
    2,//hist_MuonInfo_muon_charge
    200,//hist_MuonInfo_muon_pt
    2.5,//hist_MuonInfo_muon_eta
    3.0,//hist_MuonInfo_muon_phi
    200,//hist_MuonInfo_muon_energy
    6,//hist_MuonInfo_muon_diphoton_deltaR
    10,//hist_jets_size
    15,//hist_num_jets
    200,//hist_JetInfo_jet_pt
    2.5,//hist_JetInfo_jet_eta
    3.0,//hist_JetInfo_jet_phi
    200,//hist_JetInfo_jet_energy
    6,//hist_JetInfo_jet_diphoton_deltaR
    200,//hist_MetInfo_Pt
    3.0,//hist_MetInfo_Phi
    200,//hist_MetInfo_Px
    200,//hist_MetInfo_Py
    200,//hist_MetInfo_SumET
    200,//hist_MetInfo_Pz_solution_1
    200,//hist_MetInfo_Pz_solution_2
    1000,//hist_MetInfo_coeff_D
    1000.,//hist_MetInfo_coeff_A
    600,//hist_MetInfo_coeff_B2A
    600,//hist_MetInfo_coeff_D2A
    2,//hist_lepton_charge
    200,//hist_lepton_pt
    2.5,//hist_lepton_eta
    3.0,//hist_lepton_phi
    200,//hist_lepton_energy
    6,//hist_lepton_diphoton_deltaR
    200,//hist_jet1_pt
    2.5,//hist_jet1_eta
    3.0,//hist_jet1_phi
    200,//hist_jet1_energy
    1,//hist_jet1_btag_score
    1,//hist_jet1_CvsL_score
    1,//hist_jet1_CvsB_score
    6,//hist_jet1_diphoton_deltaR
    6,//hist_jet1_lepton_deltaR
    200,//hist_jet2_pt
    2.5,//hist_jet2_eta
    3.0,//hist_jet2_phi
    200,//hist_jet2_energy
    1,//hist_jet2_btag_score
    1,//hist_jet2_CvsL_score
    1,//hist_jet2_CvsB_score
    6,//hist_jet2_diphoton_deltaR
    6,//hist_jet2_lepton_deltaR
    200,//hist_wjet1_pt
    2.5,//hist_wjet1_eta
    3.0,//hist_wjet1_phi
    200,//hist_wjet1_energy
    1,//hist_wjet1_btag_score
    1,//hist_wjet1_CvsL_score
    1,//hist_wjet1_CvsB_score
    6,//hist_wjet1_diphoton_deltaR
    6,//hist_wjet1_lepton_deltaR
    200,//hist_wjet2_pt
    2.5,//hist_wjet2_eta
    3.0,//hist_wjet2_phi
    200,//hist_wjet2_energy
    1,//hist_wjet2_btag_score
    1,//hist_wjet2_CvsL_score
    1,//hist_wjet2_CvsB_score
    6,//hist_wjet2_diphoton_deltaR
    6,//hist_wjet2_lepton_deltaR
    200,//hist_jetq_pt
    2.5,//hist_jetq_eta
    3.0,//hist_jetq_phi
    200,//hist_jetq_energy
    1,//hist_jetq_btag_score
    1,//hist_jetq_CvsL_score
    1,//hist_jetq_CvsB_score
    6,//hist_jetq_diphoton_deltaR
    6,//hist_jetq_lepton_deltaR
    200,//hist_leading_bjet_pt
    2.5,//hist_leading_bjet_eta
    3.0,//hist_leading_bjet_phi
    200,//hist_leading_bjet_energy
    1,//hist_leading_bjet_btag_score
    1,//hist_leading_bjet_CvsL_score
    1,//hist_leading_bjet_CvsB_score
    6,//hist_deltaR_top_top
    6,//hist_deltaR_qH
    6,//hist_deltaR_photon_photon
    6,//hist_deltaR_bW
    6,//hist_deltaR_HW
    6,//hist_deltaR_tH
    6,//hist_deltaR_lep_met
    6,//hist_deltaR_jet1_jet2
    6,//hist_deltaR_wjet1_wjet2
    400,//hist_top_tqh_pt
    2.5,//hist_top_tqh_eta
    400,//hist_top_tqh_mass
    200,//hist_hadronic_w_candidate_pt
    2.5,//hist_hadronic_w_candidate_eta
    150,//hist_hadronic_w_candidate_mass
    400,//hist_hadronic_top_tbw_pt
    2.5,//hist_hadronic_top_tbw_eta
    400,//hist_hadronic_top_tbw_mass
    200,//hist_leptonic_w_candidate_solution1_pt
    2.5,//hist_leptonic_w_candidate_solution1_eta
    150,//hist_leptonic_w_candidate_solution1_mass
    400,//hist_leptonic_top_tbw_solution1_pt
    2.5,//hist_leptonic_top_tbw_solution1_eta
    400,//hist_leptonic_top_tbw_solution1_mass
    200,//hist_leptonic_w_candidate_solution2_pt
    2.5,//hist_leptonic_w_candidate_solution2_eta
    150,//hist_leptonic_w_candidate_solution2_mass
    400,//hist_leptonic_top_tbw_solution2_pt
    2.5,//hist_leptonic_top_tbw_solution2_eta
    400//hist_leptonic_top_tbw_solution2_mass
};
#endif
