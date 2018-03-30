export CXX=/usr/bin/g++
export CC=/usr/bin/gcc


Geant4Bin=/opt/allpix/geant4.10.04/install/bin
source ${Geant4Bin}/geant4.sh

Geant4Make=/opt/allpix/geant4.10.04/install/share/Geant4-10.4.0/geant4make
source ${Geant4Make}/geant4make.sh

source /opt/allpix/root/install/bin/thisroot.sh
export PATH=$ROOTSYS/bin:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib

export PATH=$PATH:${HOME}/jadepix/software/lib/ana/bin

export PATH=$PATH:${HOME}/jadepix/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HOME}/jadepix/lib
