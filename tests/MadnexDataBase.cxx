/*!
 * \file   tests/MadnexDataBase.cxx
 * \brief  coverage tests for the `madnex::DataBase` query api: material,
 * material property, behaviour and model lists, `MTest` and
 * `MFMTestGenerator` test lists, regular expression based queries and the
 * associated error paths.
 * \author Thomas Helfer
 * \date   13/11/2026
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Madnex/Exceptions.hxx"
#include "Madnex/File.hxx"
#include "Madnex/DataBase.hxx"
#include "Madnex/HDF5.hxx"

namespace {

  //
  //  Data base used by the tests:
  //
  //  MFront/
  //    MaterialProperties/  Elasticity
  //    Behaviours/          Elasticity (with MTestTests/test_g1 and
  //                                    MFMTestGeneratorTests/test_g2)
  //    GenericSteel/
  //      MaterialProperties/  IsElastic; MassDensity
  //      Behaviours/          Plasticity (with MTestTests/test_plastic)
  //      Models/              model_a
  //    Concrete/
  //      Behaviours/          MultiaxialCyclicPlasticity
  //
  // `madnex::createGroup` does not create the intermediate groups: this
  // helper creates the whole path, one component at a time
  static void createGroupPath(H5::Group& r, const std::string& p) {
    using namespace madnex;
    auto n = std::string::size_type{};
    auto g = r;
    while (true) {
      const auto e = p.find('/', n);
      if (e == std::string::npos) {
        createGroup(g, p.substr(n));
        return;
      }
      g = createGroup(g, p.substr(n, e - n));
      n = e + 1;
    }
  }  // end of createGroupPath

  static void write_database_file(const char* const fn) {
    using namespace madnex;
    auto f = File(fn, H5F_ACC_TRUNC);
    auto r = f.getRoot();
    createGroupPath(r, "MFront/MaterialProperties/Elasticity");
    createGroupPath(r, "MFront/Behaviours/Elasticity/MTestTests/test_g1");
    createGroupPath(r,
                    "MFront/Behaviours/Elasticity/MFMTestGeneratorTests/"
                    "test_g2");
    createGroupPath(r, "MFront/GenericSteel/MaterialProperties/IsElastic");
    createGroupPath(r, "MFront/GenericSteel/MaterialProperties/MassDensity");
    createGroupPath(r,
                    "MFront/GenericSteel/Behaviours/Plasticity/MTestTests/"
                    "test_plastic");
    createGroupPath(r, "MFront/GenericSteel/Models/model_a");
    createGroupPath(r, "MFront/Concrete/Behaviours/MultiaxialCyclicPlasticity");
  }  // end of write_database_file

  static void write_empty_file(const char* const fn) {
    using namespace madnex;
    auto f = File(fn, H5F_ACC_TRUNC);
    auto r = f.getRoot();
    // a data set, not a group: `MFront` exists but is not a sub group
    write(r, "not_a_group", static_cast<std::int32_t>(1));
  }  // end of write_empty_file

  static bool contains(const std::vector<std::string>& v,
                       const std::string& n) {
    return std::find(v.begin(), v.end(), n) != v.end();
  }  // end of contains

  template <typename F>
  static bool throws(F f) {
    try {
      f();
      return false;
    } catch (const madnex::Exception&) {
      return true;
    }
  }  // end of throws

  static void check_queries(const char* const fn) {
    using namespace madnex;
    const auto d = DataBase(fn);
    //
    // materials: the `MaterialProperties`, `Behaviours` and `Models`
    // generic groups are not materials
    {
      const auto materials = d.getMaterialsList();
      assert(materials.size() == 2u);
      assert(contains(materials, "GenericSteel"));
      assert(contains(materials, "Concrete"));
      assert(!contains(materials, "MaterialProperties"));
      assert(!contains(materials, "Behaviours"));
    }
    //
    // material properties
    {
      const auto mmps = d.getAvailableMaterialProperties();
      assert(mmps.size() == 2u);
      // the generic material properties are associated with an empty name
      assert(mmps.at("").size() == 1u);
      assert(mmps.at("").front() == "Elasticity");
      assert(mmps.at("GenericSteel").size() == 2u);
      // `Concrete` has no material property: it is not present in the map
      assert(mmps.find("Concrete") == mmps.end());
      //
      const auto mps = d.getAvailableMaterialProperties("GenericSteel");
      assert(mps.size() == 2u);
      assert(contains(mps, "IsElastic"));
      assert(contains(mps, "MassDensity"));
      assert(d.getAvailableMaterialProperties("Concrete").empty());
    }
    //
    // behaviours
    {
      const auto mbs = d.getAvailableBehaviours();
      assert(mbs.size() == 3u);
      assert(mbs.at("").size() == 1u);
      assert(mbs.at("").front() == "Elasticity");
      assert(mbs.at("GenericSteel").size() == 1u);
      assert(mbs.at("GenericSteel").front() == "Plasticity");
      assert(mbs.at("Concrete").size() == 1u);
      assert(mbs.at("Concrete").front() == "MultiaxialCyclicPlasticity");
      //
      const auto bs = d.getAvailableBehaviours("GenericSteel");
      assert(bs.size() == 1u);
      assert(bs.front() == "Plasticity");
    }
    //
    // models
    {
      const auto mms = d.getAvailableModels();
      // only `GenericSteel` declares models: neither the generic scope nor
      // `Concrete` are present in the map
      assert(mms.size() == 1u);
      assert(mms.at("GenericSteel").size() == 1u);
      assert(mms.at("GenericSteel").front() == "model_a");
      //
      assert(d.getAvailableModels("GenericSteel").size() == 1u);
      assert(d.getAvailableModels("Concrete").empty());
    }
    //
    // MTest tests
    {
      const auto ts = d.getAvailableMTestTests("GenericSteel", "Plasticity");
      assert(ts.size() == 1u);
      assert(ts.front() == "test_plastic");
      // a behaviour without any `MTestTests` sub group
      assert(d.getAvailableMTestTests("Concrete", "MultiaxialCyclicPlasticity")
                 .empty());
      // generic behaviours
      const auto gts = d.getAvailableMTestTests("Elasticity");
      assert(gts.size() == 1u);
      assert(gts.front() == "test_g1");
    }
    //
    // MFMTestGenerator tests
    {
      const auto ts =
          d.getAvailableMFMTestGeneratorTests("GenericSteel", "Plasticity");
      assert(ts.empty());
      const auto gts = d.getAvailableMFMTestGeneratorTests("Elasticity");
      assert(gts.size() == 1u);
      assert(gts.front() == "test_g2");
    }
  }  // end of check_queries

  static void check_matching_queries(const char* const fn) {
    using namespace madnex;
    const auto d = DataBase(fn);
    //
    // the empty string, standing for the generic scope, is a candidate
    {
      const auto ms = getMatchingMaterials(d, ".*");
      assert(ms.size() == 3u);
      assert(contains(ms, ""));
      assert(contains(ms, "GenericSteel"));
      assert(contains(ms, "Concrete"));
    }
    {
      const auto ms = getMatchingMaterials(d, "Generic.*");
      assert(ms.size() == 1u);
      assert(ms.front() == "GenericSteel");
      assert(getMatchingMaterials(d, "Nothing").empty());
    }
    //
    {
      const auto mbs = getMatchingBehaviours(d, ".*", ".*Plasticity");
      assert(mbs.size() == 2u);
      assert(mbs.at("GenericSteel").size() == 1u);
      assert(mbs.at("Concrete").size() == 1u);
    }
    {
      const auto mps = getMatchingMaterialProperties(d, ".*", "Mass.*");
      assert(mps.size() == 1u);
      assert(mps.at("GenericSteel").size() == 1u);
      assert(mps.at("GenericSteel").front() == "MassDensity");
    }
    {
      const auto mms = getMatchingModels(d, ".*", "model_.*");
      assert(mms.size() == 1u);
      assert(mms.at("GenericSteel").front() == "model_a");
    }
  }  // end of check_matching_queries

  static void check_error_paths(const char* const fn,
                                const char* const empty_fn) {
    using namespace madnex;
    const auto d = DataBase(fn);
    // unknown materials, behaviours and sub groups are reported as errors
    // of the library, whatever the entry point
    assert(throws([&d]() { d.getAvailableMaterialProperties("Unknown"); }));
    assert(throws([&d]() { d.getAvailableBehaviours("Unknown"); }));
    assert(throws([&d]() { d.getAvailableModels("Unknown"); }));
    assert(
        throws([&d]() { d.getAvailableMTestTests("Unknown", "Plasticity"); }));
    assert(throws(
        [&d]() { d.getAvailableMTestTests("GenericSteel", "Unknown"); }));
    assert(throws([&d]() {
      d.getAvailableMFMTestGeneratorTests("GenericSteel", "Unknown");
    }));
    assert(throws([&d]() { d.getAvailableMTestTests("Unknown"); }));
    assert(throws([&d]() { d.getAvailableMFMTestGeneratorTests("Unknown"); }));
    // the error message of `checkIfMaterialExists` lists the available
    // materials
    try {
      d.getAvailableBehaviours("Unknown");
      assert(false);
    } catch (const madnex::Exception& e) {
      const auto msg = std::string{e.what()};
      assert(msg.find("Unknown") != std::string::npos);
      assert(msg.find("GenericSteel") != std::string::npos);
      assert(msg.find("Concrete") != std::string::npos);
    }
    // an invalid regular expression is translated into an error of the
    // library (it must not escape as a `std::regex_error`)
    assert(throws([&d]() { getMatchingMaterials(d, "GenericSteel("); }));
    assert(throws([&d]() { getMatchingBehaviours(d, ".*", "Plasticity["); }));
    // a file without any `MFront` group: the queries which do not require
    // a material return an empty result, the others raise an error
    const auto e = DataBase(empty_fn);
    assert(e.getMaterialsList().empty());
    assert(e.getAvailableBehaviours().empty());
    assert(e.getAvailableModels().empty());
    assert(e.getAvailableMTestTests("Elasticity").empty());
    assert(e.getAvailableMFMTestGeneratorTests("Elasticity").empty());
    assert(throws([&e]() { e.getAvailableBehaviours("Unknown"); }));
  }  // end of check_error_paths

}  // end of anonymous namespace

int main(int argc, char* argv[]) {
  if (argc != 1) {
    std::cerr << "usage: " << argv[0] << "\n";
    return EXIT_FAILURE;
  }
  try {
    write_database_file("MadnexDataBase.madnex");
    write_empty_file("MadnexDataBaseEmpty.madnex");
    check_queries("MadnexDataBase.madnex");
    check_matching_queries("MadnexDataBase.madnex");
    check_error_paths("MadnexDataBase.madnex", "MadnexDataBaseEmpty.madnex");
  } catch (const std::exception& e) {
    std::cerr << "MadnexDataBase: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
