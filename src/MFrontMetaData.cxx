/*!
 * \file   src/MFrontMetaData.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   27/08/2020
 */

#include "Madnex/CompoundDataArrayView.hxx"
#include "Madnex/MFrontMetaData.hxx"

namespace madnex{

  MFrontMetaData::MFrontMetaData() = default;
  MFrontMetaData::MFrontMetaData(MFrontMetaData&&) = default;
  MFrontMetaData::MFrontMetaData(const MFrontMetaData&) = default;
  MFrontMetaData& MFrontMetaData::operator=(MFrontMetaData&&) = default;
  MFrontMetaData& MFrontMetaData::operator=(const MFrontMetaData&) = default;
  MFrontMetaData::~MFrontMetaData() noexcept = default;

  struct MFrontMetaDataWrapper {
    //! \brief author of the implementation
    fcstring<32> author;
    //! \brief date
    fcstring<32> date;
    //! \brief status
    fcstring<32> status;
    //! \brief description of the implemnetation
    fcstring<512> description;
    //! \brief universally unique identifier
    fcstring<36> uuid;
  };

  static H5::CompType getMFrontMetaDataWrapperCompType() {
    H5::CompType c(sizeof(MFrontMetaDataWrapper));
    c.insertMember("author", HOFFSET(MFrontMetaDataWrapper, author), StrType(0, 32));
    c.insertMember("date", HOFFSET(MFrontMetaDataWrapper, date), StrType(0, 32));
    c.insertMember("status", HOFFSET(MFrontMetaDataWrapper, status), StrType(0, 32));
    c.insertMember("description", HOFFSET(MFrontMetaDataWrapper, description), StrType(0, 512));
    c.insertMember("uuid", HOFFSET(MFrontMetaDataWrapper, uuid), StrType(0, 36));
    return c;
  }  // end of getMFrontMetaDataWrapperCompType

  static void write(Group& g, const std::string& n, const MFrontMetaDataWrapper& o, const bool b) {
    if(b){
      unlinkIfExists(g,n);
    }
    const auto c = getMFrontMetaDataWrapperCompType();
    if(contains(g,n)){
      removeDataSet(g,n);
    }
    hsize_t dim[] = {1}; /* Dataspace dimensions */
    const auto d = g.createDataSet(n, c, DataSpace(1, dim));
    d.write(&o, c);
  }  // end of write

  static void read(MFrontMetaDataWrapper& o,
                   const Group& g,
                   const std::string& n) {
    if (subGroupExists(g, n)) {
      return;
    }
    using size_type = size_t;
    try {
      CompoundDataArrayView a(openDataSet(g, n));
      if (a.size() != 1u) {
        raise("invalid number of metadata");
      }
      const auto v = a[0];
      extract(o.author, v, size_type(0));
      extract(o.date, v, size_type(1));
      extract(o.status, v, size_type(2));
      extract(o.description, v, size_type(3));
      extract(o.uuid, v, size_type(4));
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

  void write(Group& g,
             const std::string& n,
             const MFrontMetaData& o,
             const bool b) {
    auto w = MFrontMetaDataWrapper{};
    w.author = o.author;
    w.date = o.date;
    w.status = o.status;
    w.description = o.description;
    w.uuid = o.uuid;
    write(g, n, w, b);
  }

  void read(MFrontMetaData& o, const Group& g, const std::string& n) {
    auto w = MFrontMetaDataWrapper{};
    read(w, g, n);
    o.author = static_cast<std::string>(w.author);
    o.date = static_cast<std::string>(w.date);
    o.status = static_cast<std::string>(w.status);
    o.description = static_cast<std::string>(w.description);
    o.uuid = static_cast<std::string>(w.uuid);
  }

} // end of namespace madnex
