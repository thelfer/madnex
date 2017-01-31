/*!
 * \file   MadnexTestAPI.cxx
 * \brief    
 * \author Helfer Thomas 
 * \date   30 janv. 2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<cmath>
#include<cstdlib>
#include<limits>
#include<iostream>
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"Madnex/HDF5.hxx"
#include"Madnex/File.hxx"
#include"Madnex/Types.hxx"
#include"Madnex/MainResults.hxx"

std::string fileName;

/*!
 * \brief structure in charge of testing the `File` class.
 */
struct MadnexTestAPI final
  : public tfel::tests::TestCase
{
  MadnexTestAPI()
    : tfel::tests::TestCase("Madnex","API")
  {} // end of MadnexTestAPI
  virtual tfel::tests::TestResult
  execute() override
  {
    using namespace madnex;
    const auto f  = File(fileName,H5F_ACC_RDONLY);
    const auto r  = f.getRoot();
    const auto g  = openGroup(f.getRoot(),"resultats");
    // // main results
    auto mr = read<MainResults>(g,"resultats_principaux");
    TFEL_TESTS_ASSERT(mr.size()==3u);
    TFEL_TESTS_ASSERT(mr[0].name=="P1");
    TFEL_TESTS_ASSERT(mr[0].label=="param 1");
    TFEL_TESTS_ASSERT(mr[0].value=="12");
    TFEL_TESTS_ASSERT(mr[0].unit=="");
    TFEL_TESTS_ASSERT(mr[0].type=="int32");
    TFEL_TESTS_ASSERT(mr[1].name=="P2");
    TFEL_TESTS_ASSERT(mr[1].label=="param 2");
    TFEL_TESTS_ASSERT(mr[1].value=="0.75");
    TFEL_TESTS_ASSERT(mr[1].unit=="mm");
    TFEL_TESTS_ASSERT(mr[1].type=="float64");
    TFEL_TESTS_ASSERT(mr[2].name=="P3");
    TFEL_TESTS_ASSERT(mr[2].label=="param 3");
    TFEL_TESTS_ASSERT(mr[2].value=="test");
    TFEL_TESTS_ASSERT(mr[2].unit=="");
    TFEL_TESTS_ASSERT(mr[2].type=="XXS string");
    return this->result;
  }
};

TFEL_TESTS_GENERATE_PROXY(MadnexTestAPI,
			  "MadnexTestAPI");

int main(const int argc, const char* const* argv)
{
  if(argc!=2){
    std::cerr << "MadnexTestAPI: invalid number of arguments\n"
	      << "usage: MadnexTestAPI reference.madnex\n";
    return EXIT_FAILURE;
  }
  fileName = argv[1];
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MadnexTestAPI.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
