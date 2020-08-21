/*!
 * \file   tests/MadnexCompoundDataView.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/01/2017
 */

#include <iostream>
#include "Madnex/CompoundDataView.hxx"

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

static void write_file() {
  using namespace madnex;
  auto f = File("MadnexCompoundDataView.madnex", H5F_ACC_TRUNC);
  auto r = f.getRoot();
  struct CompoundData {
    std::int32_t integer_value;
    float float_value;
  };
  CompoundData cdata = {12, 3.14156f};
  H5::CompType c(sizeof(CompoundData));
  c.insertMember("integer_value", HOFFSET(CompoundData, integer_value),
                 getNativeType<std::int32_t>());
  c.insertMember("float_value", HOFFSET(CompoundData, float_value),
                 getNativeType<float>());
  hsize_t dimsf[1] = {1u};
  DataSpace s(1, dimsf);
  auto d = r.createDataSet("test", c, s);
  d.write(&cdata, c);
}

static void read_file() {
  using namespace madnex;
  constexpr const auto eps = std::numeric_limits<float>::epsilon();
  const auto f = File("MadnexCompoundDataView.madnex", H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  CompoundDataView e(openDataSet(r, "test"));
  assert(e.extract<std::int32_t>("integer_value") == 12);
  assert(std::abs(e.extract<float>("float_value") - 3.14156f) < eps);
}

static void read_file2(const std::string& fileName) {
  using namespace madnex;
  const auto f = File(fileName, H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  CompoundDataView e(openDataSet(r, "version"));
  assert(e.extract<std::string>("MAJ") == "1");
  assert(e.extract<std::string>("MIN") == "2");
  assert(e.extract<std::string>("REL") == "1");
}

int main(const int argc, const char* const* const argv) {
  if (argc != 2) {
    std::cerr << "MadnexCompoundDataView: invalid number of arguments\n"
              << "usage: MadnexCompoundDataView reference.madnex\n";
    return EXIT_FAILURE;
  }
  write_file();
  read_file();
  read_file2(argv[1]);
  return EXIT_SUCCESS;
}
