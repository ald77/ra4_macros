#! /usr/bin/env python

import optparse
from math import sqrt

def isfloat(value):
  try:
    float(value)
    return True
  except ValueError:
    return False

def resize(na, nb, init):
    x = []
    for i in range(0, na):
        x.append([init]*nb)
    return x

def GetBinNames(file, lines, num_bins):
    bin_name = [None]*num_bins
    method_loc = file.name.find("_method_")
    if method_loc == -1 or (method_loc+8)>=len(file.name) or not file.name[method_loc+8].isdigit():
        for bin in range(0, num_bins):
            bin_name[bin] = str(bin)
    else:
        method = int(file.name[method_loc+8])
        if (method == 0 or method == 1 or method == 2) and num_bins == 4:
            bin_name[0] = "low $E_T^{\\text{miss}}$, low $N_j$"
            bin_name[1] = "low $E_T^{\\text{miss}}$, high $N_j$"
            bin_name[2] = "high $E_T^{\\text{miss}}$, low $N_j$"
            bin_name[3] = "high $E_T^{\\text{miss}}$, high $N_j$"
        elif method == 3 and num_bins == 6:
            bin_name[0] = "low $E_T^{\\text{miss}}$, low $N_j$, low $N_b$"
            bin_name[1] = "low $E_T^{\\text{miss}}$, low $N_j$, high $N_b$"
            bin_name[2] = "low $E_T^{\\text{miss}}$, high $N_j$, low $N_b$"
            bin_name[3] = "low $E_T^{\\text{miss}}$, high $N_j$, high $N_b$"
            bin_name[4] = "high $E_T^{\\text{miss}}$, low $N_j$"
            bin_name[5] = "high $E_T^{\\text{miss}}$, high $N_j$"
        else:
            for bin in range(0, num_bins):
                bin_name[bin] = str(bin)

    return bin_name

def GetProcessNames(file, lines, num_procs):
    proc_names = [None]*num_procs
    done = False

    sig_name = "Signal"
    if file.name.find("T1tttt") != -1:
        if file.name.find("T1tttt_1500_100") != -1:
            sig_name = "T1tttt(1500,100)"
        elif file.name.find("T1tttt_1200_800") != -1:
            sig_name = "T1tttt(1200,800)"
        else:
            sig_name = "T1tttt"
    
    for line in lines:
        words = line.split()
        if words[0] == "process":
            for proc in range(0, num_procs):
                proc_names[proc] = words[proc+1]
                if proc_names[proc] == "sig":
                    proc_names[proc] = sig_name
                elif proc_names[proc] == "ttbar":
                    name = "$t\\overline{t}$"

    return proc_names

def GetLnn(lines, num_procs, num_bins):
    lnn_match = [0.]*num_bins
    lnn_diff = resize(num_procs, num_bins, 0.)
    lnn_mc = [0.]*num_bins
    for line in lines:
        words = line.split()
        if len(words)>=2 and words[1] == "lnN":
            for bin in range(0, num_bins):
                if words[0].find("mc___") != -1:
                    val = words[num_procs*bin+3]
                    if isfloat(val):
                        lnn_mc[bin] += (float(val)-1.)**2
                else:
                    val = words[num_procs*bin+3]
                    if isfloat(val):
                        all_matched = True
                        for proc in range(1, num_procs):
                            this_val = words[num_procs*bin+2+proc]
                            if this_val != val:
                                all_matched = False
                        if all_matched:
                            lnn_match[bin] += (float(val)-1.)**2
                        else:
                            for proc in range(1, num_procs):
                                this_val = words[num_procs*bin+2+proc]
                                if isfloat(this_val):
                                    lnn_diff[proc][bin] += (float(this_val)-1.)**2

    for bin in range(0, num_bins):
        lnn_mc[bin] = sqrt(lnn_mc[bin])
        lnn_match[bin] = sqrt(lnn_match[bin])
        for proc in range(0, num_procs):
            lnn_diff[proc][bin] = sqrt(lnn_diff[proc][bin])

    return (lnn_match, lnn_diff, lnn_mc)
            
