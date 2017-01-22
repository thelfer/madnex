include_directories(${PROJECT_SOURCE_DIR}/include
  ${TFEL_INCLUDE_PATH}
  SYSTEM ${HDF5_INCLUDE_DIRS})

link_directories(
  ${TFEL_LIBRARY_PATH}
  ${HDF5_LIBRARY_DIRS})

madnex_library(Madnex
  HDF5.cxx
  File.cxx
  Version.cxx
  CompoundExtractor.cxx
  ExperimentalConditions.cxx
  Scripts.cxx
  MainResults.cxx
  Exceptions.cxx)
target_link_libraries(Madnex
  TFELException
  ${HDF5_LIBRARIES})
