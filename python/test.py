#/usr/bin/env python3

import jadipix as jp
import matplotlib.pyplot as plt
import numpy as np

def readRawFile():
    dirName = "../data/"
    runnum = 3
    jd = jp.JadipixDecoder.Instance()
    jd.OpenRawFile(dirName, runnum)
    jd.ReadEvent()

    framePair=jd.GetFramePair()
    #print(framePair)

    frameDataMap=jd.GetFrameDataMap().asdict()
    #print(frameDataMap)
    frameDataMapValues=[it for it in frameDataMap.values()]
    #print(frameDataMapValues)

    fig=plt.figure(1,figsize=(6,6))
    plt.imshow(np.reshape(frameDataMapValues,(48,16)))
    plt.colorbar()
    fig.show()
    input("Please press enter to exit")

if __name__ == "__main__":
    readRawFile()
