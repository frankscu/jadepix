#!/usr/bin/env bash

# Main driver to build softwares
# Author Maoqiang Jing <jingmq@ihep.ac.cn>
# Created [2018-02-02 Fri 17:02]


usage() {
    printf "NAME\n\tbuild.sh - Main driver to build softwares \n"
    printf "\nSYNOPSIS\n"
    printf "\n\t%-5s\n"  "./build.sh [OPTION]"
    printf "\nOPTIONS\n"
    printf "\n\t%-5s  %-40s\n"  "0.1"  "Build allpix-squared"
    printf "\n"
    printf "\n\t%-5s  %-40s\n"  "0.2"  "Build genApx and convert"
    printf "\n\t%-5s  %-40s\n"  "0.2.1"  "Build genApx"
    printf "\n\t%-5s  %-40s\n"  "0.2.2"  "Build genApx"
    printf "\n"
    printf "\n\t%-5s  %-40s\n"  "0.3"  "Build jadepix"
}

if [[ $# -eq 0 ]]; then
    usage
    printf "\nPlease enter your option: "
    read option
else
    option=$1
fi


case $option in
    0.1) echo "Building allpix-squared..."
        git clone https://gitlab.cern.ch/allpix-squared/allpix-squared.git
        cd allpix-squared
        mkdir build
        cd build
        source /opt/allpix/build_env.sh
        cmake ..
        make -j20
        make install
        ;;

    0.2) echo  "Building genApx and convert..."
    0.2.1) echo  "Building genApx..."
      make 
      ;; 
    0.2.2) echo  "Building convert..."
      make 
      ;; 

    0.3) echo  "Building jadepix..."
      mkdir -p ../build
      cd ../build
      source ../etc/cepcvtx_setup.sh
      cmake ..
      make -j20
      make install
      ;; 

esac