def GetGamma(lines, num_procs, num_bins):
    gmn_raw = resize(num_procs, num_bins, None)
    gmn_wght = resize(num_procs, num_bins, None)
    for line in lines:
        words = line.split()
        if len(words)>=3 and words[1]=="gmN" and isfloat(words[2]):
            for proc in range(0, num_procs):
                for bin in range(0, num_bins):
                    i = num_procs*bin+proc+3
                    if isfloat(words[i]):
                        gmn_raw[proc][bin] = float(words[2])
                        gmn_wght[proc][bin] = float(words[i])
    return (gmn_raw, gmn_wght)

def GetBkgGamma(gmn_raw, gmn_wght, num_procs, num_bins):
    count = [0.]*num_bins
    ss = [0.]*num_bins

    out_raw = [0.]*num_bins
    out_wght = [0.]*num_bins

    for bin in range(0, num_bins):
        for proc in range(1, num_procs):
            count[bin] += gmn_raw[proc][bin]*gmn_wght[proc][bin]
            ss[bin] += gmn_raw[proc][bin]*(gmn_wght[proc][bin]**2)
        out_raw[bin] = (count[bin]**2)/ss[bin]
        out_wght[bin] = ss[bin]/count[bin]

    return (out_raw, out_wght)
                        
def GetData(lines, num_bins):
    data = [None]*num_bins
    for line in lines:
        if line.find("observation") != -1:
            words = line.split()
            for bin in range(0,num_bins):
                data[bin] = float(words[bin+1])
    return data

def GetCounts(gmn_raw, gmn_wght, num_procs, num_bins):
    counts = resize(num_procs, num_bins, 0.)
    for proc in range(0, num_procs):
        for bin in range(0, num_bins):
            counts[proc][bin] = gmn_raw[proc][bin]*gmn_wght[proc][bin]

    return counts

def GetMeans(gmn_raw, gmn_wght, num_procs, num_bins):
    counts = resize(num_procs, num_bins, 0.)
    for proc in range(0, num_procs):
        for bin in range(0, num_bins):
            counts[proc][bin] = (gmn_raw[proc][bin]+1.)*gmn_wght[proc][bin]

    return counts

def GetCountFracUncert(gmn_raw, lnn_match, lnn_diff, lnn_mc, num_procs, num_bins):
    uncerts = resize(num_procs, num_bins, 0.)
    for proc in range(0, num_procs):
        for bin in range(0, num_bins):
            uncerts[proc][bin] = 1./(gmn_raw[proc][bin]+1.)
            uncerts[proc][bin] += lnn_match[bin]**2
            uncerts[proc][bin] += lnn_diff[proc][bin]**2
            uncerts[proc][bin] += lnn_mc[bin]**2
            uncerts[proc][bin] = sqrt(uncerts[proc][bin])
            
    return uncerts

def GetKappa(lines, num_procs, num_bins):
    kappa = [None]*num_bins
    for line in lines:
        words = line.split()
        if len(words) >= num_bins*num_procs+1 and words[0] == "#kappa":
            for bin in range(0, num_bins):
                kappa[bin] = float(words[num_procs*bin+2])
    return kappa

def GetKappaFracUncert(bkg_raw, lnn_mc, num_procs, num_bins):
    out = [0.]*num_bins
    if num_procs == 1:
        return out
    
    for bin in range(0, num_bins):
        out[bin] = sqrt(1./(bkg_raw[bin]+1.) + lnn_mc[bin])
    return out

def GetPrediction(bkg_raw, bkg_wght, num_bins):
    preds = [0.]*num_bins
    for bin in range(0, num_bins):
        preds[bin] = bkg_raw[bin]*bkg_wght[bin]
    return preds

def GetPredictionMean(bkg_raw, bkg_wght, num_bins):
    preds = [0.]*num_bins
    for bin in range(0, num_bins):
        preds[bin] = (bkg_raw[bin]+1.)*bkg_wght[bin]
    return preds

