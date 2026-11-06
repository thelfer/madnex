/*!
 * \file   ExperimentalConditions.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   17 janv. 2017
 */

#include <cstdlib>
#include <stdexcept>
#include "Madnex/Raise.hxx"
#include "Madnex/Types.hxx"
#include "Madnex/ExperimentalConditions.hxx"

//! a C-structure mapping the ExperimentalCondition structure
template <typename char_type>
struct CExperimentalCondition {
  char_type* name = nullptr;
  char_type* label = nullptr;
  char_type* value = nullptr;
  char_type* unit = nullptr;
  char_type* type = nullptr;
};

template <typename char_type>
static H5::CompType getExperimentalConditionCompType() {
  try {
    using CEC = CExperimentalCondition<char_type>;
    H5::CompType c(sizeof(CEC));
    const auto type = madnex::getNativeType<std::string>();
    c.insertMember("name", HOFFSET(CEC, name), type);
    c.insertMember("label", HOFFSET(CEC, label), type);
    c.insertMember("value", HOFFSET(CEC, value), type);
    c.insertMember("unit", HOFFSET(CEC, unit), type);
    c.insertMember("type", HOFFSET(CEC, type), type);
    return c;
  } catch (H5::Exception& e) {
    madnex::raise(e.getDetailMsg());
  }
}  // end of getExperimentalConditionCompType

namespace madnex {

  ExperimentalCondition::ExperimentalCondition() = default;
  ExperimentalCondition::ExperimentalCondition(ExperimentalCondition&&) =
      default;
  ExperimentalCondition::ExperimentalCondition(const ExperimentalCondition&) =
      default;
  ExperimentalCondition& ExperimentalCondition::operator=(
      ExperimentalCondition&&) = default;
  ExperimentalCondition& ExperimentalCondition::operator=(
      const ExperimentalCondition&) = default;

  void write(Group& g, const std::string& n, const ExperimentalConditions& o, const bool b) {
    try {
      if (b) {
        unlinkIfExists(g, n);
      }
      auto make_copy = [](const ExperimentalCondition& ec) {
        CExperimentalCondition<const char> r;
        r.name = ec.name.c_str();
        r.label = ec.label.c_str();
        r.value = ec.value.c_str();
        r.unit = ec.unit.c_str();
        r.type = ec.type.c_str();
        return r;
      };
      const auto c = getExperimentalConditionCompType<const char>();
      std::vector<CExperimentalCondition<const char>> cecs;
      cecs.reserve(o.size());
      for (const auto& ec : o) {
        cecs.emplace_back(make_copy(ec));
      }
      hsize_t dim[] = {o.size()}; /* Dataspace dimensions */
      const auto d = g.createDataSet(n, c, DataSpace(1, dim));
      d.write(cecs.data(), c);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of write

  // \brief release the memory allocated by the HDF5 library while
  // reading a record of variable length strings. This function shall not
  // throw so that it can be called while unwinding.
  static void releaseCExperimentalCondition(CExperimentalCondition<char>& cec) {
    H5free_memory(cec.name);
    cec.name = nullptr;
    H5free_memory(cec.label);
    cec.label = nullptr;
    H5free_memory(cec.value);
    cec.value = nullptr;
    H5free_memory(cec.unit);
    cec.unit = nullptr;
    H5free_memory(cec.type);
    cec.type = nullptr;
  }  // end of releaseCExperimentalCondition

  void read(ExperimentalConditions& o, const Group& g, const std::string& n) {
    try {
      const auto c = getExperimentalConditionCompType<char>();
      const auto d = openDataSet(g, n);
      const auto dspace = d.getSpace();
      if (dspace.getSimpleExtentNdims() != 1) {
        raise("madnex::read: invalid dimension");
      }
      hsize_t dims[1];
      dspace.getSimpleExtentDims(dims);
      // the data set may contain less records than the rows already
      // stored in the out paramter: clear it first, like all the other
      // `read` functions of the library
      o.clear();
      std::vector<CExperimentalCondition<char>> cecs;
      cecs.resize(dims[0]);
      try {
        d.read(cecs.data(), c);
        o.reserve(dims[0]);
        for (auto& cec : cecs) {
          ExperimentalCondition ec;
          ec.name = cec.name;
          ec.label = cec.label;
          ec.value = cec.value;
          ec.unit = cec.unit;
          ec.type = cec.type;
          o.emplace_back(std::move(ec));
        }
      } catch (...) {
        // release the memory allocated by the HDF5 library before
        // propagating the exception (an exception may be thrown while
        // copying the strings, e.g. `std::bad_alloc`)
        for (auto& cec : cecs) {
          releaseCExperimentalCondition(cec);
        }
        throw;
      }
      for (auto& cec : cecs) {
        releaseCExperimentalCondition(cec);
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of read

}  // end of namespace madnex
