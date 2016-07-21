#!/usr/bin/env python
"""This script plots signal systematics for the RPV analysis"""
import sys
import math
import ROOT
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-i", "--input")
parser.add_argument("-m", "--mass")
args = parser.parse_args()
GLUINOMASS = 1200
if (args.input):
  infile = args.input
else:
  sys.exit("Please provide an input root file")
if (args.mass):
  GLUINOMASS = args.mass

one_pdf = True #put all plots in one pdf file
verbose = True  


# function to get pointers to histogram in root file
def get_hist_with_overflow(file,histname):
    if verbose:
        print" getting "+histname
    hist = file.Get(histname)
    nbinsX = hist.GetNbinsX()
    content = hist.GetBinContent(nbinsX) + hist.GetBinContent(nbinsX+1)
    error  = math.sqrt(math.pow(hist.GetBinError(nbinsX),2) + math.pow(hist.GetBinError(nbinsX+1),2))
    hist.SetBinContent(nbinsX,content)
    hist.SetBinContent(nbinsX+1,0) #avoid adding overflow N times for N calls to function
    hist.SetBinError(nbinsX+1,0)
    hist.SetBinError(nbinsX,error)
    return hist
    
#This function calculates symmetrized relative errors for a single variation in a single kinematic bin
#Return a histogram binned in Nb with the mean of the absolute value of relative errors up and down
def get_symmetrized_relative_errors(sysName,nominal,proc,sysFile,directory):


    # total hists for each variation, to include all processes
    systHistUp = ROOT.TH1F(directory+"_"+sysName+"_u","",5,0,5)
    systHistDown = ROOT.TH1F(directory+"_"+sysName+"_d","",5,0,5)

  
    #load hists and calculate SFs for floating component for each variation

    up = get_hist_with_overflow(sysFile,directory + "/" + proc + "_" + sysName + "Up")
    down =  get_hist_with_overflow(sysFile,directory + "/" + proc + "_" + sysName + "Down")


    #Put yields in new histogram to avoid modifying originals
    systHistUp.Add(up)
    systHistDown.Add(down)

    #Subtract nominal histogram from varied histograms
    systHistUp.Add(nominal,-1)     
    systHistDown.Add(nominal,-1)

    for i in range(1,systHistUp.GetNbinsX()+1):
        #Find absolute value of deviation
        systHistUp.SetBinContent(i,abs(systHistUp.GetBinContent(i)))
        systHistDown.SetBinContent(i,abs(systHistDown.GetBinContent(i)))


    #Fill histUp with symmetrized error by adding histDown and dividing by 2, then divide by nominal to get relative symmetrized error    
    systHistUp.Add(systHistDown)
    systHistUp.Scale(0.5)
    systHistUp.Divide(nominal) # now systHistUp contains our relative errors
    return systHistUp



  



ROOT.gROOT.SetBatch(ROOT.kTRUE) #prevent th1->Draw() from trying to open X11 window
ROOT.gStyle.SetCanvasDefW(600)
ROOT.gStyle.SetCanvasDefH(600)
ROOT.gStyle.SetTitleOffset(1.2,"x") 
#ROOT.gStyle.SetTitleOffset(1.7,"y")
ROOT.gStyle.SetTitleOffset(1.7,"z") 
#ROOT.gStyle.SetPadRightMargin(0.19) 
ROOT.gStyle.SetPadLeftMargin(0.12)
ROOT.gStyle.SetPadBottomMargin(0.12)
ROOT.gStyle.SetPadTopMargin(0.08)


ROOT.gStyle.SetLabelFont(42)
ROOT.gStyle.SetLabelSize(0.05)
ROOT.gStyle.SetTitleFont(42)
ROOT.gStyle.SetTitleSize(0.07)



#make list of systematics- name, title, plot color and line style
systList=[]
systList.append(["fs_btag_bc","FastSim b,c jet b-tag SF",2,1])
systList.append(["fs_btag_udsg","FastSim Light flavor jet b-tag SF",3,1])
systList.append(["fs_lep_eff","FastSim Lepton efficiency",4,1])
systList.append(["btag_bc","b,c jet b-tag SF",5,1])
systList.append(["btag_udsg","Light flavor jet b-tag SF",6,1])
systList.append(["jes","Jet energy scale",7,1])
systList.append(["jer","Jet energy resolution",8,1])
systList.append(["lep_eff","Lepton efficiency",9,1])
systList.append(["pileup","Pileup",10,1])
systList.append(["isr","Initial state radiation",11,1])
systList.append(["signal_mur","Renormalization scale",12,1])
systList.append(["signal_muf","Factorization scale",13,1])
systList.append(["signal_murf","Renorm. and fact. scale",14,1])
systList.append(["pdf","PDF",15,1])
systList.append(["mc_stat","MC statistics",1,2]) #must be done last!

nSyst = len(systList)
#make list of bins

