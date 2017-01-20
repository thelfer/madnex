/*!
 * \file   MadnexReferenceFile.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   17/01/2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<string>
#include<cstdlib>
#include<iterator>
#include<algorithm>
#include<iostream>
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"Madnex/HDF5.hxx"
#include"Madnex/File.hxx"
#include"Madnex/Version.hxx"
#include"Madnex/ExperimentalConditions.hxx"

std::string fileName;

/*!
 * \brief structure in charge of testing read/write on plain old data
 * types in HDF5 files
 */
struct MadnexReferenceFile final
  : public tfel::tests::TestCase
{
  MadnexReferenceFile()
    : tfel::tests::TestCase("Madnex","ReferenceFile")
  {} // end of MadnexReferenceFile
  virtual tfel::tests::TestResult
  execute() override
  {
    using namespace madnex;
    auto in = [](const std::vector<std::string>& v,
		 const std::string& n){
      return std::find(v.begin(),v.end(),n)!=v.end();
    };
    const auto f = File(fileName,H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    const auto gn = getSubGroupNames(r,true);
    const auto ga = getAttributeNames(r);
    TFEL_TESTS_ASSERT(gn.size()==5);
    TFEL_TESTS_ASSERT(in(gn,"conditions"));
    TFEL_TESTS_ASSERT(in(gn,"historique"));
    TFEL_TESTS_ASSERT(in(gn,"matiere"));
    TFEL_TESTS_ASSERT(in(gn,"prestation"));
    TFEL_TESTS_ASSERT(in(gn,"resultats"));
    TFEL_TESTS_ASSERT(in(ga,"ref_essai"));
    TFEL_TESTS_ASSERT(in(ga,"type_essai"));
    TFEL_TESTS_ASSERT(in(ga,"accessibilité"));
    TFEL_TESTS_ASSERT(in(ga,"droit_ecriture"));
    // TFEL_TESTS_ASSERT(getAttributeValue<std::string>(r,"ref_essai").empty());
    // TFEL_TESTS_ASSERT(getAttributeValue<std::string>(r,"type_essai").empty());
    // TFEL_TESTS_ASSERT(getAttributeValue<std::string>(r,"accessibilité").empty());
    TFEL_TESTS_ASSERT(getAttributeValue<int>(r,"droit_ecriture")==1);
    const auto v = read<Version>(r,"version");
    TFEL_TESTS_ASSERT(v.major.empty());
    // TFEL_TESTS_ASSERT(v.minor.empty());
    // TFEL_TESTS_ASSERT(v.revision.empty());
    return this->result;
  }
};

TFEL_TESTS_GENERATE_PROXY(MadnexReferenceFile,"MadnexReferenceFile");

int main(const int argc,const char* const * const argv)
{
  if(argc!=2){
    std::cerr << "MadnexReferenceFile: invalid number of arguments\n"
	      << "usage: MadnexReferenceFile reference.madnex\n";
    return EXIT_FAILURE;
  }
  fileName = argv[1];
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MadnexReferenceFile.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
