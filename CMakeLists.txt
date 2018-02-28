project(jadepix)
cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

#Set defult version
set(JADEPIX_VERSION "v0.1")

#Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo")
endif()

#Print version
message(STATUS "Building JADEPIX version ${JADEPIX_VERSION}")

#################################
# Setup the build environment 
#################################

#Enable cxx as project language to perform compuler checks:
enable_language(CXX)

#Additional packages to be searched for by cmake
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#Configure the installation prefix to allow both local as system-wide installation
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" CACHE PATH "Prefix prepended to install directories" FORCE)
endif()


#################################
# Define build flags for allpix 
#################################

#Set standard build flags
set(COMPILER_FLAGS -pedantic -Wall -Wextra)

#Required a C++14 compiler but do allow extensions
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_SKIP_BUILD_RPATG FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RRPATH FALSE)
set(CMAKE_INSTALL_RPATG_USE_LINK_PATH TRUE)

#################################
# Prerequisistes for jadepix 
#################################

#Define the libraries
set(JADEPIX_LIBRARIES "")

#ROOT is required for vector and persistency etc
option(WITH_BUILD_ROOT "Build with ROOT" ON)
if(WITH_BUILD_ROOT)
    find_package(ROOT REQUIRED)
    if(NOT ROOT_FOUND)
        message(FATAL_ERROR "Could not find ROOT, make sure to source the ROOT environment\n"
            "$ source YOUR_ROOT_DIR/bin/thisroot.sh")
    endif()

    #Check ROOT version
    if(NOT ${ROOT_VERSION} VERSION_GREATER "6.0")
        message(FATAL_ERROR "ROOT versions below 6.0 are not supported")
    endif()
endif()

#CLHEP is required 
option(WITH_EXTERNAL_CLHEP "Build with external CLHEP" OFF)
if(WITH_EXTERNAL_CLHEP)
    find_package(CLHEP REQUIRED)
    if(NOT CLHEP_FOUND)
        message(FATAL_ERROR "Could not find CLHEP, make sure to install CLHEP environment\n"
            "check with clhep-config")
    endif()
endif()

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#Set the dependencies
set(JADEPIX_DEPS_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${CLHEP_INCLUDE_DIRS})

if(WITH_EXTERNAL_CLHEP)
    set(JADEPIX_DEPS_LIBRARIES ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES} ${CLHEP_LIBRARIES} ${Geant4_LIBRARIES})
elseif(WITH_BUILD_ROOT)
    set(JADEPIX_DEPS_LIBRARIES ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES} ${Geant4_LIBRARIES})
else()
    set(JADEPIX_DEPS_LIBRARIES ${Geant4_LIBRARIES})
endif()

#################################
# Define build targets for jadepix
#################################

# Set the models directory and provide install if necessary
#add_subdirectory(models)
#add_definitions(-DJADEPIX_MODEL_DIRECTORY="${MODEL_DIRECTORY}" -DJADEPIX_MODEL_SUFFIX="${MODEL_SUFFIX}")

add_subdirectory(software)

