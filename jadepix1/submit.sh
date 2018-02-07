#!/usr/bin/env bash

# Main driver to submit 
# Author Maoqiang Jing <jingmq@ihep.ac.cn>
# Created [2018-02-02 Fri 19:24]


usage() {
    printf "NAME\n\tsubmit.sh - Main driver to submit\n"
    printf "\nSYNOPSIS\n"
    printf "\n\t%-5s\n" "./run.sh [OPTION]"
    printf "\nOPTIONS\n"
    printf "\n\t%-9s  %-40s"  "0.1"      "[Simulate jadepix1]" 
    printf "\n\t%-9s  %-40s"  "0.1.1"    "Run jadepix1 conf files" 
    printf "\n\t%-9s  %-40s"  "0.2"      "[Get CCE]"
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

    0.1) echo "Simulating jadepix1..."
         ;;
    0.1.1) echo "Running jadepix1 conf files..."
           cd conf
           ./../allpix-squared/bin/allpix -c jadepix1.conf
           ;;
    0.2) echo "Getting CCE..."
         ;;
esac
