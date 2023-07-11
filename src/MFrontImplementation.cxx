/*!
 * \file   src/MFrontImplementation.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27/08/2020
 */

#include <set>
#include <cmath>
#include <limits>
#include <sstream>
#include <algorithm>
#include "Madnex/Raise.hxx"
#include "Madnex/File.hxx"
#include "Madnex/CompoundDataArrayView.hxx"
#include "Madnex/MFrontImplementation.hxx"

namespace madnex {

  struct MFrontParameter {
    fcstring<32> code;
    fcstring<512> name;
    fcstring<16> type;
    fcstring<512> value;
    fcstring<16> unit;
  };

  static H5::CompType getMFrontParameterCompType() {
    H5::CompType c(sizeof(MFrontParameter));
    c.insertMember("code", HOFFSET(MFrontParameter, code), StrType(0, 32));
    c.insertMember("name", HOFFSET(MFrontParameter, name), StrType(0, 512));
    c.insertMember("type", HOFFSET(MFrontParameter, type), StrType(0, 16));
    c.insertMember("value", HOFFSET(MFrontParameter, value), StrType(0, 512));
    c.insertMember("unit", HOFFSET(MFrontParameter, unit), StrType(0, 16));
    return c;
  }  // end of getMFrontParameterCompType

  static void write(Group& g,
                    const std::string& n,
                    const std::vector<MFrontParameter>& o,
                    const bool b) {
    if (b) {
      unlinkIfExists(g, n);
    }
    const auto c = getMFrontParameterCompType();
    if (contains(g, n)) {
      removeDataSet(g, n);
    }
    hsize_t dim[] = {o.size()}; /* Dataspace dimensions */
    const auto d = g.createDataSet(n, c, DataSpace(1, dim));
    d.write(&o, c);
  }  // end of write

