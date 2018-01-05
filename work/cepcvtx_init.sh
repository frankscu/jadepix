Geant4Bin=/opt/allpix/geant4.10.04/install/bin
source ${Geant4Bin}/geant4.sh

Geant4Make=/opt/allpix/geant4.10.04/install/share/Geant4-10.4.0/geant4make
source ${Geant4Make}/geant4make.sh

source /opt/allpix/root/install/bin/thisroot.sh
export PATH=$ROOTSYS/bin:$PATH

export JADEPIXDIR="${HOME}/jadepix/JadePixSim/"
export G4WORKDIR="${HOME}/jadepix/JadePixSim/install"
export PATH=$PATH:$G4WORKDIR/bin
