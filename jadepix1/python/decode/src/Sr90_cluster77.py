#!/usr/bin/env python

'''
decode Sr90 source data to cluster 7*7 size
'''

__author__ = "YANG TAO <yangtao@ihep.ac.cn>"
__copyright__ = "Copyright (c) yangtao"
__created__ = "[2018-04-10 Apr 12:00]"

import os
import sys
import re
import numpy as np
import time
import ROOT
ROOT.gStyle.SetOptFit()
ROOT.gStyle.SetStatX(0.9)
ROOT.gStyle.SetStatY(0.9)
ROOT.gStyle.SetStatW(0.08)
ROOT.gStyle.SetStatH(0.12)
import logging
logging.basicConfig(level=logging.DEBUG,format= ' %(asctime)s - %(levelname)s- %(message)s')
# file_handler = logging.FileHandler('../log/iron_55.log')
# file_handler.setLevel(logging.CRITICAL)
# file_handler.setFormatter(logging.Formatter(' %(asctime)s - %(levelname)s- %(message)s'))
# logger.addHandler(file_handler)


#create output root file
if not os.path.exists('../output/'):
    os.makedirs('../output/')

OUTPUT = ROOT.TFile('../output/cluster77_Sr90_thr10.root','recreate')

#creat deposition tree
# DEPOSITION_TREE = ROOT.TTree('Deposition_Tree','Deposition')
# PIXEL_ADC = np.zeros(1, dtype='int16')
# DEPOSITION_TREE.Branch('TotalPixsSignal',PIXEL_ADC,'TotalPixsSignal/S')


#creat clusters tree
CLUSTER_TREE = ROOT.TTree('Cluster_Tree','Cluster')

SEED_ADC = np.zeros(1, dtype='int16')
CLUSTER_ADC = np.zeros(1, dtype='int16')
SIZE = np.zeros(1, dtype='int16')

CLUSTER_TREE.Branch('SeedSignal',SEED_ADC,'SeedSignal/S')
CLUSTER_TREE.Branch('ClusterSignal',CLUSTER_ADC,'ClusterSignal/S')
CLUSTER_TREE.Branch('Size',SIZE,'Size/S')

#define thread
SEED_THR = 10
CLUSTER_THR = 10
ADC_SIZE_THR = 10  #warning:ADC_SIZE_THR < SEED_THR = CLUSTER_THR
CLUSTER_SIZE = 3


#get abs for int16 number
def get_int16_abs(number):
    if (number & 0x8000) != 0:  
        abs_number = ((number - 1) ^ 0xFFFF)
    else:
        abs_number = number
    return abs_number

#to reshape cds frame
def fill_root(frame_adc):

    #global DEPOSITION_TREE,PIXEL_ADC
    global CLUSTER_TREE,SEED_ADC,CLUSTER_ADC,SIZE

    row_seed = 0
    chanel_seed = 0

    tmp_adc = 0
    tmp_seed_adc = 0
    tmp_cluster_adc = 0
    tmp_size = 0
    tmp_frame_array=np.reshape(frame_adc, newshape=(48, 16, 2))
    tmp_frame=np.zeros((48,16), dtype='int16')

    for row in xrange(0,48):
        for chanel in xrange(0,16):

            tmp_adc =tmp_frame_array[row, chanel, 0]+tmp_frame_array[row, chanel, 1]*256
            tmp_adc = get_int16_abs(tmp_adc)
            tmp_frame[row,chanel] = tmp_adc

            # PIXEL_ADC[0] = tmp_adc
            # DEPOSITION_TREE.Fill()

            #count size#
            if tmp_adc > ADC_SIZE_THR:
                tmp_size += 1
            #count end#

            #find seed#
            if tmp_adc > tmp_seed_adc:
                tmp_seed_adc = tmp_adc
                row_seed = row
                chanel_seed = chanel
            #seed  end#
 
    row_cluster_start = row_seed - CLUSTER_SIZE
    row_cluster_end = row_seed + CLUSTER_SIZE

    chanel_cluster_start = chanel_seed - CLUSTER_SIZE
    chanel_cluster_end = chanel_seed + CLUSTER_SIZE


    if tmp_seed_adc > SEED_THR:
        SEED_ADC[0] = tmp_seed_adc
        for row_pos in xrange(row_cluster_start,row_cluster_end+1):
            for chanel_pos in xrange(chanel_cluster_start,chanel_cluster_end+1):
                if( (row_pos>=0) and (row_pos<48) and (chanel_pos>=0) and (chanel_pos<16)):
                    tmp_cluster_adc += tmp_frame[row_pos,chanel_pos] 

        CLUSTER_ADC[0] = tmp_cluster_adc
        SIZE[0] = tmp_size
        CLUSTER_TREE.Fill()
                  
    #return tmp_frame    
    #print(FRAME_ADC)
    

#to reshape normal frame
# def reshape_unsigned_frame(frame_adc):
#     frame_array=np.reshape(frame_adc, newshape=(48, 16, 2))
#     frame=np.zeros((48, 16), dtype='uint16')
#     for i in range(0, 48):
#         for j in range(0, 16):
#             tmp=frame_array[i, j, 0]+frame_array[i, j, 1]*256
#             frame[i, j]= tmp
#     real_frame_adc = np.reshape(frame,-1)
#     return real_frame_adc

