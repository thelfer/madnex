/*!
 * \file   src/MTestTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27/08/2020
 */

#include <set>
#include <cmath>
#include <limits>
#include <algorithm>
#include "Madnex/Raise.hxx"
#include "Madnex/File.hxx"
#include "Madnex/MTestTest.hxx"

namespace madnex {

  MTestTest::MTestTest() = default;

  MTestTest::MTestTest(MTestTest&&) = default;

  MTestTest::MTestTest(const MTestTest&) =
      default;

  MTestTest& MTestTest::operator=(
      MTestTest&&) = default;

  MTestTest& MTestTest::operator=(
      const MTestTest&) = default;

  MTestTest::~MTestTest() noexcept = default;

  void write(Group& g, const MTestTest& i) {
    raise_if(i.name.empty(), "madnex::write: no implementation name given");
    if (i.test.empty()) {
      raise("madnex::write: no test associated with implementation '" +
            i.name + "'");
    }
    auto impl = createGroup(g, i.name);
    write(impl, "test", i.test);
    write(impl, "metadata", i.metadata);
  }  // end of write

  void read(MTestTest& i, const Group& g, const std::string& n) {
    auto impl = openGroup(g, n);
    i.name = n;
    read(i.test, impl, "test");
    if (exists(impl, "metadata")) {
      read_if(i.metadata, impl, "metadata");
    }
  }  // end of read

  MTestTest getMTestTest(const std::string& f,
                         const std::string& m,
                         const std::string& b,
                         const std::string& i) {
    if (m.empty()) {
      return getMTestTest(f, b, i);
    }
    const auto file = File(f, H5F_ACC_RDONLY);
    const auto g = openGroup(
        file.getRoot(), "MFront/" + m + "/Behaviours/" + b + "/MTestTests/");
    return read<MTestTest>(g, i);
  }  // end of getMTestTest

  MTestTest getMTestTest(const std::string& f,
                         const std::string& b,
                         const std::string& i) {
    const auto file = File(f, H5F_ACC_RDONLY);
    const auto g =
        openGroup(file.getRoot(), "MFront/Behaviours/" + b + "/MTestTests/");
    return read<MTestTest>(g, i);
  }  // end of getMTestTest

}  // end of namespace madnex
