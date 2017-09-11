# Basic Silicon Pixel Simulation Example		    
## An simple example for learining using Geant4 to simulate pixel detectors	

Reference example B1 (basic example from Geant4), see README_origin

Liejian <chenlj@ihep.ac.cn>        

### Dependencies:

1) Geant4 v4.10 or newer must be installed with the following dependencies
satisfied

- GDML (xcerces)
- OpenGL (optional, only needed to use openGL vizualisation)
- Inventor  (optional, only needed to use the very convenient OpenInventor vizualisation)
- Qt (optional, only needed to use Qt vizulisation)

2) ROOT 6.04 or newer .  A basic setup with xml parser is enough.

### Build:

Make sure you loaded your Geant4 and ROOT setups (check $G4LIB and $ROOTSYS vars for instance). For me, I use brew install Geant4 and ROOT, then create a local_setup.sh for configuration. In the file, change the folder assigned to your directory.

```
export G4WORKDIR=~/myG4WorkDirectory	
```	


I suggest the following work folder structure. 	 
```	
|--- BasicSiliconPixel_example /  				# Mother folder containing source, build and install folder
	 |----- BasicSiliconPixel 			# Source code folder to be checked out from Github 
	 |      |----- src 
	 |      |----- include
	 |      |----- macros 	 
	 |----- BasicSiliconPixel_build 	# Build folder for cmake 
	 |----- BasicSiliconPixel_install # Installation folder for cmake 
	 |		|----- bin		# BasicSiliconPixel executable folder


- include and src : These contain the source code of the BasicSiliconPixel example framework
files will be added to these directories 

- macros : This folder contains example of macro files to be used for different type of Simulation framework 

```


First create the mother folder : 	
``` 
	mkdir BasicSiliconPixel_example
	cd BasicSiliconPixel_example 
``` 

Then checkout the github version: 
    
    git clone git@github.com:frankscu/jadepix.git

Now create the build and install folder : 

	mkdir BasicSiliconPixel_build BasicSiliconPixel_install
	cd BasicSiliconPixel_build

Initialize cmake for compilation of BasicSiliconPixel example: 

	cmake ../BasicSiliconPixel -DCMAKE_INSTALL_PREFIX=$G4WORKDIR	

Once the Cmake environnement is set, to compile, simply execute : 

	make -jX install 

X is a number of processor 
	
### Running:

Setup your Geant4 and ROOT environments and run the following
way, from the source folder

Interactive run:
    basicSiliconPixel macros/Fe55.mac

Batch run : 
    basicSilionPixel macros/Fe55.mac batch
