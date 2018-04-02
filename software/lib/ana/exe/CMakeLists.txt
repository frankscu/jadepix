include_directories(../core/include)

add_executable(OptionTest src/OptionTest.cxx)
target_link_libraries(OptionTest ${JADE_CORE_LIBRARY})
list(APPEND INSTALL_TARGETS OptionTest)

add_executable(SystemTest src/SystemTest.cxx)
target_link_libraries(SystemTest ${JADE_CORE_LIBRARY})
list(APPEND INSTALL_TARGETS SystemTest)

add_executable(RunTest src/RunTest.cxx)
target_link_libraries(RunTest ${JADE_CORE_LIBRARY})
list(APPEND INSTALL_TARGETS RunTest)

install(TARGETS ${INSTALL_TARGETS}
  DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

