/*!
 * \file   MadnexWriteCompoundData.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   31 janv. 2017
 */

/*!
 * \file   tests/MadnexWriteCompoundData.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17 janv. 2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cmath>
#include <limits>
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "Madnex/HDF5.hxx"
#include "Madnex/File.hxx"

static void write_file() {
  using namespace madnex;
  auto f = File("MadnexWriteCompoundData.madnex", H5F_ACC_TRUNC);
  auto r = f.getRoot();
  // writeCompoundData(r,"compound data",
  // 		      12.,std::string("double value"));
  // fcstring<40>("fcstring value"),"fcstring value",
  // std::string("string value"),"string value");
}

static void read_file() {
  //   using namespace madnex;
  //   const auto f = File("MadnexWriteCompoundData.madnex",H5F_ACC_RDONLY);
  //   const auto r = f.getRoot();
  //   const auto ecs = read<WriteCompoundData>(r,"scripts");
  //   TFEL_TESTS_ASSERT(ecs.size()==1);
  //   const auto& ec = ecs[0];
  //   TFEL_TESTS_ASSERT(ec.author=="François Curtit");
  //   TFEL_TESTS_ASSERT(ec.date=="17/01/2017");
  //   TFEL_TESTS_ASSERT(ec.command_line=="python my_script.py");
  //   TFEL_TESTS_ASSERT(ec.script_version=="");
}

int main() {
  write_file();
  read_file();
  return EXIT_SUCCESS;
}
