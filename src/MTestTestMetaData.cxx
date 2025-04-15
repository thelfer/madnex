/*!
 * \file   src/MTestTestMetaData.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27/08/2020
 */

#include "Madnex/Raise.hxx"
#include "Madnex/MTestTestMetaData.hxx"

namespace madnex {

  MTestTestMetaData::MTestTestMetaData() = default;

  MTestTestMetaData::MTestTestMetaData(MTestTestMetaData&&) = default;

  MTestTestMetaData::MTestTestMetaData(const MTestTestMetaData&) = default;

  MTestTestMetaData& MTestTestMetaData::operator=(MTestTestMetaData&&) =
      default;

  MTestTestMetaData& MTestTestMetaData::operator=(const MTestTestMetaData&) =
      default;

  MTestTestMetaData::~MTestTestMetaData() noexcept = default;

  void read(MTestTestMetaData& d, const Group& g, const std::string& n) {
    try {
      auto data = openGroup(g, n);
      read(d.author, data, "author");
      read(d.date, data, "date");
      read(d.description, data, "description");
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of read

  void write(Group& g, const std::string& n, const MTestTestMetaData& d, const bool b) {
    try {
      if (b) {
        unlinkIfExists(g, n);
      }
      auto data = createGroup(g, n);
      write(data, "author", d.author);
      write(data, "date", d.date);
      write(data, "description", d.description);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of write

}  // end of namespace madnex
