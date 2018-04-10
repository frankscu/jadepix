project(jadepixana)
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
include(CMakeDependentOption)

set(CMAKE_CXX_STANDARD 14)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(JADE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR} CACHE PATH "Prefix prepended to install directories")
set(CMAKE_INSTALL_PREFIX ${JADE_INSTALL_PREFIX} CACHE INTERNAL "Prefix of install directories" FORCE)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_INSTALL_RPATH "@loader_path/../lib")
else()
  set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib")
endif()
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package(ROOT REQUIRED)
include_directories(include)
include_directories(${ROOT_INCLUDE_DIRS})
set(USEROOT true)
if(USEROOT)
  execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE) 
  execute_process(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${ROOT_LD_FLAGS}")
endif(USEROOT)

FIND_PACKAGE(Eigen3 REQUIRED)
INCLUDE_DIRECTORIES(SYSTEM ${EIGEN3_INCLUDE_DIR})

set(EFIELD_SRC src/EField.cc src/EFieldTest.cc)
add_executable(efield ${EFIELD_SRC})
target_link_libraries(efield ${ROOT_LIBRIRIES})

set(EFGen_SRC src/EFReadDFISE.cc src/EFGen.cc)
add_executable(efgen ${EFGen_SRC})
target_link_libraries(efgen ${ROOT_LIBRIRIES})

install(TARGETS efield efgen
    DESTINATION bin)
