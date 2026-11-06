/*!
 * \file   tests/MadnexExperimentalConditions.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/01/2017
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
#include "Madnex/ExperimentalConditions.hxx"

static void write_file() {
  using namespace madnex;
  auto f = File("MadnexExperimentalConditions.madnex", H5F_ACC_TRUNC);
  auto r = f.getRoot();
  ExperimentalCondition ec;
  ec.name = "Temperature";
  ec.value = "273.15";
  ec.unit = "K";
  write(r, "experimental_conditions",
        std::vector<ExperimentalCondition>(1, ec));
}

static void read_file() {
  using namespace madnex;
  const auto f = File("MadnexExperimentalConditions.madnex", H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  const auto ecs = read<ExperimentalConditions>(r, "experimental_conditions");
  assert(ecs.size() == 1);
  const auto& ec = ecs[0];
  assert(ec.name == "Temperature");
  assert(ec.value == "273.15");
  assert(ec.unit == "K");
  // `read` clears its output parameter: reading twice in the same
  // container does not duplicate the content (regression test for
  // issue #11)
  auto ecs2 = read<ExperimentalConditions>(r, "experimental_conditions");
  read(ecs2, r, "experimental_conditions");
  assert(ecs2.size() == 1u);
  assert(ecs2[0].name == "Temperature");
  assert(ecs2[0].value == "273.15");
  assert(ecs2[0].unit == "K");
}

int main() {
  write_file();
  read_file();
  return EXIT_SUCCESS;
}
