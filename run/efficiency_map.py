#!/usr/bin/env python
import os, sys, re
import glob
import string
from array import array 
from pprint import pprint
from get_xsec import signalCrossSection
from set_palette import set_palette
import ROOT
from ROOT import TMath
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-b","--babydir")
parser.add_argument("-i","--input")
parser.add_argument("-o","--outfilename")
parser.add_argument("-f","--first")
parser.add_argument("-l","--last")
args = parser.parse_args()

def get_weighted_entries(chain, cuts):
     temphist = ROOT.TH1D("temp","temp",2,0,2)
     chain.Project("temp","1","weight*("+cuts+")")
     n = temphist.GetBinContent(2)
     temphist.Delete()
     return n


if (args.babydir):
  outdir = args.babydir
else:
  sys.exit("Please provide a baby directory")


#these options allow you to run on a subset of mass points for parallelization and to avoid the mysterious memory leak crash
#Just combined output rootfiles using hadd and feed back into this script to complete cosmetics
# Usage example: one job with -l 350 (no need to specify -f) and one job with -f 350 (no need to specify -l) to split roughly in half


if (args.first):
  minnum = int(args.first)
else: minnum=0

if (args.last):
  maxnum = int(args.last)
else: maxnum=9999

print "starting with "+str(minnum)
print "until "+str(maxnum)    
    
  
#To do cosmetic changes without recalculating the histograms, provide the output root file from a previous iteration in the input field   
if (args.input):
  skipcalc = True
  infile = args.input
else:
  skipcalc = False 
  print "Recalculating.."

if (args.outfilename):
    outfile = args.outfilename
    if ".root" in outfile: outfile = outfile.split(".root")[0]
else: sys.exit("Please provide name for output root file")   
  
  
ROOT.gROOT.SetBatch(ROOT.kTRUE) #prevent th1->Draw() from trying to open X11 window
ROOT.gStyle.SetCanvasDefW(600);
ROOT.gStyle.SetCanvasDefH(600);
ROOT.gStyle.SetTitleOffset(1.2,"x"); 
ROOT.gStyle.SetTitleOffset(1.7,"y");
ROOT.gStyle.SetTitleOffset(1.7,"z"); 
ROOT.gStyle.SetPadRightMargin(0.18); 
ROOT.gStyle.SetPadLeftMargin(0.12);


bdir = os.getcwd()

lumi = 2.1
#outdir = os.path.join(bdir,'out',timestamp)

print outdir
if not os.path.exists(outdir):
    sys.exit("Can't find babies directory %s" %outdir)

if not skipcalc and not os.path.exists("plots/maps"):
    sys.exit("Can't find directory for output root file plots/maps")


###### With the fake dataset names:
masspoints = set([x.split("T1tttt_").pop().split("_Tune")[0] for x in glob.glob(outdir+"/baby_SMS-T1tttt*_renorm_nleps1__htg500__metg200__njetsge6__nbmge1__mjg250.root")])
# Needs renormalized ABCD skim
#masspoints = masspoints - set(["mGluino-725_mLSP-425","mGluino-1850_mLSP-850","mGluino-1350_mLSP-750"]) ## remove points with weight NaN
npoints = len(masspoints)
if npoints ==0:
    sys.exit("Need to run on renormalized ABCD skim")


#define all bins and cosmetic names
bindefs = ["njets<=8&&nbm==1&&met<=400","njets>=9&&nbm==1&&met<=400","njets<=8&&nbm==2&&met<=400","njets>=9&&nbm==2&&met<=400","njets<=8&&nbm>=3&&met<=400","njets>=9&&nbm>=3&&met<=400","njets<=8&&nbm==1&&met>400","njets>=9&&nbm==1&&met>400","njets<=8&&nbm>=2&&met>400","njets>=9&&nbm>=2&&met>400"]
cosmeticbindefs = [cut.replace("<="," #leq ").replace(">="," #geq ").replace(">"," > ").replace("=="," = ").replace("&&",", ").replace("met", "MET").replace("ht", "H_{T}").replace("njets","n_{jets}").replace("nleps","n_{lep}").replace("nbm","n_{b}") for cut in bindefs]

