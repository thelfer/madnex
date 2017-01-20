# Remove when CMake >= 2.8.4 is required
set(CMAKE_LEGACY_CYGWIN_WIN32 0) 
cmake_minimum_required(VERSION 2.4)
cmake_policy(SET CMP0005 OLD)
if(CMAKE_VERSION VERSION_GREATER 2.8.12)
  cmake_policy(SET CMP0026 OLD)
endif(CMAKE_VERSION VERSION_GREATER 2.8.12)
if(CMAKE_VERSION VERSION_GREATER 3.1.0)
  cmake_policy(SET CMP0054 NEW)
endif(CMAKE_VERSION VERSION_GREATER 3.1.0)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
# madnex specific macros
include(cmake/modules/madnex.cmake)
# some variables for compatibility with autotools
include(cmake/modules/autotools.cmake)

#major and minor version
madnex_project(2 0 "0-dev")
set(PACKAGE_BUGREPORT "françois.curtit@edf.fr")

#tfel
include(cmake/modules/tfel.cmake)

#hdf5
find_package(HDF5 COMPONENTS CXX)
if(HDF5_FOUND)
  MESSAGE(STATUS "hdf5 headers     : " ${HDF5_INCLUDE_DIRS})
  MESSAGE(STATUS "hdf5 librairies  : " ${HDF5_LIBRARY_DIRS})
  MESSAGE(STATUS "hdf5 definitions : " ${HDF5_DEFINITIONS})
else(HDF5_FOUND)
  message(FATAL_ERROR "hdf5 not found")
endif(HDF5_FOUND)
add_definitions(${HDF5_DEFINITIONS})
foreach(hdf5_definition ${HDF5_DEFINITIONS})
  set(HDF5_INCLUDES "${HDF5_INCLUDES} ${hdf5_definition} ")
endforeach(hdf5_definition ${HDF5_DEFINITIONS})
set(HDF5_INCLUDES "${HDF5_INCLUDES} -I${HDF5_INCLUDE_DIRS}")

# adding support for rpath on MacOs X
if(APPLE)
   set(CMAKE_MACOSX_RPATH ON)
   set(CMAKE_SKIP_BUILD_RPATH FALSE)
   set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
   set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
   set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
   list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
   if("${isSystemDir}" STREQUAL "-1")
      set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
   endif()
endif()

# portable-build
option(enable-portable-build "produce binary that can be shared between various machine (same architecture, same gcc version, different processors" OFF)

#compiler options
include(cmake/modules/compiler.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
  include(cmake/modules/CodeCoverage.cmake)
  SETUP_TARGET_FOR_COVERAGE(coverage ctest coverage)
endif(CMAKE_BUILD_TYPE STREQUAL "Coverage")

#python
option(enable-python-bindings "enable  python bindings support" OFF)
if(enable-python-bindings)
  find_package(PythonLibs   REQUIRED)
  find_package(PythonInterp REQUIRED)
  set(HAVE_PYTHON ON)
  add_definitions("-DHAVE_PYTHON=1")
  add_definitions("-DMADNEX_PYTHON_INCLUDES=\\\"\"-I${PYTHON_INCLUDE_DIRS}\"\\\"")
  get_filename_component(PYTHON_LIBRARY_PATH ${PYTHON_LIBRARIES} PATH)
  get_filename_component(PYTHON_LIBRARY_FULL ${PYTHON_LIBRARIES} NAME)
  string(REGEX REPLACE "lib(.+)\\.(.+)$" "\\1" PYTHON_LIBRARY ${PYTHON_LIBRARY_FULL})
  add_definitions("-DMADNEX_PYTHON_LIBS=\\\"\"-L${PYTHON_LIBRARY_PATH} -l${PYTHON_LIBRARY}\"\\\"")
  message(STATUS "found python          ${PYTHON_VERSION}")
  message(STATUS "python include path   ${PYTHON_INCLUDE_DIRS}")
  message(STATUS "python libraries path ${PYTHON_LIBRARY_PATH}")
  message(STATUS "python library        ${PYTHON_LIBRARY}")
  if(NOT HAVE_PYTHON)
    message(FATAL_ERROR "python not found. python is mandatory for python bindings.")
  endif(NOT HAVE_PYTHON)
  find_package(Boost 1.36.0 COMPONENTS python)
  if(Boost_FOUND)
    get_filename_component(Boost_PYTHON_LIBRARY ${Boost_PYTHON_LIBRARY} NAME)
    message(STATUS "Boost library python found (${Boost_PYTHON_LIBRARY}).")
  else(Boost_FOUND)
    message(FATAL_ERROR "Boost libraries not found.\n"
      "Required packages are :" ${BOOST_MANDATORY_COMPONENTS})
  endif(Boost_FOUND)
  list(APPEND CPACK_COMPONENTS_ALL python_bindings)
  set(CPACK_COMPONENT_PYTHON_BINDINGS_DESCRIPTION 
    "Contains the python bindings for MADNEX, MFront and MTest")
  set(CPACK_COMPONENT_PYTHON_BINDINGS_DEPENDS  core mfront mtest)
endif(enable-python-bindings)

#documentations
add_custom_target(doc)

# add a target to generate API documentation with Doxygen
option(enable-doxygen-doc "enable generation of the Doxygen documentation" OFF)
if(enable-doxygen-doc)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
      ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    add_custom_target(doc-html
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM)
    add_dependencies(doc doc-html)
    list(APPEND CPACK_COMPONENTS_ALL doxygen)
    set(CPACK_COMPONENT_DOXYGEN_DESCRIPTION
      "Contains the TFEL doxygen documentation")
    set(GENERATE_DOXYGEN ON)
  else(DOXYGEN_FOUND)
    message(FATAL_ERROR "doxygen is required")
  endif(DOXYGEN_FOUND)
else(enable-doxygen-doc)
  message(STATUS "doxygen documentation is not enabled")
endif(enable-doxygen-doc)

# testing
set(CTEST_CONFIGURATION_TYPE "${JOB_BUILD_CONFIGURATION}")
# (must be placed *before* any add_subdirectory, cmake bug ?)
enable_testing()
if(CMAKE_CONFIGURATION_TYPES)
  add_custom_target(check COMMAND 
    ${CMAKE_CTEST_COMMAND} -T test -C $<CONFIGURATION>)
else(CMAKE_CONFIGURATION_TYPES)
  add_custom_target(check COMMAND 
    ${CMAKE_CTEST_COMMAND} -T test )
endif(CMAKE_CONFIGURATION_TYPES)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(docs)
add_subdirectory(tests)

