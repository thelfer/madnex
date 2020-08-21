/*!
 * \file   MadnexReferenceFile.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   17/01/2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <string>
#include <cstdlib>
#include <cassert>
#include <iterator>
#include <iostream>
#include <algorithm>

#include "Madnex/HDF5.hxx"
#include "Madnex/File.hxx"
#include "Madnex/Version.hxx"
#include "Madnex/ExperimentalConditions.hxx"

int main(const int argc, const char* const* const argv) {
  using namespace madnex;
  auto in = [](const std::vector<std::string>& v, const std::string& n) {
    return std::find(v.begin(), v.end(), n) != v.end();
  };
  if (argc != 2) {
    std::cerr << "MadnexReferenceFile: invalid number of arguments\n"
              << "usage: MadnexReferenceFile reference.madnex\n";
    return EXIT_FAILURE;
  }
  const auto f = File(argv[1], H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  const auto gn = getSubGroupNames(r, true);
  const auto ga = getAttributeNames(r);
  assert(gn.size() == 5);
  assert(in(gn, "conditions"));
  assert(in(gn, "historique"));
  assert(in(gn, "matiere"));
  assert(in(gn, "prestation"));
  assert(in(gn, "resultats"));
  assert(in(ga, "ref_essai"));
  assert(in(ga, "type_essai"));
  assert(in(ga, "accessibilité"));
  assert(in(ga, "droit_ecriture"));
  // assert(getAttributeValue<std::string>(r,"ref_essai").empty());
  // assert(getAttributeValue<std::string>(r,"type_essai").empty());
  // assert(getAttributeValue<std::string>(r,"accessibilité").empty());
  assert(getAttributeValue<int>(r, "droit_ecriture") == 1);
  const auto v = read<Version>(r, "version");
  assert(v.major == "1");
  assert(v.minor == "2");
  assert(v.revision == "1");
  return EXIT_SUCCESS;
}
