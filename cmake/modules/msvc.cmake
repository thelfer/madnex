include(cmake/modules/OptimizeForArchitecture.cmake)

madnex_enable_cxx_compiler_flag(COMPILER_CXXFLAGS "MD")
madnex_enable_cxx_compiler_flag(COMPILER_CXXFLAGS "EHsc")

madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "W4")
#4251 : warning about export of STL classes
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4251")
#4710 : warning function not being inlined
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4710")
#4514 : warning inline function was not used and was removed
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4514")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4275")
#4250 : inheritance via dominance
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4250")
#4127 : conditional expression is constant
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4127")
#4244 : conversion
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4244")
#4996 : conversion
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "wd4996")

# remove a warning about deprecated symbol
set(COMPILER_FLAGS "-D_CRT_SECURE_NO_WARNINGS ${COMPILER_FLAGS}")
set(COMPILER_FLAGS "-D_SCL_SECURE_NO_WARNINGS ${COMPILER_FLAGS}")
set(OPTIMISATION_FLAGS "/O2 -DNDEBUG ${OPTIMISATION_FLAGS}")
set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if(NOT enable-fast-math)
  set(COMPILER_FLAGS "/fp:strict ${COMPILER_FLAGS}")
endif(NOT enable-fast-math)

# optimise build
OptimizeForArchitecture()