# Include the dependencies
include_directories(SYSTEM ${JADEPIX_DEPS_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(include)
add_definitions("-DJADEPIX_RECON_DLL_EXPORT")

aux_source_directory(src JADEPIX_RECON_SRC)

set(JADEPIX_RECON_LIBRARY jaderecon CACHE INTERNAL "JadePix reconstruction library")
add_library(${JADEPIX_RECON_LIBRARY} ${JADEPIX_RECON_SRC})


# Set install location
install(TARGETS 
    ${JADEPIX_RECON_LIBRARY} 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)


