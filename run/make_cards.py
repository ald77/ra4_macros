#! /usr/bin/env python

import subprocess

for sig in ['T1tttt_1500_100','T1tttt_1200_800']:
    for method in range(1,4):
        for lumi in range(1,21):
            mlsp = 100
            if sig == 'T1tttt_1200_800':
                mlsp = 800
            subprocess.Popen(['./run/make_card.exe','--lumi',str(lumi),'--method',str(method),'--mlsp',str(mlsp)])
            subprocess.Popen(['./run/make_card.exe','--lumi',str(lumi),'--method',str(method),'--mlsp',str(mlsp),'--no_systematics'])
            subprocess.Popen(['./run/make_card.exe','--lumi',str(lumi),'--method',str(method),'--mlsp',str(mlsp),'--mc_multiplier',str(10.)])
