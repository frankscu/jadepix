# Include dependencies
include_directories(SYSTEM ${JADEPIX_DEPS_INCLUDE_DIRS})

include_directories(../lib/recon/include)
include_directories(../lib/sim/include)

include(${Geant4_USE_FILE})
file(GLOB headers ../lib/sim/include/*.hh)
file(GLOB source ../lib/sim/src/*.cc)

# Create executable 
# Geant4 cannot link the Lib directely
add_executable(jadepix src/JadePix.cc ${source} ${headers})
target_link_libraries(jadepix ${JADEPIX_RECON_LIBRARY} ${JADEPIX_DEPS_LIBRARIES})

list(APPEND INSTALL_TARGETS jadepix)

# Set install location
install(TARGETS 
    ${INSTALL_TARGETS} 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
