/*!
 * \file   tests/MadnexMFrontImplementation.cxx
 * \brief  regression test for issue #14: non-finite parameter values
 * (written as empty strings by the library) must not make the reader
 * throw.
 * \author Thomas Helfer
 * \date   06/11/2026
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <limits>
#include <string>

#include "Madnex/File.hxx"
#include "Madnex/MFrontImplementation.hxx"

static void write_file() {
  using namespace madnex;
  auto f = File("MadnexMFrontImplementation.madnex", H5F_ACC_TRUNC);
  auto r = f.getRoot();
  auto g = createGroup(r, "MFront");
  auto m = MFrontImplementation{};
  m.name = "Norton";
  m.source = "@use behaviours/Norton.mfront";
  m.parameters = {
      {"a", 1.5},
      {"b", std::numeric_limits<double>::quiet_NaN()},
      {"c", std::numeric_limits<double>::infinity()}};
  m.lower_bounds = {{"T", 293.15}};
  m.upper_bounds = {{"T", std::numeric_limits<double>::infinity()}};
  write(g, m);
}

static void read_file() {
  using namespace madnex;
  const auto f = File("MadnexMFrontImplementation.madnex", H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  const auto g = openGroup(r, "MFront");
  auto m = MFrontImplementation{};
  // the file written above stores the non-finite values of the `b` and
  // `c` parameters as empty strings: reading it used to raise
  // `std::invalid_argument` (regression test for issue #14)
  read(m, g, "Norton");
  assert(m.name == "Norton");
  assert(m.parameters.count("a") == 1u);
  assert(m.parameters.at("a") == 1.5);
  // non-finite values are skipped on read, as the bounds are
  assert(m.parameters.count("b") == 0u);
  assert(m.parameters.count("c") == 0u);
  assert(m.lower_bounds.count("T") == 1u);
  assert(m.lower_bounds.at("T") == 293.15);
  // the infinite upper bound is written as an empty string and skipped
  assert(m.upper_bounds.count("T") == 0u);
}

int main() {
  try {
    write_file();
    read_file();
  } catch (const std::exception& e) {
    std::cerr << "MadnexMFrontImplementation: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
