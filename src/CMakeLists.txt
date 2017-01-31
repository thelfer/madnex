include_directories(${PROJECT_SOURCE_DIR}/include
  ${TFEL_INCLUDE_PATH}
  SYSTEM ${HDF5_INCLUDE_DIRS})

link_directories(
  ${TFEL_LIBRARY_PATH}
  ${HDF5_LIBRARY_DIRS})

madnex_library(Madnex
  Types.cxx
  HDF5.cxx
  File.cxx
  Version.cxx
  CompoundExtractor.cxx
  CompoundDataViewBase.cxx
  CompoundDataView.cxx
  CompoundDataArrayView.cxx
  ExperimentalConditions.cxx
  Scripts.cxx
  MainResults.cxx
  Exceptions.cxx)
target_link_libraries(Madnex
  TFELException
  TFELUtilities
  ${HDF5_LIBRARIES})
