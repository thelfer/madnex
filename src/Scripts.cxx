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
}  // end of getScriptCompType

namespace madnex {

  Script::Script() = default;
  Script::Script(Script&&) = default;
  Script::Script(const Script&) = default;
  Script& Script::operator=(Script&&) = default;
  Script& Script::operator=(const Script&) = default;
  Script::~Script() = default;

  void write(Group& g, const std::string& n, const Scripts& o) {
    const auto c = getScriptCompType();
    hsize_t dim[] = {o.size()}; /* Dataspace dimensions */
    const auto d = g.createDataSet(n, c, DataSpace(1, dim));
    d.write(o.data(), c);
  }  // end of write

  void read(Scripts& o, const Group& g, const std::string& n) {
    using size_type = decltype(o.size());
    try {
      const CompoundDataArrayView v(openDataSet(g, n));
      o.clear();
      o.reserve(v.size());
      for (size_type i = 0; i != v.size(); ++i) {
        const auto d = v[i];
        Script ec;
        extract(ec.author, d, size_type(0));
        extract(ec.date, d, size_type(1));
        extract(ec.command_line, d, size_type(2));
        extract(ec.script_version, d, size_type(3));
        o.emplace_back(std::move(ec));
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

}  // end of namespace madnex
