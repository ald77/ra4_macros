#!/usr/bin/env python
"""This script plots bkg systematics for the RPV analysis"""
import sys
import math
import ROOT
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-i", "--input")

args = parser.parse_args()
if (args.input):
  infile = args.input
else:
  sys.exit("Please provide an input root file")
  

verbose = True
one_pdf = True #put all plots in one pdf file

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
#First, find new floating normalization, both for the variation up and down
#Then, using all processes, find the difference from nominal for up and down
#Return a histogram binned in Nb with the mean of the absolute value of relative errors up and down
def get_symmetrized_relative_errors(sysName,tot_data,total_nominal,procList,floating_process,sysFile,directory):


    # total hists for each variation, to include all processes
    systHistUp = ROOT.TH1F(directory+"_"+sysName+"_u","",5,0,5)
    systHistDown = ROOT.TH1F(directory+"_"+sysName+"_d","",5,0,5)

    #list of hists, one for each process
    histsUp = []
    histsDown = []
    totUp=tot_data 
    totDown=tot_data
    floating = -999.
    #load hists and calculate SFs for floating component for each variation
    for ip,proc in enumerate(procList):
        fixed = (ip != floating_process) # identify if this process has fixed or floating normalization

        up = get_hist_with_overflow(sysFile,directory + "/" + proc + "_" + sysName + "Up")
        down =  get_hist_with_overflow(sysFile,directory + "/" + proc + "_" + sysName + "Down")

        histsUp.append(up)
        histsDown.append(down)
        print "Up fixed "+str(fixed)+", yield "+str(up.Integral())
        print "Down fixed "+str(fixed)+", yield "+str(down.Integral())
        #subtract all FIXED components from total observed yield
        if fixed:
            totUp -= up.Integral()
            totDown -= down.Integral()

        # keep track of FLOATING yield
        else:
            floatingUp = up.Integral()
            floatingDown = down.Integral()        

    print "floatingDown = "+str(floatingDown)
    
    #scale factor for floating is just the leftover data, after fixed subtraction, divided by floating yield
    sfUp = totUp/floatingUp
    sfDown = totDown/floatingDown
    if tot_data ==0:
        sfUp= 1.
        sfDown=1.
    histsUp[floating_process].Scale(sfUp)
    histsDown[floating_process].Scale(sfDown)


    if verbose:
        print "variation UP SF, "+directory+", floating process "+str(floating_process)+", "+str(sfUp)
        print "variation DOWN SF, "+directory+", floating process "+str(floating_process)+", "+str(sfDown)
        runningtotalu=0
        runningtotald=0
        for ip,proc in enumerate(procList):
            print "Process "+proc+": "+str(histsNom[ip].Integral())
            runningtotalu+=histsUp[ip].Integral()
            runningtotald+=histsDown[ip].Integral()

        print "UP MC total: "+str(runningtotalu)
        print "DOWN MC total: "+str(runningtotald)
        print "Data: "+str(tot_data)



    #Get total histogram that includes all processes
    for ip,proc in enumerate(procList):
        systHistUp.Add(histsUp[ip])
        systHistDown.Add(histsDown[ip])

    #Subtract nominal histogram from varied histograms
    systHistUp.Add(total_nominal,-1)     
    systHistDown.Add(total_nominal,-1)

    for i in range(1,systHistUp.GetNbinsX()+1):
        #Find absolute value of deviation
        systHistUp.SetBinContent(i,abs(systHistUp.GetBinContent(i)))
        systHistDown.SetBinContent(i,abs(systHistDown.GetBinContent(i)))


    #Fill histUp with symmetrized error by adding histDown and dividing by 2, then divide by nominal to get relative symmetrized error    
    systHistUp.Add(systHistDown)
    systHistUp.Scale(0.5)
    systHistUp.Divide(total_nominal) # now systHistUp contains our relative errors
    return systHistUp





#Main body of code

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

#list of processes
procList=["qcd","ttbar","wjets","other"]

#make list of systematics- name, title, plot color and line style
#WARNING The phrases 'pdf' and 'mu' trigger special handling later on

