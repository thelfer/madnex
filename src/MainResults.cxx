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
      using size_type = decltype(o.size());
      o.clear();
      CompoundDataArrayView a(openDataSet(g, n));
      for (decltype(a.size()) i = 0; i != a.size(); ++i) {
        const auto v = a[i];
        MainResult r;
        extract(r.name, v, size_type(0));
        extract(r.label, v, size_type(1));
        extract(r.value, v, size_type(2));
        extract(r.unit, v, size_type(3));
        extract(r.type, v, size_type(4));
        o.emplace_back(std::move(r));
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

}  // end of namespace madnex