glumass = []
lspmass = []
effs = [[] for i in range(2+len(bindefs))]
yields = [[] for i in range(2+len(bindefs))] # one for R4, one for R3, one for each R4 bin
sofar=0 #to count progress through mass points
if not skipcalc: # if not supplying root file 
       
    #loop over mass points
    for m in masspoints:
        sofar+=1
        #if sofar<=minnum: continue 
       ## if sofar>10: continue
        ch = ROOT.TChain("tree")
        ch.Add(outdir+"/baby_SMS-T1tttt_"+m+"_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring15FSPremix-MCRUN2_74_V9*.root")

        glu = float(m.split("mGluino-").pop().split("_")[0])
        lsp = float(m.split("mLSP-").pop().split("_")[0])

        glumass.append(glu)
        lspmass.append(lsp)

        print "glu is " + str(glu)
        print "lsp is " + str(lsp)

        tot = 1000.*signalCrossSection(glu)
        print "tot = "+str(tot)
        #assume integral of weights was normalized correctly pre-skim

       
        reg4 = float(get_weighted_entries(ch,"mj>400&&mt>140")) # ASSUMES ABCD SKIM!
        reg3 = float(get_weighted_entries(ch,"mj<=400&&mt>140")) 
        # first vector in effs is R4 efficiency
        effs[0].append(reg4/tot)
        yields[0].append(lumi*reg4)
        effs[-1].append(reg3/tot) # putting R3 at very end because it is convenient
        yields[-1].append(lumi*reg3) 
        print "eff = "+str(effs[0][-1])
        print "completed "+str(sofar-1)+" out of "+str(npoints)

        for i,cut in enumerate(bindefs):
            #subsequent vectors in effs are bin efficiencies
            num = float(get_weighted_entries(ch,"mj>400&&mt>140&&"+cut)) # ASSUMES ABCD SKIM!
            #print "num is " + str(num)
            #print "frac is " +str(num/reg4)
            effs[i+1].append(num/reg4)
            yields[i+1].append(lumi*num) 
    
    outputFile = ROOT.TFile("plots/maps/"+outfile+".root","recreate")
    # convert lists into TGraphs
    graphs = []
    for k in array("d",glumass):
        print str(k)
    for k in array("d",lspmass):
        print str(k)
    for k in array("d",effs[1]):
        print str(k)
        
            
    for j,vec in enumerate(effs):
        if j==0:
            graphs.append(ROOT.TGraph2D("effmap","R4 Efficiency",len(glumass),array("d",glumass),array("d",lspmass),array("d",effs[j])))
            graphs.append(ROOT.TGraph2D("yieldmap","R4 Yield",len(glumass),array("d",glumass),array("d",lspmass),array("d",yields[j]))) 
        elif j-1 < len(cosmeticbindefs):
            graphs.append(ROOT.TGraph2D(cosmeticbindefs[j-1],"Fraction of R4 efficiency with "+cosmeticbindefs[j-1],len(glumass),array("d",glumass),array("d",lspmass),array("d",effs[j])))
            graphs.append(ROOT.TGraph2D(cosmeticbindefs[j-1]+"_yield","Yield in R4 with "+cosmeticbindefs[j-1],len(glumass),array("d",glumass),array("d",lspmass),array("d",yields[j])))
        else:
            graphs.append(ROOT.TGraph2D("effmapR3","R3 Efficiency",len(glumass),array("d",glumass),array("d",lspmass),array("d",effs[j])))
            graphs.append(ROOT.TGraph2D("yieldmapR3","R3 Yield",len(glumass),array("d",glumass),array("d",lspmass),array("d",yields[j])))

         #Fix empty bottom-left corner with average of two closest points on axis
        hist = graphs[-1].GetHistogram()
        val1 = hist.GetBinContent(hist.FindBin(700,0))
        val2 = hist.GetBinContent(hist.FindBin(600,100))
        print val1
        print val2
        print (val1+val2)/2.
        graphs[-1].SetPoint(graphs[-1].GetN(),ROOT.Double(600.),0.,(val1+val2)/2.)
        hist = graphs[-2].GetHistogram()
        val1 = hist.GetBinContent(hist.FindBin(700,0))
        val2 = hist.GetBinContent(hist.FindBin(600,100))
        graphs[-2].SetPoint(graphs[-2].GetN(),ROOT.Double(600.),0.,(val1+val2)/2.)
            
        graphs[-1].Write()
        graphs[-2].Write()
        
 
     
if skipcalc:
     outputFile = ROOT.TFile(infile,"read")

