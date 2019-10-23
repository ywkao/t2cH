//***************************************************************************
//
// FileName    : generalChiSquareStudy.cpp
// Purpose     : Develop for top FCNH with H to two photons analysis
// Description : Extracting event info & Selecting objects (diphoton, leptons, jets).
// Author      : Yu-Wei Kao [ykao@cern.ch]
//
//***************************************************************************
#include <algorithm> //sts::fill_n(array, N_elements, -999)
#include <stdio.h>
#include <math.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>
#include <TLorentzVector.h>
#include <TLegend.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TTree.h>
#include <vector>
#include <string>
#include "../include/generalChiSquareStudy.h"
#include "../include/cross_section.h"
using namespace std;

int main(int argc, char *argv[]){
    //### I/O and event info{{{
    //============================//
    //----- Input file names -----//
    //============================//
    char input_file[512]; sprintf(input_file, "%s", argv[1]); printf("[INFO] input_file  = %s\n", input_file);
    char output_file[512]; sprintf(output_file, "%s", argv[2]); printf("[INFO] output_file = %s\n", output_file);
    char dataset[512]; sprintf(dataset, "%s", argv[3]); printf("[INFO] dataset     = %s\n", dataset);
    bool isData = isThisDataOrNot(dataset);
    bool isMCsignal = isThisMCsignal(dataset);
    bool isMultiFile = isThisMultiFile(dataset);
    TFile *fout = new TFile(output_file, "RECREATE");
    //==============================//
    //----- Read input file(s) -----//
    //==============================//
    flashggStdTreeReader treeReader;
    treeReader.InitChain("flashggNtuples/flashggStdTree");
    if(!isMultiFile) treeReader.AddSingleRootFile(input_file);
    else             treeReader.AddMultiRootFile(input_file);
    treeReader.SetBranchAddresses();
    //===============================//
    //----- Prepare output file -----//
    //===============================//
    myTreeClass mytree;
    mytree.InitTree();
    mytree.MakeNewBranchAddresses();
    //==================================================//
    //--------   Event Loop [Normalization]   ----------//
    //==================================================//
    // Note: Normalization factors are not used during preselection.
    // It is stored and would be used during selection.
    // Question: keep total genweight before or after preselection?
    int nentries = treeReader.GetEntries(); printf("[INFO] N_entries = %d\n", nentries);
    double NormalizationFactor;
    double Luminosity = 41.53; //fb{-1}
    double CrossSection = GetXsec(dataset); //pb
    double BranchingFraction = GetBranchingFraction(dataset); //pb
    printf("[INFO] CrossSection = %f !\n", CrossSection);
    printf("[INFO] Equivalent lumi. = %f !\n", (double)nentries/CrossSection);
    printf("[INFO] BranchingFraction = %f !\n", BranchingFraction);
    double TotalGenweight=0;
    for(int ientry=0; ientry<nentries; ientry++){
        treeReader.flashggStdTree->GetEntry(ientry);//load data
        TotalGenweight+=treeReader.GetGenWeight();
    }
    NormalizationFactor = 1000. * Luminosity * CrossSection * BranchingFraction / TotalGenweight;
    printf("[INFO] TotalGenweight = %f!\n", TotalGenweight);
    printf("[INFO] NormalizationFactor = %f!\n", isData ? 1. : NormalizationFactor);
    //}}}
    //### histograms{{{
    //==================================================//
    //--------   Histograms for mass spectra  ----------//
    //==================================================//
    TH1D *hist_num_bjets_tight = new TH1D("hist_num_bjets_tight", ";Number of tight b-tagged jets;Entries", 10, 0, 10);
    TH1D *hist_num_bjets_medium = new TH1D("hist_num_bjets_medium", ";Number of medium b-tagged jets;Entries", 10, 0, 10);
    TH1D *hist_num_bjets_loose = new TH1D("hist_num_bjets_loose", ";Number of loose b-tagged jets;Entries", 10, 0, 10);
    TH1D *hist_num_bjets_tight_matched = new TH1D("hist_num_bjets_tight_matched", ";Number of tight b-tagged jets;Entries", 10, 0, 10);
    TH1D *hist_num_bjets_medium_matched = new TH1D("hist_num_bjets_medium_matched", ";Number of medium b-tagged jets;Entries", 10, 0, 10);
    TH1D *hist_num_bjets_loose_matched = new TH1D("hist_num_bjets_loose_matched", ";Number of loose b-tagged jets;Entries", 10, 0, 10);
    TH1D *hist_num_gen_bquark = new TH1D("hist_num_gen_bquark", ";Number of b quark (gen-level);Entries", 10, 0, 10);
    TH1D *hist_num_gen_light_quark = new TH1D("hist_num_gen_light_quark", ";Number of light quarks (gen-level);Entries", 10, 0, 10);
    TH1D *hist_num_selected_jets = new TH1D("hist_num_selected_jets", ";Number of selected jets;Entries", 10, 0, 10);
    TH1D *hist_deltaR_W_genW_simple = new TH1D("hist_deltaR_W_genW_simple", ";deltaR;Entries", 20, 0, 0.01);
    TH1D *hist_deltaR_W_genW_modified = new TH1D("hist_deltaR_W_genW_modified", ";deltaR;Entries", 20, 0, 0.01);
    TH1D *hist_deltaR_W_genW_yfyj = new TH1D("hist_deltaR_W_genW_yfyj", ";deltaR;Entries", 20, 0, 0.01);

    TH1D *hist_mass_diphoton = new TH1D("hist_mass_diphoton", ";Mass [GeV/c^2];Entries", 50, 0, 250);
    TH1D *hist_mass_top_fcnh_simple = new TH1D("hist_mass_top_fcnh_simple", ";Mass [GeV/c^2];Entries", 70, 0, 350);
    TH1D *hist_mass_top_fcnh_modified = new TH1D("hist_mass_top_fcnh_modified", ";Mass [GeV/c^2];Entries", 70, 0, 350);

    TH1D *hist_mass_w_candidate_yfyj = new TH1D("hist_mass_w_candidate_yfyj", ";Mass [GeV/c^2];Entries", 60, 0, 180);
    TH1D *hist_mass_t1_candidate_yfyj = new TH1D("hist_mass_t1_candidate_yfyj", ";Mass [GeV/c^2];Entries", 70, 0, 350);
    TH1D *hist_mass_t2_candidate_yfyj = new TH1D("hist_mass_t2_candidate_yfyj", ";Mass [GeV/c^2];Entries", 70, 0, 350);
    TH1D *hist_mass_gen_w_candidate_yfyj = new TH1D("hist_mass_gen_w_candidate_yfyj", ";Mass [GeV/c^2];Entries", 50, 0, 150);
    TH1D *hist_mass_gen_t2_candidate_yfyj = new TH1D("hist_mass_gen_t2_candidate_yfyj", ";Mass [GeV/c^2];Entries", 70, 0, 350);
    TH1D *hist_mass_conditioned_gen_w_candidate_yfyj    = new TH1D("hist_mass_conditioned_gen_w_candidate_yfyj", ";Mass [GeV/c^2];Entries", 50, 0, 150);
    TH1D *hist_mass_conditioned_gen_t2_candidate_yfyj   = new TH1D("hist_mass_conditioned_gen_t2_candidate_yfyj", ";Mass [GeV/c^2];Entries", 70, 0, 350);

    TH1D *hist_mass_w_candidate_chi2_simple = new TH1D("hist_mass_w_candidate_chi2_simple", ";Mass [GeV/c^2];Entries", 60, 0, 180);
    TH1D *hist_mass_top_candidate_chi2_simple = new TH1D("hist_mass_top_candidate_chi2_simple", ";Mass [GeV/c^2];Entries", 70, 0, 350);
    TH1D *hist_mass_w_candidate_chi2_modified = new TH1D("hist_mass_w_candidate_chi2_modified", ";Mass [GeV/c^2];Entries", 60, 0, 180);
    TH1D *hist_mass_top_candidate_chi2_modified = new TH1D("hist_mass_top_candidate_chi2_modified", ";Mass [GeV/c^2];Entries", 70, 0, 350);
    TH1D *hist_mass_gen_w_candidate_chi2_simple     = new TH1D("hist_mass_gen_w_candidate_chi2_simple", ";Mass [GeV/c^2];Entries", 50, 0, 150);
    TH1D *hist_mass_gen_top_candidate_chi2_simple   = new TH1D("hist_mass_gen_top_candidate_chi2_simple", ";Mass [GeV/c^2];Entries", 70, 0, 350);
    TH1D *hist_mass_gen_w_candidate_chi2_modified   = new TH1D("hist_mass_gen_w_candidate_chi2_modified", ";Mass [GeV/c^2];Entries", 50, 0, 150);
    TH1D *hist_mass_gen_top_candidate_chi2_modified = new TH1D("hist_mass_gen_top_candidate_chi2_modified", ";Mass [GeV/c^2];Entries", 70, 0, 350);

    TH1D *hist_mass_conditioned_gen_w_candidate_chi2_simple     = new TH1D("hist_mass_conditioned_gen_w_candidate_chi2_simple", ";Mass [GeV/c^2];Entries", 50, 0, 150);
    TH1D *hist_mass_conditioned_gen_top_candidate_chi2_simple   = new TH1D("hist_mass_conditioned_gen_top_candidate_chi2_simple", ";Mass [GeV/c^2];Entries", 70, 0, 350);
    TH1D *hist_mass_conditioned_gen_w_candidate_chi2_modified   = new TH1D("hist_mass_conditioned_gen_w_candidate_chi2_modified", ";Mass [GeV/c^2];Entries", 50, 0, 150);
    TH1D *hist_mass_conditioned_gen_top_candidate_chi2_modified = new TH1D("hist_mass_conditioned_gen_top_candidate_chi2_modified", ";Mass [GeV/c^2];Entries", 70, 0, 350);
    //}}}

    //##################################################//
    //#########    Event Loop [Selection]    ###########//
    //##################################################//
    // Goal: leptons, jets, diphoton; t->b+W(jj), 1 bjet + 2 chi2 jets 
    int Nevents_pass_selection = 0;
    int check_num_bquak_is_one = 0;
    int check_num_bquak_is_two = 0;
    int check_num_bquak_is_three = 0;
    int count_bjet_is_bquark_loose = 0;
    int count_bjet_is_bquark_medium = 0;
    int count_bjet_is_bquark_tight = 0;
    int count_bjet_is_bquark = 0;
    int check_M1_20 = 0;
    int check_M20_simple = 0;
    int check_M20_modified = 0;
    int check_gen_exclude_id_999 = 0;
    int check_gen_exclude_the_same = 0;
    double accuracy_chi2_simple = 0, accuracy_chi2_modified = 0, accuracy_yfyj = 0;
    double accuracy_tqh_chi2_simple = 0, accuracy_tqh_chi2_modified = 0, accuracy_tqh_yfyj = 0;
    printf("[CHECK-0] Nevents_pass_selection = %d\n", Nevents_pass_selection);
    for(int ientry=0; ientry<nentries; ientry++){
        treeReader.flashggStdTree->GetEntry(ientry);//load data
        //if((ientry+1)%100==0 || (ientry+1)==nentries) printf("ientry = %d\n", ientry);
        //### reset, selection, Normalization factor{{{ 
        //==================================================//
        //-------------   Reset Parameters   ---------------//
        //==================================================//
        mytree.Clear();
        //==================================================//
        //--------------   Basic Selectoin   ---------------//
        //==================================================//
        //require MC events pass trigger.
        if(!treeReader.EvtInfo_passTrigger) continue;
        //control region
        if(treeReader.DiPhoInfo_mass<100 || treeReader.DiPhoInfo_mass>180) continue;
        //if( !isMCsignal && treeReader.DiPhoInfo_mass>120 && treeReader.DiPhoInfo_mass<130) continue;
        //require the quality of photons. (PT requirement)
        bool pass_leadingPhotonPT = treeReader.DiPhoInfo_leadPt > treeReader.DiPhoInfo_mass / 2.;
        bool pass_subleadingPhotonPT = treeReader.DiPhoInfo_subleadPt > treeReader.DiPhoInfo_mass / 4.;
        bool pass_photon_criteria_pt = pass_leadingPhotonPT && pass_subleadingPhotonPT;

        bool pass_leadingPhotonEta =  (treeReader.DiPhoInfo_leadEta < 1.4442) || (treeReader.DiPhoInfo_leadEta > 1.566 && treeReader.DiPhoInfo_leadEta < 2.4);
        bool pass_subleadingPhotonEta = (treeReader.DiPhoInfo_subleadEta < 1.4442) || (treeReader.DiPhoInfo_subleadEta > 1.566 && treeReader.DiPhoInfo_subleadEta < 2.4);
        bool pass_photon_criteria_eta = pass_leadingPhotonEta && pass_subleadingPhotonEta;

        bool pass_photon_criteria = pass_photon_criteria_pt && pass_photon_criteria_eta;
        if(!pass_photon_criteria) continue;
        //Others
        //if(treeReader.DiPhoInfo_mass<0) continue;
        //if( !(treeReader.jets_size>0) ) continue;
        //if(!(DiPhoInfo_leadIDMVA>0)) continue;
        //==================================================//
        //------------   Normalization factor   ------------//
        //==================================================//
        mytree.EvtInfo_totalEntry_before_preselection = nentries;
        mytree.EvtInfo_NormalizationFactor_lumi = isData ? 1. : NormalizationFactor;
        //### }}} 
        //### diphoton info{{{
        //===============================================//
        //-----------   Store Diphoton Info   -----------//
        //===============================================//
        mytree.DiPhoInfo_leadPt = treeReader.DiPhoInfo_leadPt;
        mytree.DiPhoInfo_leadEta = treeReader.DiPhoInfo_leadEta;
        mytree.DiPhoInfo_leadPhi = treeReader.DiPhoInfo_leadPhi;
        mytree.DiPhoInfo_leadE = treeReader.DiPhoInfo_leadE;
        mytree.DiPhoInfo_leadhoe = treeReader.DiPhoInfo_leadhoe;
        mytree.DiPhoInfo_leadIDMVA = treeReader.DiPhoInfo_leadIDMVA;
        mytree.DiPhoInfo_subleadPt = treeReader.DiPhoInfo_subleadPt;
        mytree.DiPhoInfo_subleadEta = treeReader.DiPhoInfo_subleadEta;
        mytree.DiPhoInfo_subleadPhi = treeReader.DiPhoInfo_subleadPhi;
        mytree.DiPhoInfo_subleadE = treeReader.DiPhoInfo_subleadE;
        mytree.DiPhoInfo_subleadhoe = treeReader.DiPhoInfo_subleadhoe;
        mytree.DiPhoInfo_subleadIDMVA = treeReader.DiPhoInfo_subleadIDMVA;
        mytree.DiPhoInfo_mass = treeReader.DiPhoInfo_mass;
        mytree.DiPhoInfo_pt = treeReader.DiPhoInfo_pt;

        TLorentzVector leading_photon, subleading_photon, diphoton;
        leading_photon.SetPtEtaPhiE(treeReader.DiPhoInfo_leadPt, treeReader.DiPhoInfo_leadEta, treeReader.DiPhoInfo_leadPhi, treeReader.DiPhoInfo_leadE);
        subleading_photon.SetPtEtaPhiE(treeReader.DiPhoInfo_subleadPt, treeReader.DiPhoInfo_subleadEta, treeReader.DiPhoInfo_subleadPhi, treeReader.DiPhoInfo_subleadE);
        diphoton = leading_photon + subleading_photon;
        mytree.DiPhoInfo_eta = diphoton.Eta();
        mytree.DiPhoInfo_phi = diphoton.Phi();
        mytree.DiPhoInfo_energy = diphoton.Energy();
        //### }}}
        //### electron selection {{{
        //==============================//
        //-----  Select Electrons  -----//
        //==============================//
        mytree.ElecInfo_Size = treeReader.ElecInfo_Size;
        std::vector<TLorentzVector> Electrons;
        bool bool_AtLeastOneElectron = treeReader.ElecInfo_Size>0 ? true : false;//treeReader.ElecInfo_Size = -999 => event without diphoton candidate
        if(bool_AtLeastOneElectron){
            for(int i=0; i<treeReader.ElecInfo_Size; i++){
                if( !treeReader.ElecInfo_EGMCutBasedIDMedium->at(i) ) continue;
                if( fabs(treeReader.ElecInfo_Eta->at(i)) > 2.4 ) continue;
                if( fabs(treeReader.ElecInfo_Eta->at(i)) > 1.4442 && fabs(treeReader.ElecInfo_Eta->at(i)) < 1.566 ) continue;
                if( fabs(treeReader.ElecInfo_Pt->at(i))  < 20  ) continue;
                //--- check deltaR(electron,photon) ---//
                TLorentzVector electron; 
                electron.SetPtEtaPhiE(treeReader.ElecInfo_Pt->at(i), treeReader.ElecInfo_Eta->at(i), treeReader.ElecInfo_Phi->at(i), treeReader.ElecInfo_Energy->at(i));
                double delta_R = electron.DeltaR(leading_photon);
                if( delta_R<0.3 ) continue;
                delta_R = electron.DeltaR(subleading_photon);
                if( delta_R<0.3 ) continue;
                //--- calculate deltaR(electron,photon/diphoton) and store info ---//
                delta_R = electron.DeltaR(diphoton);          mytree.ElecInfo_electron_diphoton_deltaR.push_back(delta_R);
                delta_R = electron.DeltaR(leading_photon);    mytree.ElecInfo_electron_leadingPhoton_deltaR.push_back(delta_R);
                delta_R = electron.DeltaR(subleading_photon); mytree.ElecInfo_electron_subleadingPhoton_deltaR.push_back(delta_R);
                //--- store information ---//
                mytree.ElecInfo_electron_pt.push_back(treeReader.ElecInfo_Pt->at(i));
                mytree.ElecInfo_electron_eta.push_back(treeReader.ElecInfo_Eta->at(i));
                mytree.ElecInfo_electron_phi.push_back(treeReader.ElecInfo_Phi->at(i));
                mytree.ElecInfo_electron_energy.push_back(treeReader.ElecInfo_Phi->at(i));
                mytree.num_electrons+=1;
                Electrons.push_back(electron);
            }
        }
        else{
                mytree.num_electrons=treeReader.ElecInfo_Size;
        }
        bool bool_AtLeastOneSelectedElectron = mytree.num_electrons>0 ? true : false;//for calculation of deltaR(e,j).
        //### }}}
        //### muon selection {{{
        //==========================//
        //-----  Select Muons  -----//
        //==========================//
        mytree.MuonInfo_Size = treeReader.MuonInfo_Size;
        std::vector<TLorentzVector> Muons;
        bool bool_AtLeastOneMuon = treeReader.MuonInfo_Size>0 ? true : false;//treeReader.MuonInfo_Size = -999 => event without diphoton candidate
        if(bool_AtLeastOneMuon){
            for(int i=0; i<treeReader.MuonInfo_Size; i++){
                if( !treeReader.MuonInfo_CutBasedIdTight->at(i) ) continue;
                if( fabs(treeReader.MuonInfo_Eta->at(i)) > 2.4 ) continue;
                if( fabs(treeReader.MuonInfo_Eta->at(i)) > 1.4442 && fabs(treeReader.MuonInfo_Eta->at(i)) < 1.566 ) continue;
                if( fabs(treeReader.MuonInfo_Pt->at(i))  < 20  ) continue;
                if( treeReader.MuonInfo_PFIsoDeltaBetaCorrR04->at(i)  > 0.25  ) continue;
                //--- check deltaR(muon,photon) ---//
                TLorentzVector muon; 
                muon.SetPtEtaPhiE(treeReader.MuonInfo_Pt->at(i), treeReader.MuonInfo_Eta->at(i), treeReader.MuonInfo_Phi->at(i), treeReader.MuonInfo_Energy->at(i));
                double delta_R = muon.DeltaR(leading_photon);
                if( delta_R<0.3 ) continue;
                delta_R = muon.DeltaR(subleading_photon);
                if( delta_R<0.3 ) continue;
                //--- calculate deltaR(muon,diphoton) and store info ---//
                delta_R = muon.DeltaR(diphoton);          mytree.MuonInfo_muon_diphoton_deltaR.push_back(delta_R);
                delta_R = muon.DeltaR(leading_photon);    mytree.MuonInfo_muon_leadingPhoton_deltaR.push_back(delta_R);
                delta_R = muon.DeltaR(subleading_photon); mytree.MuonInfo_muon_subleadingPhoton_deltaR.push_back(delta_R);
                //--- store information ---//
                mytree.MuonInfo_muon_pt.push_back(treeReader.MuonInfo_Pt->at(i));
                mytree.MuonInfo_muon_eta.push_back(treeReader.MuonInfo_Eta->at(i));
                mytree.MuonInfo_muon_phi.push_back(treeReader.MuonInfo_Phi->at(i));
                mytree.MuonInfo_muon_energy.push_back(treeReader.MuonInfo_Phi->at(i));
                
                mytree.num_muons+=1;
                Muons.push_back(muon);
            }
        }
        else{
                mytree.num_muons=treeReader.MuonInfo_Size;
        }
        bool bool_AtLeastOneSelectedMuon = mytree.num_muons>0 ? true : false;//for calculation of deltaR(mu,j).
        mytree.num_leptons = mytree.num_electrons + mytree.num_muons;
        //printf("num_leptons = %d\n", mytree.num_leptons);
        //### }}}
        //### jet selection {{{
        //=========================//
        //-----  Select Jets  -----//
        //=========================//
        mytree.jets_size = treeReader.jets_size;
        std::vector<TLorentzVector> Jets;
        std::vector<int> Jets_GenFalavor;
        bool bool_AtLeastOneJet = treeReader.jets_size>0 ? true : false;//treeReader.jets_size = -999 => event without diphoton candidate
        if(bool_AtLeastOneJet){
            for(int i=0; i<treeReader.jets_size; i++){
                //flashgg::Tight2017 jet ID #Already applied flashgg package.
                if( fabs(treeReader.JetInfo_Eta->at(i)) > 2.4 ) continue;
                if( fabs(treeReader.JetInfo_Pt->at(i))  < 25  ) continue;
                //--- check deltaR(jet,photon) ---//
                TLorentzVector jet; 
                jet.SetPtEtaPhiE(treeReader.JetInfo_Pt->at(i), treeReader.JetInfo_Eta->at(i), treeReader.JetInfo_Phi->at(i), treeReader.JetInfo_Energy->at(i));
                double delta_R = jet.DeltaR(leading_photon);
                if( delta_R<0.4 ) continue;
                delta_R = jet.DeltaR(subleading_photon);
                if( delta_R<0.4 ) continue;
                bool bool_passJetLeptonSeparation = true;//if no leptons selected, the jet pass the delta_R criterion automatically.
                //--- check deltaR(jet,e) ---//
                if(bool_AtLeastOneSelectedElectron){
                    for(int i=0; i<mytree.num_electrons; i++){
                        delta_R = jet.DeltaR(Electrons.at(i));
                        if( delta_R<0.4 ) bool_passJetLeptonSeparation = false;
                    }
                }
                if(!bool_passJetLeptonSeparation) continue;//if not pass, reject the jet.
                //--- check deltaR(jet,mu) ---//
                if(bool_AtLeastOneSelectedMuon){
                    for(int i=0; i<mytree.num_muons; i++){
                        delta_R = jet.DeltaR(Muons.at(i));
                        if( delta_R<0.4 ) bool_passJetLeptonSeparation = false;
                    }
                }
                if(!bool_passJetLeptonSeparation) continue;//if not pass, reject the jet.
                //--- calculate deltaR(jet,diphoton) and store info ---//
                delta_R = jet.DeltaR(diphoton);          mytree.JetInfo_jet_diphoton_deltaR.push_back(delta_R);
                delta_R = jet.DeltaR(leading_photon);    mytree.JetInfo_jet_leadingPhoton_deltaR.push_back(delta_R);
                delta_R = jet.DeltaR(subleading_photon); mytree.JetInfo_jet_subleadingPhoton_deltaR.push_back(delta_R);
                //--- store information ---//
                mytree.JetInfo_jet_pt.push_back(treeReader.JetInfo_Pt->at(i));
                mytree.JetInfo_jet_eta.push_back(treeReader.JetInfo_Eta->at(i));
                mytree.JetInfo_jet_phi.push_back(treeReader.JetInfo_Phi->at(i));
                mytree.JetInfo_jet_energy.push_back(treeReader.JetInfo_Phi->at(i));
                
                mytree.JetInfo_jet_pfDeepCSVJetTags_probb.push_back(treeReader.JetInfo_pfDeepCSVJetTags_probb->at(i));
                mytree.JetInfo_jet_pfDeepCSVJetTags_probbb.push_back(treeReader.JetInfo_pfDeepCSVJetTags_probbb->at(i));
                mytree.num_jets+=1;
                Jets.push_back(jet);
                Jets_GenFalavor.push_back(treeReader.JetInfo_GenFlavor->at(i));
            }
        }
        else{
                mytree.num_jets=treeReader.jets_size;
        }
        //### }}}
        //### Hadronic event selection{{{
        //================================================//
        //-----------   Add Event Selection    -----------//
        //================================================//
        //hadronic condition
        if(mytree.num_leptons>0) continue;
        if(mytree.num_jets<4) continue;
        //###}}}

        int num_bquark=0, num_light_quark=0;
        for(int i=0; i<treeReader.GenPartInfo_size; i++){
            if(treeReader.GenPartInfo_Status->at(i)==23 && abs(treeReader.GenPartInfo_PdgID->at(i))==5) num_bquark+=1;
            if(treeReader.GenPartInfo_Status->at(i)==23 && abs(treeReader.GenPartInfo_PdgID->at(i))<5)  num_light_quark+=1;
        }
        if(num_bquark==1) check_num_bquak_is_one += 1;
        if(num_bquark==2) check_num_bquak_is_two += 1;
        if(num_bquark==3) check_num_bquak_is_three += 1;

        hist_num_gen_bquark->Fill(num_bquark);
        hist_num_gen_light_quark->Fill(num_light_quark);
        hist_num_selected_jets->Fill(mytree.num_jets);
        hist_mass_diphoton->Fill(diphoton.M());

        //================================================//
        //-----------   top reconstruction     -----------//
        //================================================//
        TLorentzVector bjet;
        TLorentzVector genParticle_bjet;
        //-------------------------//
        std::vector<int> id_jet_chi2_simple(2);
        std::vector<int> index_jet_chi2_simple(2);
        std::vector<int> index_genParticle_jet_chi2_simple(2);
        std::vector<TLorentzVector> jet_chi2_simple(2);
        std::vector<TLorentzVector> genParticle_jet_chi2_simple(2);
        //-------------------------//
        std::vector<int> id_jet_chi2_modified(2);
        std::vector<int> index_jet_chi2_modified(2);
        std::vector<int> index_genParticle_jet_chi2_modified(2);
        std::vector<TLorentzVector> jet_chi2_modified(2);
        std::vector<TLorentzVector> genParticle_jet_chi2_modified(2);
        //-------------------------//
        /*
        // Check GenInfo{{{
        if(num_bquark>2){
        printf("\n");
        printf("[GenCheck] GenPartInfo_size = %d\n", treeReader.GenPartInfo_size);
        for(int i=0; i<treeReader.GenPartInfo_size; i++){
            printf("Status = %3d, ", treeReader.GenPartInfo_Status->at(i));
            printf("PdgID = %3d, ", treeReader.GenPartInfo_PdgID->at(i));
            printf("Pt = %6.2f, ", treeReader.GenPartInfo_Pt->at(i));
            printf("Eta = %9.2f, ", treeReader.GenPartInfo_Eta->at(i));
            printf("Phi = %6.2f, ", treeReader.GenPartInfo_Phi->at(i));
            printf("Mass = %6.2f, ", treeReader.GenPartInfo_Mass->at(i));
            printf("isHardProcess = %3d, ", treeReader.GenPartInfo_isHardProcess->at(i) ? 1 : 0);
            printf("isPromptFinalState = %3d, ", treeReader.GenPartInfo_isPromptFinalState->at(i) ? 1 : 0);
            printf("MomPdgID = %5d, ", treeReader.GenPartInfo_MomPdgID->at(i));
            printf("MomStatus = %3d\n", treeReader.GenPartInfo_MomStatus->at(i));
        }
        }
        //}}}
        */
        //### b-jet{{{
        //----- b-jet -----//
        int num_bjets_loose = 0, num_bjets_medium = 0;
        int num_bjets_tight = 0, index_bjet = -999;
        //TLorentzVector bjet_tight, bjet_loose, bjet_medium;
        std::vector<TLorentzVector> bjets_tight, bjets_loose, bjets_medium;
        for(int i=0; i<mytree.num_jets; ++i){
            if(mytree.JetInfo_jet_pfDeepCSVJetTags_probb.at(i)+mytree.JetInfo_jet_pfDeepCSVJetTags_probbb.at(i) >= pfDeepCSVJetTags_loose){
                num_bjets_loose += 1;
                bjets_loose.push_back(Jets[i]);
            }
            if(mytree.JetInfo_jet_pfDeepCSVJetTags_probb.at(i)+mytree.JetInfo_jet_pfDeepCSVJetTags_probbb.at(i) >= pfDeepCSVJetTags_medium){
                num_bjets_medium += 1;
                bjets_medium.push_back(Jets[i]);
            }
            if(mytree.JetInfo_jet_pfDeepCSVJetTags_probb.at(i)+mytree.JetInfo_jet_pfDeepCSVJetTags_probbb.at(i) >= pfDeepCSVJetTags_tight){
                num_bjets_tight += 1;
                bjets_tight.push_back(Jets[i]);
                index_bjet = i;
            }
        }//end of looping jets
        hist_num_bjets_loose->Fill(num_bjets_loose);
        hist_num_bjets_medium->Fill(num_bjets_medium);
        hist_num_bjets_tight->Fill(num_bjets_tight);

        //--- counting correct rate in when num_bjet==1 ---//
        if(num_bjets_loose==1){
            bool is_b_quark = CheckBJetID(bjets_loose[0], treeReader.GenPartInfo_size,\
                              treeReader.GenPartInfo_MomPdgID, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass,\
                              treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID);
            if(is_b_quark){
                count_bjet_is_bquark_loose += 1;
                hist_num_bjets_loose_matched -> Fill(num_bjets_loose);
            }
        }
        if(num_bjets_medium==1){
            bool is_b_quark = CheckBJetID(bjets_medium[0], treeReader.GenPartInfo_size,\
                              treeReader.GenPartInfo_MomPdgID, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass,\
                              treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID);
            if(is_b_quark){
                count_bjet_is_bquark_medium += 1;
                hist_num_bjets_medium_matched -> Fill(num_bjets_medium);
            }
        }
        if(num_bjets_tight==1){
            bool is_b_quark = CheckBJetID(bjets_tight[0], treeReader.GenPartInfo_size,\
                              treeReader.GenPartInfo_MomPdgID, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass,\
                              treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID);
            if(is_b_quark){
                count_bjet_is_bquark_tight += 1;
                hist_num_bjets_tight_matched -> Fill(num_bjets_tight);
            }
        }

        //if(num_bjets_loose != 1) continue;//exactly 1 loose bjet candidate
        //if(num_bjets_loose < 1) continue;//at least 1 loose bjet candidate
        //bjet = bjets_loose[0];

        if(num_bjets_tight != 1) continue;//exactly 1 tight bjet candidate
        //if(num_bjets_tight < 1) continue;//at least 1 tight bjet candidate
        bjet = bjets_tight[0];
        
        //if(num_bjets_medium != 1) continue;//exactly 1 medium bjet candidate
        //if(num_bjets_medium < 1) continue;//at least 1 medium bjet candidate
        //bjet = bjets_medium[0];
        //### }}}
        //### simple & modified chi-2 methods{{{
        //----- chi-2 simple -----//
        double chi2_min = 999;
        for(int i=0; i<mytree.num_jets; ++i){
            if(i==index_bjet) continue;//bypass bjet
            for(int j=i+1; j<mytree.num_jets; ++j){
                if(j==index_bjet) continue;//bypass bjet
                TLorentzVector w_candidate = Jets[i] + Jets[j];
                double w_mass = w_candidate.M();
                TLorentzVector top_candidate = w_candidate + bjet;
                double t_mass = top_candidate.M();
                double chi2 = Chi2_calculator_simple(w_mass, t_mass);
                if(chi2 < chi2_min){
                    index_jet_chi2_simple[0] = i;
                    index_jet_chi2_simple[1] = j;
                    jet_chi2_simple[0] = Jets[i];
                    jet_chi2_simple[1] = Jets[j];
                    id_jet_chi2_simple[0] = Jets_GenFalavor[i];
                    id_jet_chi2_simple[1] = Jets_GenFalavor[j];
                    chi2_min = chi2;
                }
            }
        }//end of looping jets
        //----- chi-2 modified -----//
        chi2_min = 999;
        for(int i=0; i<mytree.num_jets; ++i){
            if(i==index_bjet) continue;//bypass bjet
            for(int j=i+1; j<mytree.num_jets; ++j){
                if(j==index_bjet) continue;//bypass bjet
                TLorentzVector w_candidate = Jets[i] + Jets[j];
                double w_mass = w_candidate.M();
                TLorentzVector top_candidate = w_candidate + bjet;
                double t_mass = top_candidate.M();
                double chi2 = Chi2_calculator_modified(w_mass, t_mass);
                if(chi2 < chi2_min){
                    index_jet_chi2_modified[0] = i;
                    index_jet_chi2_modified[1] = j;
                    jet_chi2_modified[0] = Jets[i];
                    jet_chi2_modified[1] = Jets[j];
                    id_jet_chi2_modified[0] = Jets_GenFalavor[i];
                    id_jet_chi2_modified[1] = Jets_GenFalavor[j];
                    chi2_min = chi2;
                }
            }
        }//end of looping jets
        //### }}}
        //### GenMatching{{{
        //----- MC truth -----//
        //========================================//
        //-----  Start GenMatching for Jets  -----//
        //========================================//
        //### GenMatching: find the gen particle (MC truth) for each jet (reconstructed). 
        //### pdgID: (1, 2, 3, 4, 5, 6) = (d, u, s, c, b, t)
        //### This is the simplest version. Identify the corresponding gen particle by selecting smallest deltaR(gen, jet).
        //### One can try to print out the info of pt, eta, phi, energy, and deltaR of jet and corresponding gen particle to see if they are matched.
        std::vector<int> index_GenParticles;
        int id_genParticle_bjet=-999;
        int id_genParticle_jet_chi2_simple[2]; std::fill_n(id_genParticle_jet_chi2_simple, 2, -999);
        int id_genParticle_jet_chi2_modified[2]; std::fill_n(id_genParticle_jet_chi2_modified, 2, -999);
        double delta_R = 0;
        //printf("ientry = %d\n", ientry);
        genParticle_bjet = GetGenParticle(bjet, treeReader.GenPartInfo_size, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass, treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID, index_GenParticles, id_genParticle_bjet);

        //int momPdgID = abs(treeReader.GenPartInfo_MomPdgID->at(index_GenParticles[0])) ;
        //if( abs(id_genParticle_bjet)==5 ) printf("[debug] MomPdgID = %d\n", abs(treeReader.GenPartInfo_MomPdgID->at(index_GenParticles[0]))  );
        //if( abs(id_genParticle_bjet)==5 && abs(treeReader.GenPartInfo_MomPdgID->at(index_GenParticles[0]))==6 )
        //    printf("[debug] checked!\n");

        if( abs(id_genParticle_bjet)==5 && abs(treeReader.GenPartInfo_MomPdgID->at(index_GenParticles[0]))==6 ) count_bjet_is_bquark+=1;
        //if( abs(id_genParticle_bjet)==5 ) count_bjet_is_bquark+=1;
        //---
        genParticle_jet_chi2_simple[0] = GetGenParticle(jet_chi2_simple[0], treeReader.GenPartInfo_size, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass, treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID, index_GenParticles, id_genParticle_jet_chi2_simple[0]);
        //---
        genParticle_jet_chi2_simple[1] = GetGenParticle(jet_chi2_simple[1], treeReader.GenPartInfo_size, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass, treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID, index_GenParticles, id_genParticle_jet_chi2_simple[1]);
        //---
        genParticle_jet_chi2_modified[0] = GetGenParticle(jet_chi2_modified[0], treeReader.GenPartInfo_size, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass, treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID, index_GenParticles, id_genParticle_jet_chi2_modified[0]);
        //---
        genParticle_jet_chi2_modified[1] = GetGenParticle(jet_chi2_modified[1], treeReader.GenPartInfo_size, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass, treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID, index_GenParticles, id_genParticle_jet_chi2_modified[1]);
        //### }}}
        
        bool isMatched;//boolean used to check matching
        //### Reconstruct Mass (M2){{{
        //----- reco -----//
        TLorentzVector w_candidate_chi2_simple = jet_chi2_simple[0] + jet_chi2_simple[1];
        TLorentzVector top_candidate_chi2_simple = w_candidate_chi2_simple + bjet;
        mytree.Mass_w_candidate_chi2_simple = w_candidate_chi2_simple.M();
        mytree.Mass_top_candidate_chi2_simple = top_candidate_chi2_simple.M();
        hist_mass_w_candidate_chi2_simple->Fill(w_candidate_chi2_simple.M());
        hist_mass_top_candidate_chi2_simple->Fill(top_candidate_chi2_simple.M());
        //--------------------
        TLorentzVector w_candidate_chi2_modified = jet_chi2_modified[0] + jet_chi2_modified[1];
        TLorentzVector top_candidate_chi2_modified = w_candidate_chi2_modified + bjet;
        mytree.Mass_w_candidate_chi2_modified = w_candidate_chi2_modified.M();
        mytree.Mass_top_candidate_chi2_modified = top_candidate_chi2_modified.M();
        hist_mass_w_candidate_chi2_modified->Fill(w_candidate_chi2_modified.M());
        hist_mass_top_candidate_chi2_modified->Fill(top_candidate_chi2_modified.M());
        //----- gen -----//
        //reject candidates according to the MC truth
        //1. reject candidate with id_genParticle = -999 (soft jet, not from hard process)
        //2. reject candidate with id_genParticle_jet == 5 (w_boson cannot decay to b jet)
        //3. reject candidate with 2 jets having same genParticle
        TLorentzVector gen_w_candidate_chi2_simple = genParticle_jet_chi2_simple[0] + genParticle_jet_chi2_simple[1];
        TLorentzVector gen_top_candidate_chi2_simple = gen_w_candidate_chi2_simple + genParticle_bjet;
        hist_mass_gen_w_candidate_chi2_simple->Fill(gen_w_candidate_chi2_simple.M());
        hist_mass_gen_top_candidate_chi2_simple->Fill(gen_top_candidate_chi2_simple.M());
        //----- check -----//
        if(gen_w_candidate_chi2_simple.M() < 20){
            check_M20_simple += 1;
            if( !(id_genParticle_jet_chi2_simple[0]==-999 || id_genParticle_jet_chi2_simple[1]==-999) ){
                check_gen_exclude_id_999 += 1;
                if( !(genParticle_jet_chi2_simple[0].Pt() == genParticle_jet_chi2_simple[1].Pt()) ){
                    check_gen_exclude_the_same += 1;
                    //kinematics_report("jet[0]", jet_chi2_simple[0], id_jet_chi2_simple[0], "gen[0]", genParticle_jet_chi2_simple[0], id_genParticle_jet_chi2_simple[0]);
                    //kinematics_report("jet[1]", jet_chi2_simple[1], id_jet_chi2_simple[1], "gen[1]", genParticle_jet_chi2_simple[1], id_genParticle_jet_chi2_simple[1]);
                    //printf("\n");
                }
            }
        }

        isMatched = isMatched_with_Gen_tbw(treeReader.GenPartInfo_MomPdgID, index_GenParticles[0], index_GenParticles[1], index_GenParticles[2] );//bjet, jet12(simple)
        if( isMatched ){
            mytree.Mass_gen_w_candidate_chi2_simple = gen_w_candidate_chi2_simple.M();
            mytree.Mass_gen_top_candidate_chi2_simple = gen_top_candidate_chi2_simple.M();
            hist_mass_conditioned_gen_w_candidate_chi2_simple->Fill(gen_w_candidate_chi2_simple.M());
            hist_mass_conditioned_gen_top_candidate_chi2_simple->Fill(gen_top_candidate_chi2_simple.M());

            accuracy_chi2_simple += 1;

            if(gen_w_candidate_chi2_simple.M() < 20){// debug purpose
                //printf("\nCheck:\n");
                //kinematics_report("bjet", bjet, id_bjet, "genP", genParticle_bjet, id_genParticle_bjet);
                //kinematics_report("jet_chi2_simple[0]", jet_chi2_simple[0], id_jet_chi2_simple[0], "MatchedGenParticle", genParticle_jet_chi2_simple[0], id_genParticle_jet_chi2_simple[0]);
                //kinematics_report("jet_chi2_simple[1]", jet_chi2_simple[1], id_jet_chi2_simple[1], "MatchedGenParticle", genParticle_jet_chi2_simple[1], id_genParticle_jet_chi2_simple[1]);
                //printf("[INFO] M_gen_jj  = %6.2f\n", gen_w_candidate_chi2_simple.M());
                //printf("[INFO] M_gen_bjj = %6.2f\n", gen_top_candidate_chi2_simple.M());
            }
        }
        //--------------------
        TLorentzVector gen_w_candidate_chi2_modified = genParticle_jet_chi2_modified[0] + genParticle_jet_chi2_modified[1];
        TLorentzVector gen_top_candidate_chi2_modified = gen_w_candidate_chi2_modified + genParticle_bjet;
        hist_mass_gen_w_candidate_chi2_modified->Fill(gen_w_candidate_chi2_modified.M());
        hist_mass_gen_top_candidate_chi2_modified->Fill(gen_top_candidate_chi2_modified.M());

        isMatched = isMatched_with_Gen_tbw(treeReader.GenPartInfo_MomPdgID, index_GenParticles[0], index_GenParticles[3], index_GenParticles[4] );//bjet, jet12(modified)
        if( isMatched ){
            mytree.Mass_gen_w_candidate_chi2_modified = gen_w_candidate_chi2_modified.M();
            mytree.Mass_gen_top_candidate_chi2_modified = gen_top_candidate_chi2_modified.M();
            hist_mass_conditioned_gen_w_candidate_chi2_modified->Fill(gen_w_candidate_chi2_modified.M());
            hist_mass_conditioned_gen_top_candidate_chi2_modified->Fill(gen_top_candidate_chi2_modified.M());
            accuracy_chi2_modified += 1;
        }
        //--------------------
        //### }}}
        
        /*
        //##### check gen of rest jets{{{
        for(int i=0; i<mytree.num_jets; ++i){
            if(i==index_bjet || i==index_jet_chi2_simple[0] || i==index_jet_chi2_simple[1]) continue;//skip the jets for bjj
            kinematics_info("jet", Jets[i]);
            print_matched_gen_info(Jets[i], treeReader.GenPartInfo_size,\
                                   treeReader.GenPartInfo_MomPdgID, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass,\
                                   treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID);
        }
        //}}}
        */

        //### Reconstruct Mass (M1){{{
        int index_q = -999;
        double M1 = -999;
        TLorentzVector top_fcnh, jet_q;

        //--- simple chi2 ---//
        top_fcnh = GetBestM1(M1, mytree.num_jets, index_bjet, index_jet_chi2_simple, diphoton, Jets, index_q, jet_q);
        if(M1 != -999 && M1<20) check_M1_20+=1;
        if(M1 != -999){
            hist_mass_top_fcnh_simple->Fill(M1);
            bool is_tqh_quark = is_this_tqh_quark(jet_q, treeReader.GenPartInfo_size,\
                                treeReader.GenPartInfo_MomPdgID, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass,\
                                treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID);
            if(is_tqh_quark) accuracy_tqh_chi2_simple += 1;
        }
        
        //--- modified chi2 ---//
        top_fcnh = GetBestM1(M1, mytree.num_jets, index_bjet, index_jet_chi2_modified, diphoton, Jets, index_q, jet_q);
        if(M1 != -999){
            hist_mass_top_fcnh_modified->Fill(M1);
            bool is_tqh_quark = is_this_tqh_quark(jet_q, treeReader.GenPartInfo_size,\
                                treeReader.GenPartInfo_MomPdgID, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass,\
                                treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID);
            if(is_tqh_quark) accuracy_tqh_chi2_modified += 1;
        }

        // old code{{{
        ////record all the possible combinations
        //std::vector<double> top_fcnh_chi2;
        //std::vector<TLorentzVector> top_fcnh_candidates;
        //for(int i=0; i<mytree.num_jets; ++i){
        //    if(i==index_bjet || i==index_jet_chi2_simple[0] || i==index_jet_chi2_simple[1]) continue;//skip the jets for bjj
        //    TLorentzVector top_fcnh_tmp = diphoton + Jets[i];
        //    double chi2 = (top_fcnh_tmp.M() - top_quark_mass) * (top_fcnh_tmp.M() - top_quark_mass);
        //    top_fcnh_chi2.push_back(chi2);
        //    top_fcnh_candidates.push_back(top_fcnh_tmp);
        //}
        ////choose the candidate with Mass closest to top mass
        //int index_min_M1 = -999;
        //double chi2_min_M1 = 40000;//200^2 > 178^2 ~ (x-top_mass)^2 //NOTE: will bound the range of M1!!!
        //for(int i=0; i<top_fcnh_candidates.size(); ++i){ if(top_fcnh_chi2[i]<chi2_min_M1){ index_min_M1 = i; chi2_min_M1 = top_fcnh_chi2[i]; } }
        //if(index_min_M1 == -999) continue;
        //hist_mass_top_fcnh->Fill(top_fcnh_candidates[index_min_M1].M());
        //if(top_fcnh_candidates[index_min_M1].M()<20) check_M1_20+=1;
        ////### Debug section
        ////### Conclusion: index_min_M1 remaining -999 indicates that no suitable M1 candidate left.
        ////if(top_fcnh_candidates[index_min_M1].M()<20){
        ////    check_M1_20+=1;
        ////    printf("\n[Check] ientry = %d\n", ientry);
        ////    printf("[Check] index = %d/%d\n", index_min_M1, top_fcnh_candidates.size());
        ////    printf("[Check] chi2_min_M1 = %f\n", chi2_min_M1);
        ////    printf("[Check] MGG = %f\n", diphoton.M());
        ////    printf("[Check] Mass = %f\n", top_fcnh_candidates[index_min_M1].M());
        ////    int counter = 0;
        ////    for(int i=0; i<mytree.num_jets; ++i){
        ////        if(i==index_bjet || i==index_jet_chi2_simple[0] || i==index_jet_chi2_simple[1]) continue;//skip the jets for bjj
        ////        printf("[Debug] (%d/%d) chi2 = %f\n", i+1, mytree.num_jets, top_fcnh_chi2[counter]);
        ////        counter+=1;
        ////        kinematics_info("pho", diphoton);
        ////        kinematics_info("jet", Jets[i]);
        ////        TLorentzVector top = diphoton + Jets[i];
        ////        kinematics_info("top", top);
        ////        double chi2 = (top.M() - top_quark_mass) * (top.M() - top_quark_mass);
        ////        printf("[Debug] M1 = %6.2f, M_ref = %6.2f, chi2 = %f\n", top.M(), top_quark_mass, chi2);
        ////    }
        ////};
        //}}}
        //}}}
        //### Yueh-Feng's method{{{
        //Quick check{{{
        //printf("ientry=%d\n", ientry);
        //printf("index_bjet=%d\n", index_bjet);
        //for(int i=0; i<mytree.num_jets; ++i){
        //    kinematics_info(Form("jet-%d",i), Jets[i]);
        //}
        //}}}
        // combinations from leading jets{{{
        std::vector<TLorentzVector> leading_jets(3);
        if(index_bjet == 0){
            leading_jets[0] = Jets[1];
            leading_jets[1] = Jets[2];
            leading_jets[2] = Jets[3];
        } else if(index_bjet == 1){
            leading_jets[0] = Jets[0];
            leading_jets[1] = Jets[2];
            leading_jets[2] = Jets[3];
        } else if(index_bjet == 2){
            leading_jets[0] = Jets[0];
            leading_jets[1] = Jets[1];
            leading_jets[2] = Jets[3];
        } else{
            leading_jets[0] = Jets[0];
            leading_jets[1] = Jets[1];
            leading_jets[2] = Jets[2];
        }
        std::vector<TLorentzVector> ggj(3);
        std::vector<TLorentzVector> wjj(3);
        std::vector<TLorentzVector> bjj(3);
        for(int i=0; i<3; ++i) ggj[i] = diphoton + leading_jets[i];
        wjj[0] = leading_jets[1] + leading_jets[2];
        wjj[1] = leading_jets[0] + leading_jets[2];
        wjj[2] = leading_jets[0] + leading_jets[1];
        bjj[0] = wjj[0] + bjet;
        bjj[1] = wjj[1] + bjet;
        bjj[2] = wjj[2] + bjet;
        //}}}
        // invariant mass of best candidate{{{
        std::vector<double> invm_w(3);
        std::vector<double> invm_ggj(3);
        std::vector<double> invm_bjj(3);
        std::vector<double> t1t2_angle(3);
        std::vector<double> M1M2_criteria(3);

        for(int i=0; i<3; ++i){
            invm_w[i] = wjj[i].M();
            invm_ggj[i] = ggj[i].M();
            invm_bjj[i] = bjj[i].M();
            t1t2_angle[i] = ggj[i].Angle(bjj[i].Vect());
            M1M2_criteria[i] = GetM1M2_ratio(invm_ggj[i], invm_bjj[i]);
        }

        int minimum_index = -999;
        double minimum_mass_ratio = 999;
        for(int i=0; i<3; ++i){
            if(M1M2_criteria[i]<minimum_mass_ratio){
                minimum_mass_ratio = M1M2_criteria[i];
                minimum_index = i;
            }
        }

        hist_mass_w_candidate_yfyj->Fill(invm_w[minimum_index]);
        hist_mass_t1_candidate_yfyj->Fill(invm_ggj[minimum_index]);
        hist_mass_t2_candidate_yfyj->Fill(invm_bjj[minimum_index]);
        //}}}
        //Gen-Matching{{{
        int id_genParticles[2]; std::fill_n(id_genParticles, 2, -999);
        std::vector<TLorentzVector> leading_jets_genParticles(2);

        if(minimum_index == 0){
            leading_jets_genParticles[0] = GetGenParticle(leading_jets[1], treeReader.GenPartInfo_size, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass, treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID, index_GenParticles, id_genParticles[0]);
            leading_jets_genParticles[1] = GetGenParticle(leading_jets[2], treeReader.GenPartInfo_size, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass, treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID, index_GenParticles, id_genParticles[1]);
        } else if(minimum_index == 1){
            leading_jets_genParticles[0] = GetGenParticle(leading_jets[0], treeReader.GenPartInfo_size, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass, treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID, index_GenParticles, id_genParticles[0]);
            leading_jets_genParticles[1] = GetGenParticle(leading_jets[2], treeReader.GenPartInfo_size, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass, treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID, index_GenParticles, id_genParticles[1]);
        } else{
            leading_jets_genParticles[0] = GetGenParticle(leading_jets[0], treeReader.GenPartInfo_size, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass, treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID, index_GenParticles, id_genParticles[0]);
            leading_jets_genParticles[1] = GetGenParticle(leading_jets[1], treeReader.GenPartInfo_size, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass, treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID, index_GenParticles, id_genParticles[1]);
        }
        //}}}
        //Estimate accuracy{{{
        //reject candidates according to the MC truth
        //1. reject candidate with id_genParticle = -999 (soft jet, not from hard process)
        //2. reject candidate with id_genParticle_jet == 5 (w_boson cannot decay to b jet)
        //3. reject candidate with 2 jets having same genParticle
        TLorentzVector gen_w_candidate_yfyj = leading_jets_genParticles[0] + leading_jets_genParticles[1];
        TLorentzVector gen_top_candidate_yfyj = gen_w_candidate_yfyj + genParticle_bjet;
        hist_mass_gen_w_candidate_yfyj->Fill(gen_w_candidate_yfyj.M());
        hist_mass_gen_t2_candidate_yfyj->Fill(gen_top_candidate_yfyj.M());

        isMatched = isMatched_with_Gen_tbw(treeReader.GenPartInfo_MomPdgID, index_GenParticles[0], index_GenParticles[5], index_GenParticles[6] );//bjet, jet12(modified)
        if( isMatched ){
            hist_mass_conditioned_gen_w_candidate_yfyj->Fill(gen_w_candidate_yfyj.M());
            hist_mass_conditioned_gen_t2_candidate_yfyj->Fill(gen_top_candidate_yfyj.M());
            accuracy_yfyj += 1;
        }

        bool is_tqh_quark = is_this_tqh_quark(leading_jets[minimum_index], treeReader.GenPartInfo_size,\
                            treeReader.GenPartInfo_MomPdgID, treeReader.GenPartInfo_Pt, treeReader.GenPartInfo_Eta, treeReader.GenPartInfo_Phi, treeReader.GenPartInfo_Mass,\
                            treeReader.GenPartInfo_Status, treeReader.GenPartInfo_PdgID);
        if(is_tqh_quark) accuracy_tqh_yfyj += 1;

        //}}}
        //}}}
        //### Store EventInfo{{{
        //================================================//
        //-----------   Store EventPar Info    -----------//
        //================================================//
        mytree.EvtInfo_NPu = treeReader.EvtInfo_NPu;
        mytree.EvtInfo_Rho = treeReader.EvtInfo_Rho;
        mytree.EvtInfo_NVtx = treeReader.EvtInfo_NVtx;
        mytree.EvtInfo_genweight = treeReader.EvtInfo_genweight;
        //}}}
        //### Event Counting{{{
        //==================================================//
        //-------------   Event Counting     ---------------//
        //==================================================//
        Nevents_pass_selection += 1;
        mytree.Fill();
        if(ientry == nentries - 1) printf("[CHECK-1] Nevents_pass_selection = %d\n", Nevents_pass_selection);
        //}}}
        
    }// End of event loop.
    //==================================================//
    //---------------------  Report  -------------------//
    //==================================================//
    //### performance: width, accuracy{{{
    printf("[CHECK-2] Nevents_pass_selection = %d (%6.2f)\n", Nevents_pass_selection, 100 * (double)Nevents_pass_selection / (double)Nevents_pass_selection);
    accuracy_chi2_simple /= (double) Nevents_pass_selection;
    accuracy_chi2_modified /= (double) Nevents_pass_selection;
    accuracy_yfyj /= (double) Nevents_pass_selection;
    accuracy_tqh_chi2_simple /= (double) Nevents_pass_selection;
    accuracy_tqh_chi2_modified /= (double) Nevents_pass_selection;
    accuracy_tqh_yfyj /= (double) Nevents_pass_selection;
    double percentage_bjet_is_bquark = (double) count_bjet_is_bquark / (double) Nevents_pass_selection;
    int entries_bquark = hist_num_gen_bquark->GetEntries();
    printf("[CHECK] num_bquark = 1 (%6.3f%%)\n", 100*(double)check_num_bquak_is_one/(double)entries_bquark);
    printf("[CHECK] num_bquark = 2 (%6.3f%%)\n", 100*(double)check_num_bquak_is_two/(double)entries_bquark);
    printf("[CHECK] num_bquark = 3 (%6.3f%%)\n", 100*(double)check_num_bquak_is_three/(double)entries_bquark);
    printf("[CHECK] gen_reco_W M < 20:    %6d (%5.2f %%) (%6.2f %%)\n", check_M20_simple, 100 * check_M20_simple/(double)Nevents_pass_selection, 100 * (double)check_M20_simple/(double)check_M20_simple);
    printf("[CHECK] gen id != -999:       %6d (%5.2f %%) (%5.2f %%)\n", check_gen_exclude_id_999, 100 * check_gen_exclude_id_999/(double)Nevents_pass_selection, 100 * (double)check_gen_exclude_id_999/(double)check_M20_simple);
    printf("[CHECK] exclude the same gen: %6d (%5.2f %%) (%5.2f %%)\n", check_gen_exclude_the_same, 100 * check_gen_exclude_the_same/(double)Nevents_pass_selection, 100 * (double)check_gen_exclude_the_same/(double)check_M20_simple);
    printf("[CHECK] M1 < 20: %d\n", check_M1_20);
    printf("[INFO] percentage_bjet_is_bquark = %f\n", percentage_bjet_is_bquark);
    printf("[INFO] accuracy_chi2_simple = %6.4f\n", accuracy_chi2_simple);
    printf("[INFO] accuracy_chi2_modified = %6.4f\n", accuracy_chi2_modified);
    printf("[INFO] accuracy_yfyj = %6.4f\n", accuracy_yfyj);
    printf("[INFO] accuracy_tqh_chi2_simple = %6.4f\n", accuracy_tqh_chi2_simple);
    printf("[INFO] accuracy_tqh_chi2_modified = %6.4f\n", accuracy_tqh_chi2_modified);
    printf("[INFO] accuracy_tqh_yfyj = %6.4f\n", accuracy_tqh_yfyj);
    printf("//--------------------//\n");
    hist_report(hist_mass_w_candidate_yfyj, "w yfyj");
    hist_report(hist_mass_w_candidate_chi2_simple, "w simple");
    hist_report(hist_mass_w_candidate_chi2_modified, "w modified");
    hist_report(hist_mass_t2_candidate_yfyj, "t2 yfyj");
    hist_report(hist_mass_top_candidate_chi2_simple, "top simple");
    hist_report(hist_mass_top_candidate_chi2_modified, "top modified");
    printf("//--------------------//\n");
    hist_report(hist_mass_gen_w_candidate_yfyj, "gen w yfyj");
    hist_report(hist_mass_gen_w_candidate_chi2_simple, "gen w simple");
    hist_report(hist_mass_gen_w_candidate_chi2_modified, "gen w modified");
    hist_report(hist_mass_gen_t2_candidate_yfyj, "gen t2 yfyj");
    hist_report(hist_mass_gen_top_candidate_chi2_simple, "gen top simple");
    hist_report(hist_mass_gen_top_candidate_chi2_modified, "gen top modified");
    printf("//--------------------//\n");
    hist_report(hist_mass_conditioned_gen_w_candidate_yfyj, "conditioned_gen w yfyj");
    hist_report(hist_mass_conditioned_gen_w_candidate_chi2_simple, "conditioned_gen w simple");
    hist_report(hist_mass_conditioned_gen_w_candidate_chi2_modified, "conditioned_gen w modified");
    hist_report(hist_mass_conditioned_gen_t2_candidate_yfyj, "conditioned_gen t2 yfyj");
    hist_report(hist_mass_conditioned_gen_top_candidate_chi2_simple, "conditioned_gen top simple");
    hist_report(hist_mass_conditioned_gen_top_candidate_chi2_modified, "conditioned_gen top modified");
    printf("//--------------------//\n");
    //}}}
    printf("[INFO-hist] bin1 bin2 bin3; correcat rate in bin2; accuracy with exactly one particle cut\n");
    hist_bin_fraction(hist_num_gen_bquark, "gen_bquark", hist_num_gen_bquark->GetBinContent(2));
    hist_bin_fraction(hist_num_bjets_loose, "bjets_loose", count_bjet_is_bquark_loose);
    hist_bin_fraction(hist_num_bjets_medium, "bjets_medium", count_bjet_is_bquark_medium);
    hist_bin_fraction(hist_num_bjets_tight, "bjets_tight", count_bjet_is_bquark_tight);
    //### Make plots{{{
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    TLegend *legend_bjet = new TLegend(0.50,0.55,0.85,0.85);
    //--------------------------------------------------
    c1->SetLogy(1);
    hist_num_gen_bquark -> Draw();
    c1->SaveAs("ntuples_skimmed/hist_num_gen_bquark.png");
    //--------------------------------------------------
    c1->SetLogy(0);
    hist_num_gen_bquark   -> SetStats(0);
    hist_num_gen_bquark   -> GetXaxis() -> SetTitle("Number of particles");
    hist_num_gen_bquark   -> SetLineWidth(2);
    hist_num_bjets_loose  -> SetLineWidth(2);
    hist_num_bjets_medium -> SetLineWidth(2);
    hist_num_bjets_tight  -> SetLineWidth(2);
    hist_num_gen_bquark   -> SetLineColor(kRed);
    hist_num_bjets_tight  -> SetLineColor(kBlue);
    hist_num_bjets_medium -> SetLineColor(kGreen+2);
    hist_num_bjets_loose  -> SetLineColor(kGray+2);
    hist_num_bjets_tight_matched  -> SetLineWidth(0);
    hist_num_bjets_medium_matched -> SetLineWidth(0);
    hist_num_bjets_loose_matched  -> SetLineWidth(0);
    hist_num_bjets_tight_matched  -> SetFillColor(kBlue);
    hist_num_bjets_medium_matched -> SetFillColor(kGreen+2);
    hist_num_bjets_loose_matched  -> SetFillColor(kGray+2);
    hist_num_bjets_tight_matched  -> SetFillStyle(3001);
    hist_num_bjets_medium_matched -> SetFillStyle(3001);
    hist_num_bjets_loose_matched  -> SetFillStyle(3001);
    hist_num_gen_bquark   -> Draw();
    hist_num_bjets_medium_matched -> Draw("same");
    hist_num_bjets_tight_matched  -> Draw("same");
    hist_num_bjets_loose_matched  -> Draw("same");
    hist_num_bjets_loose  -> Draw("same");
    hist_num_bjets_medium -> Draw("same");
    hist_num_bjets_tight  -> Draw("same");
    hist_num_gen_bquark   -> Draw("same");
    legend_bjet->Clear();
    legend_bjet->SetTextSize(0.03);
    legend_bjet->AddEntry(hist_num_gen_bquark,   "b quark (gen-level)", "l");
    legend_bjet->AddEntry(hist_num_bjets_tight,  "tight b-tagged jets", "l");
    legend_bjet->AddEntry(hist_num_bjets_medium, "medium b-tagged jets", "l");
    legend_bjet->AddEntry(hist_num_bjets_loose,  "loose b-tagged jets", "l");
    legend_bjet->AddEntry(hist_num_bjets_tight_matched,  "tight b-tagged jets (matched)", "f");
    legend_bjet->AddEntry(hist_num_bjets_medium_matched, "medium b-tagged jets (matched)", "f");
    legend_bjet->AddEntry(hist_num_bjets_loose_matched,  "loose b-tagged jets (matched)", "f");
    legend_bjet->SetLineColor(0);
    legend_bjet->Draw("same");
    c1->SaveAs("ntuples_skimmed/hist_num_bjets_bquark.png");
    //--------------------------------------------------
    hist_num_selected_jets -> Draw();
    c1->SaveAs("ntuples_skimmed/hist_num_selected_jets.png");
    hist_num_gen_light_quark -> Draw();
    c1->SaveAs("ntuples_skimmed/hist_num_gen_light_quark.png");
    hist_mass_diphoton -> Draw();
    c1->SaveAs("ntuples_skimmed/hist_mass_diphoton.png");

    TLegend *legend = new TLegend(0.60,0.55,0.85,0.85);
    MakeFinalPlots(c1, hist_mass_w_candidate_chi2_simple, hist_mass_w_candidate_chi2_modified, hist_mass_w_candidate_yfyj, legend, "chi2_study_w_candidate.png");
    MakeFinalPlots(c1, hist_mass_gen_w_candidate_chi2_simple, hist_mass_gen_w_candidate_chi2_modified, hist_mass_gen_w_candidate_yfyj, legend, "chi2_study_gen_w_candidate.png");
    MakeFinalPlots(c1, hist_mass_conditioned_gen_w_candidate_chi2_simple, hist_mass_conditioned_gen_w_candidate_chi2_modified, hist_mass_conditioned_gen_w_candidate_yfyj, legend, "chi2_study_conditioned_gen_w_candidate.png");

    MakeFinalPlots(c1, hist_mass_top_candidate_chi2_simple, hist_mass_top_candidate_chi2_modified, hist_mass_t2_candidate_yfyj, legend, "chi2_study_top_candidate.png");
    MakeFinalPlots(c1, hist_mass_gen_top_candidate_chi2_simple, hist_mass_gen_top_candidate_chi2_modified, hist_mass_gen_t2_candidate_yfyj, legend, "chi2_study_gen_top_candidate.png");
    MakeFinalPlots(c1, hist_mass_conditioned_gen_top_candidate_chi2_simple, hist_mass_conditioned_gen_top_candidate_chi2_modified, hist_mass_conditioned_gen_t2_candidate_yfyj, legend, "chi2_study_conditioned_gen_top_candidate.png");

    MakeFinalPlots(c1, hist_mass_top_fcnh_simple, hist_mass_top_fcnh_modified, hist_mass_t1_candidate_yfyj, legend, "chi2_study_top_fcnh.png");
    MakeFinalPlots(c1, hist_deltaR_W_genW_simple, hist_deltaR_W_genW_modified, hist_deltaR_W_genW_yfyj, legend, "chi2_study_deltaR_w_genw.png");
    //###}}}

    fout->Write();
    fout->Close();
    return 1;
}

