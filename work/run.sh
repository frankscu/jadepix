  #!/bin/bash
 
  rm JadePix* -rf 
  for (( i =0; i<50; i= i+1))
  do
    ./runSim.sh 
    ./runRec.sh 
    mv JadePix.rawdat JadePix_$i.rawdat
    mv JadePix.recdat JadePix_$i.recdat  
  done
