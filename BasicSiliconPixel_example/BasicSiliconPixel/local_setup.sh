
Geant4Bin=/usr/local/Cellar/geant4/10.3.1/bin
source ${Geant4Bin}/geant4.sh

Geant4Make=/usr/local/Cellar/geant4/10.3.1/share/Geant4-10.3.1/geant4make
source ${Geant4Make}/geant4make.sh

source /usr/local/Cellar/root6/6.08.06_1/libexec/thisroot.sh
export PATH=$ROOTSYS/bin:$PATH

export G4WORKDIR="/Users/chenliejian/Documents/Code/jadepix/BasicSiliconPixel_example/BasicSiliconPixel_install"
export PATH=$PATH:$G4WORKDIR/bin


# Mac OS 10.11 image bug (If not, ignore)
export DYLD_LIBRARY_PATH=/System/Library/Frameworks/ImageIO.framework/Versions/A/Resources/:$DYLD_LIBRARY_PATH