double GetM1M2_ratio(double M1, double M2){
    double ratio = abs(M1/M2-1) + abs(M2/M1-1);
    return ratio;
}
// ### GetBestM1{{{
TLorentzVector GetBestM1(double &M1, int num_jets, int index_bjet, std::vector<int> index_jet, TLorentzVector diphoton, std::vector<TLorentzVector> Jets, int &index_q, TLorentzVector &jet_q){
    //record all the possible combinations
    std::vector<int> indices;
    std::vector<double> top_fcnh_chi2;
    std::vector<TLorentzVector> top_fcnh_candidates;
    for(int i=0; i<num_jets; ++i){
        if(i==index_bjet || i==index_jet[0] || i==index_jet[1]) continue;//skip the jets for bjj
        TLorentzVector top_fcnh_tmp = diphoton + Jets[i];
        double chi2 = (top_fcnh_tmp.M() - top_quark_mass) * (top_fcnh_tmp.M() - top_quark_mass);
        indices.push_back(i);
        top_fcnh_chi2.push_back(chi2);
        top_fcnh_candidates.push_back(top_fcnh_tmp);
    }
    //choose the candidate with Mass closest to top mass
    int index_M1_min_chi2 = -999;
    double chi2_min_M1 = 40000;//200^2 > 178^2 ~ (x-top_mass)^2 //NOTE: will bound the range of M1!!!
    //pick the wanted one (with index)
    for(int i=0; i<top_fcnh_candidates.size(); ++i){ if(top_fcnh_chi2[i]<chi2_min_M1){ index_M1_min_chi2 = i; chi2_min_M1 = top_fcnh_chi2[i];} }
    TLorentzVector null;
    if(index_M1_min_chi2 == -999){ M1 = -999; return null;}//No suitable candidate
    else{
        index_q = indices[index_M1_min_chi2];
        jet_q = Jets[index_M1_min_chi2];
        M1 = top_fcnh_candidates[index_M1_min_chi2].M();
        return top_fcnh_candidates[index_M1_min_chi2];
    }
}
//}}}
// ### Alternative Mother info{{{
bool isMatched_with_Gen_tbw(std::vector<int> *GenPartInfo_MomPdgID, int index_bjet, int index_jet1, int index_jet2){
    bool isCorrect = \
    index_bjet > 0 &&\
    index_jet1 > 0 &&\
    index_jet2 > 0 &&\
    abs(GenPartInfo_MomPdgID->at(index_bjet)) == 6 &&\
    abs(GenPartInfo_MomPdgID->at(index_jet1)) == 24 &&\
    abs(GenPartInfo_MomPdgID->at(index_jet2)) == 24 &&\
    (index_jet1 != index_jet2);

    return isCorrect;
}
//old code{{{
//bool isMatched_with_Gen_W_Boson(TLorentzVector gen_w_sel, TH1D *&hist, Int_t GenPartInfo_size, std::vector<float> *GenPartInfo_Pt, std::vector<float> *GenPartInfo_Eta, std::vector<float> *GenPartInfo_Phi, std::vector<float> *GenPartInfo_Mass, std::vector<int> *GenPartInfo_PdgID){
//    double delta_R_gen_w = 999;
//    for(int j=0; j<GenPartInfo_size; j++){
//        if( abs(GenPartInfo_PdgID->at(j)) == 24 ){
//            TLorentzVector genParticle;
//            genParticle.SetPtEtaPhiM(GenPartInfo_Pt->at(j), GenPartInfo_Eta->at(j), GenPartInfo_Phi->at(j), GenPartInfo_Mass->at(j));
//            delta_R_gen_w = gen_w_sel.DeltaR(genParticle);
//        }
//    }//end of gen loop
//    //--- remove event with bad combination ---//
//    //if(gen_w_sel.M() < 20) continue; 
//    hist->Fill(delta_R_gen_w);
//    if(delta_R_gen_w > 0.005 || gen_w_sel.M() < 20) return false;
//    else return true;
//}
//}}}
//}}}
//### genMatching{{{
bool is_this_tqh_quark(TLorentzVector jet, Int_t GenPartInfo_size, std::vector<int> *GenPartInfo_MomPdgID, std::vector<float> *GenPartInfo_Pt, std::vector<float> *GenPartInfo_Eta, std::vector<float> *GenPartInfo_Phi, std::vector<float> *GenPartInfo_Mass, std::vector<int> *GenPartInfo_Status, std::vector<int> *GenPartInfo_PdgID){
    TLorentzVector truelove;//we are looking for the right genParticle to match the jet.
    int index = -999, truelove_PdgID = -999; double delta_R_min = 999.;
    for(int i=0; i<GenPartInfo_size; i++){
        if( abs(GenPartInfo_Status->at(i)) != 23 ) continue;//remove incoming/intermediate particles
        if( abs(GenPartInfo_PdgID->at(i)) > 6 ) continue;//exclude top quark & other particles
        //--------------------
        TLorentzVector genParticle;
        genParticle.SetPtEtaPhiM(GenPartInfo_Pt->at(i), GenPartInfo_Eta->at(i), GenPartInfo_Phi->at(i), GenPartInfo_Mass->at(i));
        double delta_R = jet.DeltaR(genParticle);
        //select quark & require min(deltaR)
        if( delta_R < 0.4 && delta_R < delta_R_min){
            index = i;//record the matched genParticle
            delta_R_min = delta_R;
            truelove = genParticle;
            truelove_PdgID = GenPartInfo_PdgID->at(i);
        }
    }//end of gen loop

    /*
    if(index>0){
        printf("[check-tqh] \n");
            printf("Status = %3d, ", GenPartInfo_Status->at(index));
            printf("PdgID = %3d, ", GenPartInfo_PdgID->at(index));
            printf("Pt = %6.2f, ", GenPartInfo_Pt->at(index));
            printf("Eta = %9.2f, ", GenPartInfo_Eta->at(index));
            printf("Phi = %6.2f, ", GenPartInfo_Phi->at(index));
            printf("Mass = %6.2f, ", GenPartInfo_Mass->at(index));
            printf("MomPdgID = %5d\n", GenPartInfo_MomPdgID->at(index));
    } else{
        printf("[check-tqh] index<0\n");
    }
    */

    bool is_tqh_quark = ( index > 0 && abs(GenPartInfo_MomPdgID->at(index)) == 6 );
    return is_tqh_quark;

}
bool CheckBJetID(TLorentzVector jet, Int_t GenPartInfo_size, std::vector<int> *GenPartInfo_MomPdgID, std::vector<float> *GenPartInfo_Pt, std::vector<float> *GenPartInfo_Eta, std::vector<float> *GenPartInfo_Phi, std::vector<float> *GenPartInfo_Mass, std::vector<int> *GenPartInfo_Status, std::vector<int> *GenPartInfo_PdgID){
    TLorentzVector truelove;//we are looking for the right genParticle to match the jet.
    int index = -999, truelove_PdgID = -999; double delta_R_min = 999.;
    for(int i=0; i<GenPartInfo_size; i++){
        if( abs(GenPartInfo_Status->at(i)) != 23 ) continue;//remove incoming/intermediate particles
        if( abs(GenPartInfo_PdgID->at(i)) == 6 ) continue;//exclude top quark
        //--------------------
        TLorentzVector genParticle;
        genParticle.SetPtEtaPhiM(GenPartInfo_Pt->at(i), GenPartInfo_Eta->at(i), GenPartInfo_Phi->at(i), GenPartInfo_Mass->at(i));
        double delta_R = jet.DeltaR(genParticle);
        //select quark & require min(deltaR)
        if( abs(GenPartInfo_PdgID->at(i)) < 7 && delta_R < 0.4 && delta_R < delta_R_min){
        //if( abs(GenPartInfo_PdgID->at(i)) < 7 && delta_R < delta_R_min){
            index = i;//record the matched genParticle
            delta_R_min = delta_R;
            truelove = genParticle;
            truelove_PdgID = GenPartInfo_PdgID->at(i);
        }
    }//end of gen loop
    // bjet is bquark coming from top
    bool bjet_is_bquark = ( abs(truelove_PdgID) == 5 && abs(GenPartInfo_MomPdgID->at(index)) == 6);
    return bjet_is_bquark;
}
TLorentzVector GetGenParticle(TLorentzVector jet, Int_t GenPartInfo_size, std::vector<float> *GenPartInfo_Pt, std::vector<float> *GenPartInfo_Eta, std::vector<float> *GenPartInfo_Phi, std::vector<float> *GenPartInfo_Mass, std::vector<int> *GenPartInfo_Status, std::vector<int> *GenPartInfo_PdgID, std::vector<int> &index_GenParticles, int &genParticle_PdgID){
    TLorentzVector truelove;//we are looking for the right genParticle to match the jet.
    int index = -1, truelove_PdgID = -999; double delta_R_min = 999.;
    for(int i=0; i<GenPartInfo_size; i++){
        if( abs(GenPartInfo_Status->at(i)) != 23 ) continue;//remove incoming/intermediate particles
        if( abs(GenPartInfo_PdgID->at(i)) == 6 ) continue;//exclude top quark
        //bool isAvailable = checkAvailability(i, index_GenParticles); if(!isAvailable) continue;//the truelove of previous jets shall not become another one's truelove.
        //--------------------
        TLorentzVector genParticle;
        genParticle.SetPtEtaPhiM(GenPartInfo_Pt->at(i), GenPartInfo_Eta->at(i), GenPartInfo_Phi->at(i), GenPartInfo_Mass->at(i));
        double delta_R = jet.DeltaR(genParticle);
        //select quark & require min(deltaR)
        if( abs(GenPartInfo_PdgID->at(i)) < 7 && delta_R < 0.4 && delta_R < delta_R_min){
        //if( abs(GenPartInfo_PdgID->at(i)) < 7 && delta_R < delta_R_min){
            index = i;//record the matched genParticle
            delta_R_min = delta_R;
            truelove = genParticle;
            truelove_PdgID = GenPartInfo_PdgID->at(i);
        }
    }//end of gen loop
    index_GenParticles.push_back(index);
    genParticle_PdgID = truelove_PdgID;
    //store particle info
    return truelove;
}

