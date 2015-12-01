#!/usr/bin/env python
import os, sys, re
import glob
import string
from array import array 
from pprint import pprint
from get_xsec import signalCrossSection
from set_palette import set_palette
import ROOT
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-b","--babydir")
parser.add_argument("-i","--input")
parser.add_argument("-o","--outfilename")
parser.add_argument("-f","--first")
parser.add_argument("-l","--last")
args = parser.parse_args()

#def get_weighted_entries(chain, cuts):
 #    temphist = ROOT.TH1D("temp","temp",2,0,2)
  #   chain.Project("temp","1","weight*("+cuts+")")
   #  n = temphist.GetBinContent(2)
    # temphist.Delete()
     #return n


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

bdir = os.getcwd()


#outdir = os.path.join(bdir,'out',timestamp)

print outdir
if not os.path.exists(outdir):
    sys.exit("Can't find babies directory %s" %outdir)

if not skipcalc and not os.path.exists("plots/maps"):
    sys.exit("Can't find directory for output root file plots/maps")

masspoints = []
glumass = []
lspmass = []  

######This is for the skim filename convention without fake dataset names

#assumes format baby_T1tttt.....batch9_mgluino1250_mlsp500.root 
#startlist = set([x.split("batch").pop().split(".root")[0] for x in glob.glob(outdir+"/baby_SMS-T1tttt*.root")])

#extract mass points from string list- split by "_"
# two cases: batch9_mgluino1250_mlsp500.root -> 3 pieces
#batch9_rs0_mgluino1250_mlsp500.root -> 4 pieces
## for y in startlist:
##     pieces = y.split("_")
##     if len(pieces) > 4 : print "Some skim was parsed incorrectly"
##     if len(pieces) == 4 : #resubmit
##         masspoints.append(pieces[2]+"_"+pieces[3]) 
##         glumass.append(int(pieces[2].split("gluino").pop())) 
##         lspmass.append(int(pieces[3].split("lsp").pop()))    
##     if len(pieces) == 3:
##         masspoints.append(pieces[1]+"_"+pieces[2])
##         glumass.append(int(pieces[1].split("gluino").pop()))
##         lspmass.append(int(pieces[2].split("lsp").pop()))

#######

###### With the fake dataset names:
masspoints = set([x.split("T1tttt_").pop().split("_Tune")[0] for x in glob.glob(outdir+"/baby_SMS-T1tttt*.root")])
for y in masspoints:
    #print y
    glumass.append(int(y.split("mGluino-").pop().split("_")[0]))
    #print str(glumass[-1])
    lspmass.append(int(y.split("mLSP-").pop().split("_")[0]))    
    #print str(lspmass[-1])





#use "set" to remove duplicates        
npoints = len(masspoints)      
glumass = set(glumass) # list of ints
lspmass = set(lspmass) # list of ints

maxglu= max(glumass)
minglu = min(glumass)
maxlsp = max(lspmass)
minlsp = min(lspmass)




print "mgluino range "+str(minglu)+" to "+str(maxglu)
print "mlsp range "+str(minlsp)+" to "+str(maxlsp)

nbinsx = (maxglu-minglu)/25 + 1
nbinsy = (maxlsp-minlsp)/25 + 1

if nbinsy > nbinsx: nbinsx = nbinsy    #helps make bins appear square

#bins of width 25, centered on the mass points
effmap = ROOT.TH2D("effmap","R4 Efficiency",nbinsx,minglu-12.5,minglu-12.5+25*nbinsx,nbinsy,minlsp-12.5,maxlsp+12.5)

#define all bins and cosmetic names
bindefs = ["njets<=8&&nbm==1&&met<=400","njets>=9&&nbm==1&&met<=400","njets<=8&&nbm==2&&met<=400","njets>=9&&nbm==2&&met<=400","njets<=8&&nbm>=3&&met<=400","njets>=9&&nbm>=3&&met<=400","njets<=8&&nbm==1&&met>400","njets>=9&&nbm==1&&met>400","njets<=8&&nbm>=2&&met>400","njets>=9&&nbm>=2&&met>400"]
cosmeticbindefs = [cut.replace("<="," #leq ").replace(">="," #geq ").replace(">"," > ").replace("=="," = ").replace("&&",", ").replace("met", "MET").replace("ht", "H_{T}").replace("njets","n_{jets}").replace("nleps","n_{lep}").replace("nbm","n_{b}") for cut in bindefs]

