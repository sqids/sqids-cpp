cmake_minimum_required(VERSION 3.14)

project(
  "sqids"
  VERSION 0.2.0
  DESCRIPTION
  "A small header-only library that lets you generate URL-friendly IDs from numbers."
  HOMEPAGE_URL "https://github.com/sqids/sqids-cpp")

include(GNUInstallDirs)

add_library(sqids INTERFACE)

target_include_directories(
  sqids INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/sqids>
                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(sqids INTERFACE cxx_std_11)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "sqidsConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(
  TARGETS sqids
  EXPORT ${PROJECT_NAME}_Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(
  EXPORT ${PROJECT_NAME}_Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sqids DESTINATION include)

enable_testing()

add_subdirectory(tests)

option(BUILD_DOCS "Build documentation" OFF)

if(BUILD_DOCS)
  find_package(Doxygen)

  if(DOXYGEN_FOUND)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
    add_custom_target(
      doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating Doxygen documentation"
      VERBATIM)
  else(DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate documentation")
  endif(DOXYGEN_FOUND)
endif(BUILD_DOCS)
