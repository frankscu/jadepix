#!/usr/bin/env python

'''
plot cce graph
'''

__author__ = "YANG TAO <yangtao@ihep.ac.cn>"
__copyright__ = "Copyright (c) yangtao"
__created__ = "[2018-03-09 Fri 19:00]"
    
        
import cceanalysis  
import sys,os
import numpy as np
import ROOT

def plot():

    e = cceanalysis.CCEAnalysis()
    cce,rn = e.analyze('./output/data_with_electric_field_genapx.root','./conf/jadepix1_with_electric_field_main.conf')
    
    cce_canvas = ROOT.TCanvas('cce','cce',600,500)
    cce_canvas.SetGrid()       
    cce_graph = ROOT.TGraph(e.max_cluster_size,np.arange(1.,e.max_cluster_size+1.,1.),cce[1:])
    cce_graph.SetMarkerColor(38)
    cce_graph.SetMarkerStyle(20) 
    cce_graph.SetTitle('charge collection efficiency')
    cce_graph.Draw('ALP')
    cce_graph.GetXaxis().SetTitle('cluster')
    cce_graph.GetXaxis().CenterTitle()
    cce_graph.GetXaxis().SetLimits(0,5)
    cce_graph.GetXaxis().SetNdivisions(5)
    cce_graph.GetYaxis().SetTitle('cce')
    cce_graph.GetYaxis().SetRangeUser(0.,1.)
    cce_canvas.Update()
    cce_canvas.SaveAs('./fig/cce_graph.pdf')

    rn_canvas = ROOT.TCanvas('rn','rn',600,500)
    rn_canvas.SetGrid()      
    rn_graph = ROOT.TGraph(e.max_cluster_size,np.arange(1.,e.max_cluster_size+1.,1.),rn[1:]*1.0/rn[0])
    rn_graph.SetMarkerColor(38)
    rn_graph.SetMarkerStyle(20)
    rn_graph.SetTitle('cluster entries proportion')
    rn_graph.Draw('ALP')
    rn_graph.GetXaxis().SetTitle('cluster')
    rn_graph.GetXaxis().CenterTitle()
    rn_graph.GetXaxis().SetLimits(0,5)
    rn_graph.GetXaxis().SetLimits(0,5)
    rn_graph.GetXaxis().SetNdivisions(5)
    rn_graph.GetYaxis().SetTitle('entries proportion')
    rn_graph.GetYaxis().SetRangeUser(0.,1.)
    rn_canvas.Update()
    rn_canvas.SaveAs('./fig/rn_graph.pdf') 
    sys.exit()

if __name__ == '__main__':
    plot()
