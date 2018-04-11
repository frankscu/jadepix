#!/usr/bin/env python

'''
plot Sr90 clusters compare hist
'''

__author__ = "YANG TAO <yangtao@ihep.ac.cn>"
__copyright__ = "Copyright (c) yangtao"
__created__ = "[2018-04-10 Apr 12:00]"


import sys,os
import ROOT
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetStatX(0.9)
ROOT.gStyle.SetStatY(0.9)
ROOT.gStyle.SetStatW(0.08)
ROOT.gStyle.SetStatH(0.12)
import logging
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')
# console = logging.StreamHandler()
# console.setLevel(logging.DEBUG)
# console.setFormatter(logging.Formatter(' %(asctime)s - %(levelname)s- %(message)s'))
# logging.getLogger('').addHandler(console)


def main(cluster33_fname,cluster55_fname,cluster77_fname):
    try:
        cluster33_file = ROOT.TFile(cluster33_fname)
        cluster33_cluster_tree = cluster33_file.Get('Cluster_Tree')
        cluster33_cluster_entries = cluster33_cluster_tree.GetEntries()

        cluster55_file = ROOT.TFile(cluster55_fname)
        cluster55_cluster_tree = cluster55_file.Get('Cluster_Tree')
        cluster55_cluster_entries = cluster55_cluster_tree.GetEntries()

        cluster77_file = ROOT.TFile(cluster77_fname)
        cluster77_cluster_tree = cluster77_file.Get('Cluster_Tree')
        cluster77_cluster_entries = cluster77_cluster_tree.GetEntries()

    except:
        logging.error('input file is invalid!')
        sys.exit()

    cluster_canvas = ROOT.TCanvas('cluster_canvas','cluster_siganl',200,10,2400,1200)
    cluster_legend = ROOT.TLegend(0.75,0.7,0.85,0.85)


    cluster33_cluster_hist = ROOT.TH1F('cluster33 cluster signal','{}^{90}Sr  Cluster Signal',200,0,1400)
    cluster33_cluster_hist.GetXaxis().SetTitle('ADC')
    cluster33_cluster_hist.GetXaxis().CenterTitle()
    cluster33_cluster_hist.GetYaxis().SetTitle('Normalized Counts')
    cluster33_cluster_hist.GetYaxis().CenterTitle()
    cluster33_cluster_hist.SetLineColor(38)
    cluster33_cluster_hist.SetLineWidth(2)


    cluster55_cluster_hist = ROOT.TH1F('cluster55 cluster signal','{}^{90}Sr  Cluster Signal',200,0,1400)
    cluster55_cluster_hist.GetXaxis().SetTitle('ADC')
    cluster55_cluster_hist.GetXaxis().CenterTitle()
    cluster55_cluster_hist.GetYaxis().SetTitle('Normalized Counts')
    cluster55_cluster_hist.GetYaxis().CenterTitle()
    cluster55_cluster_hist.SetLineColor(46)
    cluster55_cluster_hist.SetLineWidth(2)

    cluster77_cluster_hist = ROOT.TH1F('cluster77 cluster signal','{}^{90}Sr  Cluster Signal',200,0,1400)
    cluster77_cluster_hist.GetXaxis().SetTitle('ADC')
    cluster77_cluster_hist.GetXaxis().CenterTitle()
    cluster77_cluster_hist.GetYaxis().SetTitle('Normalized Counts')
    cluster77_cluster_hist.GetYaxis().CenterTitle()
    cluster77_cluster_hist.SetLineColor(30)
    cluster77_cluster_hist.SetLineWidth(2)


    for cluster33_cluster_entry in xrange(cluster33_cluster_entries):
        cluster33_cluster_tree.GetEntry(cluster33_cluster_entry)
        cluster33_cluster_hist.Fill(cluster33_cluster_tree.ClusterSignal) 
    cluster33_cluster_hist.Scale(1/cluster33_cluster_hist.Integral())

    for cluster55_cluster_entry in xrange(cluster55_cluster_entries):
        cluster55_cluster_tree.GetEntry(cluster55_cluster_entry)
        cluster55_cluster_hist.Fill(cluster55_cluster_tree.ClusterSignal) 
    cluster55_cluster_hist.Scale(1/cluster55_cluster_hist.Integral())


    for cluster77_cluster_entry in xrange(cluster77_cluster_entries):
        cluster77_cluster_tree.GetEntry(cluster77_cluster_entry)
        cluster77_cluster_hist.Fill(cluster77_cluster_tree.ClusterSignal) 
    cluster77_cluster_hist.Scale(1/cluster77_cluster_hist.Integral())

    cluster_legend.AddEntry(cluster33_cluster_hist,'3 #times 3')
    cluster_legend.AddEntry(cluster55_cluster_hist,'5 #times 5') 
    cluster_legend.AddEntry(cluster77_cluster_hist,'7 #times 7')  

    if not os.path.exists('../fig/'):
        os.makedirs('../fig/')


    cluster_canvas.cd()
    cluster33_cluster_hist.Draw('hist')
    cluster55_cluster_hist.Draw('hist same')
    cluster77_cluster_hist.Draw('hist same')
    cluster_legend.Draw()
    cluster_canvas.Update()
    cluster_canvas.SaveAs('../fig/sr90_cluster_compare.gif')


if __name__ == '__main__':
    
    main('../output/cluster33_Sr90_thr10.root ','../output//cluster55_Sr90_thr10.root','../output/cluster77_Sr90_thr10.root')




