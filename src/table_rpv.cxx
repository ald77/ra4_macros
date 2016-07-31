#include <iostream> 
#include <vector> 
#include <iomanip> 

#include "TStyle.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TMath.h"

using namespace std;

bool doLatex=false;

float AddInQuad(float a, float b);
void printOneLine(int nb, 
                  float data, float qcd, float ttbar, float wjets, float other, 
                  float sig1000, float sig1100, float sig1200, float sig1300, float sig1400,
                  float data_err, float qcd_err, float ttbar_err, float wjets_err, float other_err, 
                  float sig1000_err, float sig1100_err, float sig1200_err, float sig1300_err, float sig1400_err, float doLatex);
void printYieldBin(int nb, float data,
                   float qcd, float ttbar, float wjets, float other, float sig,
                   float qcd_err, float ttbar_err, float wjets_err, float other_err,
                   float dataovermc, bool doNorm);


//
// main
//
int main()
{
    cout << " ........................... "<< endl; 
    cout << " .. I am making yield table " << endl; 
    cout << " ........................... "<< endl; 

    TString bin[18] = {
        // control regions
        "0-lepton,HT>1500,4#leq N_{jets}#leq5,500<MJ<800",
        "0-lepton,HT>1500,6#leq N_{jets}#leq7,500<MJ<800",
        "1-lepton,HT>1200,4#leq N_{jets}#leq5,500<MJ<800", 
        "0-lepton,HT>1500,4#leq N_{jets}#leq5,MJ>800",
        "0-lepton,HT>1500,6#leq N_{jets}#leq7,MJ>800",
        "1-lepton,HT>1200,4#leq N_{jets}#leq5,MJ>800",
        // low MJ control regions
        "0-lepton,HT>1500,4#leq N_{jets}#leq5,300<MJ<500",  // 6
        "0-lepton,HT>1500,6#leq N_{jets}#leq7,300<MJ<500",
        "0-lepton,HT>1500,8#leq N_{jets}#leq9,300<MJ<500",
        "0-lepton,HT>1500,N_{jets}#geq10,300<MJ<500",
        // signal regions, low MJ
        "0-lepton,HT>1500,N_{jets}#geq10,500<MJ<800",       // 10
        "1-lepton,HT>1200,6#leq N_{jets}#leq7,500<MJ<800",
        "1-lepton,HT>1200,N_{jets}#geq8,500<MJ<800",
        // signal regions, high MJ
        "0-lepton,HT>1500,N_{jets}#geq10,MJ>800",           // 13
        "1-lepton,HT>1200,6#leq N_{jets}#leq7,MJ>800",
        "1-lepton,HT>1200,N_{jets}#geq8,MJ>800",
        //Missing regions
        "0-lepton,HT>1500,8#leq N_{jets}#leq9,500<MJ<800",  // 16
        "0-lepton,HT>1500,8#leq N_{jets}#leq9,MJ>800"
    };

    TString binLatex[18] = {
        // control regions
        "N_{leps}=0,H_{T}>1500~\\GeV,4\\leq N_{jets}\\leq5, 500<M_{J}<800~\\GeV",
        "N_{leps}=0,H_{T}>1500~\\GeV,6\\leq N_{jets}\\leq7, 500<M_{J}<800~\\GeV",
        "N_{leps}=1,H_{T}>1200~\\GeV,4\\leq N_{jets}\\leq5, 500<M_{J}<800~\\GeV", 
        "N_{leps}=0,H_{T}>1500~\\GeV,4\\leq N_{jets}\\leq5, M_{J}>800~\\GeV",
        "N_{leps}=0,H_{T}>1500~\\GeV,6\\leq N_{jets}\\leq7, M_{J}>800~\\GeV",
        "N_{leps}=1,H_{T}>1200~\\GeV,4\\leq N_{jets}\\leq5, M_{J}>800~\\GeV",
        // low M_{J} control regions
        "N_{leps}=0,H_{T}>1500~\\GeV,4\\leq N_{jets}\\leq5,300<M_{J}<500~\\GeV",  // 6
        "N_{leps}=0,H_{T}>1500~\\GeV,6\\leq N_{jets}\\leq7,300<M_{J}<500~\\GeV",
        "N_{leps}=0,H_{T}>1500~\\GeV,8\\leq N_{jets}\\leq9,300<M_{J}<500~\\GeV",
        "N_{leps}=0,H_{T}>1500~\\GeV,N_{jets}\\geq10,300<M_{J}<500~\\GeV",
        // signal regions, low M_{J}
        "N_{leps}=0,H_{T}>1500~\\GeV,N_{jets}\\geq10,500<M_{J}<800~\\GeV",       // 10
        "N_{leps}=1,H_{T}>1200~\\GeV,6\\leq N_{jets}\\leq7,500<M_{J}<800~\\GeV",
        "N_{leps}=1,H_{T}>1200~\\GeV,N_{jets}\\geq8,500<M_{J}<800~\\GeV",
        // signal regions, high M_{J}
        "N_{leps}=0,H_{T}>1500~\\GeV,N_{jets}\\geq10,M_{J}>800~\\GeV",           // 13
        "N_{leps}=1,H_{T}>1200~\\GeV,6\\leq N_{jets}\\leq7,M_{J}>800~\\GeV",
        "N_{leps}=1,H_{T}>1200~\\GeV,N_{jets}\\geq8,M_{J}>800~\\GeV",
        //Missing regions
        "N_{leps}=0,H_{T}>1500~\\GeV,8\\leq N_{jets}\\leq9,500<M_{J}<800~\\GeV",  // 16
        "N_{leps}=0,H_{T}>1500~\\GeV,8\\leq N_{jets}\\leq9,M_{J}>800~\\GeV"
    };

    TFile* infile  = TFile::Open("variations/sum_rescaled.root", "READ");
  
    float data[18][5], qcd[18][5], ttbar[18][5], wjets[18][5], other[18][5],
          sig1000[18][5], sig1100[18][5], sig1200[18][5], sig1300[18][5], sig1400[18][5];
    float data_err[18][5], qcd_err[18][5], ttbar_err[18][5], wjets_err[18][5], other_err[18][5],
          sig1000_err[18][5], sig1100_err[18][5], sig1200_err[18][5], sig1300_err[18][5], sig1400_err[18][5];
    for(int ibin=0; ibin<18; ibin++)  
    { 
        if(ibin>=6 && ibin<=9) continue;
        for(int inb=0; inb<5; inb++) 
        {
            data[ibin][inb] = 0;
            qcd[ibin][inb] = 0;
            ttbar[ibin][inb] = 0;
            wjets[ibin][inb] = 0;
            other[ibin][inb] = 0;
            sig1000[ibin][inb] = 0;
            sig1100[ibin][inb] = 0;
            sig1200[ibin][inb] = 0;
            sig1300[ibin][inb] = 0;
            sig1400[ibin][inb] = 0;
            
            data_err[ibin][inb] = 0;
            qcd_err[ibin][inb] = 0;
            ttbar_err[ibin][inb] = 0;
            wjets_err[ibin][inb] = 0;
            other_err[ibin][inb] = 0;
            sig1000_err[ibin][inb] = 0;
            sig1100_err[ibin][inb] = 0;
            sig1200_err[ibin][inb] = 0;
            sig1300_err[ibin][inb] = 0;
            sig1400_err[ibin][inb] = 0;
        }
    }
    //
    for(int ibin=0; ibin<18; ibin++) 
    {
        if(ibin>=6 && ibin<=9) continue;
        //TDirectory* dir = infile->GetDirectory(Form("bin%i", ibin));

        for(int inb=1; inb<5; inb++) 
        {
            data[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/data_obs", ibin)))->GetBinContent(inb+1);
            qcd[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/qcd", ibin)))->GetBinContent(inb+1);
            ttbar[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/ttbar", ibin)))->GetBinContent(inb+1);
            wjets[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/wjets", ibin)))->GetBinContent(inb+1);
            other[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/other", ibin)))->GetBinContent(inb+1);
            sig1000[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/signal_M1000", ibin)))->GetBinContent(inb+1);
            sig1100[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/signal_M1100", ibin)))->GetBinContent(inb+1);
            sig1200[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/signal_M1200", ibin)))->GetBinContent(inb+1);
            sig1300[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/signal_M1300", ibin)))->GetBinContent(inb+1);
            sig1400[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/signal_M1400", ibin)))->GetBinContent(inb+1);
            
            data_err[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/data_obs", ibin)))->GetBinError(inb+1);
            qcd_err[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/qcd", ibin)))->GetBinError(inb+1);
            ttbar_err[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/ttbar", ibin)))->GetBinError(inb+1);
            wjets_err[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/wjets", ibin)))->GetBinError(inb+1);
            other_err[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/other", ibin)))->GetBinError(inb+1);
            sig1000_err[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/signal_M1000", ibin)))->GetBinError(inb+1);
            sig1100_err[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/signal_M1100", ibin)))->GetBinError(inb+1);
            sig1200_err[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/signal_M1200", ibin)))->GetBinError(inb+1);
            sig1300_err[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/signal_M1300", ibin)))->GetBinError(inb+1);
            sig1400_err[ibin][inb-1]= static_cast<TH1F*>(infile->Get(Form("bin%i/signal_M1400", ibin)))->GetBinError(inb+1);
        }  
    }
        
    float sig1000sum(0), sig1100sum(0), sig1200sum(0), sig1300sum(0), sig1400sum(0);
    for(int ibin=0; ibin<18; ibin++) 
    {
      if(ibin>5 && ibin<10) continue; // skip low MJ bins 
      for(int inb=1; inb<5; inb++)  
      {
          sig1000sum = sig1000sum + sig1000[ibin][inb-1]; 
          sig1100sum = sig1100sum + sig1100[ibin][inb-1]; 
          sig1200sum = sig1200sum + sig1200[ibin][inb-1]; 
          sig1300sum = sig1300sum + sig1300[ibin][inb-1]; 
          sig1400sum = sig1400sum + sig1400[ibin][inb-1]; 
      }
    }

    // -----------------------------------------------------
    // table for AN
    // -----------------------------------------------------
    for(int ibin=0; ibin<18; ibin++) 
    {

       if(ibin>5 && ibin<10) continue; // skip low MJ bins

       if(doLatex) 
       {

           cout << "\\begin{table}" << endl;
           cout << "\\centering" << endl;
           cout << "\\caption{Yields for $" << binLatex[ibin].Data()<< "$. Uncertainties are statistical only.}" << endl;
           cout << "\\begin{tabular}[tbp!]{ l | c  c  c  c | c  c  c  }" << endl;
           cout << "\\hline" << endl;
           cout << "$N_{b}$ & QCD & $t\\bar{t}$ & W+jets & Other & $m_{\\tilde{g}}=1000$ & $m_{\\tilde{g}}=1200$ & $m_{\\tilde{g}}=1400$ \\\\"  << endl;
           cout << "\\hline\\hline" << endl;
       }
       else 
       {
        cout << "|" << 
            left << setw(142) << Form("Bin %i: %s",ibin, (bin[ibin].ReplaceAll("#leq","<=").ReplaceAll("#geq",">=")).Data())   << " |"  << endl;
        cout << "|" << 
            setw(4) << Form("Nb")   << " |"  <<
            //setw(10) << Form("%.2f",data) << " |" << 
            setw(16) << "qcd"  << " |" <<
            setw(16) << "ttbar" << " |" <<
            setw(16) << "wjets" << " |" << 
            setw(16) << "other" << " |" << 
            setw(20) << "M=1000(S/B)"<< " |" << 
            setw(20) << "M=1200(S/B)" << " |" << 
            setw(20) << "M=1400(S/B)" << " |" << endl;
        } 



        for(int inb=1; inb<5; inb++) 
        { 
            printOneLine(inb,
                    data[ibin][inb-1], qcd[ibin][inb-1], ttbar[ibin][inb-1], wjets[ibin][inb-1], other[ibin][inb-1],
                    //sig1000[ibin][inb-1], sig1100[ibin][inb-1], sig1200[ibin][inb-1], sig1300[ibin][inb-1], sig1400[ibin][inb-1],
                    sig1000[ibin][inb-1]/sig1000sum, sig1100[ibin][inb-1]/sig1100sum, sig1200[ibin][inb-1]/sig1200sum, 
                        sig1300[ibin][inb-1]/sig1300sum, sig1400[ibin][inb-1]/sig1400sum,
                    data_err[ibin][inb-1], qcd_err[ibin][inb-1], ttbar_err[ibin][inb-1], wjets_err[ibin][inb-1], other_err[ibin][inb-1],
                    sig1000_err[ibin][inb-1], sig1100_err[ibin][inb-1], sig1200_err[ibin][inb-1], sig1300_err[ibin][inb-1], sig1400_err[ibin][inb-1],
                    doLatex);
        } 

        if(doLatex)  
        {
            cout<< "\\hline" << endl;
            cout << "\\end{tabular}"<<endl;
            cout << "\\end{table}\n"; 
            cout << endl;
        } 
        else 
        { 
            cout << endl;
        }
    }
    
    // -----------------------------------------------------
    // table for supplementary material
    // -----------------------------------------------------

    // Get pre-fit errors
    float err[4][18][4];

    for(int ibin=0; ibin<18; ibin++)
    {
        for(int inb=0; inb<4; inb++)
        {
            err[0][ibin][inb] = 0;
            err[1][ibin][inb] = 0;
            err[2][ibin][inb] = 0;
            err[3][ibin][inb] = 0;
        }
    }

    for(int ibin=0; ibin<18; ibin++) {
        
        if(ibin>=6 && ibin<=9) continue;

        for(int iproc=0; iproc<4; iproc++)
        {
            std::string process;
            if(iproc==0)  process = "qcd";
            if(iproc==1)  process = "ttbar";
            if(iproc==2)  process = "wjets";
            if(iproc==3)  process = "other";

            // loop over nb bins
            for(int inb=1; inb<5; inb++)
            {
                cout << "... " << process << endl;
                float central;
                float up;

                if(infile->Get(Form("bin%i/%s", ibin, process.c_str()))==0x0) continue;
                central = static_cast<TH1F*>(infile->Get(Form("bin%i/%s", ibin, process.c_str())))->GetBinContent(inb+1);

                if(central !=0)
                {
                    // btag bc
                    up = static_cast<TH1F*>(infile->Get(Form("bin%i/%s_btag_bcUp", ibin, process.c_str())))->GetBinContent(inb+1);
                    err[iproc][ibin][inb-1] = TMath::Abs(up-central)/central;

                    // btag udsg
                    up = static_cast<TH1F*>(infile->Get(Form("bin%i/%s_btag_udsgUp", ibin, process.c_str())))->GetBinContent(inb+1);
                    err[iproc][ibin][inb-1] = AddInQuad(err[iproc][ibin][inb-1],TMath::Abs(up-central)/central);

                    // pileup
                    up = static_cast<TH1F*>(infile->Get(Form("bin%i/%s_pileupUp", ibin, process.c_str())))->GetBinContent(inb+1);
                    err[iproc][ibin][inb-1] = AddInQuad(err[iproc][ibin][inb-1],TMath::Abs(up-central)/central);

                    // qcd flavor
                    if(iproc==0)
                    {
                        up = static_cast<TH1F*>(infile->Get(Form("bin%i/%s_qcd_flavorUp", ibin, process.c_str())))->GetBinContent(inb+1);
                        err[iproc][ibin][inb-1] = AddInQuad(err[iproc][ibin][inb-1],TMath::Abs(up-central)/central);
                    }

                    // scale variations
                    up = static_cast<TH1F*>(infile->Get(Form("bin%i/%s_%s_murUp", ibin, process.c_str(), process.c_str())))->GetBinContent(inb+1);
                    err[iproc][ibin][inb-1] = AddInQuad(err[iproc][ibin][inb-1],TMath::Abs(up-central)/central);
                    up = static_cast<TH1F*>(infile->Get(Form("bin%i/%s_%s_mufUp", ibin, process.c_str(), process.c_str())))->GetBinContent(inb+1);
                    err[iproc][ibin][inb-1] = AddInQuad(err[iproc][ibin][inb-1],TMath::Abs(up-central)/central);
                    up = static_cast<TH1F*>(infile->Get(Form("bin%i/%s_%s_murfUp", ibin, process.c_str(), process.c_str())))->GetBinContent(inb+1);
                    err[iproc][ibin][inb-1] = AddInQuad(err[iproc][ibin][inb-1],TMath::Abs(up-central)/central);

                    // jer jes
                    up = static_cast<TH1F*>(infile->Get(Form("bin%i/%s_jerUp", ibin, process.c_str())))->GetBinContent(inb+1);
                    err[iproc][ibin][inb-1] = AddInQuad(err[iproc][ibin][inb-1],TMath::Abs(up-central)/central);
                    up = static_cast<TH1F*>(infile->Get(Form("bin%i/%s_jesUp", ibin, process.c_str())))->GetBinContent(inb+1);
                    err[iproc][ibin][inb-1] = AddInQuad(err[iproc][ibin][inb-1],TMath::Abs(up-central)/central);
                    // pdf
                    float pdferr=0;
                    for(int ipdf=0; ipdf<100; ipdf++)
                    {
                        up = static_cast<TH1F*>(infile->Get(Form("bin%i/%s_w_pdf%iUp", ibin, process.c_str(), ipdf)))->GetBinContent(inb+1);
                        pdferr  = AddInQuad(pdferr,TMath::Abs(up-central)/central);
                    }
                    err[iproc][ibin][inb-1] = AddInQuad(err[iproc][ibin][inb-1],pdferr/10);
                    cout << "pdf: " <<  ibin << " " << inb << " " << pdferr << endl;

                    // Stats
                    if(infile->Get(Form("bin%i/%s_mcstat_%s_bin%i_nb%iUp", ibin, process.c_str(), process.c_str(),ibin,inb))!=0x0)
                    {
                        up = static_cast<TH1F*>(infile->Get(Form("bin%i/%s_mcstat_%s_bin%i_nb%iUp", ibin, process.c_str(), process.c_str(),ibin,inb)))->GetBinContent(inb+1);
                        err[iproc][ibin][inb-1] = AddInQuad(err[iproc][ibin][inb-1],TMath::Abs(up-central)/central);
                    }
                }
                cout << "both: " <<  ibin << " " << inb << " " << err[iproc][ibin][inb-1]<< endl;
            }
        }
    }
    
    //
    // 0-lepton
    //
    int tablebin_0lep[4]={16,17,10,13};
    cout << "\\begin{table}" << endl;
    cout << "\\centering" << endl;
    cout << "\\begin{tabular}[tbp!]{ l | c  c  c  c | c |  c | c  }" << endl;
    cout << "\\hline" << endl;
    cout << "$N_{b}$ & QCD & $t\\bar{t}$ & W+jets & Other & All bkg. & Data & $m_{\\tilde{g}}=1200\\GeV$\\\\"  << endl;
    cout << "\\hline\\hline" << endl;

    for(int ibin=0; ibin<4; ibin++)
    { 
        int tablebin=tablebin_0lep[ibin];
        
        float databin(0), mcbin(0); 
        for(int inb=1; inb<5; inb++) 
        { 
            databin = databin + data[tablebin][inb-1]; 
            mcbin = mcbin + qcd[tablebin][inb-1] + ttbar[tablebin][inb-1] + wjets[tablebin][inb-1] + other[tablebin][inb-1]; 
        }

        cout <<"\\multicolumn{8}{c}{$" <<  binLatex[tablebin].Data() << "$} \\\\" << endl;
        cout << "\\hline" << endl;
        for(int inb=1; inb<5; inb++)
            printYieldBin(inb,
                    data[tablebin][inb-1],
                    qcd[tablebin][inb-1],
                    ttbar[tablebin][inb-1],
                    wjets[tablebin][inb-1],
                    other[tablebin][inb-1],
                    sig1200[tablebin][inb-1],
                    err[0][tablebin][inb-1]*qcd[tablebin][inb-1],
                    err[1][tablebin][inb-1]*ttbar[tablebin][inb-1],
                    err[2][tablebin][inb-1]*wjets[tablebin][inb-1],
                    err[3][tablebin][inb-1]*other[tablebin][inb-1],
                    databin/mcbin, true /*renormalize to data*/);
        cout << "\\hline" << endl;
    }
    cout<< "\\hline" << endl;
    cout << "\\end{tabular}"<<endl;
    cout << "\\end{table}\n"<< endl;

    cout << endl;
    cout << endl;


    //
    // 1-lepton
    //
    int tablebin_1lep[4]={11,14,12,15};
    cout << "\\begin{table}" << endl;
    cout << "\\centering" << endl;
    cout << "\\begin{tabular}[tbp!]{ l | c  c  c  c | c |  c | c  }" << endl;
    cout << "\\hline" << endl;
    cout << "$N_{b}$ & QCD & $t\\bar{t}$ & W+jets & Other & All bkg. & Data & $m_{\\tilde{g}}=1200\\GeV$\\\\"  << endl;
    cout << "\\hline\\hline" << endl;

    for(int ibin=0; ibin<4; ibin++)
    { 
        int tablebin=tablebin_1lep[ibin];
        
        float databin(0), mcbin(0); 
        for(int inb=1; inb<5; inb++) 
        { 
            databin = databin + data[tablebin][inb-1]; 
            mcbin = mcbin + qcd[tablebin][inb-1] + ttbar[tablebin][inb-1] + wjets[tablebin][inb-1] + other[tablebin][inb-1]; 
        }

        cout <<"\\multicolumn{8}{c}{$" <<  binLatex[tablebin].Data() << "$} \\\\" << endl;
        cout << "\\hline" << endl;
        for(int inb=1; inb<5; inb++)
            printYieldBin(inb,
                    data[tablebin][inb-1],
                    qcd[tablebin][inb-1],
                    ttbar[tablebin][inb-1],
                    wjets[tablebin][inb-1],
                    other[tablebin][inb-1],
                    sig1200[tablebin][inb-1],
                    err[0][tablebin][inb-1]*qcd[tablebin][inb-1],
                    err[1][tablebin][inb-1]*ttbar[tablebin][inb-1],
                    err[2][tablebin][inb-1]*wjets[tablebin][inb-1],
                    err[3][tablebin][inb-1]*other[tablebin][inb-1],
                    databin/mcbin, true /*renormalize to data*/);
        cout << "\\hline" << endl;
    }
    cout<< "\\hline" << endl;
    cout << "\\end{tabular}"<<endl;
    cout << "\\end{table}\n"<< endl;

    infile->Close();

    return 0;
}

//
void printOneLine(int nb, 
                  float data, float qcd, float ttbar, float wjets, float other, 
                  float sig1000, float sig1100, float sig1200, float sig1300, float sig1400,
                  float data_err, float qcd_err, float ttbar_err, float wjets_err, float other_err, 
                  float sig1000_err, float sig1100_err, float sig1200_err, float sig1300_err, float sig1400_err, float formatLatex)
{

    float how_to_deal_with_unused_vars;
    how_to_deal_with_unused_vars=data;
    how_to_deal_with_unused_vars=data_err;
    how_to_deal_with_unused_vars=sig1100;
    how_to_deal_with_unused_vars=sig1100_err;
    how_to_deal_with_unused_vars=sig1300;
    how_to_deal_with_unused_vars=sig1300_err;
    how_to_deal_with_unused_vars=4;

    float totbkg=qcd+ttbar+wjets+other;

    if(formatLatex) 
    {
       cout << Form("%i", nb)   << " & "  
            << Form("$%.2f\\pm%.2f$",qcd,qcd_err)  << " & "
            << Form("$%.2f\\pm%.2f$",ttbar,ttbar_err) << " & " 
            << Form("$%.2f\\pm%.2f$",wjets,wjets_err) << " & " 
            << Form("$%.2f\\pm%.2f$",other,other_err) << " & " 
            //<< Form("$%.0f$",data) << " & " 
            << Form("$%.2f\\pm%.2f$",sig1000,sig1000_err) << " & " 
            << Form("$%.2f\\pm%.2f$",sig1200,sig1200_err) << " & " 
            << Form("$%.2f\\pm%.2f$",sig1400,sig1400_err) << " \\\\" << endl;
    } 
    else 
    { 
       cout << "|" << 
            setw(how_to_deal_with_unused_vars) << nb   << " |"  <<
            setw(16) << Form("%.2f+-%.2f",qcd,qcd_err)  << " |" <<
            setw(16) << Form("%.2f+-%.2f",ttbar,ttbar_err) << " |" <<
            setw(16) << Form("%.2f+-%.2f",wjets,wjets_err) << " |" << 
            setw(16) << Form("%.2f+-%.2f",other,other_err) << " |" << 
            //setw(16) << Form("%.2f+-%.2f",data) << " |" << 
            setw(20) << Form("%.2f+-%.2f(%.2f)",sig1000,sig1000_err,sig1000/totbkg ) << " |" << 
            setw(20) << Form("%.2f+-%.2f(%.2f)",sig1200,sig1200_err,sig1200/totbkg) << " |" << 
            setw(20) << Form("%.2f+-%.2f(%.2f)",sig1400,sig1400_err,sig1400/totbkg) << " |" << endl;
    }
} 

void printYieldBin(int nb, float data, 
        float qcd, float ttbar, float wjets, float other, float sig,
        float qcd_err, float ttbar_err, float wjets_err, float other_err, 
        float dataovermc, bool doNorm)
{
    if(qcd==0) qcd_err=0;
    if(ttbar==0) ttbar_err=0;
    if(wjets==0) wjets_err=0;
    if(other==0) other_err=0;

    string nbbin;
    if(nb==1) nbbin="$1$";
    if(nb==2) nbbin="$2$";
    if(nb==3) nbbin="$3$";
    if(nb==4) nbbin="$\\geq 4$";
   
    if(doNorm)  
    { 
        qcd = qcd * dataovermc;     qcd_err = qcd_err * dataovermc; 
        ttbar = ttbar * dataovermc; ttbar_err = ttbar_err * dataovermc; 
        wjets = wjets * dataovermc; wjets_err = wjets_err * dataovermc; 
        other = other * dataovermc; other_err = other_err * dataovermc; 
    }

    float tot_err = TMath::Sqrt(qcd_err*qcd_err+ttbar_err*ttbar_err+wjets_err*wjets_err+other_err*other_err);
   
    cout << nbbin << " & "
        << Form("$%.1f \\pm %.1f$",qcd,qcd_err)  << " & "
        << Form("$%.1f \\pm %.1f$",ttbar,ttbar_err) << " & "
        << Form("$%.1f \\pm %.1f$",wjets,wjets_err) << " & "
        << Form("$%.1f \\pm %.1f$",other,other_err) << " & "
        << Form("$%.1f \\pm %.1f$",qcd+ttbar+wjets+other,tot_err) << " & "
        << Form("$%.0f$",data) << " & "
        << Form("$%.1f$",sig) << " \\\\ " << endl;
}

float AddInQuad(float a, float b)
{
    return TMath::Sqrt(a*a+b*b);
}


