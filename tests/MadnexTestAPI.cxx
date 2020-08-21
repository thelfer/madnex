/*!
 * \file   MadnexTestAPI.cxx
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
#include "Madnex/MainResults.hxx"

int main(const int argc, const char* const* argv) {
  using namespace madnex;
  if (argc != 2) {
    std::cerr << "MadnexTestAPI: invalid number of arguments\n"
              << "usage: MadnexTestAPI reference.madnex\n";
    return EXIT_FAILURE;
  }
  const auto f = File(argv[1], H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  const auto g = openGroup(r, "resultats");
  // // main results
  auto mr = read<MainResults>(g, "resultats_principaux");
  assert(mr.size() == 3u);
  assert(mr[0].name == "P1");
  assert(mr[0].label == "param 1");
  assert(mr[0].value == "12");
  assert(mr[0].unit == "");
  assert(mr[0].type == "int32");
  assert(mr[1].name == "P2");
  assert(mr[1].label == "param 2");
  assert(mr[1].value == "0.75");
  assert(mr[1].unit == "mm");
  assert(mr[1].type == "float64");
  assert(mr[2].name == "P3");
  assert(mr[2].label == "param 3");
  assert(mr[2].value == "test");
  assert(mr[2].unit == "");
  assert(mr[2].type == "XXS string");
  return EXIT_SUCCESS;
}
