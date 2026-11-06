/*!
 * \file   MainResults.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   17 janv. 2017
 */

#include <cstdlib>
#include <stdexcept>
#include "Madnex/Types.hxx"
#include "Madnex/CompoundDataArrayView.hxx"
#include "Madnex/MainResults.hxx"

namespace madnex {

  static H5::CompType getMainResultsCompType() {
    try {
      H5::CompType c(sizeof(MainResult));
      c.insertMember("name", HOFFSET(MainResult, name), StrType(0, 32));
      c.insertMember("label", HOFFSET(MainResult, label), StrType(0, 512));
      c.insertMember("value", HOFFSET(MainResult, value), StrType(0, 256));
      c.insertMember("unit", HOFFSET(MainResult, unit), StrType(0, 16));
      c.insertMember("type", HOFFSET(MainResult, type), StrType(0, 16));
      return c;
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getMainResultsCompType

  MainResult::MainResult() = default;
  MainResult::MainResult(MainResult&&) = default;
  MainResult::MainResult(const MainResult&) = default;
  MainResult& MainResult::operator=(MainResult&&) = default;
  MainResult& MainResult::operator=(const MainResult&) = default;

  void write(Group& g,
             const std::string& n,
             const MainResults& o,
             const bool b) {
    try {
      if (b) {
        unlinkIfExists(g, n);
      }
      const auto c = getMainResultsCompType();
      if (contains(g, n)) {
        removeDataSet(g, n);
      }
      hsize_t dim[] = {o.size()}; /* Dataspace dimensions */
      const auto d = g.createDataSet(n, c, DataSpace(1, dim));
      d.write(o.data(), c);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of write

  void read(MainResults& o, const Group& g, const std::string& n) {
    try {
      o.clear();
      CompoundDataArrayView a(openDataSet(g, n));
      // resolve the members by name, falling back to the position of the
      // members in files written by this library when no member has the
      // expected name (some legacy files use different member names)
      const auto name_index = a.getMemberIndexOr("name", 0);
      const auto label_index = a.getMemberIndexOr("label", 1);
      const auto value_index = a.getMemberIndexOr("value", 2);
      const auto unit_index = a.getMemberIndexOr("unit", 3);
      const auto type_index = a.getMemberIndexOr("type", 4);
      for (decltype(a.size()) i = 0; i != a.size(); ++i) {
        const auto v = a[i];
        MainResult r;
        extract(r.name, v, name_index);
        extract(r.label, v, label_index);
        extract(r.value, v, value_index);
        extract(r.unit, v, unit_index);
        extract(r.type, v, type_index);
        o.emplace_back(std::move(r));
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

}  // end of namespace madnex