binList = []
binList.append(["bin0","4 #leq n_{jets} #leq 5","500 #leq M_{J} < 800 GeV","n_{lep} = 0"])
binList.append(["bin1","6 #leq n_{jets} #leq 7","500 #leq M_{J} < 800 GeV","n_{lep} = 0"])
binList.append(["bin2","4 #leq n_{jets} #leq 5","500 #leq M_{J} < 800 GeV","n_{lep} = 1"])
binList.append(["bin3","4 #leq n_{jets} #leq 5","M_{J} #geq 800 GeV","n_{lep} = 0"])
binList.append(["bin4","6 #leq n_{jets} #leq 7","M_{J} #geq 800 GeV","n_{lep} = 0"])
binList.append(["bin5","4 #leq n_{jets} #leq 5","M_{J} #geq 800 GeV","n_{lep} = 1"])
#
#binList.append(["bin6","4 #leq n_{jets} #leq 5","300 #leq M_{J} < 500 GeV","n_{lep} = 0"])
#binList.append(["bin7","6 #leq n_{jets} #leq 7","300 #leq M_{J} < 500 GeV","n_{lep} = 0"])
#binList.append(["bin8","8 #leq n_{jets} #leq 9","300 #leq M_{J} < 500 GeV","n_{lep} = 0"])
#binList.append(["bin9","n_{jets} #geq 10","300 #leq M_{J} < 500 GeV","n_{lep} = 0"])
# signal regions
binList.append(["bin10","n_{jets} #geq 10","500 #leq M_{J} < 800 GeV","n_{lep} = 0"])
binList.append(["bin11","6 #leq n_{jets} #leq 7","500 #leq M_{J} < 800 GeV","n_{lep} = 1"])
binList.append(["bin12","n_{jets} #geq 8","500 #leq M_{J} < 800 GeV","n_{lep} = 1"])
binList.append(["bin13","n_{jets} #geq 10","M_{J} #geq 800 GeV","n_{lep} = 0"])
binList.append(["bin14","6 #leq n_{jets} #leq 7","M_{J} #geq 800 GeV","n_{lep} = 1"])
binList.append(["bin15","n_{jets} #geq 8","M_{J} #geq 800 GeV","n_{lep} = 1"])
binList.append(["bin16","8 #leq n_{jets} #leq 9","500 #leq M_{J} < 800 GeV","n_{lep} = 0"])
binList.append(["bin17","8 #leq n_{jets} #leq 9","M_{J} #geq 800 GeV","n_{lep} = 0"])


sysFile = ROOT.TFile(infile,"read")
proc = "signal_M" + str(GLUINOMASS)

for ibin in binList: 

    directory = ibin[0]
    if verbose:
        print "directory is "+directory
    
    nominal = get_hist_with_overflow(sysFile,(directory + "/" + proc))
    nbinsX = nominal.GetNbinsX()
    

    ROOT.gStyle.SetPadRightMargin()
    ROOT.gStyle.SetPadLeftMargin(0.12) #so it's not messed by larger table margin each iteration of the loop
    c = ROOT.TCanvas()
    leg = ROOT.TLegend(0.12,0.7,0.54,0.92)

    table = ROOT.TH2F("table_"+directory,"",nbinsX,-0.5,nbinsX-0.5,nSyst,0,nSyst)
    systHists_sym = []

    
    for isys, syst in enumerate(systList,start=1):
        sysName = syst[0]
        systHist = ROOT.TH1F(directory+"_"+sysName+"_sym","",5,0,5) # will eventually contain errors; define now to remain in scope
        if verbose:
            print "starting "+sysName

        if "mc_stat" not in sysName:
            #pdf treated separately
            if "pdf" not in sysName:
                #this function does everything
                systHist = get_symmetrized_relative_errors(sysName,nominal,proc,sysFile,directory)
            
                    
            elif "pdf" in sysName:
                #This systematic is calculated from 100 variations that all represent a single source of uncertainty.
                #We want to use information from all variations without artifically inflating the total uncertainty just by sampling the same effect many times.
                #Therefore, we find symmetrized uncertainties for each pdf variation up/down, add them in quadrature and divide by sqrt(100) to normalize
                for i in range(0,100):
                    #if i == 9 : continue 
                    #Get errors for this pdf variation
                    thisvar = get_symmetrized_relative_errors("w_pdf"+str(i),nominal,proc,sysFile,directory)
                    #Add in quadrature to running total
                    for i in range(1,systHist.GetNbinsX()+1):
                        thisvar.SetBinContent(i,math.pow(thisvar.GetBinContent(i),2))
                    systHist.Add(thisvar)
                
                #take square root and normalize by sqrt(100)
                for i in range(1,systHist.GetNbinsX()+1):
                        systHist.SetBinContent(i,math.pow(systHist.GetBinContent(i),0.5)/10)
                           
            
        elif "mc_stat" in sysName:
            systHist.Add(nominal)
            for i in range(1,systHist.GetNbinsX()+1):
                #in this case, we want our relative errors to just be the MC errors
                if systHist.GetBinContent(i)>0:
                    systHist.SetBinContent(i,(systHist.GetBinError(i)/systHist.GetBinContent(i))) 

        #normalize to percentage for humans            
        for i in range(1,systHist.GetNbinsX()+1):
            if systHist.GetBinContent(i) < 0.001: 
                table.SetBinContent(i,isys,0.1)
            else: 
                table.SetBinContent(i,isys,round(100*systHist.GetBinContent(i),1))
            if verbose:
                print "symmetrized rel error bin "+str(i)+" "+str(systHist.GetBinContent(i))         



        systHists_sym.append(systHist)


        table.GetYaxis().SetBinLabel(isys,syst[1])
        systHists_sym[isys-1].SetTitle(";N_{b};Relative Error")
        systHists_sym[isys-1].SetLineColor(syst[2])
        systHists_sym[isys-1].SetLineStyle(syst[3])
        systHists_sym[isys-1].SetLineWidth(2)
        systHists_sym[isys-1].SetMaximum(0.4)
        systHists_sym[isys-1].SetMinimum(0.)
        systHists_sym[isys-1].SetAxisRange(1,4,"X")
        systHists_sym[isys-1].SetStats(0)
        systHists_sym[isys-1].GetYaxis().SetTitleOffset(1.4)
        systHists_sym[isys-1].GetYaxis().SetTitleSize(0.04)
        systHists_sym[isys-1].GetXaxis().SetTitleSize(0.04)
        systHists_sym[isys-1].GetXaxis().SetNdivisions(505)
        leg.AddEntry(systHists_sym[isys-1],syst[1],"l")
        systHists_sym[isys-1].Draw("hist same")  



    leg.Draw()
    tla = ROOT.TLatex()
    tla.SetTextSize(0.038)
    tla.DrawLatexNDC(0.12,0.93,"#font[62]{CMS} #scale[0.8]{#font[52]{Simulation}}")
    tla.SetTextFont(42)
    tla.DrawLatexNDC(0.71,0.93,"#sqrt{s} = 13 TeV")
