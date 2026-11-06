/*!
 * \file   tests/MadnexExceptions.cxx
 * \brief  regression tests for issue #15: the library throws
 * `madnex::RuntimeError` exceptions which report their message, an
 * invalid regular expression is translated into an error of the
 * library and a zero sized fixed length string attribute is read as an
 * empty string instead of triggering undefined behaviour.
 * \author Thomas Helfer
 * \date   06/11/2026
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Madnex/Raise.hxx"
#include "Madnex/Exceptions.hxx"
#include "Madnex/File.hxx"
#include "Madnex/DataBase.hxx"
#include "Madnex/HDF5.hxx"

namespace {

  static void check_exceptions() {
    using namespace madnex;
    // `raise` throws a `madnex::RuntimeError` which reports its message
    auto catched_as_exception = false;
    try {
      raise("something went wrong");
    } catch (const madnex::Exception& e) {
      catched_as_exception = true;
      assert(std::string(e.what()) == "something went wrong");
    }
    assert(catched_as_exception);
    // the new exception type remains catchable as a `std::runtime_error`
    auto catched_as_runtime_error = false;
    try {
      raise("legacy compatibility");
    } catch (const std::runtime_error& e) {
      catched_as_runtime_error = true;
      assert(std::string(e.what()) == "legacy compatibility");
    }
    assert(catched_as_runtime_error);
  }  // end of check_exceptions

  static void write_file() {
    using namespace madnex;
    auto f = File("MadnexExceptions.madnex", H5F_ACC_TRUNC);
    auto r = f.getRoot();
    write(r, "integer_value", static_cast<std::int32_t>(12));
    // create a fixed length string attribute stored with a zero sized
    // extent: the value can not be read through the first character of
    // an empty string nor checked with `back()`
    const hsize_t dims[1] = {0};
    const auto s = H5Screate_simple(1, dims, nullptr);
    const auto t = H5Tcopy(H5T_C_S1);
    H5Tset_size(t, 8);
    const auto a =
        H5Acreate2(r.getId(), "empty_fixed", t, s, H5P_DEFAULT, H5P_DEFAULT);
    if (a < 0) {
      madnex::raise("could not create the empty fixed length attribute");
    }
    const char buffer[8] = {'n', 'o', 't', 'h', 'i', 'n', 'g', '!'};
    if (H5Awrite(a, t, buffer) < 0) {
      madnex::raise("could not write the empty fixed length attribute");
    }
    H5Aclose(a);
    H5Tclose(t);
    H5Sclose(s);
  }  // end of write_file

  static void read_file() {
    using namespace madnex;
    const auto f = File("MadnexExceptions.madnex", H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    // a zero sized fixed length string attribute yields an empty string
    assert(getAttributeValue<std::string>(r, "empty_fixed") == "");
    // the errors reported by the library are `madnex::Exception` objects
    // carrying a meaningful message
    auto catched = false;
    try {
      read<std::int32_t>(r, "no_such_dataset");
    } catch (const madnex::Exception& e) {
      catched = true;
      assert(std::strlen(e.what()) != 0u);
    }
    assert(catched);
    // an invalid regular expression is translated into an error of the
    // library which mentions the offending pattern
    const auto db = DataBase("MadnexExceptions.madnex");
    catched = false;
    try {
      getMatchingMaterials(db, "[");
    } catch (const madnex::Exception& e) {
      catched = true;
      assert(std::string(e.what()).find("[") != std::string::npos);
    }
    assert(catched);
    // a valid regular expression is unaffected (the empty string
    // denotes the generic material properties, always present)
    assert((getMatchingMaterials(db, ".*") == std::vector<std::string>{""}));
    assert(getMatchingMaterials(db, "no_such_material").empty());
  }  // end of read_file

}  // end of anonymous namespace

int main() {
  try {
    check_exceptions();
    write_file();
    read_file();
  } catch (const std::exception& e) {
    std::cerr << "MadnexExceptions: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
