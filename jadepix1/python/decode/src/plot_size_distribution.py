#!/usr/bin/env python

'''
plot size distribution
'''

__author__ = "YANG TAO <yangtao@ihep.ac.cn>"
__copyright__ = "Copyright (c) yangtao"
__created__ = "[2018-03-05 Mon 19:00]"


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


def main(fname):
    try:
        file = ROOT.TFile(fname)
        size_tree = file.Get('Cluster_Tree')
        size_entries = size_tree.GetEntries()

    except:
        logging.error('input file is invalid!')
        sys.exit()

    canvas = ROOT.TCanvas('weak_iron55_canvas','size',200,10,2000,1200)
    #canvas = ROOT.TCanvas('strong_iron55_canvas','size',200,10,2000,1200)


    size_hist = ROOT.TH1F('weak_iron55 size distribution','Weak {}^{55}Fe  Size Distribution',5,1,6)
    size_hist.GetXaxis().SetTitle('Size')
    size_hist.GetXaxis().CenterTitle()
    size_hist.GetYaxis().SetTitle('Normalized Counts')
    size_hist.GetYaxis().CenterTitle()
    size_hist.SetLineColor(4)
    size_hist.SetLineWidth(2)

    # size_hist = ROOT.TH1F('strong_iron55 size distribution','Srong {}^{55}Fe  Size Distribution',5,1,6)
    # size_hist.GetXaxis().SetTitle('Size')
    # size_hist.GetXaxis().CenterTitle()
    # size_hist.GetYaxis().SetTitle('Normalized Counts')
    # size_hist.GetYaxis().CenterTitle()
    # size_hist.SetLineColor(4)
    # size_hist.SetLineWidth(2)


    for size_entry in xrange(size_entries):
        size_tree.GetEntry(size_entry)
        size_hist.Fill(size_tree.Size) 

    size_hist.Scale(1/size_hist.Integral())
    size_hist.GetYaxis().SetRangeUser(0,1)


    if not os.path.exists('../fig/'):
        os.makedirs('../fig/')

    canvas.cd()
    size_hist.Draw('hist')
    canvas.SaveAs('../fig/weak_iron55_size.gif')
    #canvas.SaveAs('../fig/strong_iron55_size.gif')

if __name__ == '__main__':
    main('../output/cluster55_weak_iron55_thr500.root')




