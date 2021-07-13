cmake_minimum_required(VERSION 3.20)
project(libfactory)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FindPkgConfig)

add_library(
  factory
  SHARED
  ${PROJECT_SOURCE_DIR}/src/factory/ObjectFactory.h
  ${PROJECT_SOURCE_DIR}/src/factory/FunctionFactory.h
  )

set_target_properties(
  factory
  PROPERTIES
  LINKER_LANGUAGE
  CXX
  POSITION_INDEPENDENT_CODE
  ON
  )

target_include_directories(
  factory
  PUBLIC
  ${PROJECT_SOURCE_DIR}/src
  )

include(GNUInstallDirs)

install(
  FILES
  ${PROJECT_SOURCE_DIR}/src/factory/ObjectFactory.h
  DESTINATION
  ${CMAKE_INSTALL_INCLUDEDIR}/factory/
  )

install(
  FILES
  ${PROJECT_SOURCE_DIR}/src/factory/FunctionFactory.h
  DESTINATION
  ${CMAKE_INSTALL_INCLUDEDIR}/factory/
  )

install(
  TARGETS
  factory
  )

configure_file(
  ${PROJECT_SOURCE_DIR}/libfactory.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/libfactory.pc
  @ONLY
  )
install(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/libfactory.pc
  DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/pkgconfig/
  )
