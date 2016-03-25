#!/usr/bin/env python
import os, sys, re
import glob
import string
from array import array 
from pprint import pprint
import math
import ROOT
from ROOT import TMath
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-i","--input")
args = parser.parse_args()
if (args.input):
  infile = args.input
else:
  sys.exit("Please provide an input root file")


verbose = False  


ROOT.gROOT.SetBatch(ROOT.kTRUE) #prevent th1->Draw() from trying to open X11 window
ROOT.gStyle.SetCanvasDefW(600);
ROOT.gStyle.SetCanvasDefH(600);
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
systList.append(["btag_bc","b,c jet b-tag SF",2,1])
systList.append(["btag_udsg","Light flavor jet b-tag SF",3,1])
systList.append(["jes","Jet energy scale",4,1])
systList.append(["lep_eff","Lepton efficiency",6,1])
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
binList.append(["bin6","4 #leq n_{jets} #leq 5","300 #leq M_{J} < 500 GeV","n_{lep} = 0"])
binList.append(["bin7","6 #leq n_{jets} #leq 7","300 #leq M_{J} < 500 GeV","n_{lep} = 0"])
binList.append(["bin8","8 #leq n_{jets} #leq 9","300 #leq M_{J} < 500 GeV","n_{lep} = 0"])
binList.append(["bin9","n_{jets} #geq 10","300 #leq M_{J} < 500 GeV","n_{lep} = 0"])
# signal regions
binList.append(["bin10","n_{jets} #geq 10","500 #leq M_{J} < 800 GeV","n_{lep} = 0"])
binList.append(["bin11","6 #leq n_{jets} #leq 7","500 #leq M_{J} < 800 GeV","n_{lep} = 1"])
binList.append(["bin12","n_{jets} #geq 8","500 #leq M_{J} < 800 GeV","n_{lep} = 1"])
binList.append(["bin13","n_{jets} #geq 10","M_{J} #geq 800 GeV","n_{lep} = 0"])
binList.append(["bin14","6 #leq n_{jets} #leq 7","M_{J} #geq 800 GeV","n_{lep} = 1"])
binList.append(["bin15","n_{jets} #geq 8","M_{J} #geq 800 GeV","n_{lep} = 1"])