bool checkAvailability(int index, std::vector<int> ID_IsChosen){
    bool result = true;//if pass the following for loop, the genParticle of the index is available.
    for(std::size_t i=0; i<ID_IsChosen.size(); ++i){
        if(index == ID_IsChosen[i]){ result = false; break; } 
    }
    return result;
}
//}}}
// ##### print gen info{{{
void print_matched_gen_info(TLorentzVector jet, Int_t GenPartInfo_size, std::vector<int> *GenPartInfo_MomPdgID, std::vector<float> *GenPartInfo_Pt, std::vector<float> *GenPartInfo_Eta, std::vector<float> *GenPartInfo_Phi, std::vector<float> *GenPartInfo_Mass, std::vector<int> *GenPartInfo_Status, std::vector<int> *GenPartInfo_PdgID){
    TLorentzVector truelove;//we are looking for the right genParticle to match the jet.
    int index = -999, truelove_PdgID = -999; double delta_R_min = 999.;
    for(int i=0; i<GenPartInfo_size; i++){
        if( abs(GenPartInfo_Status->at(i)) != 23 ) continue;//remove incoming/intermediate particles
        if( abs(GenPartInfo_PdgID->at(i)) > 6 ) continue;//exclude top quark & other particles
        //--------------------
        TLorentzVector genParticle;
        genParticle.SetPtEtaPhiM(GenPartInfo_Pt->at(i), GenPartInfo_Eta->at(i), GenPartInfo_Phi->at(i), GenPartInfo_Mass->at(i));
        double delta_R = jet.DeltaR(genParticle);
        //select quark & require min(deltaR)
        if( delta_R < 0.4 && delta_R < delta_R_min){
            index = i;//record the matched genParticle
            delta_R_min = delta_R;
            truelove = genParticle;
            truelove_PdgID = GenPartInfo_PdgID->at(i);
        }
    }//end of gen loop

    if(index>0){
        printf("[check-tqh] \n");
            printf("Status = %3d, ", GenPartInfo_Status->at(index));
            printf("PdgID = %3d, ", GenPartInfo_PdgID->at(index));
            printf("Pt = %6.2f, ", GenPartInfo_Pt->at(index));
            printf("Eta = %9.2f, ", GenPartInfo_Eta->at(index));
            printf("Phi = %6.2f, ", GenPartInfo_Phi->at(index));
            printf("Mass = %6.2f, ", GenPartInfo_Mass->at(index));
            printf("MomPdgID = %5d\n", GenPartInfo_MomPdgID->at(index));
    } else{
        printf("[check-tqh] Not found.\n");
    }
}
//}}}
//### report{{{
void kinematics_info(const char* Title, TLorentzVector Particle){
        printf("(%s) Pt = %6.2f, Eta = %6.2f, Phi = %6.2f, Energy = %6.2f, Mass = %6.2f\n", Title, Particle.Pt(), Particle.Eta(), Particle.Phi(), Particle.Energy(), Particle.M());
}
void kinematics_report(const char* recoTitle, TLorentzVector recoParticle, int id_recoParticle, const char* genTitle, TLorentzVector genParticle, int genParticle_PdgID){
        double delta_R = genParticle.DeltaR(recoParticle);
        printf("(%s) Pt = %6.2f, Eta = %6.2f, Phi = %6.2f, Energy = %6.2f, Mass = %6.2f, id = %d\n", recoTitle, recoParticle.Pt(), recoParticle.Eta(), recoParticle.Phi(), recoParticle.Energy(), recoParticle.M(), id_recoParticle);
        printf("(%s) Pt = %6.2f, Eta = %6.2f, Phi = %6.2f, Energy = %6.2f, Mass = %6.2f, id = %d, delta_R = %6.2f\n", genTitle, genParticle.Pt(), genParticle.Eta(), genParticle.Phi(), genParticle.Energy(), genParticle.M(), genParticle_PdgID, delta_R);
}
void hist_bin_fraction(TH1D *hist, const char* title, int entries_matched_in_bin2){
    int total_entries = hist->GetEntries();
    printf("[INFO-hist] %s (%d)\n", title, total_entries);
    printf("[INFO-hist] fractions = ");
    for(int i=0; i<3; ++i){
        if(i<2) printf("%6.4f, ", (double) hist->GetBinContent(i+1) / (double)total_entries);
        else{
            int accumulation = 0;
            for(int j=i; j<10; ++j) accumulation += (double) hist->GetBinContent(j+1);
            printf("%6.4f; ", i, (double) accumulation / (double)total_entries);
            printf("%6.4f, ", i, (double) entries_matched_in_bin2 / (double)total_entries);
            printf("%6.4f\n", i, (double) entries_matched_in_bin2 / (double)hist->GetBinContent(2));
        }
    }
}
void hist_report(TH1D *hist, const char* chi2_type){
    double mean = hist->GetMean();
    double sigma = hist->GetMeanError();
    double sigma2 = hist->GetRMS();
    int bin_maximum = hist->GetMaximumBin();
    double maxbin_lower_edge = hist->GetBinLowEdge(bin_maximum);
    double bin_width = hist->GetBinWidth(bin_maximum);
    double maxbin_upper_edge = maxbin_lower_edge + bin_width;

    //printf("[INFO] chi2 %s: mean = %6.2f, sigma = %6.2f, %6.2f\n", chi2_type, mean, sigma, sigma2);
    printf("[INFO] chi2 %s: mean = %6.2f, sigma = %6.2f, mode lies in [%6.2f, %6.2f]\n", chi2_type, mean, sigma2, maxbin_lower_edge, maxbin_upper_edge);
}
//}}}
//### chi2{{{
double Chi2_calculator_simple(double w_mass, double t_mass){
    TVectorD vec_mass(2);
    vec_mass(0) = w_mass - w_boson_mass;
    vec_mass(1) = t_mass - top_quark_mass;
    TMatrixD matrix(2,2);
    matrix(0,0) = 304.55; matrix(0,1) =   0.00;
    matrix(1,0) =   0.00; matrix(1,1) = 913.05;
    //--- ST ---//
    //matrix(0,0) = 305.98; matrix(0,1) = 0;
    //matrix(1,0) = 0;      matrix(1,1) = 787.07;
    return matrix.Invert()*vec_mass*vec_mass;
}
double Chi2_calculator_modified(double w_mass, double t_mass){
    TVectorD vec_mass(2);
    vec_mass(0) = w_mass - w_boson_mass;
    vec_mass(1) = t_mass - top_quark_mass;
    TMatrixD matrix(2,2);
    matrix(0,0) = 304.55; matrix(0,1) = 342.04;
    matrix(1,0) = 342.04; matrix(1,1) = 913.05;
    //--- ST ---//
    //matrix(0,0) = 305.98; matrix(0,1) = 323.97;
    //matrix(1,0) = 323.97; matrix(1,1) = 787.07;
    return matrix.Invert()*vec_mass*vec_mass;
}
//###}}}
//### mkplots{{{
void MakePlots(TCanvas *c1, TH1D* hist, const char* title, const char* outputFile){
    hist->Draw();
    hist->SetTitle(title);
    hist->SetXTitle(title);
    hist->SetYTitle("Entries");
    hist->GetYaxis()->SetTitleOffset(1.4);
    hist->Write();
    c1->SaveAs(outputFile);
}
void MakeFinalPlots(TCanvas *c1, TH1D* hist_simple, TH1D* hist_modified, TH1D*hist_yfyj, TLegend *legend, const char* name){
    double max;
    double scale = 1.2;
    double max_simple = hist_simple -> GetMaximum();
    double max_modified = hist_modified -> GetMaximum();
    double max_yfyj = hist_yfyj -> GetMaximum();
    max = (max_modified > max_simple) ? max_modified : max_simple;
    max = (max_yfyj > max_modified) ? max_yfyj : max_modified;
    double overflow_simple = hist_simple->GetBinContent(hist_simple->GetNbinsX() + 1);
    double overflow_modified = hist_modified->GetBinContent(hist_modified->GetNbinsX() + 1);
    double overflow_yfyj = hist_yfyj->GetBinContent(hist_yfyj->GetNbinsX() + 1);
    //max = (max > overflow_simple) ? max : overflow_simple;
    //max = (max > overflow_modified) ? max : overflow_modified;
    //max = (max > overflow_yfyj) ? max : overflow_yfyj;

    hist_simple->SetStats(0);
    hist_simple->SetMaximum(max*scale);
    hist_simple->SetLineWidth(2);
    hist_simple->SetLineColor(kBlue);
    hist_simple->GetXaxis()->SetRange(1, hist_simple->GetNbinsX() + 1);
    hist_simple->Draw();
    hist_modified->SetLineWidth(2);
    hist_modified->SetLineColor(kRed);
    hist_modified->GetXaxis()->SetRange(1, hist_modified->GetNbinsX() + 1);
    hist_modified->Draw("same");
    hist_yfyj->SetLineWidth(2);
    hist_yfyj->SetLineColor(kGreen+4);
    hist_yfyj->GetXaxis()->SetRange(1, hist_yfyj->GetNbinsX() + 1);
    hist_yfyj->Draw("same");

    legend->Clear();
    legend->SetTextSize(0.03);
    legend->AddEntry(hist_yfyj, "leading-jets method", "l");
    legend->AddEntry(hist_simple, "simple #chi^{2} method", "l");
    legend->AddEntry(hist_modified, "modified #chi^{2} method", "l");
    legend->SetLineColor(0);
    legend->Draw("same");

    c1->SaveAs(Form("ntuples_skimmed/%s", name));
}
//###}}}
//### bool functions{{{
bool isThisMCsignal(char* dataset){
    if((string)dataset == "ST_FCNC-TH_Thadronic_HToaa_eta_hct-MadGraph5-pythia8") return true;
    if((string)dataset == "ST_FCNC-TH_Thadronic_HToaa_eta_hut-MadGraph5-pythia8") return true;
    if((string)dataset == "ST_FCNC-TH_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8") return true;
    if((string)dataset == "ST_FCNC-TH_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8") return true;
    if((string)dataset == "TT_FCNC-T2HJ_aTleptonic_HToaa_eta_hct-MadGraph5-pythia8") return true;
    if((string)dataset == "TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hct-MadGraph5-pythia8") return true;
    if((string)dataset == "TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hut-MadGraph5-pythia8") return true;
    if((string)dataset == "TT_FCNC-TtoHJ_aTleptonic_HToaa_eta_hut-MadGraph5-pythia8") return true;
    if((string)dataset == "TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hct-MadGraph5-pythia8") return true;
    if((string)dataset == "TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hut-MadGraph5-pythia8") return true;
    if((string)dataset == "TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8") return true;
    if((string)dataset == "TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8") return true;
    return false;
}
bool isThisDataOrNot(char* dataset){
    if((string)dataset == "DoubleEG_B") return true;
    if((string)dataset == "DoubleEG_C") return true;
    if((string)dataset == "DoubleEG_D") return true;
    if((string)dataset == "DoubleEG_E") return true;
    if((string)dataset == "DoubleEG_F") return true;
    return false;
}
bool isThisMultiFile(char* dataset){
    if((string)dataset == "DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa") return true;
    if((string)dataset == "GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8") return true;
    return false;
}
//###}}}
//### flashggStdTree {{{
flashggStdTreeParameters::flashggStdTreeParameters(){
    GenPartInfo_Pt = new std::vector<float>;
    GenPartInfo_Eta = new std::vector<float>;
    GenPartInfo_Phi = new std::vector<float>;
    GenPartInfo_Mass = new std::vector<float>;
    GenPartInfo_PdgID = new std::vector<int>;
    GenPartInfo_Status = new std::vector<int>;
    GenPartInfo_nMo = new std::vector<int>;
    GenPartInfo_nDa = new std::vector<int>;
    GenPartInfo_isHardProcess = new std::vector<bool>;
    GenPartInfo_fromHardProcessFinalState = new std::vector<bool>;
    GenPartInfo_isPromptFinalState = new std::vector<bool>;
    GenPartInfo_isDirectPromptTauDecayProductFinalState = new std::vector<bool>;
    GenPartInfo_MomPdgID = new std::vector<int>;
    GenPartInfo_MomStatus = new std::vector<int>;
    GenPartInfo_MomPt = new std::vector<float>;
    GenPartInfo_MomEta = new std::vector<float>;
    GenPartInfo_MomPhi = new std::vector<float>;
    GenPartInfo_MomMass = new std::vector<float>;
    //------------------------
    JetInfo_Pt = new std::vector<float>;
    JetInfo_Eta = new std::vector<float>;
    JetInfo_Phi = new std::vector<float>;
    JetInfo_Mass = new std::vector<float>;
    JetInfo_Energy = new std::vector<float>;
    JetInfo_GenFlavor = new std::vector<int>;
    JetInfo_pfDeepCSVJetTags_probb = new std::vector<float>;
    JetInfo_pfDeepCSVJetTags_probbb = new std::vector<float>;
    //------------------------
    ElecInfo_Charge = new std::vector<int>;
    ElecInfo_Pt = new std::vector<float>;
    ElecInfo_Eta = new std::vector<float>;
    ElecInfo_Phi = new std::vector<float>;
    ElecInfo_Energy = new std::vector<float>;
    ElecInfo_EtaSC = new std::vector<float>;
    ElecInfo_PhiSC = new std::vector<float>;
    ElecInfo_GsfTrackDz = new std::vector<float>;
    ElecInfo_GsfTrackDxy = new std::vector<float>;
    ElecInfo_EGMCutBasedIDVeto = new std::vector<bool>;
    ElecInfo_EGMCutBasedIDLoose = new std::vector<bool>;
    ElecInfo_EGMCutBasedIDMedium = new std::vector<bool>;
    ElecInfo_EGMCutBasedIDTight = new std::vector<bool>;
    ElecInfo_fggPhoVeto = new std::vector<bool>;
    ElecInfo_EnergyCorrFactor = new std::vector<float>;
    ElecInfo_EnergyPostCorrErr = new std::vector<float>;
    ElecInfo_EnergyPostCorrScaleUp = new std::vector<float>;
    ElecInfo_EnergyPostCorrScaleDown = new std::vector<float>;
    ElecInfo_EnergyPostCorrSmearUp = new std::vector<float>;
    ElecInfo_EnergyPostCorrSmearDown = new std::vector<float>;
    ElecInfo_GenMatch = new std::vector<bool>;
    ElecInfo_GenPdgID = new std::vector<int>;
    ElecInfo_GenPt = new std::vector<float>;
    ElecInfo_GenEta = new std::vector<float>;
    ElecInfo_GenPhi = new std::vector<float>;
    //------------------------
    MuonInfo_Charge = new std::vector<int>;
    MuonInfo_MuonType = new std::vector<float>;
    MuonInfo_Pt = new std::vector<float>;
    MuonInfo_Eta = new std::vector<float>;
    MuonInfo_Phi = new std::vector<float>;
    MuonInfo_Energy = new std::vector<float>;
    MuonInfo_BestTrackDz = new std::vector<float>;
    MuonInfo_BestTrackDxy = new std::vector<float>;
    MuonInfo_PFIsoDeltaBetaCorrR04 = new std::vector<float>;
    MuonInfo_TrackerBasedIsoR03 = new std::vector<float>;
    MuonInfo_CutBasedIdMedium = new std::vector<bool>;
    MuonInfo_CutBasedIdTight = new std::vector<bool>;
    MuonInfo_GenMatch = new std::vector<bool>;
    MuonInfo_GenPdgID = new std::vector<int>;
    MuonInfo_GenPt = new std::vector<float>;
    MuonInfo_GenEta = new std::vector<float>;
    MuonInfo_GenPhi = new std::vector<float>;
    //------------------------
}
flashggStdTreeParameters::~flashggStdTreeParameters(){
    delete GenPartInfo_Pt;
    delete GenPartInfo_Eta;
    delete GenPartInfo_Phi;
    delete GenPartInfo_Mass;
    delete GenPartInfo_PdgID;
    delete GenPartInfo_Status;
    delete GenPartInfo_nMo;
    delete GenPartInfo_nDa;
    delete GenPartInfo_isHardProcess;
    delete GenPartInfo_fromHardProcessFinalState;
    delete GenPartInfo_isPromptFinalState;
    delete GenPartInfo_isDirectPromptTauDecayProductFinalState;
    delete GenPartInfo_MomPdgID;
    delete GenPartInfo_MomStatus;
    delete GenPartInfo_MomPt;
    delete GenPartInfo_MomEta;
    delete GenPartInfo_MomPhi;
    delete GenPartInfo_MomMass;
    //------------------------
    delete JetInfo_Pt;
    delete JetInfo_Eta;
    delete JetInfo_Phi;
    delete JetInfo_Mass;
    delete JetInfo_Energy;
    delete JetInfo_GenFlavor;
    delete JetInfo_pfDeepCSVJetTags_probb;
    delete JetInfo_pfDeepCSVJetTags_probbb;
    //------------------------
    delete ElecInfo_Charge;
    delete ElecInfo_Pt;
    delete ElecInfo_Eta;
    delete ElecInfo_Phi;
    delete ElecInfo_Energy;
    delete ElecInfo_EtaSC;
    delete ElecInfo_PhiSC;
    delete ElecInfo_GsfTrackDz;
    delete ElecInfo_GsfTrackDxy;
    delete ElecInfo_EGMCutBasedIDVeto;
    delete ElecInfo_EGMCutBasedIDLoose;
    delete ElecInfo_EGMCutBasedIDMedium;
    delete ElecInfo_EGMCutBasedIDTight;
    delete ElecInfo_fggPhoVeto;
    delete ElecInfo_EnergyCorrFactor;
    delete ElecInfo_EnergyPostCorrErr;
    delete ElecInfo_EnergyPostCorrScaleUp;
    delete ElecInfo_EnergyPostCorrScaleDown;
    delete ElecInfo_EnergyPostCorrSmearUp;
    delete ElecInfo_EnergyPostCorrSmearDown;
    delete ElecInfo_GenMatch;
    delete ElecInfo_GenPdgID;
    delete ElecInfo_GenPt;
    delete ElecInfo_GenEta;
    delete ElecInfo_GenPhi;
    //------------------------
    delete MuonInfo_Charge;
    delete MuonInfo_MuonType;
    delete MuonInfo_Pt;
    delete MuonInfo_Eta;
    delete MuonInfo_Phi;
    delete MuonInfo_Energy;
    delete MuonInfo_BestTrackDz;
    delete MuonInfo_BestTrackDxy;
    delete MuonInfo_PFIsoDeltaBetaCorrR04;
    delete MuonInfo_TrackerBasedIsoR03;
    delete MuonInfo_CutBasedIdMedium;
    delete MuonInfo_CutBasedIdTight;
    delete MuonInfo_GenMatch;
    delete MuonInfo_GenPdgID;
    delete MuonInfo_GenPt;
    delete MuonInfo_GenEta;
    delete MuonInfo_GenPhi;
    //------------------------
}
flashggStdTreeReader::flashggStdTreeReader(void){
    printf("[INFO] Reading data...\n");
}
void flashggStdTreeReader::InitChain(const char* treeName){
    flashggStdTree = new TChain(treeName);
    printf("[INFO] flashggStdTreeReader::InitChain : Finished!\n");
}
void flashggStdTreeReader::AddSingleRootFile(char* input_file){
    flashggStdTree->Add(input_file);
    printf("[INFO] flashggStdTreeReader::AddSingleRootFile : Finished!\n");
}
void flashggStdTreeReader::AddMultiRootFile(char* input_file){
    flashggStdTree->Add(Form("%s/*.root", input_file));
    printf("[INFO] flashggStdTreeReader::AddMultiRootFile : Finished!\n");
}
int flashggStdTreeReader::GetEntries(void){
    printf("[INFO] flashggStdTreeReader::GetEntries : %d\n", flashggStdTree->GetEntries());
    return flashggStdTree->GetEntries();
}
double flashggStdTreeReader::GetGenWeight(void){
    return EvtInfo_genweight;
}
TChain* flashggStdTreeReader::GetTChain(void){
    return flashggStdTree;
}
void flashggStdTreeReader::SetBranchAddresses(){
    flashggStdTree->SetBranchAddress("GenPartInfo.size", &GenPartInfo_size);
    flashggStdTree->SetBranchAddress("GenPartInfo.Pt", &GenPartInfo_Pt);
    flashggStdTree->SetBranchAddress("GenPartInfo.Eta", &GenPartInfo_Eta);
    flashggStdTree->SetBranchAddress("GenPartInfo.Phi", &GenPartInfo_Phi);
    flashggStdTree->SetBranchAddress("GenPartInfo.Mass", &GenPartInfo_Mass);
    flashggStdTree->SetBranchAddress("GenPartInfo.PdgID", &GenPartInfo_PdgID);
    flashggStdTree->SetBranchAddress("GenPartInfo.Status", &GenPartInfo_Status);
    flashggStdTree->SetBranchAddress("GenPartInfo.nMo", &GenPartInfo_nMo);
    flashggStdTree->SetBranchAddress("GenPartInfo.nDa", &GenPartInfo_nDa);
    flashggStdTree->SetBranchAddress("GenPartInfo.isHardProcess", &GenPartInfo_isHardProcess);
    flashggStdTree->SetBranchAddress("GenPartInfo.fromHardProcessFinalState", &GenPartInfo_fromHardProcessFinalState);
    flashggStdTree->SetBranchAddress("GenPartInfo.isPromptFinalState", &GenPartInfo_isPromptFinalState);
    flashggStdTree->SetBranchAddress("GenPartInfo.isDirectPromptTauDecayProductFinalState", &GenPartInfo_isDirectPromptTauDecayProductFinalState);
    flashggStdTree->SetBranchAddress("GenPartInfo.MomPdgID", &GenPartInfo_MomPdgID);
    flashggStdTree->SetBranchAddress("GenPartInfo.MomStatus", &GenPartInfo_MomStatus);
    flashggStdTree->SetBranchAddress("GenPartInfo.MomPt", &GenPartInfo_MomPt);
    flashggStdTree->SetBranchAddress("GenPartInfo.MomEta", &GenPartInfo_MomEta);
    flashggStdTree->SetBranchAddress("GenPartInfo.MomPhi", &GenPartInfo_MomPhi);
    flashggStdTree->SetBranchAddress("GenPartInfo.MomMass", &GenPartInfo_MomMass);
    //------------------------
    flashggStdTree->SetBranchAddress("EvtInfo.passTrigger", &EvtInfo_passTrigger);
    flashggStdTree->SetBranchAddress("EvtInfo.NPu", &EvtInfo_NPu);
    flashggStdTree->SetBranchAddress("EvtInfo.NVtx", &EvtInfo_NVtx);
    flashggStdTree->SetBranchAddress("EvtInfo.Rho", &EvtInfo_Rho);
    flashggStdTree->SetBranchAddress("EvtInfo.genweight", &EvtInfo_genweight);
    //------------------------
    flashggStdTree->SetBranchAddress("DiPhoInfo.mass", &DiPhoInfo_mass);
    flashggStdTree->SetBranchAddress("DiPhoInfo.pt", &DiPhoInfo_pt);
    flashggStdTree->SetBranchAddress("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
    flashggStdTree->SetBranchAddress("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
    flashggStdTree->SetBranchAddress("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
    flashggStdTree->SetBranchAddress("DiPhoInfo.leadE", &DiPhoInfo_leadE);
    flashggStdTree->SetBranchAddress("DiPhoInfo.leadhoe", &DiPhoInfo_leadhoe);
    flashggStdTree->SetBranchAddress("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
    //------------------------
    flashggStdTree->SetBranchAddress("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
    flashggStdTree->SetBranchAddress("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
    flashggStdTree->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
    flashggStdTree->SetBranchAddress("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
    flashggStdTree->SetBranchAddress("DiPhoInfo.subleadhoe", &DiPhoInfo_subleadhoe);
    flashggStdTree->SetBranchAddress("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
    //------------------------
    flashggStdTree->SetBranchAddress("jets_size", &jets_size);
    flashggStdTree->SetBranchAddress("JetInfo.Pt", &JetInfo_Pt);
    flashggStdTree->SetBranchAddress("JetInfo.Eta", &JetInfo_Eta);
    flashggStdTree->SetBranchAddress("JetInfo.Phi", &JetInfo_Phi);
    flashggStdTree->SetBranchAddress("JetInfo.Mass", &JetInfo_Mass);
    flashggStdTree->SetBranchAddress("JetInfo.Energy", &JetInfo_Energy);
    flashggStdTree->SetBranchAddress("JetInfo.GenFlavor", &JetInfo_GenFlavor);
    flashggStdTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
    flashggStdTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
    //------------------------
    flashggStdTree->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size);
    flashggStdTree->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge);
    flashggStdTree->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt);
    flashggStdTree->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta);
    flashggStdTree->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi);
    flashggStdTree->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy);
    flashggStdTree->SetBranchAddress("ElecInfo.EtaSC", &ElecInfo_EtaSC);
    flashggStdTree->SetBranchAddress("ElecInfo.PhiSC", &ElecInfo_PhiSC);
    flashggStdTree->SetBranchAddress("ElecInfo.GsfTrackDz", &ElecInfo_GsfTrackDz);
    flashggStdTree->SetBranchAddress("ElecInfo.GsfTrackDxy", &ElecInfo_GsfTrackDxy);
    flashggStdTree->SetBranchAddress("ElecInfo.EGMCutBasedIDVeto", &ElecInfo_EGMCutBasedIDVeto);
    flashggStdTree->SetBranchAddress("ElecInfo.EGMCutBasedIDLoose", &ElecInfo_EGMCutBasedIDLoose);
    flashggStdTree->SetBranchAddress("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium);
    flashggStdTree->SetBranchAddress("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight);
    flashggStdTree->SetBranchAddress("ElecInfo.fggPhoVeto", &ElecInfo_fggPhoVeto);
    flashggStdTree->SetBranchAddress("ElecInfo.EnergyCorrFactor", &ElecInfo_EnergyCorrFactor);
    flashggStdTree->SetBranchAddress("ElecInfo.EnergyPostCorrErr", &ElecInfo_EnergyPostCorrErr);
    flashggStdTree->SetBranchAddress("ElecInfo.EnergyPostCorrScaleUp", &ElecInfo_EnergyPostCorrScaleUp);
    flashggStdTree->SetBranchAddress("ElecInfo.EnergyPostCorrScaleDown", &ElecInfo_EnergyPostCorrScaleDown);
    flashggStdTree->SetBranchAddress("ElecInfo.EnergyPostCorrSmearUp", &ElecInfo_EnergyPostCorrSmearUp);
    flashggStdTree->SetBranchAddress("ElecInfo.EnergyPostCorrSmearDown", &ElecInfo_EnergyPostCorrSmearDown);
    flashggStdTree->SetBranchAddress("ElecInfo.GenMatch", &ElecInfo_GenMatch);
    flashggStdTree->SetBranchAddress("ElecInfo.GenPdgID", &ElecInfo_GenPdgID);
    flashggStdTree->SetBranchAddress("ElecInfo.GenPt", &ElecInfo_GenPt);
    flashggStdTree->SetBranchAddress("ElecInfo.GenEta", &ElecInfo_GenEta);
    flashggStdTree->SetBranchAddress("ElecInfo.GenPhi", &ElecInfo_GenPhi);
    //------------------------
    flashggStdTree->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size);
    flashggStdTree->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge);
    flashggStdTree->SetBranchAddress("MuonInfo.MuonType", &MuonInfo_MuonType);
    flashggStdTree->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt);
    flashggStdTree->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta);
    flashggStdTree->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi);
    flashggStdTree->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy);
    flashggStdTree->SetBranchAddress("MuonInfo.BestTrackDz", &MuonInfo_BestTrackDz);
    flashggStdTree->SetBranchAddress("MuonInfo.BestTrackDxy", &MuonInfo_BestTrackDxy);
    flashggStdTree->SetBranchAddress("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04);
    flashggStdTree->SetBranchAddress("MuonInfo.TrackerBasedIsoR03", &MuonInfo_TrackerBasedIsoR03);
    flashggStdTree->SetBranchAddress("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium);
    flashggStdTree->SetBranchAddress("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
    flashggStdTree->SetBranchAddress("MuonInfo.GenMatch", &MuonInfo_GenMatch);
    flashggStdTree->SetBranchAddress("MuonInfo.GenPdgID", &MuonInfo_GenPdgID);
    flashggStdTree->SetBranchAddress("MuonInfo.GenPt", &MuonInfo_GenPt);
    flashggStdTree->SetBranchAddress("MuonInfo.GenEta", &MuonInfo_GenEta);
    flashggStdTree->SetBranchAddress("MuonInfo.GenPhi", &MuonInfo_GenPhi);
    //------------------------
    printf("[INFO] flashggStdTreeReader::SetBranchAddresses : Finished!\n");
}
///### }}}
//### myTreeClass {{{
myParameters::myParameters(){
    JetInfo_jet_pt_selection = new std::vector<float>;
    JetInfo_jet_eta_selection = new std::vector<float>;
    JetInfo_jet_phi_selection = new std::vector<float>;
    JetInfo_jet_energy_selection = new std::vector<float>;
    JetInfo_jet_diphoton_deltaR_selection = new std::vector<float>;
    JetInfo_jet_leadingPhoton_deltaR_selection = new std::vector<float>;
    JetInfo_jet_subleadingPhoton_deltaR_selection = new std::vector<float>;
    JetInfo_jet_pfDeepCSVJetTags_probb_selection = new std::vector<float>;
    JetInfo_jet_pfDeepCSVJetTags_probbb_selection = new std::vector<float>;
    ElecInfo_electron_pt_selection = new std::vector<float>;
    ElecInfo_electron_eta_selection = new std::vector<float>;
    ElecInfo_electron_phi_selection = new std::vector<float>;
    ElecInfo_electron_energy_selection = new std::vector<float>;
    ElecInfo_electron_diphoton_deltaR_selection = new std::vector<float>;
    ElecInfo_electron_leadingPhoton_deltaR_selection = new std::vector<float>;
    ElecInfo_electron_subleadingPhoton_deltaR_selection = new std::vector<float>;
    MuonInfo_muon_pt_selection = new std::vector<float>;
    MuonInfo_muon_eta_selection = new std::vector<float>;
    MuonInfo_muon_phi_selection = new std::vector<float>;
    MuonInfo_muon_energy_selection = new std::vector<float>;
    MuonInfo_muon_diphoton_deltaR_selection = new std::vector<float>;
    MuonInfo_muon_leadingPhoton_deltaR_selection = new std::vector<float>;
    MuonInfo_muon_subleadingPhoton_deltaR_selection = new std::vector<float>;
}
myParameters::~myParameters(){
    delete JetInfo_jet_pt_selection;
    delete JetInfo_jet_eta_selection;
    delete JetInfo_jet_phi_selection;
    delete JetInfo_jet_energy_selection;
    delete JetInfo_jet_diphoton_deltaR_selection;
    delete JetInfo_jet_leadingPhoton_deltaR_selection;
    delete JetInfo_jet_subleadingPhoton_deltaR_selection;
    delete JetInfo_jet_pfDeepCSVJetTags_probb_selection;
    delete JetInfo_jet_pfDeepCSVJetTags_probbb_selection;
    delete ElecInfo_electron_pt_selection;
    delete ElecInfo_electron_eta_selection;
    delete ElecInfo_electron_phi_selection;
    delete ElecInfo_electron_energy_selection;
    delete ElecInfo_electron_diphoton_deltaR_selection;
    delete ElecInfo_electron_leadingPhoton_deltaR_selection;
    delete ElecInfo_electron_subleadingPhoton_deltaR_selection;
    delete MuonInfo_muon_pt_selection;
    delete MuonInfo_muon_eta_selection;
    delete MuonInfo_muon_phi_selection;
    delete MuonInfo_muon_energy_selection;
    delete MuonInfo_muon_diphoton_deltaR_selection;
    delete MuonInfo_muon_leadingPhoton_deltaR_selection;
    delete MuonInfo_muon_subleadingPhoton_deltaR_selection;
}

void myTreeClass::InitTree(){
    mytree =  new TTree("mytree", "mytree");
}
void myTreeClass::MakeNewBranchAddresses(){
    mytree -> Branch("Mass_w_candidate_chi2_simple", &Mass_w_candidate_chi2_simple, "Mass_w_candidate_chi2_simple/F");
    mytree -> Branch("Mass_top_candidate_chi2_simple", &Mass_top_candidate_chi2_simple, "Mass_top_candidate_chi2_simple/F");
    mytree -> Branch("Mass_w_candidate_chi2_modified", &Mass_w_candidate_chi2_modified, "Mass_w_candidate_chi2_modified/F");
    mytree -> Branch("Mass_top_candidate_chi2_modified", &Mass_top_candidate_chi2_modified, "Mass_top_candidate_chi2_modified/F");
    mytree -> Branch("Mass_gen_w_candidate_chi2_simple", &Mass_gen_w_candidate_chi2_simple, "Mass_gen_w_candidate_chi2_simple/F");
    mytree -> Branch("Mass_gen_top_candidate_chi2_simple", &Mass_gen_top_candidate_chi2_simple, "Mass_gen_top_candidate_chi2_simple/F");
    mytree -> Branch("Mass_gen_w_candidate_chi2_modified", &Mass_gen_w_candidate_chi2_modified, "Mass_gen_w_candidate_chi2_modified/F");
    mytree -> Branch("Mass_gen_top_candidate_chi2_modified", &Mass_gen_top_candidate_chi2_modified, "Mass_gen_top_candidate_chi2_modified/F");
    //------------------------
    mytree -> Branch("EvtInfo_totalEntry_before_preselection", &EvtInfo_totalEntry_before_preselection, "EvtInfo_totalEntry_before_preselection/I");
    mytree -> Branch("EvtInfo_NormalizationFactor_lumi", &EvtInfo_NormalizationFactor_lumi, "EvtInfo_NormalizationFactor_lumi/F");
    mytree -> Branch("EvtInfo_NPu", &EvtInfo_NPu, "EvtInfo_NPu/I");
    mytree -> Branch("EvtInfo_Rho", &EvtInfo_Rho, "EvtInfo_Rho/F");
    mytree -> Branch("EvtInfo_NVtx", &EvtInfo_NVtx, "EvtInfo_NVtx/I");
    mytree -> Branch("EvtInfo_genweight", &EvtInfo_genweight, "EvtInfo_genweight/F");
    //------------------------
    mytree -> Branch("DiPhoInfo_mass", &DiPhoInfo_mass, "DiPhoInfo_mass/F");
    mytree -> Branch("DiPhoInfo_pt", &DiPhoInfo_pt, "DiPhoInfo_pt/F");
    mytree -> Branch("DiPhoInfo_eta", &DiPhoInfo_eta, "DiPhoInfo_eta/F");
    mytree -> Branch("DiPhoInfo_phi", &DiPhoInfo_phi, "DiPhoInfo_phi/F");
    mytree -> Branch("DiPhoInfo_energy", &DiPhoInfo_energy, "DiPhoInfo_energy/F");
    mytree -> Branch("DiPhoInfo_leadPt", &DiPhoInfo_leadPt, "DiPhoInfo_leadPt/F");
    mytree -> Branch("DiPhoInfo_leadEta", &DiPhoInfo_leadEta, "DiPhoInfo_leadEta/F");
    mytree -> Branch("DiPhoInfo_leadPhi", &DiPhoInfo_leadPhi, "DiPhoInfo_leadPhi/F");
    mytree -> Branch("DiPhoInfo_leadE", &DiPhoInfo_leadE, "DiPhoInfo_leadE/F");
    mytree -> Branch("DiPhoInfo_leadhoe", &DiPhoInfo_leadhoe, "DiPhoInfo_leadhoe/F");
    mytree -> Branch("DiPhoInfo_leadIDMVA", &DiPhoInfo_leadIDMVA, "DiPhoInfo_leadIDMVA/F");
    mytree -> Branch("DiPhoInfo_subleadPt", &DiPhoInfo_subleadPt, "DiPhoInfo_subleadPt/F");
    mytree -> Branch("DiPhoInfo_subleadEta", &DiPhoInfo_subleadEta, "DiPhoInfo_subleadEta/F");
    mytree -> Branch("DiPhoInfo_subleadPhi", &DiPhoInfo_subleadPhi, "DiPhoInfo_subleadPhi/F");
    mytree -> Branch("DiPhoInfo_subleadE", &DiPhoInfo_subleadE, "DiPhoInfo_subleadE/F");
    mytree -> Branch("DiPhoInfo_subleadhoe", &DiPhoInfo_subleadhoe, "DiPhoInfo_subleadhoe/F");
    mytree -> Branch("DiPhoInfo_subleadIDMVA", &DiPhoInfo_subleadIDMVA, "DiPhoInfo_subleadIDMVA/F");
    //------------------------
    mytree -> Branch("ElecInfo_Size", &ElecInfo_Size, "ElecInfo_Size/I");
    mytree -> Branch("MuonInfo_Size", &MuonInfo_Size, "MuonInfo_Size/I");
    mytree -> Branch("num_leptons", &num_leptons, "num_leptons/I");// # of selected objects.
    mytree -> Branch("num_electrons", &num_electrons, "num_electrons/I");// # of selected objects.
    mytree -> Branch("num_muons", &num_muons, "num_muons/I");// # of selected objects.
    mytree -> Branch("ElecInfo_electron_pt", &ElecInfo_electron_pt);
    mytree -> Branch("ElecInfo_electron_eta", &ElecInfo_electron_eta);
    mytree -> Branch("ElecInfo_electron_phi", &ElecInfo_electron_phi);
    mytree -> Branch("ElecInfo_electron_energy", &ElecInfo_electron_energy);
    mytree -> Branch("ElecInfo_electron_diphoton_deltaR", &ElecInfo_electron_diphoton_deltaR);
    mytree -> Branch("ElecInfo_electron_leadingPhoton_deltaR", &ElecInfo_electron_leadingPhoton_deltaR);
    mytree -> Branch("ElecInfo_electron_subleadingPhoton_deltaR", &ElecInfo_electron_subleadingPhoton_deltaR);
    mytree -> Branch("MuonInfo_muon_pt", &MuonInfo_muon_pt);
    mytree -> Branch("MuonInfo_muon_eta", &MuonInfo_muon_eta);
    mytree -> Branch("MuonInfo_muon_phi", &MuonInfo_muon_phi);
    mytree -> Branch("MuonInfo_muon_energy", &MuonInfo_muon_energy);
    mytree -> Branch("MuonInfo_muon_diphoton_deltaR", &MuonInfo_muon_diphoton_deltaR);
    mytree -> Branch("MuonInfo_muon_leadingPhoton_deltaR", &MuonInfo_muon_leadingPhoton_deltaR);
    mytree -> Branch("MuonInfo_muon_subleadingPhoton_deltaR", &MuonInfo_muon_subleadingPhoton_deltaR);
    //------------------------
    mytree -> Branch("jets_size", &jets_size, "jets_size/I");
    mytree -> Branch("num_jets", &num_jets, "num_jets/I");
    mytree -> Branch("JetInfo_jet_pt", &JetInfo_jet_pt);
    mytree -> Branch("JetInfo_jet_eta", &JetInfo_jet_eta);
    mytree -> Branch("JetInfo_jet_phi", &JetInfo_jet_phi);
    mytree -> Branch("JetInfo_jet_energy", &JetInfo_jet_energy);
    mytree -> Branch("JetInfo_jet_diphoton_deltaR", &JetInfo_jet_diphoton_deltaR);
    mytree -> Branch("JetInfo_jet_leadingPhoton_deltaR", &JetInfo_jet_leadingPhoton_deltaR);
    mytree -> Branch("JetInfo_jet_subleadingPhoton_deltaR", &JetInfo_jet_subleadingPhoton_deltaR);
    mytree -> Branch("JetInfo_jet_pfDeepCSVJetTags_probb", &JetInfo_jet_pfDeepCSVJetTags_probb);
    mytree -> Branch("JetInfo_jet_pfDeepCSVJetTags_probbb", &JetInfo_jet_pfDeepCSVJetTags_probbb);
    //mytree -> Branch("num_btagged_jets", &num_btagged_jets, "num_btagged_jets/I");
    //mytree -> Branch("num_nonbtagged_jets", &num_nonbtagged_jets, "num_nonbtagged_jets/I");
    //------------------------
    //mytree -> Branch("inv_mass_dijet", &inv_mass_dijet, "inv_mass_dijet/F");
    //mytree -> Branch("inv_mass_diphoton", &inv_mass_diphoton, "inv_mass_diphoton/F");
    //mytree -> Branch("inv_mass_tbw", &inv_mass_tbw, "inv_mass_tbw/F");
}
void myTreeClass::Fill(){
    mytree -> Fill();
}
void myParameters::Clear(){
    EvtInfo_totalEntry_before_preselection = 0;
    EvtInfo_NormalizationFactor_lumi = 0;
    //------------------------
    DiPhoInfo_eta = 0;
    DiPhoInfo_phi = 0;
    DiPhoInfo_energy = 0;
    //------------------------
    num_jets = 0;
    JetInfo_jet_pt.clear();
    JetInfo_jet_eta.clear();
    JetInfo_jet_phi.clear();
    JetInfo_jet_energy.clear();
    JetInfo_jet_diphoton_deltaR.clear();
    JetInfo_jet_leadingPhoton_deltaR.clear();
    JetInfo_jet_subleadingPhoton_deltaR.clear();
    JetInfo_jet_pfDeepCSVJetTags_probb.clear();
    JetInfo_jet_pfDeepCSVJetTags_probbb.clear();
    //------------------------
    num_leptons = 0;
    num_electrons = 0;
    num_muons = 0;
    ElecInfo_electron_pt.clear();
    ElecInfo_electron_eta.clear();
    ElecInfo_electron_phi.clear();
    ElecInfo_electron_energy.clear();
    ElecInfo_electron_diphoton_deltaR.clear();
    ElecInfo_electron_leadingPhoton_deltaR.clear();
    ElecInfo_electron_subleadingPhoton_deltaR.clear();
    MuonInfo_muon_pt.clear();
    MuonInfo_muon_eta.clear();
    MuonInfo_muon_phi.clear();
    MuonInfo_muon_energy.clear();
    MuonInfo_muon_diphoton_deltaR.clear();
    MuonInfo_muon_leadingPhoton_deltaR.clear();
    MuonInfo_muon_subleadingPhoton_deltaR.clear();
    //------------------------
    //Not used in preselection stage
    //------------------------
    num_btagged_jets = 0;
    num_nonbtagged_jets = 0;
    //------------------------
    //Chi-2 sorting related
    //------------------------
    Mass_w_candidate_chi2_simple = 0;
    Mass_top_candidate_chi2_simple = 0;
    Mass_w_candidate_chi2_modified = 0;
    Mass_top_candidate_chi2_modified = 0;
    Mass_gen_w_candidate_chi2_simple = 0;
    Mass_gen_top_candidate_chi2_simple = 0;
    Mass_gen_w_candidate_chi2_modified = 0;
    Mass_gen_top_candidate_chi2_modified = 0;

    inv_mass_dijet = 0;
    inv_mass_diphoton = 0;
    inv_mass_tbw = 0;
    //------------------------
    JetInfo_dijet_delta_eta = 0;
    JetInfo_dijet_delta_phi = 0;
    JetInfo_dijet_delta_angle = 0;
    //------------------------
}
//### }}}
