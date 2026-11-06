/*!
 * \file   tests/MadnexReorderedMembers.cxx
 * \brief  regression test for issue #13: the members of compound data
 * sets are resolved by name (with a positional fallback for legacy
 * files), so a file whose members are stored in a different order must
 * be read correctly.
 * \author Thomas Helfer
 * \date   06/11/2026
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <string>

#include "Madnex/Raise.hxx"
#include "Madnex/File.hxx"
#include "Madnex/Types.hxx"
#include "Madnex/FCString.hxx"
#include "Madnex/MainResults.hxx"

namespace {

  // same information as `madnex::MainResult` but the members are stored
  // in a different order: a positional read would silently swap the
  // values
  struct ReorderedMainResult {
    madnex::fcstring<16> unit;
    madnex::fcstring<32> name;
    madnex::fcstring<16> type;
    madnex::fcstring<512> label;
    madnex::fcstring<256> value;
  };  // end of struct ReorderedMainResult

  static H5::CompType getCompType() {
    try {
      using namespace madnex;
      H5::CompType c(sizeof(ReorderedMainResult));
      c.insertMember("unit", HOFFSET(ReorderedMainResult, unit), StrType(0, 16));
      c.insertMember("name", HOFFSET(ReorderedMainResult, name), StrType(0, 32));
      c.insertMember("type", HOFFSET(ReorderedMainResult, type), StrType(0, 16));
      c.insertMember("label", HOFFSET(ReorderedMainResult, label),
                     StrType(0, 512));
      c.insertMember("value", HOFFSET(ReorderedMainResult, value),
                     StrType(0, 256));
      return c;
    } catch (H5::Exception& e) {
      madnex::raise(e.getDetailMsg());
    }
  }  // end of getCompType

  static void write_file() {
    using namespace madnex;
    auto f = File("MadnexReorderedMembers.madnex", H5F_ACC_TRUNC);
    const auto r = f.getRoot();
    ReorderedMainResult mr;
    mr.unit = std::string("mm");
    mr.name = std::string("R1");
    mr.type = std::string("scalar");
    mr.label = std::string("a label");
    mr.value = std::string("123");
    const hsize_t dims[1] = {1};
    auto d = r.createDataSet("main_results", getCompType(), DataSpace(1, dims));
    d.write(&mr, getCompType());
  }  // end of write_file

  static void read_file() {
    using namespace madnex;
    const auto f = File("MadnexReorderedMembers.madnex", H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    const auto mrs = read<MainResults>(r, "main_results");
    assert(mrs.size() == 1u);
    assert(mrs[0].name == "R1");
    assert(mrs[0].label == "a label");
    assert(mrs[0].value == "123");
    assert(mrs[0].unit == "mm");
    assert(mrs[0].type == "scalar");
  }  // end of read_file

}  // end of anonymous namespace

int main() {
  try {
    write_file();
    read_file();
  } catch (const std::exception& e) {
    std::cerr << "MadnexReorderedMembers: " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
