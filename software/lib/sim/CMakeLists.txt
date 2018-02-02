# Include the dependencies
include_directories(SYSTEM ${JADEPIX_DEPS_INCLUDE_DIRS})

# Add Geant4 flags before our own flags
add_definitions(${Geant4_DEFINITIONS})
set(${CMAKE_CXX_FLAGS} "${Geant4_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(${CMAKE_CXX_FLAGS} "${Geant4_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS}")
elseif(CMAKE_BUILD_TYPE MATCHES RELEASE)
    set(${CMAKE_CXX_FLAGS} "${Geant4_CXX_FLAGS_RLEASE} ${CMAKE_CXX_FLAGS}")
endif()

include_directories(include)

# Not use Geant4_USE_FILE
include_directories(SYSTEM PRIVATE ${Geant4_INCLUDE_DIRS})

add_definitions("-DJADEPIX_SIM_DLL_EXPORT")

aux_source_directory(src JADEPIX_SIM_SRC)

set(JADEPIX_SIM_LIBRARY jadesim)
add_library(${JADEPIX_SIM_LIBRARY} ${JADEPIX_SIM_SRC})

# Set install location
install(TARGETS 
    ${JADEPIX_SIM_LIBRARY} 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
