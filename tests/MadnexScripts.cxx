/*!
 * \file   tests/MadnexScripts.cxx
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
#include "Madnex/Scripts.hxx"

static void write_file() {
  using namespace madnex;
  auto f = File("MadnexScripts.madnex", H5F_ACC_TRUNC);
  auto r = f.getRoot();
  Script ec;
  ec.author = "François Curtit";
  ec.date = "17/01/2017";
  ec.command_line = "python my_script.py";
  write(r, "scripts", std::vector<Script>(1, ec));
}

static void read_file() {
  using namespace madnex;
  const auto f = File("MadnexScripts.madnex", H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  const auto ecs = read<Scripts>(r, "scripts");
  assert(ecs.size() == 1);
  const auto& ec = ecs[0];
  assert(ec.author == "François Curtit");
  assert(ec.date == "17/01/2017");
  assert(ec.command_line == "python my_script.py");
  assert(ec.script_version == "");
}

int main() {
  write_file();
  read_file();
  return EXIT_SUCCESS;
}
