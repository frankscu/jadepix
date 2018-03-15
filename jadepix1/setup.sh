# Running environment (not for develping allpix-squared)

Geant4Bin=/opt/allpix/geant4.10.04/install/bin   
source ${Geant4Bin}/geant4.sh 
source /opt/allpix/root/install/bin/thisroot.sh

# Add allpix-squared v1.1.1 
export ALLPIX=/opt/allpix/v1.1.1
export PATH=$PATH:$ALLPIX/bin:$ALLPIX/bin/tcad_dfise_converter
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ALLPIX/lib
