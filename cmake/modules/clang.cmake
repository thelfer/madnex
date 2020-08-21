madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Weverything")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-padded")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-documentation")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-documentation-unknown-command")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-exit-time-destructors")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-global-constructors")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-missing-braces")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wrange-loop-analysis")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wmove")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Winfinite-recursion")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wcomma")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wmicrosoft")
madnex_enable_cxx_compiler_flag(COMPILER_WARNINGS  "Wno-inconsistent-missing-destructor-override")
madnex_enable_cxx_compiler_flag2(COMPILER_WARNINGS "Wno-c++98-compat" "Wno_c__98_compat_AVAILABLE")
include(cmake/modules/common-compiler-flags.cmake)

madnex_enable_cxx_compiler_flag(OPTIMISATION_FLAGS_MARCH "march=native")
if(enable-fast-math)
  madnex_enable_cxx_compiler_flag(OPTIMISATION_FLAGS  "ffast-math")
else(enable-fast-math)
  madnex_enable_cxx_compiler_flag(OPTIMISATION_FLAGS2 "ffast-math")
endif(enable-fast-math)

madnex_enable_cxx_compiler_flag(VISIBILITY_FLAGS "fvisibility=hidden")
madnex_enable_cxx_compiler_flag(VISIBILITY_FLAGS "fvisibility-inlines-hidden")

set(OPTIMISATION_FLAGS "-DNO_RUNTIME_CHECK_BOUNDS ${OPTIMISATION_FLAGS}")

if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
  set(OPTIMISATION_FLAGS "-O2 -DNDEBUG ${OPTIMISATION_FLAGS}")
endif((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions("-g")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if(HAVE_FORTRAN)
  # we associate clang with the gnu fortran compiler
  include(cmake/modules/gnu-fortran-compiler.cmake)
endif(HAVE_FORTRAN)

option(enable-libcxx "use LLVM C++ Standard library" OFF)
if(enable-libcxx)
  madnex_enable_cxx_compiler_flag(COMPILER_CXXFLAGS "stdlib=libc++")
endif(enable-libcxx)

if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.4)
  message(FATAL_ERROR "MADNEX C++11 support is only available for clang version >= 3.4")
endif(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.4)
set(COMPILER_CXXFLAGS "${COMPILER_CXXFLAGS} -std=c++11")

option(enable-sanitize-options "enable various clang sanitize options (undefined, address,...)" OFF)

if(enable-sanitize-options)
  madnex_enable_cxx_compiler_flag(COMPILER_FLAGS "fsanitize=address")
  madnex_enable_cxx_compiler_flag(COMPILER_FLAGS "fsanitize=thread")
  madnex_enable_cxx_compiler_flag(COMPILER_FLAGS "fsanitize=memory")
  # madnex_enable_cxx_compiler_flag(COMPILER_FLAGS "fsanitize=undefined")
  madnex_enable_cxx_compiler_flag(COMPILER_FLAGS "fsanitize=cfi")
  madnex_enable_cxx_compiler_flag(COMPILER_FLAGS "fsanitize=safe-stack")
endif(enable-sanitize-options)

