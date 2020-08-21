macro(madnex_project madnex_version_major madnex_version_minor madnex_version_patch)
  project("madnex")
  set(PACKAGE_NAME "madnex")
  set(VERSION "${madnex_version_major}.${madnex_version_minor}.${madnex_version_patch}")
  set(MADNEX_VERSION "${VERSION}")
  set(MADNEX_WEBSITE "http://madnex.sourceforce.net")
  # the version number.
  set(MADNEX_VERSION_MAJOR "${madnex_version_major}")
  set(MADNEX_VERSION_MINOR "${madnex_version_minor}")
  set(MADNEX_VERSION_PATCH "${madnex_version_patch}")
  add_definitions("-DVERSION=\\\"\"${MADNEX_VERSION}\"\\\"")
  if(LIB_SUFFIX)
    add_definitions("-DLIB_SUFFIX=\\\"\"${LIB_SUFFIX}\"\\\"")
  endif(LIB_SUFFIX)
endmacro(madnex_project)

set(CPACK_COMPONENTS_ALL core)
set(CPACK_COMPONENT_CORE_DESCRIPTION 
  "Contains all the core libraries developped within MADNEX")

macro(install_header dir file)
  install(FILES ${dir}/${file}
          DESTINATION "include/${dir}"
          COMPONENT core)
endmacro(install_header)

macro(install_data dir file)
  install(FILES ${file}
          DESTINATION "share/madnex/${dir}"
          COMPONENT core)
endmacro(install_data)

macro(madnex_library name)
  if(${ARGC} LESS 2)
    message(FATAL_ERROR "madnex_library_internal : no source specified")
  endif(${ARGC} LESS 2)
  add_library(${name} SHARED ${ARGN})
  if(WIN32)
    install(TARGETS ${name} EXPORT ${name}
            DESTINATION bin
            COMPONENT core)
  else(WIN32)
    install(TARGETS ${name} EXPORT ${name}
            DESTINATION lib${LIB_SUFFIX}
            COMPONENT core)
  endif(WIN32)
  if(MADNEX_APPEND_SUFFIX)
    set(export_install_path "share/madnex-${MADNEX_SUFFIX}/cmake")
  else(MADNEX_APPEND_SUFFIX)
    set(export_install_path "share/madnex/cmake")
  endif(MADNEX_APPEND_SUFFIX)
  install(EXPORT ${name} DESTINATION ${export_install_path}
          NAMESPACE madnex:: FILE ${name}Config.cmake)
  if(enable-static)
    add_library(${name}-static STATIC ${ARGN})
    set_target_properties(${name}-static PROPERTIES OUTPUT_NAME "${name}")
    # Now the library target "${name}-static" will be named "${name}.lib"
    # with MS tools.
    # This conflicts with the "${name}.lib" import library corresponding
    # to "${name}.dll",
    # so we add a "lib" prefix (which is default on other platforms
    # anyway):
    set_target_properties(${name}-static PROPERTIES PREFIX "lib")
    # Help CMake 2.6.x and lower (not necessary for 2.8 and above, but
    # doesn't hurt):
    set_target_properties(${name}        PROPERTIES CLEAN_DIRECT_OUTPUT 1)
    set_target_properties(${name}-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
    set_target_properties(${name}-static PROPERTIES COMPILE_FLAGS "-D${name}_EXPORTS -DMADNEX_STATIC_BUILD")
    if(WIN32)
      install(TARGETS ${name}-static DESTINATION bin)
    else(WIN32)
      install(TARGETS ${name}-static DESTINATION lib${LIB_SUFFIX})
    endif(WIN32)
    install(EXPORT ${name}-static DESTINATION ${export_install_path}
            NAMESPACE madnex:: FILE ${name}Config.cmake)
  endif(enable-static)
endmacro(madnex_library)

macro(python_module_base fullname name)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "python_lib_module : no source specified")
  endif(${ARGC} LESS 1)
  add_library(py_${fullname} MODULE ${ARGN})
  target_include_directories(py_${fullname}
    SYSTEM
    PRIVATE "${Boost_INCLUDE_DIRS}"
    PRIVATE "${PYTHON_INCLUDE_DIRS}")
  if(WIN32)
    set_target_properties(py_${fullname} PROPERTIES
      COMPILE_FLAGS "-DHAVE_ROUND")
    set_target_properties(py_${fullname} PROPERTIES SUFFIX ".pyd")
  elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(py_${fullname} PROPERTIES SUFFIX ".so")
  endif(WIN32)
  set_target_properties(py_${fullname} PROPERTIES PREFIX "")
  set_target_properties(py_${fullname} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(py_${fullname}
    ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARIES})
endmacro(python_module_base)

macro(python_module name)
  python_module_base(${name} ${name} ${ARGN})
  install(TARGETS py_${name}
    DESTINATION lib${LIB_SUFFIX}/${PYTHON_LIBRARY}/site-packages/
    COMPONENT python_bindings)
endmacro(python_module)

macro(python_lib_module name package)
  python_module_base(${package}_${name} ${name} ${ARGN})
  install(TARGETS py_${package}_${name}
    DESTINATION lib${LIB_SUFFIX}/${PYTHON_LIBRARY}/site-packages/${package}
    COMPONENT python_bindings)
endmacro(python_lib_module)

macro(madnex_python_module name)
  python_lib_module(${name} madnex ${ARGN})
endmacro(madnex_python_module)

macro(madnex_python_script dir)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "madnex_python_script : no script specified")
  endif(${ARGC} LESS 1)
  foreach(pyscript ${ARGN})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
      configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${pyscript}" @ONLY)
      set(python_script "${CMAKE_CURRENT_BINARY_DIR}/${pyscript}")
    else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
      set(python_script "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}")
    endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${pyscript}.in")
    install(PROGRAMS ${python_script}
      DESTINATION lib${LIB_SUFFIX}/${PYTHON_LIBRARY}/site-packages/${dir}/
      COMPONENT python_bindings)
  endforeach(pyscript ${ARGN})
endmacro(madnex_python_script)
