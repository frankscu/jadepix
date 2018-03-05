#!/usr/bin/env python

'''
Draw jadepix sim data
'''

__author__ = "YANG TAO <yangtao@ihep.ac.cn>"
__copyright__ = "Copyright (c) yangtao"
__created__ = "[2018-03-04 Tue 23:00]"


import sys
import ROOT
ROOT.gStyle.SetOptFit()
import logging
logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s')
# console = logging.StreamHandler()
# console.setLevel(logging.DEBUG)
# console.setFormatter(logging.Formatter(' %(asctime)s - %(levelname)s- %(message)s'))
# logging.getLogger('').addHandler(console)

def hit_fill(t,entries,adc_hit,adc_1_hit,adc_2_hit,adc_3_hit,size_hit):

    maxsize = 0

    for ientry in xrange(entries):
        t.GetEntry(ientry)
        size = t.size
        if size > maxsize:
            maxsize = size

        size_hit.Fill(size)

        for index in xrange(size):
        
            adc = t.signal.at(index)
            adc_hit.Fill(adc)
            if size == 1:
                adc_1_hit.Fill(adc)
            if size == 2:
                adc_2_hit.Fill(adc)
            if size == 3:
                adc_3_hit.Fill(adc)
    logging.info('maxsize:'+str(maxsize))


def adc_hit_fit(adc_1_hit,adc_2_hit,adc_3_hit):
    #
    adc_1_fitf = ROOT.TF1('adc_1_fitf','gaus')
    adc_1_fitf.SetLineColor(4)
    adc_1_fitf.SetLineWidth(1)
    adc_1_hit.Fit(adc_1_fitf)

    # adc_2_fitf = ROOT.TF1('adc_2_fitf','gaus')
    # adc_2_fitf.SetLineColor(2)
    # adc_2_fitf.SetLineWidth(1)
    # adc_2_hit.Fit(adc_2_fitf)
    #

def set_adc_hit_style(legend,adc_hit,adc_1_hit,adc_2_hit,adc_3_hit):

    adc_hit.GetXaxis().SetTitle('ADC')
    adc_hit.GetXaxis().CenterTitle()
    adc_hit.GetYaxis().SetTitle('Entries')
    adc_hit.GetYaxis().CenterTitle()

    adc_hit.SetFillStyle(4050)
    adc_hit.SetLineColor(16)
    adc_hit.SetFillColor(16)
    legend.AddEntry(adc_hit,'adc')

    adc_1_hit.SetFillStyle(4050)
    adc_1_hit.SetLineColor(38)
    adc_1_hit.SetFillColor(38)
    legend.AddEntry(adc_1_hit,'cluster1')

    adc_2_hit.SetFillStyle(4050)
    adc_2_hit.SetLineColor(46)
    adc_2_hit.SetFillColor(46)
    legend.AddEntry(adc_2_hit,'cluster2')

    adc_3_hit.SetFillStyle(4050)
    adc_3_hit.SetLineColor(30)
    adc_3_hit.SetFillColor(30)
    legend.AddEntry(adc_3_hit,'cluster3')


def get_stats(adc_canvas,adc_hit,adc_1_hit,adc_2_hit,adc_3_hit,size_hit):

    adc_canvas.cd()
    adc_hit.Draw()
    adc_canvas.Update()
    stats0 = adc_hit.GetListOfFunctions().FindObject('stats')

    adc_1_hit.Draw()
    adc_canvas.Update()
    stats1 = adc_1_hit.GetListOfFunctions().FindObject('stats')

    adc_2_hit.Draw()
    adc_canvas.Update()
    stats2 = adc_2_hit.GetListOfFunctions().FindObject('stats')

    adc_3_hit.Draw()
    adc_canvas.Update()
    stats3 = adc_3_hit.GetListOfFunctions().FindObject('stats')

    return stats0,stats1,stats2,stats3


def move_stats(stats0,stats1,stats2,stats3):

    stats0.SetX1NDC(0.8)
    stats0.SetX2NDC(0.9)
    stats0.SetY1NDC(0.8)
    stats0.SetY2NDC(0.9)

    stats1.SetX1NDC(0.8)
    stats1.SetX2NDC(0.9)
    stats1.SetY1NDC(0.6)
    stats1.SetY2NDC(0.8)

    stats2.SetX1NDC(0.8)
    stats2.SetX2NDC(0.9)
    stats2.SetY1NDC(0.5)
    stats2.SetY2NDC(0.6)

    stats3.SetX1NDC(0.8)
    stats3.SetX2NDC(0.9)
    stats3.SetY1NDC(0.4)
    stats3.SetY2NDC(0.5)


def cce_analysis():

    return 0


def main():
    try:
        fname = './output/data_genapx.root'
        f = ROOT.TFile(fname)
        t = f.Get('clusters')
        entries = t.GetEntries()
        logging.info('entries :'+str(entries))

    except:
        logging.error(fname+' is invalid!')
        sys.exit()

    size_canvas = ROOT.TCanvas('size','size',200,10,500,300)
    adc_canvas = ROOT.TCanvas('adc','adc',200,10,500,300)
    legend = ROOT.TLegend(0.2,0.7,0.3,0.9)


    size_hit = ROOT.TH1F('size_hit','size',5,0,5)
    adc_hit = ROOT.TH1F('adc','adc',100,0,5000)
    adc_1_hit = ROOT.TH1F('cluster1','cluster1',100,0,5000)
    adc_2_hit = ROOT.TH1F('cluster2','cluster2',100,0,5000)
    adc_3_hit = ROOT.TH1F('cluster3','cluster3',100,0,5000)

    set_adc_hit_style(legend,adc_hit,adc_1_hit,adc_2_hit,adc_3_hit)

    hit_fill(t,entries,adc_hit,adc_1_hit,adc_2_hit,adc_3_hit,size_hit)

    adc_hit_fit(adc_1_hit,adc_2_hit,adc_3_hit)

    stats0,stats1,stats2,stats3 = get_stats(adc_canvas,adc_hit,adc_1_hit,adc_2_hit,adc_3_hit,size_hit)
    move_stats(stats0,stats1,stats2,stats3)

    adc_hit.Draw()
    adc_1_hit.Draw('same')
    adc_2_hit.Draw('same')
    adc_3_hit.Draw('same')
    legend.Draw()
    #stats0.Draw()
    #stats1.Draw()
    #stats2.Draw()
    #stats3.Draw()
    adc_canvas.Update()
    adc_canvas.SaveAs('./fig/adc_hit.pdf')

    size_canvas.cd()
    size_hit.Draw()
    size_canvas.Update()
    size_canvas.SaveAs('./fig/size_hit.pdf')

if __name__ == '__main__':
    main()