#    tla.SetTextSize(0.045)
    tla.DrawLatexNDC(0.17, 0.65, ibin[3])
    tla.DrawLatexNDC(0.17, 0.6, ibin[1])
    tla.DrawLatexNDC(0.17, 0.55, ibin[2])
    if one_pdf:
        if directory == binList[0][0]:
            outname = "plots/sig_systs_all_m" + str(GLUINOMASS) + ".pdf("
        elif directory == binList[len(binList)-1][0]:
            outname = "plots/sig_systs_all_m" + str(GLUINOMASS) + ".pdf)"
        else:
            outname = "plots/sig_systs_all_m" + str(GLUINOMASS) + ".pdf"

    else:
         outname = "plots/sig_systs_" + directory + "_m" + str(GLUINOMASS) + ".pdf"
    print "outname is " +outname
    c.Print(outname)


    #ROOT.gStyle.SetPadLeftMargin(0.25)
    ROOT.gStyle.SetPadLeftMargin(0.3)
    ROOT.gStyle.SetPadRightMargin(0.2)
    c2 = ROOT.TCanvas()
    table.GetXaxis().SetNdivisions(505)
    table.SetMaximum(20)
    table.SetMinimum(0)
    table.SetStats(0)
    table.SetMarkerSize(1.5)
    table.SetAxisRange(0.5,4.499,"X")
    table.SetXTitle("N_{b}")
    table.SetZTitle("Uncertainty [%]")
    table.GetYaxis().SetTitleOffset(1.4)
    table.GetYaxis().SetTitleSize(0.04)
    table.GetXaxis().SetTitleSize(0.04)
    table.Draw("colz text")
    ROOT.gPad.SetTicks(1,0)
    table.Draw("axis y+ same")
    tla = ROOT.TLatex()
    tla.SetTextSize(0.038)
    tla.DrawLatexNDC(0.3,0.93,"#font[62]{CMS} #scale[0.8]{#font[52]{Simulation}}")
    tla.SetTextFont(42)
    tla.DrawLatexNDC(0.66,0.93,"#sqrt{s} = 13 TeV")
    if one_pdf:
        if directory == binList[0][0]:
            outname = "plots/table_sig_systs_all_m" + str(GLUINOMASS) + ".pdf("
        elif directory == binList[len(binList)-1][0]:
            outname = "plots/table_sig_systs_all_m" + str(GLUINOMASS) + ".pdf)"
        else:
            outname = "plots/table_sig_systs_all_m" + str(GLUINOMASS) + ".pdf"

    else:
         outname = "plots/table_sig_systs_" + directory + "_m" + str(GLUINOMASS) + ".pdf"

         
    c2.Print(outname)





            
