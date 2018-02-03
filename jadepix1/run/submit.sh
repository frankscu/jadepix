#!/usr/bin/env bash

# Main driver to run 
# Author Maoqiang Jing <jingmq@ihep.ac.cn>
# Created [2018-02-02 Fri 19:24]


usage() {
    printf "NAME\n\trun.sh - Main driver to run\n"
    printf "\nSYNOPSIS\n"
    printf "\n\t%-5s\n" "./run.sh [OPTION]"
    printf "\nOPTIONS\n"
    printf "\n\t%-9s  %-40s"  "0.1"      "[build jadepix1 directory]" 
    printf "\n\t%-9s  %-40s"  "0.1.1"      "[run jadepix1 conf files]" 
    printf "\n\t%-9s  %-40s"  "0.2"      "[get cce]"
}


if [[ $# -eq 0 ]]; then
    usage
    printf "\nPlease enter your option: "
    read option
else
    option=$1
fi

case $option in

    # --------------------------------------------------------------------------
    #  0.1 allpix-squared(jadepix1)
    # --------------------------------------------------------------------------

    0.1) echo "Building jadepix1 directory..."
         cd ../allpix-squared
         rm jadepix1 -rf
         mkdir jadepix1
         ;;
    0.1.1) echo "Running jadepix1 conf files..."
           cd ../allpix-squared/jadepix1
           cp ../examples/example* . -rf
           mv example.conf jadepix1.conf
           mv example_detector.conf jadepix1_detector.conf
           mv example_electric_field.init jadepix1_electric_field.init
           sed -i 's/detectors_file = "example_detector.conf"/detectors_file = "jadepix1_detector.conf"/g' jadepix1.conf
           sed -i 's/file_name = "example_electric_field.init"/file_name = "jadepix1_electric_field.init"/g' jadepix1.conf
           sed -i 's/particle_type = "Pi+"/particle_type = "gamma"/g' jadepix1.conf
           sed -i 's/beam_energy = 120GeV/beam_energy = 5.89keV/g' jadepix1.conf
           sed -i 's/type = "test"/type = "mimosa26"/g' jadepix1_detector.conf
           ./../bin/allpix -c jadepix1.conf
           cd ../../run
           ;;
    0.2) echo "Getting CCE..."
         cd ../CCEanalysis/script
         python main.py
         ;;
esac
