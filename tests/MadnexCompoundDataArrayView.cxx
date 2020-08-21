/*!
 * \file   MadnexCompoundDataArrayView.cxx
 * \brief
 * \author Helfer Thomas
 * \date   30 janv. 2017
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
#include "Madnex/Types.hxx"
#include "Madnex/FCString.hxx"
#include "Madnex/CompoundDataArrayView.hxx"

int main(const int argc, const char* const* argv) {
  using namespace madnex;
  if (argc != 2) {
    std::cerr << "MadnexCompoundDataArrayView: invalid number of arguments\n"
              << "usage: MadnexCompoundDataArrayView reference.madnex\n";
    return EXIT_FAILURE;
  }
  const auto f = File(argv[1], H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  const auto d = openDataSet(r, "resultats/resultats_principaux");
  const auto v = CompoundDataArrayView(d);
  // // main results
  const auto l = v.extract<fcstring<32>>("intitule");
  assert(l.size() == 3u);
  assert(l[0] == "P1");
  assert(l[1] == "P2");
  assert(l[2] == "P3");
  return EXIT_SUCCESS;
}
