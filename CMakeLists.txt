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
set(PACKAGE_BUGREPORT "thomas.helfer@cea.fr;françois.curtit@edf.fr")

#hdf5
find_package(HDF5 COMPONENTS CXX)
if(HDF5_FOUND)
  MESSAGE(STATUS "hdf5 headers     : " ${HDF5_INCLUDE_DIRS})
  MESSAGE(STATUS "hdf5 librairies  : " ${HDF5_LIBRARIES})
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
option(enable-numpy-support
  "enable numpy support. This option has no effect if python bindings are disabled" ON)
option(enable-numpy-support
  "enable numpy support. This option has no effect if python bindings are disabled" ON)
option(enable-broken-boost-python-module-visibility-handling
  "workaround a bug in boost python which occurs on FreeBSD+clang. See https://github.com/boostorg/python/issues/173 for details" OFF)

option(python-static-interpreter-workaround
  "a workaround to handle the fact the conda' python interpreter is statically linked, which causes issues on Mac Os. See for details https://github.com/ContinuumIO/anaconda-issues/issues/9078. A proper solution may only appear with CMake 3.15 with the `Python::module` target, see https://gitlab.kitware.com/cmake/cmake/issues/18100. Until this version is widely available, this flag proposes a hack. Use with care." OFF)

if(enable-python-bindings)
# Python
  find_package(PythonLibs REQUIRED)
  set(HAVE_PYTHON ON)
  add_definitions("-DHAVE_PYTHON=1")
  add_definitions("-DMADNEX_PYTHON_INCLUDES=\\\"\"-I${PYTHON_INCLUDE_DIRS}\"\\\"")
  get_filename_component(PYTHON_LIBRARY_PATH ${PYTHON_LIBRARIES} PATH)
  get_filename_component(PYTHON_LIBRARY_FULL ${PYTHON_LIBRARIES} NAME)
  string(REGEX REPLACE "lib(.+)\\.(.+)$" "\\1" PYTHON_LIBRARY ${PYTHON_LIBRARY_FULL})
  message(STATUS "found python          ${PYTHONLIBS_VERSION_STRING}")
  message(STATUS "python include path   ${PYTHON_INCLUDE_DIRS}")
  message(STATUS "python libraries path ${PYTHON_LIBRARY_PATH}")
  message(STATUS "python library        ${PYTHON_LIBRARY}")
  string(REGEX REPLACE "[a-z]+.*$" "" PYTHONLIBS_VERSION_CLEANED ${PYTHONLIBS_VERSION_STRING})
  find_package(PythonInterp ${PYTHONLIBS_VERSION_CLEANED} REQUIRED)
  message(STATUS "python interpreter    ${PYTHON_EXECUTABLE}")
  if(enable-numpy-support AND PYTHON_EXECUTABLE AND (NOT NUMPY_INCLUDE_DIRS))
    execute_process(
      COMMAND "${PYTHON_EXECUTABLE}" -c
              "from __future__ import print_function\ntry: import numpy; print(numpy.get_include(), end='')\nexcept:pass\n"
              OUTPUT_VARIABLE NUMPY_INCLUDE_PATH)
      find_path(NUMPY_INCLUDE_DIRS numpy/arrayobject.h
                HINTS "${NUMPY_INCLUDE_PATH}" "${PYTHON_INCLUDE_PATH}" NO_DEFAULT_PATH)
      message(STATUS "Numpy header path     ${NUMPY_INCLUDE_DIRS}")
  endif(enable-numpy-support AND PYTHON_EXECUTABLE AND (NOT NUMPY_INCLUDE_DIRS))