systList=[]
## for i in range(0,100):
##     if i == 26 or i == 46: continue
##     systList.append(["w_pdf"+str(i),"PDF "+str(i),i,1])
systList.append(["btag_bc","b,c jet b-tag SF",2,1])
systList.append(["btag_udsg","Light flavor jet b-tag SF",3,1])
systList.append(["jes","Jet energy scale",4,1])
systList.append(["jer","Jet energy resolution",5,1])
systList.append(["w_pdf","PDF",46,1])
systList.append(["mur","Renormalization scale",6,1])
systList.append(["muf","Factorization scale",7,1])
systList.append(["murf","Renorm. and fact. scale",8,1])
#systList.append(["gs","Gluon splitting",9,1])
systList.append(["gs45","Gluon splitting (Njets=4,5)",9,1])
systList.append(["gs67","Gluon splitting (Njets=6,7)",10,1])
systList.append(["gs89","Gluon splitting (Njets=8,9)",11,1])
systList.append(["gs10Inf","Gluon splitting (Njets=10+)",12,1])

systList.append(["pileup","Pileup",13,1])
systList.append(["qcd_flavor","QCD flavor",14,1])
systList.append(["lep_eff","Lepton efficiency",15,1])
systList.append(["mc_stat","MC statistics",1,2])

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
binList.append(["bin16","8 #leq n_{jets} #leq 9","500 #leq M_{J} < 800 GeV","n_{lep} = 0"])
binList.append(["bin17","8 #leq n_{jets} #leq 9","M_{J} #geq 800 GeV","n_{lep} = 0"])


