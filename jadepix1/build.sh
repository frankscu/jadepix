#!/usr/bin/env bash

# Main driver to build softwares
# Author Maoqiang Jing <jingmq@ihep.ac.cn>
# Created [2018-02-02 Fri 17:02]


usage() {
    printf "NAME\n\tbuild.sh - Main driver to build softwares \n"
    printf "\nSYNOPSIS\n"
    printf "\n\t%-5s\n"  "./build.sh [OPTION]"
    printf "\nOPTIONS\n"
    printf "\n\t%-5s  %-40s\n"  "0.1"  "[Build allpix-squared]"
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
esac
