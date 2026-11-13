/*!
 * \file   tests/MadnexFCString.cxx
 * \brief  regression tests for fixed length strings: the content of a
 * HDF5 fixed length string is not necessarily null terminated, so any
 * scan of the raw buffer must be bounded by the size of the member.
 * \author Thomas Helfer
 * \date   06/11/2026
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cstring>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "Madnex/Raise.hxx"
#include "Madnex/File.hxx"
#include "Madnex/Types.hxx"
#include "Madnex/FCString.hxx"
#include "Madnex/CompoundDataView.hxx"
#include "Madnex/CompoundDataArrayView.hxx"

namespace {

  // a compound type made of fixed length strings only, entirely filled:
  // no null character is stored in the file, which is legal for the HDF5
  // fixed length string type. The members are contiguous and the last
  // member of the last record ends exactly at the end of the buffer read
  // by the library.
  static H5::CompType getCompType() {
    try {
      using namespace madnex;
      H5::CompType c(static_cast<size_t>(16));
      c.insertMember("first", 0, StrType(0, 8));
      c.insertMember("second", 8, StrType(0, 8));
      return c;
    } catch (H5::Exception& e) {
      madnex::raise(e.getDetailMsg());
    }
  }  // end of getCompType

  static void write_file() {
    using namespace madnex;
    auto f = File("MadnexFCString.madnex", H5F_ACC_TRUNC);
    const auto r = f.getRoot();
    auto data = std::vector<char>{
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p',
      'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
      'z', '0', '1'};
    const hsize_t dims[1] = {2};
    const auto dspace = DataSpace(1, dims);
    auto d = r.createDataSet("test", getCompType(), dspace);
    d.write(data.data(), getCompType());
  }  // end of write_file

  static void read_file() {
    using namespace madnex;
    const auto f = File("MadnexFCString.madnex", H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    const CompoundDataArrayView d(openDataSet(r, "test"));
    assert(d.size() == 2u);
    // members followed by other members
    const auto v0 = d[0u];
    assert(v0.extract<std::string>("first") == "abcdefgh");
    assert(v0.extract<std::string>("second") == "ijklmnop");
    assert(v0.extract<fcstring<8>>("first") == "abcdefgh");
    // the second member of the second record is made of the very last
    // bytes of the buffer read by the library and is not null
    // terminated: scanning it without bounds would read past the end of
    // the buffer
    const auto v1 = d[1u];
    assert(v1.extract<std::string>("first") == "mnopqrst");
    assert(v1.extract<std::string>("second") == "uvwxyz01");
    assert(v1.extract<fcstring<8>>("first") == "mnopqrst");
    assert(v1.extract<fcstring<8>>("second") == "uvwxyz01");
    // the same check, extracting whole columns
    const auto firsts = d.extract<std::string>("first");
    assert((firsts == std::vector<std::string>{"abcdefgh", "mnopqrst"}));
    const auto seconds = d.extract<fcstring<8>>("second");
    assert((seconds == std::vector<fcstring<8>>{"ijklmnop", "uvwxyz01"}));
  }  // end of read_file

  static void check_assign() {
    using namespace madnex;
    // a buffer without any null character: `assign` and `strnlen` only
    // scan the given number of characters
    constexpr const char buffer[12] = {'a', 'b', 'c', 'd', 'e', 'f',
                                       'g', 'h', 'i', 'j', 'k', 'l'};
    fcstring<8> x;
    x.assign(buffer, 8);
    assert(static_cast<std::string>(x) == "abcdefgh");
    assert(x.size() == 8u);
    assert(fcstring<8>::strnlen(buffer, 8) == 8u);
    assert(fcstring<8>::strnlen(buffer, 4) == 4u);
  }  // end of check_assign

}  // end of anonymous namespace

int main() {
  try {
    write_file();
    read_file();
    check_assign();
  } catch (const std::exception& e) {
    std::cerr << "MadnexFCString: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
