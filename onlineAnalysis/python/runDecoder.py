#/usr/bin/env python3

import jadepix as jp
import matplotlib.pyplot as plt
from matplotlib import gridspec
from matplotlib.backends.backend_pdf import PdfPages
import numpy as np

def drawHitMaps(j, data):
    fig=plt.figure(j/8,figsize=(12,12))
    gs = gridspec.GridSpec(2, 4, height_ratios=[1, 1])
    ax = plt.subplot(gs[j%8])
    cax = ax.imshow(data)
    cbar = fig.colorbar(cax)


def readRawFile():
    dirName = "../data/"
    runnum = 1
    jd = jp.JadepixDecoder.Instance()
    jd.OpenRawFile(dirName, runnum)
    eventNumbers=jd.ReadEvent()
    print("Event numbers: ", eventNumbers)

    frameNumbers=jd.NoOfFrame()
    print("Frame numbers per event: ", frameNumbers)

    for j in range(0, frameNumbers):
        frame=jd.GetFrame(j)
        print("Digi numbers per frame: ", frame.NoOfDigi())

        digi=frame.GetDigi(1)
        print("Digi ADC: ", digi.GetADC())

        frameMap=[]
        for i in range(0,768):
            digi=frame.GetDigi(i)
            frameMap.append(digi.GetADC())

        hitMaps = np.reshape(frameMap,(48,16))
        drawHitMaps(j, hitMaps)

    fPdf = PdfPages('../results/Hit_Map.pdf')
    for n in range(0, int(frameNumbers/8)):
        fig=plt.figure(n,figsize=(12,12))
        fPdf.savefig()

    fPdf.close()

    input("Please press enter to exit")

if __name__ == "__main__":
    readRawFile()
