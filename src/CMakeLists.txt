madnex_library(Madnex
  GetInstallPath.cxx
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
  Exceptions.cxx
  MFrontMetaData.cxx
  MFrontParameters.cxx
  MFrontImplementation.cxx
  MTestTest.cxx
  MTestTestMetaData.cxx
  MFMTestGeneratorTest.cxx
  MFMTestGeneratorTestMetaData.cxx
  DataBase.cxx)
target_include_directories(Madnex
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_compile_definitions(Madnex
   PUBLIC ${HDF5_DEFINITIONS})
target_include_directories(Madnex
   SYSTEM  PUBLIC ${HDF5_INCLUDE_DIRS})
target_link_libraries(Madnex
   PUBLIC ${HDF5_LIBRARIES})
