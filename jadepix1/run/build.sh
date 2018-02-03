#!/usr/bin/env bash

# Main driver to build programs
# Author Maoqiang Jing <jingmq@ihep.ac.cn>
# Created [2018-02-02 Fri 17:02]


usage() {
    printf "NAME\n\tbuild.sh - Main driver to build programs \n"
    printf "\nSYNOPSIS\n"
    printf "\n\t%-5s\n"  "./build.sh [OPTION]"
    printf "\nOPTIONS\n"
    printf "\n\t%-5s  %-40s\n"  "1"  "clone allpix-squared programs"
}

if [[ $# -eq 0 ]]; then
    usage
    printf "\nPlease enter your option: "
    read option
else
    option=$1
fi


case $option in
    1) echo "Cloning allpix-squared programs..."
        cd ../
        git clone https://gitlab.cern.ch/allpix-squared/allpix-squared.git
        cd allpix-squared
        mkdir build
        cd build
        cmake ..
        make -j20
        make install
        cd ../../run
        ;;
esac
