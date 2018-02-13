#!/usr/bin/env bash

# Main driver to submit 
# Author: 
#      Maoqiang Jing <jingmq@ihep.ac.cn> 
#      Tao Yang <yangtao@ihep.ac.cn> 
# Created [2018-02-02 Fri 19:24]


usage() {
    printf "NAME\n\tsubmit.sh - Main driver to submit\n"
    printf "\nSYNOPSIS\n"
    printf "\n\t%-5s\n" "./run.sh [OPTION]"
    printf "\nOPTIONS\n"
    printf "\n\t%-9s  %-40s"  "0.1"      "[Simulate jadepix1]" 
    printf "\n\t%-9s  %-40s"  "0.1.1"    "Run jadepix1 conf files" 

    printf "\n\t%-9s  %-40s"  "0.2"      "[Process Data]"
    printf "\n\t%-9s  %-40s"  "0.2.1"    "Process data by list"

　　printf "\n\t%-9s  %-40s"  "0.3"      "[Analyze Data]"
    printf "\n\t%-9s  %-40s"  "0.3.1"    "Analyze data"
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
    #  0.1 allpix-squared (v1.1.0)
    # --------------------------------------------------------------------------

    0.1) echo "Simulating jadepix1..."
         ;;
    0.1.1) echo "Running jadepix1 conf files..."
        allpix -c conf/jadepix1.conf -o output_directory="../output" 
        ;;
   
   
    # --------------------------------------------------------------------------
    #  0.2 Analyze data 
    # --------------------------------------------------------------------------


    0.2) echo "Process Data..."
         ;;
    0.2.1) echo "Process data by list..."
	    ./python/ProcessEvent_List.py
	    ;;


　　# --------------------------------------------------------------------------
    #  0.3 Analyze real data 
    # --------------------------------------------------------------------------


　　0.2) echo "Analyze Real Data..."
         ;;
    0.3.1) echo "Analyze one example data (10000 frames)"
	   cd  ./src/
           make
           ./pixelana /publicfs/atlas/atlasnew/higgs/tth/hbb/raw_pixel/20180208/File20180208-A1_001 ./test.root 10000
	    ;;

esac
