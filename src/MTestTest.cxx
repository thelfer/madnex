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

  MTestTest::MTestTest(const MTestTest&) = default;

  MTestTest& MTestTest::operator=(MTestTest&&) = default;

  MTestTest& MTestTest::operator=(const MTestTest&) = default;

  MTestTest::~MTestTest() noexcept = default;

  void write(Group& g, const MTestTest& i) {
    try {
      raise_if(i.name.empty(), "madnex::write: no implementation name given");
      if (!i.scheme.empty()) {
        if ((i.scheme != "mtest") && (i.scheme != "ptest")) {
          raise("madnex::write: invalid scheme '" + i.scheme + "'");
        }
      }
      if (i.test.empty()) {
        raise("madnex::write: no test associated with implementation '" +
              i.name + "'");
      }
      auto impl = createGroup(g, i.name);
      write(impl, "test", i.test);
      if (!i.scheme.empty()) {
        write(impl, "scheme", i.scheme);
      }
      write(impl, "metadata", i.metadata);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of write

  void read(MTestTest& i, const Group& g, const std::string& n) {
    try {
      auto impl = openGroup(g, n);
      i.name = n;
      read(i.test, impl, "test");
      if (exists(impl, "metadata")) {
        read_if(i.metadata, impl, "metadata");
      }
      if (exists(impl, "scheme")) {
        read_if(i.scheme, impl, "scheme");
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of read

  MTestTest getMTestTest(const std::string& f,
                         const std::string& m,
                         const std::string& b,
                         const std::string& i) {
    try {
      if (m.empty()) {
        return getMTestTest(f, b, i);
      }
      const auto file = File(f, H5F_ACC_RDONLY);
      const auto g = openGroup(
          file.getRoot(), "MFront/" + m + "/Behaviours/" + b + "/MTestTests/");
      return read<MTestTest>(g, i);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getMTestTest

  MTestTest getMTestTest(const std::string& f,
                         const std::string& b,
                         const std::string& i) {
    try {
      const auto file = File(f, H5F_ACC_RDONLY);
      const auto g =
          openGroup(file.getRoot(), "MFront/Behaviours/" + b + "/MTestTests/");
      return read<MTestTest>(g, i);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getMTestTest

  std::string getMTestTestScheme(const std::string& f,
                                 const std::string& m,
                                 const std::string& b,
                                 const std::string& i) {
    try {
      if (m.empty()) {
        return getMTestTestScheme(f, b, i);
      }
      const auto file = File(f, H5F_ACC_RDONLY);
      const auto g =
          openGroup(file.getRoot(),  //
                    "MFront/" + m + "/Behaviours/" + b + "/MTestTests/" + i);
      if (!exists(g, "scheme")) {
        return "";
      }
      return read<std::string>(g, "scheme");
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getMTestTestScheme

  std::string getMTestTestScheme(const std::string& f,
                                 const std::string& b,
                                 const std::string& i) {
    try {
      const auto file = File(f, H5F_ACC_RDONLY);
      const auto g = openGroup(file.getRoot(),  //
                               "MFront/Behaviours/" + b + "/MTestTests/" + i);
      if (!exists(g, "scheme")) {
        return "";
      }
      return read<std::string>(g, "scheme");
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getMTestTestScheme

}  // end of namespace madnex
