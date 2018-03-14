#!/usr/bin/env python

'''
plot seed and cluster scat
'''

__author__ = "YANG TAO <yangtao@ihep.ac.cn>"
__copyright__ = "Copyright (c) yangtao"
__created__ = "[2018-03-09 Fri 19:00]"
       
import cceanalysis  
import sys,os
import numpy as np
import ROOT
import logging
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')

MAX_SIZE = 5

def main(fname):
    try:
        f = ROOT.TFile(fname)
        t = f.Get('clusters')
        entries = t.GetEntries()
        logging.info('file entries :'+str(entries))

    except:
        logging.error('input file is invalid!')
        sys.exit()

    canvas = ROOT.TCanvas('canvas','',200,10,1200,1000)
    legend = ROOT.TLegend(0.15,0.8,0.35,0.9)
    legend.SetTextSize(0.03)

    seed_scat = ROOT.TH2F('signal_distribution','signal distribution',100,0,MAX_SIZE,100,0,5000)
    seed_scat.GetXaxis().SetTitle('cluster')
    seed_scat.GetXaxis().CenterTitle()
    seed_scat.GetYaxis().SetTitle('ADC')
    seed_scat.GetYaxis().CenterTitle()
    seed_scat.SetMarkerColor(46)
    seed_scat.SetLineColor(0)
    seed_scat.SetMarkerStyle(20)
    seed_scat.SetMarkerSize(0.4)
    legend.AddEntry(seed_scat,'seed signal')

    cluster_scat = ROOT.TH2F('cluster_scat','cluster_scat',100,0,MAX_SIZE,100,0,5000)
    cluster_scat.SetMarkerColor(38)
    cluster_scat.SetLineColor(0)
    cluster_scat.SetMarkerStyle(29)
    cluster_scat.SetMarkerSize(0.5)
    legend.AddEntry(cluster_scat,'cluster signal')


    for ientry in xrange(entries):
        t.GetEntry(ientry)
        size = t.size
        seed_adc = 0
        cluster_adc = 0

        for ipos in xrange(size):
            if t.signal.at(ipos) > seed_adc:
                seed_adc = t.signal.at(ipos)
            cluster_adc += t.signal.at(ipos)

        if seed_adc > 0:
            cluster_scat.Fill(size+0.05,cluster_adc)
            seed_scat.Fill(size-0.05,seed_adc)

    canvas.cd()
    seed_scat.Draw()
    cluster_scat.Draw('same')
    legend.Draw()
    canvas.Update()
    canvas.SaveAs('./fig/seed_scat.pdf')

if __name__ == '__main__':
    main('./output/data_with_electric_field_genapx.root')
