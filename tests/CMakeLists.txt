include_directories(${PROJECT_SOURCE_DIR}/include
  ${TFEL_INCLUDE_PATH}
  SYSTEM ${HDF5_INCLUDE_DIRS})
link_directories(${PROJECT_BINARY_DIR}/src
  ${TFEL_LIBRARY_PATH}
  ${HDF5_LIBRARY_DIRS})

# a simple macro
macro(madnex_test test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(${test_arg} ${test_arg} ${ARGN})
  add_dependencies(check ${test_arg})
  target_link_libraries(${test_arg} Madnex TFELTests)
endmacro(madnex_test)

madnex_test(CompType  ${PROJECT_SOURCE_DIR}/data/reference.madnex)
madnex_test(DataSetWithString)
madnex_test(CompoundDataSetWithString)
madnex_test(CompoundDataSetWithString2)
madnex_test(MadnexPOD)
madnex_test(MadnexCompoundExtractor)
madnex_test(MadnexReferenceFile ${PROJECT_SOURCE_DIR}/data/reference.madnex)
madnex_test(MadnexExperimentalConditions)
madnex_test(MadnexScripts)

set_directory_properties(PROPERTIES
  ADDITIONAL_MAKE_CLEAN_FILES "CompType.xml;DataSetWithString.xml;DataSetWithString.madnex;CompoundDataSetWithString.xml;CompoundDataSetWithString.madnex;CompoundDataSetWithString2.xml;CompoundDataSetWithString2.madnex;MadnexPOD.madnex;MadnexPOD.xml;MadnexCompoundExtractor.madnex;MadnexCompoundExtractor.xml;MadnexReferenceFile.xml;MadnexExperimentalConditions.xml;MadnexExperimentalConditions.madnex;MadnexScripts.xml;MadnexScripts.madnex")
