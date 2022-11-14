/*!
 * \file   MFrontParameters.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   17 janv. 2017
 */

#include <cstdlib>
#include <stdexcept>
#include "Madnex/Types.hxx"
#include "Madnex/CompoundDataArrayView.hxx"
#include "Madnex/MFrontParameters.hxx"

namespace madnex {

  static H5::CompType getMFrontParametersCompType() {
    H5::CompType c(sizeof(MFrontParameter));
    c.insertMember("code", HOFFSET(MFrontParameter, code), StrType(0, 32));
    c.insertMember("name", HOFFSET(MFrontParameter, name), StrType(0, 512));
    c.insertMember("type", HOFFSET(MFrontParameter, type), StrType(0, 16));
    c.insertMember("value", HOFFSET(MFrontParameter, value), StrType(0, 512));
    c.insertMember("unit", HOFFSET(MFrontParameter, unit), StrType(0, 16));
    return c;
  }  // end of getMFrontParametersCompType

  MFrontParameter::MFrontParameter() = default;
  MFrontParameter::MFrontParameter(MFrontParameter&&) = default;
  MFrontParameter::MFrontParameter(const MFrontParameter&) = default;
  MFrontParameter& MFrontParameter::operator=(MFrontParameter&&) = default;
  MFrontParameter& MFrontParameter::operator=(const MFrontParameter&) = default;

  void write(Group& g, const std::string& n, const MFrontParameters& o) {
    const auto c = getMFrontParametersCompType();
    if(contains(g,n)){
      removeDataSet(g,n);
    }
    hsize_t dim[] = {o.size()}; /* Dataspace dimensions */
    const auto d = g.createDataSet(n, c, DataSpace(1, dim));
    d.write(o.data(), c);
  }  // end of write

  void read(MFrontParameters& o, const Group& g, const std::string& n) {
    using size_type = decltype(o.size());
    o.clear();
    try {
      CompoundDataArrayView a(openDataSet(g, n));
      for (decltype(a.size()) i = 0; i != a.size(); ++i) {
        const auto v = a[i];
        MFrontParameter r;
        extract(r.code, v, size_type(0));
        extract(r.name, v, size_type(1));
        extract(r.type, v, size_type(2));
        extract(r.value, v, size_type(3));
        extract(r.unit, v, size_type(4));
        o.emplace_back(std::move(r));
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

}  // end of namespace madnex
