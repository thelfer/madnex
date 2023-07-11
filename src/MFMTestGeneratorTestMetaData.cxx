/*!
 * \file   src/MFMTestGeneratorTestMetaData.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27/08/2020
 */

#include "Madnex/MFMTestGeneratorTestMetaData.hxx"

namespace madnex {

  MFMTestGeneratorTestMetaData::MFMTestGeneratorTestMetaData() = default;

  MFMTestGeneratorTestMetaData::MFMTestGeneratorTestMetaData(
      MFMTestGeneratorTestMetaData&&) = default;

  MFMTestGeneratorTestMetaData::MFMTestGeneratorTestMetaData(
      const MFMTestGeneratorTestMetaData&) = default;

  MFMTestGeneratorTestMetaData& MFMTestGeneratorTestMetaData::operator=(
      MFMTestGeneratorTestMetaData&&) = default;

  MFMTestGeneratorTestMetaData& MFMTestGeneratorTestMetaData::operator=(
      const MFMTestGeneratorTestMetaData&) = default;

  MFMTestGeneratorTestMetaData::~MFMTestGeneratorTestMetaData() noexcept =
      default;

  void read(MFMTestGeneratorTestMetaData& d,
            const Group& g,
            const std::string& n) {
    auto data = openGroup(g, n);
    read(d.author, data, "author");
    read(d.date, data, "date");
    read(d.description, data, "description");
  }  // end of read

  void write(Group& g,
             const std::string& n,
             const MFMTestGeneratorTestMetaData& d,
             const bool b) {
    if (b) {
      unlinkIfExists(g, n);
    }
    auto data = createGroup(g, n);
    write(data, "author", d.author);
    write(data, "date", d.date);
    write(data, "description", d.description);
  }  // end of write

}  // end of namespace madnex