def bytes_to_int(thebytes):
    uint_data = np.frombuffer(thebytes, dtype=np.uint8)
    int_data = uint_data.astype('int16')
    return int_data

def process_raw(framebytes):
    clear_frame_bytes = b''
    origin_frame_bytes = framebytes                                          #without event header and footer but row header and footer
    tmp = re.findall(b'....(.{32})....',origin_frame_bytes,re.DOTALL)        #depend on data structure
    clear_frame_bytes = b''.join(tmp)
    return clear_frame_bytes

def process_frame(data,try_process_number,maxframenumber):
    seek_position = 0
    frame_number = 0
    cds_frame_number = 0
    broken_frame_number = 0
    broken_bulk_number = 0
    broken_flag = False
    print_number = 1000

    while frame_number < maxframenumber:   
        data.seek(seek_position)
        try_process_data = data.read(try_process_number)

        if len(try_process_data) != try_process_number:
            logging.critical('\033[33;1m find total %d frames!\033[0m'%frame_number)
            logging.critical('\033[32;1m find total %d cds frames!\033[0m'%cds_frame_number)
            logging.critical('\033[31;1m find total %d broken frames!\033[0m'%broken_frame_number)
            logging.critical('\033[35;1m find total %d broken bulk!\033[0m'%broken_bulk_number)
            logging.critical(' END !')
            break

        m =re.search(b'(\xaa\xaa\xaa\xaa)(.*?)(\xf0\xf0\xf0\xf0)',try_process_data,re.DOTALL)

        if m:
            if len(m.group(2)) == 1920:
                frame_number += 1
                frame_bytes = m.group(2)  
                clear_frame_bytes = process_raw(m.group(2))
                frame_adc = bytes_to_int(clear_frame_bytes)
                #real_frame_adc= reshape_unsigned_frame(frame_adc)
                #print(real_frame_adc)

            else:
                data.seek(seek_position+m.start())
                tmp_process_data = data.read(1928)
                tmp_m = re.search(b'(\xaa\xaa\xaa\xaa)(.{1920})(\xf0\xf0\xf0\xf0)',tmp_process_data,re.DOTALL)

                if tmp_m:
                    frame_number += 1
                    frame_bytes = tmp_m.group(2)
                    clear_frame_bytes = process_raw(tmp_m.group(2))
                    frame_adc = bytes_to_int(clear_frame_bytes)
                    #reshape_unsigned_frame(frame_adc)
                    #print(real_frame_adc)

                else:
                    broken_frame_number += 1
                    broken_flag = True
                    logging.info('\033[31;1m find %d broken frames!\033[0m'%broken_frame_number)
                    logging.info('\033[31;1m position: (%d %d) \033[0m'%(seek_position+m.start(),seek_position+m.end()))
                    logging.info('\033[31;1m broken length  : %d\033[0m'%len(m.group()))


            ### cds ####
            if (frame_number % 2) == 1 :
                last_frame_adc = frame_adc
            else:
                cds_frame_adc = frame_adc-last_frame_adc
                fill_root(cds_frame_adc)
                cds_frame_number += 1              
                #print(real_cds_frame_adc)
            ### cds end ####

            if frame_number % print_number == 0:
                logging.info('Find %d frames !'%frame_number)
                logging.info('position: (%d %d)'%(seek_position+m.start(),seek_position+m.end()))
                logging.info('Get %d cds frames'%cds_frame_number)
                #print(FRAME_ADC)

            seek_position += (m.start()+(len(m.group())))

        else:
            print('There is no frame in ( %d %d )'%(seek_position,seek_position+try_process_number))
            broken_flag = True
            broken_bulk_number += 1
            seek_position += try_process_number

    # DEPOSITION_TREE.Write()
    CLUSTER_TREE.Write()
    OUTPUT.Close()

    logging.critical('\033[33;1m find total %d frames!\033[0m'%frame_number)
    logging.critical('\033[32;1m find total %d cds frames!\033[0m'%cds_frame_number)
    logging.critical('\033[31;1m find total %d broken frames!\033[0m'%broken_frame_number)
    logging.critical('\033[35;1m find total %d broken bulk!\033[0m'%broken_bulk_number)

    
def process_Data(filename='',try_process_number=19280,maxframenumber=10):
    start_time = time.clock()
    data = open(filename,'rb')
    process_frame(data,try_process_number,maxframenumber)

    data.close()
    end_time = time.clock()
    print('Running time: %s Seconds'%(end_time-start_time))



if __name__=="__main__":
    
    while True:
        maxf = int(input('\033[33;1m\nPlease input the max framenumber (for example 100) :   \033[0m'))

        tmp = input('\033[33;1m \nProcess %d frames , y or n ?   \033[0m'%maxf)
        if tmp == 'y':
            #warning: try_process_number should be grater than size of frame (for example 1928) 
            process_Data(filename='../source_data/Sr_CHIPA1_180408115533.df',try_process_number=19280,maxframenumber=maxf)
            break
    sys.exit()
 
