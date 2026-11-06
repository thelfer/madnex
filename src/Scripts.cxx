/*!
 * \file   Scripts.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   17 janv. 2017
 */

#include <cstdlib>
#include <stdexcept>
#include "Madnex/Types.hxx"
#include "Madnex/Scripts.hxx"
#include "Madnex/CompoundDataArrayView.hxx"

static H5::CompType getScriptCompType() {
  try {
    using namespace madnex;
    H5::CompType c(sizeof(Script));
    const auto type = getNativeType<char>();
    c.insertMember("author", HOFFSET(Script, author), StrType(0, 100));
    c.insertMember("date", HOFFSET(Script, date), StrType(0, 20));
    c.insertMember("command_line", HOFFSET(Script, command_line),
                   StrType(0, 500));
    c.insertMember("script_version", HOFFSET(Script, script_version),
                   StrType(0, 10));
    return c;
  } catch (H5::Exception& e) {
    madnex::raise(e.getDetailMsg());
  }
}  // end of getScriptCompType

namespace madnex {

  Script::Script() = default;
  Script::Script(Script&&) = default;
  Script::Script(const Script&) = default;
  Script& Script::operator=(Script&&) = default;
  Script& Script::operator=(const Script&) = default;
  Script::~Script() = default;

  void write(Group& g, const std::string& n, const Scripts& o, const bool b) {
    try {
      if (b) {
        unlinkIfExists(g, n);
      }
      const auto c = getScriptCompType();
      hsize_t dim[] = {o.size()}; /* Dataspace dimensions */
      const auto d = g.createDataSet(n, c, DataSpace(1, dim));
      d.write(o.data(), c);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of write

  void read(Scripts& o, const Group& g, const std::string& n) {
    using size_type = decltype(o.size());
    try {
      const CompoundDataArrayView v(openDataSet(g, n));
      o.clear();
      o.reserve(v.size());
      // resolve the members by name, falling back to the position of the
      // members in files written by this library when no member has the
      // expected name (some legacy files use different member names)
      const auto author_index = v.getMemberIndexOr("author", 0);
      const auto date_index = v.getMemberIndexOr("date", 1);
      const auto command_line_index = v.getMemberIndexOr("command_line", 2);
      const auto script_version_index = v.getMemberIndexOr("script_version", 3);
      for (size_type i = 0; i != v.size(); ++i) {
        const auto d = v[i];
        Script ec;
        extract(ec.author, d, author_index);
        extract(ec.date, d, date_index);
        extract(ec.command_line, d, command_line_index);
        extract(ec.script_version, d, script_version_index);
        o.emplace_back(std::move(ec));
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

}  // end of namespace madnex
