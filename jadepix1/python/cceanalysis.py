#!/usr/bin/env python

'''
cce alalysis module
'''

__author__ = "YANG TAO <yangtao@ihep.ac.cn>"
__copyright__ = "Copyright (c) yangtao"
__created__ = "[2018-03-09 Fri 19:00]"

import sys,os
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


    def calculate_cce(self,receive_particle_number,practical_adc_deposition):
        ideal_adc_deposition = (receive_particle_number*self.gain*self.emit_particle_energy*1000/self.mean_ionization_energy)/self.adc_slope
        cce = round(practical_adc_deposition/ideal_adc_deposition,2)
        return cce,ideal_adc_deposition


    def analyze(self,fname):

        if not os.path.exists('./ccelog/'):
            os.makedirs('./ccelog/')

        logname = fname
        logname.replace('output','ccelog')
        logname.replace('.root','_cce.log')

        logging.basicConfig(level=logging.DEBUG, format=' %(asctime)s - %(levelname)s- %(message)s',filename=logname,filemode='w')
        console = logging.StreamHandler()
        console.setLevel(logging.DEBUG)
        console.setFormatter(logging.Formatter(' %(asctime)s - %(levelname)s- %(message)s'))
        logging.getLogger('').addHandler(console)

        practical_adc_deposition = 0
        receive_particle_number = 0
        try:
            f = ROOT.TFile(fname)
            t = f.Get('clusters')
            entries = t.GetEntries()
        except:
            logging.error(fname+' is invalid!')
            sys.exit()

        for ientry in xrange(entries):
            t.GetEntry(ientry)
            size = t.size
            adc = 0
            for ipos in xrange(size):
                adc += t.signal.at(ipos)        
            if adc>self.adc_threshold:
                receive_particle_number += 1
            practical_adc_deposition += adc

        cce,ideal_adc_deposition = self.calculate_cce(receive_particle_number,practical_adc_deposition)

        logging.info('file name : '+fname)
        logging.info('emit particle energy : '+str(self.emit_particle_energy)+' keV')
        logging.info('mean ionization energy : '+str(self.mean_ionization_energy)+' eV')
        logging.info('gain : '+str(self.gain))
        logging.info('adc threshold : '+str(self.adc_threshold))
        logging.info('adc slope : '+str(self.adc_slope)+ 'e')
        logging.info('emit particle number : '+str(self.emit_particle_number))
        logging.info('receive particle number : '+str(receive_particle_number))
        logging.info('ideal adc deposition : '+str(int(ideal_adc_deposition)))
        logging.info('practical adc deposition : '+str(practical_adc_deposition))
        logging.info('charge collection efficiency : '+str(cce))

if __name__ == '__main__':
    e = cceanalysis()
    e.analyze('./output/data_withelectricfield_genapx.root')
    
