/*!
 * \file   src/MFrontImplementation.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27/08/2020
 */

#include <set>
#include <cmath>
#include <limits>
#include <clocale>
#include <iostream>
#include <algorithm>
#include "Madnex/Raise.hxx"
#include "Madnex/File.hxx"
#include "Madnex/MFrontParameters.hxx"
#include "Madnex/MFrontImplementation.hxx"

namespace madnex {

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
      auto parameters = MFrontParameters{};
#pragma "warning HERE"
      for (const auto& p : i.parameters) {
        MFrontParameter parameter;
        std::copy(p.first.begin(), p.first.end(), parameter.name.begin());
        std::ostringstream value_str;
        value_str << p.second;
        const auto tmp = value_str.str();
        std::copy(tmp.begin(), tmp.end(), parameter.value.begin());
        parameters.push_back(parameter);
      }
      write(impl, "parameters", parameters);
    }
    if ((!i.lower_bounds.empty()) || (!i.upper_bounds.empty())) {
      auto bounds = createGroup(impl, "bounds");
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
      auto bvalues = std::vector<double>{2};
      for (const auto& n : names) {
        bvalues = {std::numeric_limits<double>::quiet_NaN(),
                   std::numeric_limits<double>::quiet_NaN()};
        auto plb = i.lower_bounds.find(n);
        if (plb != i.lower_bounds.end()) {
          bvalues[0] = plb->second;
        }
        auto pub = i.upper_bounds.find(n);
        if (pub != i.upper_bounds.end()) {
          bvalues[1] = pub->second;
        }
        write(bounds, n, bvalues);
      }
    }
  }  // end of write

  double convert_to_double(const std::string& s) {
    const std::string oldLocale = std::setlocale(LC_NUMERIC, nullptr);
    std::setlocale(LC_NUMERIC, "C");
    const auto v = std::stod(s);
    std::setlocale(LC_NUMERIC, oldLocale.c_str());
    return v;
  }

  void read(MFrontImplementation& i, const Group& g, const std::string& n) {
    auto impl = openGroup(g, n);
    i.name = n;
    read(i.source, impl, "source");
    if (exists(impl, "metadata")) {
      read_if(i.metadata, impl, "metadata");
    }
    if (exists(impl, "parameters")) {
      auto parameters = MFrontParameters{};
      read(parameters, impl, "parameters");
      for (const auto& p : parameters) {
        const auto name = std::string{p.name.begin(), p.name.end()};
        const auto value = std::string{p.value.begin(), p.value.end()};
        i.parameters.insert({name, convert_to_double(value)});
      }
    }
    // if (exists(impl, "bounds")) {
    //   auto bounds = openGroup(impl, "bounds");
    //   std::vector<double> bvalues;
    //   for (const auto& b : getDataSetNames(bounds)) {
    //     read(bvalues, bounds, b);
    //     if (bvalues.size() != 2u) {
    //       raise(
    //           "read: invalid bounds description "
    //           "while reading the MFrontImplementation '" +
    //           n + "'");
    //     }
    //     if (!std::isnan(bvalues[0])) {
    //       i.lower_bounds.insert(std::make_pair(b, bvalues[0]));
    //     }
    //     if (!std::isnan(bvalues[1])) {
    //       i.upper_bounds.insert(std::make_pair(b, bvalues[1]));
    //     }
    //   }
    //     }
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
