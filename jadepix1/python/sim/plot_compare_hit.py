#!/usr/bin/env python

'''
cce compare
'''

__author__ = "YANG TAO <yangtao@ihep.ac.cn>"
__copyright__ = "Copyright (c) yangtao"
__created__ = "[2018-03-05 Mon 19:00]"


import sys,os
import ROOT
ROOT.gStyle.SetOptFit()
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

def adc_hit_fill(max_cluster_size,t1,t2,entries1,entries2,adc_hit_list1,adc_hit_list2):
    for ientry in xrange(entries1):
        t1.GetEntry(ientry)
        size1 = t1.size
        for icluster in xrange(max_cluster_size):
            if size1 == (icluster+1):
                adc1 = 0
                for i in xrange(size1):
                    adc1 = adc1+t1.signal.at(i)
                adc_hit_list1[icluster].Fill(adc1)

    for jentry in xrange(entries2):
        t2.GetEntry(jentry)
        size2 = t2.size
        for jcluster in xrange(max_cluster_size):
            if size2 == (jcluster+1):
                adc2 = 0
                for j in xrange(size2):
                    adc2 = adc2+t2.signal.at(j)
                adc_hit_list2[jcluster].Fill(adc2)    

    

def set_adc_hit_style(max_cluster_size,adc_hit_list1,adc_hit_list2):
    for icluster in xrange(max_cluster_size):
        adc_hit_list1[icluster].GetXaxis().SetTitle('ADC')
        adc_hit_list1[icluster].GetXaxis().CenterTitle()
        adc_hit_list1[icluster].GetYaxis().SetTitle('Entries')
        adc_hit_list1[icluster].GetYaxis().CenterTitle()
        adc_hit_list1[icluster].SetFillStyle(3004)
        adc_hit_list1[icluster].SetLineColor(46)
        adc_hit_list1[icluster].SetFillColor(46)

        adc_hit_list2[icluster].GetXaxis().SetTitle('ADC')
        adc_hit_list2[icluster].GetXaxis().CenterTitle()
        adc_hit_list2[icluster].GetYaxis().SetTitle('Entries')
        adc_hit_list2[icluster].GetYaxis().CenterTitle()
        adc_hit_list2[icluster].SetFillStyle(3004)
        adc_hit_list2[icluster].SetLineColor(38)
        adc_hit_list2[icluster].SetFillColor(38)


def main(fname1,fname2):
    try:
        f1 = ROOT.TFile(fname1)
        t1 = f1.Get('clusters')
        entries1 = t1.GetEntries()
        logging.info('f1 entries :'+str(entries1))

        f2 = ROOT.TFile(fname2)
        t2 = f2.Get('clusters')
        entries2 = t2.GetEntries()
        logging.info('f2 entries :'+str(entries2))

    except:
        logging.error('input file is invalid!')
        sys.exit()

    max_cluster_size = 4

    adc_canvas = ROOT.TCanvas('adc','adc',200,10,1000,300*max_cluster_size)
    adc_canvas.Divide(2,max_cluster_size)
    adc_hit_list1 = []
    adc_hit_list2 = []

    for isize in xrange(max_cluster_size):
        adc_hit_list1.append(ROOT.TH1F('#cluster {size}'.format(size=isize+1),'with electricfield cluster {size}'.format(size=isize+1),100,0,5000))
        adc_hit_list2.append(ROOT.TH1F('*cluster {size}'.format(size=isize+1),'without electricfield cluster {size}'.format(size=isize+1),100,0,5000))

    set_adc_hit_style(max_cluster_size,adc_hit_list1,adc_hit_list2)

    adc_hit_fill(max_cluster_size,t1,t2,entries1,entries2,adc_hit_list1,adc_hit_list2)

    if not os.path.exists('./fig/'):
        os.makedirs('./fig/')

    for ipad in xrange(max_cluster_size):
        adc_canvas.cd(2*ipad+1)
        adc_hit_list1[ipad].Draw()
        adc_canvas.cd(2*ipad+2)
        adc_hit_list2[ipad].Draw()

    adc_canvas.Update()
    adc_canvas.SaveAs('./fig/adc_compare_hit.pdf')

if __name__ == '__main__':
    main('./output/data_with_electric_field_genapx.root','./output/data_without_electric_field_genapx.root')