  void read(std::vector<MFrontParameter>& o,
            const Group& g,
            const std::string& n) {
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

  struct MFrontBounds {
    fcstring<32> name;
    fcstring<512> min_value;
    fcstring<512> max_value;
    fcstring<16> unit;
  };

  static H5::CompType getMFrontBoundsCompType() {
    H5::CompType c(sizeof(MFrontBounds));
    // bounds
    c.insertMember("name", HOFFSET(MFrontBounds, name), StrType(0, 32));
    c.insertMember("min_value", HOFFSET(MFrontBounds, min_value),
                   StrType(0, 512));
    c.insertMember("max_value", HOFFSET(MFrontBounds, max_value),
                   StrType(0, 512));
    c.insertMember("unit", HOFFSET(MFrontBounds, unit), StrType(0, 16));
    return c;
  }  // end of getMFrontBoundsCompType

  static double extractFloatingPointNumber(const std::string& s) {
    auto throw_if = [&s](const bool b) {
      raise_if<std::invalid_argument>(b,
                                      "extractFloatingPointNumber: "
                                      "could not convert '" +
                                          s +
                                          "' "
                                          "to double");
    };
    throw_if(s.empty());
    std::size_t p;
    double r;
    try {
      r = std::stod(s, &p);
    } catch (std::exception&) {
      throw_if(true);
    }
    throw_if(p != s.size());
    return r;
  }

  static void convert(fcstring<512>& s, const double value) {
    if (!std::isfinite(value)) {
      s = "";
      return;
    }
    std::ostringstream ss;
    ss.imbue(std::locale::classic());
    ss << std::fixed << value;
    s = ss.str();
  }

  static void write(Group& g,
                    const std::string& n,
                    const std::vector<MFrontBounds>& o,
                    const bool b) {
    if (b) {
      unlinkIfExists(g, n);
    }
    const auto c = getMFrontBoundsCompType();
    if (contains(g, n)) {
      removeDataSet(g, n);
    }
    hsize_t dim[] = {o.size()}; /* Dataspace dimensions */
    const auto d = g.createDataSet(n, c, DataSpace(1, dim));
    d.write(&o, c);
  }  // end of write

  void read(std::vector<MFrontBounds>& o,
            const Group& g,
            const std::string& n) {
    using size_type = decltype(o.size());
    o.clear();
    try {
      CompoundDataArrayView a(openDataSet(g, n));
      for (decltype(a.size()) i = 0; i != a.size(); ++i) {
        const auto v = a[i];
        MFrontBounds r;
        extract(r.name, v, size_type(0));
        extract(r.min_value, v, size_type(1));
        extract(r.max_value, v, size_type(2));
        extract(r.unit, v, size_type(3));
        o.emplace_back(std::move(r));
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

  MFrontImplementation::MFrontImplementation() = default;

  MFrontImplementation::MFrontImplementation(MFrontImplementation&&) = default;

  MFrontImplementation::MFrontImplementation(const MFrontImplementation&) =
      default;

  MFrontImplementation& MFrontImplementation::operator=(
      MFrontImplementation&&) = default;

  MFrontImplementation& MFrontImplementation::operator=(
      const MFrontImplementation&) = default;

  MFrontImplementation::~MFrontImplementation() noexcept = default;

  void write(Group& g, const MFrontImplementation& i) {
    raise_if(i.name.empty(), "madnex::write: no implementation name given");
    if (i.source.empty()) {
      raise("madnex::write: no source associated with implementation '" +
            i.name + "'");
    }
    auto impl = createGroup(g, i.name);
    write(impl, "source", i.source);
    write(impl, "metadata", i.metadata);
    if (!i.parameters.empty()) {
      std::vector<MFrontParameter> parameters;
      for (const auto& nv : i.parameters) {
        MFrontParameter p;
        p.code = nv.first;
        p.name = nv.first;
        p.type = "float64";
        convert(p.value, nv.second);
        p.unit = "";
        parameters.push_back(p);
      }
      write(impl, "parameters", parameters, true);
    }
    if ((!i.lower_bounds.empty()) || (!i.upper_bounds.empty())) {
      auto names = std::set<std::string>{};
      auto insert_names =
          [&names](const std::map<std::string, double>& values) {
            std::for_each(values.begin(), values.end(),
                          [&names](const std::pair<std::string, double>& v) {
                            names.insert(v.first);
                          });
          };
      insert_names(i.upper_bounds);
      insert_names(i.lower_bounds);
      std::vector<MFrontBounds> bounds;
      for (const auto& n : names) {
        auto bvalues =
            std::array<double, 2>{std::numeric_limits<double>::quiet_NaN(),
                                  std::numeric_limits<double>::quiet_NaN()};
        auto plb = i.lower_bounds.find(n);
        if (plb != i.lower_bounds.end()) {
          bvalues[0] = plb->second;
        }
        auto pub = i.upper_bounds.find(n);
        if (pub != i.upper_bounds.end()) {
          bvalues[1] = pub->second;
        }
        MFrontBounds b;
        b.name = n;
        convert(b.min_value, bvalues[0]);
        convert(b.max_value, bvalues[1]);
        b.unit = "";
        bounds.push_back(b);
      }
      write(impl, "bounds", bounds, true);
    }
  }  // end of write

  void read(MFrontImplementation& i, const Group& g, const std::string& n) {
    try {
      auto impl = openGroup(g, n);
      i.name = n;
      read(i.source, impl, "source");
      if (exists(impl, "metadata")) {
        read_if(i.metadata, impl, "metadata");
      }
      if (exists(impl, "parameters")) {
        if (subGroupExists(impl, "parameters")) {
          auto parameters = openGroup(impl, "parameters");
          for (const auto& p : getDataSetNames(parameters)) {
            i.parameters.insert({p, read<double>(parameters, p)});
          }

        } else {
          auto parameters = std::vector<MFrontParameter>{};
          read(parameters, impl, "parameters");
          for (const auto& p : parameters) {
            i.parameters.insert({static_cast<std::string>(p.code),
                                 extractFloatingPointNumber(
                                     static_cast<std::string>(p.value))});
          }
        }
      }
      if (exists(impl, "bounds")) {
        if (subGroupExists(impl, "bounds")) {
          auto bounds = openGroup(impl, "bounds");
          std::vector<double> bvalues;
          for (const auto& b : getDataSetNames(bounds)) {
            read(bvalues, bounds, b);
            if (bvalues.size() != 2u) {
              raise(
                  "read: invalid bounds description "
                  "while reading the MFrontImplementation '" +
                  n + "'");
            }
            if (!std::isnan(bvalues[0])) {
              i.lower_bounds.insert(std::make_pair(b, bvalues[0]));
            }
            if (!std::isnan(bvalues[1])) {
              i.upper_bounds.insert(std::make_pair(b, bvalues[1]));
            }
          }

        } else {
          auto bounds = std::vector<MFrontBounds>{};
          read(bounds, impl, "bounds");
          for (const auto& b : bounds) {
            const auto vn = static_cast<std::string>(b.name);
            if (!b.min_value.empty()) {
              const auto v0 = extractFloatingPointNumber(
                  static_cast<std::string>(b.min_value));
              if (std::isfinite(v0)) {
                i.lower_bounds.insert({vn, v0});
              }
            }
            if (!b.max_value.empty()) {
              const auto v1 = extractFloatingPointNumber(
                  static_cast<std::string>(b.max_value));
              if (std::isfinite(v1)) {
                i.upper_bounds.insert({vn, v1});
              }
            }
          }
        }
      }
    } catch (std::exception& e) {
      raise(" Error while reading the MFrontImplementation '" + n +
            "': " + e.what());
    }
  }  // end of read

  static std::string getMFrontImplementationTypeSpecifier(
      const std::string& t) {
    if ((t == "material_property") || (t == "MaterialProperty")) {
      return "MaterialProperties";
    }
    if ((t == "behaviour") || (t == "Behaviour")) {
      return "Behaviours";
    }
    if (!((t == "model") || (t == "Model"))) {
      raise("getMFrontImplementation: invalid type '" + t + "'");
    }
    return "Models";
  }  // end of getMFrontImplementationTypeSpecifier

  MFrontImplementation getMFrontImplementation(const std::string& f,
                                               const std::string& t,
                                               const std::string& m,
                                               const std::string& i) {
    if (m.empty()) {
      return getMFrontImplementation(f, t, i);
    }
    const auto file = File(f, H5F_ACC_RDONLY);
    const auto type = getMFrontImplementationTypeSpecifier(t);
    const auto g = openGroup(file.getRoot(), "MFront/" + m + "/" + type);
    return read<MFrontImplementation>(g, i);
  }  // end of getMFrontImplementation

  MFrontImplementation getMFrontImplementation(const std::string& f,
                                               const std::string& t,
                                               const std::string& i) {
    const auto file = File(f, H5F_ACC_RDONLY);
    const auto type = getMFrontImplementationTypeSpecifier(t);
    const auto g = openGroup(file.getRoot(), "MFront/" + type);
    return read<MFrontImplementation>(g, i);
  }  // end of getMFrontImplementation

}  // end of namespace madnex