sysFile = ROOT.TFile(infile,"read")
for ibin in binList:
    directory = ibin[0]
    if verbose:
        print "directory is "+directory

    #get data observations for this bin in order to find normalization for floating process
    data = get_hist_with_overflow(sysFile,directory + "/data_obs")
    tot_data = data.Integral()
    nbinsX = data.GetNbinsX()

    histsNom = []
    tot = tot_data
    floating = -999.

    #Define process index to indicate whether QCD or ttbar is floating in this bin
    floating_process = 0
    if "n_{lep} = 1" in ibin[3]  :
        floating_process = 1


    #Make list of nominal hists for this bin (one entry per process) in order to calculate deviations later
    for ip,proc in enumerate(procList):
        fixed = (ip != floating_process)
            
        nominal = get_hist_with_overflow(sysFile,directory + "/" + proc)
        
        histsNom.append(nominal)
        if fixed:
            tot -= nominal.Integral() 
        else:
            floating = nominal.Integral()

    #nominal floating normalization scale factor
    sf = tot/floating
    if tot_data == 0:
        sf = 1.
    #Apply normalization
    histsNom[floating_process].Scale(sf)

    #Make master histogram of all nominal histograms (with floating normalization already corrected)
    total_nominal = ROOT.TH1F(directory+"_nominal","",5,0,5)
    total_nominal.Sumw2()
    for ip,proc in enumerate(procList):
        total_nominal.Add(histsNom[ip])
    
    if verbose:
        print "Nominal SF, "+directory+", floating process "+str(floating_process)+", "+str(sf)
        runningtotal=0
        for ip,proc in enumerate(procList):
            print "Process "+proc+": "+str(histsNom[ip].Integral())
            runningtotal+=histsNom[ip].Integral()

        print "MC total: "+str(runningtotal)
        print "Nominal hist total: " + str(total_nominal.Integral())
        print "Data: "+str(tot_data)
            
   
    #prepare cosmetics for plot, initialize table
    ROOT.gStyle.SetPadRightMargin()
    ROOT.gStyle.SetPadLeftMargin(0.12) #so it's not messed by larger table margin each iteration of the loop
    c = ROOT.TCanvas()
    leg = ROOT.TLegend(0.12,0.5,0.64,0.92)
    table = ROOT.TH2F("table_"+directory,"",nbinsX,-0.5,nbinsX-0.5,nSyst,0,nSyst)
    systHists_sym = []

    #loop over systs
    for isys, syst in enumerate(systList,start=1):
        sysName = syst[0]
        if verbose:
            print "starting "+sysName
        
        # this will eventually contain the errors to plot
        systHist = ROOT.TH1F(directory+"_"+sysName+"_sym","",5,0,5)
      
        #stat uncertainty treated separately 
        if "mc_stat" not in sysName:
            #pdf and mur,muf,murf treated separately
            if "pdf" not in sysName and "mu" not in sysName:
                #this function does everything
                systHist = get_symmetrized_relative_errors(sysName,tot_data,total_nominal,procList,floating_process,sysFile,directory)
            elif "mu" in sysName:
                #In this case, there is a separate variation in sum_rescaled.root specific to each process
                #We want to plot something to represent the total impact of varying mu for all the processes, so we just add them in quadrature
                for pr in procList:
                    #calculate errors for each process (i.e. qcd_mur, wjets_mur ...)
                    thisproc = get_symmetrized_relative_errors(pr+"_"+sysName,tot_data,total_nominal,procList,floating_process,sysFile,directory)

                    #square contents to add in quadrature
                    for i in range(1,thisproc.GetNbinsX()+1):
                        thisproc.SetBinContent(i,math.pow(thisproc.GetBinContent(i),2))

                    systHist.Add(thisproc) #add 

                #take square root again
                for i in range(1,systHist.GetNbinsX()+1):
                        systHist.SetBinContent(i,math.pow(systHist.GetBinContent(i),0.5))
                    
            elif "pdf" in sysName:
                #This systematic is calculated from 100 variations that all represent a single source of uncertainty.
                #We want to use information from all variations without artifically inflating the total uncertainty just by sampling the same effect many times.
                #Therefore, we find symmetrized uncertainties for each pdf variation up/down, add them in quadrature and divide by sqrt(100) to normalize
                for i in range(0,100):
                    #if i == 26 or i == 46: continue 
                    #Get errors for this pdf variation
                    thisvar = get_symmetrized_relative_errors("w_pdf"+str(i),tot_data,total_nominal,procList,floating_process,sysFile,directory)
                    #Add in quadrature to running total
                    for i in range(1,systHist.GetNbinsX()+1):
                        thisvar.SetBinContent(i,math.pow(thisvar.GetBinContent(i),2))
                    systHist.Add(thisvar)
                
                #take square root and normalize by sqrt(100)
                for i in range(1,systHist.GetNbinsX()+1):
                        systHist.SetBinContent(i,math.pow(systHist.GetBinContent(i),0.5)/10)
                           
            
        elif "mc_stat" in sysName:
            systHist.Add(total_nominal)
            for i in range(1,systHist.GetNbinsX()+1):
                #in this case, we want our relative errors to just be the MC errors
                if systHist.GetBinContent(i)>0:
                    systHist.SetBinContent(i,(systHist.GetBinError(i)/systHist.GetBinContent(i)))
                

       
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
        systHists_sym[isys-1].SetMaximum(1.)
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
    tla.DrawLatexNDC(0.17, 0.45, ibin[3])
    tla.DrawLatexNDC(0.17, 0.4, ibin[1])
    tla.DrawLatexNDC(0.17, 0.35, ibin[2])
    if one_pdf:
        if directory == binList[0][0]:
            outname = "plots/bkg_systs_all.pdf("
        elif directory == binList[len(binList)-1][0]:
            outname = "plots/bkg_systs_all.pdf)"
        else:
            outname = "plots/bkg_systs_all.pdf"

    else:
         outname = "plots/bkg_systs_" + directory +".pdf"
    print "outname is " +outname
    c.Print(outname)


    ROOT.gStyle.SetPadLeftMargin(0.25)
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
    tla.DrawLatexNDC(0.25,0.93,"#font[62]{CMS} #scale[0.8]{#font[52]{Simulation}}")
    tla.SetTextFont(42)
    tla.DrawLatexNDC(0.66,0.93,"#sqrt{s} = 13 TeV")
    if one_pdf:
        if directory == binList[0][0]:
            outname = "plots/table_bkg_systs_all.pdf("
        elif directory == binList[len(binList)-1][0]:
            outname = "plots/table_bkg_systs_all.pdf)"
        else:
            outname = "plots/table_bkg_systs_all.pdf"

    else:
         outname = "plots/table_bkg_systs_" + directory +".pdf"

         
    c2.Print(outname)
    
