/*!
 * \file   src/MFMTestGeneratorTest.cxx
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
#include "Madnex/MFMTestGeneratorTest.hxx"

namespace madnex {

  MFMTestGeneratorTest::MFMTestGeneratorTest() = default;

  MFMTestGeneratorTest::MFMTestGeneratorTest(MFMTestGeneratorTest&&) = default;

  MFMTestGeneratorTest::MFMTestGeneratorTest(const MFMTestGeneratorTest&) =
      default;

  MFMTestGeneratorTest& MFMTestGeneratorTest::operator=(
      MFMTestGeneratorTest&&) = default;

  MFMTestGeneratorTest& MFMTestGeneratorTest::operator=(
      const MFMTestGeneratorTest&) = default;

  MFMTestGeneratorTest::~MFMTestGeneratorTest() noexcept = default;

  void write(Group& g, const MFMTestGeneratorTest& i) {
    try {
      raise_if(i.name.empty(), "madnex::write: no implementation name given");
      if (i.test.empty()) {
        raise("madnex::write: no test associated with implementation '" +
              i.name + "'");
      }
      auto impl = createGroup(g, i.name);
      write(impl, "test", i.test);
      write(impl, "metadata", i.metadata);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of write

  void read(MFMTestGeneratorTest& i, const Group& g, const std::string& n) {
    try {
      auto impl = openGroup(g, n);
      i.name = n;
      read(i.test, impl, "test");
      if (exists(impl, "metadata")) {
        read_if(i.metadata, impl, "metadata");
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of read

  MFMTestGeneratorTest getMFMTestGeneratorTest(const std::string& f,
                                               const std::string& m,
                                               const std::string& b,
                                               const std::string& i) {
    try {
      if (m.empty()) {
        return getMFMTestGeneratorTest(f, b, i);
      }
      const auto file = File(f, H5F_ACC_RDONLY);
      const auto g =
          openGroup(file.getRoot(), "MFront/" + m + "/Behaviours/" + b +
                                        "/MFMTestGeneratorTests/");
      return read<MFMTestGeneratorTest>(g, i);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getMFMTestGeneratorTest

  MFMTestGeneratorTest getMFMTestGeneratorTest(const std::string& f,
                                               const std::string& b,
                                               const std::string& i) {
    try {
      const auto file = File(f, H5F_ACC_RDONLY);
      const auto g = openGroup(
          file.getRoot(), "MFront/Behaviours/" + b + "/MFMTestGeneratorTests/");
      return read<MFMTestGeneratorTest>(g, i);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getMFMTestGeneratorTest

}  // end of namespace madnex