#### Get histograms from root file to allow reading of input to use same code for original calculation and cosmetic re-runs

#make title list to facilitate cosmetics later
graphTitles = []
graphTitles.append(["effmap","; m_{#tilde{g}} [GeV]; m_{#tilde{#chi}^{0}_{1}} [GeV]; Region 4 Efficiency"])
graphTitles.append(["yieldmap","; m_{#tilde{g}} [GeV]; m_{#tilde{#chi}^{0}_{1}} [GeV]; Region 4 Yield"])
for name in cosmeticbindefs:
    graphTitles.append([name,"; m_{#tilde{g}} [GeV]; m_{#tilde{#chi}^{0}_{1}} [GeV]; Fraction of R4 efficiency from "+name])
    graphTitles.append([name+"_yield","; m_{#tilde{g}} [GeV]; m_{#tilde{#chi}^{0}_{1}} [GeV]; R4 Yield with "+name])
graphTitles.append(["effmapR3","; m_{#tilde{g}} [GeV]; m_{#tilde{#chi}^{0}_{1}} [GeV]; Region 3 Efficiency"])
graphTitles.append(["yieldmapR3","; m_{#tilde{g}} [GeV]; m_{#tilde{#chi}^{0}_{1}} [GeV]; Region 3 Yield"])
    

# get max z-value
maxfrac = 0.
maxyield= 0.
for name,title in graphTitles:
    if "map" in name: continue
    print name
    graph = outputFile.Get(name)
    if "yield" not in name:
        if graph.GetHistogram().GetMaximum() > maxfrac: maxfrac = graph.GetHistogram().GetMaximum()
    else:
        if graph.GetHistogram().GetMaximum() > maxyield: maxyield = graph.GetHistogram().GetMaximum()

   
            
set_palette()
#ROOT.gStyle.SetPalette(56)

#npx = int((max(glumass) - min(glumass))/5)
#npy = int((max(lspmass) - min(lspmass))/5)
npx = int((1950-600)/12.5)
npy = int(1450/12.5)
print "max frac is "+str(maxfrac) 
for name,title in graphTitles:
    graph = outputFile.Get(name)
    graph.SetTitle(title)
    graph.SetNpx(npx)
    graph.SetNpy(npy)

    if "map" not in name: #standardize range for bin maps, not for inclusive map
        if"yield" not in name:
            graph.GetHistogram().SetMaximum(maxfrac)
            graph.GetHistogram().SetMinimum(0)
        else:
            graph.GetHistogram().SetMaximum(maxyield)
            graph.GetHistogram().SetMinimum(0.01)
       # graph.GetZaxis().SetRangeUser(0,maxfrac)
       # graph.SetMaximum(maxfrac)
       # graph.SetMinimum(max)
    c = ROOT.TCanvas()
   
   # hist.SetBinContent(hist.FindBin(600,0),(val1+val2)/2.)
    graph.Draw("colz")
    tla = ROOT.TLatex()
    tla.SetTextSize(0.03)
    if "yield" in name:  tla.DrawLatexNDC(0.66,0.92,"L = 2.1 fb^{-1} (13 TeV)")
    tla.DrawLatexNDC(0.12,0.92,"#font[62]{CMS} #scale[0.8]{#font[52]{Preliminary}}")
    tla.SetTextSize(0.035)
    tla.DrawLatexNDC(0.15,0.85,"pp #rightarrow #tilde{g}#tilde{g},  #tilde{g} #rightarrow t#bar{t} #tilde{#chi}_{1}^{0}")

    if "yield" not in name:
        outname =outfile+"efficiency_"+name.replace(" #leq","lte").replace(" #geq","gte").replace(" >","gt").replace(" =","").replace(", ","_").replace("H_{T}","ht").replace("n_{jets}","njets").replace("n_{lep}","nleps").replace("n_{b}","nbm").replace(" ","")
    else:
         outname =outfile+"yield_"+name.replace(" #leq","lte").replace(" #geq","gte").replace(" >","gt").replace(" =","").replace(", ","_").replace("H_{T}","ht").replace("n_{jets}","njets").replace("n_{lep}","nleps").replace("n_{b}","nbm").replace(" ","")    
    c.Print("plots/maps/"+outname+".pdf")
    c.SetLogz(True)
    c.Print("plots/maps/log_"+outname+".pdf")

outputFile.Close()      


