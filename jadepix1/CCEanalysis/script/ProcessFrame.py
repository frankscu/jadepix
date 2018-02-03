import os
import sys
import re
import numpy as np

def Find_First_Frame_Position(filename,tryprocessnumber,maxframenumber):

    data = open(filename,'rb')

    Data_Position = 0

    Frame_Number = 0
    Broken_Frame_Number = 0

    while True:   
        
        Try_Process_Data = data.read(tryprocessnumber)
        if len(Try_Process_Data) != tryprocessnumber:
            print('Over!')
            break
        m =re.search(b'(\xaa\xaa\xaa\xaa)(.*?)(\xf0\xf0\xf0\xf0)',Try_Process_Data,re.DOTALL)
        Data_Position = m.start()

        if len(m.group(2)) == 1920:
            Frame_Number += 1
            print('Find',Frame_Number,'frames !')
        else:
            Broken_Frame_Number += 1
            print('\033[1;31;40m','\nFind',Broken_Frame_Number,'broken frames!','\033[0m')

        Data_Position = Data_Position + (len(m.group(2))+8)
        data.seek(Data_Position)
        if Frame_Number == maxframenumber:
            break

    data.close()

Find_First_Frame_Position('../data/TestFile20180105-000.dat',10000,100)




# FrameArray=np.reshape(FrameIntData, newshape=(48, 16, 2))
        
# Frame=np.zeros((48, 16), dtype=int)
# for i in range(0, 48):
#     for j in range(0, 16):
#         tmp=FrameArray[i, j, 0]*256+FrameArray[i, j, 1]
#         Frame[i, j]=tmp


if __name__=="__main__":
    sys.exit()
 