sofar=0 #to count progress through mass points
#maxfrac = 0.0
binhists = []
if not skipcalc: # if not supplying root file 
    for cosmetic in cosmeticbindefs:
        print cosmetic
        #define hists
        binhists.append(ROOT.TH2D(cosmetic,"Fraction of R4 efficiency with "+cosmetic,nbinsx,minglu-12.5,minglu-12.5+25*nbinsx,nbinsy,minlsp-12.5,maxlsp+12.5))
    #loop over mass points
    temphist = ROOT.TH1D("temp","temp",2,0,2)
    for m in masspoints:
        sofar+=1
        if sofar<=minnum: continue 
        if sofar>maxnum: continue
        ch = ROOT.TChain("tree")
        #old format
        #ch.Add(outdir+"/baby_SMS-T1tttt*FSPremix*"+m+".root")
        #pieces = m.split("_")
        #glu = int(pieces[0].split("gluino").pop())
        #lsp = int(pieces[1].split("lsp").pop())

        #new format
        #add files
        ch.Add(outdir+"/baby_SMS-T1tttt_"+m+"*FSPremix*.root")
        #get glu and lsp mass
        glu = int(m.split("mGluino-").pop().split("_")[0])
        lsp = int(m.split("mLSP-").pop().split("_")[0])
        print "glu is " + str(glu)
        print "lsp is " + str(lsp)
        #tot = float(ch.GetEntries())
        ch.Project("temp","1","weight*(1)")
        tot = float(temphist.GetBinContent(2))
        temphist.Reset()
        
        print m
        print "tot = "+str(tot)
        #First find R4 yield, and clone tree to avoid needing to reapply R4 cuts for each bin
        r4ch = ch.CopyTree("nleps==1&&mt>140&&mj>400&&ht>500&&met>200&&njets>=6&&nbm>=1"); #who needs a looper!
        ROOT.SetOwnership(r4ch, True)
      #  reg4 = float(r4ch.GetEntries())
        r4ch.Project("temp","1","weight*(1)")
        #reg4 = float(get_weighted_entries(r4ch,"1"))
        reg4 = float(temphist.GetBinContent(2))
        temphist.Reset()
        print "reg4 = "+str(reg4)
        effmap.SetBinContent(effmap.FindBin(glu,lsp),reg4/tot)
        print "eff = "+str(reg4/tot)
        
        print "completed "+str(sofar-1)+" out of "+str(npoints)
        #loop over bins
        for hist,cut in zip(binhists,bindefs):
            #binyield = float(r4ch.GetEntries(cut))
           # binyield = float(get_weighted_entries(r4ch,cut))
            r4ch.Project("temp","1","weight*("+cut+")")
        #reg4 = float(get_weighted_entries(r4ch,"1"))
            binyield = float(temphist.GetBinContent(2))
            temphist.Reset()
            hist.SetBinContent(hist.FindBin(glu,lsp),binyield/reg4)
        #r4ch.Delete()
            
        #ch.Delete()
    
     
    # save output root file to allow easy cosmetic changes 
    outputFile = ROOT.TFile("plots/maps/"+outfile+".root","recreate")
    effmap.Write()
    for hist in binhists:
        hist.Write()
        
 
     
if skipcalc:
     outputFile = ROOT.TFile(infile,"read")

#### Get histograms from root file to allow reading of input to use same code for original calculation and cosmetic re-runs

#make title list to facilitate cosmetics later
histTitles = []
histTitles.append(["effmap","; m_{gluino} [GeV]; m_{LSP} [GeV]"])
for name in cosmeticbindefs:
    histTitles.append([name,"Fraction of R4 efficiency from "+name+"; m_{gluino} [GeV]; m_{LSP} [GeV]"])
maxfrac = 0.
# get max z-value
for name,title in histTitles:
    if "effmap" in name: continue
    print name
    hist = outputFile.Get(name)
    if hist.GetMaximum() > maxfrac: maxfrac = hist.GetMaximum()

set_palette()
            
