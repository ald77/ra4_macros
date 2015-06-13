#! /usr/bin/env python

import subprocess

for sig in ['T1tttt_1500_100','T1tttt_1200_800']:
    for method in range(1,4):
        for lumi in range(1,21):
            in_name = 'txt/data_card_method_'+str(method)+'_'+sig+'_lumi_'+str(lumi)+'.txt'
            out_name = 'txt/data_card_method_'+str(method)+'_'+sig+'_lumi_'+str(lumi)+'_systs.txt'
            mlsp = 100
            if sig == 'T1tttt_1200_800':
                mlsp = 800
            subprocess.call(['./run/make_card.exe','--lumi',str(lumi),'--method',str(method),'--mlsp',str(mlsp)])
            out_file = open(out_name,'w')
            
