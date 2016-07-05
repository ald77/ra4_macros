// plot_note: Macro that plots variables both lumi weighted and normalized to the same area.

#include <iostream>
#include <vector>
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TColor.h"

#include "styles.hpp"
#include "utilities.hpp"
#include "utilities_macros.hpp"
#include "utilities_macros_rpv.hpp"

namespace {
    TString plot_type=".pdf";
    TString plot_style="CMSPaper";
}

using namespace std;
using std::cout;
using std::endl;


int main(int argc, char *argv[]){
    // don't want to include RA4 trigger efficiency
    std::string extraWeight("w_pu_rpv/eff_trig");
    //  std::string extraWeight("(w_btag_loose*sys_bctag_loose[0]/w_btag)*w_pu_rpv/eff_trig");
    //  std::string extraWeight("(w_btag_loose*sys_udsgtag_loose[1]/w_btag)*w_pu_rpv/eff_trig");
    bool showData=true;
    bool nminus1=false;
    TString outDir("rpv_base");
    if(argc==2) {
        TString nMinus1String("nminus1");
        if(argv[1]!=nMinus1String) {
            cout << "Only valid option is 'nminus1'. If no options are specified, the default plots are generated" << endl;
            return 1;
        }
        else {
            nminus1=true;
            showData=false; // don't show data in signal region
            outDir="nminus1";
        }
    }

    // TString folder_links="/homes/cawest/links/";

    vector<TString> s_rpv;
    s_rpv.push_back("/homes/cawest/babymaker/CMSSW_7_4_14/src/babymaker/RPV_M1000.root");
    vector<TString> s_rpv_M1100;
    s_rpv_M1100.push_back("/homes/cawest/babymaker/CMSSW_7_4_14/src/babymaker/RPV_M1100.root");
    vector<TString> s_rpv_NLO;
    s_rpv_NLO.push_back("/homes/cawest/CMSSW_7_4_14/src/babymaker/RPV_M1000_NLO.root");

    vector<TString> s_tt_had;
    // this dataset is skimmed to remove the leptonic component
    s_tt_had.push_back("/net/cms2/cms2r0/jaehyeokyoo/babies/skim_ht1200/*TTJets_TuneCUETP8M1_13TeV-madgraphMLM*");
    vector<TString> s_tt;
    //  s_tt.push_back(filestring("TT_TuneCUETP8M1_13TeV-powheg-pythia8"));
    s_tt.push_back(filestring("TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_tt.push_back(filestring("TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    s_tt.push_back(filestring("TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_tt.push_back(filestring("TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    s_tt.push_back(filestring("TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_tt.push_back(filestring("TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    vector<TString> s_wjets;
    s_wjets.push_back(filestring("WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    vector<TString> s_singlet;
    s_singlet.push_back(filestring("ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1"));
    s_singlet.push_back(filestring("ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
    s_singlet.push_back(filestring("ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
    s_singlet.push_back(filestring("ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
    s_singlet.push_back(filestring("ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"));
    vector<TString> s_qcd;
    s_qcd.push_back(filestring("QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_qcd.push_back(filestring("QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    s_qcd.push_back(filestring("QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_qcd.push_back(filestring("QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    s_qcd.push_back(filestring("QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_qcd.push_back(filestring("QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1"));
    vector<TString> s_other;
    s_other.push_back(filestring("DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    s_other.push_back(filestring("TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"));
    s_other.push_back(filestring("TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"));
    s_other.push_back(filestring("TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
    s_other.push_back(filestring("TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
    s_other.push_back(filestring("ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8"));
    s_other.push_back(filestring("TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8"));
    vector<TString> s_w_had;
    s_w_had.push_back(filestring("WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"));
    vector<TString> s_z_had;
    s_z_had.push_back(filestring("ZJetsToQQ_HT600toInf_13TeV-madgraph"));
    vector<TString> s_jetht;
    s_jetht.push_back(filestring("JetHT_Run2015C_25ns-05Oct2015-v1"));
    s_jetht.push_back(filestring("JetHT_Run2015D-05Oct2015-v1"));
    s_jetht.push_back(filestring("JetHT_Run2015D-PromptReco-v4"));

    // Reading ntuples
    vector<sfeats> Samples; 
    if(showData) {
        Samples.push_back(sfeats(s_jetht, "Data",kBlack,1,"trig[12] && pass && (njets<10 || (nmus+nels)==0)"));
        Samples.back().isData = true;
        Samples.back().doStack = false;
    }
    Samples.push_back(sfeats(s_rpv, "#tilde{g}(1.0 TeV)#rightarrow tbs", ra4::c_t1tttt)); 
    Samples.back().doStack = false;
    Samples.back().isSig = true;
    Samples.push_back(sfeats(s_rpv_M1100, "#tilde{g}(1.1 TeV)#rightarrow tbs", ra4::c_t1tttt, 2)); 
    Samples.back().doStack = false;
    Samples.back().isSig = true;
    Samples.push_back(sfeats(s_qcd, "QCD", kYellow, 1, cutandweight("1",extraWeight))); 
    Samples.push_back(sfeats(s_w_had, "W+jets, 0 l", ra4::c_wjets, 1, cutandweight("1",extraWeight)));
    Samples.push_back(sfeats(s_z_had, "Z+jets, 0 l", kBlack, 1, cutandweight("1",extraWeight)));
    Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", ra4::c_tt_1l, 1, cutandweight("ntruleps==1", extraWeight)));
    Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", ra4::c_tt_2l, 1, cutandweight("ntruleps>=2", extraWeight)));
    Samples.push_back(sfeats(s_tt_had, "t#bar{t}, 0 l", kTeal, 1, cutandweight("ntruleps==0", extraWeight)));
    Samples.push_back(sfeats(s_wjets, "W+jets, 1 l", ra4::c_wjets, 1, cutandweight("1",extraWeight)));
    Samples.push_back(sfeats(s_singlet, "Single t", ra4::c_singlet, 1, cutandweight("1",extraWeight)));
    Samples.push_back(sfeats(s_other, "Other", ra4::c_other, 1, cutandweight("1",extraWeight))); 


    // Adding non-skimmed samples
    vector<int> rpv_sam;
    unsigned nsam(Samples.size());
    for(unsigned sam(0); sam < nsam; sam++){
        rpv_sam.push_back(sam);
        vector<TString> sam_files = Samples[sam].file;
        Samples.push_back(sfeats(sam_files, Samples[sam].label, Samples[sam].color, Samples[sam].style,
                    Samples[sam].cut));
    } // Loop over samples

    vector<hfeats> vars;

    TString cuts("(nmus+nels)==1");

    std::vector<TString> basecut = {"(nmus+nels)==0", "(nmus+nels)==1"};
    std::vector<TString> mjcuts = {"mj<=300", "mj>300&&mj<=500", "mj>500&&mj<=800", "mj>800"};
    std::vector<TString> njetcuts = {"njets>=4&&njets<=5", "njets>=6&&njets<=7", "njets>=8&&njets<=9", "njets>=10"};
    TString htcut("ht>1500");
    if(!nminus1) {
        for(auto ibasecut : basecut) {
            for(auto imjcut : mjcuts) {
                for(auto ijetcut : njetcuts) {
                    // skip blinded regions
                    bool isBlind = (ibasecut.EqualTo("(nmus+nels)==0") && !ijetcut.EqualTo("njets>=4&&njets<=5") && !ijetcut.EqualTo("njets>=6&&njets<=7" )) ||
                        (ibasecut.EqualTo("(nmus+nels)==1") && !ijetcut.EqualTo("njets>=4&&njets<=5"));
                    // reenable all regions for low mj
                    if(imjcut.EqualTo("mj>300&&mj<=500" && !ibasecut.EqualTo("(nmus+nels)==1"))) isBlind=false;
                    if(ibasecut.EqualTo("(nmus+nels)==0") && (imjcut.EqualTo("mj<=300") || (imjcut.EqualTo("mj>300&&mj<=500")))) isBlind=false;
                    if(isBlind && showData) continue;
                    if(ibasecut=="(nmus+nels)==1") {
                        ijetcut.ReplaceAll("njets>=10","njets>=8");
                        htcut="ht>1200";
                    }
                    cuts = ibasecut + "&&" + htcut + "&&" + ijetcut + "&&" + imjcut;
                    // vars.push_back(hfeats("ht",40, 0, 4000, rpv_sam, "H_{T} (GeV)", cuts));
                    // vars.back().normalize = true;
                    // vars.push_back(hfeats("mj",25, 0, 2500, rpv_sam, "M_{J} (GeV)", cuts));
                    // vars.back().normalize = true;
                    // vars.push_back(hfeats("dr_bb",15, 0, 6, rpv_sam, "#DeltaR_{b#bar{b}}", cuts));
                    // vars.back().normalize = true;
                    vars.push_back(hfeats("nbm", 4, 1, 5, rpv_sam, "N_{b}", cuts));
                    vars.back().normalize = true;
                    // vars.push_back(hfeats("Sum$(jets_csv>0.605)", 4, 1, 5, rpv_sam, "N_{b,loose}", cuts));
                    // vars.back().normalize = true;
                    // vars.push_back(hfeats("njets",20, 0, 20, rpv_sam, "N_{jets}", cuts));
                    // vars.back().normalize = true;
                    // vars.push_back(hfeats("jets_pt[0]",30, 0, 1500, rpv_sam, "p_{T,1} (GeV)", cuts));
                    // vars.back().normalize = true;
                    // vars.push_back(hfeats("jets_pt[1]",30, 0, 1500, rpv_sam, "p_{T,2} (GeV)", cuts));
                    // vars.back().normalize = true;
                    // vars.push_back(hfeats("met",30, 0, 1500, rpv_sam, "MET (GeV)", cuts));
                    // vars.back().normalize = true;
                }
            }
        }

        //Njets plots
        htcut="ht>1500";
        for(auto ibasecut : basecut) {
            for(auto imjcut : mjcuts) {
                TString ijetcut = "njets<=7"; //Blind safe
                if(!showData) ijetcut = "njets>=0";
                if(ibasecut=="(nmus+nels)==1") {
                    ijetcut.ReplaceAll("njets<=7","njets<=5");
                    htcut="ht>1200";
                }
                cuts = ibasecut + "&&" + htcut + "&&" + ijetcut + "&&" + imjcut;

                vars.push_back(hfeats("njets",20, 0, 20, rpv_sam, "N_{jets}", cuts));
                vars.back().normalize = true; vars.back().whichPlots = "1";
            }
        }
        //mj plots
        htcut="ht>1500";
        for(auto ibasecut : basecut) {     
            for(auto ijetcut : njetcuts) {
                // skip blinded regions
                bool isBlind = (ibasecut.EqualTo("(nmus+nels)==0") && !ijetcut.EqualTo("njets>=4&&njets<=5") && !ijetcut.EqualTo("njets>=6&&njets<=7" )) ||
                    (ibasecut.EqualTo("(nmus+nels)==1") && !ijetcut.EqualTo("njets>=4&&njets<=5"));
                if(isBlind && showData) continue;
                if(ibasecut=="(nmus+nels)==1") {
                    ijetcut.ReplaceAll("njets>=10","njets>=8");
                    htcut="ht>1200";
                }
                cuts = ibasecut + "&&" + htcut + "&&" + ijetcut;

                vars.push_back(hfeats("mj",25, 0, 2500, rpv_sam, "M_{J} (GeV)", cuts));
                vars.back().normalize = true; vars.back().whichPlots = "1";
            }    
        }




    } // end if(!minus1)

    // make N-1 plots for signal regions
    else {
        std::vector<std::string> basecutsNm1 = {"(nmus+nels)==0", "(nmus+nels)==1"};
        std::vector<std::string> htcutsNm1 = {"ht>1500", "ht>1200"};
        std::vector<std::string> mjcutsNm1 = {"mj>800", "mj>500"};
        std::vector<std::string> njetcutsNm1 = {"njets>=10", "njets>=8"};
        std::vector<std::string> nbcutsNm1 = {"nbm>=3", "nbm>=3"};
        for(unsigned int i=0; i<basecutsNm1.size(); i++) {
            std::string cutsNm1=basecutsNm1[i]+"&&"+mjcutsNm1[i]+"&&"+njetcutsNm1[i]+"&&"+nbcutsNm1[i];
            vars.push_back(hfeats("ht",40, 0, 4000, rpv_sam, "H_{T} (GeV)", cutsNm1));
            cutsNm1=basecutsNm1[i]+"&&"+htcutsNm1[i]+"&&"+njetcutsNm1[i]+"&&"+nbcutsNm1[i];
            vars.push_back(hfeats("mj",25, 0, 2500, rpv_sam, "M_{J} (GeV)", cutsNm1));
            cutsNm1=basecutsNm1[i]+"&&"+htcutsNm1[i]+"&&"+mjcutsNm1[i]+"&&"+nbcutsNm1[i];
            vars.push_back(hfeats("njets",20, 0, 20, rpv_sam, "N_{jets}", cutsNm1));
            cutsNm1=basecutsNm1[i]+"&&"+htcutsNm1[i]+"&&"+mjcutsNm1[i]+"&&"+njetcutsNm1[i];
            vars.push_back(hfeats("nbm",6, 0, 6, rpv_sam, "N_{b}", cutsNm1));
            cutsNm1=basecutsNm1[i]+"&&"+htcutsNm1[i]+"&&"+mjcutsNm1[i]+"&&"+njetcutsNm1[i]+"&&"+nbcutsNm1[i];
            vars.push_back(hfeats("jets_pt[0]",30, 0, 1500, rpv_sam, "p_{T,1} (GeV)", cutsNm1));
        }
    }

    plot_distributions(Samples, vars, rpv::luminosity, plot_type, plot_style, outDir, showData, true); // last argument determines whether or not a ratio is drawn, and whether or not to show cuts

}

