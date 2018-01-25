  #!/bin/bash
  
  for (( i =0; i<50; i= i+1))
  do
    ./runSim.sh
    ./runRec.sh
    mv JadePix.recdat JadePix_$i.recdat  
  done
