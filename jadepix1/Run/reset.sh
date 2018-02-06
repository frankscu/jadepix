#!/usr/bin/env bash

# Main driver to reset
# Author Maoqiang Jing <jingmq@ihep.ac.cn>
# Created [2018-02-06 Tue 9:28]


usage() {
    printf "NAME\n\treset.sh - Main driver to reset\n"
    printf "\nSYNOPSIS\n"
    printf "\n\t%-5s\n" "./run.sh [OPTION]"
    printf "\nOPTIONS\n"
    printf "\n\t%-9s  %-40s"  "0.1"      "[Simulate jadepix1]" 
    printf "\n\t%-9s  %-40s"  "0.1.1"      "Clean ./../Simulation/output" 
    printf "\n\t%-9s  %-40s"  "0.2"      "[Allpix-squared]"
    printf "\n\t%-9s  %-40s"  "0.2"      "Clean ./../allpix-squared"
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

    0.1) echo "Simulate jadepix1..."
         ;;
    0.1.1) echo "Cleaning ./../Simulation/output..."
           cd ../Simulation
           rm output -rf
           ;;
    0.2) echo "Allpix-squared..."
         ;;
    0.2.1) echo "Cleaning ./../allpix-squared..."
           cd ../
           rm allpix-squared -rf
         ;;
esac
