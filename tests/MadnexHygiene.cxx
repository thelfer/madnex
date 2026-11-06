/*!
 * \file   tests/MadnexHygiene.cxx
 * \brief  regression tests for issue #16: error messages attribute
 * themselves to the function they originate from and
 * `File::checkIfGroupIsWritable` reports the truth instead of being a
 * stub always returning `true`.
 * \author Thomas Helfer
 * \date   06/11/2026
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Madnex/Raise.hxx"
#include "Madnex/File.hxx"
#include "Madnex/DataBase.hxx"
#include "Madnex/HDF5.hxx"

namespace {

  // `File::checkIfGroupIsWritable` is a protected member: this helper
  // exposes it so that the test can exercise it
  struct TestFile : madnex::File {
    // inheriting the constructors
    using madnex::File::File;
    // exposing the method under test
    using madnex::File::checkIfGroupIsWritable;
  };  // end of struct TestFile

  static void write_file() {
    using namespace madnex;
    auto f = File("MadnexHygiene.madnex", H5F_ACC_TRUNC);
    auto r = f.getRoot();
    write(r, "integer_value", static_cast<std::int32_t>(12));
    // declare a material so that the file is seen as an MFront database
    createGroup(r, "MFront");
    createGroup(r, "MFront/Gurson");
  }  // end of write_file

  static void check_group_is_writable() {
    {
      auto f = TestFile("MadnexHygiene.madnex", H5F_ACC_RDWR);
      // an existing group of a read/write file is writable
      assert(f.checkIfGroupIsWritable("MFront"));
      // so is a group that does not exist yet
      assert(f.checkIfGroupIsWritable("new_group"));
      // an existing object which is not a group is not
      assert(!f.checkIfGroupIsWritable("integer_value"));
    }
    {
      auto f = TestFile("MadnexHygiene.madnex", H5F_ACC_RDONLY);
      // a file open in read only mode accepts no modification: this
      // used to return `true` (the method was a stub)
      assert(!f.checkIfGroupIsWritable("MFront"));
      assert(!f.checkIfGroupIsWritable("new_group"));
    }
  }  // end of check_group_is_writable

  static void check_error_messages() {
    using namespace madnex;
    // the error reported by `checkTypeAttribute` on a data set mentions
    // a data set, not a group
    {
      const auto f = File("MadnexHygiene.madnex", H5F_ACC_RDONLY);
      const auto r = f.getRoot();
      const auto d = openDataSet(r, "integer_value");
      auto catched = false;
      try {
        checkTypeAttribute(d, "wrong_type");
      } catch (const std::runtime_error& e) {
        catched = true;
        assert(std::string(e.what()).find("data set") != std::string::npos);
      }
      assert(catched);
    }
    // `checkIfMaterialExists` attributes the error to itself and does
    // not hardcode the name of one of its callers (this test reaches it
    // through `getAvailableModels`)
    {
      const auto db = DataBase("MadnexHygiene.madnex");
      auto catched = false;
      try {
        db.getAvailableModels("Norton");
      } catch (const std::runtime_error& e) {
        catched = true;
        const auto msg = std::string(e.what());
        assert(msg.find("getAvailableBehaviours") == std::string::npos);
        assert(msg.find("Norton") != std::string::npos);
      }
      assert(catched);
    }
  }  // end of check_error_messages

}  // end of anonymous namespace

int main() {
  try {
    write_file();
    check_group_is_writable();
    check_error_messages();
  } catch (const std::exception& e) {
    std::cerr << "MadnexHygiene: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
