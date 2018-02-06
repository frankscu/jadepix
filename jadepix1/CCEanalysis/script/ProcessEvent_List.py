import os
import sys
import re
import numpy as np
from matplotlib import pyplot 

def Draw_Hist(adc):
    pyplot.hist(adc, 100,normed=1,histtype='bar',facecolor='blue',alpha=0.75)     
    pyplot.xlabel('ADC')
    pyplot.ylabel('Frequency')
    pyplot.title('Pix ADC')
    pyplot.show()


def Reshape_Frame(frameint):

    Frame_Array=np.reshape(frameint, newshape=(48, 16, 2))
    Frame=np.zeros((48, 16), dtype=int)
    for i in range(0, 48):
        for j in range(0, 16):
            tmp=Frame_Array[i, j, 0]*256+Frame_Array[i, j, 1]
            Frame[i, j]=tmp
    Frame_ADC = np.reshape(Frame,-1)
    return Frame,Frame_ADC


def Bytes_TO_Int(thebytes):
    Int_Data = np.frombuffer(thebytes, dtype=np.uint8)
    #print(len(Int_Data))
    return Int_Data


def Process_Raw(framebytes):
    Clean_Frame_Bytes = b''
    Origin_Frame_Bytes = framebytes                                          #without event header and footer but row header and footer

    tmp = re.findall(b'....(.{32})....',Origin_Frame_Bytes,re.DOTALL)        #depend on data structure

    Clean_Frame_Bytes = b''.join(tmp)

    #print(Clean_Frame_Bytes)
    return Clean_Frame_Bytes

def Process_Frame(data,tryprocessnumber,maxframenumber):


    Try_Position = 0
    Frame_Number = 0
    Broken_Frame_Number = 0
    ADC = []
    #flag = 0 # use in no frame

    while Frame_Number < maxframenumber:   

        data.seek(Try_Position)
        Try_Process_Data = data.read(tryprocessnumber)

        if len(Try_Process_Data) != tryprocessnumber:
            print(' Over !')
            break

        m =re.search(b'(\xaa\xaa\xaa\xaa)(.*?)(\xf0\xf0\xf0\xf0)',Try_Process_Data,re.DOTALL)

        if m:
        #################### process  ######################
            if len(m.group(2)) == 1920:
                Frame_Number += 1
                Frame_Bytes = m.group(2)
                if Frame_Number%1000 == 0:
                    print('\nFind',Frame_Number,'frames !')
                    print('position: ','(%d %d)'%(Try_Position+m.start(),Try_Position+m.end()))

                Clean_Frame_Bytes = Process_Raw(m.group(2))
                Frame_Int = Bytes_TO_Int(Clean_Frame_Bytes)
                #print(type(Frame_Int))
                Frame,Frame_ADC= Reshape_Frame(Frame_Int)
                ADC += Frame_ADC.tolist()
         

            else:
                Broken_Frame_Number += 1
                print('\033[33;1m','\nFind',Broken_Frame_Number,'broken frames!')
                print('position: ','(%d %d)'%(Try_Position+m.start(),Try_Position+m.end()),'\033[0m')

        #####################################################

            Try_Position = Try_Position + (len(m.group()))

        else:
        ######################## update next time ############
            print('There is no frame in ( %d %d )'%(Try_Position,Try_Position+tryprocessnumber))
            Try_Position += tryprocessnumber
            # flag += 1
            # if flag == 5:
            #     break
        ######################################################
    print('Find total frames :',Frame_Number)
    print('Find total broken frames :',Broken_Frame_Number)

    return ADC
    
def Process_Data(filename='',tryprocessnumber=1928,maxframenumber=10):

    data = open(filename,'rb')

    ADC = Process_Frame(data,tryprocessnumber,maxframenumber)

    data.close()

    print('ADC length :',len(ADC))
    Draw_Hist(ADC)



if __name__=="__main__":
    Process_Data(filename='../data/TestFile20180105-000.dat',tryprocessnumber=10000,maxframenumber=100000)    #warning: tryprocessnumber should be grater than size of frame (for example 1928)
    sys.exit()
 