for name,title in histTitles:
    hist = outputFile.Get(name)
    #These are to remove buggy mass-points in this round of the scan production
    hist.SetBinContent(hist.FindBin(725,425),0)
    hist.SetBinContent(hist.FindBin(1850,850),0)
    hist.SetBinContent(hist.FindBin(1350,750),0)
    hist.SetBinContent(hist.FindBin(1300,100),0)
    hist.SetBinContent(hist.FindBin(1350,100),0)
    hist.SetBinContent(hist.FindBin(1400,100),0)
    hist.SetBinContent(hist.FindBin(1450,100),0)
    hist.SetBinContent(hist.FindBin(1550,100),0)
    hist.SetBinContent(hist.FindBin(1500,100),0)
    hist.SetBinContent(hist.FindBin(1600,100),0)
    hist.SetBinContent(hist.FindBin(1650,100),0)
    hist.SetBinContent(hist.FindBin(1700,100),0)
    hist.SetBinContent(hist.FindBin(1800,100),0)
    hist.SetBinContent(hist.FindBin(1850,100),0)
    hist.SetBinContent(hist.FindBin(1750,100),0)
    hist.SetBinContent(hist.FindBin(1900,100),0)
    hist.SetBinContent(hist.FindBin(1950,100),0)
    hist.SetBinContent(hist.FindBin(1050,50),0)
    hist.SetBinContent(hist.FindBin(1100,50),0)
    hist.SetBinContent(hist.FindBin(1150,50),0)
    hist.SetBinContent(hist.FindBin(1200,50),0)
    hist.SetTitle(title)
    hist.SetStats(0)
    hist.SetTitleOffset(1.2,"X")
    hist.SetTitleOffset(1.2,"Y")
    if "effmap" not in name:
       hist.SetMaximum(maxfrac)
      
    c = ROOT.TCanvas()
    hist.Draw("colz")
    tla = ROOT.TLatex()
    tla.SetTextSize(0.03)
    tla.DrawLatexNDC(0.1,0.92,"#font[62]{CMS} #scale[0.8]{#font[52]{Preliminary}}")

    outname =outfile+"efficiency_"+name.replace(" #leq","lte").replace(" #geq","gte").replace(" >","gt").replace(" =","").replace(", ","_").replace("H_{T}","ht").replace("n_{jets}","njets").replace("n_{lep}","nleps").replace("n_{b}","nbm").replace(" ","")
    c.Print("plots/maps/"+outname+".pdf")
    c.SetLogz(True)
    c.Print("plots/maps/log_"+outname+".pdf")

#Yield Hists
# In order to not rely on the luminosity weight normalization, take the efficiency as fundamental quantity and look up cross section on the fly

maxyield=0
r4effhist = outputFile.Get("effmap") # need to use this to properly normalize yield for each bin
for name,title in histTitles:
    hist = outputFile.Get(name)
   # print "name, integral is"+name+", "+str(hist.Integral())
    #if "effmap" not in name: hist.Multiply(r4effhist) #convert from bin/R4 eff to bin/total eff
    #print "post multiply: "+str(hist.Integral())    
    hist.SetTitle(title.replace("Fraction of R4 efficiency from ","Yield in R4 with ").replace("Efficiency","Yield"))
    hist.SetStats(0)
    hist.SetTitleOffset(1.2,"X")
    hist.SetTitleOffset(1.2,"Y")
    for xbin in range(1,hist.GetXaxis().GetNbins()+1):
        xsec = signalCrossSection(hist.GetXaxis().GetBinCenter(xbin))
        for ybin in range(1,hist.GetYaxis().GetNbins()+1):
            eff = hist.GetBinContent(xbin,ybin) #this is bin/R4 eff
            if "effmap" not in name: eff *= r4effhist.GetBinContent(xbin,ybin) #this becomes bin/total eff
           # print r4effhist.GetBinContent(xbin,ybin)
            #if eff > 0:
            if "effmap" in name: #else, it's already normalized to lumi
                binyield = eff*2100.*xsec
                if binyield>maxyield: maxyield=binyield
            else: binyield = eff
            hist.SetBinContent(xbin,ybin,binyield)
    
    if "effmap" not in name:
       #hist.SetMaximum(maxyield)
       #hist.SetMinimum(0.05)
       hist.GetZaxis().SetRangeUser(0.05,maxyield)
    else:
        hist.SetMinimum(0.1)   
    c = ROOT.TCanvas()
    c.SetLogz(True)
    hist.Draw("colz")
    tla = ROOT.TLatex()
    tla.SetTextSize(0.03)
    tla.DrawLatexNDC(0.77,0.92,"L = 2.1 fb^{-1} (13 TeV)")
    tla.DrawLatexNDC(0.1,0.92,"#font[62]{CMS} #scale[0.8]{#font[52]{Preliminary}}")
    
    outname =outfile+"yield_"+name.replace(" #leq","lte").replace(" #geq","gte").replace(" >","gt").replace(" =","").replace(", ","_").replace("H_{T}","ht").replace("n_{jets}","njets").replace("n_{lep}","nleps").replace("n_{b}","nbm").replace(" ","")
    c.Print("plots/maps/"+outname+".pdf")       

    

outputFile.Close()      



