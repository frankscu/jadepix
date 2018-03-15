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

    e = cceanalysis.cceanalysis()
    cce1,rn1 = e.analyze('./output/data_with_electric_field_genapx.root','./conf/jadepix1_with_electric_field_main.conf')
    cce2,rn2 = e.analyze('./output/data_without_electric_field_genapx.root','./conf/jadepix1_without_electric_field_main.conf')

    cce_canvas = ROOT.TCanvas('cce','cce',600,500)
    cce_canvas.SetGrid()
    cce_multigraph = ROOT.TMultiGraph()
    cce_legend = ROOT.TLegend(0.2,0.5,0.4,0.7)
        
    cce_graph_1 = ROOT.TGraph(e.max_cluster_size,np.arange(1.,e.max_cluster_size+1.,1.),cce1[1:])
    cce_graph_1.SetMarkerColor(46)
    cce_graph_1.SetMarkerStyle(20)
    cce_multigraph.Add(cce_graph_1)
    cce_legend.AddEntry(cce_graph_1,'with electric field')

    cce_graph_2 = ROOT.TGraph(e.max_cluster_size,np.arange(1.,e.max_cluster_size+1.,1.),cce2[1:])
    cce_graph_2.SetMarkerColor(38)
    cce_graph_2.SetMarkerStyle(21)
    cce_multigraph.Add(cce_graph_2)
    cce_legend.AddEntry(cce_graph_2,'without electric field')

    cce_multigraph.SetTitle('charge collection efficiency')
    cce_multigraph.Draw('ALP')
    cce_multigraph.GetXaxis().SetTitle('cluster')
    cce_multigraph.GetXaxis().CenterTitle()
    cce_multigraph.GetYaxis().SetTitle('cce')
    cce_legend.Draw()
    cce_canvas.Update()
    cce_canvas.SaveAs('./fig/cce_compare_graph.pdf')

    rn_canvas = ROOT.TCanvas('rn','rn',600,500)
    rn_canvas.SetGrid()
    rn_multigraph = ROOT.TMultiGraph()
    rn_legend = ROOT.TLegend(0.7,0.7,0.9,0.9)
        
    rn_graph_1 = ROOT.TGraph(e.max_cluster_size,np.arange(1.,e.max_cluster_size+1.,1.),rn1[1:]*1.0/rn1[0])
    rn_graph_1.SetMarkerColor(46)
    rn_graph_1.SetMarkerStyle(20)
    rn_multigraph.Add(rn_graph_1)
    rn_legend.AddEntry(rn_graph_1,'with electric field')

    rn_graph_2 = ROOT.TGraph(e.max_cluster_size,np.arange(1.,e.max_cluster_size+1.,1.),rn2[1:]*1.0/rn2[0])
    rn_graph_2.SetMarkerColor(38)
    rn_graph_2.SetMarkerStyle(21)
    rn_multigraph.Add(rn_graph_2)
    rn_legend.AddEntry(rn_graph_2,'without electric field')

    rn_multigraph.SetTitle('cluster entries proportion')
    rn_multigraph.Draw('ALP')
    rn_multigraph.GetXaxis().SetTitle('cluster')
    rn_multigraph.GetXaxis().CenterTitle()
    rn_multigraph.GetYaxis().SetTitle('entries proportion')
    rn_legend.Draw()
    rn_canvas.Update()
    rn_canvas.SaveAs('./fig/rn_compare_graph.pdf') 
    sys.exit()

if __name__ == '__main__':
    plot()