def GetPredictionFracUncert(gmn_raw, gmn_wght, lnn_match, lnn_diff, lnn_mc, num_procs, num_bins):
    uncerts = [0.]*num_bins
    for bin in range(0, num_bins):
        uncerts[bin] += lnn_match[bin]**2 + lnn_mc[bin]**2
        num = 0.
        count = 0.
        ss = 0.
        for proc in range(1, num_procs):
            this_count = gmn_raw[proc][bin]*gmn_wght[proc][bin]
            this_mean = (gmn_raw[proc][bin]+1.)*gmn_wght[proc][bin]
            this_ss = gmn_raw[proc][bin]*(gmn_wght[proc][bin]**2)
            this_err = (gmn_raw[proc][bin]+1.)*(gmn_wght[proc][bin]**2)
            count += this_count
            ss += this_ss
            num += this_err+(this_mean*lnn_diff[proc][bin])**2
        den_raw = count**2/ss
        den_wght = ss/count
        
        uncerts[bin]+= num/(((den_raw+1.)*den_wght)**2)
        uncerts[bin] = sqrt(uncerts[bin])
    return uncerts
    
parser = optparse.OptionParser(
    description="Convert data card to human readable format",
    )

parser.add_option("-f","--file",
                  default = "txt/data_card_2015_05_25_method_0_with_mc_kappa_T1tttt_1500_100_lumi_10_no_tk_veto_ht_500_mt_0_140_mj_0_400_njets_7_9_met_200_400_nb_2_3.txt",
                  dest = "file",
                  help = "Data card file"
                  )

(options, args) = parser.parse_args()

in_file_name = options.file
file = open(in_file_name, 'r')
lines = [line.rstrip('\n') for line in file]

num_bins = 0
num_procs = 1
for line in lines:
    if line.find('imax') != -1:
        for word in line.split():
            if word.isdigit():
                num_bins = int(word)
    elif line.find('jmax') != -1:
        for word in line.split():
            if word.isdigit():
                num_procs = int(word)+1

bin_names = GetBinNames(file, lines, num_bins)
proc_names = GetProcessNames(file, lines, num_procs)

(lnn_match, lnn_diff, lnn_mc) = GetLnn(lines, num_procs, num_bins)
(gmn_raw, gmn_wght) = GetGamma(lines, num_procs, num_bins)
(bkg_raw, bkg_wght) = GetBkgGamma(gmn_raw, gmn_wght, num_procs, num_bins)

data = GetData(lines, num_bins)
count = GetCounts(gmn_raw, gmn_wght, num_procs, num_bins)
count_mean = GetMeans(gmn_raw, gmn_wght, num_procs, num_bins)
count_unc = GetCountFracUncert(gmn_raw, lnn_match, lnn_diff, lnn_mc, num_procs, num_bins)
kappa = GetKappa(lines, num_procs, num_bins)
kappa_unc = GetKappaFracUncert(bkg_raw, lnn_mc, num_procs, num_bins)
pred = GetPrediction(bkg_raw, bkg_wght, num_bins)
pred_mean = GetPredictionMean(bkg_raw, bkg_wght, num_bins)
pred_unc = GetPredictionFracUncert(gmn_raw, gmn_wght, lnn_match, lnn_diff, lnn_mc, num_procs, num_bins)

out_file_name = in_file_name + "_table.tex"
f = open(out_file_name, 'w')
f.write("\\documentclass{article}\n")
f.write("\\begin{document}\n")
f.write("\\begin{table}\n")
f.write("\\centering\n")
f.write("\\begin{tabular}{rrrrrrr}\n")
f.write("\\hline\\hline\n")
f.write("Bin & Data & Prediction & $\\kappa$ & $t\\overline{t}$ & Other & Signal\\\\\n")
f.write("\\hline\n")

for bin in range(0, num_bins):
    f.write(bin_names[bin] + " & ")
    f.write("%.2f"%data[bin] + " & ")
    f.write("$%.2f"%pred[bin] + "\\pm%.2f"%(pred_unc[bin]*pred_mean[bin]) + "$ & ")
    f.write("$%.2f"%kappa[bin] + "\\pm%.2f"%(kappa_unc[bin]*kappa[bin]) + "$ & ")
    for proc in range(1, num_procs):
        f.write("$%.2f"%count[proc][bin] + "\\pm%.2f"%(count_unc[proc][bin]*count_mean[proc][bin]) + "$ & ")
    f.write("$%.2f"%count[0][bin] + "\\pm%.2f"%(count_unc[0][bin]*count_mean[0][bin]) + "$ \\\\\n")

f.write("\\hline\\hline\n")
f.write("\\end{tabular}\n")
f.write("\\end{table}\n")
f.write("\\end{document}\n")
