#!/usr/bin/env python

'''
cce analysis module
'''

__author__ = "YANG TAO <yangtao@ihep.ac.cn>"
__copyright__ = "Copyright (c) yangtao"
__created__ = "[2018-03-09 Fri 19:00]"

import sys,os
import re
import string
import numpy as np
import ROOT
import logging
import ConfigParser

class cceanalysis():

    def __init__(self):

        self.fname = None
        self.conf_name = None

        self.mean_ionization_energy = 3.6 #ev
        self.max_cluster_size = 4

        #input : will read in conf
        self.emit_particle_energy = None
        self.emit_events_number = None
        self.emit_particle_number = None
        self.gain = None
        self.adc_threshold = None
        self.adc_slope = None
        self.electrics_noise = None

        #output 
        self.practical_adc_deposition = None
        self.receive_events_number = None
        self.cce = None

    def __del__(self):
        pass

    def read_conf(self,conf_name):
        conf = ConfigParser.ConfigParser()
        conf.read(conf_name)

        self.emit_particle_energy = self.get_conf_value(conf.get('DepositionGeant4','beam_energy'))
        self.emit_events_number = self.get_conf_value(conf.get('Allpix','number_of_events'))
        self.emit_particle_number = self.get_conf_value(conf.get('DepositionGeant4','number_of_particles'))
        self.gain = self.get_conf_value(conf.get('DefaultDigitizer','gain'))
        self.adc_threshold = self.get_conf_value(conf.get('DefaultDigitizer','threshold'))
        self.adc_slope = self.get_conf_value(conf.get('DefaultDigitizer','adc_slope'))
        self.electrics_noise = self.get_conf_value(conf.get('DefaultDigitizer','electrics_noise'))

    def get_conf_value(self,str):
        regex = re.compile(r'(\d+)(.\d+)*')
        match = regex.match(str)
        if match.group(2):
            value = string.atof(match.group())
        else:
            value = int(match.group())
        return value

    def calculate_cce(self,receive_events_number,practical_adc_deposition):
        ideal_adc_deposition = (receive_events_number*self.emit_particle_number*self.gain*self.emit_particle_energy*1000/self.mean_ionization_energy)/self.adc_slope
        cce = practical_adc_deposition/ideal_adc_deposition
        return cce,ideal_adc_deposition

    def get_logger(self):
        logging.basicConfig(level=logging.DEBUG)
        logger = logging.getLogger(self.fname)
        self.file_handler = logging.FileHandler('./ccelog/'+self.name+'_cce.log')
        self.file_handler.setLevel(logging.DEBUG)
        self.file_handler.setFormatter(logging.Formatter(' %(asctime)s - %(levelname)s- %(message)s'))
        logger.addHandler(self.file_handler)
        return logger

    def loginfo(self,logger):
        logger.info('   file name = %s'%self.fname)
        logger.info('   emit particle energy = %.2f keV'%self.emit_particle_energy)
        logger.info('   emit particle number = %d '%self.emit_particle_number)
        logger.info('   mean ionization energy = %s eV'%str(self.mean_ionization_energy))
        logger.info('   gain = %s'%str(self.gain))
        logger.info('   adc threshold = %s e'%str(self.adc_threshold))
        logger.info('   adc slope = %s'%str(self.adc_slope)+ 'e')
        logger.info('   electrics_noise = %s'%str(self.electrics_noise)+ 'e')

        logger.info('   emit particle number = %s'%str(self.emit_events_number))
        logger.info('   receive particle number = %s'%str(self.receive_events_number[0]))
        logger.info('   ideal adc deposition = %s'%str(int(self.ideal_adc_deposition[0])))
        logger.info('   practical adc deposition = %s'%str(self.practical_adc_deposition[0]))
        logger.info('   charge collection efficiency = %.2f\n'%self.cce[0])

        for icluster in xrange(1,self.max_cluster_size+1):
            logger.info('   cluster%d receive events number = %s'%(icluster,str(self.receive_events_number[icluster])))
            logger.info('   cluster%d ideal adc deposition = %s'%(icluster,str(int(self.ideal_adc_deposition[icluster]))))
            logger.info('   cluster%d practical adc deposition = %s'%(icluster,str(self.practical_adc_deposition[icluster])))
            logger.info('   cluster%d charge collection efficiency = %.2f\n'%(icluster,self.cce[icluster]))

        logger.removeHandler(self.file_handler)


    def analyze(self,fname,conf_name):

        self.fname = fname
        self.conf_name = conf_name

        self.read_conf(self.conf_name)

        if not os.path.exists('./ccelog/'):
            os.makedirs('./ccelog/')

        name_regex = re.compile('(./output/)(.*)(.root)')
        name_match = name_regex.match(self.fname)

        self.name = name_match.group(2)
        self.logger = self.get_logger()

        self.practical_adc_deposition = np.zeros(self.max_cluster_size+1)
        self.receive_events_number = np.zeros(self.max_cluster_size+1,dtype=int)

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
                    self.receive_events_number[0] += 1
                    self.receive_events_number[size] += 1
                self.practical_adc_deposition[0] += adc
                self.practical_adc_deposition[size] +=adc

            else:
                continue

        self.cce,self.ideal_adc_deposition = self.calculate_cce(self.receive_events_number,self.practical_adc_deposition)

        print('\n********************%s*********************\n'%self.fname)

        self.loginfo(self.logger)

        return self.cce,self.receive_events_number


if __name__ == '__main__':
    e = cceanalysis()
    e.analyze('./output/data_with_electric_field_genapx.root','./conf/jadepix1_with_electric_field_main.conf')