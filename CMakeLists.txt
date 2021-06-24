cmake_minimum_required(VERSION 3.10)
project(libfactory)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(
  factory
  STATIC
  ${PROJECT_SOURCE_DIR}/src/factory/ObjectFactory.h
  ${PROJECT_SOURCE_DIR}/src/factory/FunctionFactory.h
  )

set_target_properties(
  factory
  PROPERTIES
  LINKER_LANGUAGE
  CXX
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

