/*!
 * \file   CompType.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   19/01/2017
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
 * \brief structure in charge of testing the CompType class
 */
struct CompType final
  : public tfel::tests::TestCase
{
  CompType()
    : tfel::tests::TestCase("Madnex","ReferenceFile")
  {} // end of CompType
  virtual tfel::tests::TestResult
  execute() override
  {
    using namespace madnex;
    const auto f = File(fileName,H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    const auto d = openDataSet(r,"version");
    H5::CompType c(d);
    TFEL_TESTS_ASSERT(c.getNmembers());
    TFEL_TESTS_ASSERT(c.getMemberName(0)=="MAJ");
    TFEL_TESTS_ASSERT(c.getMemberName(1)=="MIN");
    TFEL_TESTS_ASSERT(c.getMemberName(2)=="REL");
    return this->result;
  }
};

TFEL_TESTS_GENERATE_PROXY(CompType,"CompType");

int main(const int argc,const char* const * const argv)
{
  if(argc!=2){
    std::cerr << "CompType: invalid number of arguments\n"
	      << "usage: CompType reference.madnex\n";
    return EXIT_FAILURE;
  }
  fileName = argv[1];
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CompType.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}

