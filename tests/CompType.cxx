/*!
 * \file   CompType.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   19/01/2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <string>
#include <cstdlib>
#include <cassert>
#include <iterator>
#include <iostream>
#include <algorithm>

#include "Madnex/HDF5.hxx"
#include "Madnex/File.hxx"
#include "Madnex/Version.hxx"
#include "Madnex/ExperimentalConditions.hxx"

int main(const int argc, const char* const* const argv) {
  using namespace madnex;
  if (argc != 2) {
    std::cerr << "CompType: invalid number of arguments\n"
              << "usage: CompType reference.madnex\n";
    return EXIT_FAILURE;
  }
  const auto f = File(argv[1], H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  const auto d = openDataSet(r, "version");
  H5::CompType c(d);
  assert(c.getNmembers());
  assert(c.getMemberName(0) == "MAJ");
  assert(c.getMemberName(1) == "MIN");
  assert(c.getMemberName(2) == "REL");
  return EXIT_SUCCESS;
}
