# Simulation(Iron-55, using allpix-squared) and analysis(scripts written by python) for jadepix on CEPC

To run the programs successfully, you must follow the following order.

## Clean old programs 

> cd $HOME/jadepix/jadepix1/run

> ./cleanup.sh 

## Setup

ROOT verion 6, Geant4 version 4.10.04, allpix-squared version 1.1.0

This have to be done when you log in every time: 

> cd $HOME/jadepix/jadepix1/run

> source setup.sh

## Build code

> cd $HOME/jadepix/jadepix1/run

> ./build.sh 

## Submit jobs

> cd $HOME/jadepix/jadepix1/run

> ./submit.sh

## For developers 

- Fork the code with your personal github ID. See [details](https://help.github.com/articles/fork-a-repo/)

> git clone git@github.com:frankscu/jadepix.git .

- Make your change, commit and push 

> git commit -a -m "Added feature A, B, C"
> git push

- Make a pull request to cepc. See [details](https://help.github.com/articles/using-pull-requests/)

## Some styles to follow 
- Minimize the number of main c++ files 
- Keep functions length less than one screen
- Seperate hard-coded cuts into script file
- Use pull-request mode on git 
- Document well the high-level bash file for work flow 


