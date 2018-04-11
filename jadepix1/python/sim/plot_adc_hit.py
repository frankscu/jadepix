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

def adc_hit_fill(t,entries,adc_hit_list,model):
    for ientry in xrange(entries):
        t.GetEntry(ientry)
        size = t.size

        if size:
            if model == 'isolate':
                adc = 0
                for ipos in xrange(size):
                    adc = t.signal.at(ipos)
                    adc_hit_list[0].Fill(adc)
                    adc_hit_list[size].Fill(adc)

            elif model == 'sum':
                adc = 0
                for ipos in xrange(size):
                    adc += t.signal.at(ipos)
                adc_hit_list[0].Fill(adc)
                adc_hit_list[size].Fill(adc)

def set_adc_hit_style(adc_hit_list,model):
    for ihit in adc_hit_list:
        ihit.GetXaxis().SetTitle('ADC')
        ihit.GetXaxis().CenterTitle()
        ihit.GetYaxis().SetTitle('Entries')
        ihit.GetYaxis().CenterTitle()
        ihit.SetFillStyle(3004)
        ihit.SetLineWidth(1)

        if model == 'sum':
            ihit.SetLineColor(46)
            ihit.SetFillColor(46)
        elif model == 'isolate':
            ihit.SetLineColor(38)
            ihit.SetFillColor(38)          

def plot(fname,model):
    try:
        f = ROOT.TFile(fname)
        t = f.Get('clusters')
        entries = t.GetEntries()
        logging.info('entries :'+str(entries))
    except:
        logging.error(fname+' is invalid!')
        sys.exit()
        
    max_cluster_size = 4

    adc_canvas = ROOT.TCanvas('adc','adc',200,10,500,300*(max_cluster_size+1))
    adc_canvas.Divide(1,max_cluster_size+1)
    adc_hit_list = []

    for icluster in xrange(max_cluster_size+1):
        if icluster == 0:
            adc_hit_list.append(ROOT.TH1F('total','total',100,0,5000))
        else:
            adc_hit_list.append(ROOT.TH1F('cluster {size}'.format(size=icluster),'cluster {size}'.format(size=icluster),100,0,5000))

    set_adc_hit_style(adc_hit_list,model)
    adc_hit_fill(t,entries,adc_hit_list,model)

    if not os.path.exists('./fig/'):
        os.makedirs('./fig/')

    for ipad in xrange(max_cluster_size+1):
        adc_canvas.cd(ipad+1)
        adc_hit_list[ipad].Draw()

    adc_canvas.Update()
    adc_canvas.SaveAs('./fig/adc_hit_%s.pdf'%model)

if __name__ == '__main__':
    plot('./output/data_genapx.root','sum')
    plot('./output/data_genapx.root','isolate')
