/*!
 * \file   src/MFrontMetaData.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   27/08/2020
 */

#include "Madnex/MFrontMetaData.hxx"

namespace madnex{

  MFrontMetaData::MFrontMetaData() = default;

  MFrontMetaData::MFrontMetaData(MFrontMetaData&&) = default;

  MFrontMetaData::MFrontMetaData(const MFrontMetaData&) = default;

  MFrontMetaData& MFrontMetaData::operator=(MFrontMetaData&&) = default;

  MFrontMetaData& MFrontMetaData::operator=(const MFrontMetaData&) = default;

  MFrontMetaData::~MFrontMetaData() noexcept = default;

  void read(MFrontMetaData& d, const Group& g, const std::string& n) {
    auto data = openGroup(g, n);
    read(d.author, data, "author");
    read(d.date, data, "date");
    read(d.description, data, "description");
  }  // end of read

  void write(Group& g, const std::string& n, const MFrontMetaData& d) {
    auto data = createGroup(g, n);
    write(data, "author",d.author);
    write(data, "date", d.date);
    write(data, "description", d.description);
  }  // end of write

} // end of namespace madnex