sysFile = ROOT.TFile(infile,"read")
for bin in binList:
    dir= bin[0]
    if verbose:
        print "dir is "+dir
    nominal = sysFile.Get(dir+"/signal")
    #add overflow to last bin
    nbinsX = nominal.GetNbinsX()
    if verbose:
        print "nbins x is"+ str(nbinsX)
    content = nominal.GetBinContent(nbinsX) + nominal.GetBinContent(nbinsX+1)
    error  = math.sqrt(math.pow(nominal.GetBinError(nbinsX),2) + math.pow(nominal.GetBinError(nbinsX+1),2))
    nominal.SetBinContent(nbinsX,content)
    nominal.SetBinError(nbinsX,error)

    ROOT.gStyle.SetPadRightMargin()
    ROOT.gStyle.SetPadLeftMargin(0.12) #so it's not messed by larger table margin each iteration of the loop
    c = ROOT.TCanvas()
    leg = ROOT.TLegend(0.12,0.7,0.54,0.92)

    table = ROOT.TH2F("table_"+dir,"",nbinsX,-0.5,nbinsX-0.5,nSyst,0,nSyst)
    
    for isys,sys in enumerate(systList,start=1):
        sysName = sys[0]
        if verbose:
            print "starting "+sysName
        if "mc_stat" not in sysName:
            histUp = sysFile.Get(dir+"/signal_"+sysName+"Up")
            histDown = sysFile.Get(dir+"/signal_"+sysName+"Down")

            #add overflow to last bin
            contentUp = histUp.GetBinContent(nbinsX) + histUp.GetBinContent(nbinsX+1)
            contentDown = histDown.GetBinContent(nbinsX) + histDown.GetBinContent(nbinsX+1)
            histUp.SetBinContent(nbinsX,contentUp)
            histDown.SetBinContent(nbinsX,contentDown)

            #dump counts
            if verbose:
                for i in range(1,histUp.GetNbinsX()+1):
                    print "histUp bin "+str(i)+" "+str(histUp.GetBinContent(i))
                    print "nominal bin "+str(i)+" "+str(nominal.GetBinContent(i))
                    print "histDown bin "+str(i)+" "+str(histDown.GetBinContent(i))

            #find deviation    
            histUp.Add(nominal,-1)
            histDown.Add(nominal,-1)



            for i in range(1,histUp.GetNbinsX()+1):
                #Find absolute value of deviation
                histUp.SetBinContent(i,abs(histUp.GetBinContent(i)))
                histDown.SetBinContent(i,abs(histDown.GetBinContent(i)))
                if verbose:
                    print "histUp Abs Error bin "+str(i)+" "+str(histUp.GetBinContent(i))
                    print "histDown Abs Error bin "+str(i)+" "+str(histDown.GetBinContent(i))

            #Fill histUp with symmetrized error by adding histDown and dividing by 2, then divide by nominal to get relative symmetrized error    
            histUp.Add(histDown)
            histUp.Scale(0.5)
            histUp.Divide(nominal) # now histUp contains our relative errors

            
            
        elif "mc_stat" in sysName: #this will affect nominal
            histUp = nominal 
            for i in range(1,histUp.GetNbinsX()+1):
                #in this case, we want our relative errors to just be the MC errors
                if histUp.GetBinContent(i)>0:
                    histUp.SetBinContent(i,(histUp.GetBinError(i)/histUp.GetBinContent(i)))
                

       
        for i in range(1,histUp.GetNbinsX()+1):
            table.SetBinContent(i,isys,round(100*histUp.GetBinContent(i),1))
            if verbose:
                print "symmetrized rel error bin "+str(i)+" "+str(histUp.GetBinContent(i))        

        table.GetYaxis().SetBinLabel(isys,sys[1])        
        histUp.SetTitle(";N_{b};Relative Error")
        histUp.SetLineColor(sys[2])
        histUp.SetLineStyle(sys[3])
        histUp.SetLineWidth(2)
        histUp.SetMaximum(1.)
        histUp.SetMinimum(0.)
        histUp.SetAxisRange(1,4,"X")
        histUp.SetStats(0)
        histUp.GetYaxis().SetTitleOffset(1.4)
        histUp.GetYaxis().SetTitleSize(0.04)
        histUp.GetXaxis().SetTitleSize(0.04)
        histUp.GetXaxis().SetNdivisions(505)
        leg.AddEntry(histUp,sys[1],"l")
        histUp.Draw("hist same")    

    leg.Draw()
    tla = ROOT.TLatex()
    tla.SetTextSize(0.038)
    tla.DrawLatexNDC(0.12,0.93,"#font[62]{CMS} #scale[0.8]{#font[52]{Simulation}}")
    tla.SetTextFont(42)
    tla.DrawLatexNDC(0.71,0.93,"#sqrt{s} = 13 TeV")
#    tla.SetTextSize(0.045)
    tla.DrawLatexNDC(0.17,0.65,bin[3])
    tla.DrawLatexNDC(0.17,0.6,bin[1])
    tla.DrawLatexNDC(0.17,0.55,bin[2])
    outname = "plots/sig_systs_"+dir+".pdf"
    c.Print(outname)


    ROOT.gStyle.SetPadLeftMargin(0.25)
    ROOT.gStyle.SetPadRightMargin(0.2)
    c2 = ROOT.TCanvas()
    table.GetXaxis().SetNdivisions(505)
    table.SetMaximum(50)
    table.SetMinimum(0)
    table.SetStats(0)
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
    tla.DrawLatexNDC(0.25,0.93,"#font[62]{CMS} #scale[0.8]{#font[52]{Simulation}}")
    tla.SetTextFont(42)
    tla.DrawLatexNDC(0.66,0.93,"#sqrt{s} = 13 TeV")
    outname = "plots/table_sig_systs_"+dir+".pdf"
    c2.Print(outname)
    
