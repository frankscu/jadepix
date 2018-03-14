#!/usr/bin/env python

'''
cce analysis module
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

        self.fname = None
        self.logname = None
        self.logger = None
        self.practical_adc_deposition = None
        self.receive_particle_number = None
        self.cce = None

    def __del__(self):
        pass

    def calculate_cce(self,receive_particle_number,practical_adc_deposition):
        ideal_adc_deposition = (receive_particle_number*self.gain*self.emit_particle_energy*1000/self.mean_ionization_energy)/self.adc_slope
        cce = practical_adc_deposition/ideal_adc_deposition
        return cce,ideal_adc_deposition

    def get_logger(self):
        logging.basicConfig(level=logging.DEBUG)
        logger = logging.getLogger(self.fname)
        self.file_handler = logging.FileHandler('./ccelog/'+self.logname+'_cce.log')
        self.file_handler.setLevel(logging.DEBUG)
        self.file_handler.setFormatter(logging.Formatter(' %(asctime)s - %(levelname)s- %(message)s'))
        logger.addHandler(self.file_handler)
        return logger



    def loginfo(self,logger):
        logger.info('   file name = %s'%self.fname)
        logger.info('   emit particle energy = %.2f keV'%self.emit_particle_energy)
        logger.info('   mean ionization energy = %s eV'%str(self.mean_ionization_energy))
        logger.info('   gain = %s'%str(self.gain))
        logger.info('   adc threshold = %s'%str(self.adc_threshold))
        logger.info('   adc slope = %s'%str(self.adc_slope)+ 'e')
        logger.info('   emit particle number = %s'%str(self.emit_particle_number))
        logger.info('   receive particle number = %s'%str(self.receive_particle_number[0]))
        logger.info('   ideal adc deposition = %s'%str(int(self.ideal_adc_deposition[0])))
        logger.info('   practical adc deposition = %s'%str(self.practical_adc_deposition[0]))
        logger.info('   charge collection efficiency = %.2f\n'%self.cce[0])

        for icluster in xrange(1,self.max_cluster_size+1):
            logger.info('   cluster%d receive particle number = %s'%(icluster,str(self.receive_particle_number[icluster])))
            logger.info('   cluster%d ideal adc deposition = %s'%(icluster,str(int(self.ideal_adc_deposition[icluster]))))
            logger.info('   cluster%d practical adc deposition = %s'%(icluster,str(self.practical_adc_deposition[icluster])))
            logger.info('   cluster%d charge collection efficiency = %.2f\n'%(icluster,self.cce[icluster]))

        logger.removeHandler(self.file_handler)


    def analyze(self,fname):

        self.fname = fname

        if not os.path.exists('./ccelog/'):
            os.makedirs('./ccelog/')
        logregex = re.compile('(./output/)(.*)(.root)')
        logmo = logregex.match(self.fname)
        self.logname = logmo.group(2)
        self.logger = self.get_logger()

        self.practical_adc_deposition = np.zeros(self.max_cluster_size+1)
        self.receive_particle_number = np.zeros(self.max_cluster_size+1,dtype=int)

        try:
            f = ROOT.TFile(self.fname)
            t = f.Get('clusters')
            entries = t.GetEntries()
        except:
            logger.error(self.fname+' is invalid!')
            sys.exit()

        for ientry in xrange(entries):
            t.GetEntry(ientry)
            size = t.size
            adc = 0

            if size <= self.max_cluster_size:
                for ipos in xrange(size):
                    adc += t.signal.at(ipos)        
                if adc>self.adc_threshold:
                    self.receive_particle_number[0] += 1
                    self.receive_particle_number[size] += 1
                self.practical_adc_deposition[0] += adc
                self.practical_adc_deposition[size] +=adc

            else:
                continue

        self.cce,self.ideal_adc_deposition = self.calculate_cce(self.receive_particle_number,self.practical_adc_deposition)

        print('\n********************%s*********************\n'%self.fname)

        self.loginfo(self.logger)

        return self.cce,self.receive_particle_number


if __name__ == '__main__':
    e = cceanalysis()
    e.analyze('./output/data_with_electricfield_genapx.root')
    e.analyze('./output/data_without_electricfield_genapx.root')