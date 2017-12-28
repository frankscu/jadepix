export CXX=/usr/local/bin/g++
export CC=/usr/local/bin/gcc
alias gcc=/usr/local/bin/gcc
alias g++=/usr/local/bin/g++

Geant4Bin=/usr/local/geant4/10.01.p03/bin
source ${Geant4Bin}/geant4.sh

Geant4Make=/usr/local/geant4/10.01.p03/share/Geant4-10.1.3/geant4make
source ${Geant4Make}/geant4make.sh

source /usr/local/root/root-6.10.08/build/bin/thisroot.sh
export PATH=$ROOTSYS/bin:$PATH

export MIMOSADIR="$HOME/jadepix/Mimosa"
export G4WORKDIR="$HOME/jadepix/Mimosa/install"
export PATH=$PATH:$G4WORKDIR/bin