# Boost/Python
  if(NOT HAVE_PYTHON)
    message(FATAL_ERROR "python not found. python is mandatory for python bindings.")
  endif(NOT HAVE_PYTHON)
  message(STATUS "Trying to find libboost_python-py${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}")
  # Fix a bug in FindBoost 1.70
  # https://github.com/boostorg/boost_install/issues/5
  if(NOT DEFINED BUILD_SHARED_LIBS)
    set(MADNEX_TMP_BUILD_SHARED_LIBS ON)
    set(BUILD_SHARED_LIBS ON)
  endif(NOT DEFINED BUILD_SHARED_LIBS)
  find_package(Boost 1.36.0 COMPONENTS
    "python-py${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}")
  if(Boost_PYTHON-PY${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_FOUND)
    set(Boost_PYTHON_FOUND ON)
    set(Boost_PYTHON_LIBRARY "${Boost_PYTHON-PY${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_LIBRARY}")
    if(enable-numpy-support)
      find_package(Boost 1.36.0 COMPONENTS
        "numpy-py${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}")
      if(Boost_NUMPY-PY${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_FOUND)
        set(Boost_NUMPY_FOUND ON)
        set(Boost_NUMPY_LIBRARY "${Boost_NUMPY-PY${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_LIBRARY}")
      endif(Boost_NUMPY-PY${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_FOUND)
    endif(enable-numpy-support)
  else(Boost_PYTHON-PY${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_FOUND)
    set(Boost_PYTHON_FOUND OFF)
  endif(Boost_PYTHON-PY${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_FOUND)
  if(NOT Boost_PYTHON_FOUND)
    message(STATUS "Trying to find libboost_python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}")
    find_package(Boost 1.36.0 COMPONENTS
      "python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}")
    if(Boost_PYTHON${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_FOUND)
      set(Boost_PYTHON_FOUND ON)
      set(Boost_PYTHON_LIBRARY "${Boost_PYTHON${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_LIBRARY}")
      if(enable-numpy-support)
        find_package(Boost 1.36.0 COMPONENTS
         "numpy${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}")
        if(Boost_NUMPY${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_FOUND)
          set(Boost_NUMPY_FOUND ON)
          set(Boost_NUMPY_LIBRARY "${Boost_NUMPY${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_LIBRARY}")
        endif(Boost_NUMPY${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_FOUND)
      endif(enable-numpy-support)
    else(Boost_PYTHON${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_FOUND)
      set(Boost_PYTHON_FOUND OFF)
    endif(Boost_PYTHON${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}_FOUND)
  endif(NOT Boost_PYTHON_FOUND)
  if(NOT Boost_PYTHON_FOUND)
    message(STATUS "Trying to find libboost_python-py${PYTHON_VERSION_MAJOR}")
    find_package(Boost 1.36.0 COMPONENTS
      "python-py${PYTHON_VERSION_MAJOR}")
    if(Boost_PYTHON-PY${PYTHON_VERSION_MAJOR}_FOUND)
      set(Boost_PYTHON_FOUND ON)
      set(Boost_PYTHON_LIBRARY "${Boost_PYTHON-PY${PYTHON_VERSION_MAJOR}_LIBRARY}")
      if(enable-numpy-support)
	find_package(Boost 1.36.0 COMPONENTS
	  "numpy-py${PYTHON_VERSION_MAJOR}")
	if(Boost_NUMPY-PY${PYTHON_VERSION_MAJOR}_FOUND)
          set(Boost_NUMPY_FOUND ON)
          set(Boost_NUMPY_LIBRARY "${Boost_NUMPY-PY${PYTHON_VERSION_MAJOR}_LIBRARY}")
	endif(Boost_NUMPY-PY${PYTHON_VERSION_MAJOR}_FOUND)
      endif(enable-numpy-support)
    else(Boost_PYTHON-PY${PYTHON_VERSION_MAJOR}_FOUND)
      set(Boost_PYTHON_FOUND OFF)
    endif(Boost_PYTHON-PY${PYTHON_VERSION_MAJOR}_FOUND)
  endif(NOT Boost_PYTHON_FOUND)
  if(NOT Boost_PYTHON_FOUND)
    message(STATUS "Trying to find libboost_python${PYTHON_VERSION_MAJOR}")
    find_package(Boost 1.36.0 COMPONENTS
      "python${PYTHON_VERSION_MAJOR}")
    if(Boost_PYTHON${PYTHON_VERSION_MAJOR}_FOUND)
      set(Boost_PYTHON_FOUND ON)
      set(Boost_PYTHON_LIBRARY "${Boost_PYTHON${PYTHON_VERSION_MAJOR}_LIBRARY}")
      if(enable-numpy-support)
        find_package(Boost 1.36.0 COMPONENTS
          "numpy${PYTHON_VERSION_MAJOR}")
        if(Boost_NUMPY${PYTHON_VERSION_MAJOR}_FOUND)
          set(Boost_NUMPY_FOUND ON)
          set(Boost_NUMPY_LIBRARY "${Boost_NUMPY${PYTHON_VERSION_MAJOR}_LIBRARY}")
        endif(Boost_NUMPY${PYTHON_VERSION_MAJOR}_FOUND)
      endif(enable-numpy-support)
    else(Boost_PYTHON${PYTHON_VERSION_MAJOR}_FOUND)
      set(Boost_PYTHON_FOUND OFF)
    endif(Boost_PYTHON${PYTHON_VERSION_MAJOR}_FOUND)
  endif(NOT Boost_PYTHON_FOUND)
  if(NOT Boost_PYTHON_FOUND)
    message(STATUS "Trying to find libboost_python")
    find_package(Boost 1.36.0 COMPONENTS python)
    if(Boost_PYTHON_FOUND)
      if(enable-numpy-support)
        find_package(Boost 1.36.0 COMPONENTS numpy)
        if(Boost_NUMPY_FOUND)
          message(STATUS "  numpy found ${Boost_NUMPY_LIBRARY}")
        else(Boost_NUMPY_FOUND)
          message(STATUS "  numpy *not* found")
        endif(Boost_NUMPY_FOUND)
        message(STATUS "Boost library python found (${Boost_PYTHON_LIBRARY}).")
      endif(enable-numpy-support)
    else(Boost_PYTHON_FOUND)
      message(FATAL_ERROR "Boost python libraries not found.\n"
	"Required packages are :" ${BOOST_MANDATORY_COMPONENTS})
    endif(Boost_PYTHON_FOUND)
  endif(NOT Boost_PYTHON_FOUND)
  if(Boost_NUMPY_FOUND)
    set(MADNEX_NUMPY_SUPPORT ON)
  endif(Boost_NUMPY_FOUND)
  if(MADNEX_USES_CONAN)
    if(NOT python-static-interpreter-workaround)
      set(MADNEX_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES
        ${CONAN_LIBS} ${PYTHON_LIBRARIES})
    else(python-static-interpreter-workaround)
      if(APPLE)
        set(MADNEX_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES
          ${CONAN_LIBS})
      else(APPLE)
        set(MADNEX_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES
          ${CONAN_LIBS} ${PYTHON_LIBRARIES})
      endif(APPLE)
    endif()
  else(MADNEX_USES_CONAN)
    if(NOT python-static-interpreter-workaround)
      set(MADNEX_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES
        ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARIES})
    else()
      if(APPLE)
        set(MADNEX_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES
          ${Boost_PYTHON_LIBRARY})
      else(APPLE)
        set(MADNEX_PYTHON_MODULES_PRIVATE_LINK_LIBRARIES
          ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARIES})
      endif(APPLE)
    endif()
  endif(MADNEX_USES_CONAN)
  if(DEFINED MADNEX_TMP_BUILD_SHARED_LIBS)
    unset(MADNEX_TMP_BUILD_SHARED_LIBS)
    unset(BUILD_SHARED_LIBS)
  endif(DEFINED MADNEX_TMP_BUILD_SHARED_LIBS)
  list(APPEND CPACK_COMPONENTS_ALL python_bindings)
  set(CPACK_COMPONENT_PYTHON_BINDINGS_DESCRIPTION 
    "Contains the python bindings for MADNEX, MFront and MTest")
  set(CPACK_COMPONENT_PYTHON_BINDINGS_DEPENDS  core mfront mtest)
  add_definitions("-DMADNEX_PYTHON_BINDINGS")
  if(enable-broken-boost-python-module-visibility-handling)
    add_definitions("-DBOOST_PYTHON_USE_GCC_SYMBOL_VISIBILITY")
  endif(enable-broken-boost-python-module-visibility-handling)
endif(enable-python-bindings)

#documentations
add_custom_target(doc)

# generation of the website
option(enable-website "enable generation of the website" ON)
if(enable-website)
  set(MADNEX_GENERATE_WEBSITE ON)
  # Looking for pandoc (http://johnmacfarlane.net/pandoc)
  include(cmake/modules/pandoc.cmake)
  if(MADNEX_HAVE_PANDOC)
    set(MADNEX_GENERATE_WEBSITE ON)
  endif(MADNEX_HAVE_PANDOC)
  if(MADNEX_GENERATE_WEBSITE)
    list(APPEND CPACK_COMPONENTS_ALL website)
    set(CPACK_COMPONENT_WEBSITE_DESCRIPTION
      "Contains the MADNEX web site")
  endif(MADNEX_GENERATE_WEBSITE)
else(enable-website)
  set(MADNEX_GENERATE_WEBSITE OFF)
  message(STATUS "Generation of the MADNEX website disabled")
endif(enable-website)

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
      "Contains the MADNEX doxygen documentation")
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
add_subdirectory(references)
add_subdirectory(tests)
add_subdirectory(bindings)
