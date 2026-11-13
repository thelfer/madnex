/*!
 * \file   tests/MadnexMFrontMetadata.cxx
 * \brief  coverage tests for the MFront meta-data types: round-trips of
 * `MFrontMetaData`, `MTestTestMetaData`, `MFMTestGeneratorTestMetaData`,
 * `MTestTest`, `MFMTestGeneratorTest` and `MainResults`, file based
 * accessors and the associated error paths.
 * \author Thomas Helfer
 * \date   13/11/2026
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

#include "Madnex/Exceptions.hxx"
#include "Madnex/File.hxx"
#include "Madnex/HDF5.hxx"
#include "Madnex/MainResults.hxx"
#include "Madnex/MFrontMetaData.hxx"
#include "Madnex/MTestTest.hxx"
#include "Madnex/MFMTestGeneratorTest.hxx"

namespace {

  template <typename F>
  static bool throws(F f) {
    try {
      f();
      return false;
    } catch (const madnex::Exception&) {
      return true;
    }
  }  // end of throws

  static void check_metadata_round_trips() {
    using namespace madnex;
    auto f = File("MadnexMFrontMetadata.madnex", H5F_ACC_TRUNC);
    auto r = f.getRoot();
    //
    // `MFrontMetaData`
    {
      auto m = MFrontMetaData{};
      m.author = "Thomas Helfer";
      m.date = "13/11/2026";
      m.status = "stable";
      m.description = "a behaviour used for testing purposes";
      m.uuid = "256d09a1-1277-4b85-a2ad-a91d97846cda";
      m.unit_system = "IEEE";
      write(r, "mfront_metadata", m);
      auto m2 = MFrontMetaData{};
      read(m2, r, "mfront_metadata");
      assert(m2.author == m.author);
      assert(m2.date == m.date);
      assert(m2.status == m.status);
      assert(m2.description == m.description);
      assert(m2.uuid == m.uuid);
      assert(m2.unit_system == m.unit_system);
      // an empty unit system means: use the default unit system
      m.unit_system = {};
      write(r, "mfront_metadata", m, true);
      read(m2, r, "mfront_metadata");
      assert(m2.unit_system.empty());
      // files written by previous versions of the library do not have
      // the `unit_system` member: reading them shall not fail and the
      // default unit system is reported (i.e. an empty unit system)
      {
        struct LegacyMFrontMetaData {
          char author[32];
          char date[32];
          char status[32];
          char description[512];
          char uuid[36];
        };
        const auto c = H5::CompType{sizeof(LegacyMFrontMetaData)};
        c.insertMember("author", HOFFSET(LegacyMFrontMetaData, author),
                       StrType(0, 32));
        c.insertMember("date", HOFFSET(LegacyMFrontMetaData, date),
                       StrType(0, 32));
        c.insertMember("status", HOFFSET(LegacyMFrontMetaData, status),
                       StrType(0, 32));
        c.insertMember("description",
                       HOFFSET(LegacyMFrontMetaData, description),
                       StrType(0, 512));
        c.insertMember("uuid", HOFFSET(LegacyMFrontMetaData, uuid),
                       StrType(0, 36));
        auto legacy = LegacyMFrontMetaData{};
        std::strncpy(legacy.author, "legacy author", sizeof(legacy.author) - 1);
        std::strncpy(legacy.date, "yesterday", sizeof(legacy.date) - 1);
        std::strncpy(legacy.status, "testing", sizeof(legacy.status) - 1);
        std::strncpy(legacy.description, "written without a unit system",
                     sizeof(legacy.description) - 1);
        // the `uuid` member is stored on 36 bytes: a canonical uuid has
        // exactly 36 characters and is not null terminated in the file
        std::memcpy(legacy.uuid, "256d09a1-1277-4b85-a2ad-a91d97846cda", 36);
        const hsize_t dim[] = {1};
        const auto d = r.createDataSet("legacy_metadata", c, DataSpace(1, dim));
        d.write(&legacy, c);
        auto m3 = MFrontMetaData{};
        read(m3, r, "legacy_metadata");
        assert(m3.author == "legacy author");
        assert(m3.status == "testing");
        assert(m3.uuid == "256d09a1-1277-4b85-a2ad-a91d97846cda");
        assert(m3.unit_system.empty());
      }
    }
    //
    // `MTestTestMetaData`
    {
      auto m = MTestTestMetaData{};
      m.author = "author";
      m.date = "16/01/2017";
      m.description = "a mtest test";
      write(r, "mtest_metadata", m);
      auto m2 = MTestTestMetaData{};
      read(m2, r, "mtest_metadata");
      assert(m2.author == m.author);
      assert(m2.date == m.date);
      assert(m2.description == m.description);
    }
    //
    // `MFMTestGeneratorTestMetaData`
    {
      auto m = MFMTestGeneratorTestMetaData{};
      m.author = "author";
      m.date = "16/01/2017";
      m.description = "a mfm-test-generator test";
      write(r, "mfm_metadata", m);
      auto m2 = MFMTestGeneratorTestMetaData{};
      read(m2, r, "mfm_metadata");
      assert(m2.author == m.author);
      assert(m2.date == m.date);
      assert(m2.description == m.description);
    }
    //
    // `MTestTest`: with and without a scheme
    {
      auto t = MTestTest{};
      t.name = "test_with_scheme";
      t.scheme = "ptest";
      t.test = "@mtest ptest;\n@material Gurson;\n";
      t.metadata.author = "author";
      t.metadata.date = "today";
      t.metadata.description = "a test with a scheme";
      write(r, t);
      auto t2 = MTestTest{};
      read(t2, r, "test_with_scheme");
      assert(t2.name == "test_with_scheme");
      assert(t2.scheme == "ptest");
      assert(t2.test == t.test);
      assert(t2.metadata.author == "author");
      assert(t2.metadata.description == "a test with a scheme");
      //
      t.name = "test_without_scheme";
      t.scheme = {};
      write(r, t);
      read(t2, r, "test_without_scheme");
      assert(t2.scheme.empty());
      assert(t2.test == t.test);
    }
    //
    // `MFMTestGeneratorTest`
    {
      auto t = MFMTestGeneratorTest{};
      t.name = "mfm_test";
      t.test = "Gurson {\n  type = 'small_strains';\n};\n";
      t.metadata.author = "author";
      t.metadata.date = "today";
      t.metadata.description = "a mfm test";
      write(r, t);
      auto t2 = MFMTestGeneratorTest{};
      read(t2, r, "mfm_test");
      assert(t2.name == "mfm_test");
      assert(t2.test == t.test);
      assert(t2.metadata.description == "a mfm test");
    }
    //
    // `MainResults`
    {
      auto rs = MainResults{};
      auto r1 = MainResult{};
      r1.name = "FM";
      r1.label = "forces sur le maitre couple";
      r1.value = "1";
      r1.unit = "N";
      r1.type = "MATRIX_3X1";
      auto r2 = MainResult{};
      r2.name = "UVPM";
      r2.label = "deplacement du plateau superieur";
      r2.value = "2e-3";
      r2.unit = "m";
      r2.type = "VECTOR_3";
      rs.push_back(r1);
      rs.push_back(r2);
      write(r, "main_results", rs);
      // an existing data set is silently replaced
      write(r, "main_results", rs);
      write(r, "main_results", rs, true);
      auto rs2 = MainResults{};
      read(rs2, r, "main_results");
      assert(rs2.size() == 2u);
      assert(rs2[0].name == "FM");
      assert(rs2[0].label == "forces sur le maitre couple");
      assert(rs2[0].value == "1");
      assert(rs2[0].unit == "N");
      assert(rs2[0].type == "MATRIX_3X1");
      assert(rs2[1].name == "UVPM");
      assert(rs2[1].type == "VECTOR_3");
    }
  }  // end of check_metadata_round_trips

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

  static void check_file_based_accessors(const char* const fn) {
    using namespace madnex;
    {
      auto f = File(fn, H5F_ACC_TRUNC);
      auto r = f.getRoot();
      auto m = MTestTest{};
      m.name = "test_1";
      m.scheme = "mtest";
      m.test = "@material GenericSteel;\n";
      m.metadata.description = "material dependent test";
      auto g1 = Group{};
      createGroupPath(r,
                      "MFront/GenericSteel/Behaviours/Plasticity/"
                      "MTestTests");
      g1 = openGroup(r,
                     "MFront/GenericSteel/Behaviours/Plasticity/"
                     "MTestTests");
      write(g1, m);
      auto g2 = Group{};
      createGroupPath(r, "MFront/Behaviours/Elasticity/MTestTests");
      g2 = openGroup(r, "MFront/Behaviours/Elasticity/MTestTests");
      m.name = "test_2";
      m.scheme = {};
      m.test = "@material Elasticity;\n";
      write(g2, m);
      auto g3 = Group{};
      createGroupPath(r,
                      "MFront/GenericSteel/Behaviours/Plasticity/"
                      "MFMTestGeneratorTests");
      g3 = openGroup(r,
                     "MFront/GenericSteel/Behaviours/Plasticity/"
                     "MFMTestGeneratorTests");
      auto t = MFMTestGeneratorTest{};
      t.name = "mfm_test_1";
      t.test = "Plasticity {};";
      t.metadata.description = "a mfm test";
      write(g3, t);
      auto g4 = Group{};
      createGroupPath(r, "MFront/Behaviours/Elasticity/MFMTestGeneratorTests");
      g4 = openGroup(r, "MFront/Behaviours/Elasticity/MFMTestGeneratorTests");
      t.name = "mfm_test_2";
      write(g4, t);
    }
    // material dependent `MTest` tests
    {
      const auto t = getMTestTest(fn, "GenericSteel", "Plasticity", "test_1");
      assert(t.name == "test_1");
      assert(t.scheme == "mtest");
      assert(t.metadata.description == "material dependent test");
      assert(getMTestTestScheme(fn, "GenericSteel", "Plasticity", "test_1") ==
             "mtest");
    }
    // generic `MTest` tests: an empty material name falls back on the
    // generic behaviours
    {
      const auto t = getMTestTest(fn, "", "Elasticity", "test_2");
      assert(t.name == "test_2");
      assert(t.scheme.empty());
      assert(getMTestTestScheme(fn, "Elasticity", "test_2").empty());
    }
    // `MFMTestGenerator` tests
    {
      const auto t = getMFMTestGeneratorTest(fn, "GenericSteel", "Plasticity",
                                             "mfm_test_1");
      assert(t.name == "mfm_test_1");
      assert(t.metadata.description == "a mfm test");
      const auto t2 = getMFMTestGeneratorTest(fn, "Elasticity", "mfm_test_2");
      assert(t2.name == "mfm_test_2");
    }
    // unknown tests or behaviours are reported as errors of the library
    assert(throws([&fn]() {
      getMTestTest(fn, "GenericSteel", "Plasticity", "unknown");
    }));
    assert(throws(
        [&fn]() { getMTestTest(fn, "Unknown", "Plasticity", "test_1"); }));
    assert(throws(
        [&fn]() { getMFMTestGeneratorTest(fn, "Elasticity", "unknown"); }));
  }  // end of check_file_based_accessors

  static void check_write_error_paths() {
    using namespace madnex;
    auto f = File("MadnexMFrontMetadataErrors.madnex", H5F_ACC_TRUNC);
    auto r = f.getRoot();
    // a `MTest` test without a name, without a test or with an invalid
    // scheme is rejected
    assert(throws([&r]() {
      auto t = MTestTest{};
      t.test = "@material Elasticity;";
      write(r, t);
    }));
    assert(throws([&r]() {
      auto t = MTestTest{};
      t.name = "test";
      write(r, t);
    }));
    assert(throws([&r]() {
      auto t = MTestTest{};
      t.name = "test";
      t.scheme = "invalid_scheme";
      t.test = "content";
      write(r, t);
    }));
    // same thing for a `MFMTestGenerator` test
    assert(throws([&r]() {
      auto t = MFMTestGeneratorTest{};
      t.test = "Elasticity {};";
      write(r, t);
    }));
    assert(throws([&r]() {
      auto t = MFMTestGeneratorTest{};
      t.name = "test";
      write(r, t);
    }));
  }  // end of check_write_error_paths

}  // end of anonymous namespace

int main(int argc, char* argv[]) {
  if (argc != 1) {
    std::cerr << "usage: " << argv[0] << "\n";
    return EXIT_FAILURE;
  }
  try {
    check_metadata_round_trips();
    check_file_based_accessors("MadnexMFrontMetadataAccessors.madnex");
    check_write_error_paths();
  } catch (const std::exception& e) {
    std::cerr << "MadnexMFrontMetadata: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
