#!/usr/bin/env python

'''
cce alalysis module
'''

__author__ = "YANG TAO <yangtao@ihep.ac.cn>"
__copyright__ = "Copyright (c) yangtao"
__created__ = "[2018-03-09 Fri 19:00]"

import sys,os,re
import numpy as np
import ROOT
import logging

class cceanalysis():

    def __init__(self):
        self.emit_particle_energy =5.89 #keV
        self.emit_particle_number = 10000
        self.mean_ionization_energy = 3.6 #ev
        self.gain = 8
        self.adc_threshold = 0.
        self.adc_slope = 3.8
        self.max_cluster_size = 4

    def __del__(self):
        pass

    def calculate_cce(self,receive_particle_number,practical_adc_deposition):
        ideal_adc_deposition = (receive_particle_number*self.gain*self.emit_particle_energy*1000/self.mean_ionization_energy)/self.adc_slope
        cce = practical_adc_deposition/ideal_adc_deposition
        return cce,ideal_adc_deposition


    def analyze(self,fname):

        if not os.path.exists('./ccelog/'):
            os.makedirs('./ccelog/')
        logregex = re.compile('(./output/)(.*)(.root)')
        logmo = logregex.match(fname)
        logname = logmo.group(2)

        logging.basicConfig(level=logging.DEBUG)
        logger = logging.getLogger(fname)
        file_handler = logging.FileHandler('./ccelog/'+logname+'_cce.log')
        file_handler.setLevel(logging.DEBUG)
        file_handler.setFormatter(logging.Formatter(' %(asctime)s - %(levelname)s- %(message)s'))
        logger.addHandler(file_handler)

        practical_adc_deposition = np.zeros(self.max_cluster_size+1)
        receive_particle_number = np.zeros(self.max_cluster_size+1,dtype=int)

        try:
            f = ROOT.TFile(fname)
            t = f.Get('clusters')
            entries = t.GetEntries()
        except:
            logger.error(fname+' is invalid!')
            sys.exit()

        for ientry in xrange(entries):
            t.GetEntry(ientry)
            size = t.size
            adc = 0

            if size <= self.max_cluster_size:
                for ipos in xrange(size):
                    adc += t.signal.at(ipos)        
                if adc>self.adc_threshold:
                    receive_particle_number[0] += 1
                    receive_particle_number[size] += 1
                practical_adc_deposition[0] += adc
                practical_adc_deposition[size] +=adc

            else:
                continue

        cce,ideal_adc_deposition = self.calculate_cce(receive_particle_number,practical_adc_deposition)

        print('\n\n********************%s*********************\n'%fname)
        logger.info('   file name = %s'%fname)
        logger.info('   emit particle energy = %.2f keV'%self.emit_particle_energy)
        logger.info('   mean ionization energy = %s eV'%str(self.mean_ionization_energy))
        logger.info('   gain = %s'%str(self.gain))
        logger.info('   adc threshold = %s'%str(self.adc_threshold))
        logger.info('   adc slope = %s'%str(self.adc_slope)+ 'e')
        logger.info('   emit particle number = %s'%str(self.emit_particle_number))

        logger.info('   receive particle number = %s'%str(receive_particle_number[0]))
        logger.info('   ideal adc deposition = %s'%str(int(ideal_adc_deposition[0])))
        logger.info('   practical adc deposition = %s'%str(practical_adc_deposition[0]))
        logger.info('   charge collection efficiency = %.2f\n'%cce[0])

        for icluster in xrange(1,self.max_cluster_size+1):
            logger.info('   cluster%d receive particle number = %s'%(icluster,str(receive_particle_number[icluster])))
            logger.info('   cluster%d ideal adc deposition = %s'%(icluster,str(int(ideal_adc_deposition[icluster]))))
            logger.info('   cluster%d practical adc deposition = %s'%(icluster,str(practical_adc_deposition[icluster])))
            logger.info('   cluster%d charge collection efficiency = %.2f\n'%(icluster,cce[icluster]))

        logger.removeHandler(file_handler)

        return cce,receive_particle_number

if __name__ == '__main__':
    e = cceanalysis()
    cce1,rn1 = e.analyze('./output/data_withelectricfield_genapx.root')
    cce2,rn2 = e.analyze('./output/data_withoutelectricfield_genapx.root')

    cce_canvas = ROOT.TCanvas('cce','cce',600,500)
    cce_canvas.SetGrid()
    cce_multigraph = ROOT.TMultiGraph()
    legend1 = ROOT.TLegend(0.2,0.5,0.4,0.7)
        
    cce_graph_1 = ROOT.TGraph(e.max_cluster_size,np.arange(1.,e.max_cluster_size+1.,1.),cce1[1:])
    cce_graph_1.SetMarkerColor(46)
    cce_graph_1.SetMarkerStyle(20)
    cce_multigraph.Add(cce_graph_1)
    legend1.AddEntry(cce_graph_1,'with electric field')

    cce_graph_2 = ROOT.TGraph(e.max_cluster_size,np.arange(1.,e.max_cluster_size+1.,1.),cce2[1:])
    cce_graph_2.SetMarkerColor(38)
    cce_graph_2.SetMarkerStyle(21)
    cce_multigraph.Add(cce_graph_2)
    legend1.AddEntry(cce_graph_2,'without electric field')

    cce_multigraph.SetTitle('charge collection efficiency')
    cce_multigraph.Draw('ALP')
    cce_multigraph.GetXaxis().SetTitle('cluster')
    cce_multigraph.GetXaxis().CenterTitle()
    cce_multigraph.GetYaxis().SetTitle('cce')
    legend1.Draw()
    cce_canvas.Update()
    cce_canvas.SaveAs('./fig/cce_graph.pdf')



    rn_canvas = ROOT.TCanvas('rn','rn',600,500)
    rn_canvas.SetGrid()
    rn_multigraph = ROOT.TMultiGraph()
    legend2 = ROOT.TLegend(0.7,0.7,0.9,0.9)
        
    rn_graph_1 = ROOT.TGraph(e.max_cluster_size,np.arange(1.,e.max_cluster_size+1.,1.),rn1[1:]*1.0/rn1[0])
    rn_graph_1.SetMarkerColor(46)
    rn_graph_1.SetMarkerStyle(20)
    rn_multigraph.Add(rn_graph_1)
    legend2.AddEntry(rn_graph_1,'with electric field')

    rn_graph_2 = ROOT.TGraph(e.max_cluster_size,np.arange(1.,e.max_cluster_size+1.,1.),rn2[1:]*1.0/rn2[0])
    rn_graph_2.SetMarkerColor(38)
    rn_graph_2.SetMarkerStyle(21)
    rn_multigraph.Add(rn_graph_2)
    legend2.AddEntry(rn_graph_2,'without electric field')

    rn_multigraph.SetTitle('cluster entries proportion')
    rn_multigraph.Draw('ALP')
    rn_multigraph.GetXaxis().SetTitle('cluster')
    rn_multigraph.GetXaxis().CenterTitle()
    rn_multigraph.GetYaxis().SetTitle('entries proportion')
    legend2.Draw()
    rn_canvas.Update()
    rn_canvas.SaveAs('./fig/rn_graph.pdf') 



    sys.exit()
