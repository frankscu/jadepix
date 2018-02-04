import os
import sys
import re
import numpy as np
import matplotlib.pyplot as plt 


def Reshape_Frame(frameint):

    Frame_Array=np.reshape(frameint, newshape=(48, 16, 2))
    Frame=np.zeros((48, 16), dtype=int)
    for i in range(0, 48):
        for j in range(0, 16):
            tmp=Frame_Array[i, j, 0]*256+Frame_Array[i, j, 1]
            Frame[i, j]=tmp

    return Frame


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

    Frame_Bytes = b''
    Frame_Position = 0
    Frame_Number = 0
    Broken_Frame_Number = 0

    while Frame_Number < maxframenumber:   
        
        Try_Process_Data = data.read(tryprocessnumber)

        if len(Try_Process_Data) != tryprocessnumber:
            print('Over!')
            break

        m =re.search(b'(\xaa\xaa\xaa\xaa)(.*?)(\xf0\xf0\xf0\xf0)',Try_Process_Data,re.DOTALL)

        try:
            Frame_Position = m.start()

        except:
            print('There is no frame in this',tryprocessnumber,'bytes !')
            continue


        #################### process  ######################
        if len(m.group(2)) == 1920:
            Frame_Number += 1
            Frame_Bytes = m.group(2)
            #print(Frame_Bytes)
            print('Find',Frame_Number,'frames !')

            Clean_Frame_Bytes = Process_Raw(m.group(2))
            Frame_Int = Bytes_TO_Int(Clean_Frame_Bytes)
            Frame = Reshape_Frame(Frame_Int)
            #Draw_Frame(Frame)


        ########################################################

        else:
            Broken_Frame_Number += 1
            print('\033[1;31;40m','\nFind',Broken_Frame_Number,'broken frames!\n','position: ',m.span(),'\033[0m')

        Frame_Position = Frame_Position + (len(m.group()))
        data.seek(Frame_Position)

        #if Frame_Number == maxframenumber:
            #break

    
def Process_Data(filename,tryprocessnumber,maxframenumber):

    data = open(filename,'rb')

    Process_Frame(data,tryprocessnumber,maxframenumber)

    data.close()


Process_Data('../data/TestFile20180105-000.dat',10000,100)




if __name__=="__main__":
    sys.exit()
 